import CMGTools.RootTools.fwlite.Config as cfg


#FIXME add run numbers

data_Run2012A = cfg.DataComponent(
    name = 'data_Run2012A',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194479_8TeV_PromptReco_Collisions12_JSON.txt'
    )

data_Run2012B = cfg.DataComponent(
    name = 'data_Run2012B',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-194479_8TeV_PromptReco_Collisions12_JSON.txt'
    )


data_2012 = [
    data_Run2012A,
    data_Run2012B
    ]
