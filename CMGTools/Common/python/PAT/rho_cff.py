import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

kt6PFJets = kt4PFJets.clone( rParam = cms.double(0.6),
                             doAreaFastjet = cms.bool(True),
                             doRhoFastjet = cms.bool(True) )
kt6PFJetsForIso = kt6PFJets.clone( rParam = cms.double(0.6),
                                   Rho_EtaMax = cms.double(2.5),
                                   Ghost_EtaMax = cms.double(2.5) )
#NOTE: now taking the PU CHS subtracted PFCandidate collection in input,
# instead of pfNoElectron
#NOTE: cloning kt6PFJetsForIso, as the rho and ghost parameters are the same
kt6PFJetsCHSForIso = kt6PFJetsForIso.clone(
    # Rho_EtaMax = cms.double(2.5),
    # Ghost_EtaMax = cms.double(2.5),
    src = cms.InputTag("pfNoPileUpIso") )

rhoSequence = cms.Sequence(
    # 44X needed (not present in AOD?)
    # kt6PFJets + #NOTE: needed for V4
    kt6PFJetsForIso +
    kt6PFJetsCHSForIso  
    )

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X

if cmsswIs44X():
    #FIXME if not done, the patMuonWithMVA cannot find the rho, while it is here!! (Mike?)
    rhoSequence += kt6PFJets
