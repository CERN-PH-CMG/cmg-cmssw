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
    recoilcut = 1000, # 20
    pfmetcut = 0, # 25
    jetptcut = 1000, # 30
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
    recoilcut = 1000, # 20
    pfmetcut = 0, # 25
    jetptcut = 1000, # 30
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

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


from CMGTools.H2TauTau.proto.samples.getFiles import getFiles


data_Run2011A_Nov8ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011A_Nov8ReReco_v1',
    files = getFiles('/SingleMu/Run2011A-08Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    )
    
data_Run2011B_Nov19ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_Nov19ReReco_v1',
    files = getFiles('/SingleMu/Run2011B-19Nov2011-v1/AOD/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    )

data_Run2011AB_Nov8Nov19ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011AB_Nov8Nov19ReReco_v1',
    files =  [],
    intLumi =  4749.90,
triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    )

data_Run2011AB_Nov8Nov19ReReco_v1.files = data_Run2011A_Nov8ReReco_v1.files + data_Run2011B_Nov19ReReco_v1.files
# data_Run2011AB_Nov8Nov19ReReco_v1.files = data_Run2011AB_Nov8Nov19ReReco_v1.files[:50]

selectedComponents = [data_Run2011AB_Nov8Nov19ReReco_v1]


data_Run2011A_Nov8ReReco_v1.splitFactor = 1
data_Run2011B_Nov19ReReco_v1.splitFactor = 1
data_Run2011AB_Nov8Nov19ReReco_v1.splitFactor = 2000

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


