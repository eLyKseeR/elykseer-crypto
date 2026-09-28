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
struct CKey96 {
   void * ptr;
};
CKey96* mk_Key96();
void release_Key96(CKey96*);
int len_Key96(CKey96*);
bool bytes_Key96(CKey96*, unsigned char buffer[], int buflen);
bool tohex_Key96(CKey96*, unsigned char buffer[], int buflen);
CKey96* fromhex_Key96(const char*);
}

#define theClass CKey96

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define CPP_PTR(v) (*((theClass**) Data_custom_val(v)))

static void del_ptr (value v) {
    CAMLparam1(v);
    theClass *s = CPP_PTR(v);
    if (s) {
        // printf("delete ptr %llx\n", s);
        release_Key96(s);
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
 *   cpp_mk_key96 : unit -> ptr
 */
extern "C" {
value cpp_mk_key96(value unit)
{
    CAMLparam1(unit);
    CAMLlocal1(res);
    auto p = mk_Key96();
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 1000);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_from_hex_key96 : string -> ptr
 */
extern "C" {
value cpp_from_hex_key96(value vs)
{
    CAMLparam1(vs);
    CAMLlocal1(res);
    auto p = fromhex_Key96(String_val(vs));
    if (!p) { caml_failwith("fromhex_Key96: invalid hex length"); }
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 1000);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_len_key96 : ptr -> int
 */
extern "C" {
value cpp_len_key96(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey96 *k = CPP_PTR(vk);
    CAMLreturn(Val_long(len_Key96(k)));
}
} // extern C

/*
 *   cpp_to_hex_key96 : ptr -> string
 */
extern "C" {
value cpp_to_hex_key96(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey96 *k = CPP_PTR(vk);
    const int len = len_Key96(k) * 2 / 8;
    value hex = caml_alloc_string(len);
    if (tohex_Key96(k, (unsigned char *)String_val(hex), len)) {
        CAMLreturn(hex);
    } else {
        caml_failwith("failure in C code: tohex_Key96");
    }
}
} // extern C

/*
 *   cpp_to_bytes_key96 : ptr -> string
 */
extern "C" {
value cpp_to_bytes_key96(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey96 *k = CPP_PTR(vk);
    const int len = len_Key96(k) / 8;
    value bytes = caml_alloc_string(len);
    if (bytes_Key96(k, (unsigned char *)String_val(bytes), len)) {
        CAMLreturn(bytes);
    } else {
        caml_failwith("failure in C code: bytes_Key96");
    }
}
} // extern C

