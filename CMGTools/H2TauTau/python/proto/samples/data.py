import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

#COLIN: I'm not completely sure we should use the jsons in our package.
# what about using the official ones? At least, one should check if it makes any difference

jsondir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV'
promptjson  =  '/'.join([jsondir, 'Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'])


data_Run2011A_May10ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011A_May10ReReco_v1',
    files = [],
    intLumi = 215.63, 
    triggers = [],
    json = '/'.join([jsondir, 'Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt'])
    )

data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files = [],
    intLumi = 951.426, # 955.212 (Pietro)
    triggers = [],
    json = promptjson
    )

data_Run2011A_05Aug2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_05Aug2011_v1',
    files = [],
    intLumi = 383.798, # 389.876 (Pietro)
    triggers = [],
    json = '/'.join([jsondir, 'Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt'])
    )

data_Run2011A_03Oct2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_03Oct2011_v1',
    files = [],
    intLumi = 696.471, #706.7 (Pietro)
    triggers = [],
    json = promptjson
    )

data_Run2011B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_PromptReco_v1',
    files = [],
    intLumi = 2714.0, #same (Pietro)
    triggers = [],
    json = promptjson
    )


data_2011A = [
    data_Run2011A_May10ReReco_v1,
    data_Run2011A_PromptReco_v4,
    data_Run2011A_05Aug2011_v1,
    data_Run2011A_03Oct2011_v1
    ]

data_2011B = [
    data_Run2011B_PromptReco_v1
    ]

data_2011 = copy.copy( data_2011A )
data_2011.extend( data_2011B ) 
