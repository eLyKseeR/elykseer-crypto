module;
/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2026 Alexander Diemand

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

#include <cstring>
#include <memory>

#include "cryptopp/aes.h"
#include "cryptopp/gcm.h"

#include "sizebounded/sizebounded.hpp"

#endif

import lxr_aes;
import lxr_key96;
import lxr_key256;


module lxr_aes_gcm;


#if CRYPTOLIB == CRYPTOPP

namespace lxr {

// Crypto++ throws on errors; the OCaml stubs are built with -fno-exceptions,
// so nothing may escape from here.
struct AesGcm::pimpl {
    std::unique_ptr<CryptoPP::AuthenticatedSymmetricCipher> _cipher;
    bool _ok {false};
    bool _started {false};   // process() was called
    bool _finished {false};
    bool _hastag {false};
    unsigned char _tag[AesGcm::tagsz];
};

AesGcm::~AesGcm() = default;

AesGcm::AesGcm()
    : _pimpl(new AesGcm::pimpl)
{
}

bool AesGcm::aad(unsigned char const *buf, int len)
{
    if (! _pimpl->_ok || _pimpl->_started || _pimpl->_finished || len < 0) { return false; }
    if (len == 0) { return true; }
    if (! buf) { return false; }
    try {
        _pimpl->_cipher->Update(buf, len);
        return true;
    } catch (...) {
        return false;
    }
}

static bool init(AesGcm::pimpl & p, bool enc, Key256 const & k, Key96 const & nonce)
{
    try {
        if (enc) { p._cipher.reset(new CryptoPP::GCM<CryptoPP::AES>::Encryption); }
        else { p._cipher.reset(new CryptoPP::GCM<CryptoPP::AES>::Decryption); }
        p._cipher->SetKeyWithIV(k.bytes(), k.length() / 8, nonce.bytes(), nonce.length() / 8);
        return true;
    } catch (...) {
        return false;
    }
}

static int update(AesGcm::pimpl & p, int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf)
{
    if (! p._ok || p._finished || inlen < 0 || inlen > (int)AesGcm::datasz) { return -1; }
    p._started = true;
    if (inlen == 0) { return 0; }
    try {
        unsigned char *buf = (unsigned char*)inoutbuf.ptr();
        p._cipher->ProcessData(buf, buf, inlen);
        memset(buf + inlen, 0, AesGcm::datasz - inlen);
        return inlen;
    } catch (...) {
        return -1;
    }
}

AesGcmEncrypt::AesGcmEncrypt(Key256 const & k, Key96 const & nonce)
    : AesGcm()
{
    _pimpl->_ok = init(*_pimpl, true, k, nonce);
}

int AesGcmEncrypt::process(int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf)
{
    return update(*_pimpl, inlen, inoutbuf);
}

int AesGcmEncrypt::finish(int inpos, sizebounded<unsigned char, AesGcm::datasz> & outbuf)
{
    if (! _pimpl->_ok || _pimpl->_finished) { return -1; }
    _pimpl->_finished = true;
    try {
        _pimpl->_cipher->TruncatedFinal(_pimpl->_tag, AesGcm::tagsz);
    } catch (...) {
        return -1;
    }
    _pimpl->_hastag = true;
    return 0;
}

bool AesGcmEncrypt::tag(unsigned char out[AesGcm::tagsz]) const
{
    if (! _pimpl->_hastag) { return false; }
    memcpy(out, _pimpl->_tag, AesGcm::tagsz);
    return true;
}

AesGcmDecrypt::AesGcmDecrypt(Key256 const & k, Key96 const & nonce)
    : AesGcm()
{
    _pimpl->_ok = init(*_pimpl, false, k, nonce);
}

int AesGcmDecrypt::process(int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf)
{
    return update(*_pimpl, inlen, inoutbuf);
}

bool AesGcmDecrypt::tag(unsigned char const in[AesGcm::tagsz])
{
    if (! _pimpl->_ok || _pimpl->_finished) { return false; }
    memcpy(_pimpl->_tag, in, AesGcm::tagsz);
    _pimpl->_hastag = true;
    return true;
}

int AesGcmDecrypt::finish(int inpos, sizebounded<unsigned char, AesGcm::datasz> & outbuf)
{
    if (! _pimpl->_ok || _pimpl->_finished || ! _pimpl->_hastag) { return -1; }
    _pimpl->_finished = true;
    try {
        return _pimpl->_cipher->TruncatedVerify(_pimpl->_tag, AesGcm::tagsz) ? 0 : -1;
    } catch (...) {
        return -1;
    }
}

} // namespace

#endif
