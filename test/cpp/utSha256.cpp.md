```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/sha256.hpp"

#include <iostream>
````

# Test suite: utSha256

on class [Sha256](../src/sha256.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utSha256 )
```
## Test case: compare message digest to known one
```cpp
BOOST_AUTO_TEST_CASE( message_digest )
{
	std::string msg = "the cleartext message, I am.";
	std::string sha256 = "b27cd6b9a45d9aaa28c2319f33721ea5e8531b978a25b9c52993b75d5e90ff96";
	BOOST_CHECK_EQUAL(lxr::Sha256::hash(msg).toHex(), sha256);
}
```

## Test case: compute checksum over file
```cpp
BOOST_AUTO_TEST_CASE( file_checksum )
{
  boost::filesystem::path fp = "/bin/sh";
  std::string sha256 = "";

#if defined(__linux__) || defined(__APPLE__)
  std::string cmd = "sha256sum "; cmd += fp.string();
  FILE* pipe = popen(cmd.c_str(), "r");
  if (pipe) {
    char buffer[128];
    fgets(buffer, 128, pipe);
    sha256 = std::string(buffer, 64);
    pclose(pipe);
  }
#else
#error Where are we?
#endif
  BOOST_CHECK_EQUAL(lxr::Sha256::hash(fp).toHex(), sha256);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```