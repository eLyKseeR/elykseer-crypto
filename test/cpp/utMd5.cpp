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

import lxr_md5;
import lxr_key128;


BOOST_AUTO_TEST_SUITE( utMd5 )

// Test case: compare message digest to known one
BOOST_AUTO_TEST_CASE( message_digest )
{
	std::string msg = "the cleartext message, I am.";
	std::string md5 = "9cf9e8974fa9d1151de1daf6983a3e71";
	BOOST_CHECK_EQUAL(lxr::Md5::hash(msg).toHex(), md5);
}

// Test case in C: compare message digest to known one
BOOST_AUTO_TEST_CASE( c_message_digest )
{
	const char* msg = "the cleartext message, I am.";
	const std::string md5 = "9cf9e8974fa9d1151de1daf6983a3e71";
	CKey128 *k = hash_Md5(std::strlen(msg), msg);
	unsigned char buf[32];
    BOOST_CHECK(tohex_Key128(k, buf, 32));
    std::string h{(const char*)buf, 32};
	BOOST_CHECK_EQUAL(h, md5);
    release_Key128(k);
}

BOOST_AUTO_TEST_SUITE_END()