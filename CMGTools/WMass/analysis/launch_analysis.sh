#!/bin/bash

  jetPtCut=20

  muonSelector=muonV2NOTMOSTPt4jetPFJetPt${jetPtCut}
  dataSample=DoubleMu__Run2011A-May10ReReco-v1__DoubleMuTriggers-AllSelections_skimmed
  MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3_V3_4_all_TrackJetsL2L3_skimmed

  echo "muonSelector = "${muonSelector}
  echo "dataSample = "${dataSample}
  echo "MCSample = "${MCSample}

  # echo "waiting 30 minutes before to launch..."
# sleep 1800
  echo "launching runMuonPtRelTemplatesProducer..."
  screen -d -m -h 1000 -S templprod1
  screen -S "templprod1" -p 0 -X stuff "source /lustre/cmswork/mgalanti/root/v5.28.00c/bin/thisroot.sh; cd ../MuonPtRelTemplatesProducer; root -b -q 'runMuonPtRelTemplatesProducer.cc(\"${MCSample}\",\"${muonSelector}\",-1)'; cp MuonPtRelTemplatesProducer__Histograms__${MCSample}__${muonSelector}.root ../../MuonPtRelBFractionAnalysis/root_files/RawTemplates__${MCSample}__${muonSelector}.root; mv MuonPtRelTemplatesProducer__Histograms__${MCSample}__${muonSelector}.root ../../MuonPtRelBFractionAnalysis/root_files/RawFakeDistributions__${MCSample}__${muonSelector}.root; $(printf \\r)" 
  echo "runMuonPtRelTemplatesProducer launched!"

  
  echo "launching runMuonPtRelDistributionsProducer for MC... (waiting 20 sec before to launch it)"
  sleep 20
  screen -d -m -h 1000 -S distrprodMC1
  screen -S "distrprodMC1" -p 0 -X stuff "source /lustre/cmswork/mgalanti/root/v5.28.00c/bin/thisroot.sh; cd ../MuonPtRelDistributionsProducer; root -b -q 'runMuonPtRelDistributionsProducer.cc(\"${MCSample}\",\"${muonSelector}\",-1)'; mv MuonPtRelDistributionsProducer__Histograms__${MCSample}__${muonSelector}.root ../../MuonPtRelBFractionAnalysis/root_files/RawDistributions__${MCSample}__${muonSelector}.root; $(printf \\r)" 
  echo "runMuonPtRelDistributionsProducer for MC launched!"

