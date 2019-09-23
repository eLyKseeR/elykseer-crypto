declared in [Md5](md5.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey128 hash_Md5(int len, const char * inbuf)
{
    auto k = lxr::Md5::hash(inbuf, len);
    CKey128 ck; ck.ptr = new lxr::Key128(k);
    return ck;
}

extern "C" EXPORT
char* shash_Md5(int len, const char * inbuf)
{
    auto k = lxr::Md5::hash(inbuf, len);
    CKey128 ck; ck.ptr = new lxr::Key128(k);
    return tohex_Key128(ck);
}

```
