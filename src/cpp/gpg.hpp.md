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

// can encrypt for address?

>bool [has_public_key](gpg_functions.cpp.md)(std::string const &) const;

// can decrypt for address?

>bool [has_private_key](gpg_functions.cpp.md)(std::string const &) const;

>std::optional&lt;std::string&gt; [encrypt_to_key](gpg_functions.cpp.md)(std::string const & fpr);

>std::optional&lt;std::string&gt; [encrypt_to_key](gpg_functions.cpp.md)(std::string const & fpr, std::string const & msg);

>std::ostream&amp; [ostream](gpg_functions.cpp.md)();

>std::istream&amp; [istream](gpg_functions.cpp.md)();

>bool [decrypt_from_file](gpg_functions.cpp.md)(std::string const & fpath);

>std::optional&lt;std::string&gt; [decrypt_from_buffer](gpg_functions.cpp.md)(std::string const & buffer);

>private:

>Gpg(Gpg const &) = delete;

>Gpg & operator=(Gpg const &) = delete;

>struct pimpl;

>std::unique_ptr&lt;pimpl&gt; _pimpl;

};

```cpp
} // namespace
```
