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

#include <string>
#include <string_view>
#include <ctime>
#include <cassert>
#include <iostream>

#if CRYPTOLIB == OPENSSL
#include "openssl/bio.h"
#include "openssl/evp.h"
#endif


module lxr_base64;


# if CRYPTOLIB == OPENSSL

namespace lxr {

std::string Base64::encode(std::string_view const & m)
{
    std::string out;
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO *bmem = BIO_new(BIO_s_mem());
    BIO *bio = BIO_push(b64, bmem);
    constexpr int blen = 1024;
    // unsigned char buffer[blen];
    const unsigned char *inbuf = (const unsigned char *)m.data();
    const int sz = m.size();
    int count = 0;
    while (count < sz) {
        int inlen = std::min(blen, sz - count);
        int outlen = BIO_write(bio, (const char*)(inbuf + count), inlen);
        count += outlen;
    }
    BIO_flush(bio);
    char *data = NULL;
    long outlen = BIO_get_mem_data(bmem, &data);
    if (data && outlen > 0) {
        out = std::string((const char*)data, outlen);
    }
    BIO_free_all(bio);
    return std::move(out);
}

std::string Base64::decode(std::string_view const & m)
{
    std::string out;
    EVP_ENCODE_CTX *ctx = EVP_ENCODE_CTX_new();
    constexpr int blen = 1024;
    unsigned char buffer[blen];
    const unsigned char *inbuf = (const unsigned char *)m.data();
    const int sz = m.size();
    int count = 0;
    EVP_DecodeInit(ctx);
    while (count < sz) {
        int outlen = 0;
        int inlen = std::min(blen, sz - count);
        EVP_DecodeUpdate(ctx,  buffer, &outlen,  inbuf + count, inlen);
        if (outlen > 0) {
            out += std::string((const char*)buffer, outlen);
        }
        count += inlen;
    }
    int outlen = 0;
    EVP_DecodeFinal(ctx, buffer, &outlen);
    if (outlen > 0) {
        out += std::string((const char*)buffer, outlen);
    }
    EVP_ENCODE_CTX_free(ctx);
    return std::move(out);
}

} // namespace

# endif