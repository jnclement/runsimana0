#!/bin/bash

NEVT=$2
bash createfilelistcommand $NEVT
a=($(wc dst_global.list))
NFILES=${a[0]}

echo "executable = firstcondor.sh" > firstcondor.sub
echo 'arguments = $(Process)' >> firstcondor.sub
echo 'output = output/out/output'${1}'$(Process).out' >> firstcondor.sub
echo 'transfer_input_files	= /sphenix/user/jocl/projects/testinstall/lib/libmdctreemaker.so' >> firstcondor.sub
echo 'should_transfer_files	= IF_NEEDED' >> firstcondor.sub
echo 'when_to_transfer_output = ON_EXIT' >> firstcondor.sub
echo 'transfer_output_files	= events_$(Process).root to=output/evt/' >> firstcondor.sub
echo 'error			= output/err/error'${1}'$(Process).err' >> firstcondor.sub
echo 'log = output/log/log'${1}'$(Process).log' >> firstcondor.sub
echo "queue $NFILES" >> firstcondor.sub
condor_submit firstcondor.sub

condor_q > temps/last.q

grep -q "Total for jocl: 0 jobs; 0 completed, 0 removed, 0 idle, 0 running, 0 held, 0 suspended" temps/last.q

until [ $? -ne 1 ]
do
    echo "Still running as of $(date +%F_%H-%M-%S)"
    grep -Eq "Total for jocl: .* jobs; .* completed, 0 removed, .* idle, .* running, 0 held, 0 suspended" temps/last.q
    if [ $? -ne 0 ]; then
	echo "Failure"
	echo "ERROR - SOME JOBS FAILED AT $(date +%F_%H-%M-%S)" > error.WARN
	exit 1
    fi
    sleep 15s
    condor_q > temps/last.q
    grep -q "Total for jocl: 0 jobs; 0 completed, 0 removed, 0 idle, 0 running, 0 held, 0 suspended" temps/last.q
done

mv events_*.root output/evt/

root -q 'merge_root.C('$NFILES',"'${1}'")'

#bash cleanup.sh

mv 'results/'${1}$NFILES'.root' 'results/'${1}"_"$NFILES"_$(date +%F_%H-%M-%S).root"
