declared in [Md5](md5.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey128* hash_Md5(int len, const char * inbuf)
{
    auto h = lxr::Md5::hash(inbuf, len);
    CKey128 * r = new CKey128; r->ptr = new lxr::Key128(h);
    return r;
}

```
