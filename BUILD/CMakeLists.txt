# call: cmake .
# on Linux/OSX to build the Makefiles
#
# on OSX: cmake -G Xcode .
# to build the Xcode project files

cmake_minimum_required(VERSION 3.5)

IF(DEFINED CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release DebugProfile RelWithDebInfo MinSizeRel.")
ELSE()
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release DebugProfile RelWithDebInfo MinSizeRel.")
ENDIF()

project(elykseer-crypto LANGUAGES CXX HOMEPAGE_URL "https://github.com/eLyKseeR/elykseer-crypto")

message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM: ${CMAKE_SYSTEM}")

# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (${PROJECT_NAME}_VERSION_PATCH 8)
set (${PROJECT_NAME}_VERSION_STRING ${elykseer-crypto_VERSION_MAJOR}.${elykseer-crypto_VERSION_MINOR}.${elykseer-crypto_VERSION_PATCH})

# choose either one
set(USE_OPENSSL NO)
set(USE_CRYPTOPP YES)

option(RUN_ADDRESS_SANITIZER "using Clang you can turn on address sanitizer in Debug")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LINKER_LANGUAGE "CXX")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig)

find_program(CPPCHECK NAMES cppcheck)

IF(CPPCHECK)
    message(STATUS "Found: cppcheck")
    add_custom_target(cppcheck
        ${CPPCHECK}
        -Uassert --std=c++17 --enable=all
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

set(PLATFORM "unk")
set(SELECTED_AR ${CMAKE_AR})
set(LIB_PREFIX "")

# OS dependent
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(PLATFORM "darwin")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0")
  include_directories(/usr/local/opt/zlib/include)
  link_directories(/usr/local/opt/zlib/lib)
  include_directories(/opt/homebrew/opt/zlib/include)
  link_directories(/opt/homebrew/opt/zlib/lib)
  set(SELECTED_AR "/opt/homebrew/opt/binutils/bin/ar")
  set(LIB_PREFIX "lib")
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(PLATFORM "win64")
  set(LIB_PREFIX "lib")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  set(PLATFORM "freebsd")
  set(SELECTED_AR "/usr/local/bin/ar")
  set(LIB_PREFIX "lib")
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(PLATFORM "linux")
  set(LIB_PREFIX "lib")
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


# other external tools
include_directories(../ext)
link_directories("../ext/${PLATFORM}/lib")
include_directories("../ext/${PLATFORM}/include")

# cryptography
IF(USE_CRYPTOPP)
  add_definitions( -DCRYPTOPP )
  add_definitions( -DCRYPTOLIB=CRYPTOPP )
  add_library(libcryptopp_s STATIC IMPORTED)
  SET_TARGET_PROPERTIES(libcryptopp_s PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/../ext/${PLATFORM}/lib/libcryptopp.a)
  set(CRYPTO_LIBS "libcryptopp_s")
ENDIF(USE_CRYPTOPP)

IF(USE_OPENSSL)
  add_definitions( -DOPENSSL )
  add_definitions( -DCRYPTOLIB=OPENSSL )
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  find_package(OpenSSL REQUIRED)
  include_directories(${OPENSSL_INCLUDE_DIR})
  set(CRYPTO_LIBS ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
ENDIF(USE_OPENSSL)

# GPGme
pkg_check_modules(GPGME REQUIRED gpgme)
pkg_check_modules(ASSUAN REQUIRED libassuan)
pkg_check_modules(GPGerror REQUIRED gpg-error)
#add_library(libgpgme_s STATIC IMPORTED)
#add_library(libassuan_s STATIC IMPORTED)
#add_library(libgpg-error_s STATIC IMPORTED)
#SET_TARGET_PROPERTIES(libgpgme_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/../ext/${PLATFORM}/lib/libgpgme.a")
#SET_TARGET_PROPERTIES(libassuan_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/../ext/${PLATFORM}/lib/libassuan.a")
#SET_TARGET_PROPERTIES(libgpg-error_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/../ext/${PLATFORM}/lib/libgpg-error.a")
#set(GPG_LIBS "libgpgme_s" "libassuan_s" "libgpg-error_s")
set(GPG_LIBS "gpgme" "assuan" "gpg-error")

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
add_subdirectory( test )

# packaging
set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "https://www.elykseer.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "base library that provides cryptographic functions to elykseer implementations")
set(CPACK_PACKAGE_VERSION ${${PROJECT_NAME}_VERSION_STRING})
set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR} 0)
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR} 0)
set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH} 0)
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${PLATFORM}_${CMAKE_HOST_SYSTEM_PROCESSOR})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}_${PLATFORM}_${CMAKE_HOST_SYSTEM_PROCESSOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_HOME_DIRECTORY}/../LICENSE")
install(FILES ${CMAKE_HOME_DIRECTORY}/../LICENSE ${CMAKE_HOME_DIRECTORY}/../README.md DESTINATION .)

include(CPack)