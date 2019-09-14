declared in [Md5](md5.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
lxr::Key128* hash_Md5(int len, const char * inbuf)
{
    lxr::Key128 k = lxr::Md5::hash(inbuf, len);
    return new lxr::Key128(k);
}

```
