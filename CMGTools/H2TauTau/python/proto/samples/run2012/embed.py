import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'

# trigger list: 'HLT_Mu17_Mu8_v16', 'HLT_Mu17_Mu8_v17', 'HLT_Mu17_Mu8_v18', 'HLT_Mu17_Mu8_v19', 'HLT_Mu17_Mu8_v21', 'HLT_Mu17_Mu8_v22'

embed_Run2012A_22Jan = cfg.EmbedComponent(
    name = 'embed_Run2012A_22Jan',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )

embed_Run2012B_22Jan = cfg.EmbedComponent(
    name = 'embed_Run2012B_22Jan',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )
    
embed_Run2012C_22Jan = cfg.EmbedComponent(
    name = 'embed_Run2012C_22Jan',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )
    
embed_Run2012D_22Jan = cfg.EmbedComponent(
    name = 'embed_Run2012D_22Jan',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = None
    )

    

embed_list = [embed_Run2012A_22Jan,
              embed_Run2012B_22Jan,
              embed_Run2012C_22Jan,
              embed_Run2012D_22Jan
              ]




