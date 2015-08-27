import PhysicsTools.HeppyCore.framework.config as cfg

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.RootTools.samples.samples_8TeVReReco_74X import * # <-- this one for the official sample
from CMGTools.ObjectStudies.samples.samples_METPOG_private import * #<-- this one for the private re-reco
from CMGTools.RootTools.samples.samples_13TeV_74X import *
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import triggers_1mu_iso_50ns, triggers_mumu, triggers_ee, triggers_photon30, triggers_photon50, triggers_photon75

#-------- INITIAL FLAG
isDiJet=False
isZSkim=False
is1L=False
isEle = False # default is diMuon
isEarlyRun = False # to be used for the filters
is25ns = False

#-------- HOW TO RUN

test = 13

if test==0:
    isData = True
    selectedComponents = [DoubleMu_742, DoubleMu_740p9]
#    selectedComponents = [ DoubleMuParked_1Apr_RelVal_dm2012D_v2_newPFHCalib , DoubleMuParked_1Apr_RelVal_dm2012D_v2_oldPFHCalib , DoubleMuparked_1Apr_RelVal_dm2012D_v2 ]
    for comp in selectedComponents:
        comp.splitFactor = 251
        comp.files = comp.files[:]
        comp.triggers = triggers_8TeV_mumu

elif test==1:
    isData = False
    selectedComponents = [ RelValZMM_7_4_1,RelValZMM_7_4_0_pre9 ]
#    selectedComponents = [RelVal_741_Philfixes]
#    selectedComponents = relValkate
    for comp in selectedComponents:
#        comp.splitFactor = 1
        comp.splitFactor = 100
        comp.files = comp.files[:]

#elif test==2:
#    selectedComponents = [ TTJets_50ns ]
#    isZSkim=True
#    for comp in selectedComponents:
#        comp.triggers = triggers_mumu
#        comp.splitFactor = 1
#        comp.files = ['/afs/cern.ch/user/d/dalfonso/public/TTbarMadP850ns/0066F143-F8FD-E411-9A0B-D4AE526A0D2E.root']
##        comp.files = comp.files[:1]



   # ----------------------- Summer15 options -------------------------------------------------------------------- #
elif test==2:
    isData = False
    selectedComponents = [ DYJetsToLL_M50_50ns ]
    isZSkim=True
    for comp in selectedComponents:
        comp.triggers = triggers_mumu
        comp.splitFactor = 1
        comp.files = comp.files[:1]

elif test==3:
    isData = False
    isZSkim=True
    selectedComponents = [ DYJetsToLL_M50_50ns,TTJets_50ns ]
    for comp in selectedComponents:
        if isEle:
            comp.triggers = triggers_ee
        else:
            comp.triggers = triggers_mumu
        comp.splitFactor = 1000
        comp.files = comp.files[:]

elif test==4:
    isData = False
    selectedComponents = [ WJetsToLNu_50ns ]
    is1L=False
    for comp in selectedComponents:
        comp.splitFactor = 1000
        comp.files = comp.files[:]

elif test==5:
    isData = False
    selectedComponents = QCDPt_50ns
    isDiJet=True
    for comp in selectedComponents:
        comp.splitFactor = 1000
        comp.files = comp.files[:]

    # ------------------------------------------------------------------------------------------- #

### this is for the Zskim
elif test==13:
    isData = True
    isZSkim=True
    if isEle:
        if is25ns:
            selectedComponents = [ DoubleEG_Run2015C ]
        else:
            selectedComponents = [ DoubleEG_Run2015B ]
    else:
        if is25ns:
            selectedComponents = [ DoubleMuon_Run2015C ]
        else:
            selectedComponents = [ DoubleMuon_Run2015B ]

    for comp in selectedComponents:
#        comp.splitFactor = 1
#        comp.files = comp.files[:]
#        comp.fineSplitFactor = 1
        comp.splitFactor = 1000
        comp.files = comp.files[:]
        if isEle:
            comp.triggers = triggers_ee
        else:
            comp.triggers = triggers_mumu
        comp.json = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data/json/Cert_246908-254879_13TeV_PromptReco_Collisions15_JSON.json"
        comp.intLumi= 0.04003
        print comp

### this is for the Wskim
elif test==14:
    isData = True
    is1L=False
    selectedComponents = [ SingleMuon_Run2015B ]
    for comp in selectedComponents:
        comp.splitFactor = 1000
        comp.files = comp.files[:]
        comp.json = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data/json/Cert_246908-254879_13TeV_PromptReco_Collisions15_JSON.json"
        comp.intLumi= 0.04003

### this is for the QCDlike
elif test==15:
    isData = True
    isDiJet=True
    if isEarlyRun:
        selectedComponents = [ JetHT_Run2015B_17Jul, HTMHT_Run2015B_17Jul, MET_Run2015B_17Jul ]
    else:
        selectedComponents = [ JetHT_Run2015B, HTMHT_Run2015B, MET_Run2015B ]
    for comp in selectedComponents:
        comp.splitFactor = 1000
        comp.files = comp.files[:]
        comp.json = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data/json/Cert_246908-254879_13TeV_PromptReco_Collisions15_JSON.json"
        comp.intLumi= 0.04003
        if isEarlyRun:
            comp.run_range=(251027,251585) # in 17july runInJSON: 251244,251251,251252,251561,251562
        else:
            comp.run_range=(251585,251883) # in promptReco runInJSON: 251643,251721,251883
        print comp

### this is for the PhotonSkim
elif test==16:
    isData = True
    is1PH=True
    selectedComponents = [ SinglePhoton_Run2015B ]
    for comp in selectedComponents:
        comp.triggers = triggers_photon30
        comp.splitFactor = 1
        comp.files = comp.files[:]
        comp.json = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data/json/Cert_246908-254879_13TeV_PromptReco_Collisions15_JSON.json"
        comp.intLumi= 0.04003


    # ------------------------------------------------------------------------------------------- #
    #        --> 25ns here

elif test==23:
    isData = False
    isZSkim=True
    is25ns=True
    selectedComponents = [ DYJetsToLL_M50 ]
    for comp in selectedComponents:
        if isEle:
            comp.triggers = triggers_ee
        else:
            comp.triggers = triggers_mumu
#        comp.splitFactor = 1
#        comp.files = comp.files[:1]
        comp.splitFactor = 1000
        comp.files = comp.files[:]

    # ------------------------------------------------------------------------------------------- #
    # ------------------------------------------------------------------------------------------- #

from CMGTools.ObjectStudies.analyzers.metCoreModules_cff import *

cfg.Analyzer.nosubdir = True

##------------------------------------------
##  PRODUCER
##------------------------------------------


from CMGTools.ObjectStudies.analyzers.treeProducerMET import *

treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerMET',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     PDFWeights = PDFWeights,
     globalVariables = met_globalVariables,
     globalObjects = met_globalObjects,
     collections = met_collections,
     defaultFloatType = 'F',
     treename = 'METtree'
)

##------------------------------------------
##  SEQUENCE
##------------------------------------------

metAna.doTkMet = True
metAna.doSpecialMet = False

metSequence = cfg.Sequence(
    metCoreSequence + [treeProducer]
    )

###---- to switch off the comptrssion
#treeProducer.isCompressed = 0

# -------------------- lepton modules below needed for the Muon Selection

if isZSkim:
    ttHLepSkim.ptCuts = [20,10]
    ttHLepSkim.minLeptons = 2
    if isZSkim and isEle:
        ttHLepSkim.ptCuts = [25,15]
        ttHZskim.lepId=[11] ## default is set To Muons
    metSequence.insert(metSequence.index(lepAna)+1,ttHLepSkim)
    metSequence.insert(metSequence.index(lepAna)+2,ttHZskim)

if is1L:
    ttHLepSkim.minLeptons = 1
    metSequence.insert(metSequence.index(lepAna)+1,ttHLepSkim)

if comp.isData and not isEarlyRun:
    eventFlagsAna.processName = 'RECO'

if comp.isData and comp.json is None:
    metSequence.remove(jsonAna)

# --------------------

triggerFlagsAna.triggerBits = {
            'SingleMu' : triggers_1mu_iso_50ns, # [ 'HLT_IsoMu17_eta2p1_v*', 'HLT_IsoTkMu17_eta2p1_v*'  ] + [ 'HLT_IsoMu20_v*', 'HLT_IsoTkMu20_v*'  ]
            'DoubleMu' : triggers_mumu, # [ "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*" ]
            'DoubleEG' : triggers_ee, # [ "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*" ]
            'Photon30' : triggers_photon30, #["HLT_Photon30_R9Id90_HE10_IsoM_v*"]
}


# ------------------------------------------------------------------------------------------- #
##------------------------------------------
##  SERVICES
##------------------------------------------

from PhysicsTools.HeppyCore.framework.services.tfile import TFileService 
output_service = cfg.Service(
      TFileService,
      'outputfile',
      name="outputfile",
      fname='METtree.root',
      option='recreate'
    )


# the following is declared in case this cfg is used in input to the heppy.py script                                                                                           
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events

# -------------------- Running Download from EOS

from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
if getHeppyOption("nofetch"):
    event_class = Events 


# -------------------- Running pre-processor
import subprocess
if isData:
    removeResiduals = False
    uncFile = os.environ['CMSSW_BASE']+'/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt'
    jecDBFile = os.environ['CMSSW_BASE']+'/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA.db'
    jecEra    = 'Summer15_50nsV4_DATA'
else:
    if not is25ns:
        removeResiduals = False
        uncFile = os.environ['CMSSW_BASE']+'/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_MC_UncertaintySources_AK4PFchs.txt'
        jecDBFile = os.environ['CMSSW_BASE']+'/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_MC.db'
        jecEra    = 'Summer15_50nsV4_MC'
    else:
        removeResiduals = True
        uncFile = os.environ['CMSSW_BASE']+'/src/CMGTools/RootTools/data/jec/Summer15_25nsV2_MC_UncertaintySources_AK4PFchs.txt'
        jecDBFile = os.environ['CMSSW_BASE']+'/src/CMGTools/RootTools/data/jec/Summer15_25nsV2_MC.db'
        jecEra    = 'Summer15_25nsV2_MC'

preprocessorFile = "$CMSSW_BASE/tmp/MetType1_jec_%s.py"%(jecEra)
extraArgs=[]
if isData:
  extraArgs.append('--isData')
  GT= '74X_dataRun2_Prompt_v1'
else:
  GT= 'MCRUN2_74_V9A'
if removeResiduals:extraArgs.append('--removeResiduals')
args = ['python', 
  os.path.expandvars(os.environ['CMSSW_BASE']+'/python/CMGTools/ObjectStudies/corMETMiniAOD_cfgCreator.py'),\
  '--GT='+GT, 
  '--outputFile='+preprocessorFile, 
  '--jecDBFile='+jecDBFile,
  '--jecEra='+jecEra
  ] + extraArgs 
#print "Making pre-processorfile:"
#print " ".join(args)
subprocess.call(args)
from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
preprocessor = CmsswPreprocessor(preprocessorFile)

#printComps(config.components, True)               
config = cfg.Config( components = selectedComponents,
                     sequence = metSequence,
                     services = [output_service],
                     preprocessor=preprocessor, # comment if pre-processor non needed
#                     events_class = event_class)
                     events_class = Events)

#printComps(config.components, True)
        
