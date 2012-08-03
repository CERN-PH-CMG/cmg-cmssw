export muTauUncsDir="/afs/cern.ch/user/k/kkaadze/public/ForJose/LimitScripts/UncSetup/mt"
export eTauUncsDir="/afs/cern.ch/user/k/kkaadze/public/ForJose/LimitScripts/UncSetup/et"

for d in 0 1 2 3 5; do
for i in 110 115 120 125 130 135 140 145; do
echo SM$d\_$i
create-datacard.py -i muTauSM.root -o muTau_$d\_$i.txt -c ${muTauUncsDir}/cgs-sm-7TeV-0$d.conf -u ${muTauUncsDir}/unc-sm-7TeV-0$d.vals -d ${muTauUncsDir}/unc-sm-7TeV-0$d.conf $i
done
done
