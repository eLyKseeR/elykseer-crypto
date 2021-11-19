# elykseer-crypto
base library that provides cryptographic functions to _elykseer_ implementations

[![Compilation and verification by unit tests](https://github.com/eLyKseeR/elykseer-crypto/actions/workflows/CI.yml/badge.svg)](https://github.com/eLyKseeR/elykseer-crypto/actions/workflows/CI.yml)

## compilation

* initialise submodules

> `git submodule init`

* clone or update submodules

> `git submodule update --remote`


## language bindings

[`C++`](src/cpp)  the library is written in C++  
[`C#`/`F#`](src/csharp)   _csharp_ / _.NET_ language bindings  
[`Haskell`](src/haskell) _Haskell_ language bindings  

| algo | C++ |  C  | C# | OCaml | Haskell |  
| ---- | ---- | ---- | ---- | ---- | ---- |  
| AES | [√](src/cpp/aes.hpp.md) | [√](src/cpp/aes_cbindings.cpp.md) | [?](src/csharp/Aes.cs.md) | [√](src/ml/aes.i) | [?](src/haskell/Aes.hs.md) |  
| MD5 | [√](src/cpp/md5.hpp.md) | [√](src/cpp/md5_cbindings.cpp.md) | [?](src/csharp/Md5.cs.md) | [√](src/ml/md5.i) | [?](src/haskell/Md5.hs.md) |  
| SHA256 | [√](src/cpp/sha256.hpp.md) | [√](src/cpp/sha256_cbindings.cpp.md) | [?](src/csharp/Sha256.cs.md) | [√](src/ml/sha256.i) | [?](src/haskell/Sha256.hs.md) |  
| KEY | [√](src/cpp/key.hpp.md) | [?](src/cpp/key_cbindings.cpp.md) | [?](src/csharp/Key.cs.md) | [?] | [?](src/haskell/Key.hs.md) |  
| KEY128 | [√](src/cpp/key128.hpp.md) | [√](src/cpp/key128_cbindings.cpp.md) | [?](src/csharp/Key128.cs.md) | [√](src/ml/key128.i) | [?](src/haskell/Key128.hs.md) |  
| KEY256 | [√](src/cpp/key256.hpp.md) | [√](src/cpp/key256_cbindings.cpp.md) | [?](src/csharp/Key256.cs.md) | [√](src/ml/key256.i) | [?](src/haskell/Key256.hs.md) |  
| Random | [√](src/cpp/random.hpp.md) | [?](src/cpp/random_cbindings.cpp.md) | [?](src/csharp/Random.cs.md) | [?] | [?](src/haskell/Random.hs.md) |  
| RandomList | [√](src/cpp/randlist.hpp.md) | [?](src/cpp/randlist_cbindings.cpp.md) | [?](src/csharp/RandList.cs.md) | [?] | [?](src/haskell/RandList.hs.md) |  


# copyright

Copyright 2019-2021 by Alexander Diemand

# license

This work is licensed under the 
[GNU General Public License v3](https://www.gnu.org/licenses/gpl.html)

