declared in [Key128](key128.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey128 mk_Key128()
{ auto k = new lxr::Key128();
  CKey128 v; v.ptr = k;
  return v;
}

extern "C" EXPORT
int len_Key128(CKey128 k)
{ return k.ptr->length(); }

extern "C" EXPORT
char* bytes_Key128(CKey128 k)
{ const unsigned char *b = k.ptr->bytes();
  return (char*)b; }

extern "C" EXPORT
char* tohex_Key128(CKey128 k)
{ auto h = k.ptr->toHex();
  return (char*)h.c_str(); }

```
