from CMGTools.TTHAnalysis.samples.getFiles import getFiles

import CMGTools.RootTools.fwlite.Config as cfg
import os
from copy import copy

##### COMPONENT CREATOR
from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

##### Triggers
from CMGTools.WMass.triggerMap import *

#-----------MC---------------

DYJets = kreator.makeMCComponentFromEOS('DYJets','/DYJetsToLL_M-50_7TeV-madgraph-pythia6-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0/','/store/cmst3/user/cmgtools/CMG/%s','.*root',2895.6)

# DYJets1.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets1 = kreator.makeMCComponentFromEOS('DYJets1','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights/','/store/cmst3/group/wmass/CMG/%s','.*root',950) # 790   

# DYJets2.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets2 = kreator.makeMCComponentFromEOS('DYJets2','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights/','/store/cmst3/group/wmass/CMG/%s','.*root',950) # 854

# DYJets3.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets3 = kreator.makeMCComponentFromEOS('DYJets3','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights/','/store/cmst3/group/wmass/CMG/%s','.*root',950)# 900

DYJets4 = kreator.makeMCComponentFromEOS('DYJets4','/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v3/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights/','/store/cmst3/group/wmass/CMG/%s','.*root',950) # 925   

#-------- WJets

WJets = kreator.makeMCComponentFromEOS('WJets1','/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 31314)

WJetsPlus1 = kreator.makeMCComponentFromEOS('WPlus1','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5913) # 1389
WJetsPlus2 = kreator.makeMCComponentFromEOS('WPlus2','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v3/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5913) # 1703
WJetsPlus3 = kreator.makeMCComponentFromEOS('WPlus3','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v7/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5913) # 1384
WJetsPlus4 = kreator.makeMCComponentFromEOS('WPlus4','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v4/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5913) # 1598
WJetsPlus5 = kreator.makeMCComponentFromEOS('WPlus5','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v6/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5913) # 678
WJetsPlus6 = kreator.makeMCComponentFromEOS('WPlus6','/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v5/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5913) # 635

WJetsMinus1 = kreator.makeMCComponentFromEOS('WMinus1','/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root',4126) # 2106
WJetsMinus2 = kreator.makeMCComponentFromEOS('WMinus2','/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root',4126) # 2106
WJetsMinus3 = kreator.makeMCComponentFromEOS('WMinus3','/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pytha8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root',4126) # 1890

#-------- TTbar

TTJets = kreator.makeMCComponentFromEOS('TTJets','/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root',165)
WW = kreator.makeMCComponentFromEOS('WW','/WW_TuneZ2_7TeV_pythia6_tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 43)
WZ = kreator.makeMCComponentFromEOS('WZ','/WZ_TuneZ2_7TeV_pythia6_tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 18.2)
ZZ = kreator.makeMCComponentFromEOS('ZZ','/ZZ_TuneZ2_7TeV_pythia6_tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 5.9 )

mcSamples7TeV = [DYJets, DYJets1,DYJets2,DYJets3,DYJets4] + [WJets, WJetsPlus1, WJetsPlus2, WJetsPlus3, WJetsPlus4, WJetsPlus5, WJetsPlus6, WJetsMinus1, WJetsMinus2, WJetsMinus3 ] + [TTJets, WW, WZ, ZZ]

#-------- 8TeV

WJets_8 = kreator.makeMCComponentFromEOS('WJets_8TeV','/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_18_0_NewTauID','/store/cmst3/user/cmgtools/CMG/%s','.*root', 37509)
DYJets_8 = kreator.makeMCComponentFromEOS('DYJets_8TeV','/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_NewTKmet','/store/cmst3/group/wmass/CMG/%s','.*root', 3532.8)
TTJets_8 = kreator.makeMCComponentFromEOS('TTJets_8TeV','/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0','/store/cmst3/user/cmgtools/CMG/%s','.*root', 234)

mcSamples8TeV = [WJets_8,DYJets_8,TTJets_8]



#-------- DATA - 7TeV

data_Run2011A_12Oct2013_v1 = cfg.DataComponent(
    name = 'data_Run2011A_12Oct2013_v1',
    files = getFiles('/SingleMu/Run2011A-12Oct2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    # files = ['file:/afs/cern.ch/work/p/perrozzi/private/git/v5_18_0/CMSSW_5_3_14/src/CMGTools/WMass/cfg/test/cmgTuple.root'],
    intLumi =  4749.90,
    triggers = triggers_mu,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
    )

data_Run2011B_12Oct2013_v1 = cfg.DataComponent(
    name = 'data_Run2011B_12Oct2013_v1',
    files = getFiles('/SingleMu/Run2011B-12Oct2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = triggers_mu,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
    )

data_Run2011AB_12Oct2013_v1 = cfg.DataComponent(
    name = 'data_Run2011AB_12Oct2013_v1',
    files =  [],
    intLumi =  4749.90,
triggers = triggers_mu,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON_v2.txt'
    )

data_Run2011AB_12Oct2013_v1.files = data_Run2011A_12Oct2013_v1.files + data_Run2011B_12Oct2013_v1.files

## selectedComponents = [data_Run2011A_12Oct2013_v1,data_Run2011B_12Oct2013_v1]
#data_Run2011A_12Oct2013_v1.splitFactor = 1000
#data_Run2011B_12Oct2013_v1.splitFactor = 1000
#data_Run2011AB_12Oct2013_v1.splitFactor = 2000
#selectedComponents = [data_Run2011A_12Oct2013_v1,data_Run2011B_12Oct2013_v1]


#----------- COMMON SETTINGS ---------------

for comp in mcSamples7TeV + mcSamples8TeV:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 1000
    comp.triggers = triggers_mu

