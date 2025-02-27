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

#include <filesystem>
#include <fstream>
#include <iostream>

#include "openssl/evp.h"
#include "openssl/sha.h"

#include "sizebounded/sizebounded.ipp"

#endif

import lxr_key256;


module lxr_sha3;


#if CRYPTOLIB == OPENSSL

namespace lxr {

struct lxr::Sha3_256::pimpl {
    pimpl() {
        ctx = EVP_MD_CTX_new();
        const EVP_MD *md = EVP_sha3_256();
        EVP_DigestInit_ex(ctx, md, nullptr);
    }
    ~pimpl() {
        if (ctx) {
            EVP_MD_CTX_free(ctx);
        }
    }
    EVP_MD_CTX *ctx = NULL;
};

Sha3_256::Sha3_256()
    : _pimpl(new Sha3_256::pimpl)
{
}

Sha3_256::~Sha3_256()
{
    if (_pimpl) {
        _pimpl.reset();
    }
}


Key256 Sha3_256::hash(const char buffer[], int length)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const EVP_MD *md = EVP_sha3_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);

    EVP_DigestUpdate(ctx, buffer, length);
    unsigned int digest_len{0};
    EVP_DigestFinal_ex(ctx, digest, &digest_len);

    Key256 k(true);
    k.fromBytes(digest);
    EVP_MD_CTX_free(ctx);
    return k;
}

Key256 Sha3_256::hash(std::filesystem::path const & fpath)
{
    Sha3_256 sha3_256;
    sizebounded<unsigned char, Sha3_256::datasz> buf;
    std::ifstream infile(fpath);
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha3_256::datasz);
        int count = sha3_256.process(infile.gcount(), buf);
        if (count < 0) {
            std::clog << "error in Sha3_256::hash(fp): " << count << std::endl;
            break;
        }
    }
    return sha3_256.finish();
}

int Sha3_256::process(int len, sizebounded<unsigned char, Sha3_256::datasz> const & buf)
{
    if (len <= Sha3_256::datasz) {
        EVP_DigestUpdate(_pimpl->ctx, buf.ptr(), len);
        return len;
    }
    return -1;
}

Key256 Sha3_256::finish()
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    unsigned int digest_len{0};
    EVP_DigestFinal_ex(_pimpl->ctx, digest, &digest_len);

    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

} // namespace

#endif