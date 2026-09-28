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

#include <algorithm>
#include <cstring>
#include <memory>
#include <string>

#include "lxr-cbindings.hpp"

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

import lxr_key96;
import lxr_key256;


module lxr_aes_gcm;


// nothing in here may throw: callers are C, and the OCaml stubs are built with -fno-exceptions

extern "C" EXPORT
long cpp_process_aes256_gcm(int direction, CKey96 *nonce, CKey256 *k, const void *aad, int aadlen, void *buf, int blen, int dlen)
{
    const bool enc = (direction == 1);
    if (!nonce || !k || !buf || blen < 0 || dlen < 0 || aadlen < 0 || (aadlen > 0 && !aad)) { return -1; }
    if (enc && blen < dlen + (int)lxr::AesGcm::tagsz) { return -1; }
    if (!enc && (dlen < (int)lxr::AesGcm::tagsz || dlen > blen)) { return -1; }

    try {
        std::unique_ptr<lxr::AesGcm> _aes;
        if (enc)
            _aes.reset(new lxr::AesGcmEncrypt(*(lxr::Key256*)(k->ptr), *(lxr::Key96*)(nonce->ptr)));
        else
            _aes.reset(new lxr::AesGcmDecrypt(*(lxr::Key256*)(k->ptr), *(lxr::Key96*)(nonce->ptr)));

        if (!_aes->aad((const unsigned char*)aad, aadlen)) { if (!enc) memset(buf, 0, blen); return -1; }

        const unsigned int tot_len = enc ? dlen : dlen - lxr::AesGcm::tagsz;
        if (!enc) {
            if (!static_cast<lxr::AesGcmDecrypt*>(_aes.get())->tag((const unsigned char*)buf + tot_len)) {
                memset(buf, 0, blen);
                return -1;
            }
        }

        sizebounded<unsigned char, lxr::AesGcm::datasz> sbuffer;
        std::string out;
        out.reserve(tot_len + lxr::AesGcm::tagsz);
        unsigned int proc_len = 0;
        bool ok = true;

        while (ok && proc_len < tot_len) {
            const unsigned int copy_len = std::min(lxr::AesGcm::datasz, tot_len - proc_len);
            memset((char*)sbuffer.ptr(), 0, lxr::AesGcm::datasz);
            memcpy((char*)sbuffer.ptr(), (const char*)buf + proc_len, copy_len);
            proc_len += copy_len;

            const int lenc = _aes->process(copy_len, sbuffer);
            if (lenc < 0) { ok = false; break; }
            out.append((const char*)sbuffer.ptr(), lenc);
        }
        if (ok && _aes->finish(0, sbuffer) < 0) { ok = false; }
        if (ok && enc) {
            unsigned char tag[lxr::AesGcm::tagsz];
            ok = static_cast<lxr::AesGcmEncrypt*>(_aes.get())->tag(tag);
            if (ok) { out.append((const char*)tag, sizeof(tag)); }
        }
        if (!ok || out.size() > (size_t)blen) {
            if (!enc) { memset(buf, 0, blen); }
            return -1;
        }
        memcpy(buf, out.data(), out.size());
        return out.size();
    } catch (...) {
        if (direction != 1 && buf && blen > 0) { memset(buf, 0, blen); }
        return -1;
    }
}

extern "C" EXPORT
CAesGcmEncrypt* mk_AesGcmEncrypt(CKey256 *k, CKey96 *nonce)
{
  if (!k || !nonce) return nullptr;
  try {
    auto r = new lxr::AesGcmEncrypt(*((lxr::Key256*)k->ptr), *((lxr::Key96*)nonce->ptr));
    CAesGcmEncrypt *cl = new CAesGcmEncrypt; cl->ptr = r;
    cl->totproc = 0;
    return cl;
  } catch (...) { return nullptr; }
}

extern "C" EXPORT
void release_AesGcmEncrypt(CAesGcmEncrypt *cl)
{
  if (cl) {
    if (cl->ptr) {
        delete (lxr::AesGcmEncrypt*)(cl->ptr);
    }
    delete cl;
  }
}

extern "C" EXPORT
bool aad_AesGcmEncrypt(CAesGcmEncrypt *cl, const unsigned char *buf, int len)
{
  if (!cl || !cl->ptr) return false;
  return ((lxr::AesGcmEncrypt*)(cl->ptr))->aad(buf, len);
}

extern "C" EXPORT
int proc_AesGcmEncrypt(CAesGcmEncrypt *cl, unsigned int inlen, unsigned char *inoutbuf)
{
  if (!cl || !cl->ptr || inlen > lxr::AesGcm::datasz) return (-1);
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  std::memcpy((void*)buf.ptr(), inoutbuf, inlen);
  int len = ((lxr::AesGcmEncrypt*)(cl->ptr))->process(inlen, buf);
  if (len > 0) {
    std::memcpy(inoutbuf, buf.ptr(), len);
    cl->totproc += len;
  }
  return len;
}

extern "C" EXPORT
int fin_AesGcmEncrypt(CAesGcmEncrypt *cl, unsigned char *outbuf)
{
  if (!cl || !cl->ptr) return (-1);
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  return ((lxr::AesGcmEncrypt*)(cl->ptr))->finish(0, buf);
}

extern "C" EXPORT
bool tag_AesGcmEncrypt(CAesGcmEncrypt *cl, unsigned char out[16])
{
  if (!cl || !cl->ptr) return false;
  return ((lxr::AesGcmEncrypt*)(cl->ptr))->tag(out);
}

extern "C" EXPORT
CAesGcmDecrypt* mk_AesGcmDecrypt(CKey256 *k, CKey96 *nonce)
{
  if (!k || !nonce) return nullptr;
  try {
    auto r = new lxr::AesGcmDecrypt(*((lxr::Key256*)k->ptr), *((lxr::Key96*)nonce->ptr));
    CAesGcmDecrypt *cl = new CAesGcmDecrypt; cl->ptr = r;
    cl->totproc = 0;
    return cl;
  } catch (...) { return nullptr; }
}

extern "C" EXPORT
void release_AesGcmDecrypt(CAesGcmDecrypt *cl)
{
  if (cl) {
    if (cl->ptr) {
        delete (lxr::AesGcmDecrypt*)(cl->ptr);
    }
    delete cl;
  }
}

extern "C" EXPORT
bool aad_AesGcmDecrypt(CAesGcmDecrypt *cl, const unsigned char *buf, int len)
{
  if (!cl || !cl->ptr) return false;
  return ((lxr::AesGcmDecrypt*)(cl->ptr))->aad(buf, len);
}

extern "C" EXPORT
int proc_AesGcmDecrypt(CAesGcmDecrypt *cl, unsigned int inlen, unsigned char *inoutbuf)
{
  if (!cl || !cl->ptr || inlen > lxr::AesGcm::datasz) return (-1);
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  std::memcpy((void*)buf.ptr(), inoutbuf, inlen);
  int len = ((lxr::AesGcmDecrypt*)(cl->ptr))->process(inlen, buf);
  if (len > 0) {
    std::memcpy(inoutbuf, buf.ptr(), len);
    cl->totproc += len;
  }
  return len;
}

extern "C" EXPORT
int fin_AesGcmDecrypt(CAesGcmDecrypt *cl, unsigned char *outbuf)
{
  if (!cl || !cl->ptr) return (-1);
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  return ((lxr::AesGcmDecrypt*)(cl->ptr))->finish(0, buf);
}

extern "C" EXPORT
bool settag_AesGcmDecrypt(CAesGcmDecrypt *cl, const unsigned char in[16])
{
  if (!cl || !cl->ptr) return false;
  return ((lxr::AesGcmDecrypt*)(cl->ptr))->tag(in);
}
