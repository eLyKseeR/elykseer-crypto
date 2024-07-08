// OCaml includes
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
// #include <caml/bigarray.h>
// #include <caml/custom.h>
// #include <caml/callback.h>
// #include <caml/fail.h>

#include <sys/errno.h>
} //extern C

// C++ includes
#include <string>
#include <filesystem>

#include "lxr/key128.hpp"
#include "lxr/key256.hpp"
#include "lxr/hmac.hpp"

using namespace lxr;

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
    auto k = lxr::HMAC::hmac_sha256(key, klen, m, mlen);
    CAMLreturn(caml_copy_string(k.toHex().c_str()));
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
    auto k = lxr::HMAC::hmac_sha256(key, klen, b->_buf, b->_len);
    CAMLreturn(caml_copy_string(k.toHex().c_str()));
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
    auto k = lxr::HMAC::hmac_md5(key, klen, m, mlen);
    CAMLreturn(caml_copy_string(k.toHex().c_str()));
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
    auto k = lxr::HMAC::hmac_md5(key, klen, b->_buf, b->_len);
    CAMLreturn(caml_copy_string(k.toHex().c_str()));
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
    auto k = lxr::HMAC::hmac_sha1(key, klen, m, mlen);
    std::string k40 = k.toHex().substr(0,40); // 20 bytes in hex: 40 chars
    CAMLreturn(caml_copy_string(k40.c_str()));
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
    auto k = lxr::HMAC::hmac_sha1(key, klen, b->_buf, b->_len);
    std::string k40 = k.toHex().substr(0,40); // 20 bytes in hex: 40 chars
    CAMLreturn(caml_copy_string(k40.c_str()));
}
} // extern C
