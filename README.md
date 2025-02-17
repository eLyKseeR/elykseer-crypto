# elykseer-crypto
base library that provides cryptographic functions to _elykseer_ implementations

[![Compilation and verification by unit tests](https://github.com/eLyKseeR/elykseer-crypto/actions/workflows/CI.yml/badge.svg)](https://github.com/eLyKseeR/elykseer-crypto/actions/workflows/CI.yml)

## compilation

* clone or update submodules

> `git submodule update --init

* add PGP key (for testing)

> gpg --recv-keys E2DFCA9AA83BA568AF39280999283F5327C5D38F

## language bindings

[`C++`](src/cpp)  the library is written in C++
[`C#`/`F#`](src/csharp)   _csharp_ / _.NET_ language bindings
[`OCaml`](src/ml) _OCaml_ language bindings
[`Haskell`](src/haskell) _Haskell_ language bindings

| algo | C++ |  C  | C# | OCaml | Haskell |  
| ---- | ---- | ---- | ---- | ---- | ---- |  
| AES | [√](src/cpp/aes.hpp.md) | [√](src/cpp/aes_cbindings.cpp.md) | [?](src/csharp/Aes.cs.md) | [√](src/ml/lib/aes256.ml) | [?](src/haskell/Aes.hs.md) |
| MD5 | [√](src/cpp/md5.hpp.md) | [√](src/cpp/md5_cbindings.cpp.md) | [?](src/csharp/Md5.cs.md) | [√](src/ml/lib/md5.ml) | [?](src/haskell/Md5.hs.md) |
| SHA256 | [√](src/cpp/sha256.hpp.md) | [√](src/cpp/sha256_cbindings.cpp.md) | [?](src/csharp/Sha256.cs.md) | [√](src/ml/lib/sha256.ml) | [?](src/haskell/Sha256.hs.md) |
| SHA3-256 | [√](src/cpp/sha3.hpp.md) | [√](src/cpp/sha3_cbindings.cpp.md) | [?](src/csharp/Sha3.cs.md) | [√](src/ml/lib/sha3_256.ml) | [?](src/haskell/Sha3.hs.md) |
| KEY128 | [√](src/cpp/key128.hpp.md) | [√](src/cpp/key128_cbindings.cpp.md) | [?](src/csharp/Key128.cs.md) | [√](src/ml/lib/key128.ml) | [?](src/haskell/Key128.hs.md) |
| KEY160 | [√](src/cpp/key160.hpp.md) | [√](src/cpp/key160_cbindings.cpp.md) | [?](src/csharp/Key160.cs.md) | [√](src/ml/lib/key160.ml) | [?](src/haskell/Key160.hs.md) |
| KEY256 | [√](src/cpp/key256.hpp.md) | [√](src/cpp/key256_cbindings.cpp.md) | [?](src/csharp/Key256.cs.md) | [√](src/ml/lib/key256.ml) | [?](src/haskell/Key256.hs.md) |
| Random | [√](src/cpp/random.hpp.md) | [?](src/cpp/random_cbindings.cpp.md) | [?](src/csharp/Random.cs.md) | [√](src/ml/lib/random.ml) | [?](src/haskell/Random.hs.md) |
| RandomList | [√](src/cpp/randlist.hpp.md) | [?](src/cpp/randlist_cbindings.cpp.md) | [?](src/csharp/RandList.cs.md) | [?](src/ml/lib/randlist.ml) | [?](src/haskell/RandList.hs.md) |
| HMAC | [√](src/cpp/hmac.hpp.md) | [√](src/cpp/hmac_cbindings.cpp.md) | [?](src/csharp/HMAC.cs.md) | [√](src/ml/lib/hmac.ml) | [?](src/haskell/HMAC.hs.md) |
| Base64 | [√](src/cpp/base64.hpp.md) | [√](src/cpp/base64_cbindings.cpp.md) | [?](src/csharp/Base64.cs.md) | [√](src/ml/lib/base64.ml) | [?](src/haskell/Base64.hs.md) |


# cross compilation

```sh
cd build
cmake --fresh -DCMAKE_TOOLCHAIN_FILE=../support/Toolchain_Darwin_to_Windows.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
```

./test/cpp/utelykseer-crypto --show_progress
./test/cpp/utelykseer-crypto --log_level=unit_scope

# copyright

Copyright 2019-2025 by Alexander Diemand

# license

This work is licensed under the 
[GNU General Public License v3](https://www.gnu.org/licenses/gpl.html)

