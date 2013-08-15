import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan


runOnMC      = True
runOld5XGT   = False
runOnFastSim = False
runOnCMGV56  = False
LOOSE_ID     = "&& abs(sourcePtr.dB('PV3D')/sourcePtr.edB('PV3D')) < 100"
LOOSE_ID     = LOOSE_ID + "&& chargedHadronIso/pt < 0.4"
process = cms.Process("CMG")


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
  sourceSeed = cms.untracked.uint32(414000),
  moduleSeeds = cms.PSet(
    patElectronsWithTrigger = cms.untracked.uint32(1041963),
  )
)


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/TTH_Inclusive_M-125_8TeV_pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5/PAT_CMG_V5_10_0',
    #'/WZJetsTo3LNu_TuneZ2_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_10_0',
    #'/TTTo2L2Nu2B_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
    #'/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
    #'/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0'
    #'/DoubleMu/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_10_0_runrange_start-203002'
   )


## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

########################################################
#### VERTICES
########################################################
process.goodPrimaryVertices = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    filter = cms.bool(True),
)

########################################################
#### MUONS
########################################################
process.correctedMuons = cms.EDProducer('RochesterPATMuonCorrector',
    src = cms.InputTag("patMuonsWithTrigger")
)

process.patMuonsWithTrigger = cms.EDProducer('CMGPATMuonCleanerBySegments',
    src = cms.InputTag("correctedMuons"),
    preselection = cms.string("track.isNonnull"),
    passthrough = cms.string("isGlobalMuon && numberOfMatches >= 2"),
    fractionOfSharedSegments = cms.double(0.499)
)

process.cmgMuon.cfg.inputCollection = 'patMuonsWithTrigger'

process.muonSequence = cms.Sequence(
    process.correctedMuons+process.patMuonsWithTrigger+process.cmgMuon+process.cmgMuonSel
)

########################################################
#### ELECTRONS
########################################################
process.patElectronsWithRegression = cms.EDProducer("RegressionEnergyPatElectronProducer",
    debug = cms.untracked.bool(False),
    inputPatElectronsTag = cms.InputTag('patElectronsWithTrigger'),
    regressionInputFile = cms.string("EGamma/EGammaAnalysisTools/data/eleEnergyRegWeights_V1.root"),
    energyRegressionType = cms.uint32(1),
    rhoCollection = cms.InputTag('kt6PFJets:rho:RECO'),
    vertexCollection = cms.InputTag('goodPrimaryVertices')
)


process.patElectronsWithTrigger = cms.EDProducer("CalibratedPatElectronProducer",
    inputPatElectronsTag = cms.InputTag("patElectronsWithRegression"),
    isMC = cms.bool(runOnMC),
    isAOD = cms.bool(False),
    debug = cms.bool(False),
    updateEnergyError = cms.bool(True),
    applyCorrections = cms.int32(1),
    inputDataset = cms.string("Summer12_DR53X_HCP2012" if runOnMC else "2012Jul13ReReco"),
)
                                                 
process.cmgElectron.cfg.inputCollection = 'patElectronsWithTrigger'
process.cmgElectron.cfg.updateConversionVeto = cms.bool(False) ## needed for CMG-on-CMG

process.electronSequence = cms.Sequence(
    process.patElectronsWithRegression +
    process.patElectronsWithTrigger +
    process.cmgElectron +
    process.cmgElectronSel
)

########################################################
#### GEN H->BB VETO
########################################################
process.genHBB = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticlesPruned"),
    cut = cms.string("pdgId == 25 && (daughterRef(0).pdgId) == 5"),
    filter = cms.bool(True),
)

########################################################
#### GEN-LEVEL SKIMMING
########################################################
process.genHWWZZtt = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticlesPruned"),
    cut = cms.string("pdgId == 25 && (abs(daughterRef(0).pdgId) == 24 || abs(daughterRef(0).pdgId) == 23 || abs(daughterRef(0).pdgId) == 15)"),
    filter = cms.bool(True),
)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.leptonsFromHardScattering1 = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticlesPruned"),
    select = cms.vstring(
        "drop  *  ", # this is the default
        "keep++ (abs(pdgId) = 11 || abs(pdgId) == 13 || abs(pdgId) == 15) && status == 3", 
    )
)
process.leptonsFromHardScattering = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("leptonsFromHardScattering1"),
    cut = cms.string("status == 1 && (abs(pdgId) == 11 && pt > 7 && abs(eta) < 2.5 ||"+
                                    " abs(pdgId) == 13 && pt > 5 && abs(eta) < 2.4)"),
)
process.bQuarksFromTopNoACC = cms.EDFilter("GenParticleSelector",
    src = cms.InputTag("genParticlesPruned"),
    cut = cms.string("status == 3 && abs(pdgId) == 5 && numberOfMothers > 0 && abs(motherRef.pdgId()) == 6"),
)
process.dileptonsFromHardScattering = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("leptonsFromHardScattering leptonsFromHardScattering"),
    cut = cms.string("deltaR(daughter(0).eta, daughter(0).phi, daughter(1).eta, daughter(1).phi) > 0.1"),
    checkCharge = cms.bool(False),
)
process.diBQuarks = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("bQuarksFromTopNoACC bQuarksFromTopNoACC"),
    cut = cms.string("deltaR(daughter(0).eta, daughter(0).phi, daughter(1).eta, daughter(1).phi) > 0.4"),
    checkCharge = cms.bool(False),
)
process.genSeq = cms.Sequence(
    process.genHWWZZtt+
    process.leptonsFromHardScattering1+process.leptonsFromHardScattering+process.dileptonsFromHardScattering+
    process.bQuarksFromTopNoACC + process.diBQuarks
)

process.triLepFilterGen  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("leptonsFromHardScattering"), minNumber = cms.uint32(3))
process.fourLepFilterGen = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("leptonsFromHardScattering"), minNumber = cms.uint32(4))
process.diLepFilterGen2010 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("dileptonsFromHardScattering"),
    cut = cms.string("max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 10"),
    filter = cms.bool(True),
)
process.diLepFilterGen2010SS = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("dileptonsFromHardScattering"),
    cut = cms.string("max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 10 && charge != 0"),
    filter = cms.bool(True),
)
process.diLepFilterGen2015SS = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("dileptonsFromHardScattering"),
    cut = cms.string("max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 15 && charge != 0"),
    filter = cms.bool(True),
)
process.diLepFilterGen2020SS = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("dileptonsFromHardScattering"),
    cut = cms.string("max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 20 && charge != 0"),
    filter = cms.bool(True),
)
process.diBQuarkEta30Pt20 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("diBQuarks"),
    cut = cms.string("max(abs(daughter(0).eta), abs(daughter(1).eta)) < 3.0 && min(daughter(0).pt, daughter(1).pt) > 20"),
    filter = cms.bool(True),
)
process.diBQuarkEta25Pt20 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("diBQuarks"),
    cut = cms.string("max(abs(daughter(0).eta), abs(daughter(1).eta)) < 2.5 && min(daughter(0).pt, daughter(1).pt) > 20"),
    filter = cms.bool(True),
)
process.diBQuarkEta25Pt30 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("diBQuarks"),
    cut = cms.string("max(abs(daughter(0).eta), abs(daughter(1).eta)) < 2.5 && min(daughter(0).pt, daughter(1).pt) > 30"),
    filter = cms.bool(True),
)

if False: ## set to True to compute gen-level acceptances
    process.GEN_WW_ZZ_tt = cms.Path(process.genSeq)
    process.ACC_2L_2010 = cms.Path(process.genSeq + process.diLepFilterGen2010)
    process.ACC_2L_2010SS = cms.Path(process.genSeq + process.diLepFilterGen2010SS)
    process.ACC_2L_2015SS = cms.Path(process.genSeq + process.diLepFilterGen2015SS)
    process.ACC_2L_2020SS = cms.Path(process.genSeq + process.diLepFilterGen2020SS)
    process.ACC_3L_2010 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.triLepFilterGen)
    process.ACC_4L_2010 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.fourLepFilterGen)
    process.ACC_2L_2010_2B_3020 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.diBQuarkEta30Pt20)
    process.ACC_2L_2010_2B_2520 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.diBQuarkEta25Pt20)
    process.ACC_2L_2010_2B_2530 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.diBQuarkEta25Pt30)
    process.ACC_3L_2010_2B_2530 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.triLepFilterGen + process.diBQuarkEta25Pt30)
    process.ACC_4L_2010_2B_2530 = cms.Path(process.genSeq + process.diLepFilterGen2010 + process.fourLepFilterGen + process.diBQuarkEta25Pt30)
    process.ACC_2L_2010SS_2B_2530 = cms.Path(process.genSeq + process.diLepFilterGen2010SS + process.diBQuarkEta25Pt30)
    process.ACC_2L_2015SS_2B_2530 = cms.Path(process.genSeq + process.diLepFilterGen2015SS + process.diBQuarkEta25Pt30)
    process.ACC_2L_2020SS_2B_2530 = cms.Path(process.genSeq + process.diLepFilterGen2020SS + process.diBQuarkEta25Pt30)


########################################################
#### SKIMMING
########################################################
process.muForSkim = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("cmgMuonSel"),
    cut = cms.string("pt > 5 && abs(eta)<2.4 && (isGlobalMuon || (isTrackerMuon && numberOfMatches>0)) "+LOOSE_ID),
)

process.eleForSkim = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("cmgElectronSel"),
    cut = cms.string("pt > 7 && abs(eta)<2.5 "+LOOSE_ID),
)
process.lepForSkim = cms.EDProducer("CandViewMerger", 
    src = cms.VInputTag(cms.InputTag("muForSkim"), cms.InputTag("eleForSkim"))
)

process.centralJetsForSkim = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string("pt > 20 && abs(eta) < 3 && getSelection('cuts_looseJetId') && passLooseFullPuJetId"),
)

process.triLepFilter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("lepForSkim"), minNumber = cms.uint32(3))
process.diJetFilter  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("centralJetsForSkim"), minNumber = cms.uint32(2))

process.diLepCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("lepForSkim lepForSkim"),
    cut = cms.string("deltaR(daughter(0).eta, daughter(0).phi, daughter(1).eta, daughter(1).phi)>0.02"),
    checkCharge = cms.bool(False),
)
process.triLepCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("lepForSkim lepForSkim lepForSkim"),
    cut = cms.string("deltaR(daughter(0).eta, daughter(0).phi, daughter(1).eta, daughter(1).phi)>0.02 &&"+
                     "deltaR(daughter(0).eta, daughter(0).phi, daughter(2).eta, daughter(2).phi)>0.02 &&"+
                     "deltaR(daughter(1).eta, daughter(1).phi, daughter(2).eta, daughter(2).phi)>0.02   "),
    checkCharge = cms.bool(False),
)
process.diJetCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("centralJetsForSkim centralJetsForSkim"),
    cut = cms.string("deltaR(daughter(0).eta, daughter(0).phi, daughter(1).eta, daughter(1).phi) > 0.3"),
    checkCharge = cms.bool(False),
)
process.llljjCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("triLepCandidate diJetCandidate"),
    cut = cms.string("deltaR(daughter(0).daughter(0).eta, daughter(0).daughter(0).phi, daughter(1).daughter(0).eta, daughter(1).daughter(0).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(0).eta, daughter(0).daughter(0).phi, daughter(1).daughter(1).eta, daughter(1).daughter(1).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(1).eta, daughter(0).daughter(1).phi, daughter(1).daughter(0).eta, daughter(1).daughter(0).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(1).eta, daughter(0).daughter(1).phi, daughter(1).daughter(1).eta, daughter(1).daughter(1).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(2).eta, daughter(0).daughter(2).phi, daughter(1).daughter(0).eta, daughter(1).daughter(0).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(2).eta, daughter(0).daughter(2).phi, daughter(1).daughter(1).eta, daughter(1).daughter(1).phi)>0.3"),
    checkCharge = cms.bool(False),
)
process.triLepCleanFilter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("triLepCandidate"), minNumber = cms.uint32(1))
process.diJetCleanFilter  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("diJetCandidate"), minNumber = cms.uint32(1))
process.llljjFilter  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("llljjCandidate"), minNumber = cms.uint32(1))

process.diLepFilter2010 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("diLepCandidate"),
    cut = cms.string("max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 10"),
    filter = cms.bool(True),
)
process.diLepFilter2010SS = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("diLepCandidate"),
    cut = cms.string("max(daughter(0).pt, daughter(1).pt) > 20 && min(daughter(0).pt, daughter(1).pt) > 10 && charge != 0"),
    filter = cms.bool(True),
)

process.llssjjCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("diLepFilter2010SS diJetCandidate"),
    cut = cms.string("deltaR(daughter(0).daughter(0).eta, daughter(0).daughter(0).phi, daughter(1).daughter(0).eta, daughter(1).daughter(0).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(0).eta, daughter(0).daughter(0).phi, daughter(1).daughter(1).eta, daughter(1).daughter(1).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(1).eta, daughter(0).daughter(1).phi, daughter(1).daughter(0).eta, daughter(1).daughter(0).phi)>0.3 &&"+
                     "deltaR(daughter(0).daughter(1).eta, daughter(0).daughter(1).phi, daughter(1).daughter(1).eta, daughter(1).daughter(1).phi)>0.3"),
    checkCharge = cms.bool(False),
)
process.llssjjFilter  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("llssjjCandidate"), minNumber = cms.uint32(1))


process.skimSequenceBase = cms.Sequence(
    process.muForSkim + process.eleForSkim + 
    process.lepForSkim + 
    process.diLepCandidate + process.diLepFilter2010 +
    process.centralJetsForSkim + process.diJetFilter +
    process.diJetCandidate + process.diJetCleanFilter 
)
process.skimSequence3 = cms.Sequence(
    process.skimSequenceBase +
    process.triLepFilter     +
    process.triLepCandidate + process.triLepCleanFilter +
    process.llljjCandidate  + process.llljjFilter
)
process.skimSequence2SS = cms.Sequence(
    process.skimSequenceBase +
    process.diLepFilter2010SS + process.llssjjCandidate + process.llssjjFilter
)

process.p3L = cms.Path(
    process.goodPrimaryVertices+  process.muonSequence + process.electronSequence +
    process.skimSequence3
)
process.p2L = cms.Path(
    process.goodPrimaryVertices+  process.muonSequence + process.electronSequence +
    process.skimSequence2SS
)
process.pOverlap = cms.Path( ## useful for counting purposes: N(pass) = N(1) + N(2) - N(overlap)
    process.goodPrimaryVertices+  process.muonSequence + process.electronSequence +
    process.skimSequence2SS +
    process.skimSequence3
)

if runOnCMGV56:
    process.p3L.remove(process.electronSequence)
    process.p2L.remove(process.electronSequence)
    process.pOverlap.remove(process.electronSequence)

########################################################
## CMG output definition
########################################################

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p3L','p2L') ),
    outputCommands = cms.untracked.vstring(
    'keep *_*_*_*',
    'drop *_*_*_CMG',
    ## Things we don't use
    'drop *_cmgPFBaseJetLeadCHS_*_*',
    'drop *_cmgDiElectronSel_*_*',
    'drop *_cmgDiMuonSel_*_*',
    'drop *_cmgPFJetLooseJetIdFailed_*_*',
    'drop *_cmgPFJetMediumJetIdFailed_*_*',
    'drop *_cmgPFJetSelCHS_*_*',
    "drop *_cmgPFJetTightJetIdFailed_*_*" , 
    "drop *_cmgPFJetVeryLooseJetId95Failed_*_*", 
    "drop *_cmgPFJetVeryLooseJetId95gammaFailed_*_*", 
    "drop *_cmgPFJetVeryLooseJetId95h0Failed_*_*",  
    "drop *_cmgPFJetVeryLooseJetId99Failed_*_*",  
    "drop *_cmgStructuredPFJetSel_*_*",         
    "drop *_cmgTauSel_*_*",                     
    "drop *_tauGenJetsSelectorAllHadrons_*_*",
    ## Things we updated
    "drop *_cmgMuonSel_*_PAT",                     
    "drop *_patMuonsWithTrigger_*_PAT",                     
    "keep *_cmgMuonSel_*_CMG",                     
    "keep *_patMuonsWithTrigger_*_CMG",                     
    "drop *_cmgElectronSel_*_PAT",                     
    "drop *_patElectronsWithTrigger_*_PAT",                     
    "keep *_cmgElectronSel_*_CMG",                     
    "keep *_patElectronsWithTrigger_*_CMG",                     
    ),
    dropMetaData = cms.untracked.string('PRIOR')
)
if runOnCMGV56:
    process.outcmg.outputCommands += [ "keep *_cmgElectronSel_*_PAT", "keep *_patElectronsWithTrigger_*_PAT" ]

process.outpath = cms.EndPath(process.outcmg)




########################################################
## Conditions 
########################################################

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

process.GlobalTag.globaltag = getGlobalTag( runOnMC,runOld5XGT)
print 'Global tag       : ', process.GlobalTag.globaltag

########################################################
## Below, stuff that you probably don't want to modify
########################################################

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

print 'starting CMSSW'

