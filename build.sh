#!/bin/sh

set -e

git submodule update --init --remote
cd ext/prngsharp; ./build.Linux.jenkins.sh
cd ../cryptopp; make -f GNUmakefile -j2
cd ../../BUILD
./mk_cpp.sh
cmake .
make
./ut.sh

