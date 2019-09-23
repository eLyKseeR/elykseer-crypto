
%module Key256
%{

typedef int CKey256;

CKey256* mk_Key256();
char* tohex_Key256(CKey256*);
%}

#include "lxr/key256.hpp"

CKey256* mk_Key256();
char* tohex_Key256(CKey256*);
