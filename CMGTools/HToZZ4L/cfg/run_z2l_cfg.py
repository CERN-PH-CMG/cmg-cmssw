##########################################################
##       CONFIGURATION FOR HZZ4L TREES                  ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import * 
from CMGTools.HToZZ4L.analyzers.hzz4lExtra_modules_cff import * 
from CMGTools.HToZZ4L.tools.configTools import * 

#-------- SEQUENCE
sequence = cfg.Sequence(hzz4lPreSequence +  [ fastSkim2L ] + hzz4lObjSequence + [
    twoLeptonAnalyzer, 
    twoLeptonEventSkimmer, 
    twoLeptonTreeProducer 
])

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *
dataSamples = [ ] # d for d in dataSamples if "DoubleMu" in d.name ]
for d in dataSamples:
    d.triggers = triggers_mumu if 'Muon' in d.name else triggers_ee
    d.vetoTriggers = []
    d.splitFactor = (len(d.files)+4)/7
    
mcSamples = [ DYJetsToLL_LO_M50 ]
for d in mcSamples:
    d.triggers = triggers_mumu + triggers_ee
    d.vetoTriggers = []
    d.splitFactor = len(d.files)/1

selectedComponents = dataSamples + mcSamples
prescaleComponents(mcSamples, 5)
printSummary(selectedComponents)

#redefineRunRange(selectedComponents,[258214,258214])
if True: autoAAA(selectedComponents)
doECalCorrections(era="50ns")
doKalmanMuonCorrections()

from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
if test in ("1","1M","1E"):
    component = { "1":DYJetsToLL_LO_M50, "1M":DoubleMuon_Run2015D_PromptV4_25ns, "1E":DoubleEG_Run2015D_PromptV4_25ns }[test]
    if not component.isMC: redefineRunRange([component],[258214,258214])
    selectedComponents = doTest1( component, sequence=sequence )
elif test in ('2','3','5'):
    doTestN(test,selectedComponents)

config = autoConfig(selectedComponents, sequence)
