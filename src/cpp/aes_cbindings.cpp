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

#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

#include "lxr-cbindings.hpp"

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

import lxr_key128;
import lxr_key256;


module lxr_aes;


extern "C" EXPORT
long cpp_process_aes256(int direction, CKey128 *iv, CKey256 *k, void *buf, int blen, int dlen)
{
    std::unique_ptr<lxr::Aes> _aes;
    if (direction == 1)
        _aes.reset(new lxr::AesEncrypt(*(lxr::Key256*)(k->ptr), *(lxr::Key128*)(iv->ptr)));
    else
        _aes.reset(new lxr::AesDecrypt(*(lxr::Key256*)(k->ptr), *(lxr::Key128*)(iv->ptr)));

    sizebounded<unsigned char, lxr::Aes::datasz> sbuffer;
    std::string cipher;
    unsigned int tot_len = std::min(blen, dlen);
    unsigned int proc_len = 0;
    unsigned int copy_len = 0;
    unsigned int tot_proc = 0;
    unsigned int lenc = 0;

    while (proc_len < tot_len) {
      copy_len = std::min(lxr::Aes::datasz, tot_len - proc_len);
      memset((char*)sbuffer.ptr(), 0, lxr::Aes::datasz);
      memcpy((char*)sbuffer.ptr(), (char*)buf + proc_len, copy_len);
      proc_len += copy_len;

      lenc = _aes->process(copy_len, sbuffer);
      tot_proc += lenc;
      cipher += std::string((const char*)sbuffer.ptr(), lenc);
    }
    lenc = _aes->finish(0, sbuffer);
    tot_proc += lenc;
    if (lenc > 0) {
      cipher += std::string((const char*)sbuffer.ptr(), lenc); }
    // std::clog << "encrypted " << lenc << " bytes." << std::endl;
    if (tot_proc > (unsigned int)blen) return -1;
    memcpy(buf, cipher.data(), tot_proc);

    return tot_proc;
}

extern "C" EXPORT
CAesEncrypt* mk_AesEncrypt(CKey256 *k, CKey128 *iv)
{
  auto r = new lxr::AesEncrypt(*((lxr::Key256*)k->ptr), *((lxr::Key128*)iv->ptr));
  CAesEncrypt *cl = new CAesEncrypt; cl->ptr = r;
  cl->totproc = 0;
  return cl;
}

extern "C" EXPORT
void release_AesEncrypt(CAesEncrypt *cl)
{
  if (cl) {
    if (cl->ptr) {
        free(cl->ptr);
    }
    delete cl;
  }
}

extern "C" EXPORT
int proc_AesEncrypt(CAesEncrypt *cl, unsigned int inlen, unsigned char *inoutbuf)
{
  if (inlen > lxr::Aes::datasz) return (-1);
  sizebounded<unsigned char, lxr::Aes::datasz> buf;
  std::memcpy((void*)buf.ptr(), inoutbuf, inlen);
  int len = ((lxr::AesEncrypt*)(cl->ptr))->process(inlen, buf);
  if (len > 0) {
    std::memcpy(inoutbuf, buf.ptr(), len);
    cl->totproc += len;
  }
  return len;
}

extern "C" EXPORT
int fin_AesEncrypt(CAesEncrypt *cl, unsigned char *outbuf)
{
  sizebounded<unsigned char, lxr::Aes::datasz> buf;
  int len = ((lxr::AesEncrypt*)(cl->ptr))->finish(0, buf);
  if (len > 0) {
    std::memcpy(outbuf, buf.ptr(), len);
    cl->totproc += len;
  }
  return len;
}

extern "C" EXPORT
CAesDecrypt* mk_AesDecrypt(CKey256 * k, CKey128 * iv)
{
  auto r = new lxr::AesDecrypt(*((lxr::Key256*)k->ptr), *((lxr::Key128*)iv->ptr));
  CAesDecrypt *cl = new CAesDecrypt; cl->ptr = r;
  cl->totproc = 0;
  return cl;
}

extern "C" EXPORT
void release_AesDecrypt(CAesDecrypt *cl)
{
  if (cl) {
    if (cl->ptr) {
        delete (lxr::AesDecrypt*)(cl->ptr);
    }
    delete cl;
  }
}

extern "C" EXPORT
int proc_AesDecrypt(CAesDecrypt *cl, unsigned int inlen, unsigned char *inoutbuf)
{
  if (inlen > lxr::Aes::datasz) return (-1);
  sizebounded<unsigned char, lxr::Aes::datasz> buf;
  std::memcpy((void*)buf.ptr(), inoutbuf, inlen);
  int len = ((lxr::AesDecrypt*)(cl->ptr))->process(inlen, buf);
  if (len > 0) {
    std::memcpy(inoutbuf, buf.ptr(), len);
    cl->totproc += len;
  }
  return len;
}

extern "C" EXPORT
int fin_AesDecrypt(CAesDecrypt *cl, unsigned char *outbuf)
{
  sizebounded<unsigned char, lxr::Aes::datasz> buf;
  int len = ((lxr::AesDecrypt*)(cl->ptr))->finish(0, buf);
  if (len > 0) {
    std::memcpy(outbuf, buf.ptr(), len);
    cl->totproc += len;
  }
  return len;
}

// Thin wrappers with C linkage that accept sizebounded* as void*.
// These let non-module callers (e.g. OCaml stubs) drive the cipher
// directly without going through the CAesEncrypt/CAesDecrypt layer,
// avoiding the extra memcpy that the proc_*/fin_* functions incur.

extern "C" EXPORT
void* lxr_aes_new_encrypt(void* key256_ptr, void* key128_ptr)
{
  return new lxr::AesEncrypt(
    *static_cast<lxr::Key256*>(key256_ptr),
    *static_cast<lxr::Key128*>(key128_ptr));
}

extern "C" EXPORT
void lxr_aes_delete_encrypt(void* p)
{
  delete static_cast<lxr::AesEncrypt*>(p);
}

extern "C" EXPORT
int lxr_aes_process_encrypt(void* p, int inlen, void* sb)
{
  auto& buf = *static_cast<sizebounded<unsigned char, lxr::Aes::datasz>*>(sb);
  return static_cast<lxr::AesEncrypt*>(p)->process(inlen, buf);
}

extern "C" EXPORT
int lxr_aes_finish_encrypt(void* p, void* sb)
{
  auto& buf = *static_cast<sizebounded<unsigned char, lxr::Aes::datasz>*>(sb);
  return static_cast<lxr::AesEncrypt*>(p)->finish(0, buf);
}

extern "C" EXPORT
void* lxr_aes_new_decrypt(void* key256_ptr, void* key128_ptr)
{
  return new lxr::AesDecrypt(
    *static_cast<lxr::Key256*>(key256_ptr),
    *static_cast<lxr::Key128*>(key128_ptr));
}

extern "C" EXPORT
void lxr_aes_delete_decrypt(void* p)
{
  delete static_cast<lxr::AesDecrypt*>(p);
}

extern "C" EXPORT
int lxr_aes_process_decrypt(void* p, int inlen, void* sb)
{
  auto& buf = *static_cast<sizebounded<unsigned char, lxr::Aes::datasz>*>(sb);
  return static_cast<lxr::AesDecrypt*>(p)->process(inlen, buf);
}

extern "C" EXPORT
int lxr_aes_finish_decrypt(void* p, void* sb)
{
  auto& buf = *static_cast<sizebounded<unsigned char, lxr::Aes::datasz>*>(sb);
  return static_cast<lxr::AesDecrypt*>(p)->finish(0, buf);
}
