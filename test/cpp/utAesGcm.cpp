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

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "boost/test/unit_test.hpp"

#include "sizebounded/sizebounded.ipp"

import lxr_aes_gcm;
import lxr_key96;
import lxr_key256;


namespace {

using bytes = std::vector<unsigned char>;

bytes unhex(std::string const & h)
{
  bytes r;
  for (size_t i = 0; i + 1 < h.size(); i += 2) {
    r.push_back((unsigned char)std::stoi(h.substr(i, 2), nullptr, 16));
  }
  return r;
}

bytes pattern(size_t n)
{
  bytes r(n);
  for (size_t i = 0; i < n; i++) { r[i] = (unsigned char)((i * 31 + 7) & 0xff); }
  return r;
}

struct Sealed {
  bytes ct;
  unsigned char tag[lxr::AesGcm::tagsz];
};

Sealed seal(lxr::Key256 const & k, lxr::Key96 const & n, bytes const & aad, bytes const & pt)
{
  Sealed s;
  lxr::AesGcmEncrypt enc(k, n);
  BOOST_REQUIRE(enc.aad(aad.data(), aad.size()));
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  for (size_t pos = 0; pos < pt.size(); pos += lxr::AesGcm::datasz) {
    const size_t len = std::min<size_t>(lxr::AesGcm::datasz, pt.size() - pos);
    memcpy((void*)buf.ptr(), pt.data() + pos, len);
    const int lenc = enc.process(len, buf);
    BOOST_REQUIRE_EQUAL(lenc, (int)len);
    s.ct.insert(s.ct.end(), buf.ptr(), buf.ptr() + lenc);
  }
  BOOST_CHECK_EQUAL(enc.finish(0, buf), 0);
  BOOST_REQUIRE(enc.tag(s.tag));
  return s;
}

// returns finish() result; plaintext in pt (unauthenticated when < 0)
int open(lxr::Key256 const & k, lxr::Key96 const & n, bytes const & aad, bytes const & ct, unsigned char const tag[lxr::AesGcm::tagsz], bytes & pt)
{
  lxr::AesGcmDecrypt dec(k, n);
  BOOST_REQUIRE(dec.aad(aad.data(), aad.size()));
  BOOST_REQUIRE(dec.tag(tag));
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  pt.clear();
  for (size_t pos = 0; pos < ct.size(); pos += lxr::AesGcm::datasz) {
    const size_t len = std::min<size_t>(lxr::AesGcm::datasz, ct.size() - pos);
    memcpy((void*)buf.ptr(), ct.data() + pos, len);
    const int lenp = dec.process(len, buf);
    BOOST_REQUIRE_EQUAL(lenp, (int)len);
    pt.insert(pt.end(), buf.ptr(), buf.ptr() + lenp);
  }
  return dec.finish(0, buf);
}

// GCM spec (McGrew, Viega) test case 16
const std::string tc16_key = "feffe9928665731c6d6a8f9467308308feffe9928665731c6d6a8f9467308308";
const std::string tc16_iv  = "cafebabefacedbaddecaf888";
const std::string tc16_p   = "d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b39";
const std::string tc16_a   = "feedfacedeadbeeffeedfacedeadbeefabaddad2";
const std::string tc16_c   = "522dc1f099567d07f47f37a32a84427d643a8cdcbfe5c0c97598a2bd2555d1aa8cb08e48590dbb3da7b08b1056828838c5f61e6393ba7a0abcc9f662";
const std::string tc16_t   = "76fc6ece0f4e1768cddf8853bb2d551b";

} // anon


BOOST_AUTO_TEST_SUITE( utAesGcm )

BOOST_AUTO_TEST_CASE( known_answer_test16 )
{
  auto k = lxr::Key256::keyFromHex(tc16_key);
  auto n = lxr::Key96::keyFromHex(tc16_iv);
  auto s = seal(k, n, unhex(tc16_a), unhex(tc16_p));
  BOOST_CHECK(s.ct == unhex(tc16_c));
  BOOST_CHECK(bytes(s.tag, s.tag + 16) == unhex(tc16_t));
  bytes pt;
  BOOST_CHECK_EQUAL(open(k, n, unhex(tc16_a), s.ct, s.tag, pt), 0);
  BOOST_CHECK(pt == unhex(tc16_p));
}

BOOST_AUTO_TEST_CASE( small_roundtrip )
{
  lxr::Key256 k; lxr::Key96 n;
  const std::string msg = "all my precious data are safe, so I will sleep fine!";
  bytes pt(msg.begin(), msg.end());
  bytes aad{1, 2, 3};
  auto s = seal(k, n, aad, pt);
  BOOST_CHECK_EQUAL(s.ct.size(), pt.size());
  BOOST_CHECK(s.ct != pt);
  bytes out;
  BOOST_CHECK_EQUAL(open(k, n, aad, s.ct, s.tag, out), 0);
  BOOST_CHECK(out == pt);
}

BOOST_AUTO_TEST_CASE( multichunk_roundtrip )
{
  lxr::Key256 k; lxr::Key96 n;
  bytes pt = pattern(3 * lxr::AesGcm::datasz + 1234 + 5); // not a multiple of 16
  bytes aad = pattern(33);
  auto s = seal(k, n, aad, pt);
  bytes out;
  BOOST_CHECK_EQUAL(open(k, n, aad, s.ct, s.tag, out), 0);
  BOOST_CHECK(out == pt);
}

BOOST_AUTO_TEST_CASE( empty_plaintext_with_aad )
{
  lxr::Key256 k; lxr::Key96 n;
  bytes aad = pattern(20);
  auto s = seal(k, n, aad, bytes{});
  BOOST_CHECK(s.ct.empty());
  bytes out;
  BOOST_CHECK_EQUAL(open(k, n, aad, s.ct, s.tag, out), 0);
  bytes badaad = aad; badaad[0] ^= 1;
  BOOST_CHECK_EQUAL(open(k, n, badaad, s.ct, s.tag, out), -1);
}

BOOST_AUTO_TEST_CASE( tampering_fails )
{
  lxr::Key256 k; lxr::Key96 n;
  bytes pt = pattern(5000);
  bytes aad = pattern(10);
  auto s = seal(k, n, aad, pt);
  bytes out;

  bytes ct = s.ct; ct[100] ^= 0x01;
  BOOST_CHECK_EQUAL(open(k, n, aad, ct, s.tag, out), -1);

  unsigned char tag[16]; memcpy(tag, s.tag, 16); tag[15] ^= 0x80;
  BOOST_CHECK_EQUAL(open(k, n, aad, s.ct, tag, out), -1);

  bytes badaad = aad; badaad[3] ^= 1;
  BOOST_CHECK_EQUAL(open(k, n, badaad, s.ct, s.tag, out), -1);

  lxr::Key96 n2;
  BOOST_CHECK_EQUAL(open(k, n2, aad, s.ct, s.tag, out), -1);

  lxr::Key256 k2;
  BOOST_CHECK_EQUAL(open(k2, n, aad, s.ct, s.tag, out), -1);

  // untouched still fine
  BOOST_CHECK_EQUAL(open(k, n, aad, s.ct, s.tag, out), 0);
}

BOOST_AUTO_TEST_CASE( missing_tag_fails )
{
  lxr::Key256 k; lxr::Key96 n;
  lxr::AesGcmDecrypt dec(k, n);
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  BOOST_CHECK_EQUAL(dec.finish(0, buf), -1);
}

BOOST_AUTO_TEST_CASE( aad_only_before_process )
{
  lxr::Key256 k; lxr::Key96 n;
  lxr::AesGcmEncrypt enc(k, n);
  sizebounded<unsigned char, lxr::AesGcm::datasz> buf;
  unsigned char a[4] = {1,2,3,4};
  BOOST_CHECK(enc.aad(a, 4));
  BOOST_CHECK_EQUAL(enc.process(16, buf), 16);
  BOOST_CHECK(! enc.aad(a, 4));
  unsigned char t[16];
  BOOST_CHECK(! enc.tag(t)); // not before finish
}

// C bindings

BOOST_AUTO_TEST_CASE( c_streaming_roundtrip )
{
  CKey256 *k = mk_Key256();
  CKey96 *n = mk_Key96();
  const unsigned char aad[5] = {9, 8, 7, 6, 5};
  bytes pt = pattern(3000);
  bytes ct = pt;

  CAesGcmEncrypt *e = mk_AesGcmEncrypt(k, n);
  BOOST_REQUIRE(e);
  BOOST_CHECK(aad_AesGcmEncrypt(e, aad, 5));
  BOOST_CHECK_EQUAL(proc_AesGcmEncrypt(e, ct.size(), ct.data()), (int)pt.size());
  BOOST_CHECK_EQUAL(fin_AesGcmEncrypt(e, nullptr), 0);
  unsigned char tag[16];
  BOOST_CHECK(tag_AesGcmEncrypt(e, tag));
  release_AesGcmEncrypt(e);

  CAesGcmDecrypt *d = mk_AesGcmDecrypt(k, n);
  BOOST_REQUIRE(d);
  BOOST_CHECK(aad_AesGcmDecrypt(d, aad, 5));
  BOOST_CHECK(settag_AesGcmDecrypt(d, tag));
  BOOST_CHECK_EQUAL(proc_AesGcmDecrypt(d, ct.size(), ct.data()), (int)pt.size());
  BOOST_CHECK_EQUAL(fin_AesGcmDecrypt(d, nullptr), 0);
  BOOST_CHECK(ct == pt);
  release_AesGcmDecrypt(d);

  // wrong tag
  CAesGcmDecrypt *d2 = mk_AesGcmDecrypt(k, n);
  tag[0] ^= 1;
  BOOST_CHECK(aad_AesGcmDecrypt(d2, aad, 5));
  BOOST_CHECK(settag_AesGcmDecrypt(d2, tag));
  BOOST_CHECK_EQUAL(proc_AesGcmDecrypt(d2, ct.size(), ct.data()), (int)pt.size());
  BOOST_CHECK_EQUAL(fin_AesGcmDecrypt(d2, nullptr), -1);
  release_AesGcmDecrypt(d2);

  release_Key256(k); release_Key96(n);
}

BOOST_AUTO_TEST_CASE( c_oneshot_roundtrip )
{
  CKey256 *k = mk_Key256();
  CKey96 *n = mk_Key96();
  const bytes aad = pattern(12);
  const bytes pt = pattern(2 * lxr::AesGcm::datasz + 77);
  const int dlen = pt.size();

  bytes buf(dlen + 16);
  memcpy(buf.data(), pt.data(), dlen);
  BOOST_CHECK_EQUAL(cpp_process_aes256_gcm(1, n, k, aad.data(), aad.size(), buf.data(), buf.size(), dlen), dlen + 16);

  // agrees with the streaming API
  CAesGcmEncrypt *e = mk_AesGcmEncrypt(k, n);
  bytes ct = pt;
  aad_AesGcmEncrypt(e, aad.data(), aad.size());
  for (size_t pos = 0; pos < ct.size(); pos += lxr::AesGcm::datasz) {
    const size_t len = std::min<size_t>(lxr::AesGcm::datasz, ct.size() - pos);
    BOOST_CHECK_EQUAL(proc_AesGcmEncrypt(e, len, ct.data() + pos), (int)len);
  }
  fin_AesGcmEncrypt(e, nullptr);
  unsigned char tag[16];
  BOOST_CHECK(tag_AesGcmEncrypt(e, tag));
  release_AesGcmEncrypt(e);
  BOOST_CHECK(bytes(buf.begin(), buf.begin() + dlen) == ct);
  BOOST_CHECK(memcmp(buf.data() + dlen, tag, 16) == 0);

  // too-small output buffer
  bytes small(dlen + 15);
  memcpy(small.data(), pt.data(), dlen);
  BOOST_CHECK_EQUAL(cpp_process_aes256_gcm(1, n, k, aad.data(), aad.size(), small.data(), small.size(), dlen), -1);

  // decrypt
  bytes enc = buf;
  BOOST_CHECK_EQUAL(cpp_process_aes256_gcm(-1, n, k, aad.data(), aad.size(), enc.data(), enc.size(), dlen + 16), dlen);
  BOOST_CHECK(bytes(enc.begin(), enc.begin() + dlen) == pt);

  // tampered: zeroed and -1
  bytes bad = buf; bad[5] ^= 1;
  BOOST_CHECK_EQUAL(cpp_process_aes256_gcm(-1, n, k, aad.data(), aad.size(), bad.data(), bad.size(), dlen + 16), -1);
  BOOST_CHECK(std::all_of(bad.begin(), bad.end(), [](unsigned char c){ return c == 0; }));

  // input shorter than a tag
  bytes tiny(8);
  BOOST_CHECK_EQUAL(cpp_process_aes256_gcm(-1, n, k, nullptr, 0, tiny.data(), tiny.size(), 8), -1);

  release_Key256(k); release_Key96(n);
}

BOOST_AUTO_TEST_SUITE_END()
