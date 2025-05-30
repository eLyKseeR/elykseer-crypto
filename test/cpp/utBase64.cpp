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

#include <string>
#include <string_view>

#include "boost/test/unit_test.hpp"

import lxr_base64;

BOOST_AUTO_TEST_SUITE( utBase64 )

// Test case: compare message encoding to known one
BOOST_AUTO_TEST_CASE( base64_encoding )
{
	std::string_view msg = "hello world.";
	std::string_view b64 = "aGVsbG8gd29ybGQu";
	BOOST_CHECK_EQUAL(lxr::Base64::encode(msg), b64);
}

BOOST_AUTO_TEST_CASE( base64_long_encoding )
{
    constexpr int repetitions = 1000;
	const std::string base_m = "hello world.";
    std::string t_m;
    t_m.reserve(base_m.length() * repetitions);
	const std::string base_b64 = "aGVsbG8gd29ybGQu";
    std::string t_b64;
    t_b64.reserve(base_b64.length() * repetitions);
    for (int i = 0; i < repetitions; i++) {
        t_m += base_m;
        t_b64 += base_b64;
    }
    std::string_view msg{t_m};
	std::string_view b64{t_b64};
	BOOST_CHECK_EQUAL(lxr::Base64::encode(msg), b64);
}

// Test case: compare message decoding to known one
BOOST_AUTO_TEST_CASE( base64_decoding )
{
	std::string_view b64 = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIG5lYyBlaXVzIHZvbHVtdXMgcGF0cmlvcXVlIGN1LCBuZWMgZXQgY29tbW9kby4uLg==";
	std::string_view msg = "Lorem ipsum dolor sit amet, nec eius volumus patrioque cu, nec et commodo...";
	BOOST_CHECK_EQUAL(lxr::Base64::decode(b64), msg);
}

BOOST_AUTO_TEST_SUITE_END()