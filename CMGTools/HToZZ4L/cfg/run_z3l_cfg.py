##########################################################
##       CONFIGURATION FOR HZZ4L TREES                  ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import * 
from CMGTools.HToZZ4L.analyzers.hzz4lExtra_modules_cff import * 
from CMGTools.HToZZ4L.tools.configTools import * 



#-------- SEQUENCE
sequence = cfg.Sequence(hzz4lPreSequence +  [ fastSkim3L ] + hzz4lObjSequence + [
    twoLeptonAnalyzer, 
    twoLeptonEventSkimmer, 
    threeLeptonAnalyzer, 
    threeLeptonEventSkimmer, 
    threeLeptonTreeProducer 
])

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *
dataSamples = [ d for d in dataSamples if 'Double' in d.name and not ( "V4" in d.name and "Silver" not in d.name ) ] 
for d in dataSamples:
    d.triggers = triggers_mumu if 'Muon' in d.name else triggers_ee
    d.vetoTriggers = []
    d.splitFactor = max((len(d.files)+8)/12,1)
    
mcSamples = [] #[ TTLep ] + SingleTop
for d in mcSamples:
    d.triggers = triggers_mumu + triggers_ee
    d.vetoTriggers = []
    d.splitFactor = max(len(d.files)/(1 if "WZ" in d.name else 10),1)
cropToLumi( mcSamples, 10.0 )
configureSplittingFromTime([TTLep], 20., 0.3)
configureSplittingFromTime(SingleTop, 10., 0.3)


selectedComponents = dataSamples + mcSamples
printSummary(selectedComponents)
autoAAA(selectedComponents)


from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
if test == "1":
    selectedComponents = doTest1( WZTo3LNu, sequence=sequence )
elif test == "1DY":
    selectedComponents = doTest1( DYJetsToLL_LO_M50 , sequence=sequence )
elif test in ('2','3','5'):
    doTestN(test,selectedComponents)

config = autoConfig(selectedComponents, sequence)
