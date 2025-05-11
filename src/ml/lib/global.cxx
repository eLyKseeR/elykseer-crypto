// OCaml includes
extern "C" {
    #include <caml/mlvalues.h>
    #include <caml/memory.h>
    #include <caml/alloc.h>
    // #include <caml/bigarray.h>
    #include <caml/custom.h>
    // #include <caml/callback.h>
    // #include <caml/fail.h>
}

extern "C" {
    bool global_initialize();
    bool global_cleanup();
}

/*
 *   cpp_initialize_global : unit -> bool
 */
extern "C" {
value cpp_initialize_global(value unit)
{
    CAMLparam1(unit);
    bool res = global_initialize();
    CAMLreturn(Val_bool(res));
}
} // extern C

/*
 *   cpp_initialize_global : unit -> bool
 */
extern "C" {
value cpp_cleanup_global(value unit)
{
    CAMLparam1(unit);
    bool res = global_cleanup();
    CAMLreturn(Val_bool(res));
}
} // extern C