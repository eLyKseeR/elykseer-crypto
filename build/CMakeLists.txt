
cmake_minimum_required(VERSION 3.5)

IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING 
        "Choose the type of build, options are: 
        None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) 
        Debug Release RelWithDebInfo MinSizeRel." )
ELSE()
   SET(CMAKE_BUILD_TYPE Debug CACHE STRING 
        "Choose the type of build, options are: 
        None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) 
        Debug Release RelWithDebInfo MinSizeRel." )
ENDIF()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(elykseer-crypto CXX)

# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (${PROJECT_NAME}_VERSION_PATCH 9)
set (${PROJECT_NAME}_VERSION_STRING ${elykseer-crypto_VERSION_MAJOR}.${elykseer-crypto_VERSION_MINOR}.${elykseer-crypto_VERSION_PATCH})

# choose either one
set(USE_OPENSSL NO)
set(USE_CRYPTOPP YES)

option(RUN_ADDRESS_SANITIZER "using Clang you can turn on address sanitizer in Debug")

# set to '-DDEBUG' to turn on debugging output
# set to '-UDEBUG' to disable debugging output
add_definitions( -UDEBUG )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig)

if(NOT CMAKE_CROSSCOMPILING)
find_program(CPPCHECK NAMES cppcheck)
endif()

IF(CPPCHECK)
    message(STATUS "Found: cppcheck")
    add_custom_target(cppcheck
        ${CPPCHECK}
        -Uassert --std=c++20 --enable=all
        ${CMAKE_SOURCE_DIR}/src/lxr/*.hpp
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/test/*.cpp
        -I ${CMAKE_SOURCE_DIR}/../ext
        -I /opt/homebrew/include
    )
ELSE()
    message(STATUS "Cppcheck - not found")
    message(STATUS "  Build target 'cppcheck' not available.")
ENDIF()

set(PROCESSOR_TYPE ${CMAKE_SYSTEM_PROCESSOR})
set(SELECTED_AR ${CMAKE_AR})
set(LIB_PREFIX "")

# OS dependent
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "13.6")
  find_program (SELECTED_AR NAMES ar PATHS /opt/homebrew/opt/binutils/bin /opt/homebrew/ NO_DEFAULT_PATH REQUIRED)
  message(STATUS "Selected: ar in " ${SELECTED_AR})
  set(LIB_PREFIX "lib")
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
      set(PROCESSOR_TYPE "x86_64")
  ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
  set(LIB_PREFIX "lib")
  set(libs "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
  set(SELECTED_AR "/usr/local/bin/ar")
ENDIF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  set(LIB_PREFIX "lib")
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LIB_PREFIX "lib")
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_CROSSCOMPILING})
    set(CROSS_PREFIX "x")
ELSE()
    set(CROSS_PREFIX "")
ENDIF()
set(EXT_PATH ${CROSS_PREFIX}${CMAKE_SYSTEM_NAME}_${PROCESSOR_TYPE})

# other external tools
include_directories(../ext/)
include_directories(../ext/${EXT_PATH}/include)
link_directories("../ext/${EXT_PATH}/lib")

IF(${CMAKE_CROSSCOMPILING})
    set(HTML2TEXT ${CMAKE_HOME_DIRECTORY}/../ext/${CMAKE_HOST_SYSTEM_NAME}_${CMAKE_HOST_SYSTEM_PROCESSOR}/bin/html2text)
    message(STATUS "setting html2text to host version. " ${HTML2TEXT})
    set(ENV{HTML2TEXT} ${HTML2TEXT})
ELSE()
    find_program (HTML2TEXT NAMES html2text html2text.exe PATHS ../ext/${EXT_PATH}/bin NO_DEFAULT_PATH REQUIRED)
    IF(HTML2TEXT)
        message(STATUS "Found: html2text in " ${HTML2TEXT})
        set(ENV{HTML2TEXT} ${HTML2TEXT})
    ENDIF()
ENDIF()

# cryptography
IF(USE_CRYPTOPP)
  add_definitions( -DCRYPTOPP )
  add_definitions( -DCRYPTOLIB=CRYPTOPP )
  add_library(libcryptopp_s STATIC IMPORTED)
  SET_TARGET_PROPERTIES(libcryptopp_s PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/../ext/${EXT_PATH}/lib/libcryptopp.a)
  set(CRYPTO_LIBS "libcryptopp_s")
ENDIF(USE_CRYPTOPP)

IF(USE_OPENSSL)
  add_definitions( -DOPENSSL )
  add_definitions( -DCRYPTOLIB=OPENSSL )
  IF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl")
ENDIF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
  find_package(OpenSSL REQUIRED)
  include_directories(${OPENSSL_INCLUDE_DIR})
  set(CRYPTO_LIBS ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
ENDIF(USE_OPENSSL)

# GPGme
#pkg_check_modules(GPGME REQUIRED gpgme)
#pkg_check_modules(ASSUAN REQUIRED libassuan)
#pkg_check_modules(GPGerror REQUIRED gpg-error)
add_library(libgpgme_s STATIC IMPORTED)
add_library(libassuan_s STATIC IMPORTED)
add_library(libgpg-error_s STATIC IMPORTED)
SET_TARGET_PROPERTIES(libgpgme_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/../ext/${EXT_PATH}/lib/libgpgme.a")
SET_TARGET_PROPERTIES(libassuan_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/../ext/${EXT_PATH}/lib/libassuan.a")
SET_TARGET_PROPERTIES(libgpg-error_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/../ext/${EXT_PATH}/lib/libgpg-error.a")
set(GPG_LIBS "libgpgme_s" "libassuan_s" "libgpg-error_s")

# address sanitizer
IF(RUN_ADDRESS_SANITIZER)
  IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	  IF(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang")
		  message("enabling address sanitizer on compiler ${CMAKE_CXX_COMPILER_ID}")
		  add_compile_options(-fsanitize=address)
		  add_link_options(-fsanitize=address)
	  ENDIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
ENDIF(RUN_ADDRESS_SANITIZER)


add_subdirectory( src )

if(NOT CMAKE_CROSSCOMPILING)
enable_testing()
add_subdirectory( test )
endif()

