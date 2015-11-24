##########################################################
##       CONFIGURATION FOR HZZ4L TREES                  ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import * 
from CMGTools.HToZZ4L.tools.configTools import * 


#-------- SAMPLES AND TRIGGERS -----------

#-------- SEQUENCE
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *

selectedComponents = dataSamples

sequence = cfg.Sequence(hzz4lCoreSequence)

for comp in mcSamples:
    comp.triggers = triggers_any
    comp.vetoTriggers = []

printSummary(selectedComponents)

if True: autoAAA(selectedComponents)


from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
if test == "1":
    selectedComponents = doTest1( ZZTo4L, sequence=sequence )
elif test in ('2','3','5'):
    doTestN(test,selectedComponents)
elif test == "data":
    selectedComponents = doTest1( DoubleMuon_Run2015D_05Oct2015_25ns, 
                            url = 'root://eoscms//eos/cms/store/data/Run2015D/DoubleMuon/MINIAOD/05Oct2015-v1/40000/006906D0-646F-E511-B01E-0025905A6118.root',
                            sequence=sequence )
elif test=="sync":
    comp = GGHZZ4L
    comp.name = 'HZZ4L'
    comp.files = [ 'root://eoscms.cern.ch//eos/cms'+X for X in (
        '/store/mc/RunIISpring15MiniAODv2/VBF_HToZZTo4L_M125_13TeV_powheg_JHUgen_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/3E964C5D-1D6E-E511-8B9A-0050560207C5.root',
        '/store/mc/RunIISpring15MiniAODv2/WminusH_HToZZTo4L_M125_13TeV_powheg-minlo-HWJ_JHUgen_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/D8CA6B54-056F-E511-BB1A-02163E014CE3.root',
        '/store/mc/RunIISpring15MiniAODv2/WplusH_HToZZTo4L_M125_13TeV_powheg-minlo-HWJ_JHUgen_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/D22BEE88-C26D-E511-B330-002590A81EF0.root',
        '/store/mc/RunIISpring15MiniAODv2/ttH_HToZZ_4LFilter_M125_13TeV_powheg_JHUgen_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/80000/84F62DD7-1475-E511-9F59-009C02AB98A6.root'
    )]
    if getHeppyOption('turbo'):
        comp.fineSplitFactor = int(getHeppyOption('turbo'))
        comp.splitFactor = 1
    else:
        comp.fineSplitFactor = 1
        comp.splitFactor = 1 if getHeppyOption('single') else 5
    selectedComponents = [ comp ]
    if getHeppyOption('events'): insertEventSelector(sequence)

config = autoConfig(selectedComponents, sequence)
