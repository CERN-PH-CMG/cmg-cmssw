import copy

from CMGTools.Production.getFiles import getFiles
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.proto.samples.run2012.data import *
from CMGTools.H2TauTau.proto.samples.run2012.embed import *
from CMGTools.H2TauTau.proto.samples.run2012.ewk import *
from CMGTools.H2TauTau.proto.samples.run2012.diboson import *
from CMGTools.H2TauTau.proto.samples.run2012.higgs import *

from CMGTools.H2TauTau.proto.samples.run2012.triggers_tauEle import data_triggers, mc_triggers

MC = copy.copy( mc_higgs )
HiggsVBF125Small = copy.deepcopy( HiggsVBF125 )
HiggsVBF125Small.name = 'HiggsVBF125Small'

MC.append( HiggsVBF125Small )
for sam in MC:
    sam.triggers = mc_triggers
    
allsamples = copy.copy(MC)

HiggsVBF125Small.files = getFiles('/H2TAUTAU/Sync/2012/VBF/AOD/PAT_CMG_V5_5_1/TAUELE_ColinAug08',
                                  'cbern', '.*root')

HiggsVBF125.files = getFiles('/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_5_1/TAUELE_ColinAug13',
                             'cbern', '.*root')


