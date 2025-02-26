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

#if CRYPTOLIB == OPENSSL
#include "openssl/evp.h"
#endif

#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"
#endif

#include "lxr-cbindings.hpp"

import lxr_key128;
import lxr_key160;
import lxr_key256;


module lxr_hmac;


extern "C" EXPORT
CKey128* hmac_Md5(int klen, const char k[], int mlen, const char * m)
{
    auto h = lxr::HMAC::hmac_md5(k, klen, m, mlen);
    CKey128 * r = new CKey128; r->ptr = new lxr::Key128(h);
    return r;
}

extern "C" EXPORT
CKey256* hmac_Sha256(int klen, const char k[], int mlen, const char * m)
{
    auto h = lxr::HMAC::hmac_sha256(k, klen, m, mlen);
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

extern "C" EXPORT
CKey160* hmac_Sha1(int klen, const char k[], int mlen, const char * m)
{
    lxr::Key160 h = lxr::HMAC::hmac_sha1(k, klen, m, mlen);
    CKey160 * r = new CKey160; r->ptr = new lxr::Key160(h);
    return r;
}