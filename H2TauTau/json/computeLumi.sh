
alias lumiCalcVersion="lumiCalc2.py"
#alias lumiCalcVersion="pixelLumiCalc.py"

which lumiCalcVersion

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

#######mu-tau
#echo 2012A
export Json2012="./Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt"
echo $Json2012
lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6
#
#echo 2012Arecover
#export Json2012="./Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v3
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6
#
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
#export Json2012="./Cert_190456-203002_8TeV_PromptReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7






######e-tau#######################
###2012A
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4 | grep HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v4 
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5 | grep HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v5 
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6 | grep HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v6 
###2012B
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2 | grep  HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v2
