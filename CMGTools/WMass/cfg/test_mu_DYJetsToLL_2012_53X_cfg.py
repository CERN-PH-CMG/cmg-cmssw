#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

vertexAna.allVertices = 'offlinePrimaryVertices'
vertexAna.goodVertices = 'offlinePrimaryVertices'

sequence = cfg.Sequence(CoreZsequence)

from CMGTools.H2TauTau.proto.samples.ewk import DYJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0', 'cmgtools', '.*root')
DYJets.triggers = triggers_mu

DYJets.splitFactor = 750

## TEST
#DYJets.splitFactor = 1
#DYJets.files = DYJets.files[0:5]



DYJets2 = copy.deepcopy(DYJets)

# selectedComponents = [DYJets,DYJets2]
# DYJets.files = DYJets.files[:1250]
# DYJets2.files = DYJets2.files[1250:]

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


