cmake_minimum_required(VERSION 3.30)

IF(DEFINED CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release DebugProfile RelWithDebInfo MinSizeRel.")
ELSE()
   SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release DebugProfile RelWithDebInfo MinSizeRel.")
ENDIF()

# boost
set(Boost_DEBUG OFF)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.83.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# mine
include_directories(../src)

set(TEST_PRIVATE_SRCS
      utMain.cpp
      utAes.cpp
      utHMAC.cpp
      utKey128.cpp
      utKey256.cpp
      utKey160.cpp
      utBase64.cpp
      utMd5.cpp
      utRandList.cpp
      utRandom.cpp
      # utSha256.cpp # deprecated
      utSha3.cpp
)

# add the following test case to also test GnuPG features:
#   utGpg.cpp

set(TEST_SRCS "")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable ( ut${PROJECT_NAME} )
    target_sources(ut${PROJECT_NAME}
      PRIVATE
        ${TEST_PRIVATE_SRCS}
      PUBLIC
      FILE_SET CXX_MODULES FILES
        ${TEST_SRCS}
	  )
    add_test(NAME ut${PROJECT_NAME} COMMAND ut${PROJECT_NAME} --show_progress)

	add_library(lib${PROJECT_NAME}_s STATIC IMPORTED)
	set_target_properties(lib${PROJECT_NAME}_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/src/libelykseer-crypto_d.a")

    target_link_libraries ( ut${PROJECT_NAME}
        ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s
        ${libs}
        ${Boost_LIBRARIES}
        ${CRYPTO_LIBS}
        # ${GPG_LIBS}
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "DebugProfile")
    add_executable ( utp${PROJECT_NAME}
        ${TEST_SRCS}
    )
    set_target_properties(utp${PROJECT_NAME} PROPERTIES LINK_FLAGS "-fprofile-arcs -ftest-coverage ")
    target_link_libraries ( utp${PROJECT_NAME}
        ${PROJECT_NAME}_p${CMAKE_BUILD_TYPE}
        #gcov
        ${libs}
        ${Boost_LIBRARIES}
        ${CRYPTO_LIBS}
        # ${GPG_LIBS}
    )
endif()
