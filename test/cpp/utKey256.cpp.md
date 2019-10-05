```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/key256.hpp"
#include "sizebounded/sizebounded.ipp"

#include <iostream>
````

# Test suite: utKey256

on class [Key256](../src/key256.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utKey256 )
```
## Test case: verify that key is random
```cpp
BOOST_AUTO_TEST_CASE( new_key_is_random )
{
    lxr::Key256 k1, k2;
	BOOST_CHECK(k1.toHex() != k2.toHex());
}
```

## Test case: key length is 256 bits
```cpp
BOOST_AUTO_TEST_CASE( key_length )
{
    lxr::Key256 k;
	BOOST_CHECK_EQUAL(k.toHex().size(), 256 / 8 * 2);
}
```

## Test case: key restored from string representation
```cpp
BOOST_AUTO_TEST_CASE( restore_key_from_string_representation )
{
	lxr::Key256 k1, k2;
	k2.fromHex(k1.toHex());
	BOOST_CHECK_EQUAL(k1, k2);
}
```

## Test case in C: verify that key is random
```cpp
BOOST_AUTO_TEST_CASE( c_new_key_is_random )
{
    CKey256 *k1 = mk_Key256();
    CKey256 *k2 = mk_Key256();
	char *h1 = tohex_Key256(k1);
	char *h2 = tohex_Key256(k2);
	BOOST_CHECK(strcmp(h1, h2) != 0);
	release_Key256(k1); release_Key256(k2);
    free(h1); free(h2);
}
```

## Test case in C: key length is 256 bits
```cpp
BOOST_AUTO_TEST_CASE( c_key_length )
{
    CKey256 * k = mk_Key256();
	BOOST_CHECK_EQUAL(len_Key256(k), 256);
	release_Key256(k);
}
```

## Test case in C: bytes(fromhex(tohex(k)))==bytes(k)
```cpp
BOOST_AUTO_TEST_CASE( c_fromhex_regenerates_key )
{
    CKey256 *k1 = mk_Key256();
    char *h1 = tohex_Key256(k1);
    CKey256 *k2 = fromhex_Key256(h1);
    char *h2 = tohex_Key256(k2);
    std::string b1 = std::string(h1, len_Key256(k1)*2/8);
    std::string b2 = std::string(h2, len_Key256(k2)*2/8);
    BOOST_CHECK_EQUAL(b1, b2);
    release_Key256(k1); release_Key256(k2);
    free(h1); free(h2);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
