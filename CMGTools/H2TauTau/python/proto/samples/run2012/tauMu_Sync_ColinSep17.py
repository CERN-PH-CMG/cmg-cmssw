import copy

from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauMu import data_triggers, mc_triggers

MC = copy.copy( mc_higgs )
HiggsVBF125Small = copy.deepcopy( HiggsVBF125 )
HiggsVBF125Small.name = 'HiggsVBF125Small'

MC.append( HiggsVBF125Small )
for sam in MC:
    sam.triggers = mc_triggers
    
allsamples = copy.copy(MC)


# HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_6_0_B/TAUMU_NewSVFit_Colin17Sep',
#                             'cbern', '.*root')

HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B/TAUMU_NewSVFit_Colin17Sep',
                             'cbern', '.*root')



