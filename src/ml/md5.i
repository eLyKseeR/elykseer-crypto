
%module lxr_Md5

%import "key128.i"

%{
  typedef int CKey128;

  CKey128* hash_Md5(int, char*);
%}

#include "lxr/key128.hpp";

CKey128* hash_Md5(int, char*);

%insert("mli") %{
val with_md5 : string -> (string -> 'a) -> 'a
%}
%insert("ml") %{
let with_md5 s f =
    let p = _hash_Md5 (C_list [make_int (String.length s); make_string s]) in
    let h = Lxr_Key128._tohex_Key128 p |> Swig.get_string in
    let r = f h in
    Lxr_Key128._release_Key128 p ;
    r
%}
