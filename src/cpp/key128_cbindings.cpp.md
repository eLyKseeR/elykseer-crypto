declared in [Key128](key128.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
CKey128* mk_Key128()
{ auto k = new lxr::Key128;
  CKey128 * r = new CKey128;
  r->ptr = k;
  return r;
}

extern "C" EXPORT
void release_Key128(CKey128 * k)
{ if (k) {
    if (k->ptr) {
        delete (lxr::Key128*)k->ptr;
    }
    delete k;
  }
}

extern "C" EXPORT
int len_Key128(CKey128 * k)
{ return ((lxr::Key128*)k->ptr)->length(); }

extern "C" EXPORT
char* bytes_Key128(CKey128 * k)
{ const unsigned char *b = ((lxr::Key128*)k->ptr)->bytes();
  return (char*)b; }

extern "C" EXPORT
char* tohex_Key128(CKey128 * k)
{ auto h = ((lxr::Key128*)k->ptr)->toHex();
  return strdup(h.c_str()); }

extern "C" EXPORT
CKey128* fromhex_Key128(const char * s)
{ auto k = new lxr::Key128(true);
  std::string h = std::string(s, k->length()*2/8);
  k->fromHex(h);
  CKey128 * r = new CKey128;
  r->ptr = k;
  return r;
}

```
