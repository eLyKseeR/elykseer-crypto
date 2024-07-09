declared in [Key160](key160.hpp.md)

```cpp

unsigned char const* Key160::bytes() const
{
    if (! _pimpl) { throw("uninitialised"); }
    return _pimpl->_buffer.ptr();
}

void Key160::map(std::function<void(const int, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.map(f);
}

void Key160::zip(Key160 const & other, std::function<void(const unsigned char, const unsigned char)> f) const
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.zip(f, other._pimpl->_buffer);
}

void Key160::transform(std::function<unsigned char(const int, const unsigned char)> f)
{
    if (! _pimpl) { throw("uninitialised"); }
    _pimpl->_buffer.transform(f);
}

```

```cpp
void Key160::fromHex(std::string const &h)
{
    _pimpl.reset(new Key160::pimpl);
    Key::fromHex(h);
}

void Key160::fromBytes(unsigned char const *h)
{
    _pimpl.reset(new Key160::pimpl);
    Key::fromBytes(h);
}

```

```cpp
bool Key160::operator==(Key160 const & other) const
{
    if (! _pimpl) { throw("uninitialised"); }
    if (length() != other.length()) { return false; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 == c2);
    });
    return res;
}

bool Key160::operator!=(Key160 const & other) const
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
