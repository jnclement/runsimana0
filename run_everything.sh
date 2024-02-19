#!/bin/bash

TAG=$1
DATMC=$2
DEBUG=$3
NEVT=$4
COR=$5
ZS=$6
UWB=$7
DOUW=$8
RN=$9
NFILE=${10}

if [ $# -lt 10 ]; then
    echo "Need arguments (in order): tag (string), data/mc (0/1), debug (0/1), nevt (int), correct zvtx (0/1), zero suppression (0/1), upweight reco (0/1), upweight truth (0/1), run number (int)"
    exit 1
fi

if [ $DATMC -eq 0 ]; then
    CreateDstList.pl --run $RN --cdb 2023p009 --build ana.402 DST_CALO
fi

if [ $DATMC -eq 1]; then
    CreateFileList.pl -type 4 -n 500000 -run 10 -nop DST_GLOBAL DST_CALO_CLUSTER DST_CALO_NOZERO DST_TRUTH G4Hits
fi

TAG+="_run${RN}"

BASENAME="condor_${TAG}_${NFILE}_${NEVT}_${DATMC}_${DEBUG}${COR}${ZS}_${UWB}${DOUW}"

SUBNAME="${BASENAME}.sub"

echo "executable = dETdeta.sh" > $SUBNAME
echo "arguments = \$(Process) ${TAG} ${DATMC} ${DEBUG} ${NEVT} ${COR} ${ZS} ${UWB} $DOUW $RN" >> $SUBNAME
echo "output = output/out/output_${BASENAME}.out" >> $SUBNAME
echo "should_transfer_files   = IF_NEEDED" >> $SUBNAME
echo "when_to_transfer_output = ON_EXIT" >> $SUBNAME
echo "error = output/err/error_${BASENAME}.err" >> $SUBNAME
echo "log = /tmp/jocl_${BASENAME}.log" >> $SUBNAME
echo "queue ${NFILE}" >> $SUBNAME

condor_submit $SUBNAME
