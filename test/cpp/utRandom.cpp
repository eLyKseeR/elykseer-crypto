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

import lxr_random;


BOOST_AUTO_TEST_SUITE( utRandom )
// Test case: subsequent random numbers have a significant difference
BOOST_AUTO_TEST_CASE( test_for_randomness )
{
    uint32_t arr[5];
    for (int i=0; i<5; i++) {
        arr[i] = lxr::Random::rng().random();
    }
    int32_t dff = 0L;
    for (int i=1; i<5; i++) {
        dff += arr[0];
        dff -= arr[i];
    }
	BOOST_CHECK(abs(dff) > 10000);
}

// Test case: try hard to find an invalid number
BOOST_AUTO_TEST_CASE( test_for_interval )
{
    std::vector<uint32_t> f(201);
    for (int i=0; i<201; i++) { f[i] = 0; }
    constexpr int n = 1000000;
    for (int i=0; i<n; i++) {
        auto r = lxr::Random::rng().random(200);
        //std::clog << r << " ";
        BOOST_CHECK( r >= 0 );
        BOOST_CHECK( r < 200 );
        ++f[r];
    }
    BOOST_CHECK_EQUAL(f[200], 0);
    //for (int i=0; i<201; i++) {
    //    std::clog << i << " " << f[i] << std::endl;
    //}
}

BOOST_AUTO_TEST_SUITE_END()