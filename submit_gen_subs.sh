for COR in cor unc; do
    for DM in dat mc; do
	condor_submit dETdeta_$DM\_$COR.sub
    done
done
