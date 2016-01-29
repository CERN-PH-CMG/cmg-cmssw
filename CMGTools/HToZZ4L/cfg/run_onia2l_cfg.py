#########################################################
##       CONFIGURATION FOR HZZ4L TREES                  ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg
import re

#Load all analyzers
from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import * 
from CMGTools.HToZZ4L.analyzers.hzz4lExtra_modules_cff import * 
from CMGTools.HToZZ4L.tools.configTools import * 


#-------- SEQUENCE
sequence = cfg.Sequence(hzz4lPreSequence +  [ fastSkim2L ] + hzz4lObjSequence + [
    twoLeptonAnalyzerOnia, 
    twoLeptonEventSkimmerOnia, 
    twoLeptonTreeProducerOnia 
])

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *
dataSamples = dataSamples_onia
for d in dataSamples:
    d.triggers = triggers_jpsi2mu if 'Charmonium' in d.name else triggers_upsilon2mu
    d.vetoTriggers = []
    d.splitFactor = len(d.files)/3
    
mcSamples = [ JpsiToMuMuPt8, UpsToMuMuPt6 ]
UpsToMuMuPt6.files = UpsToMuMuPt6.files[:len(UpsToMuMuPt6.files)/2+1]
for d in mcSamples:
    d.triggers = triggers_jpsi2mu + triggers_upsilon2mu
    d.vetoTriggers = []
    d.splitFactor = len(d.files)/4

selectedComponents = mcSamples

if True: autoAAA(selectedComponents)
 

from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
if test == "1":
    selectedComponents = doTest1( Charmonium_Run2015D_PromptV4_25ns,
                                  "root://eoscms//eos/cms/store/data/Run2015D/Charmonium/MINIAOD/PromptReco-v4/000/258/656/00000/42F839D8-7970-E511-A33C-02163E013678.root",
                                  sequence = sequence, cache = True)
elif test == "1M":
    selectedComponents = doTest1( JpsiToMuMuPt8, sequence=sequence )
elif test in ('2','3','5'):
    doTestN(test,selectedComponents)

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
