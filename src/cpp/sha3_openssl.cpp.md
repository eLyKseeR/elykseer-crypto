declared in [Sha3](sha3.hpp.md)

```cpp

#if CRYPTOLIB == OPENSSL
Key256 Sha3_256::hash(const char buffer[], int length)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const EVP_MD *md = EVP_sha3_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);

    EVP_DigestUpdate(ctx, buffer, length);
    EVP_DigestFinal_ex(ctx, digest, nullptr);

    Key256 k(true);
    k.fromBytes(digest);
    EVP_MD_CTX_free(ctx);
    return k;
}

Key256 Sha3_256::hash(std::filesystem::path const & fpath)
{
    Sha3_256 sha3_256;
    sizebounded<unsigned char, Sha3_256::datasz> buf;
    std::filesystem::ifstream infile(fpath);
    while (infile.good()) {
        infile.read((char*)buf.ptr(), Sha3_256::datasz);
        sha3_256.process(infile.gcount(), buf);
    }
    k.fromBytes(digest);
    return sha3_256.finish();
}

int Sha3_256::process(int len, sizebounded<unsigned char, Sha3_256::datasz> const & buf)
{
    if (len <= Sha3_256::datasz) {
        EVP_DigestUpdate(_pimpl->ctx, buf->ptr(), len);
        return len;
    }
    return 0;
}

Key256 Sha3_256::finish()
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    EVP_DigestFinal_ex(_pimpl->ctx, digest, nullptr);
    Key256 k(true);
    k.fromBytes(digest);
    EVP_MD_CTX_free(_pimpl->ctx);
    return k;
}
#endif

```
