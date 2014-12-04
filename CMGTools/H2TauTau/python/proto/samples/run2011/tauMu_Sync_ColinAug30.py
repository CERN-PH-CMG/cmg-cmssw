import copy

from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.proto.samples.run2011.data import *
from CMGTools.H2TauTau.proto.samples.run2011.embed import *
# from CMGTools.H2TauTau.proto.samples.run2011.ewk import *
# from CMGTools.H2TauTau.proto.samples.run2011.diboson import *
from CMGTools.H2TauTau.proto.samples.run2011.higgs import *

from CMGTools.H2TauTau.proto.samples.run2011.triggers_tauMu import mc_triggers

MC = copy.copy( mc_higgs )
HiggsVBF125Small = copy.deepcopy( HiggsVBF125 )
HiggsVBF125Small.name = 'HiggsVBF125Small'

MC.append( HiggsVBF125Small )
for sam in MC:
    sam.triggers = mc_triggers
    
allsamples = copy.copy(MC)

HiggsVBF125Small.files = []

HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_5_1_Test3/TAUMU_NewSVFit_ColinSep3',
                             'cbern', '.*root')



