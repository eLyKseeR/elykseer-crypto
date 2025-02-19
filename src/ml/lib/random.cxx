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
uint32_t random_one();
uint32_t random_upto(uint32_t max);
}
/*
 *   cpp_r32_rng : () -> int
 */
extern "C" {
value cpp_r32_rng(value unit)
{
    CAMLparam1(unit);
    CAMLreturn(Val_long(random_one()));
}
} // extern C

/*
 *   cpp_r32_below_rng : int -> int
 */
extern "C" {
value cpp_r32_below_rng(value vup)
{
    CAMLparam1(vup);
    CAMLreturn(Val_long(random_upto(Long_val(vup))));
}
} // extern C
