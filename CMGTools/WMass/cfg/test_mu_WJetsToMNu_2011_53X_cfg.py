#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import * 

sequence = cfg.Sequence(CoreWsequence)


from CMGTools.H2TauTau.proto.samples.ewk import WJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# WJets.files = getFiles('/WJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# WJets.files = getFiles('/WJetsToLL_M-50_7TeV-madgraph-pythia6-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
WJets.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
WJets.triggers = triggers_mu
WJets.splitFactor = 900
WJets.name = 'WPlus1'

WJets2 = copy.deepcopy(WJets)
WJets2.name = 'WPlus2'
WJets2.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v3/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

WJets3 = copy.deepcopy(WJets)
WJets3.name = 'WPlus3'
WJets3.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v7/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

WJets4 = copy.deepcopy(WJets)
WJets4.name = 'WPlus4'
WJets4.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v4/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

WJets5 = copy.deepcopy(WJets)
WJets5.name = 'WPlus5'
WJets5.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v6/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

WJets6 = copy.deepcopy(WJets)
WJets6.name = 'WPlus6'
WJets6.files = getFiles('/WplusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v5/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

#####################################################################
#####################################################################

WJets7 = copy.deepcopy(WJets)
WJets7.name = 'WMinus1'
WJets7.files = getFiles('/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

WJets8 = copy.deepcopy(WJets)
WJets8.name = 'WMinus2'
WJets8.files = getFiles('/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

WJets9 = copy.deepcopy(WJets)
WJets9.name = 'WMinus3'
WJets9.files = getFiles('/WminusToMuNu_M-50To250_ew-BMNNP_7TeV-powheg-pytha8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')


# WJets_p2 = copy.deepcopy(WJets)
# WJets_p2.name = 'WJets_p2'
# WJets2_p2 = copy.deepcopy(WJets2)
# WJets2_p2.name = 'WJets2_p2'
# WJets3_p2 = copy.deepcopy(WJets3)
# WJets3_p2.name = 'WJets3_p2'


# WJets.files = WJets.files[400:]
# WJets_p2.files = WJets_p2.files[:400]
# WJets2.files = WJets2.files[400:]
# WJets2_p2.files = WJets2_p2.files[:400]
# WJets3.files = WJets3.files[400:]
# WJets3_p2.files = WJets3_p2.files[:400]

# selectedComponents = [WJets,WJets2,WJets3]
# WJets.files = WJets.files[:3750]
# WJets2.files = WJets2.files[3750:]

# ONE AT THE TIME
# selectedComponents = [WJets]
selectedComponents = [WJets7,WJets8]
# selectedComponents = [WJets_p2]
# selectedComponents = [WJets2]
# selectedComponents = [WJets2_p2]
# selectedComponents = [WJets3]
# selectedComponents = [WJets3_p2]


# # TEST
# # selectedComponents = [WJets,WJets2,WJets3]
# selectedComponents = [WJets]
# WJets.files = WJets.files[:1]
# WJets.splitFactor = 1
# WJets2.files = WJets2.files[:1]
# WJets2.splitFactor = 1
# WJets3.files = WJets3.files[:1]
# WJets3.splitFactor = 1

# WJets4 = copy.deepcopy(WJets)
# WJets4.name = 'WJets4'
# WJets4.files = ['file:/afs/cern.ch/work/p/perrozzi/private/git/v5_18_0/CMSSW_5_3_19/src/GeneratorInterface/LHEInterface/test/test_cmgTuple.root']
# selectedComponents = [WJets4]
# WJets.files = WJets.files[:1]
# WJets.splitFactor = 1


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


