```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include <cassert>
#include <functional>

#include "lxr/sha256.hpp"
#include "sizebounded/sizebounded.ipp"

#if CRYPTOLIB == OPENSSL
#include "openssl/evp.h"
#include "openssl/sha.h"

struct lxr::Sha256::pimpl {
    const EVP_MD *md = EVP_sha2_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);
};

#endif

#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/sha.h"

struct lxr::Sha256::pimpl {
    CryptoPP::SHA256 hash;
};

#endif

#include <fstream>

namespace lxr {

````
