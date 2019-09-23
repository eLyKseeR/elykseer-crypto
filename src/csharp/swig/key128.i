
%module Key128
%{

typedef int CKey128;

CKey128* mk_Key128();
char* tohex_Key128(CKey128*);
%}

#include "lxr/key128.hpp"

CKey128* mk_Key128();
char* tohex_Key128(CKey128*);
