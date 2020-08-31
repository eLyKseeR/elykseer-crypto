
%module lxr_Key256
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

%insert("mli") %{
val with_key256 : (string -> 'a) -> 'a
%}
%insert("ml") %{
let with_key128 f =
    let p = _mk_Key256 C_void in
    let s = _tohex_Key256 p |> Swig.get_string in
    let r = f s in
    _release_Key256 p ;
    r
%}
