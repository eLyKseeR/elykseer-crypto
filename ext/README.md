External Dependencies
=====================

the content of the 'ext/' directory:

  cryptopp
  gitalk/
  sizebounded/


gitalk
======

license: BSD3

https://github.com/CodiePP/gitalk.git


sizebounded
===========

license: BSD3

https://github.com/CodiePP/sizebounded.git


Crypto++
========

license: Boost Software License 1.0

https://github.com/weidai11/cryptopp.git

on OSX build with env. var. set:

> export USER_CXXFLAGS="-mmacosx-version-min=10.12"
> ./cryptest.sh fast


make install

on Darwin:
PREFIX="../darwin" make install

GPGme
=====

license: GNU Lesser General Public License (LGPL)
         or the GNU General Public License (GPL)

https://dev.gnupg.org/source/gpgme.git


and its dependencies:

export CPPFLAGS="-I$(pwd)/../win64/include"
export LDFLAGS="-L$(pwd)/../win64/lib"

libgpg-error
	./configure --enable-static --disable-shared  --disable-doc --prefix=$(pwd)/../win64 --enable-install-gpg-error-config
libassuan
	./configure --enable-static --disable-shared  --disable-doc --prefix=$(pwd)/../win64
gpgme
	./configure --enable-static --disable-shared --enable-languages=cpp --disable-gpgsm-test --disable-g13-test --disable-gpg-test --disable-gpgconf-test --with-libassuan-prefix=$(pwd)/../win64  --with-libgpg-error-prefix=$(pwd)/../win64  --prefix=$(pwd)/../win64


(from https://gnupg.org/ftp/gcrypt/)


## cross compilation

### Windows


Prepare dependencies in "ext":

1. cd ext

2. download a zip and extract here: https://github.com/skeeto/w64devkit/releases

3. set environment variables CC, CXX, AR, RANLIB to point to "x86_64-w64-mingw32-gcc"

4. run `make xWin64`


Compile project using platform file:

1. cd build

2. cmake --fresh -DCMAKE_TOOLCHAIN_FILE=Toolchain_Darwin_to_Windows.cmake -DCMAKE_BUILD_TYPE=Release .



