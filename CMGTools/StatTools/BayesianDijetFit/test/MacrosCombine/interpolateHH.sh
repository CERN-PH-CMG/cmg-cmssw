#!/bin/sh

for resonance in `seq  1000 100 3000`
do
    python interpolate.py ~hinzmann/public/limit_input_files/RtoHHto4b $resonance &
done
