##########################################################
##       CONFIGURATION FOR EXO MONOJET TREES            ##
## skim condition:   MET > 200 GeV                      ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.MonoXAnalysis.analyzers.dmCore_modules_cff import * 
from PhysicsTools.Heppy.analyzers.objects.METAnalyzer import *

# Redefine what I need

# --- MONOJET SKIMMING ---
monoJetSkim.metCut = 200
monoJetSkim.jetPtCuts = []

# --- W->munu control sample SKIMMING ---
monoJetCtrlLepSkim.minLeptons = 0

# run miniIso
lepAna.doMiniIsolation = True
lepAna.packedCandidates = 'packedPFCandidates'
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.miniIsolationVetoLeptons = None # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
## will become miniIso perhaps?
#lepAna.loose_muon_isoCut     = lambda muon : muon.relIso03 < 10.5
#lepAna.loose_electron_isoCut = lambda electron : electron.relIso03 < 10.5
    

# switch off slow photon MC matching
photonAna.do_mc_match = False


##------------------------------------------
##  TOLOLOGIAL VARIABLES: RAZOR
##------------------------------------------
from PhysicsTools.Heppy.analyzers.eventtopology.RazorAnalyzer import RazorAnalyzer
monoXRazorAna = cfg.Analyzer(
    RazorAnalyzer, name = 'RazorAnalyzer',
    doOnlyDefault = False
    )

##------------------------------------------
##  TOLOLOGIAL VARIABLES: MT2
##------------------------------------------
from CMGTools.TTHAnalysis.analyzers.ttHTopoVarAnalyzer import ttHTopoVarAnalyzer
ttHTopoJetAna = cfg.Analyzer(
    ttHTopoVarAnalyzer, name = 'ttHTopoVarAnalyzer',
    doOnlyDefault = True
    )

from PhysicsTools.Heppy.analyzers.eventtopology.MT2Analyzer import MT2Analyzer
monoXMT2Ana = cfg.Analyzer(
    MT2Analyzer, name = 'MT2Analyzer',
    doOnlyDefault = False
    )

##------------------------------------------
##  TOLOLOGIAL VARIABLES: ALPHAT
##------------------------------------------
from CMGTools.TTHAnalysis.analyzers.ttHAlphaTVarAnalyzer import ttHAlphaTVarAnalyzer
ttHAlphaTAna = cfg.Analyzer(
    ttHAlphaTVarAnalyzer, name = 'ttHAlphaTVarAnalyzer',
    )

from CMGTools.TTHAnalysis.analyzers.ttHAlphaTControlAnalyzer import ttHAlphaTControlAnalyzer
ttHAlphaTControlAna = cfg.Analyzer(
    ttHAlphaTVarAnalyzer, name = 'ttHAlphaTControlAnalyzer',
    )
##-----------------------------------------------
##  TOLOLOGIAL VARIABLES: MONOJET SPECIFIC ONES
##-----------------------------------------------
from CMGTools.MonoXAnalysis.analyzers.monoJetVarAnalyzer import monoJetVarAnalyzer
monoJetVarAna = cfg.Analyzer(
    monoJetVarAnalyzer, name = 'monoJetVarAnalyzer',
    )

##------------------------------------------
# Event Analyzer for monojet 
##------------------------------------------
from CMGTools.MonoXAnalysis.analyzers.monoJetEventAnalyzer import monoJetEventAnalyzer
MonoJetEventAna = cfg.Analyzer(
    monoJetEventAnalyzer, name="monoJetEventAnalyzer",
    minJets25 = 0,
    )


from CMGTools.MonoXAnalysis.analyzers.treeProducerDarkMatterMonoJet import * 
## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerDarkMatterMonoJet',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     PDFWeights = PDFWeights,
     globalVariables = dmMonoJet_globalVariables,
     globalObjects = dmMonoJet_globalObjects,
     collections = dmMonoJet_collections,
)

## histo counter
# dmCoreSequence.insert(dmCoreSequence.index(skimAnalyzer),
#                       dmCounter)

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.MonoXAnalysis.samples.samples_monojet import triggers_monojet
triggerFlagsAna.triggerBits = {
    'MonoJet' : triggers_monojet,
}

from CMGTools.MonoXAnalysis.samples.samples_monojet import *
signalSamples = MonojetSignalSamples
backgroundSamples =  WJetsToLNuHT + ZJetsToNuNuHT + SingleTop + [TTJets] + DYJetsM50HT + GJetsHT + QCDHT
selectedComponents = backgroundSamples + signalSamples

#-------- SEQUENCE
sequence = cfg.Sequence(dmCoreSequence+[
    monoXRazorAna,
    monoXMT2Ana,
    ttHFatJetAna,
    ttHAlphaTAna,
    ttHAlphaTControlAna,
    monoJetVarAna,
    MonoJetEventAna,
    treeProducer,
    ])

# -- fine splitting, for some private MC samples with a single file
#for comp in selectedComponents:
#    comp.splitFactor = 1
#    comp.fineSplitFactor = 40

#-------- HOW TO RUN ----------- 
from PhysicsTools.HeppyCore.framework.heppy import getHeppyOption
#test = getHeppyOption('test')
test = '1'
if test == '1':
    #comp = GJets_fixPhoton
    monoJetSkim.metCut = 0
    for comp in GJets_RelVal:
        #comp.files = comp.files[:1]
        #comp.files = [ '/afs/cern.ch/work/a/avartak/public/dmVM10.root' ]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
        if not getHeppyOption('single'):
            comp.fineSplitFactor = 4
            selectedComponents = [ comp ]
elif test == '2':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
elif test == 'EOS':
    comp = DYJetsToLL_M50#TTJets
    comp.files = comp.files[:1]
    if getHeppyOption('Wigner'):
        print "Will read from WIGNER"
        comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0432E62A-7A6C-E411-87BB-002590DB92A8.root' ]
    else:
        print "Will read from CERN Meyrin"
        comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/10000/F675C068-5E6C-E411-B915-0025907DC9AC.root' ]
    os.system("/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select fileinfo "+comp.files[0].replace("root://eoscms//","/"))
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    selectedComponents = [ comp ]
elif test == 'SingleMu':
    comp = SingleMu
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    selectedComponents = [ comp ]
elif test == '5':
    for comp in selectedComponents:
        comp.files = comp.files[:5]
        comp.splitFactor = 1
        comp.fineSplitFactor = 5
elif test == '6':
    # test data
    comp = DoubleElectronAB; comp.name = "dataSamplesAll"
    comp.triggers = []
    jetAna.recalibrateJets = False 
    jetAna.smearJets       = False 
    comp.files = [ '/afs/cern.ch/work/e/emanuele/monox/heppy/CMSSW_7_2_3_patch1/src/step5.root' ]
    comp.isMC = False
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    monoJetSkim.metCut = 0
    selectedComponents = [ comp ]
elif test == 'monojet-synch': # sync
    #eventSelector.toSelect = [ 11809 ]
    #sequence = cfg.Sequence([eventSelector] + dmCoreSequence+[ ttHEventAna, treeProducer, ])
    jetAna.recalibrateJets = False 
    jetAna.smearJets       = False 
    comp = Monojet_M_10_V;
    comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0432E62A-7A6C-E411-87BB-002590DB92A8.root' ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    monoJetSkim.metCut = 0
    selectedComponents = [ comp ]
elif test == 'SR':
    selectedComponents = backgroundSamples + signalSamples
    monoJetSkim.metCut = 200
    monoJetCtrlLepSkim.minLeptons = 0
    for comp in selectedComponents:
        comp.splitFactor = 350

## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='treeProducerDarkMatterMonoJet/tree.root',
    option='recreate'
    )    
outputService.append(output_service)

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events)


