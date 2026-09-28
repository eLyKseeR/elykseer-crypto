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
#include <string>
#include <cstring>

#include "lxr-cbindings.hpp"


module lxr_key96;


extern "C" EXPORT
CKey96* mk_Key96()
{ auto k = new lxr::Key96;
  CKey96 * r = new CKey96;
  r->ptr = k;
  return r;
}

extern "C" EXPORT
void release_Key96(CKey96 * k)
{ if (k) {
    if (k->ptr) {
        delete k->ptr;
    }
    delete k;
  }
}

extern "C" EXPORT
int len_Key96(CKey96 * k)
{ return k->ptr->length(); }

extern "C" EXPORT
bool bytes_Key96(CKey96 * k, unsigned char buffer[], int buflen)
{ const int len = k->ptr->length() / 8;
  if (buflen < len) { return false; }
  memcpy(buffer, k->ptr->bytes(), len);
  return true;
}

extern "C" EXPORT
bool tohex_Key96(CKey96 * k, unsigned char buffer[], int buflen)
{ const int len = k->ptr->length() * 2 / 8;
  if (buflen < len) { return false; }
  const auto shex = k->ptr->toHex();
  memcpy(buffer, shex.c_str(), len);
  return true;
}

extern "C" EXPORT
CKey96* fromhex_Key96(const char * hex)
{ if (!hex || strlen(hex) != 96 * 2 / 8) { return nullptr; }
  auto k = new lxr::Key96(true);
  k->fromHex(std::string(hex));
  CKey96 * r = new CKey96;
  r->ptr = k;
  return r;
}