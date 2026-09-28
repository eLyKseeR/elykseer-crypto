/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2026 Alexander Diemand

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

#include "sizebounded/sizebounded.ipp"

import lxr_key96;


BOOST_AUTO_TEST_SUITE( utKey96 )

// Test case: verify that key is random
BOOST_AUTO_TEST_CASE( new_key_is_random )
{
    lxr::Key96 k1;
    char buffer[96]; 
    {
        lxr::Key96 k2;
        if (k1 == k2)
            std::clog << "? keys different: " << k1.toHex() << " - " << k2.toHex() << std::endl;
        BOOST_CHECK_NE(k1, k2);
    }
}

// Test case: key length is 96 bits
BOOST_AUTO_TEST_CASE( key_length )
{
    lxr::Key96 k;
    BOOST_CHECK_EQUAL(k.length(), 96);
	BOOST_CHECK_EQUAL(k.toHex().size(), 96 / 8 * 2);
}

// Test case in C: verify that key is random
BOOST_AUTO_TEST_CASE( c_new_key_is_random )
{
    CKey96 *k1 = mk_Key96();
    CKey96 *k2 = mk_Key96();
	unsigned char buf[24];
    BOOST_CHECK(tohex_Key96(k1, buf, 24));
    std::string h1{(const char*)buf,24};
    BOOST_CHECK(tohex_Key96(k2, buf, 24));
    std::string h2{(const char*)buf,24};
	BOOST_CHECK_NE(h1, h2);
    release_Key96(k1); release_Key96(k2);
}

// Test case in C: key length is 96 bits
BOOST_AUTO_TEST_CASE( c_key_length )
{
    CKey96 *k = mk_Key96();
	BOOST_CHECK_EQUAL(len_Key96(k), 96);
    release_Key96(k);
}

// Test case in C: bytes(fromhex(tohex(k)))==bytes(k)
BOOST_AUTO_TEST_CASE( c_fromhex_regenerates_key )
{
    CKey96 *k1 = mk_Key96();
	unsigned char buf[24];
    BOOST_CHECK(tohex_Key96(k1, buf, 24));
    std::string h1{(const char*)buf,24};
    CKey96 *k2 = fromhex_Key96(h1.c_str());
    BOOST_CHECK(tohex_Key96(k2, buf, 24));
    std::string h2{(const char*)buf,24};
    BOOST_CHECK_EQUAL(h1, h2);
    release_Key96(k1); release_Key96(k2);
}

// Test case in C: hex string of wrong length is rejected
BOOST_AUTO_TEST_CASE( c_fromhex_wrong_length )
{
    BOOST_CHECK(fromhex_Key96("") == nullptr);
    BOOST_CHECK(fromhex_Key96("00") == nullptr);
    BOOST_CHECK(fromhex_Key96(std::string(24-1, '0').c_str()) == nullptr);
    BOOST_CHECK(fromhex_Key96(std::string(24+1, '0').c_str()) == nullptr);
    BOOST_CHECK(fromhex_Key96(nullptr) == nullptr);
    CKey96 *k = fromhex_Key96(std::string(24, '0').c_str());
    BOOST_REQUIRE(k != nullptr);
    release_Key96(k);
}

BOOST_AUTO_TEST_SUITE_END()