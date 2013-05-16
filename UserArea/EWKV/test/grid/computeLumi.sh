#/bin/bash

#jsons=(DoubleElectron2012A DoubleElectron2012B DoubleElectron2012C Data8TeV_DoubleElectron2012D)
#minbias=(69000 71000)

jsons=(
    DoubleElectron2012A DoubleElectron2012B DoubleElectron2012C DoubleElectron2012D
    DoubleMu2012A DoubleMu2012B DoubleMu2012C DoubleMu2012D
    MuEG2012A MuEG2012B MuEG2012C MuEG2012D
)
minbias=(70300)

PUJSON=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_latest.txt

for j in ${jsons[@]}; do 
    lumiCalc2.py overview -i Data8TeV_${j}.lumi > Data8TeV_${j}.hf-based.lumi
    pixelLumiCalc.py overview -i Data8TeV_${j}.lumi > Data8TeV_${j}.px-based.lumi
    for mb in ${minbias[@]}; do
	pileupCalc.py -i Data8TeV_${j}.lumi --inputLumiJSON ${PUJSON} --calcMode observed --minBiasXsec ${mb} --maxPileupBin 100 --numPileupBins 100  Data8TeV_${j}_targetpu_${mb}.root &
    done
done