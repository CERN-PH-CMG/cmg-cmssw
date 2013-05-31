#!/bin/sh

for resonance in `seq  1000 100 2700`
do
    python interpolate.py ~hinzmann/public/yxin/RSGWWherwig $resonance &
    python interpolate.py ~hinzmann/public/yxin/RSGZZherwig $resonance &
    python interpolate.py ~hinzmann/public/yxin/Wprime $resonance
done

for resonance in `seq  1000 100 3700`
do
    python interpolate.py ~hinzmann/public/yxin/QstarQW $resonance &
    python interpolate.py ~hinzmann/public/yxin/QstarQZ $resonance
done
