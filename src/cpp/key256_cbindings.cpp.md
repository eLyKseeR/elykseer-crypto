declared in [Key256](key256.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey256* mk_Key256()
{ auto k = new lxr::Key256();
  CKey256 * r = new CKey256;
  r->ptr = k;
  return r;
}

extern "C" EXPORT
int len_Key256(CKey256 * k)
{ return ((lxr::Key256*)k->ptr)->length(); }

extern "C" EXPORT
char* bytes_Key256(CKey256 * k)
{ const unsigned char *b = ((lxr::Key256*)k->ptr)->bytes();
  return (char*)b; }

extern "C" EXPORT
char* tohex_Key256(CKey256 * k)
{ auto h = ((lxr::Key256*)k->ptr)->toHex();
  return strdup(h.c_str()); }

```
