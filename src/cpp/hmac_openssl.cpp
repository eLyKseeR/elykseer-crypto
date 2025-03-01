module;
/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2025 Alexander Diemand

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstddef>
#include <algorithm>
#include <cstring>
#include <string>
#include <cassert>

#if CRYPTOLIB == OPENSSL
#include "openssl/evp.h"
#include "openssl/core_names.h"
#endif

import lxr_key128;
import lxr_key160;
import lxr_key256;


module lxr_hmac;


#if CRYPTOLIB == OPENSSL

namespace lxr {

Key128 HMAC::hmac_md5(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_md5(k, klen, msg.c_str(), msg.size());
}

Key128 HMAC::hmac_md5(const char k[], int klen, const char *buffer, int blen)
{
    // CryptoPP::HMAC<CryptoPP::Weak::MD5> hmac((const CryptoPP::byte *)k, klen);
    // assert(128/8 == hmac.DIGESTSIZE);
    // unsigned char digest[hmac.DIGESTSIZE];
    // hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, "md5", 11);
    params[1] = OSSL_PARAM_construct_end();
    constexpr int tblen = 512*2;
    unsigned char digest[tblen];
    memset(digest, 0, tblen);
    int count = 0;
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    EVP_MAC_CTX *ctx = EVP_MAC_CTX_new(mac);
    EVP_MAC_init(ctx, (const unsigned char *)k, klen, params);
    while (count < blen) {
        unsigned long sz = std::min(tblen, blen - count);
        EVP_MAC_update(ctx, (const unsigned char *)(buffer + count), sz);
        count += sz;
    }
    EVP_MAC_final(ctx, digest, (std::size_t*)&count, tblen);
    assert(count == 16);
    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);

    Key128 k128(true);
    k128.fromBytes(digest);
    return k128;
}

Key256 HMAC::hmac_sha256(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_sha256(k, klen, msg.c_str(), msg.size());
}

Key256 HMAC::hmac_sha256(const char k[], int klen, const char *buffer, int blen)
{
    // CryptoPP::HMAC<CryptoPP::SHA256> hmac((const CryptoPP::byte *)k, klen);
    // assert(256/8 == hmac.DIGESTSIZE);
    // unsigned char digest[hmac.DIGESTSIZE];
    // hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, "SHA2-256", 11);
    params[1] = OSSL_PARAM_construct_end();
    constexpr int tblen = 512*2;
    unsigned char digest[tblen];
    memset(digest, 0, tblen);
    int count = 0;
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    EVP_MAC_CTX *ctx = EVP_MAC_CTX_new(mac);
    EVP_MAC_init(ctx, (const unsigned char *)k, klen, params);
    while (count < blen) {
        unsigned long sz = std::min(tblen, blen - count);
        EVP_MAC_update(ctx, (const unsigned char *)(buffer + count), sz);
        count += sz;
    }
    EVP_MAC_final(ctx, digest, (std::size_t*)&count, tblen);
    assert(count == 32);
    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);

    Key256 k256(true);
    k256.fromBytes(digest);
    return k256;
}

Key160 HMAC::hmac_sha1(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_sha1(k, klen, msg.c_str(), msg.size());
}

Key160 HMAC::hmac_sha1(const char k[], int klen, const char *buffer, int blen)
{
    // EVP_aes_256_cbc_hmac_sha1
    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, "sha1", 11);
    params[1] = OSSL_PARAM_construct_end();
    constexpr int tblen = 512*2;
    unsigned char digest[tblen];
    memset(digest, 0, tblen);
    int count = 0;
    EVP_MAC *mac = EVP_MAC_fetch(NULL, "HMAC", NULL);
    EVP_MAC_CTX *ctx = EVP_MAC_CTX_new(mac);
    EVP_MAC_init(ctx, (const unsigned char *)k, klen, params);
    while (count < blen) {
        unsigned long sz = std::min(tblen, blen - count);
        EVP_MAC_update(ctx, (const unsigned char *)(buffer + count), sz);
        count += sz;
    }
    EVP_MAC_final(ctx, digest, (std::size_t*)&count, tblen);
    assert(count == 20);
    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);

    Key160 k160(true);
    k160.fromBytes(digest);
    return k160;
}

} // namespace

#endif