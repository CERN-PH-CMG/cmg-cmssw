import CMGTools.RootTools.fwlite.Config as cfg
import copy

#FIXME add run numbers

data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194479_8TeV_PromptReco_Collisions12_JSON.txt'
    )

data_Run2012B_start_194479 = cfg.DataComponent(
    name = 'data_Run2012B_start_194479',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194479_8TeV_PromptReco_Collisions12_JSON.txt'
    )

data_Run2012B_194480_195016 = cfg.DataComponent(
    name = 'data_Run2012B_194480_195016',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON.txt'
    )

data_list_Run2012A = [data_Run2012A]
data_list_Run2012B = [data_Run2012B_start_194479,
                      data_Run2012B_194480_195016]

data_list_2012 = copy.copy(data_list_Run2012A)
data_list_2012.extend(data_list_Run2012B)
