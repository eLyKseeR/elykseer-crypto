declared in [Sha3](sha3.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey256* hash_Sha3_256(int len, const char * inbuf)
{
    auto h = lxr::Sha3_256::hash(inbuf, len);
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

extern "C" EXPORT
CKey256* filehash_Sha3_256(const char * fp)
{
    auto h = lxr::Sha3_256::hash(std::filesystem::path(fp));
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

```
