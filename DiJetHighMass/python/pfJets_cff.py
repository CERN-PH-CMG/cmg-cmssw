import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFJet_cfi import cmgPFJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
from CMGTools.Common.factories.cmgDiJet_cfi import cmgDiJet
from CMGTools.Common.factories.cmgFatJet_cfi import cmgFatJet
from CMGTools.Common.factories.cmgDiFatJet_cfi import cmgDiFatJet
from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms
from CMGTools.Common.skims.leadingCMGPFJetSelector_cfi import leadingCMGPFJetSelector

patJet = 'selectedPatJets'
pfJet = 'cmgPFJet'
pfLeadJet = 'cmgPFLeadJet'
pfFatJet = 'cmgPFFatJet'

# Produce PF Base Jet
cmgPFJet = cmgPFJet.clone()
cmgPFJet.cfg.inputCollection = patJet

# Select PF PF Leading Jet
cmgPFLeadJet = leadingCMGPFJetSelector.clone()
cmgPFLeadJet.inputCollection = pfJet
cmgPFLeadJet.index = cms.untracked.int32(2)

# Produce PF PF Dijets
diPFJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(pfLeadJet),
    leg2Collection = cms.InputTag(pfLeadJet),
    metCollection = cms.InputTag("cmgPFMET")
    )

cmgPFDiJet = cmgDiJet.clone()
cmgPFDiJet.cfg = diPFJetFactory

# Produce PFFatJets
cmgFatJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(pfLeadJet),
    leg2Collection = cms.InputTag(pfLeadJet),
    metCollection = cms.InputTag("cmgPFMET")
    )

cmgPFFatJet = cmgFatJet.clone()
cmgPFFatJet.cfg.inputCollection = cms.InputTag(pfJet)


cmgPFDiFatJet = cmgDiFatJet.clone()
cmgPFDiFatJet.cfg.leg1Collection = cms.InputTag(pfFatJet)
cmgPFDiFatJet.cfg.leg2Collection = cms.InputTag(pfFatJet)


jetPFLorentzVector = pfJetLorentzVector.clone()
jetPFLorentzVector.inputCollection = pfJet

jetPFLeadLorentzVector = pfJetLorentzVector.clone()
jetPFLeadLorentzVector.inputCollection = pfLeadJet

jetPFHistograms = pfJetHistograms.clone()
jetPFHistograms.inputCollection = pfJet

jetPFLeadHistograms = pfJetHistograms.clone()
jetPFLeadHistograms.inputCollection = pfLeadJet

pfJetSequence = cms.Sequence(
    cmgPFJet +
    cmgPFLeadJet +
    jetPFLorentzVector +
    jetPFLeadLorentzVector +
    jetPFHistograms +
    jetPFLeadHistograms +
    cmgPFDiJet+
    cmgPFFatJet+
    cmgPFDiFatJet
    )
