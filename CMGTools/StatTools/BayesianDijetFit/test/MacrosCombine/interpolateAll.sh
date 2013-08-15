#!/bin/sh

for resonance in `seq  1000 100 3000`
do
    python interpolate.py ~hinzmann/public/yxin/RSGWWherwig $resonance &
    python interpolate.py ~hinzmann/public/yxin/RSGZZherwig $resonance &
    python interpolate.py ~hinzmann/public/yxin/RSGWWpythia $resonance &
    python interpolate.py ~hinzmann/public/yxin/RSGZZpythia $resonance &
    python interpolate.py ~hinzmann/public/yxin/Wprime $resonance
done

for resonance in `seq  1000 100 4000`
do
    python interpolate.py ~hinzmann/public/yxin/QstarQW $resonance &
    python interpolate.py ~hinzmann/public/yxin/QstarQZ $resonance
done

for resonance in `seq  1000 100 2500`
do
    python interpolate.py ~hinzmann/public/yxin/Bulk $resonance
done
