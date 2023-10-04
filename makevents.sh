#!/bin/bash

#NEVT=$2
#bash createfilelistcommand $NEVT
a=($(wc dsts_dEdeta_study))
NFILES=${a[0]}

echo "executable = firstcondor.sh" > firstcondor.sub
echo 'arguments = $(Process)' >> firstcondor.sub
echo 'output = output/out/output$(Process).out' >> firstcondor.sub
echo 'transfer_input_files	= /sphenix/user/jocl/projects/testinstall/lib/libmdctreemaker.so' >> firstcondor.sub
echo 'should_transfer_files	= IF_NEEDED' >> firstcondor.sub
echo 'when_to_transfer_output = ON_EXIT' >> firstcondor.sub
echo 'error			= output/err/error$(Process).err' >> firstcondor.sub
echo 'log = /tmp/jocl_$(Process).log' >> firstcondor.sub
echo "queue $NFILES" >> firstcondor.sub
#condor_submit firstcondor.sub
