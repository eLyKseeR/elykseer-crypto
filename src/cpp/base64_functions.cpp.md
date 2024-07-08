declared in [Md5](md5.hpp.md)

```cpp
# if CRYPTOLIB == CRYPTOPP

std::string Base64::encode(std::string const & m)
{
    std::string out;
    CryptoPP::Base64Encoder filter;
    CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(CryptoPP::Name::InsertLineBreaks(), false);
    filter.IsolatedInitialize(params);
    filter.Detach(new CryptoPP::StringSink(out));
    filter.Put((const unsigned char *)m.c_str(), m.size());
    filter.MessageEnd();
    return std::move(out);
}

std::string Base64::decode(std::string const & m)
{
    std::string out;
    CryptoPP::Base64Decoder filter;
    filter.Detach(new CryptoPP::StringSink(out));
    filter.Put((const unsigned char *)m.c_str(), m.size());
    filter.MessageEnd();
    return std::move(out);
}

# endif 
```
