cmake_minimum_required(VERSION 3.5)

# search path for headers
include_directories(".")

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

# targets

add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s STATIC
  ${SRCS}
)

# combine all static libs into one
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
set(LIBRARY_NAME "lib${PROJECT_NAME}_d.a")
ELSE()
set(LIBRARY_NAME "lib${PROJECT_NAME}.a")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_custom_command(OUTPUT ${LIBRARY_NAME}
    COMMAND libtool -static -o ${LIBRARY_NAME} ${LIB_PREFIX}${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s.a ../../ext/${EXT_PATH}/lib/libgpgme.a ../../ext/${EXT_PATH}/lib/libgpg-error.a ../../ext/${EXT_PATH}/lib/libassuan.a ../../ext/${EXT_PATH}/lib/libcryptopp.a
        DEPENDS ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s 
        COMMENT "combining static libraries into ${LIBRARY_NAME}"
  )
ELSE()
  file(WRITE "lib_${CMAKE_BUILD_TYPE}.mri"
"create ${LIBRARY_NAME}\n\
addlib ${LIB_PREFIX}${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s.a\n\
addlib ../../ext/${EXT_PATH}/lib/libgpgme.a\n\
addlib ../../ext/${EXT_PATH}/lib/libgpg-error.a\n\
addlib ../../ext/${EXT_PATH}/lib/libassuan.a\n\
addlib ../../ext/${EXT_PATH}/lib/libcryptopp.a\n\
save\n\
end")

  add_custom_command(OUTPUT ${LIBRARY_NAME}
	COMMAND ${SELECTED_AR} -M <"lib_${CMAKE_BUILD_TYPE}.mri"
	DEPENDS ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s 
	COMMENT "combining static libraries into ${LIBRARY_NAME}"
  )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

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
	)
endif()


