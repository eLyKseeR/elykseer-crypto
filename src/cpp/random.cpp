module;
/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2026 Alexander Diemand

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

#include <cstddef>
#include <limits>
#include <memory>
#include <random>
#include <stdexcept>
#include <stdint.h>

#if CRYPTOLIB == OPENSSL
#include "openssl/rand.h"
#endif

#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/osrng.h"
#endif


module lxr_random;


namespace lxr {


void Random::fill(unsigned char *buf, std::size_t len)
{
#if CRYPTOLIB == OPENSSL
    while (len > 0) {
        const int n = len > (std::size_t)std::numeric_limits<int>::max()
                    ? std::numeric_limits<int>::max() : (int)len;
        if (RAND_bytes(buf, n) != 1) {
            throw std::runtime_error("RAND_bytes failed");
        }
        buf += n; len -= n;
    }
#elif CRYPTOLIB == CRYPTOPP
    CryptoPP::OS_GenerateRandomBlock(false, buf, len);
#else
#error "no crypto library selected for random number generation"
#endif
}

// UniformRandomBitGenerator on top of the system CSPRNG
struct csprng32 {
    using result_type = uint32_t;
    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
    result_type operator()() {
        result_type r;
        Random::fill(reinterpret_cast<unsigned char*>(&r), sizeof(r));
        return r;
    }
};

struct Random::pimpl {
};

Random::Random()
    : _pimpl(new Random::pimpl)
{}

Random::~Random()
{
    if (_pimpl) {
        _pimpl.reset();
    }
}

Random& Random::rng() {
    static Random _rng;
    return _rng;
}

uint32_t Random::random() const
{
    return csprng32{}();
}

uint32_t Random::random(uint32_t max) const
{
    csprng32 gen;
    return std::uniform_int_distribution<uint32_t>(0, max - 1)(gen);
}

} // namespace