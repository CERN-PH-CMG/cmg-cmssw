##########################################################
##       CONFIGURATION FOR HZZ4L TREES                  ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import * 


#-------- SAMPLES AND TRIGGERS -----------

#-------- SEQUENCE
from CMGTools.HToZZ4L.samples.samples_13TeV_PHYS14 import *

selectedComponents = []
sequence = cfg.Sequence(hzz4lCoreSequence)

for comp in mcSamples:
    comp.triggers = triggers_multilep
    comp.vetoTriggers = []

from PhysicsTools.HeppyCore.framework.heppy import getHeppyOption
test = getHeppyOption('test')
if test == "1":
    comp = GGHZZ4L
    comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3295EF7C-2070-E411-89C4-7845C4FC35DB.root ' ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1 if getHeppyOption('single') else 5
    selectedComponents = [ comp ]
elif test == '2':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
elif test == 'Debug':
    comp = GGHZZ4L
    comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/GluGluToHToZZTo4L_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/3295EF7C-2070-E411-89C4-7845C4FC35DB.root ' ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    selectedComponents = [ comp ]
    eventSelector.toSelect = [ (1,1,2), (1,1,53), (1,1,69), (1,1,75), (1,1,79) ] 
    if getHeppyOption('events'):
        eventSelector.toSelect = [ eval("("+x.replace(":",",")+")") for x in getHeppyOption('events').split(",") ]
        print "Will select events ",eventSelector.toSelect
    sequence = cfg.Sequence([eventSelector] + hzz4lCoreSequence)
elif test == 'CR':
    comp = GGHZZ4L
    comp.files = [
        '/afs/cern.ch/user/g/gortona/work/public/miniAODPhys14/DYJetsToLL_M-50_13TeV_3leptons.root'
    ]
    comp.fineSplitFactor = 1 if getHeppyOption('single') else 4
    selectedComponents = [ comp ]
    if getHeppyOption('events'):
        eventSelector.toSelect = [ eval("("+x.replace(":",",")+")") for x in getHeppyOption('events').split(",") ]
        sequence = cfg.Sequence([eventSelector] + hzz4lCoreSequence)
        print "Will select events ",eventSelector.toSelect
elif test == 'Cat':
    comp = GGHZZ4L
    comp.files = [
        'root://eoscms//eos/cms/store/cmst3/group/susy/gpetrucc/13TeV/Phys14DR/MINIAODSIM/ggH_JHU_125/ggH_JHU_125.MINIAODSIM00.root',
        'root://eoscms//eos/cms/store/cmst3/group/susy/gpetrucc/13TeV/Phys14DR/MINIAODSIM/VBF_JHU_125/VBF_JHU_125.MINIAODSIM00.root',
        'root://eoscms//eos/cms/store/cmst3/group/susy/gpetrucc/13TeV/Phys14DR/MINIAODSIM/WminusH_JHU_125/WminusH_JHU_125.MINIAODSIM00.root',
        'root://eoscms//eos/cms/store/cmst3/group/susy/gpetrucc/13TeV/Phys14DR/MINIAODSIM/ZH_JHU_125/ZH_JHU_125.MINIAODSIM00.root',
        'root://eoscms//eos/cms/store/cmst3/group/susy/gpetrucc/13TeV/Phys14DR/MINIAODSIM/ttH_JHU_125/ttH_JHU_125.MINIAODSIM00.root',
    ]
    comp.splitFactor = 1 if getHeppyOption('single') else 99
    comp.fineSplitFactor = 1
    selectedComponents = [ comp ]
    if getHeppyOption('events'):
        eventSelector.toSelect = [ eval("("+x.replace(":",",")+")") for x in getHeppyOption('events').split(",") ]
        sequence = cfg.Sequence([eventSelector] + hzz4lCoreSequence)
        print "Will select events ",eventSelector.toSelect


# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events)


