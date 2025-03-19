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
#include <memory>
#include <string>

#include "sizebounded/sizebounded.hpp"

import lxr_key128;
import lxr_key256;


export module lxr_aes;


export namespace lxr {

class Aes
{
   public:
      virtual ~Aes();
      static constexpr unsigned int datasz { 1024*4 };
      virtual int process(int inlen, sizebounded<unsigned char, Aes::datasz> & inoutbuf) = 0;
      virtual int finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf) = 0;
   protected:
      Aes();
      struct pimpl;
      std::unique_ptr<pimpl> _pimpl;
};

class AesDecrypt : public Aes
{
   public:
      AesDecrypt(Key256 const & k, Key128 const & iv);
      virtual ~AesDecrypt() = default;
      virtual int process(int inlen, sizebounded<unsigned char, Aes::datasz> & inoutbuf) override;
      virtual int finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf) override;
   private:
      AesDecrypt(AesDecrypt const &) = delete;
      AesDecrypt & operator=(AesDecrypt const &) = delete;
};

class AesEncrypt : public Aes
{
   public:
      AesEncrypt(Key256 const & k, Key128 const & iv);
      virtual ~AesEncrypt() = default;
      virtual int process(int inlen, sizebounded<unsigned char, Aes::datasz> & inoutbuf) override;
      virtual int finish(int inpos, sizebounded<unsigned char, Aes::datasz> & outbuf) override;
   private:
      AesEncrypt(AesEncrypt const &) = delete;
      AesEncrypt & operator=(AesEncrypt const &) = delete;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" {
export struct CAes {
   void *ptr;
   unsigned int totproc;
};

export struct CAesEncrypt : public CAes {
};

export CAesEncrypt* mk_AesEncrypt(CKey256 *k, CKey128 *iv);

export void release_AesEncrypt(CAesEncrypt *cl);

export int proc_AesEncrypt(CAesEncrypt *cl, unsigned int inlen, unsigned char *inoutbuf);

export int fin_AesEncrypt(CAesEncrypt *cl, unsigned char *outbuf);

export struct CAesDecrypt : public CAes {
};

export CAesDecrypt* mk_AesDecrypt(CKey256 *k, CKey128 *iv);

export void release_AesDecrypt(CAesDecrypt *cl);

export int proc_AesDecrypt(CAesDecrypt *cl, unsigned int inlen, unsigned char *inoutbuf);

export int fin_AesDecrypt(CAesDecrypt *cl, unsigned char *outbuf);

}