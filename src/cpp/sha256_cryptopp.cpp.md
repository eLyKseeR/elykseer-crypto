declared in [Sha256](sha256.hpp.md)

```cpp

#if CRYPTOLIB == CRYPTOPP
Key256 Sha256::hash(std::string const & msg)
{
    return Sha256::hash(msg.c_str(), msg.size());
}

Key256 Sha256::hash(const char buffer[], int length)
{
    assert(256/8 == CryptoPP::SHA3_256::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA3_256::DIGESTSIZE];
    CryptoPP::SHA3_256 hash;
    hash.CalculateDigest( digest, (const unsigned char*)buffer, length );

    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

Key256 Sha256::hash(std::filesystem::path const & fpath)
{
    unsigned char digest[CryptoPP::SHA3_256::DIGESTSIZE];
    CryptoPP::SHA3_256 hash;

    Key256 k(true);
    unsigned char buf[1024];
    std::ifstream infile(fpath);
    while (infile.good()) {
        infile.read((char*)buf, 1024);
        hash.Update(buf, infile.gcount());
    }
    hash.Final(digest);
    k.fromBytes(digest);
    return k;
}
#endif

```
