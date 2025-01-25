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

#include <iostream>
#include <string>
#include <memory>
#include <functional>


export module lxr_key;


export namespace lxr {

class Key
{
    public:
        virtual int length() const = 0;
        virtual unsigned char const* bytes() const = 0;
        virtual std::string toHex() const;
    protected:
        Key() = default;
        virtual void fromHex(std::string const &);
        virtual void fromBytes(unsigned char const*);
        virtual void randomize() final;
        virtual void map(std::function<void(const int, const unsigned char)>) const = 0;
        virtual void transform(std::function<unsigned char(const int, const unsigned char)>) = 0;
};

std::ostream & operator<<(std::ostream & os, Key const & k);

} // namespace