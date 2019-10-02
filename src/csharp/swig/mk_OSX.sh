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

CPPFLAGS="-I."
LDFLAGS="-L ../../../BUILD/src/ -lelykseer-crypto_Debug"
CFLAGS_dyn="-Wall -fPIC -O2 -Isrc ${LDFLAGS} ${CPPFLAGS}"
#CFLAGS_s="-Wall -O2 -Isrc ${LDFLAGS} ${CPPFLAGS}"

export MACOSX_DEPLOYMENT_TARGET="10.11.0"

clang -o lib${DLLNAME}.osx.${VERSION}.dylib -dynamiclib \
   md5_wrap.c \
   key128_wrap.c \
   key256_wrap.c \
   sha256_wrap.c \
   aes_wrap.c \
   ${CFLAGS_dyn}


# the real DLL
csc -target:library -out:${DLLNAME}.dll [A-Z]*.cs

# an example program using the DLL
csc -target:exe -out:runme.exe -reference:${DLLNAME}.dll  runme.cs

