for d in 0 1 2 3 5; do
echo $d
rm -rf SM$d
mkdir SM$d
for i in 110 115 120 125 130 135 140 145; do
echo SM$d/$i
mkdir SM$d/$i
rm -f ./SM$d/$i/muTau_$d\_$i.txt
cp muTau_$d\_$i.txt ./SM$d/$i/.
rm -f ./SM$d/$i/muTauSM.root
cp muTauSM.root ./SM$d/$i/.
done
done
