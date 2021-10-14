#!/bin/sh

set -xe


DLLNAME=elykseer-crypto-ml

TLANG=ocaml

swig -${TLANG} -co swig.mli ; swig -${TLANG} -co swig.ml
swig -${TLANG}  key128.i
swig -${TLANG}  key256.i
swig -${TLANG}  aes.i
swig -${TLANG}  md5.i
swig -${TLANG}  sha256.i

ocamlc -c swig.mli   ; ocamlc -c swig.ml
ocamlc -c lxr_Key128.mli ; ocamlc -c lxr_Key128.ml
ocamlc -c lxr_Key256.mli ; ocamlc -c lxr_Key256.ml
ocamlc -c lxr_Aes.mli    ; ocamlc -c lxr_Aes.ml
ocamlc -c lxr_Md5.mli    ; ocamlc -c lxr_Md5.ml
ocamlc -c lxr_Sha256.mli ; ocamlc -c lxr_Sha256.ml

VERSION="1.0.1"

LDFLAGS="-L ../../BUILD/src/ -lelykseer-crypto_Debug"
# for Clang
export CXX=clang++
export CC=clang
CFLAGS="-Wall -fPIC -O2 -I. -Iocaml-4.09.1/_opam/lib/ocaml -compatibility_version 1.0.1 -current_version ${VERSION}"
# for GCC
#export CXX=g++
#export CC=gcc
#CFLAGS="-Wall -fPIC -O2 -I. -Iocaml-4.09.1/_opam/lib/ocaml "
CFLAGS_dyn="${CFLAGS} ${LDFLAGS}"
CFLAGS_s="${CFLAGS} ../../BUILD/src/libelykseer-crypto_Debug_s.a ../../ext/prngsharp/libprngCpp_s.osx.1.0.5.a ../../ext/cryptopp/libcryptopp.a -lz -lboost_contract -lboost_system"

## patch: int64 -> int64_t
gsed -i 's,#define SWIG_Int64_val(v) (\*((int64 \*) SWIG_Data_custom_val(v))),#define SWIG_Int64_val(v) (*((int64_t *) SWIG_Data_custom_val(v))),' *_wrap.c
gsed -i 's,CAMLextern int64 Int64_val(caml_value_t v);,CAMLextern int64_t Int64_val(caml_value_t v);,' *_wrap.c

$CC ${CFLAGS} -c *_wrap.c
ar r libwrap_all.a aes_wrap.o key128_wrap.o key256_wrap.o md5_wrap.o sha256_wrap.o
ranlib libwrap_all.a

$CXX -o lib${DLLNAME}.osx.${VERSION}.so -shared \
    *.o \
    ${CFLAGS_s}

## top level

ocamlmktop -linkall -custom -o elykseerTop swig.cmo lxr_Key128.cmo lxr_Key256.cmo lxr_Aes.cmo lxr_Md5.cmo lxr_Sha256.cmo \
    -cclib libwrap_all.a \
    -cclib ../../BUILD/src/libelykseer-crypto_Debug_s.a \
    -cclib ../../ext/prngsharp/libprngCpp_s.osx.1.0.5.a \
    -cclib ../../ext/cryptopp/libcryptopp.a \
    -cclib "-lz -lboost_contract -lboost_system -lstdc++"

