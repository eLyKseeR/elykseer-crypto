cmake_minimum_required(VERSION 3.15)
project(elykseer-crypto-tools C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

add_executable(encryptCBC encrypt_cbc.c)
target_link_libraries(encryptCBC PRIVATE OpenSSL::Crypto)

add_executable(decryptCBC decrypt_cbc.c)
target_link_libraries(decryptCBC PRIVATE OpenSSL::Crypto)

add_executable(encryptGCM encrypt_gcm.c)
target_link_libraries(encryptGCM PRIVATE OpenSSL::Crypto)

add_executable(decryptGCM decrypt_gcm.c)
target_link_libraries(decryptGCM PRIVATE OpenSSL::Crypto)

add_executable(encryptCTR encrypt_ctr.c)
target_link_libraries(encryptCTR PRIVATE OpenSSL::Crypto)

add_executable(decryptCTR decrypt_ctr.c)
target_link_libraries(decryptCTR PRIVATE OpenSSL::Crypto)

# Optional: install to build directory's bin/
install(TARGETS encryptCBC decryptCBC encryptGCM decryptGCM encryptCTR decryptCTR RUNTIME DESTINATION bin)
