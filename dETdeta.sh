#!/bin/bash
# file name: firstcondor.sh

source /opt/sphenix/core/bin/sphenix_setup.sh -n
source /opt/sphenix/core/bin/setup_local.sh "/sphenix/user/jocl/projects/testinstall"
export TESTINSTALL="/sphenix/user/jocl/projects/testinstall"
export HOME=/sphenix/u/jocl
root -b -q 'run_dETdeta.C('${1},\"${2}\",${3},${4},${5},${6},${7},${8}')'
