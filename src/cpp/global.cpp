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

#include "openssl/conf.h"
#include "openssl/evp.h"
#include "openssl/err.h"

#endif

module lxr_global;


namespace lxr {

bool Global::_initialized = false;

#if CRYPTOLIB == OPENSSL
bool Global::initialize() {
    if (! _initialized) {
        _initialized = true;
        return OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_CIPHERS | OPENSSL_INIT_ADD_ALL_DIGESTS | OPENSSL_INIT_NO_ATEXIT, NULL);
    }
    return false;
}

bool Global::cleanup() {
    if (_initialized) {
        _initialized = false;
        OPENSSL_cleanup();
        return true;
    } else {
        return false;
    }
}
#endif

#if CRYPTOLIB == CRYPTOPP
bool Global::initialize() { return true; }
bool Global::cleanup() { return true; }
#endif

} // namespace
