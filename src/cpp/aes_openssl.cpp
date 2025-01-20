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

#error AES using OpenSSL needs a rework!
#include "openssl/conf.h"
#include "openssl/evp.h"
#include "openssl/err.h"

#endif

#include "sizebounded/sizebounded.ipp"

import lxr_key128;
import lxr_key256;


module lxr_aes;


#if CRYPTOLIB == OPENSSL

namespace lxr {

struct Aes::pimpl {
    pimpl() {};
    ~pimpl() { if (_ctx) { EVP_CIPHER_CTX_free(_ctx); _ctx = NULL; } };
    EVP_CIPHER_CTX *_ctx {nullptr};
    int _len {0};
};

Aes::~Aes()
{
    if (_pimpl) {
        _pimpl.reset();
    }
}

Aes::Aes()
    : _pimpl(new Aes::pimpl)
{
    EVP_add_cipher(EVP_aes_256_cbc());
    _pimpl->_ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(_pimpl->_ctx);
}

AesEncrypt::AesEncrypt(Key256 const & k, Key128 const & iv)
    : Aes()
{
    if (_pimpl->_ctx) {
        if (EVP_EncryptInit(_pimpl->_ctx, EVP_aes_256_cbc(), (const unsigned char *)k.bytes(),
                               (const unsigned char *)iv.bytes()) != 1)
        {
            EVP_CIPHER_CTX_free(_pimpl->_ctx);
            _pimpl->_ctx = NULL;
            std::clog << "failed to init encryption!" << std::endl;
        }
    }
}

int AesEncrypt::process(int inlen, sizebounded<unsigned char, Aes::datasz> & inoutbuf)
{
    if (! _pimpl->_ctx) { return -1; }
    int len = 0;
    unsigned char tbuf[Aes::datasz];
    try {
        if (EVP_EncryptUpdate(_pimpl->_ctx, &tbuf[0], &len, inoutbuf.ptr(), inlen) == 1) {
        inoutbuf.transform([&len,&tbuf](const int i, const char c)->char {
              if (i < len) { return tbuf[i]; }
              else { return '\0'; }
          });
        } else {
          std::clog << "failed to update encryption!" << std::endl;
        }
    } catch (std::exception & e) {
        std::clog << "failed to update encryption! " << e.what() << std::endl;
    }
    return len;
}

int AesEncrypt::finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf)
{
    if (! _pimpl->_ctx) { return -1; }
    int len = 0;
    unsigned char tbuf[Aes::datasz];
    if (EVP_EncryptFinal(_pimpl->_ctx, tbuf, &len) != 1) {
         len = -1;
    }
    outbuf.transform([&inpos,&len,&tbuf](const int i, const char c)->char {
        if (i >= inpos && i < len+inpos) { return tbuf[i]; }
        else { return '\0'; }
    });
    EVP_CIPHER_CTX_free(_pimpl->_ctx);
    _pimpl->_ctx = NULL;
    return len;
}

AesDecrypt::AesDecrypt(Key256 const & k, Key128 const & iv)
    : Aes()
{
    if (_pimpl->_ctx && 
        EVP_DecryptInit(_pimpl->_ctx, EVP_aes_256_cbc(), k.bytes(), iv.bytes()) != 1)
    {
        EVP_CIPHER_CTX_free(_pimpl->_ctx);
        _pimpl->_ctx = NULL;
    }
}

int AesDecrypt::process(int inlen, sizebounded<unsigned char, Aes::datasz> & outbuf)
{
    if (! _pimpl->_ctx) { return -1; }
    int len = 0;
    unsigned char tbuf[Aes::datasz];
    if (EVP_DecryptUpdate(_pimpl->_ctx, tbuf, &len, outbuf.ptr(), inlen) != 1) {
        throw "AesDecrypt::process failed";
    }
    return len;
}

int AesDecrypt::finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf)
{
    if (! _pimpl->_ctx) { return -1; }
    int len = 0;
    if (EVP_DecryptFinal(_pimpl->_ctx, (unsigned char*)outbuf.ptr()+inpos, &len) != 1) {
        len = -1;
    }
    EVP_CIPHER_CTX_free(_pimpl->_ctx);
    _pimpl->_ctx = NULL;
    return len;
}

} // namespace

#endif