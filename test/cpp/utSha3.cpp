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
#include <filesystem>
#include <string>

#include "boost/test/unit_test.hpp"

import lxr_sha3;
import lxr_key256;

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
const std::string shacmd = "sha3sum -a 256 ";
#else
#error Where are we?
#endif


BOOST_AUTO_TEST_SUITE( utSha3 )
// Test case: compare message digest to known one
BOOST_AUTO_TEST_CASE( message_digest )
{
	std::string msg = "the cleartext message, I am.";
	std::string sha3_256 = "975069d1d1eefe92b2c1e81983df7cc503635614e81190c70364fe32e5683df3";
	BOOST_CHECK_EQUAL(lxr::Sha3_256::hash(msg).toHex(), sha3_256);
}

// Test case: compute checksum over file
BOOST_AUTO_TEST_CASE( file_checksum )
{
  std::filesystem::path fp = "/bin/sh";
  std::string sha3_256 = "";

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
  std::string cmd = shacmd; cmd += fp.string();
  FILE* pipe = popen(cmd.c_str(), "r");
  if (pipe) {
    char buffer[128];
    fgets(buffer, 128, pipe);
    sha3_256 = std::string(buffer, 64);
    pclose(pipe);
  }
#else
#error Where are we?
#endif
  BOOST_CHECK_EQUAL(lxr::Sha3_256::hash(fp).toHex(), sha3_256);
}

// Test case in C: compare message digest to known one
BOOST_AUTO_TEST_CASE( c_message_digest )
{
	const char* msg = "the cleartext message, I am.";
	std::string sha3_256 = "975069d1d1eefe92b2c1e81983df7cc503635614e81190c70364fe32e5683df3";
  CKey256 * k = hash_Sha3_256(std::strlen(msg), msg);
	auto h = tohex_Key256(k);
	BOOST_CHECK_EQUAL(h, sha3_256);
}

// Test case in C: compute checksum over file
BOOST_AUTO_TEST_CASE( c_file_checksum )
{
  const char* fp = "/bin/sh";
  char sha3_256[65]; sha3_256[64]=0;

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
  std::string cmd = shacmd; cmd += fp;
  FILE* pipe = popen(cmd.c_str(), "r");
  if (pipe) {
    char buffer[128];
    fgets(buffer, 128, pipe);
    memcpy(sha3_256, buffer, 64);
    pclose(pipe);
  }
#else
#error Where are we?
#endif
  CKey256 * k = filehash_Sha3_256(fp);
  auto h = tohex_Key256(k);
  BOOST_CHECK_EQUAL(h, sha3_256);
}

BOOST_AUTO_TEST_SUITE_END()