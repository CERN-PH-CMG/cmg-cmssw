import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFJet_cfi import cmgPFJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
from CMGTools.Common.factories.cmgDiJet_cfi import cmgDiJet
from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms
from CMGTools.Common.Tools.indexPFJetSelector_cfi import indexPFJetSelector

patJet = 'selectedPatJets'
pfJet = 'cmgPFJet'
pfLeadJet = 'cmgPFLeadJet'

# Produce PF Base Jet
cmgPFJet = cmgPFJet.clone()
cmgPFJet.cfg.inputCollection = patJet

# Select PF PF Leading Jet
cmgPFLeadJet = indexPFJetSelector.clone()
cmgPFLeadJet.inputCollection = pfJet
cmgPFLeadJet.index = cms.untracked.int32(2)

# Produce PF PF Dijets
diPFJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(pfLeadJet),
    leg2Collection = cms.InputTag(pfLeadJet)
    )

cmgPFDiJet = cmgDiJet.clone()
cmgPFDiJet.cfg = diPFJetFactory

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
    cmgPFDiJet
    )
