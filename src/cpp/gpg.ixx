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
#include <iostream>
#include <optional>
#include <string>


export module lxr_gpg;


export namespace lxr {

class Gpg
{
    public:
        Gpg();
        virtual ~Gpg();
        // can encrypt for address?
        bool has_public_key(std::string const &) const;
        // can decrypt for address?
        bool has_private_key(std::string const &) const;
        bool set_sender(std::string const &);
        std::optional<std::string> encrypt_to_key(std::string const & fpr);
        std::optional<std::string> encrypt_to_key(std::string const & fpr, std::string const & msg);
        std::ostream & ostream();
        std::istream & istream();
        bool decrypt_from_file(std::string const & fpath);
        std::optional<std::string> decrypt_from_buffer(std::string const & buffer);
    private:
        Gpg(Gpg const &) = delete;
        Gpg & operator=(Gpg const &) = delete;
        struct pimpl;
        std::unique_ptr<pimpl> _pimpl;
};

} // namespace