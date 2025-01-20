module;
/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2025 Alexander Diemand

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>

#if CRYPTOLIB == CRYPTOPP
#include <cryptopp/osrng.h>
#else
#error not yet defined
#endif


module lxr_random;


namespace lxr {

#if CRYPTOLIB == CRYPTOPP

static std::unique_ptr<CryptoPP::AutoSeededX917RNG<CryptoPP::AES>> _rng;

Random::Random()
{
    if (! _rng) {
        _rng.reset(new CryptoPP::AutoSeededX917RNG<CryptoPP::AES>);
    }
}

Random::~Random()
{
    if (_rng) {
        _rng.reset();
    }
}

#else
#error not yet defined
#endif


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

} // namespace