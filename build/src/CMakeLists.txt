cmake_minimum_required(VERSION 3.5)

# search path for headers
include_directories(".")

# list of source files to be compiled
set(GENERATED_SRCS
  aes.cpp
  gpg.cpp
  key.cpp
  key128.cpp
  key256.cpp
  md5.cpp
  hmac.cpp
  base64.cpp
  randlist.cpp
  random.cpp
  sha256.cpp)
set_source_files_properties(${GENERATED_SRCS} GENERATED)
set(GENERATED_HDRS
  lxr/aes.hpp
  lxr/gpg.hpp
  lxr/key.hpp
  lxr/key128.hpp
  lxr/key256.hpp
  lxr/lxr-cbindings.hpp
  lxr/lxr-crypto.hpp
  lxr/md5.hpp
  lxr/hmac.hpp
  lxr/base64.hpp
  lxr/randlist.hpp
  lxr/random.hpp
  lxr/sha256.hpp)
set_source_files_properties(${GENERATED_HDRS} GENERATED)

# generate sources from literate c++ files
set(MAKE_CPP_CMD ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/gitalk/utils/make_cpp.sh)
set(MAKE_HPP_CMD ${CMAKE_COMMAND} -E env HTML2TEXT=${HTML2TEXT} ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/gitalk/utils/make_hpp.sh)

set(LITERATE_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../src/cpp")

add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f lxr-crypto.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/lxr-crypto.hpp.md"
  OUTPUT lxr/lxr-crypto.hpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f lxr-cbindings.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/lxr-cbindings.hpp.md"
  OUTPUT lxr/lxr-cbindings.hpp)

add_custom_command(
  COMMAND rm -f aes.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/aes.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/aes.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/aes*.md"
  OUTPUT aes.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f aes.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/aes.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/aes.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/aes*.md"
  OUTPUT lxr/aes.hpp)

add_custom_command(
  COMMAND rm -f gpg.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/gpg.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/gpg.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/gpg*.md"
  OUTPUT gpg.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f gpg.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/gpg.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/gpg.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/gpg*.md"
  OUTPUT lxr/gpg.hpp)

add_custom_command(
  COMMAND rm -f key.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/key.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/key.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/key*.md"
  OUTPUT key.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f key.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/key.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/key.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/key*.md"
  OUTPUT lxr/key.hpp)

add_custom_command(
  COMMAND rm -f key128.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/key128.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/key128.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/key128*.md"
  OUTPUT key128.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f key128.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/key128.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/key128.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/key128*.md"
  OUTPUT lxr/key128.hpp)

add_custom_command(
  COMMAND rm -f key256.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/key256.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/key256.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/key256*.md"
  OUTPUT key256.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f key256.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/key256.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/key256.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/key256*.md"
  OUTPUT lxr/key256.hpp)

add_custom_command(
  COMMAND rm -f md5.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/md5.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/md5.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/md5*.md"
  OUTPUT md5.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f md5.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/md5.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/md5.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/md5*.md"
  OUTPUT lxr/md5.hpp)

add_custom_command(
  COMMAND rm -f hmac.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/hmac.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/hmac.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/hmac*.md"
  OUTPUT hmac.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f hmac.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/hmac.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/hmac.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/hmac*.md"
  OUTPUT lxr/hmac.hpp)

add_custom_command(
  COMMAND rm -f base64.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/base64.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/base64.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/base64*.md"
  OUTPUT base64.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f base64.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/base64.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/base64.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/base64*.md"
  OUTPUT lxr/base64.hpp)

  add_custom_command(
  COMMAND rm -f randlist.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/randlist.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/randlist.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/randlist*.md"
  OUTPUT randlist.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f randlist.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/randlist.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/randlist.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/randlist*.md"
  OUTPUT lxr/randlist.hpp)

add_custom_command(
  COMMAND rm -f random.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/random.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/random.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/random*.md"
  OUTPUT random.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f random.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/random.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/random.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/random*.md"
  OUTPUT lxr/random.hpp)

add_custom_command(
  COMMAND rm -f sha256.cpp
  COMMAND ${MAKE_CPP_CMD} "${LITERATE_SRC_PATH}/sha256.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/sha256.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/sha256*.md"
  OUTPUT sha256.cpp)
add_custom_command(
  WORKING_DIRECTORY "lxr"
  COMMAND rm -f sha256.hpp
  COMMAND ${MAKE_HPP_CMD} "${LITERATE_SRC_PATH}/sha256.hpp.md"
  MAIN_DEPENDENCY "${LITERATE_SRC_PATH}/sha256.hpp.md"
  DEPENDS "${LITERATE_SRC_PATH}/sha256*.md"
  OUTPUT lxr/sha256.hpp)


IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("not building dynamic library on ${CMAKE_SYSTEM_NAME}")
ELSE()
  add_library (${PROJECT_NAME}_${CMAKE_BUILD_TYPE} SHARED
               ${GENERATED_SRCS}
               ${GENERATED_HDRS})
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
	${GENERATED_SRCS}
	${GENERATED_HDRS}
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
		${GENERATED_SRCS}
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
install(FILES ${LIBRARY_NAME} DESTINATION lib)
install(FILES ${GENERATED_HDRS} DESTINATION include/lxr)
include(CPack)

set(CPACK_BINARY_STGZ "OFF")
set(CPACK_BINARY_TGZ "ON")
