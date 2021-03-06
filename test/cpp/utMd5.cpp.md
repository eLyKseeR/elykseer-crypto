```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/md5.hpp"
#include "lxr/key128.hpp"

#include <iostream>
````

# Test suite: utMd5

on class [Md5](../src/md5.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utMd5 )
```

## Test case: compare message digest to known one
```cpp
BOOST_AUTO_TEST_CASE( message_digest )
{
	std::string msg = "the cleartext message, I am.";
	std::string md5 = "9cf9e8974fa9d1151de1daf6983a3e71";
	BOOST_CHECK_EQUAL(lxr::Md5::hash(msg).toHex(), md5);
}
```

## Test case in C: compare message digest to known one
```cpp
BOOST_AUTO_TEST_CASE( c_message_digest )
{
	const char* msg = "the cleartext message, I am.";
	const char* md5 = "9cf9e8974fa9d1151de1daf6983a3e71";
	CKey128 *k = hash_Md5(std::strlen(msg), msg);
	char *h = tohex_Key128(k);
	BOOST_CHECK_EQUAL(h, md5);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
