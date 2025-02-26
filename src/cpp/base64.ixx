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
#include <string_view>


export module lxr_base64;


export namespace lxr {

class Base64
{
    public:
        static std::string encode(std::string_view const & m);
        static std::string decode(std::string_view const & m);
    protected:
        Base64() {}
    private:
        Base64(Base64 const &) = delete;
        Base64 & operator=(Base64 const &) = delete;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

extern "C" {
export int base64_encode(int mlen, const char * m, int outlen, unsigned char * out);

export int base64_decode(int mlen, const char * m, int outlen, unsigned char * out);
}