##########################################################
##       CONFIGURATION FOR EXO MONOJET TREES            ##
## skim condition:   MET > 200 GeV                      ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.MonoXAnalysis.analyzers.dmCore_modules_cff import * 
from PhysicsTools.Heppy.analyzers.objects.METAnalyzer import *

# Redefine what I need
signalSkim = False
diLepSkim = True
singleLepSkim = False

# --- MONOJET SKIMMING ---
if signalSkim == True:
    monoJetSkim.metCut = 200
    monoJetSkim.jetPtCuts = []

# --- Z->ll control sample SKIMMING ---
if diLepSkim == True:
    monoJetCtrlLepSkim.minLeptons = 2
if singleLepSkim == True:
    monoJetCtrlLepSkim.minLeptons = 1

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
    metCollection     = "slimmedMETs",
    doOnlyDefault = False,
    jetPt = 40.,
    collectionPostFix = "",
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

# for electron scale and resolution checks
saveSuperClusterVariables=True
if saveSuperClusterVariables:
    leptonTypeExtra.addVariables([
            NTupleVariable("e5x5", lambda x: x.e5x5() if (abs(x.pdgId())==11 and hasattr(x,"e5x5")) else -999, help="Electron e5x5"),
            NTupleVariable("r9", lambda x: x.r9() if (abs(x.pdgId())==11 and hasattr(x,"r9")) else -999, help="Electron r9"),
            NTupleVariable("sigmaIetaIeta", lambda x: x.sigmaIetaIeta() if (abs(x.pdgId())==11 and hasattr(x,"sigmaIetaIeta")) else -999, help="Electron sigmaIetaIeta"),
            NTupleVariable("sigmaIphiIphi", lambda x: x.sigmaIphiIphi() if (abs(x.pdgId())==11 and hasattr(x,"sigmaIphiIphi")) else -999, help="Electron sigmaIphiIphi"),
            NTupleVariable("hcalOverEcal", lambda x: x.hcalOverEcal() if (abs(x.pdgId())==11 and hasattr(x,"hcalOverEcal")) else -999, help="Electron hcalOverEcal"),
            NTupleVariable("full5x5_e5x5", lambda x: x.full5x5_e5x5() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_e5x5")) else -999, help="Electron full5x5_e5x5"),
            NTupleVariable("full5x5_r9", lambda x: x.full5x5_r9() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_r9")) else -999, help="Electron full5x5_r9"),
            NTupleVariable("full5x5_sigmaIetaIeta", lambda x: x.full5x5_sigmaIetaIeta() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_sigmaIetaIeta")) else -999, help="Electron full5x5_sigmaIetaIeta"),
            NTupleVariable("full5x5_sigmaIphiIphi", lambda x: x.full5x5_sigmaIphiIphi() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_sigmaIphiIphi")) else -999, help="Electron full5x5_sigmaIphiIphi"),
            NTupleVariable("full5x5_hcalOverEcal", lambda x: x.full5x5_hcalOverEcal() if (abs(x.pdgId())==11 and hasattr(x,"full5x5_hcalOverEcal")) else -999, help="Electron full5x5_hcalOverEcal"),
            NTupleVariable("correctedEcalEnergy", lambda x: x.correctedEcalEnergy() if (abs(x.pdgId())==11 and hasattr(x,"correctedEcalEnergy")) else -999, help="Electron correctedEcalEnergy"),
            NTupleVariable("eSuperClusterOverP", lambda x: x.eSuperClusterOverP() if (abs(x.pdgId())==11 and hasattr(x,"eSuperClusterOverP")) else -999, help="Electron eSuperClusterOverP"),
            NTupleVariable("ecalEnergy", lambda x: x.ecalEnergy() if (abs(x.pdgId())==11 and hasattr(x,"ecalEnergy")) else -999, help="Electron ecalEnergy"),
            NTupleVariable("superCluster_rawEnergy", lambda x: x.superCluster().rawEnergy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.rawEnergy"),
            NTupleVariable("superCluster_preshowerEnergy", lambda x: x.superCluster().preshowerEnergy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.preshowerEnergy"),
            NTupleVariable("superCluster_correctedEnergy", lambda x: x.superCluster().correctedEnergy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.correctedEnergy"),
            NTupleVariable("superCluster_energy", lambda x: x.superCluster().energy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.energy"),
            NTupleVariable("superCluster_clustersSize", lambda x: x.superCluster().clustersSize() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.clustersSize"),
            NTupleVariable("superCluster_seed.energy", lambda x: x.superCluster().seed().energy() if (abs(x.pdgId())==11 and hasattr(x,"superCluster")) else -999, help="Electron superCluster.seed.energy"),
])


## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerDarkMatterMonoJet',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     PDFWeights = PDFWeights,
     doPDFVars = True,
     globalVariables = dmMonoJet_globalVariables,
     globalObjects = dmMonoJet_globalObjects,
     collections = dmMonoJet_collections,
)

## histo counter
# dmCoreSequence.insert(dmCoreSequence.index(skimAnalyzer),
#                       dmCounter)

# HBHE new filter
from CMGTools.TTHAnalysis.analyzers.hbheAnalyzer import hbheAnalyzer
hbheAna = cfg.Analyzer(
    hbheAnalyzer, name="hbheAnalyzer", IgnoreTS4TS5ifJetInLowBVRegion=False
    )
dmCoreSequence.insert(dmCoreSequence.index(ttHCoreEventAna),hbheAna)
treeProducer.globalVariables.append(NTupleVariable("hbheFilterNew50ns", lambda ev: ev.hbheFilterNew50ns, int, help="new HBHE filter for 50 ns"))
treeProducer.globalVariables.append(NTupleVariable("hbheFilterNew25ns", lambda ev: ev.hbheFilterNew25ns, int, help="new HBHE filter for 25 ns"))
treeProducer.globalVariables.append(NTupleVariable("hbheFilterIso", lambda ev: ev.hbheFilterIso, int, help="HBHE iso-based noise filter"))



#-------- SEQUENCE
sequence = cfg.Sequence(dmCoreSequence+[
   monoXRazorAna,
   monoXMT2Ana,
   ttHFatJetAna,
   monoJetVarAna,
   MonoJetEventAna,
   treeProducer,
    ])


from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import *
from CMGTools.RootTools.samples.triggers_8TeV import triggers_1mu_8TeV, triggers_mumu_8TeV, triggers_mue_8TeV, triggers_ee_8TeV;
triggerFlagsAna.triggerBits = {
    'DoubleMu' : triggers_mumu_iso,
    'DoubleMuSS' : triggers_mumu_ss,
    'DoubleMuNoIso' : triggers_mumu_noniso,
    'DoubleEl' : triggers_ee,
    'MuEG'     : triggers_mue,
    'DoubleMuHT' : triggers_mumu_ht,
    'DoubleElHT' : triggers_ee_ht,
    'MuEGHT' : triggers_mue_ht,
    'TripleEl' : triggers_3e,
    'TripleMu' : triggers_3mu,
    'TripleMuA' : triggers_3mu_alt,
    'DoubleMuEl' : triggers_2mu1e,
    'DoubleElMu' : triggers_2e1mu,
    'SingleMu' : triggers_1mu_iso,
    'SingleMu50ns' : triggers_1mu_iso_50ns,
    'SingleEl'     : triggers_1e,
    'SingleEl50ns' : triggers_1e_50ns,
    'SingleMu_8TeV' : triggers_1mu_8TeV + triggers_1mu_iso_r,
    'DoubleMu_8TeV' : triggers_mumu_8TeV + triggers_mumu_run1,
    'MuEG_8TeV'     : triggers_mue_8TeV + triggers_mue_run1,
    'DoubleEl_8TeV' : triggers_ee_8TeV + triggers_ee_run1,
    'MonoJet80MET90' : triggers_Jet80MET90,
    'MonoJet80MET120' : triggers_Jet80MET120,
    'METMu5' : triggers_MET120Mu5,
}

from CMGTools.MonoXAnalysis.samples.samples_monojet_13TeV_74X import *

selectedComponents = []; is50ns = False

isData = True

if True: # For running on data
    isData = True
    # Run2015C (Golden) + Run2015D (DCS) up to run 256941 , 25 ns, 3.8T     
    json = "$CMSSW_BASE/src/CMGTools/MonoXAnalysis/python/samples/Golden_246908-255031_13TeV_PromptReco_Collisions15_25ns_v2_OR_DCS_254914_256941_JSON.txt"
    processing = "Run2015D-PromptReco-v3"; short = "Run2015D_v3"; run_ranges = [ (256584,256842) ]; useAAA=False; is50ns=False

    compSelection = ""
    DatasetsAndTriggers = []
    selectedComponents = []; vetos = []

    if diLepSkim == True:
        DatasetsAndTriggers.append( ("DoubleMuon", triggers_mumu_iso + triggers_mumu_ss + triggers_mumu_ht + triggers_3mu + triggers_3mu_alt) )
        DatasetsAndTriggers.append( ("DoubleEG",   triggers_ee + triggers_ee_ht + triggers_3e) )
    if singleLepSkim == True:
        DatasetsAndTriggers.append( ("SingleMuon", triggers_1mu_iso + triggers_1mu_iso_50ns + triggers_1mu_noniso) )
        DatasetsAndTriggers.append( ("SingleElectron", triggers_1e + triggers_1e_50ns) )
    if signalSkim == True:
        DatasetsAndTriggers.append( ("MET", triggers_Jet80MET90 + triggers_Jet80MET120 + triggers_MET120Mu5 ) )

    for pd,triggers in DatasetsAndTriggers:
        for run_range in run_ranges:
            label = "runs_%d_%d" % run_range if run_range[0] != run_range[1] else "run_%d" % (run_range[0],)
            compname = pd+"_"+short+"_"+label
            if ((compSelection and not re.search(compSelection, compname))):
                    print "Will skip %s" % (compname)
                    continue
            comp = kreator.makeDataComponent(compname, 
                                             "/"+pd+"/"+processing+"/MINIAOD", 
                                             "CMS", ".*root", 
                                             json=json, 
                                             run_range=run_range, 
                                             triggers=triggers[:], vetoTriggers = vetos[:],
                                             useAAA=useAAA)
            print "Will process %s (%d files)" % (comp.name, len(comp.files))
#            print "\ttrigger sel %s, veto %s" % (triggers, vetos)
            comp.splitFactor = len(comp.files)
            comp.fineSplitFactor = 1
            selectedComponents.append( comp )
        vetos += triggers
    if json is None:
        dmCoreSequence.remove(jsonAna)



if False: # MC all
### 25 ns 74X MC samples
#selectedComponents = mcSamples_monojet_Asymptotic25ns ; is50ns = False
### 50 ns 74X MC samples
#selectedComponents = mcSamples_monojet_Asymptotic50ns ; is50ns = True
    for comp in selectedComponents:
        comp.splitFactor = 20
        comp.fineSplitFactor = 1


#-------- HOW TO RUN ----------- 
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
test = getHeppyOption('test')
if test == '1':
    monoJetSkim.metCut = 0
    comp = DYJetsToLL_M50_HT100to200
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    selectedComponents = [ comp ]
elif test == '2':
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
elif test == 'EOS':
    comp = DYJetsToLL_M50
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
elif test == 'synch-74X': # sync
    from CMGTools.MonoXAnalysis.samples.samples_monojet_13TeV_74X import *
    #eventSelector.toSelect = [ 11809 ]
    #sequence = cfg.Sequence([eventSelector] + dmCoreSequence+[ ttHEventAna, treeProducer, ])
    monoJetCtrlLepSkim.minLeptons = 0
    monoJetSkim.metCut = 0  
    what = getHeppyOption("sample")
    if what == "ADD":
        comp = ADD_MJ
        comp.files = [ 'root://eoscms//eos/cms/store/relval/CMSSW_7_4_1/RelValADDMonoJet_d3MD3_13/MINIAODSIM/MCRUN2_74_V9_gensim71X-v1/00000/80CF5456-B9EC-E411-93DA-002618FDA248.root' ]
        selectedComponents = [ comp ]
    elif what == "TTLep":
        comp = TTLep
        comp.files = [ 'root://eoscms//eos/cms/store/relval/CMSSW_7_4_1/RelValProdTTbar_13/MINIAODSIM/MCRUN2_74_V9_gensim71X-v1/00000/0A9E2CED-C9EC-E411-A8E4-003048FFCBA8.root' ]
        selectedComponents = [ comp ]
    elif what == "DYJets":
        comp = DYJetsToLL_M50_50ns
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v2/60000/04963444-D107-E511-B245-02163E00F339.root' ]
        jetAna.mcGT = "MCRUN2_74_V9A"
        selectedComponents = [ comp ]
    elif what == "TTbar":
        comp = TTbar
        comp.files = [ 'root://eoscms//eos/cms/store/relval/CMSSW_7_4_1/RelValProdTTbar_13/MINIAODSIM/MCRUN2_74_V9_gensim71X-v1/00000/0A9E2CED-C9EC-E411-A8E4-003048FFCBA8.root' ]
        selectedComponents = [ comp ]
    elif what == "WJets":
        comp = WJetsToLNu_HT400to600
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15DR74/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v3/00000/6408230F-9F08-E511-A1A6-D4AE526A023A.root' ]
        selectedComponents = [ comp ]
    elif what == "RSGrav":
        comp = RSGravGaGa
        comp.files = [ 'root://eoscms//eos/cms/store/relval/CMSSW_7_4_1/RelValRSGravitonToGaGa_13TeV/MINIAODSIM/MCRUN2_74_V9_gensim71X-v1/00000/189277BA-DCEC-E411-B3B8-0025905B859E.root' ]
        selectedComponents = [ comp ]
    else:
        selectedComponents = RelVals741
    jetAna.recalibrateJets = True
    jetAna.smearJets       = False
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.fineSplitFactor = 10
elif test == 'SR':
    selectedComponents = backgroundSamples + signalSamples
    #selectedComponents = backgroundSamples
    monoJetSkim.metCut = 200
    monoJetCtrlLepSkim.minLeptons = 0
    for comp in selectedComponents:
        comp.splitFactor = 350
elif test == '74X-MC':
    from CMGTools.MonoXAnalysis.samples.samples_monojet_13TeV_74X import *
    what = getHeppyOption("sample")
    if what == "TT":
        monoJetCtrlLepSkim.minLeptons = 0
        selectedComponents = TTJets_LO_50ns
    elif what == "Z":
        monoJetCtrlLepSkim.minLeptons = 0
        monoJetSkim.metCut = 0
        selectedComponents = [ DYJetsToLL_M50_50ns ]
    elif what == "WJets":
        monoJetSkim.metCut = 0
        selectedComponents = [ WJetsToLNu_50ns ]
    else:
        selectedComponents = RelVals740
    if not getHeppyOption("all"):
        for comp in selectedComponents:
            comp.files = comp.files[:1]
            comp.splitFactor = 1
            comp.fineSplitFactor = 1 if getHeppyOption("single") else 4
elif test == '74X-Data':
    from CMGTools.MonoXAnalysis.samples.samples_monojet_13TeV_74X import *
    from CMGTools.MonoXAnalysis.samples.samples_8TeVReReco_74X import *
    what = getHeppyOption("sample")
    if what == "JetHT":
        monoJetSkim.metCut = 0
        selectedComponents = [ JetHT_742 ]
    elif what == "Z":
        monoJetCtrlLepSkim.minLeptons = 2
        monoJetSkim.metCut = 0
        selectedComponents = [ SingleMuZ_742, DoubleElectronZ_742 ]
    elif what == "MuEG":
        selectedComponents = [ MuEG_742 ]
    elif what == "EGamma":
        selectedComponents = [ privEGamma2015A ]
        lepAna.loose_electron_id = ""
        lepAna.loose_electron_relIso = 1000.
        photonAna.gammaID = "POG_PHYS14_25ns_Loose_NoIso"
        monoJetCtrlLepSkim.minLeptons = 1
        monoJetSkim.metCut = 0
    elif what == "DoubleEG":
        selectedComponents = [ DoubleEG_Run2015B ]
        monoJetCtrlLepSkim.minLeptons = 2
    else:
        selectedComponents = dataSamples742
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.fineSplitFactor = 1 if getHeppyOption("single") else 6
        if not getHeppyOption("all"):
            comp.files = comp.files[:1]

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
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
if getHeppyOption("nofetch"):
    event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = outputService,  
                     events_class = event_class)


