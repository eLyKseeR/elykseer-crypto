```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include <cassert>
#include <functional>

#include "lxr/sha3.hpp"
#include "sizebounded/sizebounded.ipp"

#if CRYPTOLIB == OPENSSL
#include "openssl/evp.h"
#include "openssl/sha.h"

struct lxr::Sha3_256::pimpl {
    const EVP_MD *md = EVP_sha3_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);
};

#endif

#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/sha3.h"

struct lxr::Sha3_256::pimpl {
    CryptoPP::SHA3_256 hash;
};

#endif

#include <fstream>

namespace lxr {

````
