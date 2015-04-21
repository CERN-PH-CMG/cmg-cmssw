import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import triggers_jpsi,triggers_upsilon,triggers_mu,triggers_jpsi2012

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
    # keepFailingEvents = True,
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
    triggerBits = {'Dimuon10_Jpsi' : triggers_jpsi2012},
    # triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False,    
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
    savegenp = False,
    verbose = True,
    triggerBits = {'Dimuon10_Jpsi' : triggers_jpsi2012},
    # triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False,
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer',
    # storeNeutralCMGcandidates = True,
    # storeCMGcandidates = True,
    # storeSlimRecoInfo = True,
    # storeSlimGenInfo = True,    
    )

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )


from CMGTools.H2TauTau.proto.samples.getFiles import getFiles


data_Run2012D_Jan22ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2012D_Jan22ReReco_v1',
    files = getFiles('/MuOnia/Run2012D-22Jan2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ],
    triggers = triggers_jpsi2012
               # +triggers_upsilon
               ,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )
    
data_Run2012C_Jan22ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2012C_Jan22ReReco_v1',
    files = getFiles('/MuOnia/Run2012C-22Jan2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ],
    triggers = triggers_jpsi2012
              # +triggers_upsilon
              ,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )

data_Run2012B_Jan22ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2012B_Jan22ReReco_v1',
    files = getFiles('/MuOnia/Run2012B-22Jan2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ],
    triggers = triggers_jpsi2012
              # +triggers_upsilon
              ,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )

data_Run2012A_Jan22ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2012A_Jan22ReReco_v1',
    files = getFiles('/MuOnia/Run2012A-22Jan2013-v1/AOD/PAT_CMG_V5_18_0', 'cmgtools', '.*root'),
    intLumi =  4749.90,
    # triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ],
    triggers = triggers_jpsi2012
              # +triggers_upsilon
              ,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )

data_Run2011AB_Nov8Nov19ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011AB_Nov8Nov19ReReco_v1',
    files =  [],
    intLumi =  4749.90,
# triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # ],
    triggers = triggers_jpsi2012
              # +triggers_upsilon
              ,
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    )

data_Run2011AB_Nov8Nov19ReReco_v1.files = data_Run2012D_Jan22ReReco_v1.files + data_Run2012C_Jan22ReReco_v1.files

# selectedComponents = [data_Run2012D_Jan22ReReco_v1,data_Run2012C_Jan22ReReco_v1]
# selectedComponents = [data_Run2012D_Jan22ReReco_v1]
# selectedComponents = [data_Run2012C_Jan22ReReco_v1]
selectedComponents = [data_Run2012A_Jan22ReReco_v1,data_Run2012B_Jan22ReReco_v1]
data_Run2012D_Jan22ReReco_v1.splitFactor = 900
data_Run2012C_Jan22ReReco_v1.splitFactor = 1000
data_Run2012B_Jan22ReReco_v1.splitFactor = 1000
data_Run2012A_Jan22ReReco_v1.splitFactor = 1000
data_Run2011AB_Nov8Nov19ReReco_v1.splitFactor = 2000


## TEST
# selectedComponents = [data_Run2012D_Jan22ReReco_v1]
# data_Run2012D_Jan22ReReco_v1.splitFactor = 1
# data_Run2012D_Jan22ReReco_v1.files = data_Run2012D_Jan22ReReco_v1.files[:10]
# selectedComponents = [data_Run2012C_Jan22ReReco_v1]
# data_Run2012C_Jan22ReReco_v1.splitFactor = 1
# data_Run2012C_Jan22ReReco_v1.files = data_Run2012C_Jan22ReReco_v1.files[:10]


# data_Run2012C_Jan22ReReco_v1_testPFcands = cfg.DataComponent(
    # name = 'data_Run2012C_Jan22ReReco_v1_testPFcands',
    # files = [
# 'root://eoscms//eos/cms/store/cmst3/user/cmgtools/CMG/MuOnia/Run2011B-19Nov2011-v1/AOD/PAT_CMG_5_6_0_B_pfcandidates/cmgTuple_2405.root',

    # ],
    # # files = getFiles('/MuOnia/Run2011A-08Nov2011-v1/AOD/PAT_CMG_5_6_0_B_pfcandidates', 'cmgtools', '.*root'),
    # intLumi =  4749.90,
    # # triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   # # "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   # # "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   # # "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   # # ],
    # triggers = ["HLT_Dimuon10_Jpsi_Barrel_v1",
                   # "HLT_Dimuon10_Jpsi_Barrel_v2",
                   # "HLT_Dimuon10_Jpsi_Barrel_v3",
                   # "HLT_Dimuon10_Jpsi_Barrel_v5",
                   # "HLT_Dimuon10_Jpsi_Barrel_v6",
                   # "HLT_Dimuon10_Jpsi_Barrel_v9",
                   # "HLT_Dimuon10_Jpsi_Barrel_v10",
                   # ],
    # json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt'
    # )

# # data_Run2012C_Jan22ReReco_v1_testPFcands.splitFactor = 1
# # data_Run2012C_Jan22ReReco_v1_testPFcands.files = data_Run2012C_Jan22ReReco_v1_testPFcands.files[:5]
# data_Run2012C_Jan22ReReco_v1_testPFcands.splitFactor = 1000

# # selectedComponents = [data_Run2012D_Jan22ReReco_v1_testPFcands]
# # selectedComponents = [data_Run2012C_Jan22ReReco_v1_testPFcands]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


