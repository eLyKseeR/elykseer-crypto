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
