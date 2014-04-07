import CMGTools.RootTools.fwlite.Config as cfg
import copy


data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    # intLumi = 96.404 + 396.906 + 395.991, # old lumi
    intLumi = 94.676 + 390.113 + 391.376, # pixel lumi
    triggers = [],
    json = None
    )
data_Run2012B = cfg.DataComponent(
    name = 'data_Run2012B',
    files = [],
    # intLumi = 4429., # old lumi
    intLumi = 4411., # pixel lumi
    triggers = [],
    json = None
    )


data_Run2012C = cfg.DataComponent(
    name = 'data_Run2012C',
    files = [],
    # intLumi = 1783. + 5087. + 282.692, # old lumi
    intLumi = 1734. + 5041. + 279.843, # pixel lumi
    triggers = [],
    json = None
    )

data_Run2012D = cfg.DataComponent(
    name = 'data_Run2012D',
    files = [],
    # intLumi = 7318., # old lumi
    intLumi = 7369., # pixel lumi
    triggers = [],
    )

data_list = [
    data_Run2012A,
    data_Run2012B,
    data_Run2012C,
    data_Run2012D,
    ]

# Replace lumiCalc2.py by pixelLumiCalc.py below for pixel lumi 
#
# # 2012 D
# 
# lumiCalc2.py --begin 203777 --end 208687 -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_* | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 |      74410 | 7.318(/fb) | 7.318(/fb) |
# 
# # 2012 C
# 
# lumiCalc2.py --begin 198022 --end 203743 -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_* | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v3 |      22016 |   1.783(/fb) |   1.783(/fb) |
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v6 |      54989 |   5.087(/fb) |   5.087(/fb) |
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v7 |       3578 | 282.692(/pb) | 282.692(/pb) |
# 
# # 2012 B
# 
# lumiCalc2.py --begin 193833 --end 196532 -i $Json2012 recorded --hltpath HLT_IsoMu17_eta2p1_LooseIsoPFTau20_* | grep HLT_IsoMu17_eta2p1_LooseIsoPFTau20
# | HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v2 |      50904 | 4.429(/fb) | 4.429(/fb) |
# 
# # 2012 A
# 
# lumiCalc2.py --begin 190456 --end 193621 -i $Json2012 recorded --hltpath HLT_IsoMu18_eta2p1_LooseIsoPFTau20_* | grep HLT_IsoMu18_eta2p1_LooseIsoPFTau20
# | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v4 |       2803 |  96.404(/pb) |  96.404(/pb) |
# | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v5 |       7037 | 396.906(/pb) | 396.906(/pb) |
# | HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v6 |       6514 | 395.991(/pb) | 395.991(/pb) |
