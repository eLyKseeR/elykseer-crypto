```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include <cassert>
#include <string>

#include "lxr/base64.hpp"

#if CRYPTOLIB == OPENSSL
#error to-be-done
#endif
#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/base64.h"
#endif

namespace lxr {

````
