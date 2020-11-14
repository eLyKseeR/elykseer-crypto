declared in [Key256](key256.hpp.md)

```cpp

unsigned char const* Key256::bytes() const
{
    if (! _pimpl) { throw("uninitialised"); }
    return _pimpl->_buffer.ptr();
}

void Key256::map(std::function<void(const int, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.map(f);
}

void Key256::zip(Key256 const &other, std::function<void(const unsigned char, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.zip(f, other._pimpl->_buffer);
}

void Key256::transform(std::function<unsigned char(const int, const unsigned char)> f)
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.transform(f);
}

```

```cpp
void Key256::fromHex(std::string const &h)
{
    _pimpl.reset(new Key256::pimpl);
    Key::fromHex(h);
}

void Key256::fromBytes(unsigned char const *h)
{
    _pimpl.reset(new Key256::pimpl);
    Key::fromBytes(h);
}

```

```cpp
bool Key256::operator==(Key256 const & other) const
{
    if (! _pimpl) { throw("uninitialised"); }
    if (length() != other.length()) { return false; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 == c2);
    });
    return res;
}

bool Key256::operator!=(Key256 const & other) const
{
    if (! _pimpl) { throw("uninitialised"); }
    if (length() != other.length()) { return true; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 != c2);
    });
    return res;
}

```
