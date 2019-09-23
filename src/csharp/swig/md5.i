
%module Md5

%import "key128.i"

%{
  typedef int CKey128;

  CKey128* hash_Md5(int, char*);
%}

#include "lxr/key128.hpp";

CKey128* hash_Md5(int, char*);

