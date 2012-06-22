import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles
from CMGTools.H2TauTau.proto.samples.sampleShift import sampleShift

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'

json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195947_8TeV_PromptReco_Collisions12_JSON_v2.txt'

embed_Run2012A = cfg.EmbedComponent(
    name = 'embed_Run2012A',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )

embed_Run2012B_193752_195135 = cfg.EmbedComponent(
    name = 'embed_Run2012B_193752_195135',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )

embed_Run2012B_195147_196070 = cfg.EmbedComponent(
    name = 'embed_Run2012B_195147_196070',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json,
    )




embed_list_Run2012A = [embed_Run2012A]
embed_list_Run2012B = [embed_Run2012B_193752_195135,
                       embed_Run2012B_195147_196070 ]

embed_list_2012 = copy.copy(embed_list_Run2012A)
embed_list_2012.extend(embed_list_Run2012B)

embed_list_2012_up = sampleShift( locals(), embed_list_2012, 'Up')
embed_list_2012_down = sampleShift( locals(), embed_list_2012, 'Down')
