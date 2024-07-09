```cpp
/*
```
<fpaste ../../src/copyright.md>
```cpp
*/

#include "lxr/key160.hpp"
#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

#include <cstring>

namespace lxr {

struct Key160::pimpl {
    sizebounded<unsigned char, 160/8> _buffer;
};

```
