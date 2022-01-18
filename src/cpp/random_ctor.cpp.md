declared in [Random](random.hpp.md)

```cpp

static std::unique_ptr<prngCpp::MT19937> _rng;

Random::Random()
{
    if (! _rng) {
        _rng.reset(new prngCpp::MT19937);
    }
}

```