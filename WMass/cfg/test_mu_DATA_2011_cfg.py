import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
     keepFailingEvents = False    
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1.
    keepFailingEvents = False    
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    # recoilcut = 20,
    # pfmetcut = 25,
    # jetptcut = 30,
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'ZAnalyzer',
    # recoilcut = 25, # rised from 20 to 25 to allow offline MZ/MW scaled analysis cut
    # pfmetcut = 25,
    # jetptcut = 30,
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer'
    )

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


# from CMGTools.H2TauTau.proto.samples.ewk import WJets, DYJets, TTJets, QCD15, QCD30, QCD50, QCD80
# from CMGTools.H2TauTau.proto.samples.diboson import WWJetsTo2L2Nu, WZJetsTo2L2Q, WZJetsTo3LNu, ZZJetsTo2L2Nu, ZZJetsTo2L2Q, ZZJetsTo4L
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles


data_Run2011A_Nov8ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011A_Nov8ReReco_v1',
    files = getFiles('/SingleMu/Run2011A-08Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_eta2p1_v*"],
    triggers = ["HLT_IsoMu24_v*","HLT_IsoMu24_eta2p1_v*"],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    )
data_Run2011B_Nov19ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_Nov19ReReco_v1',
    files = getFiles('/SingleMu/Run2011B-19Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_eta2p1_v*"],
    triggers = ["HLT_IsoMu24_v*","HLT_IsoMu24_eta2p1_v*"],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    )

data_Run2011AB_Nov8Nov19ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011AB_Nov8Nov19ReReco_v1',
    files =  [],
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_eta2p1_v*"],
    triggers = ["HLT_IsoMu24_v*","HLT_IsoMu24_eta2p1_v*"],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    )

# data_Run2011A_Nov8ReReco_v1.files = data_Run2011A_Nov8ReReco_v1.files[:50]
# data_Run2011B_Nov19ReReco_v1.files = data_Run2011B_Nov19ReReco_v1.files[:50]
# data_Run2011AB_Nov8Nov19ReReco_v1.files = data_Run2011AB_Nov8Nov19ReReco_v1.files[:50]

data_Run2011AB_Nov8Nov19ReReco_v1.files = data_Run2011A_Nov8ReReco_v1.files + data_Run2011B_Nov19ReReco_v1.files

# selectedComponents = [data_Run2011A_Nov8ReReco_v1,data_Run2011B_Nov19ReReco_v1]
# selectedComponents = [data_Run2011A_Nov8ReReco_v1]
# selectedComponents = [data_Run2011B_Nov19ReReco_v1]
selectedComponents = [data_Run2011AB_Nov8Nov19ReReco_v1]


data_Run2011A_Nov8ReReco_v1.splitFactor = 1
data_Run2011B_Nov19ReReco_v1.splitFactor = 1
data_Run2011AB_Nov8Nov19ReReco_v1.splitFactor = 1000

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


