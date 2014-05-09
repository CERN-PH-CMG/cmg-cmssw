import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import pathsAndFilters

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
     keepFailingEvents = False        
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1.,
     keepFailingEvents = False        
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
    triggerMap = pathsAndFilters,
    keepFailingEvents = False
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    # 'ZAnalyzer',
    'JPsiAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True,
    triggerMap = pathsAndFilters,
    keepFailingEvents = False
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
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )

from CMGTools.H2TauTau.proto.samples.ewk import TTJets as JPsiMM
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

JPsiMM.files = getFiles('/JPsiToMuMu_2MuPEtaFilter_7TeV-pythia6-evtgen/Fall11-PU_S6_START44_V9B-v1/AODSIM/V5_B/PAT_CMG_5_6_0_B', 'cmgtools', '.*root')
JPsiMM.triggers = ["HLT_Dimuon10_Jpsi_Barrel_v1",
                   "HLT_Dimuon10_Jpsi_Barrel_v2",
                   "HLT_Dimuon10_Jpsi_Barrel_v3",
                   "HLT_Dimuon10_Jpsi_Barrel_v5",
                   "HLT_Dimuon10_Jpsi_Barrel_v6",
                   "HLT_Dimuon10_Jpsi_Barrel_v9",
                   "HLT_Dimuon10_Jpsi_Barrel_v10",
                   ]

selectedComponents = [JPsiMM]

JPsiMM.splitFactor = 750

# TEST
JPsiMM.splitFactor = 1
JPsiMM.files = JPsiMM.files[0:2]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


