#!/bin/bash

for COR in cor unc; do
    for DM in dat; do
	condor_submit merge_$DM\_$COR.sub
    done
done
