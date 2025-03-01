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
class CKey128;
int len_Key128(CKey128*);
bool tohex_Key128(CKey128*, unsigned char buffer[], int buflen);
void release_Key128(CKey128 *k);
CKey128* hash_Md5(int len, const char *);
}

// using namespace lxr;

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   cpp_string_md5 : string -> string
 */
extern "C" {
value cpp_string_md5(value vs)
{
    CAMLparam1(vs);
    const char *s = String_val(vs);
    const int len = caml_string_length(vs);
    auto k = hash_Md5(len, s);
    const int klen = len_Key128(k) * 2 / 8;
    value hex = caml_alloc_string(klen);
    if (tohex_Key128(k, (unsigned char *)String_val(hex), klen)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key128");
    }
}
} // extern C

/*
 *   cpp_buffer_md5 : buffer -> string
 */
extern "C" {
value cpp_buffer_md5(value vb)
{
    CAMLparam1(vb);
    struct _cpp_cstdio_buffer *b = CPP_CSTDIO_BUFFER(vb);
    auto k = hash_Md5(b->_len, b->_buf);
    const int klen = len_Key128(k) * 2 / 8;
    value hex = caml_alloc_string(klen);
    if (tohex_Key128(k, (unsigned char *)String_val(hex), klen)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key128");
    }
}
} // extern C
