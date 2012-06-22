import CMGTools.RootTools.fwlite.Config as cfg
import copy

#FIXME add run numbers

json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195947_8TeV_PromptReco_Collisions12_JSON_v2.txt'

data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json
    )

data_Run2012B_start_194479 = cfg.DataComponent(
    name = 'data_Run2012B_start_194479',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json
    )

data_Run2012B_194480_195016 = cfg.DataComponent(
    name = 'data_Run2012B_194480_195016',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json
    )

data_Run2012B_195017_195947 = cfg.DataComponent(
    name = 'data_Run2012B_195017_195947',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = json
    )

data_list_Run2012A = [ data_Run2012A ]
data_list_Run2012B = [ data_Run2012B_start_194479,
                       data_Run2012B_194480_195016,
                       data_Run2012B_195017_195947 ]

data_list_2012 = copy.copy(data_list_Run2012A)
data_list_2012.extend(data_list_Run2012B)
