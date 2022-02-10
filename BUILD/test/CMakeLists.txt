cmake_minimum_required(VERSION 3.5)

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
find_package( Boost 1.67.0 REQUIRED COMPONENTS system unit_test_framework contract)
include_directories(${Boost_INCLUDE_DIRS})

# mine
include_directories(../src)

# link external libraries
set(libs "dl" "z")

file(GLOB SRCS ut*.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")

        add_executable ( ut${PROJECT_NAME}
          ${SRCS}
        )

        target_link_libraries ( ut${PROJECT_NAME}
          elykseer-crypto_${CMAKE_BUILD_TYPE}
          ${libs}
          ${Boost_LIBRARIES}
          ${CRYPTO_LIBS}
        )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "DebugProfile")
        add_executable ( utp${PROJECT_NAME}
                ${SRCS}
        )
        set_target_properties(utp${PROJECT_NAME} PROPERTIES LINK_FLAGS "-fprofile-arcs -ftest-coverage ")
        target_link_libraries ( utp${PROJECT_NAME}
                elykseer-crypto_p${CMAKE_BUILD_TYPE}
                #gcov
                ${libs}
                ${Boost_LIBRARIES}
                ${CRYPTO_LIBS}
        )

endif()

