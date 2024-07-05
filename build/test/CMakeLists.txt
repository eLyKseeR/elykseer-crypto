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
find_package( Boost 1.83.0 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# mine
include_directories(../src)

set(GENERATED_SRCS
        utAes.cpp
        utGpg.cpp
        utKey128.cpp
        utKey256.cpp
        utMain.cpp
        utMd5.cpp
        utHMAC.cpp
        utRandList.cpp
        utRandom.cpp
        utSha256.cpp)
set_source_files_properties(${GENERATED_SRCS} GENERATED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable ( ut${PROJECT_NAME}
        ${GENERATED_SRCS}
    )
    add_test(NAME ut${PROJECT_NAME} COMMAND ut${PROJECT_NAME} --show_progress)

	add_library(lib${PROJECT_NAME}_s STATIC IMPORTED)
	set_target_properties(lib${PROJECT_NAME}_s PROPERTIES IMPORTED_LOCATION "${CMAKE_HOME_DIRECTORY}/src/libelykseer-crypto_d.a")

    target_link_libraries ( ut${PROJECT_NAME}
        ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}
        ${libs}
        ${Boost_LIBRARIES}
        ${CRYPTO_LIBS}
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "DebugProfile")
    add_executable ( utp${PROJECT_NAME}
        ${GENERATED_SRCS}
    )
    set_target_properties(utp${PROJECT_NAME} PROPERTIES LINK_FLAGS "-fprofile-arcs -ftest-coverage ")
    target_link_libraries ( utp${PROJECT_NAME}
        ${PROJECT_NAME}_p${CMAKE_BUILD_TYPE}
        #gcov
        ${libs}
        ${Boost_LIBRARIES}
        ${CRYPTO_LIBS}
    )
endif()

# generate sources from literate c++ files
set(EXTRACT_CPP_CMD "${CMAKE_CURRENT_SOURCE_DIR}/../../ext/gitalk/utils/extract_cpp.sh")

set(LITERATE_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../test/cpp")

add_custom_command(
  COMMAND rm -f utAes.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utAes.cpp.md"  utAes.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utAes.cpp.md"
  OUTPUT utAes.cpp)
add_custom_command(
  COMMAND rm -f utGpg.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utGpg.cpp.md"  utGpg.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utGpg.cpp.md"
  OUTPUT utGpg.cpp)
add_custom_command(
  COMMAND rm -f utKey128.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utKey128.cpp.md"  utKey128.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utKey128.cpp.md"
  OUTPUT utKey128.cpp)
add_custom_command(
  COMMAND rm -f utKey256.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utKey256.cpp.md"  utKey256.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utKey256.cpp.md"
  OUTPUT utKey256.cpp)
add_custom_command(
  COMMAND rm -f utMain.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utMain.cpp.md"  utMain.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utMain.cpp.md"
  OUTPUT utMain.cpp)
add_custom_command(
  COMMAND rm -f utMd5.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utMd5.cpp.md"  utMd5.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utMd5.cpp.md"
  OUTPUT utMd5.cpp)
add_custom_command(
  COMMAND rm -f utHMAC.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utHMAC.cpp.md"  utHMAC.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utHMAC.cpp.md"
  OUTPUT utHMAC.cpp)
add_custom_command(
  COMMAND rm -f utRandList.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utRandList.cpp.md"  utRandList.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utRandList.cpp.md"
  OUTPUT utRandList.cpp)
add_custom_command(
  COMMAND rm -f utRandom.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utRandom.cpp.md"  utRandom.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utRandom.cpp.md"
  OUTPUT utRandom.cpp)
add_custom_command(
  COMMAND rm -f utSha256.cpp
  COMMAND ${EXTRACT_CPP_CMD} "${LITERATE_SRC_PATH}/utSha256.cpp.md"  utSha256.cpp
  DEPENDS "${LITERATE_SRC_PATH}/utSha256.cpp.md"
  OUTPUT utSha256.cpp)
