// OCaml includes
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>

#include <sys/errno.h>
} //extern C

// C binding structs for key objects (void* ptr avoids needing module types)
extern "C" {
struct CKey96 { void *ptr; };
struct CKey256 { void *ptr; };

extern long cpp_process_aes256_gcm(int direction, CKey96 *nonce, CKey256 *k, const void *aad, int aadlen, void *buf, int blen, int dlen);
}

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   returns pair: (dlen, buffer)
 *   encrypt: dlen = plaintext length + 16 (ciphertext || tag)
 *   decrypt: dlen = plaintext length, or -1 if authentication failed
 */
static value _cpp_process_aes256_gcm(value vnonce, value vk, value vaad, value vn, value varr, value vdlen, const int crypto)
{
    CAMLparam5(vnonce, vk, vaad, vn, varr);
    CAMLxparam1(vdlen);
    CAMLlocal1(pair);
    CKey96 *nonce = *((CKey96**) Data_custom_val(vnonce));
    CKey256 *k    = *((CKey256**) Data_custom_val(vk));
    long blen = Long_val(vn);
    long dlen = Long_val(vdlen);
    struct _cpp_cstdio_buffer *arr = CPP_CSTDIO_BUFFER(varr);

    long nread = cpp_process_aes256_gcm(crypto, nonce, k,
                                        String_val(vaad), (int)caml_string_length(vaad),
                                        arr->_buf, (int)blen, (int)dlen);

    pair = caml_alloc_tuple(2);
    Store_field(pair, 0, Val_long(nread));
    Store_field(pair, 1, varr);
    CAMLreturn(pair);
}

extern "C" {

value cpp_encrypt_aes256_gcm(value vnonce, value vk, value vaad, value vn, value varr, value dlen)
{
    return _cpp_process_aes256_gcm(vnonce, vk, vaad, vn, varr, dlen, 1);
}
// bytecode entry point (more than 5 arguments)
value cpp_encrypt_aes256_gcm_bytecode(value *argv, int argn)
{
    return cpp_encrypt_aes256_gcm(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
}
value cpp_decrypt_aes256_gcm(value vnonce, value vk, value vaad, value vn, value varr)
{
    return _cpp_process_aes256_gcm(vnonce, vk, vaad, vn, varr, vn, -1);
}

} // extern C
