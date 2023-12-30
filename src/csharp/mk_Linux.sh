#!/bin/sh

set -xe

DLLNAME=elykseer-crypto-cs
NAMESPACE=lxr

swig -csharp -dllimport ${DLLNAME} -namespace ${NAMESPACE} md5.i
swig -csharp -dllimport ${DLLNAME} -namespace ${NAMESPACE} key128.i
swig -csharp -dllimport ${DLLNAME} -namespace ${NAMESPACE} key256.i
swig -csharp -dllimport ${DLLNAME} -namespace ${NAMESPACE} sha256.i
swig -csharp -dllimport ${DLLNAME} -namespace ${NAMESPACE} aes.i

VERSION="1.0.1"

LDFLAGS="-L ../../build/src/ -lelykseer-crypto_Debug"
CFLAGS="-Wall -fPIC -O2 -I. -compatibility_version 1.0.0 -current_version ${VERSION}"
CFLAGS_dyn="${CFLAGS} ${LDFLAGS}"
CFLAGS_s="${CFLAGS} ../../build/src/libelykseer-crypto_Debug_s.a ../../ext/cryptopp/libcryptopp.a -lz -lboost_contract -lboost_system"

clang ${CFLAGS} -c *_wrap.c

clang++ -o lib${DLLNAME}.linux.${VERSION}.so -shared \
    *.o \
    ${CFLAGS_s}


# the real DLL
csc -target:library -out:${DLLNAME}.dll [A-Z]*.cs

# an example program using the DLL
csc -target:exe -out:runme.exe -reference:${DLLNAME}.dll  runme.cs

