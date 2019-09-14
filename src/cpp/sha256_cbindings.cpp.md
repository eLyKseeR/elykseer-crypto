declared in [Sha256](sha256.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
lxr::Key256* hash_Sha256(int len, const char * inbuf)
{
    lxr::Key256 k = lxr::Sha256::hash(inbuf, len);
    return new lxr::Key256(k);
}

extern "C" EXPORT
lxr::Key256* filehash_Sha256(const char * fp)
{
    lxr::Key256 k = lxr::Sha256::hash(boost::filesystem::path(fp));
    return new lxr::Key256(k);
}

```
