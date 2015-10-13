##########################################################
##       CONFIGURATION FOR SUSY MULTILEPTON TREES       ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################

import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import * 
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 0
ttHLepSkim.maxLeptons = 999

from CMGTools.TTHAnalysis.analyzers.ttHGenLevelOnlyStudy import * 
ttHGenLevel = cfg.Analyzer(
    ttHGenLevelOnlyStudy, name='ttHGenLevelOnlyStudy',
    muon_pt_min = 5.,
    electron_pt_min = 7.,
)


## Tree Producer
from CMGTools.TTHAnalysis.analyzers.treeProducerSusyGenLevelOnly import * 
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyGenLevelOnly',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     globalVariables = treeProducerSusyGenLevelOnly_globalVariables,
     globalObjects = treeProducerSusyGenLevelOnly_globalObjects,
     collections = treeProducerSusyGenLevelOnly_collections,
)

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.RootTools.samples.samples_13TeV_74X import kreator, TTZToLLNuNu, TTGJets
TTLL_m1to10_LO_NoMS  = kreator.makePrivateMCComponent('TTLL_m1to10_LO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m1to10_LO_NoMS_for74X', ['TTLL_m1to10_LO_NoMS_for74X.run%d.GEN.root' % i for i in (1,2,3,4,5,6,7,8)] )
TTLL_m1to10_LO_MS  = kreator.makePrivateMCComponent('TTLL_m1to10_LO_MS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m1to10_LO_MS_for74X', ['TTLL_m1to10_LO_MS_for74X.run%d.GEN.root' % i for i in (5,6,7,8)] )
TTLL_m1toInf_LO_NoMS  = kreator.makePrivateMCComponent('TTLL_m1toInf_LO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m1toInf_LO_NoMS_for74X', ['TTLL_m1toInf_LO_NoMS_for74X.run%d.GEN.root' % i for i in (9,)] )
TTLL_m1toInf_LO_MS  = kreator.makePrivateMCComponent('TTLL_m1toInf_LO_MS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m1toInf_LO_MS_for74X', ['TTLL_m1toInf_LO_MS_for74X.run%d.GEN.root' % i for i in (9,)] )
TTLL_m10toInf_LO_NoMS  = kreator.makePrivateMCComponent('TTLL_m10toInf_LO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m10toInf_LO_NoMS_for74X', ['TTLL_m10toInf_LO_NoMS_for74X.run%d.GEN.root' % i for i in (10,)] )
TTLL_m10toInf_LO_MS  = kreator.makePrivateMCComponent('TTLL_m10toInf_LO_MS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m10toInf_LO_MS_for74X', ['TTLL_m10toInf_LO_MS_for74X.run%d.GEN.root' % i for i in (10,)] )
TTLLJets_m1to10_LO_NoMS  = kreator.makePrivateMCComponent('TTLLJets_m1to10_LO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLLJets_m1to10_LO_NoMS_for74X', ['TTLLJets_m1to10_LO_NoMS_for74X.run%d.GEN.root' % i for i in (3,5,6,7,8,9,10,11)] )
TTLLJets_m1to10_LO_MS  = kreator.makePrivateMCComponent('TTLLJets_m1to10_LO_MS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLLJets_m1to10_LO_MS_for74X', ['TTLLJets_m1to10_LO_MS_for74X.run%d.GEN.root' % i for i in (3,9,10,11)] )
TTLLJets_m10toInf_LO_NoMS  = kreator.makePrivateMCComponent('TTLLJets_m10toInf_LO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLLJets_m10toInf_LO_NoMS_for74X', ['TTLLJets_m10toInf_LO_NoMS_for74X.run%d.GEN.root' % i for i in (12,13)] )
TTLL_m1toInf_NLO_NoMS  = kreator.makePrivateMCComponent('TTLL_m1toInf_NLO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m1toInf_NLO_NoMS_for74X', ['TTLL_m1toInf_NLO_NoMS_for74X.run%d.GEN.root' % i for i in (2,4,5,8,9,10,12)] )
#TTLL_m10toInf_NLO_NoMS  = kreator.makePrivateMCComponent('TTLL_m10toInf_NLO_NoMS', '/store/cmst3/group/susy/gpetrucc/13TeV/GEN/TTLL_m10toInf_NLO_NoMS_for74X', ['TTLL_m10toInf_NLO_NoMS_for74X.run%d.GEN.root' % i for i in (2,4,5,8,9,10,12)] )

TTZToLLNuNu_GEN = kreator.makeMCComponent("TTZToLLNuNu","/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/AODSIM", "CMS", ".*root", 0.2529, useAAA=True)


#selectedComponents = [  TTZToLLNuNu, TTGJets ]
#selectedComponents = [  TTZToLLNuNu_GEN ]
selectedComponents =  [ ]
## ---- LO
#selectedComponents += [  TTLL_m1to10_LO_NoMS, TTLL_m1to10_LO_MS ]
#selectedComponents += [  TTLL_m1toInf_LO_MS, TTLL_m1toInf_LO_NoMS ]
#selectedComponents += [  TTLL_m10toInf_LO_MS ] 
selectedComponents += [  TTLL_m10toInf_LO_NoMS ] # running
## ---- LO + MLM
#selectedComponents += [  TTLLJets_m1to10_LO_MS, TTLLJets_m1to10_LO_NoMS ] 
#selectedComponents += [   TTLLJets_m10toInf_LO_NoMS ] # running
## ---- NLO
#selectedComponents += [  TTLL_m1toInf_NLO_NoMS ]  
#selectedComponents += [  TTLL_m10toInf_NLO_NoMS ]  # running
for c in selectedComponents: c.splitFactor = 100
#-------- SEQUENCE

sequence = cfg.Sequence([
    skimAnalyzer,
    ttHGenLevel, 
    ttHLepSkim,
    treeProducer,
    ])


#-------- HOW TO RUN
test = getHeppyOption('test')
if test=='1':
    # test a single component, using a single thread.
    skimAnalyzer.verbose = True
    comp = TTLL_m1to10_LO_NoMS
    comp.files = comp.files[:1]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test=='2':    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:1]
elif test=='5':    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.fineSplitFactor = 4
        comp.files = comp.files[:5]
elif test=='5b':    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.fineSplitFactor = 2
        comp.files = comp.files[5:20]



from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='treeProducerSusyGenLevelOnly/tree.root',
    option='recreate'
    )    

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
EOSEventsWithDownload.aggressive = 2 # always fetch
if getHeppyOption("nofetch"):
    event_class = Events

config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [ output_service ],
                     events_class = event_class)
