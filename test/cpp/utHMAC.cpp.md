```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/hmac.hpp"
#include "lxr/key128.hpp"
#include "lxr/key256.hpp"

#include <iostream>
````

# Test suite: utHMAC

on class [Md5](../src/hmac.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utHMAC )
```

## Test case: compare message digest to known one
```cpp
BOOST_AUTO_TEST_CASE( message_digest )
{
    const char *key = "Jefe";
    const int klen = 4;
	std::string msg = "what do ya want for nothing?";
	std::string md5 = "750c783e6ab0b503eaa86e310a5db738";
	BOOST_CHECK_EQUAL(lxr::HMAC::hmac_md5(key, klen, msg).toHex(), md5);
}
```

## Test case in C: compare message digest to known one
```cpp
BOOST_AUTO_TEST_CASE( c_message_digest )
{
    const char *key = "Jefe";
    const int klen = 4;
	const char* msg = "what do ya want for nothing?";
	const char* md5 = "750c783e6ab0b503eaa86e310a5db738";
	CKey128 *k = hmac_Md5(klen, key, std::strlen(msg), msg);
	char *h = tohex_Key128(k);
	BOOST_CHECK_EQUAL(h, md5);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
