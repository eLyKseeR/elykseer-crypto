
%module Key256
%{

typedef int CKey256;

CKey256* mk_Key256();
void release_Key256(CKey256*);
int len_Key256(CKey256*);
char* tohex_Key256(CKey256*);
CKey256* fromhex_Key256(const char*);
%}

#include "lxr/key256.hpp"

CKey256* mk_Key256();
void release_Key256(CKey256*);
int len_Key256(CKey256*);
char* tohex_Key256(CKey256*);
CKey256* fromhex_Key256(const char*);
