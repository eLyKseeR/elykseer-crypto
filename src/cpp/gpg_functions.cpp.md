declared in [Random](random.hpp.md)

```cpp

bool Gpg::exists_address(std::string const & addr) const
{
    return _pimpl->has_public_key(addr);
}

std::optional<std::string> Gpg::encrypt_to_key(std::string const & addr)
{
    return _pimpl->encrypt_to_key(addr);
}

bool Gpg::good()
{
    return _pimpl->good();
}

std::ostream& Gpg::ostream()
{
   return _pimpl->ostream();
}
```