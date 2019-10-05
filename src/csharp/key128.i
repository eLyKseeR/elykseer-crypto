
%module Key128
%{

typedef int CKey128;

CKey128* mk_Key128();
void release_Key128(CKey128*);
int len_Key128(CKey128*);
char* tohex_Key128(CKey128*);
CKey128* fromhex_Key128(const char*);
%}

#include "lxr/key128.hpp"

CKey128* mk_Key128();
void release_Key128(CKey128*);
int len_Key128(CKey128*);
char* tohex_Key128(CKey128*);
CKey128* fromhex_Key128(const char*);
