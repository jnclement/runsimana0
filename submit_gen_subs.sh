for ZS in nz; do
    for DM in mc dat; do
	condor_submit dETdeta_$DM\_$ZS.sub
    done
done
