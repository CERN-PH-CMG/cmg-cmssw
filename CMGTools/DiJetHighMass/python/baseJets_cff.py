import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
from CMGTools.Common.factories.cmgDiJet_cfi import cmgDiJet
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector
from CMGTools.Common.Tools.indexBaseJetSelector_cfi import indexBaseJetSelector

patJet = 'selectedPatJets'
baseJet = 'cmgBaseJet'
baseLeadJet = 'cmgBaseleadjet'

# Produce Base Jet
cmgBaseJet.cfg.inputCollection = patJet

# Select Base Leading Jet
cmgBaseLeadJet = indexBaseJetSelector.clone()
cmgBaseLeadJet.inputCollection = baseJet
cmgBaseLeadJet.index = cms.untracked.int32(2)

# Produce Base Dijet
diBaseJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(baseLeadJet),
    leg2Collection = cms.InputTag(baseLeadJet)
    )

cmgBaseDiJet = cmgDiJet.clone()
cmgBaseDiJet.cfg = diBaseJetFactory

jetBaseLorentzVector = baseJetLorentzVector.clone()
jetBaseLorentzVector.inputCollection = baseJet

jetBaseLeadLorentzVector = baseJetLorentzVector.clone()
jetBaseLeadLorentzVector.inputCollection = baseLeadJet

baseJetSequence = cms.Sequence(
    cmgBaseJet +
    cmgBaseLeadJet +
    jetBaseLorentzVector +
    jetBaseLeadLorentzVector +
    cmgBaseDiJet
    )
