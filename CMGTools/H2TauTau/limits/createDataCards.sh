for i in 110 115 120 125 130 135 140 145; do
echo $i
rootb histosForLimit.C\(0,$i\) >> datacard_0_$i.log  
rootb histosForLimit.C\(1,$i\) >> datacard_1_$i.log 
rootb histosForLimit.C\(2,$i\) >> datacard_2_$i.log 
done