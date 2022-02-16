```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include <cassert>

#include "lxr/md5.hpp"

#if CRYPTOLIB == OPENSSL
#include "openssl/md5.h"
#endif
#if CRYPTOLIB == CRYPTOPP
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"
#endif

namespace lxr {

````
