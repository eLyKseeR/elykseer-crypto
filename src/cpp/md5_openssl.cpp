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

#if CRYPTOLIB == OPENSSL

#include <string>

#include "openssl/evp.h"

#endif

import lxr_key128;


module lxr_md5;


#if CRYPTOLIB == OPENSSL

namespace lxr {

Key128 Md5::hash(std::string const & msg)
{
    return Md5::hash(msg.c_str(), msg.size());
}

Key128 Md5::hash(const char buffer[], int length)
{
    const EVP_MD *md5 = EVP_md5();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int md5_len;

    EVP_DigestInit_ex2(ctx, md5, NULL);
    EVP_DigestUpdate(ctx, buffer, length);
    EVP_DigestFinal_ex(ctx, digest, &md5_len);
    EVP_MD_CTX_free(ctx);

    Key128 k(true);
    k.fromBytes(digest);
    return k;
}

} // namespace

#endif