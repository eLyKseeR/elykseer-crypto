```cpp

/*
<fpaste ../../src/copyright.md>
*/

#pragma once

#include <filesystem>

#include "lxr/key256.hpp"
#include "sizebounded/sizebounded.hpp"

````

namespace [lxr](namespace.list) {

# class Sha3_256

// implementations exist for [OpenSSL](sha3_openssl.cpp.md)
// and [Crypto++](sha3_cryptopp.cpp.md)

{

>public:

>static Key256 [hash](sha3_functions.cpp.md)(std::string const &);

>static Key256 [hash](sha3_functions.cpp.md)(const char[], int length);

>static Key256 [hash](sha3_functions.cpp.md)(std::filesystem::path const &);

>Sha3_256();

>~Sha3_256();

>static constexpr unsigned int datasz { 1024*64 };

>int [process](sha3_functions.cpp.md)(int inlen, sizebounded&lt;unsigned char, Sha3_256::datasz&gt; const & inbuf);

>Key256 [finish](sha3_functions.cpp.md)();

>private:

>struct pimpl;

>std::unique_ptr&lt;pimpl&gt; _pimpl;

>Sha3_256(Sha3_256 const &) = delete;

>Sha3_256 & operator=(Sha3_256 const &) = delete;

};

```cpp
} // namespace
```

# // C binding interface
```cpp
#include "lxr/lxr-cbindings.hpp"
```

extern `"C"` EXPORT
CKey256* [hash_Sha3_256](sha3_cbindings.cpp.md)(int len, const char *s);

extern `"C"` EXPORT
CKey256* [filehash_Sha3_256](sha3_cbindings.cpp.md)(const char * fp);
