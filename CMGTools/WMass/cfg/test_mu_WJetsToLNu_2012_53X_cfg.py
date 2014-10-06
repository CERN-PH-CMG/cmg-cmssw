#Load all analyzers
from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

sequence = cfg.Sequence(CoreWZsequence)

from CMGTools.H2TauTau.proto.samples.ewk import WJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# # WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# WJets.triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ]

# selectedComponents = [WJets]

# WJets.splitFactor = 1500

# TEST
WJets1 = copy.deepcopy(WJets)
WJets1.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_18_0_NewTauID', 'cmgtools', '.*root')
WJets1.triggers = triggers_mu
# WJets1.files = WJets1.files[:1600]
WJets1.splitFactor = 1000
WJets1.name = 'WJets1'
WJets2 = copy.deepcopy(WJets)
WJets2.files = getFiles('/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_18_0_NewTauID', 'cmgtools', '.*root')
WJets2.triggers = triggers_mu
WJets2.files = WJets2.files[1600:]
WJets2.splitFactor = 1000
WJets2.name = 'WJets2'
selectedComponents = [WJets1,WJets2]

# # TEST
selectedComponents = [WJets1]
# WJets1.files = WJets1.files[:2]
# WJets1.splitFactor = 1


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)


