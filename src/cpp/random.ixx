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
#include <stdint.h>


export module lxr_random;


export namespace lxr {

class Random
{
    public:
        // access the random number generator
        static Random& rng();

        uint32_t random() const;
        // generate a randum number in the interval [0,max), thus incl. zero but excl. max
        uint32_t random(uint32_t max) const;
    private:
        Random();
        ~Random();
        Random(Random const &) = delete;
        Random & operator=(Random const &) = delete;

        struct pimpl;
        mutable std::unique_ptr<pimpl> _pimpl;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" {
export uint32_t random_one();
export uint32_t random_upto(uint32_t max);
}