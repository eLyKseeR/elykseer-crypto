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
#include <cassert>

#if CRYPTOLIB == CRYPTOPP

#include "cryptopp/sha.h"

#endif

#include "sizebounded/sizebounded.ipp"

import lxr_key256;


module lxr_sha256;


#if CRYPTOLIB == CRYPTOPP

namespace lxr {

struct lxr::Sha256::pimpl {
    CryptoPP::SHA256 hash;
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
    CryptoPP::SHA256 hash;
    assert(256/8 == CryptoPP::SHA256::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    hash.CalculateDigest( digest, (const unsigned char*)buffer, length );

    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

Key256 Sha256::hash(std::filesystem::path const & fpath)
{
    Sha256 sha256;
    sizebounded<unsigned char, Sha256::datasz> buf;
    std::ifstream infile(fpath.string());
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha256::datasz);
        sha256.process(infile.gcount(), buf);
    }
    return sha256.finish();
}

int Sha256::process(int len, sizebounded<unsigned char, Sha256::datasz> const & buf)
{
    if (len <= Sha256::datasz) {
        _pimpl->hash.Update(buf.ptr(), len);
        return len;
    }
    return 0;
}

Key256 Sha256::finish()
{
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    _pimpl->hash.Final(digest);
    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

} // namespace

#endif