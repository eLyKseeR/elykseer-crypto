# call: cmake .
# on Linux/OSX to build the Makefiles
# run with 'make VERBOSE=1' to get detailed output
#
# on OSX: cmake -G Xcode .
# to build the Xcode project files

cmake_minimum_required(VERSION 3.5)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

IF(DEFINED CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release DebugProfile RelWithDebInfo MinSizeRel.")
ELSE()
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release DebugProfile RelWithDebInfo MinSizeRel.")
ENDIF()

project(elykseer-crypto CXX)

# The version number.
set (${PROJECT_NAME}_VERSION_MAJOR 1)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (${PROJECT_NAME}_VERSION_PATCH 7)
set (${PROJECT_NAME}_VERSION_STRING ${elykseer-crypto_VERSION_MAJOR}.${elykseer-crypto_VERSION_MINOR}.${elykseer-crypto_VERSION_PATCH})

# choose either one
set(USE_OPENSSL NO)
set(USE_CRYPTOPP YES)

#if(XCODE)
#  set(CMAKE_CXX_COMPILER_WORKS TRUE)
#  set(CMAKE_C_COMPILER_WORKS TRUE)
#endif()

# boost
add_definitions( -DBOOST_ALL_DYN_LINK )
set(Boost_DEBUG OFF)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.67.0 REQUIRED COMPONENTS system contract )
LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})

# cryptography
IF(USE_CRYPTOPP)
  add_definitions( -DCRYPTOPP )
  add_definitions( -DCRYPTOLIB=CRYPTOPP )
  add_library(libcryptopp STATIC IMPORTED)
  SET_TARGET_PROPERTIES(libcryptopp PROPERTIES IMPORTED_LOCATION ${CMAKE_HOME_DIRECTORY}/../ext/cryptopp/libcryptopp.a)
  set(CRYPTO_LIBS "libcryptopp")
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


set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# OS dependent
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12.0")
  include_directories(/usr/local/opt/zlib/include)
  link_directories(/usr/local/opt/zlib/lib)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# other external tools
include_directories(../ext)


add_subdirectory( src )
add_subdirectory( test )

