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
#include <algorithm>

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

// C binding structs for key objects (void* ptr avoids needing module types)
extern "C" {
struct CKey128 { void *ptr; };
struct CKey256 { void *ptr; };

extern long cpp_process_aes256(int direction, CKey128 *iv, CKey256 *k, void *buf, int blen, int dlen);
}

// static constexpr unsigned int datasz { 1024*4 };

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   returns pair: (dlen, buffer)
 */
static value _cpp_process_aes256(value viv, value vk, value vn, value varr, value vdlen, const int crypto)
{
    CAMLparam5(viv, vk, vn, varr, vdlen);
    CAMLlocal1(pair);
    CKey128 *iv = *((CKey128**) Data_custom_val(viv));
    CKey256 *k  = *((CKey256**) Data_custom_val(vk));
    long blen = Long_val(vn);
    long dlen = Long_val(vdlen);
    struct _cpp_cstdio_buffer *arr = CPP_CSTDIO_BUFFER(varr);
    
    long nread = cpp_process_aes256(crypto, iv, k, arr->_buf, blen, dlen);
    
    pair = caml_alloc_tuple(2);
    Store_field(pair, 0, Val_long(nread));
    Store_field(pair, 1, varr);
    CAMLreturn(pair);
}

extern "C" {

value cpp_encrypt_aes256(value viv, value vk, value vn, value varr, value dlen)
{
    return _cpp_process_aes256(viv, vk, vn, varr, dlen, 1);
}
value cpp_decrypt_aes256(value viv, value vk, value vn, value varr)
{
    return _cpp_process_aes256(viv, vk, vn, varr, vn, -1);
}

} // extern C
