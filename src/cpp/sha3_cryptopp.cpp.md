declared in [Sha3](sha3.hpp.md)

```cpp

#if CRYPTOLIB == CRYPTOPP
Key256 Sha3_256::hash(const char buffer[], int length)
{
    assert(256/8 == CryptoPP::SHA3_256::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA3_256::DIGESTSIZE];
    CryptoPP::SHA3_256 hash;
    hash.CalculateDigest( digest, (const unsigned char*)buffer, length );

    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

Key256 Sha3_256::hash(std::filesystem::path const & fpath)
{
    Sha3_256 sha3_256;
    sizebounded<unsigned char, Sha3_256::datasz> buf;
    std::ifstream infile(fpath.string());
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha3_256::datasz);
        sha3_256.process(infile.gcount(), buf);
    }
    return sha3_256.finish();
}

int Sha3_256::process(int len, sizebounded<unsigned char, Sha3_256::datasz> const & buf)
{
    if (len <= Sha3_256::datasz) {
        _pimpl->hash.Update(buf.ptr(), len);
        return len;
    }
    return 0;
}

Key256 Sha3_256::finish()
{
    unsigned char digest[CryptoPP::SHA3_256::DIGESTSIZE];
    _pimpl->hash.Final(digest);
    Key256 k(true);
    k.fromBytes(digest);
    return k;
}
#endif

```
