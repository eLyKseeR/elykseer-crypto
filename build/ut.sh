#!/bin/sh

#./test/utelykseer-crypto --show_progress -l all
./test/utelykseer-crypto --show_progress  $*

# run the disabled GPG test:
# ./test/utelykseer-crypto --run_test=utGpg -l all
