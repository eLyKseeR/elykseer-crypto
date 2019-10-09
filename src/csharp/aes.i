
%module Aes

%import "key256.i"
%import "key128.i"

%{

typedef int CKey256;
typedef int CKey128;
typedef int CAesEncrypt;
typedef int CAesDecrypt;

CAesEncrypt* mk_AesEncrypt(CKey256*, CKey128*);
void release_AesEncrypt(CAesEncrypt*);
int proc_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char const*);
int fin_AesEncrypt(CAesEncrypt*);
unsigned int len_AesEncrypt(CAesEncrypt*);
unsigned int copy_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char*);

CAesDecrypt* mk_AesDecrypt(CKey256*, CKey128*);
void release_AesDecrypt(CAesDecrypt*);
int proc_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char const*);
int fin_AesDecrypt(CAesDecrypt*);
unsigned int len_AesDecrypt(CAesDecrypt*);
unsigned int copy_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char*);

%}

#ifdef SWIGCSHARP
// rename argument type (input)
%typemap(cstype) unsigned char const * "in byte[]"
// rename argument type (output)
%typemap(cstype) unsigned char * "out byte[]"

// type map for "proc_Aes[De|En]crypt"
%typemap(csin,
         pre="    // pre cond \n"
             "    int sz = System.Runtime.InteropServices.Marshal.SizeOf($csinput[0]) * $csinput.Length; \n"
             "    System.IntPtr temp$csinput = System.Runtime.InteropServices.Marshal.AllocHGlobal(sz); \n"
             "    System.Runtime.InteropServices.Marshal.Copy($csinput, 0, temp$csinput, $csinput.Length); ",
         post="    // post cond \n"
              "    System.Runtime.InteropServices.Marshal.FreeHGlobal(temp$csinput); "
        ) unsigned char const *
          "new System.Runtime.InteropServices.HandleRef(null, temp$csinput)"

// type map for "copy_Aes[De|En]crypt": adapt input type
%typemap(csin,
         pre="    // pre cond \n"
             "    int sz = (int)arg1; \n"
             "    System.IntPtr temp$csinput = System.Runtime.InteropServices.Marshal.AllocHGlobal(sz); ",
         post="    // post cond \n"
              "    System.Runtime.InteropServices.Marshal.FreeHGlobal(temp$csinput); "
        ) unsigned char *
          "new System.Runtime.InteropServices.HandleRef(null, temp$csinput)"

// type map for "copy_Aes[De|En]crypt": adapt intermediate type; assign output argument
%typemap(csout) buflen %{
   {
    uint len = $imcall;$excode
    if (len > 0) {
      byte[] newarr = new byte[len];
      System.Runtime.InteropServices.Marshal.Copy(temptgtArr, newarr, 0, (int)len);
      tgtArr = newarr;
    } else {
      tgtArr = null;
    }
    return len;
   }
%}
#endif

#include "lxr/key256.hpp";
#include "lxr/key128.hpp";
#include "lxr/aes.hpp"

CAesEncrypt* mk_AesEncrypt(CKey256*, CKey128*);
void release_AesEncrypt(CAesEncrypt*);
int proc_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char const *srcArr);
int fin_AesEncrypt(CAesEncrypt*);
unsigned int len_AesEncrypt(CAesEncrypt*);
buflen copy_AesEncrypt(CAesEncrypt*, unsigned int, unsigned char *tgtArr);

CAesDecrypt* mk_AesDecrypt(CKey256*, CKey128*);
void release_AesDecrypt(CAesDecrypt*);
int proc_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char const *srcArr);
int fin_AesDecrypt(CAesDecrypt*);
unsigned int len_AesDecrypt(CAesDecrypt*);
buflen copy_AesDecrypt(CAesDecrypt*, unsigned int, unsigned char *tgtArr);

%inline %{
#define datasz 4096
typedef unsigned int buflen;
%}
