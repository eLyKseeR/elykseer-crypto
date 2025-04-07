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
#include <string>
#include <cstring>

#include "lxr-cbindings.hpp"


module lxr_key160;


extern "C" EXPORT
CKey160* mk_Key160()
{ auto k = new lxr::Key160;
  CKey160 * r = new CKey160;
  r->ptr = k;
  return r;
}

extern "C" EXPORT
void release_Key160(CKey160 * k)
{ if (k) {
    if (k->ptr) {
        delete (lxr::Key160*)k->ptr;
    }
    delete k;
  }
}

extern "C" EXPORT
int len_Key160(CKey160 * k)
{ return ((lxr::Key160*)k->ptr)->length(); }

extern "C" EXPORT
bool bytes_Key160(CKey160 * k, unsigned char buffer[], int buflen)
{ const int len = k->ptr->length() / 8;
  if (buflen < len) { return false; }
  memcpy(buffer, k->ptr->bytes(), len);
  return true;
}

extern "C" EXPORT
bool tohex_Key160(CKey160 * k, unsigned char buffer[], int buflen)
{ const int len = k->ptr->length() * 2 / 8;
  if (buflen < len) { return false; }
  const auto shex = k->ptr->toHex();
  memcpy(buffer, shex.c_str(), len);
  return true;
}

extern "C" EXPORT
CKey160* fromhex_Key160(std::string const & s)
{ auto k = new lxr::Key160(true);
  k->fromHex(s);
  CKey160 * r = new CKey160;
  r->ptr = k;
  return r;
}