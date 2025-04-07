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
struct CKey160;
int len_Key160(CKey160*);
bool tohex_Key160(CKey160*, unsigned char buffer[], int buflen);
class CKey256;
int len_Key256(CKey256*);
bool tohex_Key256(CKey256*, unsigned char buffer[], int buflen);
// import lxr_hmac;
CKey256* hmac_Sha256(int klen, const char k[], int mlen, const char *m);
CKey128* hmac_Md5(int klen, const char k[], int mlen, const char *m);
CKey160* hmac_Sha1(int klen, const char k[], int mlen, const char *m);
}

// using namespace lxr;

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   cpp_string_hmac_sha256 : string -> string -> string
 */
extern "C" {
value cpp_string_hmac_sha256(value vk, value vm)
{
    CAMLparam2(vk,vm);
    const char *key = String_val(vk);
    const int klen = caml_string_length(vk);
    const char *m = String_val(vm);
    const int mlen = caml_string_length(vm);
    auto k = hmac_Sha256(klen, key, mlen, m);
    const int len = len_Key256(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key256(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key256");
    }
}
} // extern C

/*
 *   cpp_buffer_hmac_sha256 : string -> buffer -> string
 */
extern "C" {
value cpp_buffer_hmac_sha256(value vk, value vb)
{
    CAMLparam2(vk,vb);
    const char *key = String_val(vk);
    const int klen = caml_string_length(vk);
    struct _cpp_cstdio_buffer *b = CPP_CSTDIO_BUFFER(vb);
    auto k = hmac_Sha256(klen, key, b->_len, b->_buf);
    const int len = len_Key256(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key256(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key256");
    }
}
} // extern C

/*
 *   cpp_buffer_hmac_md5 : string -> buffer -> string
 */
extern "C" {
value cpp_string_hmac_md5(value vk, value vm)
{
    CAMLparam2(vk,vm);
    const char *key = String_val(vk);
    const int klen = caml_string_length(vk);
    const char *m = String_val(vm);
    const int mlen = caml_string_length(vm);
    auto k = hmac_Md5(klen, key, mlen, m);
    const int len = len_Key128(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key128(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key128");
    }
}
} // extern C

/*
 *   cpp_buffer_hmac_md5 : string -> buffer -> string
 */
extern "C" {
value cpp_buffer_hmac_md5(value vk, value vb)
{
    CAMLparam2(vk,vb);
    const char *key = String_val(vk);
    const int klen = caml_string_length(vk);
    struct _cpp_cstdio_buffer *b = CPP_CSTDIO_BUFFER(vb);
    auto k = hmac_Md5(klen, key, b->_len, b->_buf);
    const int len = len_Key128(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key128(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key128");
    }
}
} // extern C


/*
 *   cpp_string_hmac_sha1 : string -> buffer -> string
 */
extern "C" {
value cpp_string_hmac_sha1(value vk, value vm)
{
    CAMLparam2(vk,vm);
    const char *key = String_val(vk);
    const int klen = caml_string_length(vk);
    const char *m = String_val(vm);
    const int mlen = caml_string_length(vm);
    auto k = hmac_Sha1(klen, key, mlen, m);
    const int len = len_Key160(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key160(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key160");
    }
}
} // extern C

/*
 *   cpp_buffer_hmac_sha1 : string -> buffer -> string
 */
extern "C" {
value cpp_buffer_hmac_sha1(value vk, value vb)
{
    CAMLparam2(vk,vb);
    const char *key = String_val(vk);
    const int klen = caml_string_length(vk);
    struct _cpp_cstdio_buffer *b = CPP_CSTDIO_BUFFER(vb);
    auto k = hmac_Sha1(klen, key, b->_len, b->_buf);
    const int len = len_Key160(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key160(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key160");
    }
}
} // extern C
