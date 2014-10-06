#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

sequence = cfg.Sequence(CoreWZsequence)


from CMGTools.H2TauTau.proto.samples.ewk import DYJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
DYJets.files = getFiles('/DYJetsToLL_M-50_7TeV-madgraph-pythia6-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets.triggers = triggers_mu


DYJets.splitFactor = 750

DYJets2 = copy.deepcopy(DYJets)

# selectedComponents = [DYJets,DYJets2]
# DYJets.files = DYJets.files[:3750]
# DYJets2.files = DYJets2.files[3750:]

# ONE AT THE TIME
selectedComponents = [DYJets]
# selectedComponents = [DYJets2]

# # TEST
# selectedComponents = [DYJets]
# DYJets.files = DYJets.files[:10]
# DYJets.splitFactor = 1


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


