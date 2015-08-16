import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

##------------------------------------------
## Redefine analyzer parameters
##------------------------------------------

# Muons
#------------------------------
lepAna.loose_muon_dxy = 0.5
lepAna.loose_muon_dz  = 1.0
lepAna.loose_muon_relIso  = 0.15
lepAna.loose_muon_isoCut = lambda muon :muon.miniRelIso < 0.2

lepAna.loose_electron_pt  = 5
lepAna.loose_electron_eta    = 2.4
lepAna.loose_electron_relIso = 0.15
lepAna.loose_electron_isoCut = lambda electron : electron.miniRelIso < 0.1

lepAna.loose_electron_id  = "POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto_full5x5"
lepAna.loose_electron_lostHits = 999. # no cut
lepAna.loose_electron_dxy    = 999.
lepAna.loose_electron_dz     = 999.

lepAna.inclusive_electron_id  = "POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto_full5x5"
lepAna.inclusive_electron_lostHits = 999. # no cut since embedded in ID
lepAna.inclusive_electron_dxy    = 999. # no cut since embedded in ID
lepAna.inclusive_electron_dz     = 999. # no cut since embedded in ID

lepAna.mu_isoCorr = "deltaBeta"
lepAna.ele_isoCorr = "deltaBeta"
lepAna.ele_tightId = "Cuts_PHYS14_25ns_v1_ConvVetoDxyDz"
lepAna.notCleaningElectrons = True
lepAna.doMiniIsolation = True
lepAna.miniIsolationPUCorr = 'rhoArea'
# Photons
#------------------------------
photonAna.ptMin                        = 25,
photonAna.epaMax                       = 2.5,

# Taus 

jetAna.relaxJetId = False
jetAna.doPuId = False
jetAna.doQG = True
jetAna.jetEta = 4.7
jetAna.jetEtaCentral = 2.5
jetAna.jetPt = 40.
#jetAna.mcGT     = "Summer15_50nsV2_MC", # jec corrections
#jetAna.dataGT   = "Summer15_50nsV2_MC", # jec corrections
jetAna.recalibrateJets = True
jetAna.jetLepDR = 0.4
jetAna.smearJets = False

#--------------------------------


# Isolated Track
isoTrackAna.setOff=False

# recalibrate MET
metAna.recalibrate = False

# store all taus by default
genAna.allGenTaus = True

#------------------------------


# Energy sums
#------------------------------
# NOTE: Currently energy sums are calculated with 40 GeV jets (ttHCoreEventAnalyzer.py)
#       However, the input collection is cleanjets which have a 50 GeV cut so this is a labeling problem


# Jet-MET based Skim (generic, but requirements depend on the final state)
from CMGTools.TTHAnalysis.analyzers.ttHJetMETSkimmer import ttHJetMETSkimmer
ttHJetMETSkim = cfg.Analyzer(
   ttHJetMETSkimmer, name='ttHJetMETSkimmer',
   jets      = "cleanJets", # jet collection to use
   jetPtCuts = [],  # e.g. [60,40,30,20] to require at least four jets with pt > 60,40,30,20
   jetVetoPt =  0,  # if non-zero, veto additional jets with pt > veto beyond the ones in jetPtCuts
   metCut    =  150,  # MET cut
   htCut     = ('htJet40j', 100), # cut on HT defined with only jets and pt cut 40, at zero; i.e. no cut
                                # see ttHCoreEventAnalyzer for alternative definitions
   mhtCut    = ('mhtJet40', 0), # cut on MHT defined with all leptons, and jets with pt > 40.
   nBJet     = ('CSVv2IVFM', 0, "jet.pt() > 40"),     # require at least 0 jets passing CSV medium and pt > 30
   )

ttHJetMETSkim.htCut       = ('htJet50j', 0)
ttHJetMETSkim.mhtCut      = ('htJet40j', 0)
ttHJetMETSkim.nBJet       = ('CSVv2IVFM', 0, "jet.pt() > 30")     # require at least 0 jets passing CSVM and pt > 50

##------------------------------------------
##  ISOLATED TRACK
##------------------------------------------

isoTrackAna.setOff=False
isoTrackAna.doRelIsolation = True

##------------------------------------------
##------------------------------------------


from CMGTools.TTHAnalysis.analyzers.ttHDiJetControl import ttHDiJetControl

ttHDiJetControlAna = cfg.Analyzer(
            ttHDiJetControl, name = 'ttHDiJetControl',
            jetPt = 40.,
            )

#------------------------------------------
##  PRODUCER
##------------------------------------------
from CMGTools.TTHAnalysis.analyzers.treeProducerSusyDiJet import * 
## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyDiJet',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     PDFWeights = PDFWeights,
     globalVariables = susyDiJet_globalVariables,
     globalObjects = susyDiJet_globalObjects,
     collections = susyDiJet_collections,
)

#-------- SEQUENCE

sequence = cfg.Sequence(susyCoreSequence + [
                        ttHDiJetControlAna,
                        ttHJetMETSkim, 
                        treeProducer,
                        ])


from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import triggers_HT900, triggers_HT800, triggers_MET170, triggers_HTMET100, triggers_HTMET120, triggers_MT2_mumu, triggers_MT2_ee, triggers_MT2_e, triggers_MT2_mu, triggers_MT2_mue, triggers_dijet, triggers_dijet70met120, triggers_dijet55met110, triggers_ht350, triggers_ht475,  triggers_ht600, triggers_photon75, triggers_photon90, triggers_photon120, triggers_photon75ps, triggers_photon90ps, triggers_photon120ps, triggers_photon155, triggers_photon165_HE10, triggers_photon175, triggers_Jet80MET90, triggers_Jet80MET120

triggerFlagsAna.triggerBits = {
'PFHT900' : triggers_HT900,
'PFHT800' : triggers_HT800,
'PFMET170' : triggers_MET170,
'PFHT350_PFMET100' : triggers_HTMET100,
'PFHT350_PFMET120' : triggers_HTMET120,
'SingleMu' : triggers_MT2_mu,
'SingleEl' : triggers_MT2_e,
'DoubleMu' : triggers_MT2_mumu,
'DoubleEl' : triggers_MT2_ee,
'MuEG' : triggers_MT2_mue,
'DiCentralPFJet70_PFMET120' : triggers_dijet70met120,
'DiCentralPFJet55_PFMET110' : triggers_dijet55met110,
'MonoCentralPFJet80_PFMET90' : triggers_Jet80MET90,
'MonoCentralPFJet80_PFMET120' : triggers_Jet80MET120,
}
#------------------------------------------
##  PRODUCER
##---------------

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.RootTools.samples.samples_13TeV_74X import *


# Selected samples as defined on the AlphaT twiki
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

#-------- HOW TO RUN
# choose 2 for full production
test = 2 
isData = False
doSpecialSettingsForMECCA = 1
if test==0:
    # ------------------------------------------------------------------------------------------- #
    # --- all this lines taken from CMGTools.RootTools.samples.samples_13TeV_PHYS14
    # --- They may not be in synch anymore 
    from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
    kreator = ComponentCreator()
    testComponent = kreator.makeMCComponent("testComponent", "/GJets_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",489.9)
    samples=[testComponent]

    dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
    json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'
    from CMGTools.TTHAnalysis.setup.Efficiencies import *

    for comp in samples:
        comp.isMC = True
        comp.isData = False
        comp.splitFactor = 250
        comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
        comp.puFileData=dataDir+"/puProfile_Data12.root"
        comp.efficiency = eff2012
        comp.json = json
    # ------------------------------------------------------------------------------------------- #

    comp=testComponent
    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/74samples/JetHT_GR_R_74_V12_19May_RelVal/1294BDDB-B7FE-E411-8028-002590596490.root']


    selectedComponents = [comp]
    comp.splitFactor = 10
#    comp.fineSplitFactor = 100


# Test a single component, using a single thread.
#--------------------------------------------------
elif test==1:
    comp               =  SMS_T2bb_2J_mStop600_mLSP580 
    #comp.files = ['/afs/cern.ch/work/p/pandolf/CMSSW_7_0_6_patch1_2/src/CMGTools/TTHAnalysis/cfg/pickevents.root']
    comp.files         = comp.files[:]
    
    selectedComponents = [comp]
    comp.splitFactor   = len(comp.files)
elif test==2:


    selectedComponents = [
#ZJetsToNuNu_HT100to200,
ZJetsToNuNu_HT200to400,
ZJetsToNuNu_HT400to600,
ZJetsToNuNu_HT600toInf,
TTJets,
#SMS_T2bb_2J_mStop900_mLSP100,
#SMS_T2bb_2J_mStop600_mLSP580,
WJetsToLNu,
WJetsToLNu_HT100to200,
WJetsToLNu_HT200to400,
WJetsToLNu_HT400to600,
WJetsToLNu_HT600toInf,
WJetsToLNu_HT600to800,
WJetsToLNu_HT800to1200,
WJetsToLNu_HT1200to2500,
WJetsToLNu_HT2500toInf

]

    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.files = comp.files[:]
        comp.splitFactor = len(comp.files)
        #comp.fineSplitFactor = 2 # to run two jobs per file
        #comp.files = comp.files[:1]
        #comp.files = comp.files[57:58]  # to process only file [57]  
        # triggers on MC
        #comp.triggers = triggers_HT900 + triggers_HTMET + triggers_photon155 + triggers_1mu_isolow + triggers_MT2_mumu + triggers_MT2_ee + triggers_MT2_mue # to apply trigger skimming

elif test==3:
    # run on data
    isData = True
    from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *
    comp = JetHT_Run2015B
    comp.files = ['/afs/cern.ch/user/m/mangano/public/MECCA/dataset/74X/data/JetHT_promptReco_Run2015B.root']

    selectedComponents = [comp]

# ------------------------------------------------------------------------------------------- #


if doSpecialSettingsForMECCA:
    jetAna.doQG = False
    photonAna.do_randomCone = False



if isData:
    eventFlagsAna.processName = 'HLT'
    for comp in samples:
        comp.isMC = False
        comp.isData = True

#--------------------------------------------------

# Test all components (1 thread per component).
#--------------------------------------------------


#config = cfg.Config( components = selectedComponents,
#                     sequence = sequence )

#printComps(config.components, True)
        # the following is declared in case this cfg is used in input to the heppy.py script

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events)

