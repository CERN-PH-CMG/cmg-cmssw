#!/bin/sh

for resonance in `seq  1000 100 2300`
do
    python double.py dijetWtag_Moriond_WWHpp $resonance &
    python double.py dijetWtag_Moriond_ZZHpp $resonance &
    python double.py dijetWtag_Moriond_WZPy6 $resonance
done
