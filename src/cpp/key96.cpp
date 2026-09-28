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
#include <memory>
#include <string>

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

import lxr_key;


module lxr_key96;


namespace lxr {

struct Key96::pimpl {
    sizebounded<unsigned char, 96/8> _buffer;
};


Key96::Key96(bool noinit)
{
    if (!noinit) {
        _pimpl.reset(new Key96::pimpl);
        randomize();
    }
}

Key96::~Key96() = default;

Key96::Key96(Key96 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key96::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
}

Key96 & Key96::operator=(Key96 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key96::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
    return *this;
}

Key96 Key96::keyFromHex(std::string const &s)
{
    Key96 key{true};
    key.fromHex(s);
    return std::move(key);
}


unsigned char const* Key96::bytes() const
{
    if (! _pimpl) { throw("uninitialised"); }
    return _pimpl->_buffer.ptr();
}

void Key96::map(std::function<void(const int, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.map(f);
}

void Key96::zip(Key96 const & other, std::function<void(const unsigned char, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.zip(f, other._pimpl->_buffer);
}

void Key96::transform(std::function<unsigned char(const int, const unsigned char)> f)
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.transform(f);
}

void Key96::fromHex(std::string const &h)
{
    _pimpl.reset(new Key96::pimpl);
    Key::fromHex(h);
}

void Key96::fromBytes(unsigned char const *h)
{
    _pimpl.reset(new Key96::pimpl);
    Key::fromBytes(h);
}

bool Key96::operator==(Key96 const & other) const
{
    if (! _pimpl) { throw("uninitialised"); }
    if (length() != other.length()) { return false; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 == c2);
    });
    return res;
}

bool Key96::operator!=(Key96 const & other) const
{
    if (! _pimpl) { throw("uninitialised"); }
    if (length() != other.length()) { return true; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 != c2);
    });
    return res;
}

} // namespace