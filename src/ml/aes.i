
%module lxr_Aes

%import "key256.i"
%import "key128.i"

%{

typedef int CKey256;
typedef int CKey128;
typedef int CAesEncrypt;
typedef int CAesDecrypt;

CAesEncrypt* mk_AesEncrypt(CKey256*, CKey128*);
void release_AesEncrypt(CAesEncrypt*);
unsigned int sz_AesEncrypt();
int proc_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char const*);
int fin_AesEncrypt(CAesEncrypt*);
unsigned int len_AesEncrypt(CAesEncrypt*);
unsigned int copy_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char*);

CAesDecrypt* mk_AesDecrypt(CKey256*, CKey128*);
void release_AesDecrypt(CAesDecrypt*);
unsigned int sz_AesEncrypt();
int proc_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char const*);
int fin_AesDecrypt(CAesDecrypt*);
unsigned int len_AesDecrypt(CAesDecrypt*);
unsigned int copy_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char*);

%}

#include "lxr/key256.hpp";
#include "lxr/key128.hpp";
#include "lxr/aes.hpp"

CAesEncrypt* mk_AesEncrypt(CKey256*, CKey128*);
unsigned int sz_AesEncrypt();
void release_AesEncrypt(CAesEncrypt*);
int proc_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char const *srcArr);
int fin_AesEncrypt(CAesEncrypt*);
unsigned int len_AesEncrypt(CAesEncrypt*);
buflen copy_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char *tgtArr);

CAesDecrypt* mk_AesDecrypt(CKey256*, CKey128*);
unsigned int sz_AesDecrypt();
void release_AesDecrypt(CAesDecrypt*);
int proc_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char const *srcArr);
int fin_AesDecrypt(CAesDecrypt*);
unsigned int len_AesDecrypt(CAesDecrypt*);
buflen copy_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char *tgtArr);

%inline %{
#define datasz 4096
typedef unsigned int buflen;
%}
