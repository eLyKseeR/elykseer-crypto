```cpp
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "boost/test/unit_test.hpp"

#include "lxr/aes.hpp"
#include "lxr/key256.hpp"
#include "lxr/key128.hpp"
#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
````

# Test suite: utAes

on class [Aes](../src/aes.hpp.md)

```cpp
BOOST_AUTO_TEST_SUITE( utAes )
```
## Test case: encrypt then decrypt a small buffer
```cpp
BOOST_AUTO_TEST_CASE( small_encrypt_then_decrypt )
{
  const std::string msg = "all my precious data are save, so I will sleep fine!";

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
  //std::clog << "encrypted " << lenc << " bytes." << std::endl;
  lenc += _aesenc.finish(lenc, buf);
  //std::clog << "finished: " << lenc << " bytes." << std::endl;

  // decrypt and compare to original message
  lxr::AesDecrypt _aesdec(_k, _iv);
  int ldec = 0;
  ldec = _aesdec.process(lenc, buf);
  ldec += _aesdec.finish(ldec, buf);

  std::string msg2 = buf.toString().substr(0, msg.size());

  BOOST_CHECK_EQUAL(msg, msg2);
}
```

## Test case: encrypt then decrypt a large buffer
```cpp

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
```

## Test case in C: encrypt then decrypt a small buffer
```cpp
BOOST_AUTO_TEST_CASE( c_small_encrypt_then_decrypt )
{
  const unsigned char msg[] = "all my precious data are save, so I will sleep fine!";
  unsigned char buf[lxr::Aes::datasz];

  CKey256 *_k = mk_Key256();
  CKey128 *_iv = mk_Key128();
  CAesEncrypt * _aesenc = mk_AesEncrypt(_k, _iv);
  int mlen = std::strlen((const char*)msg);
  int lenc = 0;
  try {
    memcpy(buf, msg, mlen);
    lenc = proc_AesEncrypt(_aesenc, mlen, buf);
  } catch (std::exception & e) {
    std::clog << "exception " << e.what() << std::endl;
  }
  // std::clog << "encrypted " << lenc << " bytes." << std::endl;
  lenc += fin_AesEncrypt(_aesenc, lxr::Aes::datasz, buf);
  // std::clog << "finished: " << lenc << " bytes." << std::endl;
  delete _aesenc;

  // decrypt and compare to original message
  CAesDecrypt * _aesdec = mk_AesDecrypt(_k, _iv);
  int ldec = 0;
  ldec = proc_AesDecrypt(_aesdec, lenc, buf);
  ldec += fin_AesDecrypt(_aesdec, lxr::Aes::datasz, buf);
  delete _aesdec;

  char *hex = tohex_Key128(_iv);
  // std::clog << "iv used: " << hex << std::endl;

  std::string msg1((const char*)msg, mlen);
  std::string msg2((const char*)buf, ldec);

  BOOST_CHECK_EQUAL(msg1, msg2);
}
```

```cpp
BOOST_AUTO_TEST_SUITE_END()
```
