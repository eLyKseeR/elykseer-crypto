declared in [Random](random.hpp.md)

```cpp

#if CRYPTOLIB == CRYPTOPP
uint32_t rnd_uint32() {
    const unsigned int BLOCKSIZE = 4 * 8;
    CryptoPP::SecByteBlock scratch(BLOCKSIZE);
    _rng->GenerateBlock(scratch, scratch.size());
    return *(uint32_t*)(scratch.BytePtr());
}
#else
#error not yet defined
#endif

uint32_t Random::random() const
{
    return rnd_uint32();
}

uint32_t Random::random(int max) const
{
    uint32_t r0 = rnd_uint32();
    double r = double(r0) * double(max) / (pow(2, 32) - 1) - 0.5;
    long int r1 = lrint(r);
    return (uint32_t)r1;
}

```