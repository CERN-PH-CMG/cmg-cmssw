##########################################################
##       GENERIC SUSY TREE PRODUCTION CONFIG.           ##
## no skim is applied in this configuration, as it is   ##
## meant only to check that all common modules run ok   ##
##########################################################


import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

#Load all common analyzers
from CMGTools.VVResonances.analyzers.core_cff import * 

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.VVResonances.samples.loadSamples import *

selectedComponents = mcSamples+dataSamples
#selectedComponents = dataSamples


#import pdb;pdb.set_trace()

#-------- Analyzer
from CMGTools.VVResonances.analyzers.tree_cff import * 

#-------- SEQUENCE

sequence = cfg.Sequence(coreSequence+[vvSkimmer,vvTreeProducer])


from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *


triggerFlagsAna.triggerBits ={
    "ISOMU":triggers_1mu_iso,
    "MU":triggers_1mu_noniso,
    "ISOELE":triggers_1e,
    "ELE":triggers_1e_noniso,
    "HT800":triggers_HT800,
    "HT900":triggers_HT900,
    "JJ":triggers_dijet_fat,  
    "MET90":triggers_met90_mht90+triggers_metNoMu90_mhtNoMu90,
    "MET120":triggers_metNoMu120_mhtNoMu120
}


#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    selectedComponents = [VBF_RadionToZZ_narrow_4500]
    for c in selectedComponents:
        c.files = c.files[:1]
        c.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component).
    selectedComponents = [BulkGravToWW_narrow_2500]
    for comp in selectedComponents:
        comp.splitFactor = 1
#        comp.files = comp.files[:1]
elif test==3:    
    # test all components (1 thread per component).
    selectedComponents = [DYJetsToLL_M50_HT600toInf]
    for comp in selectedComponents:
        comp.splitFactor = 1

elif test==4:    
    # test all components (1 thread per component).
    selectedComponents = [RSGravToWWToLNQQ_kMpl01_4500]
    for comp in selectedComponents:
        comp.splitFactor = 1

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = event_class)

