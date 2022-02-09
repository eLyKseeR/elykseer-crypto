declared in [Random](random.hpp.md)

```cpp

#if CRYPTOLIB == CRYPTOPP

static std::unique_ptr<CryptoPP::AutoSeededX917RNG<CryptoPP::AES>> _rng;

Random::Random()
{
    if (! _rng) {
        _rng.reset(new CryptoPP::AutoSeededX917RNG<CryptoPP::AES>);
    }
}

#else
#error not yet defined
#endif

```