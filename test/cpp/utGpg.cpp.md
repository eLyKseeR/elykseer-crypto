```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/gpg.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
````

# Test suite: utGpg

on class [Gpg](../src/gpg.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utGpg )
```
## Test case: check address availability
```cpp
BOOST_AUTO_TEST_CASE( gpg_check_address_fail )
{
  const std::string addr = "02355D3AE5A849EA2FFC0E8887EFE7D9796875C2";

  lxr::Gpg gpg;
  BOOST_CHECK(!gpg.exists_address(addr));
}

BOOST_AUTO_TEST_CASE( gpg_check_address_succeed )
{
  const std::string addr = "E2DFCA9AA83BA568AF39280999283F5327C5D38F";

  lxr::Gpg gpg;
  BOOST_CHECK(gpg.exists_address(addr));
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_stream )
{
  const std::string addr = "E2DFCA9AA83BA568AF39280999283F5327C5D38F";

  lxr::Gpg gpg;
  gpg.ostream() << "hello world." << std::endl;
  auto s = gpg.encrypt_to_key(addr);
  if (s) { std::clog << s.value() << std::endl; }
  BOOST_CHECK(gpg.good());
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
