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

extern "C" {
class CKey256;
std::string tohex_Key256(CKey256 *k);

CKey256* hash_Sha256(int len, const char *s);
CKey256* filehash_Sha256(const char * fp);
}

// using namespace lxr;

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   cpp_string_sha256 : string -> string
 */
extern "C" {
value cpp_string_sha256(value vs)
{
    CAMLparam1(vs);
    const char *s = String_val(vs);
    const int len = caml_string_length(vs);
    auto k = hash_Sha256(len, s);
    CAMLreturn(caml_copy_string(tohex_Key256(k).c_str()));
}
} // extern C

/*
 *   cpp_file_sha256 : string -> string
 */
extern "C" {
value cpp_file_sha256(value vfp)
{
    CAMLparam1(vfp);
    const char *fp = String_val(vfp);
    auto k = filehash_Sha256(std::filesystem::path(fp).c_str());
    CAMLreturn(caml_copy_string(tohex_Key256(k).c_str()));
}
} // extern C

/*
 *   cpp_buffer_sha256 : buffer -> string
 */
extern "C" {
value cpp_buffer_sha256(value vb)
{
    CAMLparam1(vb);
    struct _cpp_cstdio_buffer *b = CPP_CSTDIO_BUFFER(vb);
    auto k = hash_Sha256(b->_len, b->_buf);
    CAMLreturn(caml_copy_string(tohex_Key256(k).c_str()));
}
} // extern C
