declared in [Key256](key256.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
lxr::Key256* mk_Key256()
{ return new lxr::Key256(); }

extern "C" EXPORT
int len_Key256(lxr::Key256* k)
{ return k->length(); }

extern "C" EXPORT
char* bytes_Key256(lxr::Key256* k)
{ const unsigned char *b = k->bytes();
  return strdup((const char*)b); }

extern "C" EXPORT
char* tohex_Key256(lxr::Key256* k)
{ auto h = k->toHex();
  return strdup(h.c_str()); }

```
