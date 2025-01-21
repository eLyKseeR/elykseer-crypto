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
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

#include "boost/test/unit_test.hpp"

import lxr_gpg;


BOOST_AUTO_TEST_SUITE( utGpg, * boost::unit_test_framework::disabled() )

// set this to the fingerprint of a PGP key you own the private key.
// it will be used to encrypt data that only you may decrypt.
static const std::string addr = "E2DFCA9AA83BA568AF39280999283F5327C5D38F";

// Test case: check address availability
BOOST_AUTO_TEST_CASE( gpg_check_address_fail )
{
  const std::string addr = "02355D3AE5A849EA2FFC0E8887EFE7D9796875C2";

  lxr::Gpg gpg;
  BOOST_CHECK(!gpg.has_public_key(addr));
}

BOOST_AUTO_TEST_CASE( gpg_check_address_succeed )
{

  lxr::Gpg gpg;
  BOOST_CHECK(gpg.has_public_key(addr));
}

BOOST_AUTO_TEST_CASE( gpg_check_non_private_key )
{
  lxr::Gpg gpg;
  BOOST_TEST_WARN(!gpg.has_private_key(addr), "the archiving process should not have access to this private key.");
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_decrypt_string )
{
  lxr::Gpg gpg;
  std::optional<std::string> cipher = gpg.encrypt_to_key(addr, "hello world.");
  BOOST_CHECK(cipher);
  if (cipher) {
    std::optional<std::string> plain = gpg.decrypt_from_buffer(cipher.value());
    BOOST_CHECK(plain);
    BOOST_CHECK_EQUAL("hello world.", plain.value());
  }
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_stream )
{
  lxr::Gpg gpg;
  gpg.ostream() << "hello world." << std::endl;
  auto s = gpg.encrypt_to_key(addr);
  BOOST_CHECK(s);
  // if (s) { std::clog << s.value() << std::endl; }
}

BOOST_AUTO_TEST_CASE( gpg_encrypt_stream2 )
{
  lxr::Gpg gpg;
  std::ostream &os = gpg.ostream();
  os << "hello world." << std::endl;
  auto s = gpg.encrypt_to_key(addr);
  BOOST_CHECK(s);
  if (s) {
    auto const tmpd = std::filesystem::temp_directory_path();
    std::ofstream fout; fout.open((tmpd / std::string("test.gpg")).native());
    fout << s.value();
    fout.close();
  }
}

// the following might ask for your private key and thus is disabled.
// feel free to test it on your own.
BOOST_AUTO_TEST_CASE( gpg_decrypt_file, * boost::unit_test::disabled() )
{
  auto const tmpd = std::filesystem::temp_directory_path();
  auto const fpath = tmpd / "test.gpg";
  // std::clog << "gpg encrypted file path: " << fpath << std::endl;
  BOOST_CHECK(std::filesystem::exists(fpath));
  lxr::Gpg gpg;
  BOOST_CHECK(gpg.decrypt_from_file(fpath.string()));
  char plain[256]; memset(plain, '\0', 256);
  gpg.istream().getline(plain, 256);
  BOOST_CHECK_EQUAL("hello world.", plain);
}

BOOST_AUTO_TEST_SUITE_END()