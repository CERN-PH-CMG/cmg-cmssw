import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 


data_Run2012A_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2012A_PromptReco_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    )

data_Run2012B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2012B_PromptReco_v1',
    files = [],
    intLumi = 1.0, # dummy
    triggers = [],
    )


data_2012A = [
    data_Run2012A_PromptReco_v1
    ]

data_2012B = [
    data_Run2012B_PromptReco_v1
    ]

data_2012 = data_2012A + data_2012B
