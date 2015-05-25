#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import * 

WAna.doMad = False

CoreWsequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    # ZAna,
    # ZtreeProducer,
   ] )
sequence = cfg.Sequence(CoreWsequence)

WAna.storeLHE_weight = True
WtreeProducer.storeLHE_weight = True
# # WtreeProducer.superslimNtuples = True

from CMGTools.H2TauTau.proto.samples.ewk import WJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# WJetsPlus.files = getFiles('/WJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# WJetsPlus.files = getFiles('/WJetsToLL_M-50_7TeV-madgraph-pythia6-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
WJetsPlus = copy.deepcopy(WJets)
WJetsPlus.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 1389
WJetsPlus.triggers = triggers_mu
WJetsPlus.splitFactor = 900
WJetsPlus.name = 'WPlus1'

WJetsPlus2 = copy.deepcopy(WJetsPlus)
WJetsPlus2.name = 'WPlus2'
WJetsPlus2.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v3/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 1703

WJetsPlus3 = copy.deepcopy(WJetsPlus)
WJetsPlus3.name = 'WPlus3'
WJetsPlus3.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v7/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 1384

WJetsPlus4 = copy.deepcopy(WJetsPlus)
WJetsPlus4.name = 'WPlus4'
WJetsPlus4.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v4/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 1598

WJetsPlus5 = copy.deepcopy(WJetsPlus)
WJetsPlus5.name = 'WPlus5'
WJetsPlus5.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v6/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 678

WJetsPlus6 = copy.deepcopy(WJetsPlus)
WJetsPlus6.name = 'WPlus6'
WJetsPlus6.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v5/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 635

WJetsPlus_p2 = copy.deepcopy(WJetsPlus)
WJetsPlus_p2.name = 'WPlus_p2'
WJetsPlus2_p2 = copy.deepcopy(WJetsPlus2)
WJetsPlus2_p2.name = 'WPlus2_p2'
WJetsPlus3_p2 = copy.deepcopy(WJetsPlus3)
WJetsPlus3_p2.name = 'WPlus3_p2'
WJetsPlus4_p2 = copy.deepcopy(WJetsPlus4)
WJetsPlus4_p2.name = 'WPlus4_p2'
# WJetsPlus5_p2 = copy.deepcopy(WJetsPlus5)
# WJetsPlus5_p2.name = 'WJetsPlus5_p2'
# WJetsPlus6_p2 = copy.deepcopy(WJetsPlus6)
# WJetsPlus6_p2.name = 'WJetsPlus6_p2'


WJetsPlus.files = WJetsPlus.files[700:]
WJetsPlus_p2.files = WJetsPlus_p2.files[:700]
WJetsPlus2.files = WJetsPlus2.files[800:]
WJetsPlus2_p2.files = WJetsPlus2_p2.files[:800]
WJetsPlus3.files = WJetsPlus3.files[700:]
WJetsPlus3_p2.files = WJetsPlus3_p2.files[:700]
WJetsPlus4.files = WJetsPlus4.files[800:]
WJetsPlus4_p2.files = WJetsPlus4_p2.files[:800]
# WJetsPlus5.files = WJetsPlus5.files[350:]
# WJetsPlus5_p2.files = WJetsPlus5_p2.files[:350]
# WJetsPlus6.files = WJetsPlus6.files[350:]
# WJetsPlus6_p2.files = WJetsPlus6_p2.files[:350]

#####################################################################
#####################################################################

WJetsMinus = copy.deepcopy(WJetsPlus)
WJetsMinus.name = 'WMinus1'
WJetsMinus.files = getFiles('/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 2106

WJetsMinus2 = copy.deepcopy(WJetsPlus)
WJetsMinus2.name = 'WMinus2'
WJetsMinus2.files = getFiles('/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 2106

WJetsMinus3 = copy.deepcopy(WJetsPlus)
WJetsMinus3.name = 'WMinus3'
WJetsMinus3.files = getFiles('/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pytha8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root') # 1890


WJetsMinus_p2 = copy.deepcopy(WJetsMinus)
WJetsMinus_p2.name = 'WMinus_p2'
WJetsMinus2_p2 = copy.deepcopy(WJetsMinus2)
WJetsMinus2_p2.name = 'WMinus2_p2'
WJetsMinus3_p2 = copy.deepcopy(WJetsMinus3)
WJetsMinus3_p2.name = 'WMinus3_p2'


# WJetsPlus.files = WJetsPlus.files[400:]
# WJetsPlus_p2.files = WJetsPlus_p2.files[:400]
WJetsMinus.files = WJetsMinus.files[1000:]
WJetsMinus_p2.files = WJetsMinus_p2.files[:1000]
WJetsMinus2.files = WJetsMinus2.files[1000:]
WJetsMinus2_p2.files = WJetsMinus2_p2.files[:1000]
WJetsMinus3.files = WJetsMinus3.files[900:]
WJetsMinus3_p2.files = WJetsMinus3_p2.files[:900]

# selectedComponents = [WJetsPlus,WJetsPlus2,WJetsPlus3]
# WJetsPlus.files = WJetsPlus.files[:3750]
# WJetsPlus2.files = WJetsPlus2.files[3750:]

# ONE AT THE TIME
# selectedComponents = [WJetsPlus]
# selectedComponents = [WJetsPlus_p2]
# selectedComponents = [WJetsPlus2]
# selectedComponents = [WJetsPlus2_p2]
# selectedComponents = [WJetsPlus3]
# selectedComponents = [WJetsPlus3_p2]
# selectedComponents = [WJetsPlus4]
# selectedComponents = [WJetsPlus4_p2]
# selectedComponents = [WJetsPlus5]
# # selectedComponents = [WJetsPlus5_p2]
# selectedComponents = [WJetsPlus6]
# # selectedComponents = [WJetsPlus6_p2]

# selectedComponents = [WJetsPlus,WJetsPlus_p2,WJetsPlus2]
# selectedComponents = [WJetsPlus2_p2,WJetsPlus3,WJetsPlus3_p2]
# selectedComponents = [WJetsPlus4,WJetsPlus4_p2,WJetsPlus5]
# selectedComponents = [WJetsPlus6]

# selectedComponents = [WJetsMinus]
# selectedComponents = [WJetsMinus_p2]
# selectedComponents = [WJetsMinus2]
# selectedComponents = [WJetsMinus2_p2]
# selectedComponents = [WJetsMinus3]
# selectedComponents = [WJetsMinus3_p2]

# selectedComponents = [WJetsMinus,WJetsMinus_p2]
# selectedComponents = [WJetsMinus2,WJetsMinus2_p2]
selectedComponents = [WJetsMinus3,WJetsMinus3_p2]

# # TEST
# # selectedComponents = [WJetsPlus,WJetsPlus2,WJetsPlus3]
selectedComponents = [WJetsPlus]
WJetsPlus.files = WJetsPlus.files[:1]
# WJetsPlus.splitFactor = 1
# WJetsPlus2.files = WJetsPlus2.files[:1]
# WJetsPlus2.splitFactor = 1
# WJetsPlus3.files = WJetsPlus3.files[:1]
# WJetsPlus3.splitFactor = 1

# WJetsPlus4 = copy.deepcopy(WJetsPlus)
# WJetsPlus4.name = 'WJetsPlus4'
# WJetsPlus4.files = ['file:/afs/cern.ch/work/p/perrozzi/private/git/v5_18_0/CMSSW_5_3_19/src/GeneratorInterface/LHEInterface/test/test_cmgTuple.root']
# selectedComponents = [WJetsPlus4]
# WJetsPlus.files = WJetsPlus.files[:1]
# WJetsPlus.splitFactor = 1


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


