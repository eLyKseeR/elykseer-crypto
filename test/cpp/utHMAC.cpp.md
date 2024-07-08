```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/hmac.hpp"
#include "lxr/base64.hpp"
#include "lxr/key128.hpp"
#include "lxr/key256.hpp"

#include <iostream>
````

# Test suite: utHMAC

on class [HMAC](../src/hmac.hpp.md)

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

## Test case: compare message SHA1 digest to known one
```cpp
BOOST_AUTO_TEST_CASE( test_sha1_hmac )
{
    const char *key = "test";
    const int klen = 4;
    std::string msg = "test";
	auto sha1 = lxr::HMAC::hmac_sha1(key, klen, msg);
	BOOST_CHECK_EQUAL(sha1.toHex(), "0c94515c15e5095b8a87a50ba0df3bf38ed05fe6");
}
```

## Test case: compare message SHA1 digest to known one

example from AWS documentation: https://docs.aws.amazon.com/AmazonS3/latest/userguide/RESTAuthentication.html

```cpp
BOOST_AUTO_TEST_CASE( message_sha1_digest )
{
    const char *key = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY";
    const int klen = 40;
    std::string msg = "GET\\n\\n\\nTue, 27 Mar 2007 19:36:42 +0000\\n/awsexamplebucket1/photos/puppy.jpg";
	auto sha1 = lxr::HMAC::hmac_sha1(key, klen, msg);
	BOOST_CHECK_EQUAL(sha1.toHex(), "aa0936fba4aff7fa0cec6dea2c48d31f56b59758");
	std::string sha1_b64 = "qgk2+6Sv9/oM7G3qLEjTH1a1l1g=";
	constexpr int blen = 128;
	unsigned char buffer[blen];
	int b64len = base64_encode(160/8, (const char *)sha1.bytes(), blen, buffer);
	buffer[b64len] = '\0';
	std::string b64((const char *)buffer, b64len);
	BOOST_CHECK_EQUAL(b64, sha1_b64);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
