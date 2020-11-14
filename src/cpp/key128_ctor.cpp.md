declared in [Key128](key128.hpp.md)

initialize with random data

```cpp

Key128::Key128(bool noinit)
{
    if (!noinit) {
        _pimpl.reset(new Key128::pimpl);
        randomize();
    }
}

Key128::~Key128() = default;

Key128::Key128(Key128 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key128::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
}

Key128 & Key128::operator=(Key128 const & o)
{
    if (o._pimpl) {
        _pimpl.reset(new Key128::pimpl);
        _pimpl->_buffer = o._pimpl->_buffer;
    }
    return *this;
}

```