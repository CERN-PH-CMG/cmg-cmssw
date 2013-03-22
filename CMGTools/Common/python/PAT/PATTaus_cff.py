# from PhysicsTools.PatAlgos.patSequences_cff import *
from CMGTools.Common.Tools.cmsswRelease import isNewerThan

from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.tauSelector_cfi import selectedPatTaus

from RecoTauTag.Configuration.RecoPFTauTag_cff import *
PFTau.remove( recoTauClassicShrinkingConeSequence )
PFTau.remove( recoTauClassicShrinkingConeMVASequence )
PFTau.remove( recoTauHPSTancSequence )

# will insert a jet selector before tau ID to speed up processing
from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets
jetSelectionForTaus = 'pt()>15.0 && abs(eta())<3.0'
pfJetsForHPSTau = selectedPfJets.clone( src = 'ak5PFJets',
                                        cut = jetSelectionForTaus )
# from CMGTools.Common.Tools.visitorUtils import replaceSrc
# replaceSrc( PFTau, 'ak5PFJets', 'pfJetsForHPSTau')
tauSource =  'pfJetsForHPSTau'
recoTauAK5PFJets08Region.src = tauSource #NOTE this guy has a pfSrc
ak5PFJetTracksAssociatorAtVertex.jets = tauSource
ak5PFJetsLegacyHPSPiZeros.jetSrc = tauSource
combinatoricRecoTaus.jetSrc = tauSource
tauMatch.src = 'hpsPFTauProducer'
tauGenJetMatch.src = 'hpsPFTauProducer'

tauIsoDepositPFCandidates.ExtractorPSet.tauSource = 'hpsPFTauProducer'
tauIsoDepositPFCandidates.src = 'hpsPFTauProducer'
tauIsoDepositPFChargedHadrons.ExtractorPSet.tauSource = 'hpsPFTauProducer'
tauIsoDepositPFChargedHadrons.src = 'hpsPFTauProducer'
tauIsoDepositPFNeutralHadrons.ExtractorPSet.tauSource = 'hpsPFTauProducer'
tauIsoDepositPFNeutralHadrons.src = 'hpsPFTauProducer'
tauIsoDepositPFGammas.ExtractorPSet.tauSource = 'hpsPFTauProducer'
tauIsoDepositPFGammas.src = 'hpsPFTauProducer'

# 44X : HPS taus are not yet the default. 
patTaus.tauSource = 'hpsPFTauProducer'

tauIDs = dict(
    decayModeFinding = cms.InputTag('hpsPFTauDiscriminationByDecayModeFinding'),
    byVLooseCombinedIsolationDeltaBetaCorr = cms.InputTag('hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr'),
    byLooseCombinedIsolationDeltaBetaCorr = cms.InputTag('hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr'),
    byMediumCombinedIsolationDeltaBetaCorr = cms.InputTag('hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr'),
    byTightCombinedIsolationDeltaBetaCorr = cms.InputTag('hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr'),
    byRawIsoMVA = cms.InputTag('hpsPFTauDiscriminationByIsolationMVAraw'),
    byLooseIsoMVA = cms.InputTag('hpsPFTauDiscriminationByLooseIsolationMVA'),
    byMediumIsoMVA = cms.InputTag('hpsPFTauDiscriminationByMediumIsolationMVA'),
    byTightIsoMVA = cms.InputTag('hpsPFTauDiscriminationByTightIsolationMVA'),
    againstElectronLoose = cms.InputTag('hpsPFTauDiscriminationByLooseElectronRejection'),
    againstElectronMedium = cms.InputTag('hpsPFTauDiscriminationByMediumElectronRejection'),
    againstElectronTight = cms.InputTag('hpsPFTauDiscriminationByTightElectronRejection'),
    againstElectronMVA = cms.InputTag('hpsPFTauDiscriminationByMVAElectronRejection'),
    againstElectronDeadECAL = cms.InputTag('hpsPFTauDiscriminationByDeadECALElectronRejection'),
    againstElectronMVA2raw = cms.InputTag('hpsPFTauDiscriminationByMVA2rawElectronRejection'),
    againstElectronMVA2category = cms.InputTag('hpsPFTauDiscriminationByMVA2rawElectronRejection:category'),
    againstElectronVLooseMVA2 = cms.InputTag('hpsPFTauDiscriminationByMVA2VLooseElectronRejection'),
    againstElectronLooseMVA2 = cms.InputTag('hpsPFTauDiscriminationByMVA2LooseElectronRejection'),
    againstElectronMediumMVA2 = cms.InputTag('hpsPFTauDiscriminationByMVA2MediumElectronRejection'),
    againstElectronTightMVA2 = cms.InputTag('hpsPFTauDiscriminationByMVA2TightElectronRejection'),  
    againstMuonLoose = cms.InputTag('hpsPFTauDiscriminationByLooseMuonRejection'),
    againstMuonMedium = cms.InputTag('hpsPFTauDiscriminationByMediumMuonRejection'),
    againstMuonTight = cms.InputTag('hpsPFTauDiscriminationByTightMuonRejection'),
    )
tauIDsNew = dict(
    byIsolationMVA2raw = cms.InputTag('hpsPFTauDiscriminationByIsolationMVA2raw'),
    byLooseIsolationMVA2 = cms.InputTag('hpsPFTauDiscriminationByLooseIsolationMVA2'),
    byMediumIsolationMVA2 = cms.InputTag('hpsPFTauDiscriminationByMediumIsolationMVA2'),
    byTightIsolationMVA2 = cms.InputTag('hpsPFTauDiscriminationByTightIsolationMVA2'),
    byLooseCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag('hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits'),
    byMediumCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag('hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits'),
    byTightCombinedIsolationDeltaBetaCorr3Hits = cms.InputTag('hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits'),
    byCombinedIsolationDeltaBetaCorrRaw3Hits = cms.InputTag('hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr3Hits'),
    againstElectronMVA3raw = cms.InputTag('hpsPFTauDiscriminationByMVA3rawElectronRejection'),
    againstElectronMVA3category = cms.InputTag('hpsPFTauDiscriminationByMVA3rawElectronRejection:category'),	
    againstElectronLooseMVA3 = cms.InputTag('hpsPFTauDiscriminationByMVA3LooseElectronRejection'),
    againstElectronMediumMVA3 = cms.InputTag('hpsPFTauDiscriminationByMVA3MediumElectronRejection'),
    againstElectronTightMVA3 = cms.InputTag('hpsPFTauDiscriminationByMVA3TightElectronRejection'),
    againstElectronVTightMVA3 = cms.InputTag('hpsPFTauDiscriminationByMVA3VTightElectronRejection'),
    againstMuonLoose2 = cms.InputTag('hpsPFTauDiscriminationByLooseMuonRejection2'),
    againstMuonMedium2 = cms.InputTag('hpsPFTauDiscriminationByMediumMuonRejection2'),
    againstMuonTight2 = cms.InputTag('hpsPFTauDiscriminationByTightMuonRejection2'),
    )


# if isNewerThan('CMSSW_5_3_5'):
# deactivated until we have a 2011 tau id recipe
if 1: 
    tauIDs = dict( tauIDs.items() + tauIDsNew.items() )

patTaus.tauIDSources = cms.PSet(
    # configure many IDs as InputTag <someName> = <someTag>
    # you can comment out those you don't want to save some
    # disk space
    **tauIDs
    )


patTaus.embedLeadTrack                        = True
patTaus.embedSignalTracks                     = True
patTaus.embedIsolationTracks                  = True

patTaus.embedLeadPFCand                       = False
patTaus.embedLeadPFChargedHadrCand            = False
patTaus.embedLeadPFNeutralCand                = False
patTaus.embedSignalPFCands                    = False
patTaus.embedSignalPFChargedHadrCands         = False
patTaus.embedSignalPFNeutralHadrCands         = False
patTaus.embedSignalPFGammaCands               = False
patTaus.embedIsolationPFCands                 = False
patTaus.embedIsolationPFChargedHadrCands      = False
patTaus.embedIsolationPFNeutralHadrCands      = False
patTaus.embedIsolationPFGammaCands            = False

# need to deal with tau matching
patTaus.addGenJetMatch = True
patTaus.addGenMatch = True

# for simplicity, now selecting at the end. check if this ok in terms of speed
selectedPatTaus.cut = 'pt()>15 && tauID("decayModeFinding")'

PATTauSequence = cms.Sequence(
    # add jet selection
    pfJetsForHPSTau + 
    PFTau + 
    makePatTaus +
    selectedPatTaus 
    )

