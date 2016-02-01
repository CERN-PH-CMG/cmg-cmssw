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
    trigMatcher1Mu, trigMatcher1El,
    twoLeptonAnalyzerTnP, 
    twoLeptonEventSkimmerTnP, 
    twoLeptonTreeProducerTnP 
])

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *
dataSamples = [ d for d in dataSamples if 'Single' in d.name and "25ns" in d.name ] 
for d in dataSamples:
    d.triggers = triggers_1mu if 'Muon' in d.name else triggers_1e
    d.vetoTriggers = []
    d.splitFactor = max((len(d.files)+4)/7,1)
configureSplittingFromTime([d for d in dataSamples if "Silver" not in d.name], 10.0, 2)
    
mcSamples = [ DYJetsToLL_LO_M50, DYJetsToLL_M50 ]
for d in mcSamples:
    d.triggers = triggers_1mu + triggers_1e
    d.vetoTriggers = []
    d.splitFactor = max(len(d.files)/5,1)
configureSplittingFromTime(mcSamples, 25.0, 2)

selectedComponents = dataSamples + mcSamples
printSummary(selectedComponents)
if True: autoAAA(selectedComponents)
doECalCorrections(era="25ns")


from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
lepAna.do_mc_match_photons = False
if test in ("1","1M","1E"):
    trigMatcher1Mu.verbose = True
    trigMatcher1El.verbose = True
    component = { "1":DYJetsToLL_LO_M50, "1M":SingleMuon_Run2015D_PromptV4_25ns, "1E":SingleElectron_Run2015D_PromptV4_25ns }[test]
    if not component.isMC: redefineRunRange([component],[258214,258214])
    selectedComponents = doTest1( component, sequence=sequence )
elif test == "1M":
    selectedComponents = doTest1( DYJetsToLL_LO_M50 , sequence=sequence )
elif test in ('2','3','5'):
    doTestN(test,selectedComponents)

config = autoConfig(selectedComponents, sequence)
