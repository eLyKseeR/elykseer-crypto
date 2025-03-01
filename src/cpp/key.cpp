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

#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <cstring>

import lxr_random;


module lxr_key;


namespace lxr {

int hex2int(unsigned char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        return -1;
    }
}

unsigned char char2hex(unsigned char c) {
    if (c <= 9) {
        return c + '0';
    } else if (c <= 16) {
        return c + 'a' - 10;
    } else {
        return '?';
    }
}

int int2hex(unsigned char c) {
    unsigned char c1 = char2hex(c & 0x0f);
    unsigned char c2 = char2hex((c >> 4) & 0x0f);
    return (c2 << 8) | c1;
}

unsigned char const* Key::bytes() const
{
    return nullptr;
}

std::string Key::toHex() const
{
    const int alen = this->length() * 2 / 8;
    assert(alen <= 512*2);
    char buf[512*2+2]; // max alloc: 512 bit keys
    memset(buf, 0, 512*2+2);
    map([&buf](const int i, const unsigned char c) {
        int cc = int2hex(c);
        buf[2*i] = (cc >> 8) & 0xff;
        buf[2*i+1] = cc & 0xff;
    });
    return std::string(buf, alen);
}

void Key::fromHex(std::string const &k)
{
    transform([this,&k](const int i, const unsigned char c) -> unsigned char {
        if (i < length() * 2 / 8) {
            unsigned char c1 = k[i*2];
            unsigned char c2 = k[i*2+1];
            return (hex2int(c1)<<4 | hex2int(c2));
        } else {
            return 0;
        }
    });
}

void Key::fromBytes(unsigned char const *buf)
{
    transform([this,&buf](const int i, const unsigned char _c) -> unsigned char {
        if (i < length() / 8) {
            unsigned char c = buf[i];
            return buf[i];
        } else {
            return 0;
        }
    });
}

void Key::randomize()
{
    uint32_t r = 0;
    transform([&r](const int i, const unsigned char c) -> unsigned char {
        if (i % 4 == 0) {
            r = Random::rng().random(); }
        unsigned char c2 = r & 0xff;
        r = (r >> 8);
        return c2;
    });
}

std::ostream & operator<<(std::ostream & os, Key const & k)
{
    os << k.toHex();
    return os;
}

} // namespace