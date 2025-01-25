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

#include <string>

import lxr_key128;


export module lxr_md5;


export namespace lxr {

class Md5
{
    public:
        static Key128 hash(std::string const &);
        static Key128 hash(const char[], int length);
        // static Key128 hash(std::filesystem::path const &);
    protected:
        Md5() {}
    private:
        Md5(Md5 const &) = delete;
        Md5 & operator=(Md5 const &) = delete;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" EXPORT
export CKey128* hash_Md5(int len, const char *);
