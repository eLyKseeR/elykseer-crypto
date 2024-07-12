declared in [Sha256](sha256.hpp.md)

```cpp

#if CRYPTOLIB == CRYPTOPP
Key256 Sha256::hash(const char buffer[], int length)
{
    assert(256/8 == CryptoPP::SHA256::DIGESTSIZE);
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256 hash;
    hash.CalculateDigest( digest, (const unsigned char*)buffer, length );

    Key256 k(true);
    k.fromBytes(digest);
    return k;
}

Key256 Sha256::hash(std::filesystem::path const & fpath)
{
    Sha256 sha256;
    sizebounded<unsigned char, Sha256::datasz> buf;
    std::ifstream infile(fpath.string());
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha256::datasz);
        sha256.process(infile.gcount(), buf);
    }
    return sha256.finish();
}

int Sha256::process(int len, sizebounded<unsigned char, Sha256::datasz> const & buf)
{
    if (len <= Sha256::datasz) {
        _pimpl->hash.Update(buf.ptr(), len);
        return len;
    }
    return 0;
}

Key256 Sha256::finish()
{
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    _pimpl->hash.Final(digest);
    Key256 k(true);
    k.fromBytes(digest);
    return k;
}
#endif

```
