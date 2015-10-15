##########################################################
##       CONFIGURATION FOR HZZ4L TREES                  ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import * 
from CMGTools.HToZZ4L.analyzers.TwoLeptonAnalyzer import TwoLeptonAnalyzer

twoLeptonAnalyzer = cfg.Analyzer(
    TwoLeptonAnalyzer, name="twoLeptonAnalyzer",
    #attachFsrToGlobalClosestLeptonOnly = True
)

twoLeptonEventSkimmer = cfg.Analyzer(
    FourLeptonEventSkimmer, name="twoLeptonEventSkimmer",
    required = ['bestIsoZ']
)


twoLeptonTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='twoLeptonTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     globalVariables = hzz_globalVariables, # rho, nvertices, njets
     globalObjects = hzz_globalObjects, # met
     collections = {
         "bestIsoZ"        : NTupleCollection("z",   ZType, 1, help="Four Lepton Candidates"),    
         "selectedLeptons" : NTupleCollection("Lep", leptonTypeHZZ, 10, help="Leptons after the preselection"),
         "cleanJets"       : NTupleCollection("Jet", jetTypeExtra, 10, help="Cental jets after full selection and cleaning, sorted by pt"),
         #"fsrPhotonsNoIso" : NTupleCollection("FSR", fsrPhotonTypeHZZ, 10, help="Photons for FSR recovery (isolation not applied)"),
     },
     defaultFloatType = 'F',
)


#-------- SEQUENCE
sequence = cfg.Sequence([
    skimAnalyzer,
    jsonAna,
    triggerAna,
    fastSkim2L,
    genAna,
    pileUpAna,
    vertexAna,
    lepAna,
    eleMuClean,
    jetAna,
    metAna,
    triggerFlagsAna,
    #fsrPhotonMaker,
    twoLeptonAnalyzer, 
    twoLeptonEventSkimmer, 
    twoLeptonTreeProducer 
])

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *
dataSamples = [ d for d in dataSamples if 'Double' in d.name ]
for d in dataSamples:
    d.triggers = triggers_mumu if 'Muon' in d.name else triggers_ee
    d.vetoTriggers = []
    d.splitFactor = len(d.files)/4
    
mcSamples = [ DYJetsToLL_M50_v2 ]
for d in mcSamples:
    d.triggers = triggers_mumu + triggers_ee
    d.vetoTriggers = []
    d.splitFactor = len(d.files)/2
    
selectedComponents = dataSamples + mcSamples

if True:
    import re
    from CMGTools.Production import changeComponentAccessMode
    from CMGTools.Production.localityChecker import LocalityChecker
    tier2Checker = LocalityChecker("T2_CH_CERN", datasets="/*/*/MINIAOD*")
    for comp in selectedComponents:
        if not hasattr(comp,'dataset'): continue
        if not re.match("/[^/]+/[^/]+/MINIAOD(SIM)?", comp.dataset): continue
        if "/store/" not in comp.files[0]: continue
        if re.search("/store/(group|user|cmst3)/", comp.files[0]): continue
        if not tier2Checker.available(comp.dataset):
            print "Dataset %s is not available, will use AAA" % comp.dataset
            changeComponentAccessMode.convertComponent(comp, "root://cms-xrd-global.cern.ch/%s")


from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
if test == "1":
    comp = DYJetsToLL_M50
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1 if getHeppyOption('single') else 5
    selectedComponents = [ comp ]
    if getHeppyOption('events'):
        eventSelector.toSelect = [ eval("("+x.replace(":",",")+")") for x in getHeppyOption('events').split(",") ]
        sequence = cfg.Sequence([eventSelector] + hzz4lCoreSequence)
        print "Will select events ",eventSelector.toSelect
elif test == '2':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
elif test == '3':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 3


# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
EOSEventsWithDownload.aggressive = 2 # always fetch if running on Wigner
if getHeppyOption("nofetch") or getHeppyOption("isCrab"):
    event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = event_class)

