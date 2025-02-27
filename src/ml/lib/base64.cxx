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

extern "C" {
int base64_encode(int mlen, const char * m, int outlen, unsigned char * out);
int base64_decode(int mlen, const char * m, int outlen, unsigned char * out);
}

/*
 *   cpp_encode_base64 : string -> string
 */
extern "C" {
value cpp_encode_base64(value vm)
{
    CAMLparam1(vm);
    const char *m = String_val(vm);
    const int mlen = caml_string_length(vm);
    const int outlen = 2048;
    unsigned char buffer[outlen+1];
    int copied = base64_encode(mlen, m, outlen, buffer);
    buffer[copied] = '\0';
    CAMLreturn(caml_copy_string((const char *)buffer));
}
} // extern C

/*
 *   cpp_decode_base64 : string -> string
 */
extern "C" {
value cpp_decode_base64(value vm)
{
    CAMLparam1(vm);
    const char *m = String_val(vm);
    const int mlen = caml_string_length(vm);
    const int outlen = 2048;
    unsigned char buffer[outlen+1];
    int copied = base64_decode(mlen, m, outlen, buffer);
    buffer[copied] = '\0';
    CAMLreturn(caml_copy_string((const char *)buffer));
}
} // extern C

