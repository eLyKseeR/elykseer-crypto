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

import lxr_randlist;


BOOST_AUTO_TEST_SUITE( utRandList )

// Test case: generate list of integers
BOOST_AUTO_TEST_CASE( list_integers )
{
	auto vs = lxr::RandList::Make(1, 100);
	int sum = 0;
	for (auto v : vs) {
		//std::clog << v << std::endl;
		sum += v;
	}
	BOOST_CHECK_EQUAL(vs.size(), 100);
	BOOST_CHECK_EQUAL(sum, 50*101);  // (1 + 100) + (2 + 99) + (3 + 98) ..
}

BOOST_AUTO_TEST_SUITE_END()