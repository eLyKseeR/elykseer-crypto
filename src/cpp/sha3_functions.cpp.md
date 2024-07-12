declared in [Sha3](sha3.hpp.md)

```cpp

Sha3_256::Sha3_256()
    : _pimpl(new Sha3_256::pimpl)
{
}

Sha3_256::~Sha3_256()
{
    if (_pimpl) {
        _pimpl.reset();
    }
}

Key256 Sha3_256::hash(std::string const & msg)
{
    return Sha3_256::hash(msg.c_str(), msg.size());
}
```
