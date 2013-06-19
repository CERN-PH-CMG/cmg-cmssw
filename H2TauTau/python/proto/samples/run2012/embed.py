import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'


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




