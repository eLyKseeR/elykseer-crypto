declared in [Key256](key256.hpp.md)

```cpp

unsigned char const* Key256::bytes() const
{
    return _pimpl->_buffer.ptr();
}

void Key256::map(std::function<void(const int, const unsigned char)> f) const
{
    _pimpl->_buffer.map(f);
}

void Key256::zip(Key256 const &other, std::function<void(const unsigned char, const unsigned char)> f) const
{
    _pimpl->_buffer.zip(f, other._pimpl->_buffer);
}

void Key256::transform(std::function<unsigned char(const int, const unsigned char)> f)
{
    _pimpl->_buffer.transform(f);
}

```

```cpp
bool Key256::operator==(Key256 const & other) const
{
    if (length() != other.length()) { return false; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 == c2);
    });
    return res;
}

bool Key256::operator!=(Key256 const & other) const
{
    if (length() != other.length()) { return true; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 != c2);
    });
    return res;
}

```
