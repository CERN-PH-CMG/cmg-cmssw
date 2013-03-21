import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 

data_Run2012A_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2012A_PromptReco_v1',
    files = [],
    intLumi = 806.194,
    triggers = [],
    )

data_Run2012B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2012B_PromptReco_v1',
    files = [],
    intLumi = 4420.,
    triggers = [],
    )

data_Run2012C_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2012C_PromptReco_v1',
    files = [],
    intLumi = 493.975,
    triggers = [],
    )

data_Run2012C_PromptReco_v2 = cfg.DataComponent(
    name = 'data_Run2012C_PromptReco_v2',
    files = [],
    intLumi = 6397.,
    triggers = [],
    )
    
data_Run2012D_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2012D_PromptReco_v1',
    files = [],
    intLumi = 7274.,
    triggers = [],
    )



data_2012A = [
    data_Run2012A_PromptReco_v1
    ]

data_2012B = [
    data_Run2012B_PromptReco_v1
    ]

data_2012C = [
    data_Run2012C_PromptReco_v1,
    data_Run2012C_PromptReco_v2
    ]

data_2012D = [
    data_Run2012D_PromptReco_v1,
    ]

data_2012 = data_2012A + data_2012B + data_2012C + data_2012D
#data_2012 = data_2012C
