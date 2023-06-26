#!/bin/bash
# file name: firstcondor.sh

source /opt/sphenix/core/bin/sphenix_setup.sh -n
source /opt/sphenix/core/bin/setup_local.sh ~/sphenix/install > /dev/null 2>&1
export TESTINSTALL="/sphenix/user/jocl/projects/testinstall"
export LD_LIBRARY_PATH="${TESTINSTALL}/lib:$LD_LIBRARY_PATH"
export HOME=/sphenix/u/jocl
root -q 'run_macro.C('${1}')'
