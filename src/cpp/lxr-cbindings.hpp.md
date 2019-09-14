```cpp

/*
<fpaste ../../../src/copyright.md>
*/

#pragma once

#if defined(_MSC_VER)

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#elif defined(__GNUC__)

#define EXPORT __attribute__((visibility("default")))
#define IMPORT __attribute__((visibility("default")))

#else

#define EXPORT
#define IMPORT
#pragma warning undefined import/export attributes

#endif

```
