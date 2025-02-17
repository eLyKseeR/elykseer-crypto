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
#include <memory>
#include <string>

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

import lxr_key;


module lxr_key128;


namespace lxr {

struct Key128::pimpl {
    sizebounded<unsigned char, 128/8> _buffer;
};


Key128::Key128(bool noinit)
{
    if (!noinit) {
        _pimpl.reset(new Key128::pimpl);
        randomize();
    }
}

Key128::~Key128() = default;

Key128::Key128(Key128 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key128::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
}

Key128 & Key128::operator=(Key128 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key128::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
    return *this;
}

Key128 Key128::key128FromHex(std::string const &s)
{
    Key128 key{true};
    key.fromHex(s);
    return key;
}


unsigned char const* Key128::bytes() const
{
    if (! _pimpl) { throw("uninitialised"); }
    return _pimpl->_buffer.ptr();
}

void Key128::map(std::function<void(const int, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.map(f);
}

void Key128::zip(Key128 const & other, std::function<void(const unsigned char, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.zip(f, other._pimpl->_buffer);
}

void Key128::transform(std::function<unsigned char(const int, const unsigned char)> f)
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.transform(f);
}

void Key128::fromHex(std::string const &h)
{
    _pimpl.reset(new Key128::pimpl);
    Key::fromHex(h);
}

void Key128::fromBytes(unsigned char const *h)
{
    _pimpl.reset(new Key128::pimpl);
    Key::fromBytes(h);
}

bool Key128::operator==(Key128 const & other) const
{
    if (! _pimpl) { throw("uninitialised"); }
    if (length() != other.length()) { return false; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 == c2);
    });
    return res;
}

bool Key128::operator!=(Key128 const & other) const
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