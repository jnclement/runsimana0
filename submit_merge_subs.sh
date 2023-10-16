B#!/bin/bash

for file in merge_*.sub; do
    condor_submit $file
done
