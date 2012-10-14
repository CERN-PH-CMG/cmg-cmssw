import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'

json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196509_8TeV_PromptReco_Collisions12_JSON.txt'


embed_Run2012A_13Jul2012_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_13Jul2012_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )

embed_Run2012A_recover_06Aug2012_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_recover_06Aug2012_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )

embed_Run2012B_13Jul2012_v4 = cfg.EmbedComponent(
    name = 'embed_Run2012B_13Jul2012_v4',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )
    
embed_Run2012C_24Aug2012_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012C_24Aug2012_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )
    
embed_Run2012C_PromptReco_v2 = cfg.EmbedComponent(
    name = 'embed_Run2012C_PromptReco_v2',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )
    
    

embed_list = [embed_Run2012A_13Jul2012_v1,
    embed_Run2012A_recover_06Aug2012_v1,
    embed_Run2012B_13Jul2012_v4,
    embed_Run2012C_24Aug2012_v1,
    embed_Run2012C_PromptReco_v2
]




