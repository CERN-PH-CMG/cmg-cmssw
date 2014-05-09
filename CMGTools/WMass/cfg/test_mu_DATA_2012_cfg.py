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


Run2012A_22Jan2013_v1 = cfg.DataComponent(
    name = 'Run2012A-22Jan2013-v1',
    files = getFiles('/SingleMu/Run2012A-22Jan2013-v1/AOD/CMGPF_V5_16_0', 'cmgtools', '.*222.root'),
    intLumi =  4749.90,
    triggers = ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )
    
Run2012B_22Jan2013_v1 = cfg.DataComponent(
    name = 'Run2012B_22Jan2013_v1',
    files = getFiles('/SingleMu/Run2012B-22Jan2013-v1/AOD/CMGPF_V5_16_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )

Run2012C_22Jan2013_v1= cfg.DataComponent(
    name = 'Run2012C_22Jan2013_v1',
    files = getFiles('/SingleMu/Run2012C-22Jan2013-v1/AOD/CMGPF_V5_16_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )

Run2012D_22Jan2013_v1= cfg.DataComponent(
    name = 'Run2012D_22Jan2013_v1',
    files = getFiles('/SingleMu/Run2012D-22Jan2013-v1/AOD/CMGPF_V5_16_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    triggers = ["HLT_IsoMu24_eta2p1_v9","HLT_IsoMu24_eta2p1_v10","HLT_IsoMu24_eta2p1_v11",
                "HLT_IsoMu24_eta2p1_v12",
                "HLT_IsoMu24_eta2p1_v13",
                "HLT_IsoMu24_eta2p1_v14",
                "HLT_IsoMu24_eta2p1_v15",
                "HLT_IsoMu24_eta2p1_v16"
                   ],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    ) 



selectedComponents = [Run2012A_22Jan2013_v1]
Run2012A_22Jan2013_v1.splitFactor  = 1000
Run2012B_22Jan2013_v1.splitFactor  = 1000
Run2012C_22Jan2013_v1.splitFactor  = 1000
Run2012D_22Jan2013_v1.splitFactor  = 1000

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


