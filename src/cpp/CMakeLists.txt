cmake_minimum_required(VERSION 3.30)


# list of source files to be compiled
set(LIB_PRIVATE_SRCS
  aes_cryptopp.cpp
  aes_openssl.cpp
  aes_cbindings.cpp
  global.cpp
  global_cbindings.cpp
  gpg.cpp
  key.cpp
  key128.cpp
  key128_cbindings.cpp
  key160.cpp
  key160_cbindings.cpp
  key256.cpp
  key256_cbindings.cpp
  md5_cryptopp.cpp
  md5_openssl.cpp
  md5_cbindings.cpp
  hmac_cryptopp.cpp
  hmac_openssl.cpp
  hmac_cbindings.cpp
  base64_cryptopp.cpp
  base64_openssl.cpp
  base64_cbindings.cpp
  randlist.cpp
  random.cpp
  random_cbindings.cpp
  # sha256.cpp  # deprecated
  # sha256_cryptopp.cpp
  # sha256_openssl.cpp
  # sha256_cbindings.cpp
  sha3.cpp
  sha3_cryptopp.cpp
  sha3_openssl.cpp
  sha3_cbindings.cpp
  lxr-cbindings.hpp
)
set(LIB_SRCS "")
set(LIB_HDRS
  aes.ixx
  global.ixx
  gpg.ixx
  key.ixx
  key128.ixx
  key160.ixx
  key256.ixx
  lxr_crypto.ixx
  md5.ixx
  hmac.ixx
  base64.ixx
  randlist.ixx
  random.ixx
  # sha256.ixx  # deprecated
  sha3.ixx
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("not building dynamic library on ${CMAKE_SYSTEM_NAME}")
ELSE()
  add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} SHARED)
  target_sources(${PROJECT_NAME}_${CMAKE_BUILD_TYPE}
      PRIVATE
        ${LIB_PRIVATE_SRCS}
      PUBLIC
      FILE_SET CXX_MODULES FILES
        ${LIB_HDRS}
        ${LIB_SRCS}
  )
  set_target_properties(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PROPERTIES
                        VERSION ${elykseer-crypto_VERSION_STRING}
                        SOVERSION ${elykseer-crypto_VERSION_MAJOR})

  target_link_libraries (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
      ${libs}
      ${Boost_LIBRARIES}
      ${CRYPTO_LIBS}
      ${GPG_LIBS}
  )
  if(OpenMP_CXX_FOUND)
    target_link_libraries (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC
	  OpenMP::OpenMP_CXX)
  endif()
  target_compile_features(${PROJECT_NAME}_${CMAKE_BUILD_TYPE} PUBLIC cxx_std_20)

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s STATIC)
target_sources(${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s
      PRIVATE
        ${LIB_PRIVATE_SRCS}
      PUBLIC
      FILE_SET CXX_MODULES FILES
        ${LIB_HDRS}
        ${LIB_SRCS}
)
target_link_libraries(${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s PUBLIC
  ${libs}
  ${CRYPTO_LIBS}
)
if(OpenMP_CXX_FOUND)
  target_link_libraries (${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s PUBLIC
    OpenMP::OpenMP_CXX)
endif()
target_compile_features(${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s PUBLIC cxx_std_20)

# combine all static libs into one
IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
set(LIBRARY_NAME "lib${PROJECT_NAME}_d.a")
ELSE()
set(LIBRARY_NAME "lib${PROJECT_NAME}.a")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_custom_command(OUTPUT ${LIBRARY_NAME}
    COMMAND libtool -static -o ${LIBRARY_NAME}
          ${LIB_PREFIX}${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s.a
          ../../../ext/${EXT_PATH}/lib/libgpgme.a
          ../../../ext/${EXT_PATH}/lib/libgpg-error.a
          ../../../ext/${EXT_PATH}/lib/libassuan.a
          ${CRYPTO_LIBPATHS}
        DEPENDS ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s 
        COMMENT "combining static libraries into ${LIBRARY_NAME}"
  )
ELSE()
  list(TRANSFORM CRYPTO_LIBPATHS
       PREPEND "addlib "
       OUTPUT_VARIABLE CRYPTO_ADD_LIBS
       )
  list(TRANSFORM CRYPTO_ADD_LIBS
       APPEND "\n"
       )
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/lib_${CMAKE_BUILD_TYPE}.mri"
"create ${LIBRARY_NAME}\n\
addlib ${LIB_PREFIX}${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s.a\n\
addlib ../../../ext/${EXT_PATH}/lib/libgpgme.a\n\
addlib ../../../ext/${EXT_PATH}/lib/libgpg-error.a\n\
addlib ../../../ext/${EXT_PATH}/lib/libassuan.a\n\
${CRYPTO_ADD_LIBS}\
save\n\
end")

  add_custom_command(OUTPUT ${LIBRARY_NAME}
    COMMAND ${SELECTED_AR} -M <"${CMAKE_CURRENT_BINARY_DIR}/lib_${CMAKE_BUILD_TYPE}.mri"
    DEPENDS ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}_s 
    COMMENT "combining static libraries into ${LIBRARY_NAME}"
  )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_custom_target(static_lib ALL
	DEPENDS ${LIBRARY_NAME}
)

# profiling
if (CMAKE_BUILD_TYPE STREQUAL "DebugProfile")
	add_library (${PROJECT_NAME}_p${CMAKE_BUILD_TYPE} STATIC)
  target_sources(${PROJECT_NAME}_p${CMAKE_BUILD_TYPE}
      PRIVATE
        ${LIB_PRIVATE_SRCS}
      PUBLIC
      FILE_SET CXX_MODULES FILES
        ${LIB_HDRS}
        ${LIB_SRCS}
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

set(CPACK_PACKAGE_VERSION ${${PROJECT_NAME}_VERSION_STRING})
set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME} DESTINATION lib)
install(FILES ${LIB_HDRS} lxr-cbindings.hpp DESTINATION include/lxr)

include(CPack)

set(CPACK_BINARY_STGZ "OFF")
set(CPACK_BINARY_TGZ "ON")
