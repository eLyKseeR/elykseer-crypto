```cpp

/*
<fpaste ../../src/copyright.md>
*/

#pragma once

#include "lxr/key128.hpp"
#include "lxr/key160.hpp"
#include "lxr/key256.hpp"
#include <functional>

````

namespace [lxr](namespace.list) {


# class HMAC

// implementation with Crypto++

{

>public:

>static Key128 [hmac_md5](hmac_functions.cpp.md)(const char k[], int klen, std::string const & m);

>static Key128 [hmac_md5](hmac_functions.cpp.md)(const char k[], int klen, const char m[], int mlen);

>static Key256 [hmac_sha256](hmac_functions.cpp.md)(const char k[], int klen, std::string const & m);

>static Key256 [hmac_sha256](hmac_functions.cpp.md)(const char k[], int klen, const char m[], int mlen);

>static Key160 [hmac_sha1](hmac_functions.cpp.md)(const char k[], int klen, std::string const & m);

>static Key160 [hmac_sha1](hmac_functions.cpp.md)(const char k[], int klen, const char m[], int mlen);

>protected:

>HMAC() {}

>private:

>HMAC(HMAC const &) = delete;

>HMAC & operator=(HMAC const &) = delete;

};

```cpp
} // namespace
```

# // C binding interface
```cpp
#include "lxr/lxr-cbindings.hpp"
```

extern `"C"` EXPORT
CKey128* [hmac_Md5](hmac_cbindings.cpp.md)(int klen, const char k[], int mlen, const char * m);

extern `"C"` EXPORT
CKey256* [hmac_Sha256](hmac_cbindings.cpp.md)(int klen, const char k[], int mlen, const char * m);

extern `"C"` EXPORT
CKey160* [hmac_Sha1](hmac_cbindings.cpp.md)(int klen, const char k[], int mlen, const char * m);
