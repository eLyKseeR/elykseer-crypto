```cpp

/*
<fpaste ../../src/copyright.md>
*/

#pragma once

````

namespace [lxr](namespace.list) {


# class Base64

// implementation with Crypto++

{

>public:

>static std::string [encode](base64_functions.cpp.md)(std::string const & m);

>static std::string [decode](base64_functions.cpp.md)(std::string const & m);

>protected:

>Base64() {}

>private:

>Base64(Base64 const &) = delete;

>Base64 & operator=(Base64 const &) = delete;

};

```cpp
} // namespace
```

# // C binding interface
```cpp
#include "lxr/lxr-cbindings.hpp"
```

extern `"C"` EXPORT
int [base64_encode](base64_cbindings.cpp.md)(int mlen, const char * m, int outlen, unsigned char * out);

extern `"C"` EXPORT
int [base64_decode](base64_cbindings.cpp.md)(int mlen, const char * m, int outlen, unsigned char * out);
