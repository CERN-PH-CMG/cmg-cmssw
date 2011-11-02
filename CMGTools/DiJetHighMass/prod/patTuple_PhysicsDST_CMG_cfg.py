## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

#process.source = cms.Source(
#	"PoolSource",
#	noEventSort = cms.untracked.bool(True),
#	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
#	fileNames = cms.untracked.vstring()
#)
process.source.fileNames=([
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/282/AE5C1890-9803-E111-8E41-001D09F2432B.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/278/EE30ECC8-9003-E111-9D15-001D09F2AF1E.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/275/D2E7031D-8503-E111-8DAD-BCAEC5329715.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/252/DCC46FC1-1A03-E111-B62C-BCAEC518FF89.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/F2E88F7B-F702-E111-8546-001D09F24399.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/F0A1CD60-F502-E111-BB6D-BCAEC53296F4.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/E2AAFE1A-F102-E111-8E18-003048D37580.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/D8665764-0103-E111-A28A-003048D2C174.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/D22B0F04-1303-E111-B814-003048D373AE.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/C0A6A36D-FC02-E111-9165-003048D3C90E.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/9C754AF0-FF02-E111-B8A2-001D09F250AF.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/96DF4E84-0303-E111-BCFE-BCAEC53296F2.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/9249D27E-F202-E111-A425-0025901D5C88.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/8AF3858E-0503-E111-BEE0-BCAEC532971A.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/82B568F9-F302-E111-B8C1-001D09F29524.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/6AE2CEFE-FA02-E111-BDF0-BCAEC518FF52.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/4AC25F40-FF02-E111-BBAC-003048F118C6.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/30BD9FD0-0703-E111-BD0C-003048F118AC.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/2E05E4C6-0903-E111-AD0F-BCAEC518FF89.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/250/22CFB9EE-F802-E111-A27C-0025901D62A6.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/249/F4AC64F3-F302-E111-A4DF-BCAEC5329700.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/CE141DDC-CA02-E111-96BA-BCAEC5329719.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/A89A653F-C402-E111-BBF2-BCAEC532970D.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/A2D09760-C802-E111-BED7-BCAEC5329705.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/862C6EE4-CE02-E111-8182-003048D2C0F2.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/7E9E983A-C602-E111-9953-003048D2BF1C.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/7639BA46-CE02-E111-A568-BCAEC5329716.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/74161FFF-CB02-E111-89B9-E0CB4E4408D5.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/68F9C7A7-D002-E111-8309-0030486733B4.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/4292EA83-D202-E111-9C52-BCAEC518FF56.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/3471EEB0-C102-E111-BC83-0025901D5D90.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/185D81CC-D402-E111-AA55-BCAEC5329702.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/241/02FA8063-DF02-E111-9E6E-003048F024FE.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/224/C4FCCBF9-8402-E111-9887-001D09F23C73.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/222/74F08284-7702-E111-8B34-001D09F26C5C.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/165/4AC03689-8301-E111-AEE9-001D09F291D7.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/163/0E443A67-8201-E111-85D2-BCAEC518FF6E.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/154/12A4F62D-6401-E111-928E-BCAEC518FF30.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/153/AC62FA88-5701-E111-A9D5-BCAEC5329725.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/093/CED8CD64-0B01-E111-9D55-BCAEC518FF8D.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/093/A801A4FC-0801-E111-92C8-003048F024E0.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/093/34E80A33-0D01-E111-909F-485B39897227.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/093/20101437-0A01-E111-98D4-001D09F2924F.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/E4427B2D-ED00-E111-A2D0-BCAEC53296FD.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/DC6BE69B-E800-E111-8DF3-BCAEC518FF89.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/D6C6EB8C-0601-E111-98F7-BCAEC5329700.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/B4E30EFB-F200-E111-B950-BCAEC518FF68.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/A6AD0D06-F100-E111-A770-001D09F29321.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/A28343A6-E400-E111-B326-BCAEC5329702.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/807CA99B-DF00-E111-94A6-BCAEC53296FB.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/076/7ECD0BF5-0601-E111-AA03-BCAEC518FF5A.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/075/C2456F11-F800-E111-B04A-003048D374CA.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/072/6CBFE765-D000-E111-A234-003048D3C90E.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/072/369572E0-F800-E111-A718-001D09F2905B.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/180/072/00B91EBD-D700-E111-AC34-BCAEC518FF30.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/179/977/D449A7F4-3F00-E111-A157-003048F117F6.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/179/977/B844B113-4200-E111-B0A6-003048F1183E.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/179/977/A2EA92E8-4B00-E111-B772-003048F1C424.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/179/977/7E9AE477-4300-E111-A61C-002481E0D524.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/179/959/E6DBF3CE-1500-E111-9502-00237DDC5CB0.root',
		'/store/data/Run2011B/PhysicsDST/RAW/v1/000/179/959/CC1F559B-1800-E111-97EC-003048F01E88.root',
])

# COLIN : Cert JSON not ready, too bad.
from CMGTools.Common.Tools.applyJSON_cff import *
json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/DCSOnly/json_DCSONLY.txt'
applyJSON(process, json )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

print process.source.fileNames

process.GlobalTag.globaltag = cms.string('GR_R_42_V12::All')

print process.GlobalTag.globaltag

process.out.fileName = cms.untracked.string('patTuple_PhysicsDST.root' )

# trigger filters
process.hltCaloPath = cms.EDFilter('HLTHighLevel',
    TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    HLTPaths           = cms.vstring('DST_FatJetMass300_DR1p1_Deta2p0_v*',
                                     ),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(False)
)

process.hltPFPath = cms.EDFilter('HLTHighLevel',
    TriggerResultsTag  = cms.InputTag('TriggerResults','','HLT'),
    HLTPaths           = cms.vstring('DST_FatJetMass400_DR1p1_Deta2p0_RunPF_v*',
                                     'DST_HT350_RunPF_v*',
                                     ),
    eventSetupPathsKey = cms.string(''),
    andOr              = cms.bool(True), #----- True = OR, False = AND between the HLTPaths
    throw              = cms.bool(False)
)

# PAT sequence
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching( process ) 
removeCleaning( process ) 
removeSpecificPATObjects( process, ['Electrons','Photons','METs','Taus'])

process.patMuons.addTeVRefits = cms.bool(False)
process.patMuons.embedCaloMETMuonCorrs = cms.bool(False)
process.patMuons.embedCombinedMuon = cms.bool(False)
process.patMuons.embedPFCandidate = cms.bool(False)
process.patMuons.embedHighLevelSelection = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedTcMETMuonCorrs = cms.bool(False)
process.patMuons.embedTpfmsMuon = cms.bool(False)
process.patMuons.muonSource = cms.InputTag("hltMuons")
process.patMuons.pvSrc = cms.InputTag("hltPixelVertices")

from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection( process, cms.InputTag('hltAntiKT5PFJets'),
                     doJTA=False,
		     doBTagging=False,
		     jetCorrLabel=None, #('AK5PF',['L1FastJet','L2Relative','L3Absolute','L2L3Residual']),
		     doType1MET=False,
		     genJetCollection=None,
		     doJetID=False)
process.patJets.addDiscriminators = cms.bool(False)
process.patJets.embedCaloTowers = cms.bool(False)
process.patJets.embedGenJetMatch = cms.bool(False)
process.patJets.embedPFCandidates = cms.bool(False)
#process.patJetCorrFactors.primaryVertices = cms.InputTag("hltPixelVertices")
#process.patJetCorrFactors.rho = cms.InputTag("hltAntiKT5PFJets","rho")
#process.patJetCorrFactors.useNPV = cms.bool(False)
#process.patJetCorrFactors.useRho = cms.bool(True)

addJetCollection( process, cms.InputTag('hltAntiKT5CaloJetsSelected'),
                  algoLabel='AK5',
                  typeLabel='Calo',
                  doJTA=False,
		  doBTagging=False,
		  jetCorrLabel=None, #('AK5Calo',['L1Offset','L2Relative','L3Absolute','L2L3Residual']),
		  doType1MET=False,
		  doL1Cleaning=False,
		  doL1Counters=False,
		  genJetCollection=None,
		  doJetID=False)
process.patJetsAK5Calo.addDiscriminators = cms.bool(False)
process.patJetsAK5Calo.embedCaloTowers = cms.bool(False)
process.patJetsAK5Calo.embedGenJetMatch = cms.bool(False)
process.patJetsAK5Calo.embedPFCandidates = cms.bool(False)
#process.patJetCorrFactorsAK5Calo.primaryVertices = cms.InputTag("hltPixelVertices")
#process.patJetCorrFactorsAK5Calo.rho = cms.InputTag("hltAntiKT5PFJets","rho")
#process.patJetCorrFactorsAK5Calo.useNPV = cms.bool(True)
#process.patJetCorrFactorsAK5Calo.useRho = cms.bool(False)

addJetCollection( process , cms.InputTag('hltCaloJetCorrectedSelected'),
                  algoLabel='AK5',
                  typeLabel='CaloCor',
                  doJTA=False,
		  doBTagging=False,
		  jetCorrLabel=None,
		  doType1MET=False,
		  doL1Cleaning=False,
		  doL1Counters=False,
		  genJetCollection=None,
		  doJetID=False)
process.patJetsAK5CaloCor.addDiscriminators = cms.bool(False)
process.patJetsAK5CaloCor.addJetCorrFactors = cms.bool(False)
process.patJetsAK5CaloCor.embedCaloTowers = cms.bool(False)
process.patJetsAK5CaloCor.embedGenJetMatch = cms.bool(False)
process.patJetsAK5CaloCor.embedPFCandidates = cms.bool(False)

process.caloPatSequence = cms.Sequence(
    process.patJetsAK5CaloCor +
    process.patJetsAK5Calo
)

process.pfPatSequence = cms.Sequence(
    process.caloPatSequence +
    process.patMuons +
    process.patJets
)

process.out.outputCommands = cms.untracked.vstring('keep *',
						   'drop recoCaloJets_*_*_*',
						   'drop recoPFJets_*_*_*',
						   'drop recoMuons_*_*_*',
                                                   'drop recoPFCandidates_*_*_*',
						   'drop recoGenJets_*_*_*',
						   'drop CaloTowers_*_*_*',
						   'drop recoBaseTagInfoOwned_*_*_*',
						   )

# CMG analysis
process.load('CMGTools.Common.analysis_cff')
process.load('CMGTools.Common.factories.cmgFatJet_cfi')
process.load('CMGTools.Common.factories.cmgDiFatJet_cfi')

process.cmgPFJet.cfg.inputCollection = 'patJets'
process.cmgPFJet.cfg.useConstituents = False
process.cmgPFJet.cfg.baseJetFactory.fillJec = False
process.cmgCaloBaseJet.cfg.inputCollection = 'patJetsAK5Calo'
process.cmgCaloBaseJet.cfg.fillJec = False
process.cmgFatJet.cfg.inputCollection = 'cmgPFJetSel'

process.caloAnalysisSequence = cms.Sequence(
    process.cmgCaloBaseJet +
    process.cmgCaloBaseJetSel
)

process.pfAnalysisSequence = cms.Sequence(
    # PFJet factory wants to access PFCandidates.
    # implement a mode to work with JetSpecific
    process.cmgPFJet +
    process.cmgPFJetSel +
    # doing the fat jets only from PF jets for now. 
    process.cmgFatJet+
    process.cmgDiFatJet
)

process.caloAnalysis = cms.Path(
    process.hltCaloPath *
    process.caloPatSequence *
    process.caloAnalysisSequence
)

process.pfAnalysis = cms.Path(
    process.hltPFPath *
    process.pfPatSequence *
    process.pfAnalysisSequence
)

process.out.SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('caloAnalysis','pfAnalysis')
)

# from CMGTools.Common.eventContent.everything_cff import everything 
# process.out.outputCommands.extend( everything )

from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting
#process.out.outputCommands += runInfoAccounting

process.MessageLogger.cerr.FwkReport.reportEvery = 10

# CMG tuple
process.load('CMGTools.Common.analysis_cff')
#process.p += process.analysisSequence

from CMGTools.Common.eventContent.everything_cff import everything 

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_CMG.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = everything 
    )

#process.outpath += process.outcmg

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_CMG.root"))
