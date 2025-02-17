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


export module lxr_key160;


export namespace lxr {

class Key160 : public Key
{
   public:
      Key160(bool noinit = false);
      virtual ~Key160();
      Key160(Key160 const &);
      Key160 & operator=(Key160 const &);
      static Key160 key160FromHex(std::string const &);
      virtual unsigned char const* bytes() const override;
      virtual int length() const override { return 160; };
      virtual bool operator==(Key160 const &) const final;
      virtual bool operator!=(Key160 const &) const final;
      virtual void fromHex(std::string const &) override;
      virtual void fromBytes(unsigned char const*) override;
   protected:
      virtual void map(std::function<void(const int, const unsigned char)>) const override;
      void zip(Key160 const &, std::function<void(const unsigned char, const unsigned char)>) const;
      virtual void transform(std::function<unsigned char(const int, const unsigned char)>) override;
   private:
      struct pimpl;
      std::unique_ptr<pimpl> _pimpl;
};

} // namespace

// C binding interface
#include "lxr-cbindings.hpp"

// #define CKey160 lxr::Key160

extern "C" {
export struct CKey160 {
   void * ptr;
};

export CKey160* mk_Key160();

export void release_Key160(CKey160*);

export int len_Key160(CKey160*);

export char* bytes_Key160(CKey160*);

export char* tohex_Key160(CKey160*);

export CKey160* fromhex_Key160(const char*);
}