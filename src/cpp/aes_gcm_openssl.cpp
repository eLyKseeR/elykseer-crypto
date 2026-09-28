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

#if CRYPTOLIB == OPENSSL

#include <cstring>
#include <memory>

#include "openssl/evp.h"

#include "sizebounded/sizebounded.ipp"

#endif

import lxr_aes;
import lxr_key96;
import lxr_key256;


module lxr_aes_gcm;


#if CRYPTOLIB == OPENSSL

namespace lxr {

struct AesGcm::pimpl {
    pimpl() {};
    ~pimpl() { if (_ctx) { EVP_CIPHER_CTX_free(_ctx); _ctx = NULL; } };
    EVP_CIPHER_CTX *_ctx {nullptr};
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
    _pimpl->_ctx = EVP_CIPHER_CTX_new();
}

static bool init(AesGcm::pimpl & p, bool enc, Key256 const & k, Key96 const & nonce)
{
    if (! p._ctx) { return false; }
    if (EVP_CipherInit_ex(p._ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr, enc ? 1 : 0) != 1) { return false; }
    if (EVP_CIPHER_CTX_ctrl(p._ctx, EVP_CTRL_GCM_SET_IVLEN, nonce.length() / 8, nullptr) != 1) { return false; }
    return EVP_CipherInit_ex(p._ctx, nullptr, nullptr, k.bytes(), nonce.bytes(), enc ? 1 : 0) == 1;
}

bool AesGcm::aad(unsigned char const *buf, int len)
{
    if (! _pimpl->_ok || _pimpl->_started || _pimpl->_finished || len < 0) { return false; }
    if (len == 0) { return true; }
    if (! buf) { return false; }
    int outl = 0;
    // same call for both directions
    return EVP_CipherUpdate(_pimpl->_ctx, nullptr, &outl, buf, len) == 1;
}

static int update(AesGcm::pimpl & p, int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf)
{
    if (! p._ok || p._finished || inlen < 0 || inlen > (int)AesGcm::datasz) { return -1; }
    p._started = true;
    if (inlen == 0) { return 0; }
    int len = 0;
    unsigned char tbuf[AesGcm::datasz + EVP_MAX_BLOCK_LENGTH];
    if (EVP_CipherUpdate(p._ctx, tbuf, &len, inoutbuf.ptr(), inlen) != 1 || len < 0 || len > (int)AesGcm::datasz) {
        return -1;
    }
    unsigned char *out = (unsigned char*)inoutbuf.ptr();
    memcpy(out, tbuf, len);
    memset(out + len, 0, AesGcm::datasz - len);
    return len;
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
    int len = 0;
    unsigned char tbuf[EVP_MAX_BLOCK_LENGTH];
    if (EVP_EncryptFinal_ex(_pimpl->_ctx, tbuf, &len) != 1) { return -1; }
    if (EVP_CIPHER_CTX_ctrl(_pimpl->_ctx, EVP_CTRL_GCM_GET_TAG, AesGcm::tagsz, _pimpl->_tag) != 1) { return -1; }
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
    if (EVP_CIPHER_CTX_ctrl(_pimpl->_ctx, EVP_CTRL_GCM_SET_TAG, AesGcm::tagsz, _pimpl->_tag) != 1) { return -1; }
    int len = 0;
    unsigned char tbuf[EVP_MAX_BLOCK_LENGTH];
    if (EVP_DecryptFinal_ex(_pimpl->_ctx, tbuf, &len) != 1) { return -1; }
    return 0;
}

} // namespace

#endif
