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

#if CRYPTOLIB == CRYPTOPP

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"

#endif

#include <cassert>

import lxr_key128;


module lxr_md5;


#if CRYPTOLIB == CRYPTOPP

namespace lxr {

Key128 Md5::hash(std::string const & msg)
{
    return Md5::hash(msg.c_str(), msg.size());
}

Key128 Md5::hash(const char buffer[], int length)
{
    assert(128/8 == CryptoPP::Weak::MD5::DIGESTSIZE);
    unsigned char digest[CryptoPP::Weak::MD5::DIGESTSIZE];
    CryptoPP::Weak::MD5 hash;
    hash.CalculateDigest(digest, (unsigned char const *)buffer, length);

    Key128 k(true);
    k.fromBytes(digest);
    return k;
}

} // namespace

#endif