declared in [Key128](key128.hpp.md)

```cpp

unsigned char const* Key128::bytes() const
{
    return _pimpl->_buffer.ptr();
}

void Key128::map(std::function<void(const int, const unsigned char)> f) const
{
    _pimpl->_buffer.map(f);
}

void Key128::zip(Key128 const & other, std::function<void(const unsigned char, const unsigned char)> f) const
{
    _pimpl->_buffer.zip(f, other._pimpl->_buffer);
}

void Key128::transform(std::function<unsigned char(const int, const unsigned char)> f)
{
    _pimpl->_buffer.transform(f);
}

```

```cpp
bool Key128::operator==(Key128 const & other) const
{
    if (length() != other.length()) { return false; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 == c2);
    });
    return res;
}

bool Key128::operator!=(Key128 const & other) const
{
    if (length() != other.length()) { return true; }
    bool res = true;
    zip(other, [&res](const unsigned char c1, const unsigned char c2) {
        res &= (c1 != c2);
    });
    return res;
}

```
