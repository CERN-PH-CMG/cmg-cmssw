import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import triggers_jpsi,triggers_upsilon,triggers_mu

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
     'triggerBitFilter',
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    allVertices = 'slimmedPrimaryVertices',
    goodVertices = 'slimmedPrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False,
    keepFailingEvents = True,
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000, # 20
    pfmetcut = 0, # 25
    jetptcut = 1000, # 30
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerBits = {'Dimuon10_Jpsi' : triggers_jpsi},
    # triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = True,    
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'JPsiAnalyzer',
    recoilcut = 1000, # 20
    pfmetcut = 0, # 25
    jetptcut = 1000, # 30
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    # savegenp = False,
    savegenp = True,
    verbose = True,
    triggerBits = {'Dimuon10_Jpsi' : triggers_jpsi},
    # triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = True,
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer',
    # storeNeutralCMGcandidates = True,
    # storeCMGcandidates = True,
    # storeSlimRecoInfo = True,
    # storeSlimGenInfo = True,    
    )

genAna = cfg.Analyzer(
    'GenParticleAnalyzerFSR',
    src = 'genParticlesPruned'
)

sequence = cfg.Sequence( [
    genAna,
    # jsonAna,
    # triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


from CMGTools.WMass.analyzers.CoreModule_53X_cff import *

# sequence = cfg.Sequence(CoreWZsequence)

from CMGTools.H2TauTau.proto.samples.ewk import WJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles



JpsiMC = copy.deepcopy(WJets)
JpsiMC.files = getFiles('/JPsiToMuMu_2MuPtEtaFilter_tuneD6T_8TeV-pythia6-evtgen/Summer12_DR53X-PU_RD2_START53_V19F-v3/AODSIM/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
JpsiMC.name = 'JpsiMC'
JpsiMC.triggers = triggers_jpsi
JpsiMC.splitFactor = 750

JpsiMC2 = copy.deepcopy(WJets)
JpsiMC2.files = getFiles('/JPsiToMuMu_2MuPtEtaFilter_tuneD6T_8TeV-pythia6-evtgen/Summer12_DR53X-PU_RD2_START53_V19F-v2/AODSIM/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
JpsiMC2.name = 'JpsiMC2'
JpsiMC2.triggers = triggers_jpsi
JpsiMC2.splitFactor = 750

JpsiMC3 = copy.deepcopy(WJets)
JpsiMC3.files = getFiles('/JPsiToMuMu_JPsiPt20_2MuPtEtaFilter_tuneD6T_8TeV-pythia6-evtgen/Summer12_DR53X-PU_RD2_START53_V19F-v1/AODSIM/PAT_CMG_V5_18_0', 'cmgtools', '.*root')
JpsiMC3.name = 'JpsiMC3'
JpsiMC3.triggers = triggers_jpsi
JpsiMC3.splitFactor = 750

# selectedComponents = [JpsiMC,JpsiMC2,JpsiMC3]
selectedComponents = [JpsiMC2]

# # ######## TEST ##################
# JpsiMC.files = JpsiMC.files[:10]
# JpsiMC.splitFactor = 1
# selectedComponents = [JpsiMC]
# # ################################


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


