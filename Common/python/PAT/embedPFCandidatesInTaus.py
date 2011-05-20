import FWCore.ParameterSet.Config as cms

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

