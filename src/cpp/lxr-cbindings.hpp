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

#pragma once


#if defined(_MSC_VER)

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#elif defined(__GNUC__) || defined(__CLANG__)

// try: protected | public (=default) | private
#define EXPORT __attribute__((visibility("default")))
#define IMPORT __attribute__((visibility("default")))

#define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))

#else

#define EXPORT
#define IMPORT
#error undefined import/export attributes

#endif
