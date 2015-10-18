import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

### ----------------------------- 25 ns ----------------------------------------
# TTbar cross section: NNLO, https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO (172.5)

TTJets_LO = kreator.makeMCComponent("TTJets_LO", "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 831.76)

TTJets_SingleLeptonFromTbar = kreator.makeMCComponent("TTJets_SingleLeptonFromTbar", "/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 831.76*(3*0.108) )
TTJets_DiLepton = kreator.makeMCComponent("TTJets_DiLepton", "/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 831.76*((3*0.108)**2) )

TTLep_pow = kreator.makeMCComponent("TTLep_pow", "/TTTo2L2Nu_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 831.76*((3*0.108)**2) )

TTs = [ TTJets_LO , TTJets_SingleLeptonFromTbar, TTJets_DiLepton, TTLep_pow ]


# Single top cross sections: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SingleTopSigma

TToLeptons_tch_amcatnlo = kreator.makeMCComponent("TToLeptons_tch_amcatnlo", "/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM", "CMS", ".*root", (136.05+80.97)*0.108*3) 

TToLeptons_sch_amcatnlo = kreator.makeMCComponent("TToLeptons_sch", "/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", (7.20+4.16)*0.108*3)
TBar_tWch = kreator.makeMCComponent("TBar_tWch", "/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",35.6)   
T_tWch = kreator.makeMCComponent("T_tWch", "/ST_tW_top_5f_DS_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",35.6)

tZq_ll = kreator.makeMCComponent("tZq_ll", "/tZq_ll_4f_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 0.0758 )
tZq_nunu = kreator.makeMCComponent("tZq_nunu", "/tZq_nunu_4f_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 0.1379 )

SingleTop = [
    TToLeptons_tch_amcatnlo, TToLeptons_sch_amcatnlo, TBar_tWch, T_tWch, tZq_ll, tZq_nunu
]

### V+jets inclusive (from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV)
WJetsToLNu = kreator.makeMCComponent("WJetsToLNu","/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 3* 20508.9)

DYJetsToLL_M10to50 = kreator.makeMCComponent("DYJetsToLL_M10to50", "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 18610)
DYJetsToLL_M50 = kreator.makeMCComponent("DYJetsToLL_M50", "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 2008.*3)

DYJetsToLL_M50_LO =  kreator.makeMCComponent("DYJetsToLL_M50_LO", "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 2008.*3)

VJets = [ WJetsToLNu, DYJetsToLL_M10to50, DYJetsToLL_M50]

# DY HT bins:
DYJetsToLL_M50_HT100to200 = kreator.makeMCComponent("DYJetsToLL_M50_HT100to200", "/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",139.4*1.27)
DYJetsToLL_M50_HT200to400 = kreator.makeMCComponent("DYJetsToLL_M50_HT200to400", "/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",42.75*1.27)

DYJetsToLL_M50_HT600toInf = kreator.makeMCComponent("DYJetsToLL_M50_HT600toInf", "/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",2.21*1.27)

DYJetsM50HT = [
DYJetsToLL_M50_HT100to200,
DYJetsToLL_M50_HT200to400,
#DYJetsToLL_M50_HT400to600,
DYJetsToLL_M50_HT600toInf,
]

### W+jets
WJetsToLNu_HT100to200 = kreator.makeMCComponent("WJetsToLNu_HT100to200", "/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",1347*1.23)
WJetsToLNu_HT200to400 = kreator.makeMCComponent("WJetsToLNu_HT200to400", "/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",360*1.23)
WJetsToLNu_HT400to600 = kreator.makeMCComponent("WJetsToLNu_HT400to600", "/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",48.9*1.23)
WJetsToLNu_HT600toInf = kreator.makeMCComponent("WJetsToLNu_HT600toInf", "/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",18.77*1.23)
WJetsToLNu_HT600to800 = kreator.makeMCComponent("WJetsToLNu_HT600to800", "/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",12.8*1.23)

WJetsToLNu_HT1200to2500 = kreator.makeMCComponent("WJetsToLNu_HT1200to2500", "/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",1.33*1.23)
WJetsToLNu_HT2500toInf = kreator.makeMCComponent("WJetsToLNu_HT2500toInf", "/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",0.03089*1.23)

WJetsToLNuHT = [
WJetsToLNu_HT100to200,
WJetsToLNu_HT200to400,
WJetsToLNu_HT400to600,
WJetsToLNu_HT600toInf,
WJetsToLNu_HT600to800,
#WJetsToLNu_HT800to1200,
WJetsToLNu_HT1200to2500,
WJetsToLNu_HT2500toInf
]


### GJets (cross sections from McM)
GJets_HT40to100 = kreator.makeMCComponent("GJets_HT40to100", "/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",23080)
GJets_HT100to200 = kreator.makeMCComponent("GJets_HT100to200", "/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",9110)
GJets_HT200to400 = kreator.makeMCComponent("GJets_HT200to400", "/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",2298) # from Adish Vartak, since value in McM is 10xReasonable
GJets_HT400to600 = kreator.makeMCComponent("GJets_HT400to600", "/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",273.)
GJets_HT600toInf = kreator.makeMCComponent("GJets_HT600toInf", "/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",94.5)

GJetsHT = [
GJets_HT40to100,
GJets_HT100to200,
GJets_HT200to400,
GJets_HT400to600,
GJets_HT600toInf
]


### Zinv
ZJetsToNuNu_HT100to200 = kreator.makeMCComponent("ZJetsToNuNu_HT100to200", "/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",280.47*1.27)
ZJetsToNuNuHT = [
ZJetsToNuNu_HT100to200,
#ZJetsToNuNu_HT200to400,
#ZJetsToNuNu_HT400to600,
#ZJetsToNuNu_HT600toInf,
]

### QCD

## marc: i had to comment this section, but i really don't understand why (except for the first one for which i don't find the x-sec
## missing xsec QCD_Pt5to10      = kreator.makeMCComponent("QCD_Pt5to10"      , "/QCD_Pt_5to10_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"      , "CMS" , "*.root" , )
#QCD_Pt10to15     = kreator.makeMCComponent("QCD_Pt10to15"     , "/QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"     , "CMS" , "*.root" , 5887580000)
#QCD_Pt15to30     = kreator.makeMCComponent("QCD_Pt15to30"     , "/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"     , "CMS" , "*.root" , 1837410000)
#QCD_Pt30to50     = kreator.makeMCComponent("QCD_Pt30to50"     , "/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"     , "CMS" , "*.root" , 140932000)


#QCD_Pt120to170   = kreator.makeMCComponent("QCD_Pt120to170"   , "/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"   , "CMS" , "*.root" , 471100)
#QCD_Pt170to300   = kreator.makeMCComponent("QCD_Pt170to300"   , "/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"   , "CMS" , "*.root" , 117276)
#
#
#QCD_Pt600to800   = kreator.makeMCComponent("QCD_Pt600to800"   , "/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"   , "CMS" , "*.root" , 186.9)
#QCD_Pt800to1000  = kreator.makeMCComponent("QCD_Pt800to1000"  , "/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS" , "*.root" , 32.293)
#QCD_Pt1000to1400 = kreator.makeMCComponent("QCD_Pt1000to1400" , "/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM" , "CMS" , "*.root" , 9.4183)
#QCD_Pt1400to1800 = kreator.makeMCComponent("QCD_Pt1400to1800" , "/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM" , "CMS" , "*.root" , 0.84265)
#QCD_Pt1800to2400 = kreator.makeMCComponent("QCD_Pt1800to2400" , "/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM" , "CMS" , "*.root" , 0.114943)
#QCD_Pt2400to3200 = kreator.makeMCComponent("QCD_Pt2400to3200" , "/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM" , "CMS" , "*.root" , 0.00682981)
#QCD_Pt3200toInf  = kreator.makeMCComponent("QCD_Pt3200"       , "/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS" , "*.root" , 0.000165445)
#
#QCDPt = [
#QCD_Pt10to15,
#QCD_Pt15to30,
#QCD_Pt30to50,
##QCD_Pt50to80,
##QCD_Pt80to120,
#QCD_Pt120to170,
#QCD_Pt170to300,
##QCD_Pt300to470,
##QCD_Pt470to600,
#QCD_Pt600to800,
#QCD_Pt800to1000,
#QCD_Pt1000to1400,
#QCD_Pt1400to1800,
#QCD_Pt1800to2400,
#QCD_Pt2400to3200,
#QCD_Pt3200toInf
#]

# qcd muenr
QCD_Pt15to20_Mu5    = kreator.makeMCComponent("QCD_Pt15to20_Mu5"    , "/QCD_Pt-15to20_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"       , "CMS" , ".*root" , 1273190000*0.003)
QCD_Pt20to30_Mu5    = kreator.makeMCComponent("QCD_Pt20to30_Mu5"    , "/QCD_Pt-20to30_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"      , "CMS" , ".*root" , 558528000*0.0053)

QCD_Pt50to80_Mu5    = kreator.makeMCComponent("QCD_Pt50to80_Mu5"    , "/QCD_Pt-50to80_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM" , "CMS" , ".*root" , 19222500*0.02276)
QCD_Pt80to120_Mu5   = kreator.makeMCComponent("QCD_Pt80to120_Mu5"   , "/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"     , "CMS" , ".*root" , 2758420*0.03844)
QCD_Pt120to170_Mu5  = kreator.makeMCComponent("QCD_Pt120to170_Mu5"  , "/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM", "CMS" , ".*root" , 469797*0.05362)

QCD_Pt300to470_Mu5  = kreator.makeMCComponent("QCD_Pt300to470_Mu5"  , "/QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"    , "CMS" , ".*root" , 7820.25*0.10196)
QCD_Pt470to600_Mu5  = kreator.makeMCComponent("QCD_Pt470to600_Mu5"  , "/QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"    , "CMS" , ".*root" , 645.528*0.12242)

QCD_Pt800to1000_Mu5 = kreator.makeMCComponent("QCD_Pt800to1000_Mu5" , "/QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"   , "CMS" , ".*root" , 32.3486*0.14552)
QCD_Pt1000toInf_Mu5 = kreator.makeMCComponent("QCD_Pt1000toInf_Mu5" , "/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"   , "CMS" , ".*root" , 10.4305*0.15544)

QCD_Mu5 = [
QCD_Pt15to20_Mu5,
QCD_Pt20to30_Mu5,
QCD_Pt50to80_Mu5,
QCD_Pt80to120_Mu5,
QCD_Pt120to170_Mu5,
QCD_Pt300to470_Mu5,
QCD_Pt470to600_Mu5,
QCD_Pt800to1000_Mu5,
QCD_Pt1000toInf_Mu5
]

# qcd emenr
QCD_Pt20to30_EMEnriched   = kreator.makeMCComponent("QCD_Pt20to30_EMEnriched"  ,"/QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS", ".*root", 557600000*0.0096)
QCD_Pt30to50_EMEnriched   = kreator.makeMCComponent("QCD_Pt30to50_EMEnriched"  ,"/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS", ".*root", 136000000*0.073)

QCD_Pt80to120_EMEnriched  = kreator.makeMCComponent("QCD_Pt80to120_EMEnriched" ,"/QCD_Pt-80to120_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM" , "CMS", ".*root", 2800000*0.125)
QCD_Pt120to170_EMEnriched = kreator.makeMCComponent("QCD_Pt120to170_EMEnriched","/QCD_Pt-120to170_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 477000*0.132)

QCDPtEMEnriched = [
#QCD_Pt15to20_EMEnriched,
QCD_Pt20to30_EMEnriched,
QCD_Pt30to50_EMEnriched,
#QCD_Pt50to80_EMEnriched,
QCD_Pt80to120_EMEnriched,
QCD_Pt120to170_EMEnriched,
#QCD_Pt170to300_EMEnriched,
#QCD_Pt300toInf_EMEnriched
]

# qcd bctoe

QCD_Pt_15to20_bcToE   = kreator.makeMCComponent("QCD_Pt_15to20_bcToE"   , "/QCD_Pt_15to20_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS" , ".*root" , 1272980000*0.0002)
QCD_Pt_20to30_bcToE   = kreator.makeMCComponent("QCD_Pt_20to30_bcToE"   , "/QCD_Pt_20to30_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS" , ".*root" , 557627000*0.00059)
QCD_Pt_30to80_bcToE   = kreator.makeMCComponent("QCD_Pt_30to80_bcToE"   , "/QCD_Pt_30to80_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS" , ".*root" , 159068000*0.00255)
QCD_Pt_80to170_bcToE  = kreator.makeMCComponent("QCD_Pt_80to170_bcToE"  , "/QCD_Pt_80to170_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM" , "CMS" , ".*root" , 3221000*0.01183)

QCD_Pt_250toInf_bcToE = kreator.makeMCComponent("QCD_Pt_250toInf_bcToE" , "/QCD_Pt_250toInf_bcToE_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS" , ".*root" , 21094.1*0.03375)

QCDPtbcToE = [
QCD_Pt_15to20_bcToE,
QCD_Pt_20to30_bcToE,
QCD_Pt_30to80_bcToE,
QCD_Pt_80to170_bcToE,
#QCD_Pt_170to250_bcToE,
QCD_Pt_250toInf_bcToE
]



# QCD HT bins (cross sections from McM)
## need xsec QCD_HT100to200 = kreator.makeMCComponent("QCD_HT100to200", "/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", "*.root",    1234214213421342314123432.)
QCD_HT200to300 = kreator.makeMCComponent("QCD_HT200to300", "/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",1735000)
QCD_HT300to500 = kreator.makeMCComponent("QCD_HT300to500", "/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",367000)
QCD_HT500to700 = kreator.makeMCComponent("QCD_HT500to700", "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",29400)
QCD_HT700to1000 = kreator.makeMCComponent("QCD_HT700to1000", "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",6524)
QCD_HT1000to1500 = kreator.makeMCComponent("QCD_HT1000to1500", "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",1064)
QCD_HT1500to2000 = kreator.makeMCComponent("QCD_HT1500to2000", "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",121.5)
QCD_HT2000toInf = kreator.makeMCComponent("QCD_HT2000toInf", "/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",25.42)  

QCDHT = [
QCD_HT200to300,
QCD_HT300to500,
QCD_HT500to700,
QCD_HT700to1000,
QCD_HT1000to1500,
QCD_HT1500to2000,
QCD_HT2000toInf
]

###DiBosons

# cross section from https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns#Diboson
WWTo2L2Nu = kreator.makeMCComponent("WWTo2L2Nu", "/WWTo2L2Nu_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 10.481 )
WWToLNuQQ = kreator.makeMCComponent("WWToLNuQQ", "/WWToLNuQQ_13TeV-powheg/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 43.53 ) 

ZZTo2L2Q = kreator.makeMCComponent("ZZTo2L2Q", "/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  3.28)
ZZTo2Q2Nu = kreator.makeMCComponent("ZZTo2Q2Nu", "/ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  4.04)

WZTo1L3Nu   = kreator.makeMCComponent("WZTo1L3Nu"  , "/WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"  , "CMS", ".*root", (47.13)*(3*0.108)*(0.2) )
WZTo1L1Nu2Q = kreator.makeMCComponent("WZTo1L1Nu2Q", "/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  10.71)
WZTo2L2Q    = kreator.makeMCComponent("WZTo2L2Q"   , "/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM"   , "CMS", ".*root",  5.60)

VVTo2L2Nu = kreator.makeMCComponent("VVTo2L2Nu","/VVTo2L2Nu_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  12.05)

DiBosons = [ WWTo2L2Nu, WWToLNuQQ, ZZTo2L2Q, ZZTo2Q2Nu, WZTo1L1Nu2Q, WZTo2L2Q, VVTo2L2Nu ] 

###TriBosons
# cross section from https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns#Triboson
WWZ = kreator.makeMCComponent("WWZ", "/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  0.1651 )
WZZ = kreator.makeMCComponent("WZZ", "/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  0.05565 )
ZZZ = kreator.makeMCComponent("ZZZ", "/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root",  0.01398 )

TriBosons = [ WWZ, WZZ, ZZZ ]

###TTV
TTWToLNu = kreator.makeMCComponent("TTWToLNu", "/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 0.2043)
TTWToQQ = kreator.makeMCComponent("TTWToQQ", "/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 0.40620)
TTZToQQ = kreator.makeMCComponent("TTZToQQ","/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 0.5297)

TTGJets = kreator.makeMCComponent("TTGJets","/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 3.697)

TTV = [TTWToLNu, TTWToQQ, TTZToQQ, TTGJets]

### ----------------------------- summary ----------------------------------------


mcSamples_Asymptotic25ns = TTs + SingleTop + VJets + DYJetsM50HT + WJetsToLNuHT + GJetsHT + QCDHT + DiBosons + TriBosons + TTV

mcSamples = mcSamples_Asymptotic25ns

samples = mcSamples

### ---------------------------------------------------------------------

from CMGTools.TTHAnalysis.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"

#Define splitting
for comp in mcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250 #  if comp.name in [ "WJets", "DY3JetsM50", "DY4JetsM50","W1Jets","W2Jets","W3Jets","W4Jets","TTJetsHad" ] else 100
    comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
    comp.puFileData=dataDir+"/puProfile_Data12.root"
    comp.efficiency = eff2012

if __name__ == "__main__":
   import sys
   if "test" in sys.argv:
       from CMGTools.RootTools.samples.ComponentCreator import testSamples
       testSamples(samples)
