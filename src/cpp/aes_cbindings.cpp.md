declared in [Aes](aes.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CAesEncrypt* mk_AesEncrypt(CKey256 * k, CKey128 * iv)
{ auto cl = new lxr::AesEncrypt(*((lxr::Key256*)k->ptr), *((lxr::Key128*)iv->ptr));
  CAesEncrypt *r = new CAesEncrypt; r->ptr = cl;
  return r;
}

extern "C" EXPORT
int proc_AesEncrypt(CAesEncrypt *cl, int inlen, unsigned char *inoutbuf)
{   if (inlen > lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    std::memcpy((void*)buf.ptr(), inoutbuf, std::min(inlen, lxr::Aes::datasz));
    int len = ((lxr::AesEncrypt*)(cl->ptr))->process(inlen, buf);
    std::memcpy(inoutbuf, buf.ptr(), len);
    return len;
}

extern "C" EXPORT
int fin_AesEncrypt(CAesEncrypt *cl, int outlen, unsigned char *outbuf)
{   if (outlen < lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    int len = ((lxr::AesEncrypt*)(cl->ptr))->finish(0, buf);
    std::memcpy(outbuf, buf.ptr(), len);
    return len;
}

extern "C" EXPORT
CAesDecrypt* mk_AesDecrypt(CKey256 * k, CKey128 * iv)
{ auto cl = new lxr::AesDecrypt(*((lxr::Key256*)k->ptr), *((lxr::Key128*)iv->ptr));
  CAesDecrypt *r = new CAesDecrypt; r->ptr = cl;
  return r;
}

extern "C" EXPORT
int proc_AesDecrypt(CAesDecrypt *cl, int inlen, unsigned char *inoutbuf)
{   if (inlen > lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    std::memcpy((void*)buf.ptr(), inoutbuf, std::min(inlen, lxr::Aes::datasz));
    int len = ((lxr::AesDecrypt*)(cl->ptr))->process(inlen, buf);
    std::memcpy(inoutbuf, buf.ptr(), len);
    return len;
}

extern "C" EXPORT
int fin_AesDecrypt(CAesDecrypt *cl, int outlen, unsigned char *outbuf)
{   if (outlen < lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    int len = ((lxr::AesDecrypt*)(cl->ptr))->finish(0, buf);
    std::memcpy(outbuf, buf.ptr(), len);
    return len;
}

```
