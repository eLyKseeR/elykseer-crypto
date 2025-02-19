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
struct CKey256 {
    void * ptr;
};
CKey256* mk_Key256();
void release_Key256(CKey256*);
int len_Key256(CKey256*);
char* bytes_Key256(CKey256*);
std::string tohex_Key256(CKey256*);
CKey256* fromhex_Key256(const char*);
}

#define theClass CKey256

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define CPP_PTR(v) (*((theClass**) Data_custom_val(v)))

static void del_ptr (value v) {
    CAMLparam1(v);
    theClass *s = CPP_PTR(v);
    if (s) {
        // printf("delete ptr %llx\n", s);
        release_Key256(s);
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
 *   cpp_mk_key256 : unit -> ptr
 */
extern "C" {
value cpp_mk_key256(value unit)
{
    CAMLparam1(unit);
    CAMLlocal1(res);
    auto p = mk_Key256();
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 1000);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_mk_key256 : unit -> ptr
 */
extern "C" {
value cpp_from_hex_key256(value vs)
{
    CAMLparam1(vs);
    CAMLlocal1(res);
    auto p = fromhex_Key256(String_val(vs));
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 1000);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_len_key256 : ptr -> int
 */
extern "C" {
value cpp_len_key256(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey256 *k = CPP_PTR(vk);
    CAMLreturn(Val_long(len_Key256(k)));
}
} // extern C

/*
 *   cpp_to_hex_key256 : ptr -> string
 */
extern "C" {
value cpp_to_hex_key256(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey256 *k = CPP_PTR(vk);
    CAMLreturn(caml_copy_string(tohex_Key256(k).c_str()));
}
} // extern C

/*
 *   cpp_to_bytes_key256 : ptr -> string
 */
extern "C" {
value cpp_to_bytes_key256(value vk)
{
    CAMLparam1(vk);
    CAMLlocal1(res);
    CKey256 *k = CPP_PTR(vk);
    CAMLreturn(caml_copy_string((const char *)bytes_Key256(k)));
}
} // extern C
