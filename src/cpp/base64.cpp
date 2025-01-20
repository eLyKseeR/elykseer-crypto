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

#include <string>
#include <ctime>
#include <cassert>

#if CRYPTOLIB == OPENSSL
#error to-be-done
#endif

#if CRYPTOLIB == CRYPTOPP
#include "cryptopp/base64.h"
#endif


module lxr_base64;


# if CRYPTOLIB == CRYPTOPP

namespace lxr {

std::string Base64::encode(std::string const & m)
{
    std::string out;
    CryptoPP::Base64Encoder filter;
    CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(CryptoPP::Name::InsertLineBreaks(), false);
    filter.IsolatedInitialize(params);
    filter.Detach(new CryptoPP::StringSink(out));
    filter.Put((const unsigned char *)m.c_str(), m.size());
    filter.MessageEnd();
    return std::move(out);
}

std::string Base64::decode(std::string const & m)
{
    std::string out;
    CryptoPP::Base64Decoder filter;
    filter.Detach(new CryptoPP::StringSink(out));
    filter.Put((const unsigned char *)m.c_str(), m.size());
    filter.MessageEnd();
    return std::move(out);
}

} // namespace

# endif