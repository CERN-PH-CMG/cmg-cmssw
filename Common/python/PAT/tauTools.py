import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.helpers import applyPostfix


def embedPFCandidatesInTaus( process, postfix, enable ):
    patTaus = getattr(process,'patTaus'+postfix)
    patTaus.embedLeadTrack                        = cms.bool(enable)
    patTaus.embedSignalTracks                     = cms.bool(enable)
    patTaus.embedIsolationTracks                  = cms.bool(enable)
    patTaus.embedLeadPFCand                       = cms.bool(enable)
    patTaus.embedLeadPFChargedHadrCand            = cms.bool(enable)
    patTaus.embedLeadPFNeutralCand                = cms.bool(enable)
    patTaus.embedSignalPFCands                    = cms.bool(enable)
    patTaus.embedSignalPFChargedHadrCands         = cms.bool(enable)
    patTaus.embedSignalPFNeutralHadrCands         = cms.bool(enable)
    patTaus.embedSignalPFGammaCands               = cms.bool(enable)
    patTaus.embedIsolationPFCands                 = cms.bool(enable)
    patTaus.embedIsolationPFChargedHadrCands      = cms.bool(enable)
    patTaus.embedIsolationPFNeutralHadrCands      = cms.bool(enable)
    patTaus.embedIsolationPFGammaCands            = cms.bool(enable)



# insert a selected pfJet and apply it to the HPS Tau sequences
from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets 
def adaptSelectedPFJetForHPSTau(process,
                                jetSelection = "",
                                postfix = ""):
    process.pfJetsForHPSTau = selectedPfJets.clone()
    process.pfJetsForHPSTau.src = "pfJets"+postfix
    process.pfJetsForHPSTau.cut = cms.string(jetSelection)
    setattr(process, "pfJetsForHPSTau"+postfix, process.pfJetsForHPSTau)
    getattr(process,"pfTausPreSequence"+postfix).insert(0,getattr(process,"pfJetsForHPSTau"+postfix))
    applyPostfix(process,"pfTausBase",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    # need to fix the tau presequence because it depends on the new jet collection
    applyPostfix(process,"pfJetTracksAssociatorAtVertex",postfix).jets = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfTauPFJets08Region",postfix).src = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfJetsPiZeros",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfJetsLegacyTaNCPiZeros",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    applyPostfix(process,"pfJetsLegacyHPSPiZeros",postfix).jetSrc = "pfJetsForHPSTau"+postfix
    # fix because the TopProjection in pfNoTau does not work because the taus come from the selected jets
    applyPostfix(process,"pfNoTau",postfix).bottomCollection = "pfJetsForHPSTau"+postfix
    # must use the pfJets for the patJets otherwise we will have the selection on the patJets
    print 'Warning: switching patJet.jetSource from pfNoTau to pfJets, so no Tau cleaning can be applied'
    applyPostfix(process,"patJets",postfix).jetSource = "pfJets"+postfix
