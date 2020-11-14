declared in [Key256](key256.hpp.md)

initialize with random data

```cpp

Key256::Key256(bool noinit)
{
    if (!noinit) {
        _pimpl.reset(new Key256::pimpl);
        randomize();
    }
}

Key256::~Key256() = default;

Key256::Key256(Key256 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key256::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
}

Key256 & Key256::operator=(Key256 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key256::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
    return *this;
}

```