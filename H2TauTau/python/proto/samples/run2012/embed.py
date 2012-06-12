import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

embed_Run2012A = cfg.DataComponent(
    name = 'embed_Run2012A',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195396_8TeV_PromptReco_Collisions12_JSON_v2.txt'
    )

embed_Run2012B_193752_195135 = cfg.DataComponent(
    name = 'embed_Run2012B_193752_195135',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195396_8TeV_PromptReco_Collisions12_JSON_v2.txt'
    )




embed_list_Run2012A = [embed_Run2012A]
embed_list_Run2012B = [embed_Run2012B_193752_195135]

embed_list_2012 = copy.copy(embed_list_Run2012A)
embed_list_2012.extend(embed_list_Run2012B)
