#!/bin/bash
for FS in ee em mm; do 
    for J in $(seq 2 6); do
        root.exe -b -l "trainMVA_2lss.cxx(\"2lss_mix_${FS}_${J}jet\")" -q
    done
done
