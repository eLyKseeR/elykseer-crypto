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
#include <iostream>
#include <string>
#include <memory>
#include <functional>

import lxr_key128;
import lxr_key160;
import lxr_key256;


export module lxr_hmac;


export namespace lxr {

class HMAC
{
    public:
        static Key128 hmac_md5(const char k[], int klen, std::string const & m);
        static Key128 hmac_md5(const char k[], int klen, const char *m, int mlen);
#if CRYPTOLIB == OPENSSL
        static Key256 hmac_sha256(const char k[], int klen, std::string const & m);
        static Key256 hmac_sha256(const char k[], int klen, const char *m, int mlen);
#endif
        static Key160 hmac_sha1(const char k[], int klen, std::string const & m);
        static Key160 hmac_sha1(const char k[], int klen, const char *m, int mlen);
    protected:
        HMAC() {}
    private:
        HMAC(HMAC const &) = delete;
        HMAC & operator=(HMAC const &) = delete;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" {
export CKey128* hmac_Md5(int klen, const char k[], int mlen, const char *m);

#if CRYPTOLIB == OPENSSL
export CKey256* hmac_Sha256(int klen, const char k[], int mlen, const char *m);
#endif

export CKey160* hmac_Sha1(int klen, const char k[], int mlen, const char *m);
}