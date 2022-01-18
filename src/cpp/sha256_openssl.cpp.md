declared in [Sha256](sha256.hpp.md)

```cpp

#if CRYPTOLIB == OPENSSL
Key256 Sha256::hash(std::string const & msg)
{
    return Sha256::hash(msg.c_str(), msg.size());
}

Key256 Sha256::hash(const char buffer[], int length)
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

Key256 Sha256::hash(boost::filesystem::path const & fpath)
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    const EVP_MD *md = EVP_sha3_256();
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, md, nullptr);

    Key256 k(true);
    unsigned char buf[1024];
    boost::filesystem::ifstream infile(fpath);
    while (infile.good()) {
        infile.read((char*)buf, 1024);
        EVP_DigestUpdate(ctx, buf, infile.gcount());
    }
    EVP_DigestFinal_ex(ctx, digest, nullptr);
    k.fromBytes(digest);
    EVP_MD_CTX_free(ctx);
    return k;
}
#endif

```
