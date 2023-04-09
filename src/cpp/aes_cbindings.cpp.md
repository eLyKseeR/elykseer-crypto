declared in [Aes](aes.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CAesEncrypt* mk_AesEncrypt(CKey256 * k, CKey128 * iv)
{ auto r = new lxr::AesEncrypt(*((lxr::Key256*)k->ptr), *((lxr::Key128*)iv->ptr));
  CAesEncrypt *cl = new CAesEncrypt; cl->ptr = r;
  cl->lastpos = 0;
  return cl;
}

extern "C" EXPORT
unsigned int sz_AesEncrypt()
{   return lxr::Aes::datasz;
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
int proc_AesEncrypt(CAesEncrypt *cl, unsigned int inlen, unsigned char const *inbuf)
{   if (inlen > lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    std::memcpy((void*)buf.ptr(), inbuf, std::min(inlen, lxr::Aes::datasz));
    int len = ((lxr::AesEncrypt*)(cl->ptr))->process(inlen, buf);
    std::memcpy(cl->buf+cl->lastpos, buf.ptr(), len);
    cl->lastpos += len;
    return len;
}

extern "C" EXPORT
int fin_AesEncrypt(CAesEncrypt *cl)
{   sizebounded<unsigned char, lxr::Aes::datasz> buf;
    int len = ((lxr::AesEncrypt*)(cl->ptr))->finish(0, buf);
    std::memcpy(cl->buf+cl->lastpos, buf.ptr(), len);
    cl->lastpos += len;
    return len;
}

extern "C" EXPORT
unsigned int len_AesEncrypt(CAesEncrypt *cl)
{   return cl->lastpos;
}

ATTRIBUTE_NO_SANITIZE_ADDRESS
extern "C" EXPORT
unsigned int copy_AesEncrypt(CAesEncrypt *cl, unsigned int outlen, unsigned char *outbuf)
{   unsigned int copied = std::min(outlen, cl->lastpos);
    memcpy(outbuf, cl->buf, copied);
    if (copied < cl->lastpos) {
      // copied 'copied' bytes out; move from 'copied' to lastpos to front
      memcpy(cl->buf, cl->buf + copied, cl->lastpos - copied);
    }
    cl->lastpos -= copied;
    return copied;
}

extern "C" EXPORT
CAesDecrypt* mk_AesDecrypt(CKey256 * k, CKey128 * iv)
{ auto r = new lxr::AesDecrypt(*((lxr::Key256*)k->ptr), *((lxr::Key128*)iv->ptr));
  CAesDecrypt *cl = new CAesDecrypt; cl->ptr = r;
  cl->lastpos = 0;
  return cl;
}

extern "C" EXPORT
unsigned int sz_AesDecrypt()
{   return lxr::Aes::datasz;
}

extern "C" EXPORT
void release_AesDecrypt(CAesDecrypt *cl)
{
  if (cl) {
    if (cl->ptr) {
        free(cl->ptr);
    }
    delete cl;
  }
}

extern "C" EXPORT
int proc_AesDecrypt(CAesDecrypt *cl, unsigned int inlen, unsigned char const *inbuf)
{   if (inlen > lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    std::memcpy((void*)buf.ptr(), inbuf, std::min(inlen, lxr::Aes::datasz));
    int len = ((lxr::AesDecrypt*)(cl->ptr))->process(inlen, buf);
    std::memcpy(cl->buf+cl->lastpos, buf.ptr(), len);
    cl->lastpos += len;
    return len;
}

extern "C" EXPORT
int fin_AesDecrypt(CAesDecrypt *cl)
{   sizebounded<unsigned char, lxr::Aes::datasz> buf;
    int len = ((lxr::AesDecrypt*)(cl->ptr))->finish(0, buf);
    std::memcpy(cl->buf+cl->lastpos, buf.ptr(), len);
    cl->lastpos += len;
    return len;
}

extern "C" EXPORT
unsigned int len_AesDecrypt(CAesDecrypt *cl)
{   return cl->lastpos;
}

ATTRIBUTE_NO_SANITIZE_ADDRESS
extern "C" EXPORT
unsigned int copy_AesDecrypt(CAesDecrypt *cl, unsigned int outlen, unsigned char *outbuf)
{   unsigned int copied = std::min(outlen, cl->lastpos);
    memcpy(outbuf, cl->buf, copied);
    if (copied < cl->lastpos) {
      // copied 'copied' bytes out; move from 'copied' to lastpos to front
      memcpy(cl->buf, cl->buf + copied, cl->lastpos - copied);
    }
    cl->lastpos -= copied;
    return copied;
}

```
