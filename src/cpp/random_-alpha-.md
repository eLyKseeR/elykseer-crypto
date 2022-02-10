```cpp
/*
````
<fpaste ../../src/copyright.md>
```cpp
*/

#include "lxr/random.hpp"

#include <math.h>

#if CRYPTOLIB == CRYPTOPP
#include <cryptopp/osrng.h>
#else
#error not yet defined
#endif

namespace lxr {

````
