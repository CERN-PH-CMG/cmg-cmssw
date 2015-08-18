##################i#######################################
##       CONFIGURATION FOR JZBEDGE TREES                ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################
# 1.- Common Stuff
# 2.- Lepton Skimming
# 3.- Lepton Analyzer
# 4.- Isolation
# 5.- Photon Analyzer
# 6.- TTHLepEventAnalyzer
# 7.- TTHJetTauAnalyzer
# 8.- Declustering Analyzer
# 9.- TreeProducer SUSY Multilepton
# 10.- Insertion in SUSY Core Sequence
# 11.- Trigger definition an bits
# 12.- Sample imports
# 13.- Sequence
# 14.- Services
# 15.- HOW TO RUN
# 16.- Services
##########################################################
##################Commont stuff###########################
##########################################################
import os
import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *


##########################################################
##################Lepton Skimming#########################
##########################################################
ttHLepSkim.minLeptons = 2
ttHLepSkim.maxLeptons = 999
#ttHLepSkim.idCut  = ""
#ttHLepSkim.ptCuts = []


##########################################################
##################Lepton Analyzier########################
##########################################################
lepAna.doMiniIsolation = True
lepAna.packedCandidates = 'packedPFCandidates'
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.miniIsolationVetoLeptons = None # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
lepAna.loose_electron_id  = "POG_MVA_ID_Run2_NonTrig_VLoose"
lepAna.loose_muon_id      = "POG_ID_Loose"
lepAna.loose_electron_eta = 2.5
lepAna.loose_electron_pt  = 7
lepAna.loose_muon_eta     = 2.4
lepAna.loose_muon_pt      = 5
#These are the nominal cuts
#lepAna.loose_electron_dxy = 0.05
#lepAna.loose_electron_dz  =  0.1
#lepAna.loose_muon_dxy     = 0.05
#lepAna.loose_muon_dz      = 0.1
#But I go more inclusive for further selection in the edgeFriends
lepAna.loose_electron_dxy = 1.0
lepAna.loose_electron_dz  =  1.0
lepAna.loose_muon_dxy     = 0.3
lepAna.loose_muon_dz      = 20.0


jetAna.recalibrateJets = False

##########################################################
######################Isolation###########################
##########################################################
isolation = "miniIso"
#isolation = "ptRel"
if isolation == "ptRel": 
    # delay isolation cut for leptons of pt > 10, for which we do pTrel recovery
    lepAna.loose_muon_isoCut     = lambda muon : muon.relIso03 < 0.5 or muon.pt() > 10
    lepAna.loose_electron_isoCut = lambda elec : elec.relIso03 < 0.5 or elec.pt() > 10
    # in the cleaning, keep the jet if the lepton fails relIso or ptRel
    jetAna.jetLepArbitration = lambda jet,lepton : (
        lepton if (lepton.relIso03 < 0.4 or ptRelv1(lepton.p4(),jet.p4()) > 5) else jet
    )
    ttHCoreEventAna.leptonMVAKindTTH = "SusyWithBoost"
    ttHCoreEventAna.leptonMVAKindSusy = "SusyWithBoost" 
    ttHCoreEventAna.leptonMVAPathTTH = "CMGTools/TTHAnalysis/macros/leptons/trainingPHYS14leptonMVA_PHYS14eleMVA_MiniIso_ttH/weights/%s_BDTG.weights.xml"
    ttHCoreEventAna.leptonMVAPathSusy = "CMGTools/TTHAnalysis/macros/leptons/trainingPHYS14leptonMVA_PHYS14eleMVA_MiniIso_SusyT1/weights/%s_BDTG.weights.xml"
    # insert a second skimmer after the jet cleaning 
    ttHLepSkim2 = cfg.Analyzer(
        ttHLepSkimmer, name='ttHLepSkimmer2',
        minLeptons = 2,
        maxLeptons = 999,
        )
    susyCoreSequence.insert(susyCoreSequence.index(jetAna)+1, ttHLepSkim2)
elif isolation == "miniIso": 
    lepAna.loose_muon_isoCut     = lambda muon : muon.miniRelIso < 0.4
    lepAna.loose_electron_isoCut = lambda elec : elec.miniRelIso < 0.4
elif isolation == None:
    lepAna.loose_muon_isoCut     = lambda muon : True
    lepAna.loose_electron_isoCut = lambda elec : True
else:
    # nothing to do, will use normal relIso03
    pass

##########################################################
##################Photon Analyzer#########################
##########################################################
# Switch off slow photon MC matching
photonAna.do_mc_match = False



##########################################################
###############TTHLepEventAnalyzer########################
##########################################################
## Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
from CMGTools.TTHAnalysis.analyzers.ttHLepEventAnalyzer import ttHLepEventAnalyzer
ttHEventAna = cfg.Analyzer(
    ttHLepEventAnalyzer, name="ttHLepEventAnalyzer",
    minJets25 = 0,
    )

##########################################################
#################TTHJetTauAnalyzer########################
##########################################################
## JetTau analyzer, to be called (for the moment) once bjetsMedium are produced
from CMGTools.TTHAnalysis.analyzers.ttHJetTauAnalyzer import ttHJetTauAnalyzer
ttHJetTauAna = cfg.Analyzer(
    ttHJetTauAnalyzer, name="ttHJetTauAnalyzer",
    )


##########################################################
#################DeClustering Analyzer####################
##########################################################
## Insert declustering analyzer
from CMGTools.TTHAnalysis.analyzers.ttHDeclusterJetsAnalyzer import ttHDeclusterJetsAnalyzer
ttHDecluster = cfg.Analyzer(
    ttHDeclusterJetsAnalyzer, name='ttHDecluster',
    lepCut     = lambda lep,ptrel : lep.pt() > 10,
    maxSubjets = 6, # for exclusive reclustering
    ptMinSubjets = 5, # for inclusive reclustering
    drMin      = 0.2, # minimal deltaR(l,subjet) required for a successful subjet match
    ptRatioMax = 1.5, # maximum pt(l)/pt(subjet) required for a successful match
    ptRatioDiff = 0.1,  # cut on abs( pt(l)/pt(subjet) - 1 ) sufficient to call a match successful
    drMatch     = 0.02, # deltaR(l,subjet) sufficient to call a match successful
    ptRelMin    = 5,    # maximum ptRelV1(l,subjet) sufficient to call a match successful
    prune       = True, # also do pruning of the jets 
    pruneZCut       = 0.1, # pruning parameters (usual value in CMS: 0.1)
    pruneRCutFactor = 0.5, # pruning parameters (usual value in CMS: 0.5)
    verbose     = 0,   # print out the first N leptons
    jetCut = lambda jet : jet.pt() > 20,
    mcPartonPtCut = 20,
    mcLeptonPtCut =  5,
    mcTauPtCut    = 15,
    )


##########################################################
#############TreeProducer SusyMultilepton#################
##########################################################
from CMGTools.TTHAnalysis.analyzers.treeProducerSusyMultilepton import * 
## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyEdge',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     PDFWeights = PDFWeights,
     globalVariables = susyMultilepton_globalVariables,
     globalObjects = susyMultilepton_globalObjects,
     collections = susyMultilepton_collections,
)



##########################################################
############Insertion in susyCoreSequence#################
##########################################################
## Insert the FatJet, SV, HeavyFlavour analyzers in the sequence
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHFatJetAna)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHSVAna)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHHeavyFlavourHadronAna)

susyCoreSequence.insert(susyCoreSequence.index(ttHFatJetAna)+1, ttHDecluster)

## histo counter
susyCoreSequence.insert(susyCoreSequence.index(skimAnalyzer),
                        susyCounter)


##########################################################
############Trigger Definition and bits  #################
##########################################################
from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *
#from CMGTools.RootTools.samples.triggers_8TeV import triggers_1mu_8TeV, triggers_mumu_8TeV, triggers_mue_8TeV, triggers_ee_8TeV;
triggerFlagsAna.triggerBits = {
    'mu17mu8' : triggers_mu17mu8,
    'mu17mu8_dz' : triggers_mu17mu8_dz,
    'mu17tkmu8_dz' : triggers_mu17tkmu8_dz,
    'mu17el12' : triggers_mu17el12,
    'el17el12_dz' : triggers_el17el12_dz,
    'el23el12_dz' : triggers_el23el12_dz,
    'mu8el17' : triggers_mu8el17,
    'mu8el23' : triggers_mu8el23,
    'pfht200' : triggers_pfht200,
    'pfht250' : triggers_pfht250,
    'pfht300' : triggers_pfht300,
    'pfht350' : triggers_pfht350,
    'pfht400' : triggers_pfht400,
    'pfht475' : triggers_pfht475,
    'pfht600' : triggers_pfht600,
    'pfht800' : triggers_pfht800,
    'at57' : triggers_at57,
    'at55' : triggers_at55,
    'at53' : triggers_at53,
    'at52' : triggers_at52,
    'at51' : triggers_at51,
    'DoubleMu' : triggers_mumu,
    #'DoubleMuNoIso' : triggers_mumu_noniso,
    'DoubleEl' : triggers_ee,
    'MuEG'     : triggers_mue,
    'DoubleMuHT'     : triggers_mumu_ht,
    'DoubleElHT'     : triggers_ee_ht,
    'MuEGHT'     : triggers_mue_ht,
    'SingleMu' : triggers_1mu_iso,
    'SingleEl' : triggers_FR_1e_iso,
    'HTMET' : triggers_HTMET120,
    'HTJet' : triggers_htjet,
}


##########################################################
################### Sample imports  ######################
##########################################################
from CMGTools.RootTools.samples.samples_13TeV_74X import *
from CMGTools.RootTools.samples.samples_13TeV_74X_susySignalsPriv import *
from CMGTools.RootTools.samples.samples_8TeVReReco_74X import *
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *


##########################################################
######################## Sequence ########################
##########################################################
sequence = cfg.Sequence(susyCoreSequence+[
        ttHJetTauAna,
        ttHEventAna,
        treeProducer,
    ])

preprocessor = None
##########################################################
###################### HOW TO RUN#########################
##########################################################
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
selectedComponents = [] 

selectedComponents = TTs + [ DYJetsToLL_M50, DYJetsToLL_M10to50]
#selectedComponents = [ TTJets, TTJets_LO, WJetsToLNu, DYJetsToLL_M10to50,  DYJetsToLL_M50  ]


for comp in selectedComponents:
    comp.splitFactor = 500
    comp.finesplitFactor = 4


#test = '74X-MC'
if test == 'synch':
    print 'I\'m in the synch test thing here!!'
    comp = TTJets
    selectedComponents = [comp]
    comp.files = comp.files[:1]
    comp.files = [
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/022B08C4-C702-E511-9995-D4856459AC30.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/027A951D-4103-E511-8B6B-A0040420FE80.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/10950426-4103-E511-8E6B-0025905A60DA.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/143E401F-4103-E511-85AC-B083FED0FFCF.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/16AC5033-A302-E511-88B1-0025905B855E.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/181280CD-B202-E511-B632-842B2B2922E2.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/1EE4C617-9F02-E511-A57E-008CFA1CBB34.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/1EFCABAE-A602-E511-85C3-00259074AE80.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/2021DBC4-8F02-E511-8636-0025905AA9CC.root',
    '/afs/cern.ch/work/m/mdunser/public/synchFiles/24142626-D302-E511-AB2A-0CC47A13CBEA.root'
     ]
    #comp.finesplitFactor = 10
    #comp.finesplitFactor = 4
elif test == '74X-MC':
    #what = getHeppyOption("sample")
    what = 'TT'
    if what == "TTLep":
        selectedComponents = [ TTLep_pow ]
        comp = selectedComponents[0]
        comp.files = [ '/store/mc/RunIISpring15DR74/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/50000/0C1B984D-F408-E511-872E-0002C90B7F2E.root' ]
        tmpfil = os.path.expandvars("/tmp/$USER/0C1B984D-F408-E511-872E-0002C90B7F2E.root")
        if not os.path.exists(tmpfil):
            os.system("xrdcp root://eoscms//eos/cms%s %s" % (comp.files[0],tmpfil))
        comp.files = [ tmpfil ]
    elif what == "TT":
        ttHLepSkim.minLeptons = 0
        selectedComponents = [ TT_bx25 ]
    elif what == "Z":
        selectedComponents = [ ZEE_bx25, ZMM_bx25, ZTT_bx25 ]
    else:
        selectedComponents = RelVals740
    if not getHeppyOption("all"):
        for comp in selectedComponents:
            comp.files = comp.files[:1]
            comp.splitFactor = 1
            comp.fineSplitFactor = 1 if getHeppyOption("single") else 4
elif test == '74X-Data':
    #selectedComponents = [DoubleMuon_Run2015B, MuonEG_Run2015B, DoubleEG_Run2015B, JetHT_Run2015B, HTMHT_Run2015B]
    selectedComponents = [JetHT_Run2015B]#, HTMHT_Run2015B]
    #selectedComponents = [DoubleMuon_Run2015B]
    eventFlagsAna.processName = 'HLT'
    jetAna.recalibrateJets = False
    jetAna.smearJets       = False 
    photonAna.do_mc_match = False
    vertexAna.keepFailingEvents = True 
    jsonAna.useLumiBlocks = True
    #sequence.remove(jsonAna)
    for comp in selectedComponents:
        comp.isMC = False
        comp.isData = True
        #comp.files = ["/afs/cern.ch/work/p/pablom/public/E42FEF61-6E27-E511-B93A-02163E0143C0.root"]
        comp.files = comp.files[:1]

elif test == "express":
    selectedComponents = [ MuEG_740p9 ]
    comp = selectedComponents[0]
    comp.files = [ 'root://eoscms//eos/cms/store/express/Run2015A/ExpressPhysics/FEVT/Express-v1/000/246/908/00000/04B152E7-DE09-E511-8B18-02163E011D4A.root' ]
    comp.name  = 'ExpressPhysics'
    comp.triggers = []
    comp.json     = None
    jetAna.recalibrateJets = False 
    ttHLepSkim.minLeptons = 0
    # preprocessor cfg to be created with
    #    cmsDriver.py miniAOD-data -s PAT --data --runUnscheduled --eventcontent MINIAOD --conditions GR_P_V56 --no_exec
    #    sed -i 's/process.MINIAODoutput_step/process.endpath/' miniAOD-data_PAT.py
    from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
    preprocessor = CmsswPreprocessor("miniAOD-data_PAT.py")



##########################################################
############# Output Module && services###################
##########################################################
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='treeProducerSusyEdge/tree.root',
    option='recreate'
    )    
outputService.append(output_service)

##########################################################
###################### Services ##########################
##########################################################
# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload

if getHeppyOption("nofetch"):
    event_class = Events 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = outputService, 
                     preprocessor = preprocessor, 
                     events_class = event_class)



