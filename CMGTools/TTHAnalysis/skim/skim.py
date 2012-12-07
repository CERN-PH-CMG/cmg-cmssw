import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan


runOnMC      = True
runOld5XGT   = False
runOnFastSim = False
runOnCMGV56  = False
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
    #'/MuEG/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_10_0_runrange_start-203002'
   )


## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

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
#### SKIMMING
########################################################
process.muForSkim = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("cmgMuonSel"),
    cut = cms.string("pt > 5 && sourcePtr.track.isNonnull && abs(sourcePtr.dB('PV3D')/sourcePtr.edB('PV3D')) < 100"),
)
process.eleForSkim = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("cmgElectronSel"),
    cut = cms.string("pt > 7 && abs(sourcePtr.dB('PV3D')/sourcePtr.edB('PV3D')) < 100"),
)
process.lepForSkim = cms.EDProducer("CandViewMerger", 
    src = cms.VInputTag(cms.InputTag("muForSkim"), cms.InputTag("eleForSkim"))
)

process.centralJetsForSkim = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string("pt > 25 && abs(eta) < 3 && getSelection('cuts_looseJetId') && passLooseFullPuJetId"),
)

process.triLepFilter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("lepForSkim"), minNumber = cms.uint32(3))
process.diJetFilter  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("centralJetsForSkim"), minNumber = cms.uint32(2))

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

process.skimSequence = cms.Sequence(
    process.muForSkim + process.eleForSkim + 
    process.lepForSkim + process.triLepFilter +
    process.triLepCandidate + process.triLepCleanFilter +
    process.centralJetsForSkim + process.diJetFilter +
    process.diJetCandidate + process.diJetCleanFilter +
    process.llljjCandidate + process.llljjFilter
)

process.p = cms.Path(
    ##~process.genHBB +
    process.goodPrimaryVertices+
    process.muonSequence +
    process.electronSequence +
    process.skimSequence
)
if runOnCMGV56:
    process.p.remove(process.electronSequence)

########################################################
## CMG output definition
########################################################

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
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

