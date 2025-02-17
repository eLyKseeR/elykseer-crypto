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
#include <string>
#include <cassert>

#if CRYPTOLIB == OPENSSL
// #include "openssl/hmac.h"
#error to-be-done
#endif

#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"
#endif

import lxr_key128;
import lxr_key160;
import lxr_key256;


module lxr_hmac;


#if CRYPTOLIB == CRYPTOPP

namespace lxr {

Key128 HMAC::hmac_md5(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_md5(k, klen, msg.c_str(), msg.size());
}

Key128 HMAC::hmac_md5(const char k[], int klen, const char *buffer, int blen)
{
    CryptoPP::HMAC<CryptoPP::Weak::MD5> hmac((const CryptoPP::byte *)k, klen);
    assert(128/8 == hmac.DIGESTSIZE);
    unsigned char digest[hmac.DIGESTSIZE];
    hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);
    
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
    CryptoPP::HMAC<CryptoPP::SHA256> hmac((const CryptoPP::byte *)k, klen);
    assert(256/8 == hmac.DIGESTSIZE);
    unsigned char digest[hmac.DIGESTSIZE];
    hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);

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
    CryptoPP::HMAC<CryptoPP::SHA1> hmac((const CryptoPP::byte *)k, klen);
    assert(160/8 == hmac.DIGESTSIZE);
    unsigned char digest[hmac.DIGESTSIZE];
    hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);

    Key160 k160(true);
    k160.fromBytes(digest);
    return k160;
}

} // namespace

#endif