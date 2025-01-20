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

#include <filesystem>
#include <fstream>

#if CRYPTOLIB == OPENSSL

#include "openssl/evp.h"
#include "openssl/sha.h"

#endif

#include "sizebounded/sizebounded.ipp"

import lxr_key256;


module lxr_sha256;


#if CRYPTOLIB == OPENSSL

namespace lxr {

struct lxr::Sha256::pimpl {
    const EVP_MD *md = EVP_sha2_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);
};

Sha256::Sha256()
    : _pimpl(new Sha256::pimpl)
{
}

Sha256::~Sha256()
{
    if (_pimpl) {
        _pimpl.reset();
    }
}


Key256 Sha256::hash(const char buffer[], int length)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const EVP_MD *md = EVP_sha2_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);

    EVP_DigestUpdate(ctx, buffer, length);
    EVP_DigestFinal_ex(ctx, digest, nullptr);

    Key256 k(true);
    k.fromBytes(digest);
    EVP_MD_CTX_free(ctx);
    return k;
}

Key256 Sha256::hash(std::filesystem::path const & fpath)
{
    Sha256 sha256;
    sizebounded<unsigned char, Sha256::datasz> buf;
    std::filesystem::ifstream infile(fpath);
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha256::datasz);
        sha256.process(infile.gcount(), buf);
    }
    k.fromBytes(digest);
    return sha256.finish();
}

int Sha256::process(int len, sizebounded<unsigned char, Sha256::datasz> const & buf)
{
    if (len <= Sha256::datasz) {
        EVP_DigestUpdate(_pimpl->ctx, buf->ptr(), len);
        return len;
    }
    return 0;
}

Key256 Sha256::finish()
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    EVP_DigestFinal_ex(_pimpl->ctx, digest, nullptr);
    Key256 k(true);
    k.fromBytes(digest);
    EVP_MD_CTX_free(_pimpl->ctx);
    return k;
}

} // namespace

#endif