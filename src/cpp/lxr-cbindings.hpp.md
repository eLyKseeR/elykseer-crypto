```cpp

/*
<fpaste ../../../src/copyright.md>
*/

#pragma once

#if defined(_MSC_VER)

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#elif defined(__GNUC__) || defined(__CLANG__)

// try: protected | public (=default) | private
#define EXPORT __attribute__((visibility("default")))
#define IMPORT __attribute__((visibility("default")))

# define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else

#define EXPORT
#define IMPORT
#error undefined import/export attributes

#endif

```
