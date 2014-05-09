import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap8TeV import pathsAndFilters

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
     keepFailingEvents = True        
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1.,
     keepFailingEvents = True        
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerMap = pathsAndFilters
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
    savegenp = False,
    verbose = True,
    triggerMap = pathsAndFilters
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer'
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
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


from CMGTools.H2TauTau.proto.samples.getFiles import getFiles



Zpow_mumu = cfg.MCComponent(
    name = 'Zpow_mumu',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)

Zpow_tautau = cfg.MCComponent(
    name = 'Zpow_tautau',
    files = [],
    xSection = 7.467,
    nGenEvents = 1,
    triggers = [],
    effCorrFactor = 1)



Zpow_mumu.files = getFiles('/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0_2nd', 'cmgtools', '.*root')
Zpow_mumu.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]


Zpow_tautau.files = getFiles('/DYToTauTau_M-20_CT10_TuneZ2star_v2_8TeV-powheg-tauola-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM/V5_B/PAT_CMG_V5_17_0_2nd', 'cmgtools','.*root')
Zpow_tautau.triggers =  ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ]



Zpow_mumu.splitFactor = 1000
Zpow_tautau.splitFactor = 1000


selectedComponents = [Zpow_mumu, Zpow_tautau]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


