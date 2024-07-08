declared in [Key160](key160.hpp.md)

initialize with random data

```cpp

Key160::Key160(bool noinit)
{
    if (!noinit) {
        _pimpl.reset(new Key160::pimpl);
        randomize();
    }
}

Key160::~Key160() = default;

Key160::Key160(Key160 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key160::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
}

Key160 & Key160::operator=(Key160 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key160::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
    return *this;
}

Key160 Key160::key160FromHex(std::string const &s)
{
    Key160 key{true};
    key.fromHex(s);
    return key;
}

```