
%module Aes

%import "key256.i"
%import "key128.i"

%{

typedef int CKey256;
typedef int CKey128;
typedef int CAesEncrypt;
typedef int CAesDecrypt;

CAesEncrypt* mk_AesEncrypt(CKey256*, CKey128*);
CAesDecrypt* mk_AesDecrypt(CKey256*, CKey128*);
int proc_AesEncrypt(CAesEncrypt*, int, char*);
int fin_AesEncrypt(CAesEncrypt*, int, char*);
int proc_AesDecrypt(CAesDecrypt*, int, char*);
int fin_AesDecrypt(CAesDecrypt*, int, char*);

%}

#include "lxr/key256.hpp";
#include "lxr/key128.hpp";
#include "lxr/aes.hpp"

CAesEncrypt* mk_AesEncrypt(CKey256*, CKey128*);
CAesDecrypt* mk_AesDecrypt(CKey256*, CKey128*);
int proc_AesEncrypt(CAesEncrypt*, int, char*);
int fin_AesEncrypt(CAesEncrypt*, int, char*);
int proc_AesDecrypt(CAesDecrypt*, int, char*);
int fin_AesDecrypt(CAesDecrypt*, int, char*);

