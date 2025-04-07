// OCaml includes
#include <fstream>
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
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
// CKey128* fromhex_Key128(const char*);
// void release_Key128(CKey128 *k);
// bool tohex_Key128(CKey128*, unsigned char buffer[], int buflen);

class CKey256;
// CKey256* fromhex_Key256(const char*);
// void release_Key256(CKey256 *k);
// bool tohex_Key256(CKey256*, unsigned char buffer[], int buflen);

static constexpr unsigned int datasz { 1024*4 };

// struct CAes {
//    void *ptr;
//    unsigned int lastpos;
//    unsigned char buf[datasz];
// };
// struct CAesEncrypt : public CAes {};
struct CAesEncrypt;
CAesEncrypt* mk_AesEncrypt(CKey256 *k, CKey128 *iv);
void release_AesEncrypt(CAesEncrypt *cl);
int proc_AesEncrypt(CAesEncrypt *cl, unsigned int inlen, unsigned char *);
int fin_AesEncrypt(CAesEncrypt *cl, unsigned char *);

struct CAesDecrypt; // : public CAes {};
CAesDecrypt* mk_AesDecrypt(CKey256 *k, CKey128 *iv);
void release_AesDecrypt(CAesDecrypt *cl);
int proc_AesDecrypt(CAesDecrypt *cl, unsigned int inlen, unsigned char *);
int fin_AesDecrypt(CAesDecrypt *cl, unsigned char *);
}

#include "sizebounded/sizebounded.hpp"
#include "sizebounded/sizebounded.ipp"

struct _cpp_cstdio_buffer {
    char *_buf {nullptr};
    long _len {0};
};

#define CPP_CSTDIO_BUFFER(v) (*((_cpp_cstdio_buffer**) Data_custom_val(v)))

/*
 *   cpp_encrypt_aes256 : key128 -> key256 -> blen -> buffer -> (len,buffer)
 *   cpp_decrypt_aes256 : key128 -> key256 -> blen -> buffer -> (len,buffer)
 */
extern "C" {
value cpp_process_aes256(value viv, value vk, value vn, value varr, const int crypto) {
    CAMLparam4(viv, vk, vn, varr);
    CAMLlocal1(pair);
    CKey128 *iv = *((CKey128**) Data_custom_val(viv));
    CKey256 *k = *((CKey256**) Data_custom_val(vk));
    void *p = NULL;
    if (crypto == 1) {
        p = (void*)mk_AesEncrypt(k, iv);
    } else {
        p = (void*)mk_AesDecrypt(k, iv);
    }
    struct _cpp_cstdio_buffer *arr = CPP_CSTDIO_BUFFER(varr);
    long sz = std::min(arr->_len, Long_val(vn));
    long ridx = 0;
    sizebounded<unsigned char, datasz> buf;
    long cnt = 0;
    long nread = 0;
    while (ridx < sz) {
        cnt = 0;
        long n = std::min(sz - ridx, (long)datasz - 16);
        std::memcpy((void*)buf.ptr(), arr->_buf + ridx, n);
        if (crypto == 1) {
            cnt = proc_AesEncrypt((CAesEncrypt *)p, n, (unsigned char*)buf.ptr());
            if (cnt > 0) {
                memcpy((unsigned char *)(arr->_buf + nread), buf.ptr(), cnt);
                nread += cnt;
            }
        } else {
            cnt = proc_AesDecrypt((CAesDecrypt *)p, n, (unsigned char*)buf.ptr());
            if (cnt > 0) {
                memcpy((unsigned char *)(arr->_buf + nread), buf.ptr(), cnt);
                nread += cnt;
            }
        }
        ridx += n;
    } // while
    cnt = 0;
    if (crypto == 1) {
        cnt = fin_AesEncrypt((CAesEncrypt *)p, (unsigned char *)(arr->_buf + nread));
        if (cnt > 0) {
            nread += cnt;
        }
    } else {
        cnt = fin_AesDecrypt((CAesDecrypt *)p, (unsigned char *)(arr->_buf + nread));
        if (cnt > 0) {
            nread += cnt;
        }
    }
    if (crypto == 1) {
        release_AesEncrypt((CAesEncrypt *)p);
    } else {
        release_AesDecrypt((CAesDecrypt *)p);
    }
    // std::clog << "total processed " << nread << std::endl;
    pair = caml_alloc_tuple(2);
    Store_field(pair, 0, Val_long(nread));
    Store_field(pair, 1, varr);
    CAMLreturn(pair);
}

value cpp_encrypt_aes256(value viv, value vk, value vn, value varr)
{
    return cpp_process_aes256(viv, vk, vn, varr, 1);
}
value cpp_decrypt_aes256(value viv, value vk, value vn, value varr)
{
    return cpp_process_aes256(viv, vk, vn, varr, -1);
}

} // extern C
