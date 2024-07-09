```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include <cassert>

#include "lxr/hmac.hpp"

#if CRYPTOLIB == OPENSSL
// #include "openssl/hmac.h"
#error to-be-done
#endif
#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/hmac.h"
#include "cryptopp/sha.h"
#include "cryptopp/sha3.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"
#endif

namespace lxr {

````
