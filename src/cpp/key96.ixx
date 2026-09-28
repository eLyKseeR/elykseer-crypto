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

#include <functional>
#include <memory>
#include <string>

import lxr_key;


export module lxr_key96;


export namespace lxr {

class Key96 : public Key
{
    public:
        Key96(bool noinit = false);
        virtual ~Key96();
        Key96(Key96 const &);
        Key96 & operator=(Key96 const &);
        static Key96 keyFromHex(std::string const &);
        virtual unsigned char const* bytes() const override;
        virtual int length() const override { return 96; };
        virtual bool operator==(Key96 const &) const final;
        virtual bool operator!=(Key96 const &) const final;
        virtual void fromHex(std::string const &) override;
        virtual void fromBytes(unsigned char const*) override;
    protected:
        virtual void map(std::function<void(const int, const unsigned char)>) const override;
        void zip(Key96 const &, std::function<void(const unsigned char, const unsigned char)>) const;
        virtual void transform(std::function<unsigned char(const int, const unsigned char)>) override;
    private:
        struct pimpl;
        std::unique_ptr<pimpl> _pimpl;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

// #define CKey96 lxr::Key96

extern "C" {
export struct CKey96 {
   lxr::Key96 * ptr;
};

export CKey96* mk_Key96();

export void release_Key96(CKey96*);

export int len_Key96(CKey96*);

export bool bytes_Key96(CKey96*, unsigned char buffer[], int buflen);

export bool tohex_Key96(CKey96*, unsigned char buffer[], int buflen);

export CKey96* fromhex_Key96(const char *hex);
}