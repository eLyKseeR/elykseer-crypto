
%module lxr_Sha256

%import "key256.i"

%{
  typedef int CKey256;

  CKey256* hash_Sha256(int, char*);
  CKey256* filehash_Sha256(char*);
%}

#include "lxr/key256.hpp";

CKey256* hash_Sha256(int, char*);
CKey256* filehash_Sha256(char*);

%insert("mli") %{
val with_sha256 : string -> (string -> 'a) -> 'a
val with_file_sha256 : string -> (string -> 'a) -> 'a
%}
%insert("ml") %{
let with_sha256 s f =
    let p = _hash_Sha256 (C_list [make_int (String.length s); make_string s]) in
    let h = Lxr_Key256._tohex_Key256 p |> Swig.get_string in
    let r = f h in
    Lxr_Key256._release_Key256 p ;
    r
let with_file_sha256 s f =
    let p = _filehash_Sha256 (make_string s) in
    let h = Lxr_Key256._tohex_Key256 p |> Swig.get_string in
    let r = f h in
    Lxr_Key256._release_Key256 p ;
    r
%}
