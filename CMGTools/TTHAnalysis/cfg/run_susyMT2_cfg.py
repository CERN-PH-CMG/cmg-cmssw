import PhysicsTools.HeppyCore.framework.config as cfg


#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

# Comment this line if you want the diagnostic folders produced along with the output root file
cfg.Analyzer.nosubdir = True 


##------------------------------------------
## Redefine what I need
##------------------------------------------

#Vertex
vertexAna.keepFailingEvents = True # keep events with no good vertices

#Lepton
lepAna.loose_muon_dxy = 0.5
lepAna.loose_muon_dz  = 1.0
lepAna.loose_muon_relIso  = 0.15
lepAna.loose_electron_id  = "POG_Cuts_ID_2012_Veto"
lepAna.loose_electron_pt  = 5
lepAna.loose_electron_eta    = 2.4
lepAna.loose_electron_dxy    = 0.04
lepAna.loose_electron_dz     = 0.2
lepAna.loose_electron_relIso = 0.15
lepAna.loose_electron_lostHits = 999 # no cut
lepAna.inclusive_electron_lostHits = 999 # no cut
lepAna.mu_isoCorr = "deltaBeta"
lepAna.ele_isoCorr = "deltaBeta"
lepAna.ele_tightId = "Cuts_2012"
lepAna.notCleaningElectrons = True

# JET (for event variables do apply the jetID and not PUID yet)
jetAna.relaxJetId = False
jetAna.doPuId = False
jetAna.jetEta = 5.2
jetAna.jetEtaCentral = 2.5
jetAna.jetPt = 10.
jetAna.recalibrateJets = True
jetAna.jetLepDR = 0.4
jetAna.smearJets = False
jetAna.jetGammaDR = 0.4
jetAna.minGammaPt = 20
jetAna.gammaEtaCentral = 2.4
jetAna.cleanJetsFromFirstPhoton = True
jetAna.cleanJetsFromIsoTracks = True ## added for Dominick

# TAU 
tauAna.etaMax = 2.3
tauAna.dxyMax = 99999.
tauAna.dzMax = 99999.
tauAna.vetoLeptons = False
tauAna.vetoLeptonsPOG = True
tauAna.decayModeID = "" # ignored if not set or ""
tauAna.tauAntiMuonID = "againstMuonTight"
tauAna.tauAntiElectronID = "againstElectronLoose"

# Photon
photonAna.etaCentral = 2.5
photonAna.gammaID = "PhotonCutBasedIDLoose_CSA14"

# Isolated Track
isoTrackAna.setOff=False

# recalibrate MET
metAna.recalibrate = False

# store all taus by default
genAna.allGenTaus = True

##------------------------------------------ 
##  CONTROL VARIABLES
##------------------------------------------ 

from CMGTools.TTHAnalysis.analyzers.ttHMT2Control import ttHMT2Control

ttHMT2Control = cfg.Analyzer(
            ttHMT2Control, name = 'ttHMT2Control'
            )

##------------------------------------------
##  TOLOLOGIAL VARIABLES: MT, MT2
##------------------------------------------

from CMGTools.TTHAnalysis.analyzers.ttHTopoVarAnalyzer import ttHTopoVarAnalyzer

ttHTopoJetAna = cfg.Analyzer(
            ttHTopoVarAnalyzer, name = 'ttHTopoVarAnalyzer',
            doOnlyDefault = True
            )

##------------------------------------------
##  Z skim
##------------------------------------------

from CMGTools.TTHAnalysis.analyzers.ttHmllSkimmer import ttHmllSkimmer
# Tree Producer                                                                                                                                                                         
ttHZskim = cfg.Analyzer(
            ttHmllSkimmer, name='ttHmllSkimmer',
            lepId=[13],
            maxLeps=3,
            massMin=60,
            massMax=120,
            doZGen = False,
            doZReco = True
            )


##------------------------------------------
##  PRODUCER
##------------------------------------------

from CMGTools.TTHAnalysis.samples.triggers_13TeV_PHYS14 import triggers_HT900, triggers_MET170, triggers_HTMET, triggers_MT2_mumu, triggers_MT2_ee, triggers_MT2_mue, triggers_1mu, triggers_photon155,triggers_1mu_isolow


triggerFlagsAna.triggerBits = {
            'HT900' : triggers_HT900,
            'MET170' : triggers_MET170,
            'ht350met120' : triggers_HTMET,
            'SingleMu' : triggers_1mu_isolow,
            'DoubleMu' : triggers_MT2_mumu,
            'DoubleEl' : triggers_MT2_ee,
            'MuEG'     : triggers_MT2_mue,
            'htXprescale' : triggers_HTMET,
            'Photons'  : triggers_photon155
}

#-------- SEQUENCE

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyFullHad import *

treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyFullHad',
##     AutoFillTreeProducer, name='treeProducerSusyCore',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     PDFWeights = PDFWeights,
     globalVariables = susyFullHad_globalVariables,
     globalObjects = susyFullHad_globalObjects,
     collections = susyFullHad_collections,
     defaultFloatType = 'F',
)

susyCoreSequence.insert(susyCoreSequence.index(skimAnalyzer),
                        susyCounter)

#susyCoreSequence.insert(susyCoreSequence.index(ttHLepSkim),
#                        ttHZskim)

sequence = cfg.Sequence(
    susyCoreSequence+[
    ttHMT2Control,
    ttHTopoJetAna,
    ttHFatJetAna,
    treeProducer,
    ])

###---- to switch off the compression
#treeProducer.isCompressed = 0


#-------- HOW TO RUN
test = 0
if test==0:
    # ------------------------------------------------------------------------------------------- #
    # --- all this lines taken from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 
    # --- They may not be in synch anymore 
    from CMGTools.TTHAnalysis.samples.ComponentCreator import ComponentCreator
    kreator = ComponentCreator()
    testComponent = kreator.makeMCComponent("testComponent", "/GJets_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",489.9)
    mcSamples=[testComponent]

    dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
    json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'
    from CMGTools.TTHAnalysis.setup.Efficiencies import *

    for comp in mcSamples:
        comp.isMC = True
        comp.isData = False
        comp.splitFactor = 250 
        comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
        comp.puFileData=dataDir+"/puProfile_Data12.root"
        comp.efficiency = eff2012
    # ------------------------------------------------------------------------------------------- #

    comp=testComponent
    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/TESTfilesPHY14/gjets_ht200to400_miniaodsim_fix.root']
    selectedComponents = [comp]
    comp.splitFactor = 10

elif test==1:
    from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import *
    comp=GJets_HT200to400
    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/TESTfilesPHY14/gjets_ht200to400_miniaodsim_fix.root']
    selectedComponents = [comp]
    comp.splitFactor = 10
elif test==2:
    from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import *
    selectedComponents = [ SingleMu, DoubleElectron, TTHToWW_PUS14, DYJetsToLL_M50_PU20bx25, TTJets_PUS14 ]
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 251
        comp.files = comp.files[:]
        #comp.files = comp.files[:1]

from PhysicsTools.HeppyCore.framework.services.tfile import TFileService 
output_service = cfg.Service(
      TFileService,
      'outputfile',
      name="outputfile",
      fname='mt2.root',
      option='recreate'
    )

# the following is declared in case this cfg is used in input to the heppy.py script                                                                                                                   
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [output_service],
                     events_class = Events)

#printComps(config.components, True)
        
