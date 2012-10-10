import CMGTools.RootTools.fwlite.Config as cfg
import copy


data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 |        833 | 28.303(/pb) | 28.303(/pb) |
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 |       5606 | 302.772(/pb) | 299.191(/pb) |
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 |       6858 | 377.239(/pb) | 371.436(/pb) |
    intLumi = 28.303 + 299.191 + 371.436, 
    triggers = [],
    json = None
    )

data_Run2012A_aug6 = cfg.DataComponent(
    name = 'data_Run2012A_aug6',
    files = [],
    # | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 |       1810 | 82.136(/pb) | 82.136(/pb) |
    intLumi = 82.136, 
    triggers = [],
    json = None
    )

data_Run2012B = cfg.DataComponent(
    name = 'data_Run2012B',
    files = [],
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 |      50799 | 4.389(/fb) | 4.389(/fb) |
    intLumi = 4389., #pixellumicalc 4389
    triggers = [],
    json = None
    )

data_Run2012C_v1 = cfg.DataComponent(
    name = 'data_Run2012C_v1',
    files = [],
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |       6719 | 495.003(/pb) | 495.003(/pb) |
    intLumi = 495.003,
    triggers = [],
    json = None
    )

data_Run2012C_v2 = cfg.DataComponent(
    name = 'data_Run2012C_v2',
    files = [],
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |      17991 | 1.464(/fb) | 1.464(/fb) |
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 |      46975 | 4.358(/fb) | 4.358(/fb) |
    # | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 |       3425 | 281.155(/pb) | 281.155(/pb) |
    intLumi = 1464+4358+281.155,
    triggers = [],
    json = None
    )
#  lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 missing! 
# running nohup in  /afs/cern.ch/work/c/cbern/ProductionSoftware/5X/V5_8_0/CMSSW_5_3_3_patch3/src

data_list = [
    data_Run2012A,
    data_Run2012A_aug6,
    data_Run2012B,
    data_Run2012C_v1,
    data_Run2012C_v2,
    ]


# from Jose:
##   TauPlusX2012A->setSampleLumi(96.977+316.128+396.213);
##   TauPlusX2012A2->setSampleLumi(82.136);
##   TauPlusX2012B->setSampleLumi(4403.);
##   TauPlusX2012Cv1->setSampleLumi(495.003);
##   TauPlusX2012Cv2->setSampleLumi(1288.0 + 4751.0 + 272.040);
#
# how to measure it:
#
# alias lumiCalcVersion="lumiCalc2.py"
#######mu-tau
#echo 2012A
#export Json2012="./Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
#lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6
#
#echo 2012Arecover
#export Json2012="./Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
# lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v3 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v3
# lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4
# lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5
# lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6
#
#echo 2012B
#export Json2012="./Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON.txt"
#echo $Json2012
# lumiCalcVersion -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2
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
