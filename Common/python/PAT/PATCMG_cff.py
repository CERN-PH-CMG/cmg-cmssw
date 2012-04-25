import FWCore.ParameterSet.Config as cms


from CommonTools.ParticleFlow.PFBRECO_cff import *
from PhysicsTools.PatAlgos.patSequences_cff import *

# FIXME add CMG sequence for each object. 

# PU SUB AND PARTICLES FOR ISO ---------------

pileUpSubtractionSequence = cms.Sequence(
    pfNoPileUpSequence +
    pfParticleSelectionSequence 
    )


# RHO's            ----------------------------

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

# kt6PFJetsAOD = kt4PFJets.clone( rParam = cms.double(0.6),
#                                doAreaFastjet = cms.bool(True),
#                                doRhoFastjet = cms.bool(True) )
kt6PFJetsForIso = kt6PFJets.clone( Rho_EtaMax = cms.double(2.5),
                                   Ghost_EtaMax = cms.double(2.5) )
#NOTE: now taking the PU CHS subtracted PFCandidate collection in input,
# instead of pfNoElectron
#NOTE: cloning kt6PFJetsForIso, as the rho and ghost parameters are the same
kt6PFJetsCHSForIso = kt6PFJetsForIso.clone(
    # Rho_EtaMax = cms.double(2.5),
    # Ghost_EtaMax = cms.double(2.5),
    src = cms.InputTag("pfNoPileUpIso") )

rhoSequence = cms.Sequence(
    # kt6PFJets + #NOTE: needed for V4
    kt6PFJetsForIso +
    kt6PFJetsCHSForIso  
    )


# MUONS           ----------------------------

#FIXME add detector based iso, also for electrons

patMuons.embedCaloMETMuonCorrs = False
patMuons.embedTcMETMuonCorrs = False
patMuons.embedTrack = True

from CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff import *

sourceMuons = 'muons'

muPFIsoDepositCharged.src = sourceMuons
muPFIsoDepositChargedAll.src = sourceMuons
muPFIsoDepositNeutral.src = sourceMuons
muPFIsoDepositGamma.src = sourceMuons
muPFIsoDepositPU.src = sourceMuons

patMuons.isoDeposits = cms.PSet(
    pfChargedHadrons = cms.InputTag("muPFIsoDepositCharged" ),
    pfChargedAll = cms.InputTag("muPFIsoDepositChargedAll" ),
    pfPUChargedHadrons = cms.InputTag("muPFIsoDepositPU" ),
    pfNeutralHadrons = cms.InputTag("muPFIsoDepositNeutral" ),
    pfPhotons = cms.InputTag("muPFIsoDepositGamma" )
    )

patMuons.isolationValues = cms.PSet(
    pfChargedHadrons = cms.InputTag("muPFIsoValueCharged04"),
    pfChargedAll = cms.InputTag("muPFIsoValueChargedAll04"),
    pfPUChargedHadrons = cms.InputTag("muPFIsoValuePU04" ),
    pfNeutralHadrons = cms.InputTag("muPFIsoValueNeutral04" ),
    pfPhotons = cms.InputTag("muPFIsoValueGamma04" )
    )

muonSequence = cms.Sequence(
    pfMuonIsolationSequence + 
    makePatMuons +
    selectedPatMuons
    )


# ELECTRONS      ----------------------------


from CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff import *

sourceElectrons = 'gsfElectrons'

elPFIsoDepositCharged.src = sourceElectrons
elPFIsoDepositChargedAll.src = sourceElectrons
elPFIsoDepositNeutral.src = sourceElectrons
elPFIsoDepositGamma.src = sourceElectrons
elPFIsoDepositPU.src = sourceElectrons


electronSequence = cms.Sequence(
    pfElectronIsolationSequence + 
    makePatElectrons +
    selectedPatElectrons 
    )



# JETS NO PU CHS ----------------------------

jetSource = 'ak5PFJets'

# corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactors.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactors.levels = ['L1Offset', 'L2Relative', 'L3Absolute']
patJetCorrFactors.payload = 'AK5PF'

patJets.jetSource = jetSource
patJets.addJetCharge = False
patJets.embedCaloTowers = False
patJets.embedPFCandidates = False
patJets.addAssociatedTracks = False

# b tagging 
from RecoJets.JetAssociationProducers.ak5JTA_cff import *
ak5JetTracksAssociatorAtVertex.jets = jetSource
from RecoBTag.Configuration.RecoBTag_cff import * # btagging sequence
softMuonTagInfos.jets = jetSource
softElectronTagInfos.jets = jetSource

# parton and gen jet matching

from CommonTools.ParticleFlow.genForPF2PAT_cff import * 

from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
patJetPartonMatch.src = jetSource
patJetGenJetMatch.src = jetSource
patJetGenJetMatch.matched = 'ak5GenJetsNoNu'

from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
patJetPartonAssociation.jets = jetSource

# we want GenJets without neutrinos!
genJetSequence = cms.Sequence(
    genParticlesForJetsNoNu +
    ak5GenJetsNoNu 
    )

jetMCSequence = cms.Sequence(
    patJetPartonMatch +
    patJetGenJetMatch
    )

jetSequence = cms.Sequence(
    jetMCSequence +
    ak5JetTracksAssociatorAtVertex + 
    btagging + 
    patJetCorrFactors +
    patJetFlavourId +
    patJets +
    selectedPatJets 
    )


# TAUS NO PU CHS  ----------------------------

from RecoTauTag.Configuration.RecoPFTauTag_cff import *
PFTau.remove( recoTauClassicShrinkingConeSequence )
PFTau.remove( recoTauClassicShrinkingConeMVASequence )
PFTau.remove( recoTauHPSTancSequence )

# will insert a jet selector before tau ID to speed up processing
jetSelectionForTaus = 'pt()>15.0 && abs(eta())<3.0'
from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets
pfJetsForHPSTau = selectedPfJets.clone( src = 'ak5PFJets',
                                        cut = jetSelectionForTaus )
# from CMGTools.Common.Tools.visitorUtils import replaceSrc
# replaceSrc( PFTau, 'ak5PFJets', 'pfJetsForHPSTau')
tauSource =  'pfJetsForHPSTau'
recoTauAK5PFJets08Region.src = tauSource #NOTE this guy has a pfSrc
ak5PFJetTracksAssociatorAtVertex.jets = tauSource
ak5PFJetsLegacyHPSPiZeros.jetSrc = tauSource
combinatoricRecoTaus.jetSrc = tauSource

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

tauSequence = cms.Sequence(
    # add jet selection
    pfJetsForHPSTau + 
    PFTau + 
    makePatTaus +
    selectedPatTaus
    )



# JETS CHS ----------------------------

# modifying PU charged hadron subtraction according to:
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetEnCorPFnoPU
pfPileUp.checkClosestZVertex = False
pfPileUp.Vertices = 'goodOfflinePrimaryVertices'
from CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi import goodOfflinePrimaryVertices
pfNoPileUpSequence.insert(0, goodOfflinePrimaryVertices)

# taking all PFCandidates which are not PU charged hadron
from RecoJets.JetProducers.ak5PFJets_cfi import *
ak5PFJetsCHS = ak5PFJets.clone()
ak5PFJetsCHS.src = 'pfNoPileUp'

ak5PFJetsCHS.doAreaFastjet = True
ak5PFJetsCHS.doRhoFastjet = False

#NOTE: please refer to the cfg, where the cloning of the PAT jet sequence is done
# could be rewritten here, but painful...

# MET      ----------------------------

#FIXME: do we still have access to raw MET? 
patMETs.metSource = 'pfType1CorrectedMet'
# dammit! the PAT MET sequence contains jet clustering modules... 
producePFMETCorrections.remove( kt6PFJets )
producePFMETCorrections.remove( ak5PFJets )
patMETs.addMuonCorrections = False

# adding raw PFMET
patMETsRaw = patMETs.clone()
patMETsRaw.addMuonCorrections = False
patMETsRaw.metSource = 'pfMet'

metSequence = cms.Sequence(
    producePFMETCorrections + 
    patMETs +
    patMETsRaw
    )

####  FULL SEQUENCE  ####

# NOTE: object sequences are defined so that they can be easily removed from the path
# contrary to PAT layer-wise sequences

PATCMGSequence = cms.Sequence(
    pileUpSubtractionSequence +
    rhoSequence +
    electronSequence +
    muonSequence +
    genJetSequence +
    jetSequence +
    tauSequence +
    metSequence 
    )
