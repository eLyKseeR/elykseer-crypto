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

#include <string_view>
#include <cstring>

#include "lxr-cbindings.hpp"


module lxr_base64;


extern "C" EXPORT
int base64_encode(int mlen, const char * m, int outlen, unsigned char * out)
{
    auto tr = lxr::Base64::encode(std::string_view(m, mlen));
    int sz = tr.size();
    unsigned int copied = std::min(outlen, sz);
    std::memcpy(out, tr.c_str(), copied);
    return copied;
}

extern "C" EXPORT
int base64_decode(int mlen, const char * m, int outlen, unsigned char * out)
{
    auto tr = lxr::Base64::decode(std::string_view(m, mlen));
    int sz = tr.size();
    unsigned int copied = std::min(outlen, sz);
    std::memcpy(out, tr.c_str(), copied);
    return copied;
}

