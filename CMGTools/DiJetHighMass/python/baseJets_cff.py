import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
from CMGTools.Common.factories.cmgDiJet_cfi import cmgDiJet
from CMGTools.Common.factories.cmgFatJet_cfi import cmgFatJet
from CMGTools.Common.factories.cmgDiFatJet_cfi import cmgDiFatJet
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector
from CMGTools.Common.skims.leadingCMGBaseJetSelector_cfi import leadingCMGBaseJetSelector

patJet = 'selectedPatJets'
baseJet = 'cmgBaseJet'
baseLeadJet = 'cmgBaseleadjet'
baseFatJet = 'cmgBaseFatJet'

# Produce Base Jet
cmgBaseJet.cfg.inputCollection = patJet

# Select Base Leading Jet
cmgBaseLeadJet = leadingCMGBaseJetSelector.clone()
cmgBaseLeadJet.inputCollection = baseJet
cmgBaseLeadJet.index = cms.untracked.int32(2)

# Produce Base Dijet
diBaseJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(baseLeadJet),
    leg2Collection = cms.InputTag(baseLeadJet),
    metCollection = cms.InputTag("cmgPFMET")
    )

cmgBaseDiJet = cmgDiJet.clone()
cmgBaseDiJet.cfg = diBaseJetFactory

cmgBaseFatJet = cmgFatJet.clone()
cmgBaseFatJet.cfg.inputCollection = cms.InputTag(baseJet)


cmgBaseDiFatJet = cmgDiFatJet.clone()
cmgBaseDiFatJet.cfg.leg1Collection = cms.InputTag(baseFatJet)
cmgBaseDiFatJet.cfg.leg2Collection = cms.InputTag(baseFatJet)

jetBaseLorentzVector = baseJetLorentzVector.clone()
jetBaseLorentzVector.inputCollection = baseJet

jetBaseLeadLorentzVector = baseJetLorentzVector.clone()
jetBaseLeadLorentzVector.inputCollection = baseLeadJet

baseJetSequence = cms.Sequence(
    cmgBaseJet +
    cmgBaseLeadJet +
    jetBaseLorentzVector +
    jetBaseLeadLorentzVector +
    cmgBaseDiJet +
    cmgBaseFatJet +
    cmgBaseDiFatJet
    )
