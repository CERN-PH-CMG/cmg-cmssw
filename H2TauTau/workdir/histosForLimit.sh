
rm -rf muTau
mkdir muTau
for i in 110 115 120 125 130 135 140 145; do
echo $i
rootb histosForLimit.C\(0,$i\) 
rootb histosForLimit.C\(1,$i\) 
rootb histosForLimit.C\(2,$i\) 
done
