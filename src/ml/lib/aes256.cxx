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
#include <iostream>

extern "C" {
class CKey128;
CKey128* fromhex_Key128(const char*);
void release_Key128(CKey128 *k);
class CKey256;
CKey256* fromhex_Key256(const char*);
void release_Key256(CKey256 *k);

static constexpr unsigned int datasz { 1024*4 };

struct CAes {
   void *ptr;
   unsigned int lastpos;
   unsigned char buf[datasz];
};
struct CAesEncrypt : public CAes {};
struct CAesEncrypt;
CAesEncrypt* mk_AesEncrypt(CKey256 *k, CKey128 *iv);
void release_AesEncrypt(CAesEncrypt *cl);
int proc_AesEncrypt(CAesEncrypt *cl, unsigned int inlen, unsigned char const *);
int fin_AesEncrypt(CAesEncrypt *cl);
unsigned int len_AesEncrypt(CAesEncrypt *cl);
unsigned int copy_AesEncrypt(CAesEncrypt *cl, unsigned int outlen, unsigned char *);
unsigned int sz_AesEncrypt();

struct CAesDecrypt : public CAes {};
CAesDecrypt* mk_AesDecrypt(CKey256 *k, CKey128 *iv);
void release_AesDecrypt(CAesDecrypt *cl);
int proc_AesDecrypt(CAesDecrypt *cl, unsigned int inlen, unsigned char const *);
int fin_AesDecrypt(CAesDecrypt *cl);
unsigned int len_AesDecrypt(CAesDecrypt *cl);
unsigned int copy_AesDecrypt(CAesDecrypt *cl, unsigned int outlen, unsigned char *);
unsigned int sz_AesDecrypt();
}

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

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
    CKey128 *iv = fromhex_Key128(String_val(viv));
    CKey256 *k = fromhex_Key256(String_val(vk));
    CAes *p;
    if (crypto == 1) {
        p = (CAes*)mk_AesEncrypt(k, iv);
    } else {
        p = (CAes*)mk_AesDecrypt(k, iv);
    }
    struct _cpp_cstdio_buffer *arr = CPP_CSTDIO_BUFFER(varr);
    long sz = arr->_len;
    long idx = 0;
    long lst = 0;
    sizebounded<unsigned char, datasz> buf;
    unsigned char outbuf[datasz];
    long cnt = 0;
    long nread = 0;
    while (idx < sz) {
        long n = std::min(sz - idx, (long)datasz);
        std::memcpy((void*)buf.ptr(), arr->_buf + idx, n);
        if (crypto == 1) {
            cnt = proc_AesEncrypt((CAesEncrypt *)p, n, buf.ptr());
            if (cnt > 0)
                cnt = copy_AesEncrypt((CAesEncrypt *)p, datasz, outbuf);
        } else {
            cnt = proc_AesDecrypt((CAesDecrypt *)p, n, buf.ptr());
            if (cnt > 0)
                cnt = copy_AesDecrypt((CAesDecrypt *)p, datasz, outbuf);
        }
        std::clog << "processed " << cnt << std::endl;
        if (cnt > 0) {
            nread += cnt;
            std::memcpy((void*)(arr->_buf + idx), outbuf, cnt);
        }
        lst = idx;
        idx = idx + n;
    }
    if (nread < sz) {
        if (crypto == 1) {
            // cnt = proc_AesEncrypt((CAesEncrypt *)p, cnt, buf.ptr());
            cnt = fin_AesEncrypt((CAesEncrypt *)p);
            if (cnt > 0)
                cnt = copy_AesEncrypt((CAesEncrypt *)p, datasz, outbuf);
        } else {
            // cnt = proc_AesDecrypt((CAesDecrypt *)p, cnt, buf.ptr());
            cnt = fin_AesDecrypt((CAesDecrypt *)p);
            if (cnt > 0)
                cnt = copy_AesDecrypt((CAesDecrypt *)p, datasz, outbuf);
        } 
        std::clog << "processed (finally) " << cnt << std::endl;
        if (cnt > 0) {
            std::memcpy((void*)(arr->_buf + lst), outbuf, cnt);
        }
    }
    if (crypto == 1) {
        release_AesEncrypt((CAesEncrypt *)p);
    } else {
        release_AesDecrypt((CAesDecrypt *)p);
    }
    release_Key128(iv);
    release_Key256(k);
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
