// OCaml includes
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
// #include <caml/bigarray.h>
// #include <caml/custom.h>
// #include <caml/callback.h>
#include <caml/fail.h>

#include <sys/errno.h>
} //extern C

// C++ includes
#include <string>
#include <filesystem>

extern "C" {
// #include "lxr/key256.hpp"
class CKey256;
int len_Key256(CKey256*);
bool tohex_Key256(CKey256*, unsigned char buffer[], int buflen);
// #include "lxr/sha3.hpp"
CKey256* hash_Sha3_256(int len, const char *s);
CKey256* filehash_Sha3_256(const char * fp);
}

// using namespace lxr;

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   cpp_string_sha3_256 : string -> string
 */
extern "C" {
value cpp_string_sha3_256(value vs)
{
    CAMLparam1(vs);
    const char *s = String_val(vs);
    const int len = caml_string_length(vs);
    auto k = hash_Sha3_256(len, s);
    const int klen = len_Key256(k) * 2 / 8;
    value hex = caml_alloc_string(klen);
    if (tohex_Key256(k, (unsigned char *)String_val(hex), klen)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key256");
    }
}
} // extern C

/*
 *   cpp_file_sha3_256 : string -> string
 */
extern "C" {
value cpp_file_sha3_256(value vfp)
{
    CAMLparam1(vfp);
    const char *fp = String_val(vfp);
    auto k = filehash_Sha3_256(std::filesystem::path(fp).c_str());
    const int klen = len_Key256(k) * 2 / 8;
    value hex = caml_alloc_string(klen);
    if (tohex_Key256(k, (unsigned char *)String_val(hex), klen)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key256");
    }
}
} // extern C

/*
 *   cpp_buffer_sha3_256 : buffer -> string
 */
extern "C" {
value cpp_buffer_sha3_256(value vb)
{
    CAMLparam1(vb);
    struct _cpp_cstdio_buffer *b = CPP_CSTDIO_BUFFER(vb);
    auto k = hash_Sha3_256(b->_len, b->_buf);
    const int klen = len_Key256(k) * 2 / 8;
    value hex = caml_alloc_string(klen);
    if (tohex_Key256(k, (unsigned char *)String_val(hex), klen)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key256");
    }
}
} // extern C
