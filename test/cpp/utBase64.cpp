```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/base64.hpp"

#include <iostream>
````

# Test suite: utBase64

on class [Base64](../src/base64.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utBase64 )
```

## Test case: compare message encoding to known one
```cpp
BOOST_AUTO_TEST_CASE( base64_encoding )
{
	std::string msg = "hello world.";
	std::string b64 = "aGVsbG8gd29ybGQu";
	BOOST_CHECK_EQUAL(lxr::Base64::encode(msg), b64);
}
```

## Test case: compare message decoding to known one
```cpp
BOOST_AUTO_TEST_CASE( base64_decoding )
{
	std::string b64 = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIG5lYyBlaXVzIHZvbHVtdXMgcGF0cmlvcXVlIGN1LCBuZWMgZXQgY29tbW9kby4uLg==";
	std::string msg = "Lorem ipsum dolor sit amet, nec eius volumus patrioque cu, nec et commodo...";
	BOOST_CHECK_EQUAL(lxr::Base64::decode(b64), msg);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
