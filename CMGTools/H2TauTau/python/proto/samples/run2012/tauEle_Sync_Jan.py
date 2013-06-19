import copy

from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauEle import data_triggers, mc_triggers

MC_list = copy.copy( mc_higgs )
for sam in MC_list:
    sam.triggers = mc_triggers
    
allsamples = copy.copy(MC_list)

# full new VBF recipe
## HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PATCMG_V5_8_0_Test/TAUELE_MVAMET_ColinSep1',
##                              'cbern', '.*root')

# test 3
pat = ''
HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0/TAUELE_SVFit_Jun18_steggema/',
                             'steggema', '.*root')


