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
#include <filesystem>
#include <fstream>
#include <cassert>

#if CRYPTOLIB == CRYPTOPP

#include "cryptopp/sha3.h"

#endif

#include "sizebounded/sizebounded.ipp"

import lxr_key256;


module lxr_sha3;


#if CRYPTOLIB == CRYPTOPP

namespace lxr {

struct lxr::Sha3_256::pimpl {
    CryptoPP::SHA3_256 hash;
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
    assert(256/8 == CryptoPP::SHA3_256::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA3_256::DIGESTSIZE];
    CryptoPP::SHA3_256 hash;
    hash.CalculateDigest( digest, (const unsigned char*)buffer, length );

    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

Key256 Sha3_256::hash(std::filesystem::path const & fpath)
{
    Sha3_256 sha3_256;
    sizebounded<unsigned char, Sha3_256::datasz> buf;
    std::ifstream infile(fpath.string());
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha3_256::datasz);
        sha3_256.process(infile.gcount(), buf);
    }
    return sha3_256.finish();
}

int Sha3_256::process(int len, sizebounded<unsigned char, Sha3_256::datasz> const & buf)
{
    if (len <= Sha3_256::datasz) {
        _pimpl->hash.Update(buf.ptr(), len);
        return len;
    }
    return 0;
}

Key256 Sha3_256::finish()
{
    unsigned char digest[CryptoPP::SHA3_256::DIGESTSIZE];
    _pimpl->hash.Final(digest);
    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

} // namespace

#endif