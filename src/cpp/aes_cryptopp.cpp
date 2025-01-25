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

#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"

#endif

#include "sizebounded/sizebounded.ipp"

import lxr_key128;
import lxr_key256;


module lxr_aes;


#if CRYPTOLIB == CRYPTOPP

namespace lxr {

struct Aes::pimpl {
    //pimpl() {};
    std::unique_ptr<CryptoPP::SymmetricCipher> _cipher;
    std::unique_ptr<CryptoPP::StreamTransformationFilter> _filter;
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
}

AesEncrypt::AesEncrypt(Key256 const & k, Key128 const & iv)
    : Aes()
{
    if (_pimpl) {
      _pimpl->_cipher.reset(new CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption);
      _pimpl->_cipher.get()->SetKeyWithIV(k.bytes(), k.length()/8, iv.bytes());
      _pimpl->_filter.reset(new CryptoPP::StreamTransformationFilter(*_pimpl->_cipher.get(), NULL, CryptoPP::BlockPaddingSchemeDef::NO_PADDING));
    }
}

int AesEncrypt::process(int inlen, sizebounded<unsigned char, Aes::datasz> & inoutbuf)
{
    if (! _pimpl->_filter) { return -1; }

    if (inlen > 0) {
      _pimpl->_filter.get()->Put(inoutbuf.ptr(), inlen); }

    if (!_pimpl->_filter.get()->AnyRetrievable()) {
      return 0; }

    size_t len = std::min((unsigned long)_pimpl->_filter.get()->MaxRetrievable(), (unsigned long)Aes::datasz);
    len = _pimpl->_filter.get()->Get((unsigned char*)inoutbuf.ptr(), len);

    return len;
}

int AesEncrypt::finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf)
{
    if (! _pimpl->_filter) { return -1; }
    _pimpl->_filter.get()->MessageEnd();

    if (!_pimpl->_filter.get()->AnyRetrievable()) {
      return 0; }

    size_t len = std::min((unsigned long)_pimpl->_filter.get()->MaxRetrievable(), (unsigned long)Aes::datasz - inpos);
    len = _pimpl->_filter.get()->Get((unsigned char*)outbuf.ptr() + inpos, len);

    return len;
}

AesDecrypt::AesDecrypt(Key256 const & k, Key128 const & iv)
    : Aes()
{
    if (_pimpl) {
      _pimpl->_cipher.reset(new CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption);
      _pimpl->_cipher.get()->SetKeyWithIV(k.bytes(), k.length()/8, iv.bytes());
      _pimpl->_filter.reset(new CryptoPP::StreamTransformationFilter(*_pimpl->_cipher.get(), NULL, CryptoPP::BlockPaddingSchemeDef::NO_PADDING));
    }
}

int AesDecrypt::process(int inlen, sizebounded<unsigned char, Aes::datasz> & inoutbuf)
{
    if (! _pimpl->_filter) { return -1; }

    if (inlen > 0) {
      _pimpl->_filter.get()->Put(inoutbuf.ptr(), inlen); }

    if (!_pimpl->_filter.get()->AnyRetrievable()) {
      return 0; }

    size_t len = std::min((unsigned long)_pimpl->_filter.get()->MaxRetrievable(), (unsigned long)Aes::datasz);
    len = _pimpl->_filter.get()->Get((unsigned char*)inoutbuf.ptr(), len);

    return len;
}

int AesDecrypt::finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf)
{
    if (! _pimpl->_filter) { return -1; }
    _pimpl->_filter.get()->MessageEnd();

    if (!_pimpl->_filter.get()->AnyRetrievable()) {
      return 0; }

    size_t len = std::min((unsigned long)_pimpl->_filter.get()->MaxRetrievable(), (unsigned long)Aes::datasz - inpos);
    len = _pimpl->_filter.get()->Get((unsigned char*)outbuf.ptr() + inpos, len);

    return len;
}

} // namespace

#endif