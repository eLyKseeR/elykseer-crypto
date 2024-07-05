declared in [HMAC](hmac.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey128* hmac_Md5(int klen, const char k[], int mlen, const char * m)
{
    auto h = lxr::HMAC::hmac_md5(k, klen, m, mlen);
    CKey128 * r = new CKey128; r->ptr = new lxr::Key128(h);
    return r;
}

extern "C" EXPORT
CKey256* hmac_Sha256(int klen, const char k[], int mlen, const char * m)
{
    auto h = lxr::HMAC::hmac_sha256(k, klen, m, mlen);
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

```
