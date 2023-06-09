#!/usr/bin/env bash

UNAME_S=$(uname -s)
UNAME_M=$(uname -m)

GITALK=$(pwd)/../ext/gitalk

if [ -e ${GITALK}/../${UNAME_S}_${UNAME_M}/bin/html2text ]; then
  export PATH=${GITALK}/../${UNAME_S}_${UNAME_M}/bin/:$PATH
fi

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

