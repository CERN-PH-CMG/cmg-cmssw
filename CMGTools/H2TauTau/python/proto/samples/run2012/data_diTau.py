import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg

H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 



data_Run2012A_22Jan2013_v1 = cfg.DataComponent(
    name = 'data_Run2012A_22Jan2013_v1',
    files = [],
    intLumi = 96.404 + 396.906 + 395.991,
    triggers = [],
    )
data_Run2012B_22Jan2013_v1 = cfg.DataComponent(
    name = 'data_Run2012B_22Jan2013_v1',
    files = [],
    intLumi = 4429.,
    triggers = [],
    )
data_Run2012C_22Jan2013_v1 = cfg.DataComponent(
    name = 'data_Run2012C_22Jan2013_v1',
    files = [],
    intLumi = 1783. + 5087. + 282.692,
    triggers = [],
    )
data_Run2012D_22Jan2013_v1 = cfg.DataComponent(
    name = 'data_Run2012D_22Jan2013_v1',
    files = [],
    intLumi = 7318.,
    triggers = [],
    )



data_parked_2012 = [ data_Run2012B_22Jan2013_v1, 
                     data_Run2012C_22Jan2013_v1,
                     data_Run2012D_22Jan2013_v1, 
                     data_Run2012A_22Jan2013_v1,
                   ]















data_Run2012A_13Jul2012_v1 = cfg.DataComponent(
    name = 'data_Run2012A_13Jul2012_v1',
    files = [],
    intLumi = 806.194,
    triggers = [],
    )

data_Run2012A_recover_06Aug2012_v1 = cfg.DataComponent(
    name = 'data_Run2012A_recover_06Aug2012_v1',
    files = [],
    intLumi = 82.136,
    triggers = [],
    )

data_Run2012B_13Jul2012_v1 = cfg.DataComponent(
    name = 'data_Run2012B_13Jul2012_v1',
    files = [],
    intLumi = 4420.,
    triggers = [],
    )

data_Run2012C_24Aug2012_v1 = cfg.DataComponent(
    name = 'data_Run2012C_24Aug2012_v1',
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
    data_Run2012A_13Jul2012_v1,
    data_Run2012A_recover_06Aug2012_v1,
    ]

data_2012B = [
    data_Run2012B_13Jul2012_v1
    ]

data_2012C = [
    data_Run2012C_24Aug2012_v1,
    data_Run2012C_PromptReco_v2
    ]

data_2012D = [
    data_Run2012D_PromptReco_v1,
    ]


data_2012        = data_2012A + data_2012B + data_2012C + data_2012D
