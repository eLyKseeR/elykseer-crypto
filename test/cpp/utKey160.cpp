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

#include "boost/test/unit_test.hpp"

#include "sizebounded/sizebounded.ipp"

import lxr_key160;


BOOST_AUTO_TEST_SUITE( utKey160 )
// Test case: verify that key is random
BOOST_AUTO_TEST_CASE( new_key_is_random )
{
    lxr::Key160 k1;
    {
        lxr::Key160 k2;
        BOOST_CHECK(k1 != k2);
    }
}

// Test case: key length is 160 bits
BOOST_AUTO_TEST_CASE( key_length )
{
    lxr::Key160 k;
	BOOST_CHECK_EQUAL(k.toHex().size(), 160 / 8 * 2);
}

// Test case in C: verify that key is random
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

// Test case in C: key length is 160 bits
BOOST_AUTO_TEST_CASE( c_key_length )
{
    CKey160 *k = mk_Key160();
	BOOST_CHECK_EQUAL(len_Key160(k), 160);
    release_Key160(k);
}

// Test case in C: bytes(fromhex(tohex(k)))==bytes(k)
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

BOOST_AUTO_TEST_SUITE_END()