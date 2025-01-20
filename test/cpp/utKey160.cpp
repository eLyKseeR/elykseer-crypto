```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/key160.hpp"
#include "sizebounded/sizebounded.ipp"

#include <iostream>
````

# Test suite: utKey160

on class [Key160](../src/key160.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utKey160 )
```
## Test case: verify that key is random
```cpp
BOOST_AUTO_TEST_CASE( new_key_is_random )
{
    lxr::Key160 k1, k2;
    BOOST_CHECK(k1 != k2);
}
```

## Test case: key length is 160 bits
```cpp
BOOST_AUTO_TEST_CASE( key_length )
{
    lxr::Key160 k;
	BOOST_CHECK_EQUAL(k.toHex().size(), 160 / 8 * 2);
}
```

## Test case in C: verify that key is random
```cpp
BOOST_AUTO_TEST_CASE( c_new_key_is_random )
{
    CKey160 *k1 = mk_Key160();
    CKey160 *k2 = mk_Key160();
	char *h1 = tohex_Key160(k1);
	char *h2 = tohex_Key160(k2);
	BOOST_CHECK(strncmp(h1, h2, 160/8*2) != 0);
    release_Key160(k1); release_Key160(k2);
    free(h1); free(h2);
}
```

## Test case in C: key length is 160 bits
```cpp
BOOST_AUTO_TEST_CASE( c_key_length )
{
    CKey160 *k = mk_Key160();
	BOOST_CHECK_EQUAL(len_Key160(k), 160);
    release_Key160(k);
}
```

## Test case in C: bytes(fromhex(tohex(k)))==bytes(k)
```cpp
BOOST_AUTO_TEST_CASE( c_fromhex_regenerates_key )
{
    CKey160 *k1 = mk_Key160();
    char *h1 = tohex_Key160(k1);
    CKey160 *k2 = fromhex_Key160(h1);
    char *h2 = tohex_Key160(k2);
    std::string b1 = std::string(h1, len_Key160(k1)*2/8);
    std::string b2 = std::string(h2, len_Key160(k2)*2/8);
    BOOST_CHECK_EQUAL(b1, b2);
    release_Key160(k1); release_Key160(k2);
    free(h1); free(h2);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
