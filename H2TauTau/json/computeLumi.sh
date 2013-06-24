
#alias lumiCalcVersion="lumiCalc2.py"
#alias lumiCalcVersion="pixelLumiCalc.py"

#which lumiCalcVersion

###-------2011-----------------------------
#export JsonMay10="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt"
#export Jsonv4="./PromtReco_JSON_PRv4.txt"
#export JsonAug5="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt"
#export JsonOct3="./PromtReco_JSON_PROct3.txt"
#export Json2011B="./PromtReco_JSON_PR2011B.txt"
#
##echo $JsonMay10
##lumiCalcVersion -i $JsonMay10 recorded --hltpath HLT_IsoMu12_LooseIsoPFTau10_v4 | grep HLT_IsoMu12_LooseIsoPFTau10_v4
#
#echo $Jsonv4
#lumiCalcVersion -i $Jsonv4 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v2 | grep HLT_IsoMu15_LooseIsoPFTau15_v2
#lumiCalcVersion -i $Jsonv4 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v4 | grep HLT_IsoMu15_LooseIsoPFTau15_v4
#lumiCalcVersion -i $Jsonv4 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v5 | grep HLT_IsoMu15_LooseIsoPFTau15_v5
#lumiCalcVersion -i $Jsonv4 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v6 | grep HLT_IsoMu15_LooseIsoPFTau15_v6
#
##echo $JsonAug5
##lumiCalcVersion -i $JsonAug5 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v8 | grep HLT_IsoMu15_LooseIsoPFTau15_v8
#
#echo $JsonOct3
#lumiCalcVersion -i $JsonOct3 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v8 | grep HLT_IsoMu15_LooseIsoPFTau15_v8
#lumiCalcVersion -i $JsonOct3 recorded --hltpath  HLT_IsoMu15_LooseIsoPFTau15_v9 | grep HLT_IsoMu15_LooseIsoPFTau15_v9
#
#echo $Json2011B
#lumiCalcVersion -i $Json2011B recorded --hltpath  HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1 | grep HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1
#lumiCalcVersion -i $Json2011B recorded --hltpath  HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v5
#lumiCalcVersion -i $Json2011B recorded --hltpath  HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v6

###-------2012-----------------------------

########mu-tau
#echo 2012A
#export Json2012="./Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6

#echo 2012Arecover
#export Json2012="./Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v3
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6

#echo 2012B
#export Json2012="./Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2
#
#echo 2012Cv1
#export Json2012="./Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3
#
#echo 2012Cv2
#export Json2012="./Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON_v2_For2012Cv2Lumi.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7

#echo 2012D
##export Json2012="./Cert_190456-206539_8TeV_PromptReco_Collisions12_JSON_for2012DLumi.txt"
#export Json2012="./Cert_190456-207898_8TeV_PromptReco_Collisions12_JSON_for2012DLumi.txt"
#export Json2012="./Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON_for2012DLumi.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7


######e-tau#######################
###2012A
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4 | grep HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4 
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5 | grep HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5 
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6 | grep HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6 
###2012B
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2 | grep  HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2


###Data Pile-Up distributions
###pileup for 2012D : 6/fb
#pileupCalc.py -i Cert_190456-207898_8TeV_PromptReco_Collisions12_JSON_for2012DLumi.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 60 --numPileupBins 60  MyDataPileupHistogram.root

##pile-up for 2012ABCD: full 2012D 
#pileupCalc.py -i Cert_8TeV_PromptReco_Collisions12_JSON_for2012ABCDPileUp.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 60 --numPileupBins 60  Pileup_2012ABCD.true.root

##pile-up for 2012BCD (for Will)
#pileupCalc.py -i Cert_PromptReco_Collisions12_JSON_for2012BCDPileUp.txt --inputLumiJSON /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_latest.txt --calcMode true --minBiasXsec 69400 --maxPileupBin 60 --numPileupBins 60  Pileup_2012BCD.true.root


######################2012 ReReco#######################
alias lumiCalcVersion="lumiCalc2.py"
export jsondir=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing
export Json2012=$jsondir/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt
echo $Json2012
#######mu-tau
echo 2012A
export range="--begin 190450 --end 193686"
echo $range
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6

echo 2012B
export range="--begin 193752 --end 197044"
echo $range
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2

echo 2012C
export range="--begin 197770 --end 203755"
echo $range
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7

echo 2012D
export range="--begin 203773 --end 208686"
echo $range
lumiCalcVersion -i $Json2012 $range recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7
