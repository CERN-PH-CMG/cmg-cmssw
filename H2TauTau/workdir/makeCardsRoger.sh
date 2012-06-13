for d in 0 1 2 3 4; do
for i in 110 115 120 125 130 135 140 145; do
echo SM$d\_$i
create-datacard.py -i muTauSM.root -o muTau_$d\_$i.txt -c ../cgs-7TeV-0$d.conf -u ../unc-7TeV-0$d.vals -d ../unc-7TeV-0$d.conf $i
done
done
