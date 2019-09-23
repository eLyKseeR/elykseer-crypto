declared in [Aes](aes.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
lxr::AesEncrypt* mk_AesEncrypt(CKey256 k, CKey128 iv)
{ return new lxr::AesEncrypt(*k.ptr, *iv.ptr); }

extern "C" EXPORT
int proc_AesEncrypt(lxr::AesEncrypt *cl, int inlen, unsigned char *inoutbuf)
{   if (inlen > lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    std::memcpy((void*)buf.ptr(), inoutbuf, std::min(inlen, lxr::Aes::datasz));
    int len = cl->process(inlen, buf);
    std::memcpy(inoutbuf, buf.ptr(), len);
    return len;
}

extern "C" EXPORT
int fin_AesEncrypt(lxr::AesEncrypt *cl, int outlen, unsigned char *outbuf)
{   if (outlen < lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    int len = cl->finish(0, buf);
    std::memcpy(outbuf, buf.ptr(), len);
    return len;
}

extern "C" EXPORT
lxr::AesDecrypt* mk_AesDecrypt(CKey256 k, CKey128 iv)
{ return new lxr::AesDecrypt(*k.ptr, *iv.ptr); }

extern "C" EXPORT
int proc_AesDecrypt(lxr::AesDecrypt *cl, int inlen, unsigned char *inoutbuf)
{   if (inlen > lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    std::memcpy((void*)buf.ptr(), inoutbuf, std::min(inlen, lxr::Aes::datasz));
    int len = cl->process(inlen, buf);
    std::memcpy(inoutbuf, buf.ptr(), len);
    return len;
}

extern "C" EXPORT
int fin_AesDecrypt(lxr::AesDecrypt *cl, int outlen, unsigned char *outbuf)
{   if (outlen < lxr::Aes::datasz) return (-1);
    sizebounded<unsigned char, lxr::Aes::datasz> buf;
    int len = cl->finish(0, buf);
    std::memcpy(outbuf, buf.ptr(), len);
    return len;
}

```
