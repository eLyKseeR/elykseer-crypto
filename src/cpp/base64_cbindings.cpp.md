declared in [Base64](base64.hpp.md)

```c++
#include "lxr/lxr-cbindings.hpp"

extern "C" EXPORT
int base64_encode(int mlen, const char * m, int outlen, unsigned char * out)
{
    auto tr = lxr::Base64::encode(std::string(m, mlen));
    int sz = tr.size();
    unsigned int copied = std::min(outlen, sz);
    std::memcpy(out, tr.c_str(), copied);
    return copied;
}

extern "C" EXPORT
int base64_decode(int mlen, const char * m, int outlen, unsigned char * out)
{
    auto tr = lxr::Base64::decode(std::string(m, mlen));
    int sz = tr.size();
    unsigned int copied = std::min(outlen, sz);
    std::memcpy(out, tr.c_str(), copied);
    return copied;
}

```
