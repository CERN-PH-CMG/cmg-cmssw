import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import triggers_mu

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
     'triggerBitFilter',
     # verbose = True,
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    allVertices = 'slimmedPrimaryVertices',
    goodVertices = 'slimmedPrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False,
    keepFailingEvents = False,
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True,
    triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'ZAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True,
    triggerBits = {'SingleMu' : triggers_mu},
    # keepFailingEvents = True,
    keepFailingEvents = False,
    storeLHE_weight = True,
    use_newWeights = True,
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer',
    storeNeutralCMGcandidates = False,
    # storeCMGcandidates = True,
    storeLHE_weight = True
    )

genAna = cfg.Analyzer(
'GenParticleAnalyzerFSR',
src = 'genParticlesPruned'
)

sequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer,
   ] )

from CMGTools.H2TauTau.proto.samples.ewk import DYJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# DYJets.files = getFiles('/DYJetsToLL_M-50_7TeV-madgraph-pythia6-tauola/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
DYJets.triggers = triggers_mu
DYJets.splitFactor = 900


DYJets2 = copy.deepcopy(DYJets)
DYJets2.name = 'DYJets2'
DYJets2.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

DYJets3 = copy.deepcopy(DYJets)
DYJets3.name = 'DYJets3'
DYJets3.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg-pythia8/Summer11LegDR-PU_S13_START53_LV6-v2/AODSIM/V5_B/PAT_CMG_V5_18_0', 'cmgtools', '.*root')

DYJets1_newWeights = copy.deepcopy(DYJets)
DYJets1_newWeights.name = 'DYJets1_newWeights'
DYJets1_newWeights.files = getFiles('/DYToMuMu_M-50To250_ew-BMNNP_7TeV-powheg/Summer11LegDR-PU_S13_START53_LV6-v1/AODSIM/V5_B/PAT_CMG_V5_18_0_newLHEweights', 'cmgtools', '.*root')

DYJets_p2 = copy.deepcopy(DYJets)
DYJets_p2.name = 'DYJets_p2'
DYJets2_p2 = copy.deepcopy(DYJets2)
DYJets2_p2.name = 'DYJets2_p2'
DYJets3_p2 = copy.deepcopy(DYJets3)
DYJets3_p2.name = 'DYJets3_p2'


DYJets.files = DYJets.files[400:]
DYJets_p2.files = DYJets_p2.files[:400]
DYJets2.files = DYJets2.files[400:]
DYJets2_p2.files = DYJets2_p2.files[:400]
DYJets3.files = DYJets3.files[400:]
DYJets3_p2.files = DYJets3_p2.files[:400]

# selectedComponents = [DYJets,DYJets2,DYJets3]
# DYJets.files = DYJets.files[:3750]
# DYJets2.files = DYJets2.files[3750:]

# ONE AT THE TIME
# selectedComponents = [DYJets]
# selectedComponents = [DYJets_p2]
# selectedComponents = [DYJets2]
# selectedComponents = [DYJets2_p2]
# selectedComponents = [DYJets3]
# selectedComponents = [DYJets3_p2]
selectedComponents = [DYJets1_newWeights]


# # TEST
# # selectedComponents = [DYJets,DYJets2,DYJets3]
# selectedComponents = [DYJets]
# DYJets.files = DYJets.files[:1]
# DYJets.splitFactor = 1
# DYJets2.files = DYJets2.files[:1]
# DYJets2.splitFactor = 1
# DYJets3.files = DYJets3.files[:1]
# DYJets3.splitFactor = 1

# DYJets4 = copy.deepcopy(DYJets)
# DYJets4.name = 'DYJets4'
# DYJets4.files = ['file:/afs/cern.ch/work/p/perrozzi/private/git/v5_18_0/CMSSW_5_3_19/src/GeneratorInterface/LHEInterface/test/test_cmgTuple.root']
# selectedComponents = [DYJets4]

DYJets1_newWeights.splitFactor = 1
DYJets1_newWeights.files = DYJets1_newWeights.files[:1]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


