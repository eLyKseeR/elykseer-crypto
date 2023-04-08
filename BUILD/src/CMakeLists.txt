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

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("not building dynamic library on ${CMAKE_SYSTEM_NAME}")
ELSE()
  add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} SHARED
               ${SRCS})
  set_target_properties(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PROPERTIES
                        VERSION ${elykseer-crypto_VERSION_STRING}
                        SOVERSION ${elykseer-crypto_VERSION_MAJOR})

  target_link_libraries (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
      ${libs}
      ${Boost_LIBRARIES}
      ${CRYPTO_LIBS}
      ${GPG_LIBS}
  )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s STATIC
  ${SRCS}
)

# combine all static libs into one
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
set(LIBRARY_NAME "lib${PROJECT_NAME}_d.a")
ELSE()
set(LIBRARY_NAME "lib${PROJECT_NAME}.a")
ENDIF()

add_custom_command(OUTPUT ${LIBRARY_NAME}
	COMMAND ${CMAKE_AR} -M <"lib_${CMAKE_BUILD_TYPE}.mri"
	DEPENDS ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s 
	COMMENT "combining static libraries into ${LIBRARY_NAME}"
)

add_custom_target(static_lib ALL
	DEPENDS ${LIBRARY_NAME}
)

# profiling
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
            ${GPG_LIBS}
            #${GPGME_LIBRARIES}
            #${ASSUAN_LIBRARIES}
            #${GPGerror_LIBRARIES}
	)
endif()


