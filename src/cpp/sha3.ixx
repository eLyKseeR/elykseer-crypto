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

#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>

#include "sizebounded/sizebounded.hpp"

import lxr_key256;


export module lxr_sha3;


export namespace lxr {

class Sha3_256
{
    public:
        static Key256 hash(std::string const &);
        static Key256 hash(const char[], int length);
        static Key256 hash(std::filesystem::path const &);
        Sha3_256();
        ~Sha3_256();
        static constexpr unsigned int datasz { 1024*64 };
        int process(int inlen, sizebounded<unsigned char, Sha3_256::datasz>const & inbuf);
        Key256 finish();
    private:
        struct pimpl;
        std::unique_ptr<pimpl> _pimpl;
        Sha3_256(Sha3_256 const &) = delete;
        Sha3_256 & operator=(Sha3_256 const &) = delete;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" {
export CKey256* hash_Sha3_256(int len, const char *s);

export CKey256* filehash_Sha3_256(const char * fp);
}