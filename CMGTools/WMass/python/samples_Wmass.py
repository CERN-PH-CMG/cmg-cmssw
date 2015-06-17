from CMGTools.TTHAnalysis.samples.getFiles import getFiles
import CMGTools.RootTools.fwlite.Config as cfg
import os
from copy import copy

#####COMPONENT CREATOR

from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

#-----------MC---------------

DYJets = kreator.makePrivateMCComponent('DYJets','/DYJetsToLL_M-50_7TeV-madgraph-pythia6-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root',2895.6)

# DYJets1.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets1 = kreator.makePrivateMCComponent('DYJets1','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights', '.*root',950) # 790   

# DYJets2.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets2 = kreator.makePrivateMCComponent('DYJets2','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights', '.*root',950) # 854

# DYJets3.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets3 = kreator.makePrivateMCComponent('DYJets3','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights', '.*root',950)# 900

DYJets4 = kreator.makePrivateMCComponent('DYJets4','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v3/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights', '.*root',950) # 925   

#-------- WJets

# 
WJets = kreator.makePrivateMCComponent('WJets1','/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 31314)

WJetsPlus1 = kreator.makePrivateMCComponent('WPlus1','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5913) # 1389
WJetsPlus2 = kreator.makePrivateMCComponent('WPlus2','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v3/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5913) # 1703
WJetsPlus3 = kreator.makePrivateMCComponent('WPlus3','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v7/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5913) # 1384
WJetsPlus4 = kreator.makePrivateMCComponent('WPlus4','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v4/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5913) # 1598
WJetsPlus5 = kreator.makePrivateMCComponent('WPlus5','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v6/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5913) # 678
WJetsPlus6 = kreator.makePrivateMCComponent('WPlus6','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v5/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5913) # 635


WJetsMinus1 = kreator.makePrivateMCComponent('WMinus1','/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root',4126) # 2106
WJetsMinus2 = kreator.makePrivateMCComponent('WMinus2','/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root',4126) # 2106
WJetsMinus3 = kreator.makePrivateMCComponent('WMinus3','/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pytha8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root',4126) # 1890

#-------- TTbar

TTJets = kreator.makePrivateMCComponent('TTJets','/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root',165)
WW = kreator.makePrivateMCComponent('WW','/WW_TuneZ2_7TeV_pythia6_tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 43)
WZ = kreator.makePrivateMCComponent('WZ','/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 18.2)
ZZ = kreator.makePrivateMCComponent('ZZ','/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', '.*root', 5.9 )

mcSamples7TeV = [DYJets, DYJets1,DYJets2,DYJets3,DYJets4] + [WJets, WJetsPlus1, WJetsPlus2, WJetsPlus3, WJetsPlus4, WJetsPlus5, WJetsPlus6, WJetsMinus1, WJetsMinus2, WJetsMinus3 ] + [TTJets, WW, WZ, ZZ]

#-------- 8TeV

WJets_8 = kreator.makePrivateMCComponent('WJets1_8TeV','/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_18_0_NewTauID', '.*root', 37509)
DYJets_8 = kreator.makePrivateMCComponent('DYJets_8TeV','/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_NewTKmet', '.*root', 3532.8)
TTJets_8 = kreator.makePrivateMCComponent('TTJets_8TeV','/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0','.*root', 234)

mcSamples8TeV = [WJets_8,DYJets_8,TTJets_8]

################## Triggers
from CMGTools.WMass.triggerMap import *

#----------- COMMON SETTINGS ---------------

for comp in mcSamples7TeV + mcSamples8TeV:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 1000
    comp.triggers = triggers_mu

