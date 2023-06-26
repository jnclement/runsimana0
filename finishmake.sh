NFILES=$2

mv events_*.root output/evt/

root -q 'merge_root.C('$NFILES',"'${1}'")'

#bash cleanup.sh

mv 'results/'${1}$NFILES'.root' 'results/'${1}"_"$NFILES"_$(date +%F_%H-%M-%S).root"
