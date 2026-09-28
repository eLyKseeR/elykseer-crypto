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

#include <algorithm>
#include <map>
#include <vector>

#include "boost/test/unit_test.hpp"

import lxr_randlist;


BOOST_AUTO_TEST_SUITE( utRandList )

// Test case: generate list of integers
BOOST_AUTO_TEST_CASE( list_integers )
{
	auto vs = lxr::RandList::Make(1, 100);
	int sum = 0;
	for (auto v : vs) {
		sum += v;
	}
    if (sum != 50*101) {
	    for (auto v : vs) {
            std::clog << v << " ";
        }
        std::clog << "ERROR!" << std::endl;
    }
	BOOST_CHECK_EQUAL(vs.size(), 100);
	BOOST_CHECK_EQUAL(sum, 50*101);  // (1 + 100) + (2 + 99) + (3 + 98) ..
}

// Test case: every permutation is a complete, in-bounds shuffle
BOOST_AUTO_TEST_CASE( permutation_is_complete )
{
    for (int k=0; k<2000; k++) {
        auto vs = lxr::RandList::Make(1, 100);
        std::sort(vs.begin(), vs.end());
        for (int i=0; i<100; i++) {
            BOOST_REQUIRE_EQUAL(vs[i], i + 1);
        }
    }
}

// Test case: complete permutation for every length n=1..64
BOOST_AUTO_TEST_CASE( permutation_all_lengths )
{
    for (int n=1; n<=64; n++) {
        for (int k=0; k<50; k++) {
            auto vs = lxr::RandList::Make(1, n);
            BOOST_REQUIRE_EQUAL(vs.size(), n);
            std::sort(vs.begin(), vs.end());
            for (int i=0; i<n; i++) {
                BOOST_REQUIRE_EQUAL(vs[i], i + 1);
            }
        }
    }
}

// Test case: all 24 permutations of 4 elements are about equally likely
BOOST_AUTO_TEST_CASE( permutation_is_uniform )
{
    constexpr int rounds = 240000;
    std::map<std::vector<int>, int> counts;
    for (int k=0; k<rounds; k++) {
        ++counts[lxr::RandList::Make(1, 4)];
    }
    BOOST_REQUIRE_EQUAL(counts.size(), 24);
    // chi-square with 23 degrees of freedom; p=0.0001 critical value is ~54
    const double expected = rounds / 24.0;
    double chi2 = 0.0;
    for (auto const & [perm, c] : counts) {
        chi2 += (c - expected) * (c - expected) / expected;
    }
    BOOST_CHECK_LT(chi2, 54.0);
}

BOOST_AUTO_TEST_SUITE_END()