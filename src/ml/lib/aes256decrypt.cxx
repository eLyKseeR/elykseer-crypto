// OCaml includes
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/custom.h>
// #include <caml/callback.h>
#include <caml/fail.h>

#include <sys/errno.h>
} //extern C

// C++ includes
#include <string>

#define theClass AesDecrypt

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "lxr/aes.hpp"
// #include "lxr/key256.hpp"
// #include "lxr/key128.hpp"
#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

using namespace lxr;

#define CPP_PTR(v) (*((theClass**) Data_custom_val(v)))
// #define CPP_KEY128_PTR(v) (*((Key128**) Data_custom_val(v)))
// #define CPP_KEY256_PTR(v) (*((Key256**) Data_custom_val(v)))

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
 *   cpp_mk_aes256decrypt : key256 -> key128 -> ptr
 */
extern "C" {
value cpp_mk_aes256decrypt(value vk, value viv)
{
    CAMLparam2(vk, viv);
    CAMLlocal1(res);
    Key256 k(true); k.fromHex(std::string(String_val(vk)));
    Key128 iv(true); iv.fromHex(std::string(String_val(viv)));
    auto p = new AesDecrypt(k, iv);
    res = caml_alloc_custom(&cpp_ptr_ops,
                            sizeof(theClass*), 1, 100);
    CPP_PTR(res) = p;
    CAMLreturn(res);
}
} // extern C

/*
 *   cpp_process_aes256decrypt  : ptr -> int -> b -> int
 */
extern "C" {
value cpp_process_aes256decrypt(value vaes, value vlen, value varr)
{
    CAMLparam3(vaes, vlen, varr);
    CAMLlocal1(res);
    long len = Long_val(vlen);
    const char *src = (const char *)Caml_ba_data_val(varr);
    long sz = caml_ba_byte_size(Caml_ba_array_val(varr));
    if (sz < len || sz > Aes::datasz) { return Val_long(-1); }
    AesDecrypt *p = CPP_PTR(vaes);
    sizebounded<unsigned char, Aes::datasz> buf;
    long n = Aes::datasz;
    n = std::min(len,n);
    std::memcpy((void*)buf.ptr(), src, n);
    long cnt = p->process(n, buf);
    if (cnt > 0) {
        std::memcpy((void*)src, buf.ptr(), cnt);
    }
    CAMLreturn(Val_long(cnt));
}
} // extern C

/*
 *   cpp_finish_aes256decrypt  : ptr -> int -> b -> int
 */
extern "C" {
value cpp_finish_aes256decrypt(value vaes, value vlen, value varr)
{
    CAMLparam3(vaes, vlen, varr);
    CAMLlocal1(res);
    long len = Long_val(vlen);
    const char *src = (const char *)Caml_ba_data_val(varr);
    long sz = caml_ba_byte_size(Caml_ba_array_val(varr));
    if (sz < len || sz > Aes::datasz) { return Val_long(-1); }
    AesDecrypt *p = CPP_PTR(vaes);
    sizebounded<unsigned char, Aes::datasz> buf;
    long n = Aes::datasz;
    n = std::min(len,n);
    std::memcpy((void*)buf.ptr(), src, n);
    long cnt = p->finish(n, buf);
    if (cnt > 0) {
        std::memcpy((void*)src, buf.ptr(), cnt);
    }
    CAMLreturn(Val_long(cnt));
}
} // extern C
