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

#include <cstddef>
#include <memory>
#include <string>

#include "sizebounded/sizebounded.hpp"

import lxr_aes;
import lxr_key96;
import lxr_key256;


export module lxr_aes_gcm;


export namespace lxr {

/*
   AES-256-GCM (96-bit nonce, 128-bit tag); compatible with the Rust 'aes-gcm' crate
   and the OpenSSL tools in src/ml (ciphertext || tag).

   WARNING: the plaintext returned by AesGcmDecrypt::process() is UNAUTHENTICATED
   until finish() has returned >= 0. If finish() returns -1, all plaintext produced
   so far must be discarded.

   A nonce must be unique per key: never encrypt two messages with the same (key, nonce).
*/
class AesGcm
{
   public:
      virtual ~AesGcm();
      static constexpr unsigned int datasz { Aes::datasz };
      static constexpr unsigned int tagsz { 16 };
      // in-place: reads inlen bytes from inoutbuf, overwrites it with the output;
      // returns the number of output bytes, or -1 on error
      virtual int process(int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf) = 0;
      // completes the message; no padding, so nothing is appended to outbuf at inpos
      // and 0 is returned (decrypt: -1 if authentication failed)
      virtual int finish(int inpos, sizebounded<unsigned char, AesGcm::datasz> & outbuf) = 0;
      // additional authenticated data (not encrypted); may be called repeatedly
      // (data is concatenated) but only before the first process()
      bool aad(unsigned char const *buf, int len);
      struct pimpl;   // opaque; public only so that the backend helpers can name it
   protected:
      AesGcm();
      std::unique_ptr<pimpl> _pimpl;
   private:
      AesGcm(AesGcm const &) = delete;
      AesGcm & operator=(AesGcm const &) = delete;
};

class AesGcmEncrypt : public AesGcm
{
   public:
      AesGcmEncrypt(Key256 const & k, Key96 const & nonce);
      virtual ~AesGcmEncrypt() = default;
      virtual int process(int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf) override;
      virtual int finish(int inpos, sizebounded<unsigned char, AesGcm::datasz> & outbuf) override;
      // valid only after finish(); returns false before
      bool tag(unsigned char out[AesGcm::tagsz]) const;
};

class AesGcmDecrypt : public AesGcm
{
   public:
      AesGcmDecrypt(Key256 const & k, Key96 const & nonce);
      virtual ~AesGcmDecrypt() = default;
      virtual int process(int inlen, sizebounded<unsigned char, AesGcm::datasz> & inoutbuf) override;
      // returns -1 if the tag is missing or authentication fails
      virtual int finish(int inpos, sizebounded<unsigned char, AesGcm::datasz> & outbuf) override;
      // the expected tag; must be set before finish()
      bool tag(unsigned char const in[AesGcm::tagsz]);
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" {

// one-shot for OCaml. direction 1: encrypt, otherwise decrypt.
// encrypt: reads dlen plaintext bytes, writes ciphertext||tag (needs blen >= dlen+16), returns dlen+16.
// decrypt: reads dlen bytes of ciphertext||tag, returns dlen-16;
//          on authentication failure buf is zeroed and -1 is returned.
export long cpp_process_aes256_gcm(int direction, CKey96 *nonce, CKey256 *k, const void *aad, int aadlen, void *buf, int blen, int dlen);

export struct CAesGcm {
   void *ptr;
   unsigned int totproc;
};

export struct CAesGcmEncrypt : public CAesGcm {
};

export CAesGcmEncrypt* mk_AesGcmEncrypt(CKey256 *k, CKey96 *nonce);

export void release_AesGcmEncrypt(CAesGcmEncrypt *cl);

export bool aad_AesGcmEncrypt(CAesGcmEncrypt *cl, const unsigned char *buf, int len);

export int proc_AesGcmEncrypt(CAesGcmEncrypt *cl, unsigned int inlen, unsigned char *inoutbuf);

export int fin_AesGcmEncrypt(CAesGcmEncrypt *cl, unsigned char *outbuf);

export bool tag_AesGcmEncrypt(CAesGcmEncrypt *cl, unsigned char out[16]);

export struct CAesGcmDecrypt : public CAesGcm {
};

export CAesGcmDecrypt* mk_AesGcmDecrypt(CKey256 *k, CKey96 *nonce);

export void release_AesGcmDecrypt(CAesGcmDecrypt *cl);

export bool aad_AesGcmDecrypt(CAesGcmDecrypt *cl, const unsigned char *buf, int len);

export int proc_AesGcmDecrypt(CAesGcmDecrypt *cl, unsigned int inlen, unsigned char *inoutbuf);

// returns -1 if authentication fails
export int fin_AesGcmDecrypt(CAesGcmDecrypt *cl, unsigned char *outbuf);

export bool settag_AesGcmDecrypt(CAesGcmDecrypt *cl, const unsigned char in[16]);

}
