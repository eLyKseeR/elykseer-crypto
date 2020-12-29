```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include "lxr/sha256.hpp"

#if CRYPTOLIB == OPENSSL
#error SHA2 is deprecated! need to upgrade to SHA3 in OpenSSL
#include "openssl/sha.h"
#endif
#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/sha3.h"
#endif

#include <stdio.h>
#include <fstream>

namespace lxr {

````
