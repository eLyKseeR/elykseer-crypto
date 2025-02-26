/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2025 Alexander Diemand

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include <iostream>
#include <string>

#include "boost/test/unit_test.hpp"

import lxr_hmac;
import lxr_md5;
import lxr_base64;
import lxr_key128;
import lxr_key256;


BOOST_AUTO_TEST_SUITE( utHMAC )

// Test case: compare message digest to known one
BOOST_AUTO_TEST_CASE( message_digest )
{
    const char *key = "Jefe";
    const int klen = 4;
	std::string msg = "what do ya want for nothing?";
	std::string md5 = "750c783e6ab0b503eaa86e310a5db738";
	BOOST_CHECK_EQUAL(lxr::HMAC::hmac_md5(key, klen, msg).toHex(), md5);
}

// Test case in C: compare message digest to known one
BOOST_AUTO_TEST_CASE( c_message_digest )
{
    const char *key = "Jefe";
    const int klen = 4;
	const char* msg = "what do ya want for nothing?";
	std::string md5 = "750c783e6ab0b503eaa86e310a5db738";
	CKey128 *k = hmac_Md5(klen, key, std::strlen(msg), msg);
	auto h = tohex_Key128(k);
	BOOST_CHECK_EQUAL(h, md5);
    release_Key128(k);
}

// Test case: compare message MD5 digest to known one
BOOST_AUTO_TEST_CASE( test_md5_hmac )
{
    const char *key = "test";
    const int klen = 4;
    std::string msg = "test";
	auto md5 = lxr::HMAC::hmac_md5(key, klen, msg);
	BOOST_CHECK_EQUAL(md5.toHex(), "cd4b0dcbe0f4538b979fb73664f51abe");
}

// Test case: compare message SHA1 digest to known one
BOOST_AUTO_TEST_CASE( test_sha1_hmac )
{
    const char *key = "test";
    const int klen = 4;
    std::string msg = "test";
	auto sha1 = lxr::HMAC::hmac_sha1(key, klen, msg);
	BOOST_CHECK_EQUAL(sha1.toHex(), "0c94515c15e5095b8a87a50ba0df3bf38ed05fe6");
}

// Test case: compare message SHA256 digest to known one
// openssl dgst -hmac "test" -sha256 test.txt # faad4ffcac0c6eb99d3f3cd5975001bc69011844238d50729ff485c2d8f8724f
BOOST_AUTO_TEST_CASE( test_sha256_hmac )
{
    const char *key = "test";
    const int klen = 4;
    std::string msg = "test";
	auto sha256 = lxr::HMAC::hmac_sha256(key, klen, msg);
	BOOST_CHECK_EQUAL(sha256.toHex(), "88cd2108b5347d973cf39cdf9053d7dd42704876d8c9a9bd8e2d168259d3ddf7");
}

// Test case: compare message SHA1 digest to known one

// example from AWS documentation: https://docs.aws.amazon.com/AmazonS3/latest/userguide/RESTAuthentication.html

BOOST_AUTO_TEST_CASE( message_sha1_digest )
{
    const char *key = "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY"; // access key
    const int klen = 40;
    // the canonicalised message to sign:
    std::string msg = "GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n/awsexamplebucket1/photos/puppy.jpg";
	auto sha1 = lxr::HMAC::hmac_sha1(key, klen, msg);
	BOOST_CHECK_EQUAL(sha1.toHex(), "aa0936fba4aff7fa0cec6dea2c48d31f56b59758");
	std::string sha1_b64 = "qgk2+6Sv9/oM7G3qLEjTH1a1l1g=";
	constexpr int blen = 256/8;
	unsigned char buffer[blen+1];
    memset(buffer, 0, blen+1);
	int b64len = base64_encode(160/8, (const char *)sha1.bytes(), blen, buffer);
	buffer[b64len] = '\0';
	std::string b64((const char *)buffer, b64len);
	BOOST_CHECK_EQUAL(b64, sha1_b64);
}

BOOST_AUTO_TEST_SUITE_END()