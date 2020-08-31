
%module lxr_Key128
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

%insert("mli") %{
val with_key128 : (string -> 'a) -> 'a
%}
%insert("ml") %{
let with_key128 f =
    let p = _mk_Key128 C_void in
    let s = _tohex_Key128 p |> Swig.get_string in
    let r = f s in
    _release_Key128 p ;
    r
%}
