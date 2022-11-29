// OCaml includes
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
// #include <caml/alloc.h>
// #include <caml/bigarray.h>
// #include <caml/custom.h>
// #include <caml/callback.h>
// #include <caml/fail.h>

#include <sys/errno.h>
} //extern C

// C++ includes
#include <algorithm>
#include <string>

#include "lxr/aes.hpp"
#include "lxr/key256.hpp"
#include "lxr/key128.hpp"
#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

using namespace lxr;

#define CPP_KEY128_PTR(v) (*((Key128**) Data_custom_val(v)))
#define CPP_KEY256_PTR(v) (*((Key256**) Data_custom_val(v)))

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   cpp_encrypt_aes256 : key128 -> key256 -> buffer -> buffer
 *   cpp_decrypt_aes256 : key128 -> key256 -> buffer -> buffer
 */
extern "C" {
value cpp_process_aes256(value viv, value vk, value varr, int crypto) {
    CAMLparam3(viv, vk, varr);
    Key128 *iv = CPP_KEY128_PTR(viv);
    Key256 *k = CPP_KEY256_PTR(vk);
    Aes *p;
    if (crypto == 1) {
        p = new AesEncrypt(*k, *iv);
    } else {
        p = new AesDecrypt(*k, *iv);
    }
    struct _cpp_cstdio_buffer *arr = CPP_CSTDIO_BUFFER(varr);
    long sz = arr->_len;
    long idx = 0;
    long lst = 0;
    sizebounded<unsigned char, Aes::datasz> buf;
    long cnt = 0;
    long nread = 0;
    while (idx < sz) {
        long n = std::min(sz - idx, (long)Aes::datasz);
        std::memcpy((void*)buf.ptr(), arr->_buf + idx, n);
        cnt = p->process(n, buf);
        if (cnt > 0) {
            nread += cnt;
            std::memcpy((void*)(arr->_buf + idx), buf.ptr(), cnt);
        }
        lst = idx;
        idx = idx + n;
    }
    if (nread < sz) {
        cnt += p->finish(cnt, buf);
        if (cnt > 0) {
            std::memcpy((void*)(arr->_buf + lst), buf.ptr(), cnt);
        }
    }
    CAMLreturn(varr);
}

value cpp_encrypt_aes256(value viv, value vk, value varr)
{
    return cpp_process_aes256(viv, vk, varr, 1);
}
value cpp_decrypt_aes256(value viv, value vk, value varr)
{
    return cpp_process_aes256(viv, vk, varr, -1);
}

} // extern C
