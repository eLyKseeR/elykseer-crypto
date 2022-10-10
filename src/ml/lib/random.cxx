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

#define theClass Random

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "lxr/random.hpp"

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


extern "C" {
value cpp_mk_rng(value unit)
{
    CAMLparam1(unit);
    CAMLlocal1(res);
    auto p = new lxr::theClass();
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 100);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_r32_rng : ptr -> int
 */
extern "C" {
value cpp_r32_rng(value vr)
{
    CAMLparam1(vr);
    theClass *r = CPP_PTR(vr);
    CAMLreturn(Val_long(r->random()));
}
} // extern C

/*
 *   cpp_r32_below_rng : ptr -> int -> int
 */
extern "C" {
value cpp_r32_below_rng(value vr, value vup)
{
    CAMLparam2(vr, vup);
    theClass *r = CPP_PTR(vr);
    CAMLreturn(Val_long(r->random(Long_val(vup))));
}
} // extern C
