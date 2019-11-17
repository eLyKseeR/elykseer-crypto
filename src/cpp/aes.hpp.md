```cpp

/*
<fpaste ../../src/copyright.md>
*/

#pragma once

#include "lxr/key128.hpp"
#include "lxr/key256.hpp"
#include "sizebounded/sizebounded.hpp"

````

namespace [lxr](namespace.list) {

/*

```fsharp

module Aes =

    exception BadLength

    val encrypt : Key256.t -> byte array -> byte array
    (** encrypts bytes with key *)

    val decrypt : Key256.t -> byte array -> byte array
    (** decrypts bytes with key *)
```

*/

## class Aes

// implementations exist for [OpenSSL](aes_openssl.cpp.md)
// and [Crypto++](aes_cryptopp.cpp.md)

{

>public:

>virtual ~Aes();

>static constexpr unsigned int datasz { 1024*4 };

>virtual int [process](aes_functions.cpp.md)(int inlen, sizebounded&lt;unsigned char, Aes::datasz&gt; & inoutbuf) = 0;

>virtual int [finish](aes_functions.cpp.md)(int inpos, sizebounded&lt;unsigned char, Aes::datasz&gt; & outbuf) = 0;

>protected:

>Aes();

>struct pimpl;

>std::unique_ptr&lt;pimpl&gt; _pimpl;

};

## class AesDecrypt : public Aes

{

>public:

>AesDecrypt(Key256 const & k, Key128 const & iv);

>virtual ~AesDecrypt() = default;

>virtual int [process](aes_functions.cpp.md)(int inlen, sizebounded&lt;unsigned char, Aes::datasz&gt; & inoutbuf) override;

>virtual int [finish](aes_functions.cpp.md)(int inpos, sizebounded&lt;unsigned char, Aes::datasz&gt; & outbuf) override;

>protected:

>private:

>AesDecrypt(AesDecrypt const &) = delete;

>AesDecrypt & operator=(AesDecrypt const &) = delete;

};

## class AesEncrypt : public Aes

{

>public:

>AesEncrypt(Key256 const & k, Key128 const & iv);

>virtual ~AesEncrypt() = default;

>virtual int [process](aes_functions.cpp.md)(int inlen, sizebounded&lt;unsigned char, Aes::datasz&gt; & inoutbuf) override;

>virtual int [finish](aes_functions.cpp.md)(int inpos, sizebounded&lt;unsigned char, Aes::datasz&gt; & outbuf) override;

>protected:

>private:

>AesEncrypt(AesEncrypt const &) = delete;

>AesEncrypt & operator=(AesEncrypt const &) = delete;

};


```cpp
} // namespace
```

# // C binding interface
```cpp
#include "lxr/lxr-cbindings.hpp"
```

struct CAesEncrypt {
   void * ptr;
   unsigned int lastpos;
   unsigned char buf[lxr::Aes::datasz];
};

extern "C" EXPORT
CAesEncrypt* [mk_AesEncrypt](aes_cbindings.cpp.md)(CKey256 * k, CKey128 * iv);

extern "C" EXPORT
void [release_AesEncrypt](aes_cbindings.cpp.md)(CAesEncrypt * cl);

extern "C" EXPORT
int [proc_AesEncrypt](aes_cbindings.cpp.md)(CAesEncrypt * cl, unsigned int inlen, unsigned char const *);

extern "C" EXPORT
int [fin_AesEncrypt](aes_cbindings.cpp.md)(CAesEncrypt * cl);

extern "C" EXPORT
unsigned int [len_AesEncrypt](aes_cbindings.cpp.md)(CAesEncrypt * cl);

extern "C" EXPORT
unsigned int [copy_AesEncrypt](aes_cbindings.cpp.md)(CAesEncrypt * cl, unsigned int outlen, unsigned char *);

extern "C" EXPORT
unsigned int [sz_AesEncrypt](aes_cbindings.cpp.md)();

struct CAesDecrypt {
   void * ptr;
   unsigned int lastpos;
   unsigned char buf[lxr::Aes::datasz];
};

extern "C" EXPORT
CAesDecrypt* [mk_AesDecrypt](aes_cbindings.cpp.md)(CKey256 * k, CKey128 * iv);

extern "C" EXPORT
void [release_AesDecrypt](aes_cbindings.cpp.md)(CAesDecrypt * cl);

extern "C" EXPORT
int [proc_AesDecrypt](aes_cbindings.cpp.md)(CAesDecrypt * cl, unsigned int inlen, unsigned char const *);

extern "C" EXPORT
int [fin_AesDecrypt](aes_cbindings.cpp.md)(CAesDecrypt * cl);

extern "C" EXPORT
unsigned int [len_AesDecrypt](aes_cbindings.cpp.md)(CAesDecrypt * cl);

extern "C" EXPORT
unsigned int [copy_AesDecrypt](aes_cbindings.cpp.md)(CAesDecrypt * cl, unsigned int outlen, unsigned char *);

extern "C" EXPORT
unsigned int [sz_AesDecrypt](aes_cbindings.cpp.md)();
