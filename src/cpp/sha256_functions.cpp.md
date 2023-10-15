declared in [Sha256](sha256.hpp.md)

```cpp

Sha256::Sha256()
    : _pimpl(new Sha256::pimpl)
{
}

Sha256::~Sha256()
{
    if (_pimpl) {
        _pimpl.reset();
    }
}

Key256 Sha256::hash(std::string const & msg)
{
    return Sha256::hash(msg.c_str(), msg.size());
}
```
