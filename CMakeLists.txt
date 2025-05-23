
cmake_minimum_required(VERSION 3.30)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0167 NEW)

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

project(elykseer-crypto CXX)

# Add options for enabling OpenSSL or Crypto++
option(USE_OPENSSL "Enable OpenSSL support" ON)
option(USE_CRYPTOPP "Enable Crypto++ support" OFF)

# Print the selected option
if(USE_CRYPTOPP)
    message(STATUS "Crypto++ support enabled.")
    set(USE_OPENSSL OFF)
elseif(USE_OPENSSL)
    message(STATUS "OpenSSL support enabled.")
    set(USE_CRYPTOPP OFF)
else()
    message(STATUS "Neither OpenSSL nor Crypto++ support enabled.")
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 2)
set (${PROJECT_NAME}_VERSION_MINOR 1)
set (${PROJECT_NAME}_VERSION_PATCH 1)
set (${PROJECT_NAME}_VERSION_STRING ${elykseer-crypto_VERSION_MAJOR}.${elykseer-crypto_VERSION_MINOR}.${elykseer-crypto_VERSION_PATCH})

option(RUN_ADDRESS_SANITIZER "using Clang you can turn on address sanitizer in Debug")

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions( -DDEBUG )
  set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
ELSE()
  add_definitions( -UDEBUG )
ENDIF()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)
find_package(ZLIB REQUIRED)
#find_package(OpenMP REQUIRED)

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
  set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")
  find_program (SELECTED_AR NAMES ar PATHS /opt/homebrew/opt/binutils/bin /opt/homebrew/ NO_DEFAULT_PATH REQUIRED)
  message(STATUS "Selected: ar in " ${SELECTED_AR})
  set(LIB_PREFIX "lib")
  set(libs "dl" ${ZLIB_LIBRARIES})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
      set(PROCESSOR_TYPE "x86_64")
  ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "AMD64")
  set(LIB_PREFIX "lib")
  set(libs ${ZLIB_LIBRARIES})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
  set(SELECTED_AR "/usr/local/bin/ar")
ENDIF(${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  set(LIB_PREFIX "lib")
  set(libs "dl" ${ZLIB_LIBRARIES})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LIB_PREFIX "lib")
  set(libs "dl" ${ZLIB_LIBRARIES} zstd)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_CROSSCOMPILING})
    set(CROSS_PREFIX "x")
ELSE()
    set(CROSS_PREFIX "")
ENDIF()
set(EXT_PATH ${CROSS_PREFIX}${CMAKE_SYSTEM_NAME}_${PROCESSOR_TYPE})

# other external tools
include_directories(ext)
include_directories(ext/${EXT_PATH}/include)
link_directories("ext/${EXT_PATH}/lib")

# cryptography
IF(USE_CRYPTOPP)
  add_definitions( -DCRYPTOPP )
  add_definitions( -DCRYPTOLIB=CRYPTOPP )
  add_library(libcryptopp_s STATIC IMPORTED)
  set(CRYPTO_CRYPTOPP_LIBPATH ${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH}/lib/libcryptopp.a)
  SET_TARGET_PROPERTIES(libcryptopp_s PROPERTIES IMPORTED_LOCATION ${CRYPTO_CRYPTOPP_LIBPATH})
  set(CRYPTO_LIBS "libcryptopp_s")
  set(CRYPTO_LIBPATHS ${CRYPTO_CRYPTOPP_LIBPATH})
ENDIF(USE_CRYPTOPP)

IF(USE_OPENSSL)
  add_definitions( -DOPENSSL )
  add_definitions( -DCRYPTOLIB=OPENSSL )
  set(OPENSSL_ROOT_DIR ${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH})
  find_package(OpenSSL REQUIRED)
  include_directories(${OPENSSL_INCLUDE_DIR})
  add_library(libopensslcrypto_s STATIC IMPORTED)
  set(CRYPTO_OPENSSL_CRYPTO_LIBPATH ${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH}/lib/libcrypto.a)
  SET_TARGET_PROPERTIES(libopensslcrypto_s PROPERTIES IMPORTED_LOCATION ${CRYPTO_OPENSSL_CRYPTO_LIBPATH})
  add_library(libopensslssl_s STATIC IMPORTED)
  set(CRYPTO_OPENSSL_SSL_LIBPATH ${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH}/lib/libssl.a)
  SET_TARGET_PROPERTIES(libopensslssl_s PROPERTIES IMPORTED_LOCATION ${CRYPTO_OPENSSL_SSL_LIBPATH})
  set(CRYPTO_LIBS "libopensslcrypto_s" "libopensslssl_s")
  set(CRYPTO_LIBPATHS ${CRYPTO_OPENSSL_SSL_LIBPATH} ${CRYPTO_OPENSSL_CRYPTO_LIBPATH})
ENDIF(USE_OPENSSL)

# GPGme
#pkg_check_modules(GPGME REQUIRED gpgme)
#pkg_check_modules(ASSUAN REQUIRED libassuan)
#pkg_check_modules(GPGerror REQUIRED gpg-error)
add_library(libgpgme_s STATIC IMPORTED)
add_library(libassuan_s STATIC IMPORTED)
add_library(libgpg-error_s STATIC IMPORTED)
SET_TARGET_PROPERTIES(libgpgme_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH}/lib/libgpgme.a")
SET_TARGET_PROPERTIES(libassuan_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH}/lib/libassuan.a")
SET_TARGET_PROPERTIES(libgpg-error_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/ext/${EXT_PATH}/lib/libgpg-error.a")
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


add_subdirectory( src/cpp )

if(NOT CMAKE_CROSSCOMPILING)
enable_testing()
add_subdirectory( test/cpp )
endif()
