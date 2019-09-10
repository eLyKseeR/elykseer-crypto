#!/bin/bash

GITALK=`pwd`/../ext/gitalk

cd src
cd lxr
for HPP in `bash $GITALK/utils/find_hpp.sh ../../../src/cpp/elykseer-crypto.md`; do
  bash $GITALK/utils/make_hpp.sh ${HPP}
done

cd ..
for HPP in `bash $GITALK/utils/find_hpp.sh ../../src/cpp/elykseer-crypto.md`; do
  echo ${HPP}
  bash $GITALK/utils/make_cpp.sh ${HPP}
done

cd ..
cd test

$GITALK/utils/make_test.sh ../../test/cpp/elykseer-crypto-ut.md

cd ..

