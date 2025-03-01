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

extern "C" {
struct CKey128 {
   void * ptr;
};
CKey128* mk_Key128();
void release_Key128(CKey128*);
int len_Key128(CKey128*);
bool bytes_Key128(CKey128*, unsigned char buffer[], int buflen);
bool tohex_Key128(CKey128*, unsigned char buffer[], int buflen);
CKey128* fromhex_Key128(const char*);
}

#define theClass CKey128

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define CPP_PTR(v) (*((theClass**) Data_custom_val(v)))

static void del_ptr (value v) {
    CAMLparam1(v);
    theClass *s = CPP_PTR(v);
    if (s) {
        // printf("delete ptr %llx\n", s);
        release_Key128(s);
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
    auto p = mk_Key128();
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
    auto p = fromhex_Key128(String_val(vs));
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
    CKey128 *k = CPP_PTR(vk);
    CAMLreturn(Val_long(len_Key128(k)));
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
    CKey128 *k = CPP_PTR(vk);
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
 *   cpp_to_bytes_key128 : ptr -> string
 */
extern "C" {
value cpp_to_bytes_key128(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey128 *k = CPP_PTR(vk);
    const int len = len_Key128(k) / 8;
    value bytes = caml_alloc_string(len);
    if (bytes_Key128(k, (unsigned char *)String_val(bytes), len)) {
        CAMLreturn(bytes);
    } else {
        caml_failwith("failure in C code: bytes_Key128");
    }
}
} // extern C

