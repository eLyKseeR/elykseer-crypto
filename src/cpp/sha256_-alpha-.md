```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include "lxr/sha256.hpp"

#if CRYPTOLIB == OPENSSL
#include "openssl/evp.h"
#include "openssl/sha.h"
#endif
#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/sha3.h"
#endif

#include <fstream>

namespace lxr {

````
