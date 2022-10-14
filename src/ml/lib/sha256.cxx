// OCaml includes
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
// #include <caml/bigarray.h>
#include <caml/custom.h>
// #include <caml/callback.h>
#include <caml/fail.h>

#include <sys/errno.h>
} //extern C

// C++ includes
#include <string>
#include <filesystem>

#include "lxr/key256.hpp"
#include "lxr/sha256.hpp"

using namespace lxr;


/*
 *   cpp_string_sha256 : string -> string
 */
extern "C" {
value cpp_string_sha256(value vs)
{
    CAMLparam1(vs);
    const char *s = String_val(vs);
    const int len = string_length(vs);
    auto k = lxr::Sha256::hash(s, len);
    CAMLreturn(caml_copy_string(k.toHex().c_str()));
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
    auto k = lxr::Sha256::hash(std::filesystem::path(fp));
    CAMLreturn(caml_copy_string(k.toHex().c_str()));
}
} // extern C
