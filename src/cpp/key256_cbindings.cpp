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


module lxr_key256;


extern "C" EXPORT
CKey256* mk_Key256()
{ auto k = new lxr::Key256;
  CKey256 * r = new CKey256;
  r->ptr = k;
  return r;
}

extern "C" EXPORT
void release_Key256(CKey256 * k)
{ if (k) {
    if (k->ptr) {
        delete (lxr::Key256*)k->ptr;
    }
    delete k;
  }
}

extern "C" EXPORT
int len_Key256(CKey256 * k)
{ return ((lxr::Key256*)k->ptr)->length(); }

extern "C" EXPORT
char* bytes_Key256(CKey256 * k)
{ const unsigned char *b = ((lxr::Key256*)k->ptr)->bytes();
  return (char*)b; }

extern "C" EXPORT
char* tohex_Key256(CKey256 * k)
{ auto h = ((lxr::Key256*)k->ptr)->toHex();
  return strdup(h.c_str()); }

extern "C" EXPORT
CKey256* fromhex_Key256(const char * s)
{ auto k = new lxr::Key256(true);
  std::string h = std::string(s, k->length()*2/8);
  k->fromHex(h);
  CKey256 * r = new CKey256;
  r->ptr = k;
  return r;
}