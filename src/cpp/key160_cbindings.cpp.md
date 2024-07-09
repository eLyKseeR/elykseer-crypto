declared in [Key160](key160.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey160* mk_Key160()
{ auto k = new lxr::Key160;
  CKey160 * r = new CKey160;
  r->ptr = k;
  return r;
}

extern "C" EXPORT
void release_Key160(CKey160 * k)
{ if (k) {
    if (k->ptr) {
        delete (lxr::Key160*)k->ptr;
    }
    delete k;
  }
}

extern "C" EXPORT
int len_Key160(CKey160 * k)
{ return ((lxr::Key160*)k->ptr)->length(); }

extern "C" EXPORT
char* bytes_Key160(CKey160 * k)
{ const unsigned char *b = ((lxr::Key160*)k->ptr)->bytes();
  return (char*)b; }

extern "C" EXPORT
char* tohex_Key160(CKey160 * k)
{ auto h = ((lxr::Key160*)k->ptr)->toHex();
  return strdup(h.c_str()); }

extern "C" EXPORT
CKey160* fromhex_Key160(const char * s)
{ auto k = new lxr::Key160(true);
  std::string h = std::string(s, k->length()*2/8);
  k->fromHex(h);
  CKey160 * r = new CKey160;
  r->ptr = k;
  return r;
}

```
