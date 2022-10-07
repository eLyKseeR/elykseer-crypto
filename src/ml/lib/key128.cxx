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

#define theClass Key128

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "lxr/key128.hpp"

using namespace lxr;

#define CPP_PTR(v) (*((theClass**) Data_custom_val(v)))

static void del_ptr (value v) {
    CAMLparam1(v);
    theClass *s = CPP_PTR(v);
    if (s) {
        // printf("delete ptr %llx\n", s);
        delete s;
    }
    CAMLreturn0;
}

static struct custom_operations cpp_ptr_ops = {
    (char *)"mlcpp_" TOSTRING(theClass),
    del_ptr,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default
};



/*
 *   cpp_mk_key128 : unit -> ptr
 */
extern "C" {
value cpp_mk_key128(value unit)
{
    CAMLparam1(unit);
    CAMLlocal1(res);
    auto p = new Key128();
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 1000);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_mk_key128 : unit -> ptr
 */
extern "C" {
value cpp_from_hex_key128(value vs)
{
    CAMLparam1(vs);
    CAMLlocal1(res);
    auto p = new Key128(true);
    p->fromHex(std::string(String_val(vs)));
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 1000);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_len_key128 : ptr -> int
 */
extern "C" {
value cpp_len_key128(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    Key128 *k = CPP_PTR(vk);
    CAMLreturn(Val_long(k->length()));
}
} // extern C

/*
 *   cpp_to_hex_key128 : ptr -> string
 */
extern "C" {
value cpp_to_hex_key128(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    Key128 *k = CPP_PTR(vk);
    CAMLreturn(caml_copy_string(k->toHex().c_str()));
}
} // extern C

/*
 *   cpp_to_bytes_key128 : ptr -> string
 */
extern "C" {
value cpp_to_bytes_key128(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    Key128 *k = CPP_PTR(vk);
    CAMLreturn(caml_copy_string((const char *)k->bytes()));
}
} // extern C

