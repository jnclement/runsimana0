#!/bin/bash

for ZS in nzs; do
    for DM in mc dat; do
	condor_submit merge_$DM\_$ZS.sub
    done
done
