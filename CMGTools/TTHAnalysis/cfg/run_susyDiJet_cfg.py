import PhysicsTools.HeppyCore.framework.config as cfg
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *


jsonAna.useLumiBlocks = True

#------------------------------------------
## Redefine analyzer parameters
##------------------------------------------

# Muons
#------------------------------
lepAna.loose_muon_dxy = 0.2
lepAna.loose_muon_dz  = 0.5
lepAna.loose_muon_relIso  = 0.15
lepAna.loose_muon_isoCut = lambda muon :muon.miniRelIso < 0.2

lepAna.loose_electron_pt  = 5
lepAna.loose_electron_eta    = 2.4
lepAna.loose_electron_relIso = 0.15
lepAna.loose_electron_isoCut = lambda electron : electron.miniRelIso < 0.1

lepAna.loose_electron_id  = "POG_Cuts_ID_SPRING15_25ns_v1_ConvVetoDxyDz_Veto_full5x5"
lepAna.loose_electron_lostHits = 999. # no cut
lepAna.loose_electron_dxy    = 999.
lepAna.loose_electron_dz     = 999.

lepAna.inclusive_electron_id  = "POG_Cuts_ID_SPRING15_25ns_v1_ConvVetoDxyDz_Veto_full5x5"
lepAna.inclusive_electron_lostHits = 999. # no cut since embedded in ID
lepAna.inclusive_electron_dxy    = 999. # no cut since embedded in ID
lepAna.inclusive_electron_dz     = 999. # no cut since embedded in ID

lepAna.mu_isoCorr = "deltaBeta"
lepAna.ele_isoCorr = "deltaBeta"
lepAna.ele_tightId = "Cuts_PHYS14_25ns_v1_ConvVetoDxyDz"
lepAna.notCleaningElectrons = True
lepAna.doMiniIsolation = True
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.ele_effectiveAreas = 'Spring15_25ns_v1'             #new default 
lepAna.mu_effectiveAreas = 'Spring15_25ns_v1'              #new default
lepAna.rhoMuon= 'fixedGridRhoFastjetCentralNeutral',      #new default
lepAna.rhoElectron = 'fixedGridRhoFastjetCentralNeutral', #new default


lepAna.doIsoAnnulus = True

# Photons
#------------------------------
photonAna.ptMin                        = 25,
photonAna.epaMax                       = 2.5,

# Taus 

jetAna.relaxJetId = False
jetAna.doPuId = False
jetAna.doQG = False
jetAna.jetEta = 4.7
jetAna.jetEtaCentral = 2.5
jetAna.jetPt = 30.
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
   jetPtCuts = [100,40,30],  # e.g. [60,40,30,20] to require at least four jets with pt > 60,40,30,20
   jetVetoPt =  70,  # if non-zero, veto additional jets with pt > veto beyond the ones in jetPtCuts
   metCut    =  0,  # MET cut
   htCut     = ('htJet40j', 0), # cut on HT defined with only jets and pt cut 40, at zero; i.e. no cut
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
from CMGTools.TTHAnalysis.analyzers.hbheAnalyzer import hbheAnalyzer
hbheFilterAna = cfg.Analyzer(
    hbheAnalyzer, name = 'hbheAnalyzer',IgnoreTS4TS5ifJetInLowBVRegion=False
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
                        hbheFilterAna,
                        treeProducer,
                        ])

sequence.insert(sequence.index(metAna),
                metNoHFAna)


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import triggers_dijet55met110,  triggers_MET170_JetIdCleaned, triggers_Jet80MET90, triggers_Jet80MET120, triggers_1mu_iso, triggers_1e, triggers_mumu, triggers_ee, triggers_MET170_NotCleaned

triggerFlagsAna.triggerBits = {
'PFMET170_JetId' : triggers_MET170_JetIdCleaned,
'PFMET170' : triggers_MET170_NotCleaned,
'SingleMu' : triggers_1mu_iso,
'SingleEl' : triggers_1e,
'DoubleMu' : triggers_mumu,
'DoubleEl' : triggers_ee,
'DiCentralPFJet55_PFMET110' : triggers_dijet55met110,
'MonoCentralPFJet80_PFMET90' : triggers_Jet80MET90,
'MonoCentralPFJet80_PFMET120' : triggers_Jet80MET120,
}
#------------------------------------------
##  PRODUCER
##---------------

#-------- SAMPLES AND TRIGGERS -----------

#-------- HOW TO RUN
#isData =True
isData =True
runPreprocessor = False
#sample = 'data'
sample = 'MC'
test = 1 



if sample == "MC":


        from CMGTools.RootTools.samples.samples_13TeV_RunIISpring15MiniAODv2 import *
#	from CMGTools.RootTools.samples.samples_13TeV_74X import *


        from CMGTools.RootTools.samples.samples_13TeV_74X_susySignalsPriv import *
	
	# select components
	#selectedComponents = [
        #SMS_T2cc_mstop400_mLSP390,SMS_T2bb_2J_mStop900_mLSP100_Fast,TTJets, TToLeptons_tch, TToLeptons_sch, TBar_tWch, T_tWch,
        #DYJetsToLL_M50_HT100to200, DYJetsToLL_M50_HT200to400, DYJetsToLL_M50_HT400to600, DYJetsToLL_M50_HT600toInf, WJetsToLNu 
#
#	]
	selectedComponents = [
        TTJets_LO
       # SMS_T2bb_2J_mStop900_mLSP100_Fast,
       # DYJetsToLL_M50_HT100to200, DYJetsToLL_M50_HT200to400, DYJetsToLL_M50_HT400to600, DYJetsToLL_M50_HT600toInf

	]

	if test==1:
		# test a single component, using a single thread.
		comp = TTJets_LO 
		comp.files = comp.files[:1]
		selectedComponents = [comp]
		comp.splitFactor = 1
	elif test==2:
		# test all components (1 thread per component).
		for comp in selectedComponents:
			comp.splitFactor = 1
			comp.fineSplitFactor = 2
			comp.files = comp.files[:1]
	elif test==3:
		# run all components (1 thread per component).
		for comp in selectedComponents:
			comp.fineSplitFactor = 2
			comp.splitFactor = len(comp.files)

elif sample == "data":




	# central samples
        from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

	selectedComponents = [ SingleElectron_Run2015D_05Oct, SingleMuon_Run2015D_05Oct]

	if test==1:
		# test a single component, using a single thread.
		comp = SingleElectron_Run2015D_05Oct
		comp.files = comp.files[:2]
		selectedComponents = [comp]
		comp.splitFactor = 1
	elif test==2:
		# test all components (1 thread per component).
		for comp in selectedComponents:
			comp.splitFactor = 1
			comp.fineSplitFactor = 2
			comp.files = comp.files[:1]
	elif test==3:
		# PRODUCTION
		# run on everything
		for comp in selectedComponents:
			comp.fineSplitFactor = 1  # force for DATA (because of pre-processor)
			comp.splitFactor = len(comp.files)
                        comp.trigger = triggers_dijet55met110 +  triggers_MET170_JetId + triggers_Jet80MET90 + triggers_Jet80MET120 + triggers_1mu_iso + triggers_1e + triggers_MET170_JetIdCleaned+ triggers_MET170_NotCleaned




if runPreprocessor:
    removeResiduals = False
    # -------------------- Running pre-processor
    import subprocess

    if isData:
        #    uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt'
        #    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA.db'
        #    jecEra    = 'Summer15_50nsV4_DATA'
        uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_25nsV5_DATA_UncertaintySources_AK4PFchs.txt'
        jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_25nsV5_DATA.db'
        jecEra    = 'Summer15_25nsV5_DATA'
    else:
        #    uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt'
        #    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_MC.db'
        #    jecEra    = 'Summer15_50nsV4_MC'
        uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_25nsV5_MC_UncertaintySources_AK4PFchs.txt'
        jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_25nsV5_MC.db'
        jecEra    = 'Summer15_25nsV5_MC'
    preprocessorFile = "$CMSSW_BASE/tmp/MetType1_jec_%s.py"%(jecEra)
    extraArgs=[]
    if isData:
        extraArgs.append('--isData')
        GT= '74X_dataRun2_Prompt_v1'
    else:
        GT= 'MCRUN2_74_V9A'
    if removeResiduals:extraArgs.append('--removeResiduals')
    args = ['python',
            os.path.expandvars('$CMSSW_BASE/python/CMGTools/ObjectStudies/corMETMiniAOD_cfgCreator.py'),\
                '--GT='+GT,
            '--outputFile='+preprocessorFile,
            '--jecDBFile='+jecDBFile,
            '--uncFile='+uncFile,
            '--jecEra='+jecEra
            ] + extraArgs
    #print "Making pre-processorfile:"
    #print " ".join(args)
    subprocess.call(args)
    from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
    preprocessor = CmsswPreprocessor(preprocessorFile)

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events

config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],
                     #preprocessor=preprocessor, # comment if pre-processor non needed
                     events_class = Events)


