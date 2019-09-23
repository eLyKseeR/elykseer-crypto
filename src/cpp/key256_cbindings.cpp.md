declared in [Key256](key256.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey256 mk_Key256()
{ auto k = new lxr::Key256();
  CKey256 v; v.ptr = k;
  return v;
}

extern "C" EXPORT
int len_Key256(CKey256 k)
{ return k.ptr->length(); }

extern "C" EXPORT
char* bytes_Key256(CKey256 k)
{ const unsigned char *b = k.ptr->bytes();
  return (char*)b; }

extern "C" EXPORT
char* tohex_Key256(CKey256 k)
{ auto h = k.ptr->toHex();
  return (char*)h.c_str(); }

```
