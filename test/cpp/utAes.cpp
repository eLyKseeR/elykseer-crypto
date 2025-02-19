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
#include <fstream>
#include <sstream>
#include <cstring>

#include "boost/test/unit_test.hpp"

#include "sizebounded/sizebounded.ipp"

import lxr_aes;
import lxr_key128;
import lxr_key256;


BOOST_AUTO_TEST_SUITE( utAes )

// Test case: encrypt then decrypt a small buffer
BOOST_AUTO_TEST_CASE( small_encrypt_then_decrypt )
{
  const std::string msg = "all my precious data are safe, so I will sleep fine!";

  lxr::Key256 _k;
  lxr::Key128 _iv;
  lxr::AesEncrypt _aesenc(_k, _iv);
  sizebounded<unsigned char, lxr::Aes::datasz> buf;
  buf.transform([&msg](const int i, const char c0)->char {
      if (i < msg.size()) { return msg[i]; }
      else { return '\0'; }
      });
  int lenc = 0;
  try {
    lenc = _aesenc.process(msg.size(), buf);
  } catch (std::exception & e) {
    std::clog << "exception " << e.what() << std::endl;
  }
  // std::clog << "encrypted " << lenc << " bytes." << std::endl;
  lenc += _aesenc.finish(lenc, buf);
  // std::clog << "finished: " << lenc << " bytes." << std::endl;

  // compare buffer
  std::string msg1 = buf.toString().substr(0, msg.size());
  // std::clog << msg1 << std::endl;
  BOOST_CHECK_NE(msg, msg1);

  // decrypt and compare to original message
  lxr::AesDecrypt _aesdec(_k, _iv);
  int ldec = 0;
  ldec = _aesdec.process(lenc, buf);
  ldec += _aesdec.finish(ldec, buf);

  std::string msg2 = buf.toString().substr(0, msg.size());

  BOOST_CHECK_EQUAL(msg, msg2);
}

// Test case: encrypt then decrypt a large buffer
std::string encrypt_decrypt_test(std::string const & part, int counter)
{
  std::string msg;
  for (int i=0; i<counter; i++) {
    msg += part;
  }

  int tot_len = msg.size();
  int proc_len = 0;
  int last_len = 0;
  int tot_proc = 0;

  lxr::Key256 _k;
  lxr::Key128 _iv;
  lxr::AesEncrypt _aesenc(_k, _iv);
  sizebounded<unsigned char, lxr::Aes::datasz> buf;
  std::string cipher;
  int lenc = 0;

  while (proc_len < tot_len) {
    last_len = proc_len;
    // fill buffer
    buf.transform([&msg,&tot_len,&proc_len](const int i, const char c0)->char {
        if (proc_len < tot_len) { return msg[proc_len++]; }
        else { return '\0'; }
        });
    lenc = _aesenc.process(proc_len-last_len, buf);
    tot_proc += lenc;
    //std::clog << "encrypted " << lenc << " bytes." << std::endl;
    cipher += std::string((const char*)buf.ptr(), lenc);
  }
  lenc = _aesenc.finish(0, buf);
  tot_proc += lenc;
  //std::clog << "finished: " << lenc << " bytes => total " << tot_proc << " bytes." << std::endl;
  if (lenc > 0) {
    cipher += std::string((const char*)buf.ptr(), lenc); }

  // decrypt and compare to original message
  lxr::AesDecrypt _aesdec(_k, _iv);
  int ldec = 0;
  std::string cleartext;
  tot_len = cipher.size();
  proc_len = 0;
  last_len = 0;
  tot_proc = 0;
  while (proc_len < tot_len) {
    last_len = proc_len;
    // fill buffer
    buf.transform([&cipher,&tot_len,&proc_len](const int i, const char c0)->char {
        if (proc_len < tot_len) { return cipher[proc_len++]; }
        else { return '\0'; }
        });
    ldec = _aesdec.process(proc_len-last_len, buf);
    tot_proc += ldec;
    //std::clog << "decrypted " << ldec << " bytes." << std::endl;
    cleartext += std::string((const char*)buf.ptr(), ldec);
  }
  ldec = _aesdec.finish(0, buf);
  tot_proc += ldec;
  //std::clog << "finished: " << lenc << " bytes => total " << tot_proc << " bytes." << std::endl;
  if (ldec > 0) {
    cleartext += std::string((const char*)buf.ptr(), ldec); }

  return cleartext;
}

std::string s_repeat(std::string const & s0, int ct)
{
  std::ostringstream ss;
  while (ct-- > 0) {
    ss << s0;
  }
  return ss.str();
}

BOOST_AUTO_TEST_CASE( large_encrypt_then_decrypt )
{
  const std::string part = "all my precious data are save, so I will sleep fine!\\n";

  for (int k = 0; k < 99; k++) {
    std::string msg = s_repeat(part, k);
    std::string msg2 = encrypt_decrypt_test(part, k);
    BOOST_CHECK_EQUAL(msg, msg2);
  }
}

// Test case in C: encrypt then decrypt a small buffer
BOOST_AUTO_TEST_CASE( c_small_encrypt_then_decrypt )
{
  const unsigned char msg[] = "all my precious data are save, so I will sleep fine!";
  unsigned char buf[lxr::Aes::datasz];

  CKey256 *_k = mk_Key256();
  CKey128 *_iv = mk_Key128();
  CAesEncrypt * _aesenc = mk_AesEncrypt(_k, _iv);
  int mlen = std::strlen((const char*)msg);
  int lenc = 0;
  int copied = 0;
  try {
      memcpy(buf, msg, mlen);
      lenc = proc_AesEncrypt(_aesenc, mlen, buf);
      if (lenc > 0) {
          copied += copy_AesEncrypt(_aesenc, lenc, buf);
      }
  } catch (std::exception & e) {
      std::clog << "exception " << e.what() << std::endl;
  }
  int flenc = fin_AesEncrypt(_aesenc);
  if (flenc > 0) {
      lenc += flenc;
      copied += copy_AesEncrypt(_aesenc, flenc, buf+copied);
  }
  //std::clog << "encrypted " << lenc << " bytes." << std::endl;
  //std::clog << "finished: " << flenc << " bytes." << std::endl;
  memcpy(buf, _aesenc->buf, copied); // copy ciphertext into buffer
  release_AesEncrypt(_aesenc);

  // decrypt and compare to original message
  CAesDecrypt * _aesdec = mk_AesDecrypt(_k, _iv);
  int ldec = 0;
  copied = 0;
  ldec = proc_AesDecrypt(_aesdec, lenc, buf);
  if (ldec > 0) {
      copied += copy_AesDecrypt(_aesdec, ldec, buf);
  }
  int fldec = fin_AesDecrypt(_aesdec);
  if (fldec > 0) {
      ldec += fldec;
      copied += copy_AesDecrypt(_aesdec, fldec, buf+copied);
  }
  release_AesDecrypt(_aesdec);

  std::string msg1((const char*)msg, mlen);
  std::string msg2((const char*)buf, ldec);

  BOOST_CHECK_EQUAL(msg1, msg2);
}

// Test case in C: encrypt then decrypt a small buffer
// data buffers to pass data in and out are of different size
BOOST_AUTO_TEST_CASE( c_encrypt_decrypt_with_databuffers_sz )
{
  const unsigned char msg[] = "all my precious data are save, so I will sleep fine!"
                              "02345678901234567890123456789012345678901234567891"
                              "01234567890134567890123456789012345678901234567892"
                              "01234567890123456789012456789012345678901234567893"
                              "01234567890123456789012345678901235678901234567894"
                              "01234567890123456789012345678901234567890123467895"
                              "01234567891234567890123456789012345678901234567890"
                              "01234567890123456789012345678903456789012345678912"
                              "01234567890123456789012345690123456789012345678978"
                              "01234567890123456789012567890123456789012345678934"
                              "01234567890123678901234567890123456789012345678945"
                              "01234567890123456789012345678901234567890123456789";
  unsigned char bufin[lxr::Aes::datasz];
  unsigned char bufout[lxr::Aes::datasz];

  CKey256 *_k = mk_Key256();
  CKey128 *_iv = mk_Key128();
  CAesEncrypt * _aesenc = mk_AesEncrypt(_k, _iv);
  int mlen = std::min((unsigned long)lxr::Aes::datasz, sizeof(msg));
  int lenc = 0;
  int copied = 0;
  try {
      memcpy(bufin, msg, mlen);
      lenc = proc_AesEncrypt(_aesenc, mlen, bufin);
      if (lenc > 0) {
          copied += copy_AesEncrypt(_aesenc, 12, bufout);
          copied += copy_AesEncrypt(_aesenc, lxr::Aes::datasz - 12, bufout+copied);
      } else {
          throw "encryption error: proc_AesEncrypt";
      }
  } catch (std::exception & e) {
      std::clog << "exception " << e.what() << std::endl;
  }
  int flenc = fin_AesEncrypt(_aesenc);
  if (flenc < 0) { throw "encryption error: fin_AesEncrypt"; }
  lenc += flenc;
  copied += copy_AesEncrypt(_aesenc, lxr::Aes::datasz - copied, bufout+copied);
  // std::clog << "encrypted " << lenc << " bytes." << std::endl;
  // std::clog << "finished: " << flenc << " bytes." << std::endl;
  // std::clog << "copied: " << copied << " bytes." << std::endl;
  release_AesEncrypt(_aesenc);

  std::string msg1((const char*)msg, mlen);
  std::string msg2((const char*)bufout, lenc);
  BOOST_CHECK_NE(msg1, msg2);

  // decrypt and compare to original message
  memset(bufin, '.', lxr::Aes::datasz);
  CAesDecrypt * _aesdec = mk_AesDecrypt(_k, _iv);
  int ldec = 0;
  copied = 0;
  ldec = proc_AesDecrypt(_aesdec, lenc, bufout);
  if (ldec > 0) {
      copied += copy_AesDecrypt(_aesdec, 17, bufin);
      copied += copy_AesDecrypt(_aesdec, lxr::Aes::datasz - 17, bufin+copied);
  } else {
      throw "decryption error: proc_AesDecrypt";
  }
  int fldec = fin_AesDecrypt(_aesdec);
  if (fldec < 0) { throw "decryption error: proc_AesDecrypt"; }
  ldec += fldec;
  copied += copy_AesDecrypt(_aesdec, lxr::Aes::datasz - copied, bufin+copied);
  // std::clog << "decrypted " << ldec << " bytes." << std::endl;
  // std::clog << "finished: " << fldec << " bytes." << std::endl;
  // std::clog << "copied: " << copied << " bytes." << std::endl;
  release_AesDecrypt(_aesdec);

  std::string msg3((const char*)bufin, ldec);

  BOOST_CHECK_EQUAL(msg1, msg3);
}

BOOST_AUTO_TEST_SUITE_END()