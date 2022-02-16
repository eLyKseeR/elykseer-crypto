cmake_minimum_required(VERSION 3.5)

# search path for headers
include_directories(".")

# link external libraries
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  # specific code
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Linux specific code
  set(libs "dl" "z")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# list of source files to be compiled
file(GLOB SRCS *.cpp)

add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} SHARED
  ${SRCS}
)
add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s STATIC
  ${SRCS}
)

set_target_properties(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PROPERTIES
                      VERSION ${elykseer-crypto_VERSION_STRING}
                      SOVERSION ${elykseer-crypto_VERSION_MAJOR})

if (CMAKE_BUILD_TYPE STREQUAL "DebugProfile")
	add_library (${PROJECT_NAME}_p${CMAKE_BUILD_TYPE} STATIC
		${SRCS}
	)
	set_target_properties(${PROJECT_NAME}_p${CMAKE_BUILD_TYPE} PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	#set(libs, "${libs} gcov")
	target_link_libraries (${PROJECT_NAME}_p${CMAKE_BUILD_TYPE}
		${libs}
		${Boost_LIBRARIES}
    ${CRYPTO_LIBS}
    ${GPGME_LIBRARIES}
    ${ASSUAN_LIBRARIES}
    ${GPGerror_LIBRARIES}
	)
endif()


target_link_libraries (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
    ${libs}
    ${Boost_LIBRARIES}
    ${CRYPTO_LIBS}
    ${GPGME_LIBRARIES}
    ${ASSUAN_LIBRARIES}
    ${GPGerror_LIBRARIES}
)

