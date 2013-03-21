import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'


embed_Run2012A_13Jul2012_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_13Jul2012_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )

embed_Run2012A_recover_06Aug2012_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012A_recover_06Aug2012_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )

embed_Run2012B_13Jul2012_v4 = cfg.EmbedComponent(
    name = 'embed_Run2012B_13Jul2012_v4',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )
    
embed_Run2012C_24Aug2012_v1 = cfg.EmbedComponent(
    name = 'embed_Run2012C_24Aug2012_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )
    
embed_Run2012C_PromptReco_v2 = cfg.EmbedComponent(
    name = 'embed_Run2012C_PromptReco_v2',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )

embed_2012D_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_2012D_PromptReco_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )

    

embed_list = [embed_Run2012A_13Jul2012_v1,
              embed_Run2012A_recover_06Aug2012_v1,
              embed_Run2012B_13Jul2012_v4,
              embed_Run2012C_24Aug2012_v1,
              embed_Run2012C_PromptReco_v2,
              embed_2012D_PromptReco_v1
              ]




