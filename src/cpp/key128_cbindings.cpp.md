declared in [Key128](key128.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
lxr::Key128* mk_Key128()
{ return new lxr::Key128(); }

extern "C" EXPORT
int len_Key128(lxr::Key128* k)
{ return k->length(); }

extern "C" EXPORT
char* bytes_Key128(lxr::Key128* k)
{ const unsigned char *b = k->bytes();
  return strdup((const char*)b); }

extern "C" EXPORT
char* tohex_Key128(lxr::Key128* k)
{ auto h = k->toHex();
  return strdup(h.c_str()); }

```
