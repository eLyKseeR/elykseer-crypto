declared in [Gpg](gpg.hpp.md)

```cpp

bool Gpg::has_public_key(std::string const & addr) const
{
    return _pimpl->has_public_key(addr);
}

bool Gpg::has_private_key(std::string const & addr) const
{
    return _pimpl->has_private_key(addr);
}

std::optional<std::string> Gpg::encrypt_to_key(std::string const & addr, std::string const & msg)
{
    return _pimpl->encrypt_to_key(addr, msg);
}

std::optional<std::string> Gpg::encrypt_to_key(std::string const & addr)
{
    return _pimpl->encrypt_to_key(addr);
}

std::ostream& Gpg::ostream()
{
   return _pimpl->ostream();
}

std::istream& Gpg::istream()
{
   return _pimpl->istream();
}

bool Gpg::decrypt_from_file(std::string const &fp)
{
    return _pimpl->decrypt_from_file(fp);
}

std::optional<std::string> Gpg::decrypt_from_buffer(std::string const &b)
{
    return _pimpl->decrypt_from_buffer(b);
}

```