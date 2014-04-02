import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg

jsondir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV'
promptjson  =  '/'.join([jsondir, 'Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'])


embed_Run2011A_May10ReReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_May10ReReco_v1',
    files = [],
    triggers = [],
    json = '/'.join([jsondir, 'Reprocessing/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.txt'])
    )

embed_Run2011A_PromptReco_v4 = cfg.EmbedComponent(
    name = 'embed_Run2011A_PromptReco_v4',
    files = [],
    triggers = [],
    json = promptjson
    ) 

embed_Run2011A_05Aug2011_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_05Aug2011_v1',
    files = [],
    triggers = [],
    json = '/'.join([jsondir, 'Reprocessing/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v3.txt'])
    ) 

embed_Run2011A_03Oct2011_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011A_03Oct2011_v1',
    files = [],
    triggers = [],
    json = promptjson
    ) 

embed_Run2011B_PromptReco_v1 = cfg.EmbedComponent(
    name = 'embed_Run2011B_PromptReco_v1',
    files = [],
    triggers = [],
    json = promptjson
    ) 



embed_list_2011A = [
    embed_Run2011A_May10ReReco_v1,
    embed_Run2011A_PromptReco_v4,
    embed_Run2011A_05Aug2011_v1,
    embed_Run2011A_03Oct2011_v1
    ]

embed_list_2011B = [
    embed_Run2011B_PromptReco_v1
    ]

embed_list_2011 = copy.copy( embed_list_2011A )
embed_list_2011.extend( embed_list_2011B ) 
