import FWCore.ParameterSet.Config as cms

tauCuts = cms.PSet(
    kinematics = cms.PSet(
      pt = cms.string('leg1().pt()>20'),
      eta = cms.string('abs(leg1().eta())<2.3')
    ),
    id = cms.PSet(
      decay = cms.string('leg1().tauID("decayModeFinding")'),
      eVeto = cms.string('leg1().tauID("againstElectronLoose")'),
      muVeto = cms.string('leg1().tauID("againstMuonTight")')
      ),
    iso = cms.string('leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr")'),
    )
