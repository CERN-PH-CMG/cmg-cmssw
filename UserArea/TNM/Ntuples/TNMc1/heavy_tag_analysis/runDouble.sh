#!/bin/sh

for resonance in `seq  1000 100 2300`
do
    python heavy_tag_analysis/double.py dijetWtag_Moriond_WWHpp $resonance &
    python heavy_tag_analysis/double.py dijetWtag_Moriond_ZZHpp $resonance &
    python heavy_tag_analysis/double.py dijetWtag_Moriond_WZPy6 $resonance
done
