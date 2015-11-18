##########################################################
##       CONFIGURATION FOR EXO MONOJET TREES            ##
## skim condition:   MET > 200 GeV                      ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg
import re

# Load all analyzers
from CMGTools.MonoXAnalysis.analyzers.dmCore_modules_cff import * 
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

#-------- SET OPTIONS AND REDEFINE CONFIGURATIONS -----------

is50ns = getHeppyOption("is50ns",False)
runData = getHeppyOption("runData",True)
scaleProdToLumi = float(getHeppyOption("scaleProdToLumi",-1)) # produce rough equivalent of X /pb for MC datasets
saveSuperClusterVariables = getHeppyOption("saveSuperClusterVariables",True)
removeJetReCalibration = getHeppyOption("removeJetReCalibration",False)
doT1METCorr = getHeppyOption("doT1METCorr",True)
forcedSplitFactor = getHeppyOption("splitFactor",-1)
forcedFineSplitFactor = getHeppyOption("fineSplitFactor",-1)
isTest = getHeppyOption("test",None) != None and not re.match("^\d+$",getHeppyOption("test"))

# Define skims
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

#additional MET quantities
metAna.doTkMet = True
treeProducer.globalVariables.append(NTupleVariable("met_trkPt", lambda ev : ev.tkMet.pt() if  hasattr(ev,'tkMet') else  0, help="tkmet p_{T}"))
treeProducer.globalVariables.append(NTupleVariable("met_trkPhi", lambda ev : ev.tkMet.phi() if  hasattr(ev,'tkMet') else  0, help="tkmet phi"))
if doT1METCorr:
    jetAna.calculateType1METCorrection = True
    metAna.recalibrate = "type1"
    metAna.old74XMiniAODs = False


#-------- SEQUENCE
sequence = cfg.Sequence(dmCoreSequence+[
#   monoXRazorAna,
#   monoXMT2Ana,
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
    'SingleEl'     : triggers_1e,
    'MonoJet80MET90' : triggers_Jet80MET90,
    'MonoJet80MET120' : triggers_Jet80MET120,
    'METMu5' : triggers_MET120Mu5,
}
triggerFlagsAna.unrollbits = True
triggerFlagsAna.saveIsUnprescaled = True
triggerFlagsAna.checkL1Prescale = True

from CMGTools.MonoXAnalysis.samples.samples_monojet_13TeV_74X import *
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

selectedComponents = [];

if scaleProdToLumi>0: # select only a subset of a sample, corresponding to a given luminosity (assuming ~30k events per MiniAOD file, which is ok for central production)
    target_lumi = scaleProdToLumi # in inverse picobarns
    for c in selectedComponents:
        if not c.isMC: continue
        nfiles = int(min(ceil(target_lumi * c.xSection / 30e3), len(c.files)))
        #if nfiles < 50: nfiles = min(4*nfiles, len(c.files))
        print "For component %s, will want %d/%d files; AAA %s" % (c.name, nfiles, len(c.files), "eoscms" not in c.files[0])
        c.files = c.files[:nfiles]
        c.splitFactor = len(c.files)
        c.fineSplitFactor = 1

if runData and not isTest: # For running on data
    json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258750_13TeV_PromptReco_Collisions15_25ns_JSON.txt"
    run_ranges = [ (246908,258750) ]; useAAA=False; is50ns=False

    compSelection = ""
    DatasetsAndTriggers = []
    selectedComponents = []; vetos = []
    ProcessingsAndRunRanges = []; Shorts = []
    ProcessingsAndRunRanges.append( ("Run2015C_25ns-05Oct2015-v1", [254227,255031] ) ); Shorts.append("Run2015C_05Oct")
    ProcessingsAndRunRanges.append( ("Run2015D-05Oct2015-v1", [256630,258158] ) ); Shorts.append("Run2015D_05Oct")
    ProcessingsAndRunRanges.append( ("Run2015D-PromptReco-v4", [258159,999999] ) ); Shorts.append("Run2015D_v4")
    
    if diLepSkim == True:
        DatasetsAndTriggers.append( ("DoubleMuon", triggers_mumu_iso + triggers_mumu_ss + triggers_mumu_ht + triggers_3mu + triggers_3mu_alt) )
        DatasetsAndTriggers.append( ("DoubleEG",   triggers_ee + triggers_ee_ht + triggers_3e) )
    if singleLepSkim == True:
        DatasetsAndTriggers.append( ("SingleMuon", triggers_1mu_iso + triggers_1mu_iso_50ns + triggers_1mu_noniso) )
        DatasetsAndTriggers.append( ("SingleElectron", triggers_1e + triggers_1e_50ns) )
    if signalSkim == True:
        DatasetsAndTriggers.append( ("MET", triggers_Jet80MET90 + triggers_Jet80MET120 + triggers_MET120Mu5 ) )

    for pd,triggers in DatasetsAndTriggers:
        iproc=0 
        for processing,run_dslimits in ProcessingsAndRunRanges:
            for run_range in run_ranges:
                run_min = max(run_range[0],run_dslimits[0])
                run_max = min(run_range[1],run_dslimits[1])
                this_run_range = (run_min,run_max)
                label = "runs_%d_%d" % this_run_range if this_run_range[0] != this_run_range[1] else "run_%d" % (this_run_range[0],)
                compname = pd+"_"+Shorts[iproc]+"_"+label
                if ((compSelection and not re.search(compSelection, compname))):
                    print "Will skip %s" % (compname)

                    continue
                print "Building component ",compname," with run range ",label, "\n"
                comp = kreator.makeDataComponent(compname, 
                                                 "/"+pd+"/"+processing+"/MINIAOD", 
                                                 "CMS", ".*root", 
                                                 json=json, 
                                                 run_range=this_run_range, 
                                                 #triggers=triggers[:], vetoTriggers = vetos[:],
                                                 useAAA=useAAA)
                print "Will process %s (%d files)" % (comp.name, len(comp.files))
                print "\ttrigger sel %s, veto %s" % (triggers, vetos)
                comp.splitFactor = len(comp.files)/4
                comp.fineSplitFactor = 1
                selectedComponents.append( comp )
            iproc += 1
        vetos += triggers
    if json is None:
        dmCoreSequence.remove(jsonAna)

if is50ns:
    jetAna.mcGT     = "Summer15_50nsV5_MC"
    jetAna.dataGT   = "Summer15_50nsV5_DATA"
    pfChargedCHSjetAna.mcGT     = "Summer15_50nsV5_MC"
    pfChargedCHSjetAna.dataGT   = "Summer15_50nsV5_DATA"

if removeJetReCalibration:
    ## NOTE: jets will still be recalibrated, since calculateSeparateCorrections is True,
    ##       however the code will check that the output 4-vector is unchanged.
    jetAna.recalibrateJets = False

if forcedSplitFactor>0 or forcedFineSplitFactor>0:
    if forcedFineSplitFactor>0 and forcedSplitFactor!=1: raise RuntimeError, 'splitFactor must be 1 if setting fineSplitFactor'
    for c in selectedComponents:
        if forcedSplitFactor>0: c.splitFactor = forcedSplitFactor
        if forcedFineSplitFactor>0: c.fineSplitFactor = forcedFineSplitFactor

if runData==False and not isTest: # MC all
    ### 25 ns 74X MC samples
    is50ns = False
    mcSamples = mcSamples_monojet_Asymptotic25ns
    if signalSkim:
        mcSamples += mcSamples_monojet_Asymptotic25ns_signals
    selectedComponents = mcSamples 

### 50 ns 74X MC samples
#selectedComponents = mcSamples_monojet_Asymptotic50ns ; is50ns = True
    for comp in selectedComponents:
        comp.splitFactor = len(comp.files)/4
        comp.fineSplitFactor = 1



#-------- HOW TO RUN ----------- 
test = getHeppyOption('test')
if test == 'DYJets':
    monoJetSkim.metCut = 0
    monoJetCtrlLepSkim.minLeptons = 2
    comp = DYJetsToLL_M50_HT100to200
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    selectedComponents = [ comp ]
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
elif test == 'synch-74X': # sync
    #eventSelector.toSelect = [ 11809 ]
    #sequence = cfg.Sequence([eventSelector] + dmCoreSequence+[ ttHEventAna, treeProducer, ])
    monoJetCtrlLepSkim.minLeptons = 0
    monoJetSkim.metCut = 0  
    what = getHeppyOption("sample")
    if what == "TTbarDM":
        comp = kreator.makeMCComponent("TTbarDM","/TTbarDMJets_pseudoscalar_Mchi-1_Mphi-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM", "CMS", ".*root", 1.0, useAAA=True)
        selectedComponents = [ comp ]
    elif what == "DMS":
        comp = DMS_Mchi_100_gSM_1p0_gDM_1p0
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15MiniAODv2/DMS_NNPDF30_Scalar_Mphi-300_Mchi-100_gSM-1p0_gDM-1p0_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/10000/6872703C-7972-E511-8228-0CC47A009E26.root' ]
        selectedComponents = [ comp ]
    elif what == "TTLep":
        comp = TTLep_pow
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15MiniAODv2/TTTo2L2Nu_13TeV-powheg/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/60000/F8910E89-C56D-E511-BD90-003048F35244.root' ]
        selectedComponents = [ comp ]
    elif what == "DYJets":
        comp = DYJetsToLL_M50
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/50000/00759690-D16E-E511-B29E-00261894382D.root' ]
        selectedComponents = [ comp ]
    elif what == "TTJets":
        comp = TJets_LO
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/001F4F14-786E-E511-804F-0025905A60FE.root' ]
        selectedComponents = [ comp ]
    elif what == "WJets":
        comp = WJetsToLNu_HT100to200
        comp.files = [ 'root://eoscms//eos/cms/store/mc/RunIISpring15MiniAODv2/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/10000/003F1529-D36D-E511-9E33-001E6724816F.root' ]
        selectedComponents = [ comp ]
    else:
        selectedComponents = mcSamples_monojet_Asymptotic25ns
    jetAna.smearJets       = False
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.fineSplitFactor = 1 if getHeppyOption("single") else 2
elif test == '74X-Data':
    what = getHeppyOption("sample")
    if what == "DoubleEG":
        comp = DoubleEG_Run2015D_05Oct
        comp.files = [ 'root://eoscms//eos/cms/store/data/Run2015D/DoubleEG/MINIAOD/05Oct2015-v1/50000/0014E86F-656F-E511-9D3F-002618943831.root' ]
        selectedComponents = [ comp ]
        monoJetCtrlLepSkim.minLeptons = 2
    elif what == "DoubleMuon":
        comp = DoubleMuon_Run2015D_05Oct
        comp.files = [ 'root://eoscms//eos/cms/store/data/Run2015D/DoubleMuon/MINIAOD/05Oct2015-v1/30000/04008DF6-8A6F-E511-B034-0025905A6136.root' ]
        selectedComponents = [ comp ]
        monoJetCtrlLepSkim.minLeptons = 2
    else:
        selectedComponents = dataSamples_Run2015D_05Oct
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.fineSplitFactor = 1 if getHeppyOption("single") else 4
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
EOSEventsWithDownload.aggressive = 2 # always fetch if running on Wigner
if getHeppyOption("nofetch")  or getHeppyOption("isCrab"):
    event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = outputService,  
                     events_class = event_class)


