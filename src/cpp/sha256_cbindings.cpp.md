declared in [Sha256](sha256.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey256* hash_Sha256(int len, const char * inbuf)
{
    auto h = lxr::Sha256::hash(inbuf, len);
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

extern "C" EXPORT
CKey256* filehash_Sha256(const char * fp)
{
    auto h = lxr::Sha256::hash(std::filesystem::path(fp));
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

```
