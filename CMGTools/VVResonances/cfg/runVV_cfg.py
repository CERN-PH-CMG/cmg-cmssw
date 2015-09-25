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

selectedComponents = mcSamples+dataSamples_50ns
#selectedComponents = dataSamples

#-------- Analyzer
from CMGTools.VVResonances.analyzers.tree_cff import * 

#-------- SEQUENCE

sequence = cfg.Sequence(coreSequence+[vvSkimmer,vvTreeProducer])


from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *


triggerFlagsAna.triggerBits ={
    "ISOMU":triggers_1mu_iso,
    "ISOMU_50ns":triggers_1mu_iso_50ns,
    "MU":triggers_1mu_noniso,
    "ISOELE":triggers_1e,
    "ISOELE_50ns":triggers_1e_50ns,
    "ELE":triggers_1e_noniso,
    "HT800":triggers_HT800,
    "HT900":triggers_HT900,
    "JJ":triggers_dijet_fat  
}


#-------- HOW TO RUN
test = 3
if test==1:
    # test a single component, using a single thread.
    selectedComponents = [signalSamples[7]]
    for c in selectedComponents:
        c.files = c.files[:1]
        c.splitFactor = 1

elif test==2:    
    # test all components (1 thread per component).
    selectedComponents = [dataSamples[0]]
    for comp in selectedComponents:
        comp.splitFactor = 1
#        comp.files = comp.files[:1]

elif test==3:
    # test a single component, using a single thread.
    selectedComponents = [signalSamples[0]]
    for c in selectedComponents:
        c.files = c.files[:1]
        c.splitFactor = 1





## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='vvTreeProducer/tree.root',
    option='recreate'
    )    
outputService.append(output_service)



from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
event_class = Events
if getHeppyOption("nofetch"):
    event_class = Events 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = event_class)

