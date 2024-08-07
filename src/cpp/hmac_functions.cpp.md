declared in [Md5](md5.hpp.md)

```cpp
# if CRYPTOLIB == CRYPTOPP

Key128 HMAC::hmac_md5(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_md5(k, klen, msg.c_str(), msg.size());
}

Key128 HMAC::hmac_md5(const char k[], int klen, const char buffer[], int blen)
{
    assert(128/8 == CryptoPP::Weak::MD5::DIGESTSIZE);
    unsigned char digest[CryptoPP::Weak::MD5::DIGESTSIZE];
    // CryptoPP::Weak::MD5 hash;
    CryptoPP::HMAC<CryptoPP::Weak::MD5> hmac((const CryptoPP::byte *)k, klen);
    hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);

    Key128 k128(true);
    k128.fromBytes(digest);
    return k128;
}

Key256 HMAC::hmac_sha256(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_sha256(k, klen, msg.c_str(), msg.size());
}

Key256 HMAC::hmac_sha256(const char k[], int klen, const char buffer[], int blen)
{
    assert(256/8 == CryptoPP::SHA256::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::HMAC<CryptoPP::SHA256> hmac((const CryptoPP::byte *)k, klen);
    hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);

    Key256 k256(true);
    k256.fromBytes(digest);
    return k256;
}

Key160 HMAC::hmac_sha1(const char k[], int klen, std::string const & msg)
{
    return HMAC::hmac_sha1(k, klen, msg.c_str(), msg.size());
}

Key160 HMAC::hmac_sha1(const char k[], int klen, const char buffer[], int blen)
{
    assert(160/8 == CryptoPP::SHA1::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA1::DIGESTSIZE];
    CryptoPP::HMAC<CryptoPP::SHA1> hmac((const CryptoPP::byte *)k, klen);
    hmac.CalculateDigest(digest, (unsigned char const *)buffer, blen);

    Key160 k160(true);
    k160.fromBytes(digest);
    return k160;
}

# endif 
```
