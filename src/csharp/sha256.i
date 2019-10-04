
%module Sha256

%import "key256.i"

%{
  typedef int CKey256;

  CKey256* hash_Sha256(int, char*);
  CKey256* filehash_Sha256(char*);
%}

#include "lxr/key256.hpp";

CKey256* hash_Sha256(int, char*);
CKey256* filehash_Sha256(char*);

