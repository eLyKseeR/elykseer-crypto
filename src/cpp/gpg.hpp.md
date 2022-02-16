```cpp

/*
<fpaste ../../src/copyright.md>
*/

#pragma once

#include <memory>
#include <iostream>
#include <optional>

````

namespace [lxr](namespace.list) {


# class Gpg

{

>public:

>[Gpg](gpg_ctor.cpp.md)();

>virtual [~Gpg](gpg_ctor.cpp.md)();

>bool [exists_address](gpg_functions.cpp.md)(std::string const &) const;

>std::optional&lt;std::string&gt; [encrypt_to_key](gpg_functions.cpp.md)(std::string const & fpr);

>std::ostream& [ostream](gpg_functions.cpp.md)();

>bool [good](gpg_functions.cpp.md)();

>//std::istream& [decrypt_from_file](gpg_functions.cpp.md)(std::string const & fpr);

>private:

>Gpg(Gpg const &) = delete;

>Gpg & operator=(Gpg const &) = delete;

>struct pimpl;

>std::unique_ptr&lt;pimpl&gt; _pimpl;

};

```cpp
} // namespace
```
