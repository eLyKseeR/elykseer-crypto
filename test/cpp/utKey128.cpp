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

#include "sizebounded/sizebounded.ipp"

import lxr_key128;


BOOST_AUTO_TEST_SUITE( utKey128 )

// Test case: verify that key is random
BOOST_AUTO_TEST_CASE( new_key_is_random )
{
    lxr::Key128 k1;
    char buffer[128]; 
    {
        lxr::Key128 k2;
        if (k1 == k2)
            std::clog << "? keys different: " << k1.toHex() << " - " << k2.toHex() << std::endl;
        BOOST_CHECK_NE(k1, k2);
    }
}

// Test case: key length is 128 bits
BOOST_AUTO_TEST_CASE( key_length )
{
    lxr::Key128 k;
    BOOST_CHECK_EQUAL(k.length(), 128);
	BOOST_CHECK_EQUAL(k.toHex().size(), 128 / 8 * 2);
}

// Test case in C: verify that key is random
BOOST_AUTO_TEST_CASE( c_new_key_is_random )
{
    CKey128 *k1 = mk_Key128();
    CKey128 *k2 = mk_Key128();
	unsigned char buf[32];
    BOOST_CHECK(tohex_Key128(k1, buf, 32));
    std::string h1{(const char*)buf,32};
    BOOST_CHECK(tohex_Key128(k2, buf, 32));
    std::string h2{(const char*)buf,32};
	BOOST_CHECK_NE(h1, h2);
    release_Key128(k1); release_Key128(k2);
}

// Test case in C: key length is 128 bits
BOOST_AUTO_TEST_CASE( c_key_length )
{
    CKey128 *k = mk_Key128();
	BOOST_CHECK_EQUAL(len_Key128(k), 128);
    release_Key128(k);
}

// Test case in C: bytes(fromhex(tohex(k)))==bytes(k)
BOOST_AUTO_TEST_CASE( c_fromhex_regenerates_key )
{
    CKey128 *k1 = mk_Key128();
	unsigned char buf[32];
    BOOST_CHECK(tohex_Key128(k1, buf, 32));
    std::string h1{(const char*)buf,32};
    CKey128 *k2 = fromhex_Key128(h1);
    BOOST_CHECK(tohex_Key128(k2, buf, 32));
    std::string h2{(const char*)buf,32};
    BOOST_CHECK_EQUAL(h1, h2);
    release_Key128(k1); release_Key128(k2);
}

BOOST_AUTO_TEST_SUITE_END()