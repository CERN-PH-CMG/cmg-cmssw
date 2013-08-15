rm -rf SMAll
mkdir SMAll
for i in 110 115 120 125 130 135 140 145; do
echo SMAll/$i
mkdir SMAll/$i
rm -f ./SMAll/$i/muTauSM.root
cp muTauSM.root ./SMAll/$i/.
for d in 0 1 2 3 5; do
echo $d
rm -f ./SMAll/$i/muTau_$d\_$i.txt
cp muTau_$d\_$i.txt ./SMAll/$i/.
done
done
