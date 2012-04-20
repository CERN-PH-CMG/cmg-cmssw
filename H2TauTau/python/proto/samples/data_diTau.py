import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from   CMGTools.H2TauTau.proto.samples.getFiles import getFiles

H2TauTauPackage = '/'.join( [ os.environ['CMSSW_BASE'],
                              'src/CMGTools/H2TauTau' ] ) 


data_Run2011A_May10ReReco_v1 = cfg.DataComponent(
    name = 'data_Run2011A_May10ReReco_v1',
    files = [],
    intLumi = 168.597,
    triggers = [],
    json = '{H2TauTauPackage}/json/finalTauPlusXMay.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_PromptReco_v4 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v4',
    files = [],
    intLumi = 929.748,
    triggers = [],
    json = '{H2TauTauPackage}/json/finalTauPlusXv4.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_05Aug2011_v1 = cfg.DataComponent(
    name = 'data_Run2011A_05Aug2011_v1',
    files = [],
    intLumi = 373.349,
    triggers = [],
    json = '{H2TauTauPackage}/json/finalTauPlusXAug5.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011A_PromptReco_v6 = cfg.DataComponent(
    name = 'data_Run2011A_PromptReco_v6',
    files = [],
    intLumi = 658.886,
    triggers = [],
    json = '{H2TauTauPackage}/json/finalTauPlusXOct3.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )

data_Run2011B_PromptReco_v1 = cfg.DataComponent(
    name = 'data_Run2011B_PromptReco_v1',
    files = [],
    intLumi = 2511.0,
    triggers = [],
    json = '{H2TauTauPackage}/json/finalTauPlusX2011B.txt'.format(H2TauTauPackage=H2TauTauPackage)
    )


data_2011A = [
    data_Run2011A_May10ReReco_v1,
    data_Run2011A_PromptReco_v4,
    data_Run2011A_05Aug2011_v1,
    data_Run2011A_PromptReco_v6
    ]

data_2011B = [
    #data_Run2011B_PromptReco_v1
    ]

data_2011 = copy.copy( data_2011A )
data_2011.extend( data_2011B ) 
