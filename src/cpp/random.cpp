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

#include <memory>
#include <random>
#include <mutex>
#include <stdint.h>


module lxr_random;


namespace lxr {


struct Random::pimpl {
    pimpl() {
        std::random_device rng_dev;
        rng_gen.reset(new std::mt19937(rng_dev()));
    }
    std::unique_ptr<std::mt19937> rng_gen{};
    std::mutex rng_mutex;
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
    std::lock_guard<std::mutex> lock(_pimpl->rng_mutex);
    return std::uniform_int_distribution<uint32_t>(0)(*_pimpl->rng_gen);
}

uint32_t Random::random(uint32_t max) const
{
    std::lock_guard<std::mutex> lock(_pimpl->rng_mutex);
    return std::uniform_int_distribution<uint32_t>(0, max - 1)(*_pimpl->rng_gen);
}

} // namespace