```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/gpg.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

````

# Test suite: utGpg

on class [Gpg](../src/gpg.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utGpg, * boost::unit_test_framework::disabled() )
```

set this to the fingerprint of a PGP key you own the private key.
it will be used to encrypt data that only you may decrypt.
```cpp
static const std::string addr = "E2DFCA9AA83BA568AF39280999283F5327C5D38F";
```

## Test case: check address availability
```cpp
BOOST_AUTO_TEST_CASE( gpg_check_address_fail )
{
  const std::string addr = "02355D3AE5A849EA2FFC0E8887EFE7D9796875C2";

  lxr::Gpg gpg;
  BOOST_CHECK(!gpg.has_public_key(addr));
}

BOOST_AUTO_TEST_CASE( gpg_check_address_succeed )
{

  lxr::Gpg gpg;
  BOOST_CHECK(gpg.has_public_key(addr));
}

BOOST_AUTO_TEST_CASE( gpg_check_non_private_key )
{
  lxr::Gpg gpg;
  BOOST_TEST_WARN(!gpg.has_private_key(addr), "the archiving process should not have access to this private key.");
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_string )
{
  lxr::Gpg gpg;
  auto s = gpg.encrypt_to_key(addr, "hello world.");
  BOOST_CHECK(s);
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_stream )
{
  lxr::Gpg gpg;
  gpg.ostream() << "hello world." << std::endl;
  auto s = gpg.encrypt_to_key(addr);
  BOOST_CHECK(s);
  // if (s) { std::clog << s.value() << std::endl; }
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_stream2 )
{
  lxr::Gpg gpg;
  std::ostream &os = gpg.ostream();
  os << "hello world." << std::endl;
  auto s = gpg.encrypt_to_key(addr);
  BOOST_CHECK(s);
  if (s) {
    auto const tmpd = std::filesystem::temp_directory_path();
    std::ofstream fout; fout.open((tmpd / std::string("test.gpg")).native());
    fout << s.value();
    fout.close();
  }
}
```

the following might ask for your private key and thus is disabled.
feel free to test it on your own.
```cpp
BOOST_AUTO_TEST_CASE( gpg_decrypt_file, * boost::unit_test::disabled() )
{
  auto const tmpd = std::filesystem::temp_directory_path();
  BOOST_CHECK(std::filesystem::exists(tmpd / "test.gpg"));
  lxr::Gpg gpg;
  BOOST_CHECK(gpg.decrypt_from_file((tmpd / "test.gpg").string()));
  char plain[256]; memset(plain, '\0', 256);
  gpg.istream().getline(plain, 256);
  BOOST_CHECK_EQUAL("hello world.", plain);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
