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

#include <filesystem>

#include "lxr-cbindings.hpp"

import lxr_key256;


module lxr_sha3;


extern "C" EXPORT
CKey256* hash_Sha3_256(int len, const char * inbuf)
{
    auto h = lxr::Sha3_256::hash(inbuf, len);
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}

extern "C" EXPORT
CKey256* filehash_Sha3_256(const char * fp)
{
    auto h = lxr::Sha3_256::hash(std::filesystem::path(fp));
    CKey256 * r = new CKey256; r->ptr = new lxr::Key256(h);
    return r;
}