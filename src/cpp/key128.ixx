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

#include <functional>
#include <memory>
#include <string>

import lxr_key;


export module lxr_key128;


export namespace lxr {

class Key128 : public Key
{
    public:
        Key128(bool noinit = false);
        virtual ~Key128();
        Key128(Key128 const &);
        Key128 & operator=(Key128 const &);
        static Key128 key128FromHex(std::string const &);
        virtual unsigned char const* bytes() const override;
        virtual int length() const override { return 128; };
        virtual bool operator==(Key128 const &) const final;
        virtual bool operator!=(Key128 const &) const final;
        virtual void fromHex(std::string const &) override;
        virtual void fromBytes(unsigned char const*) override;
    protected:
        virtual void map(std::function<void(const int, const unsigned char)>) const override;
        void zip(Key128 const &, std::function<void(const unsigned char, const unsigned char)>) const;
        virtual void transform(std::function<unsigned char(const int, const unsigned char)>) override;
    private:
        struct pimpl;
        std::unique_ptr<pimpl> _pimpl;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

// #define CKey128 lxr::Key128

extern "C" {
export struct CKey128 {
   void * ptr;
};

export CKey128* mk_Key128();

export void release_Key128(CKey128*);

export int len_Key128(CKey128*);

export char* bytes_Key128(CKey128*);

export char* tohex_Key128(CKey128*);

export CKey128* fromhex_Key128(const char*);
}