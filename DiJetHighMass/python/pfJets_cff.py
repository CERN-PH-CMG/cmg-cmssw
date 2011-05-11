import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFJet_cfi import cmgPFJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
from CMGTools.Common.factories.cmgDiPFJet_cfi import cmgDiPFJet
from CMGTools.Common.factories.cmgFatJet_cfi import cmgFatJet
from CMGTools.Common.factories.cmgDiFatJet_cfi import cmgDiFatJet
from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms
from CMGTools.Common.skims.leadingCMGPFJetSelector_cfi import leadingCMGPFJetSelector
from CMGTools.DiJetHighMass.skims.cmgDiJetSelector_cfi import *
from CMGTools.DiJetHighMass.histograms.cmgDiJetHistograms_cfi import *
from CMGTools.DiJetHighMass.histograms.diJetLorentzVector_cfi import *

from CMGTools.Common.selections.kinematics_cfi import kinematics
from CMGTools.Common.selections.jetid_cfi import looseJetId

patJet = 'selectedPatJets'
pfJet = 'cmgPFJet'
pfLeadJet = 'cmgPFLeadJet'
pfDiJet = 'cmgPFDiJet'
pfFatJet = 'cmgPFFatJet'
pfFatDiJet = 'cmgPFFatDiJet'
pfTightDiJet = 'cmgPFTightDiJet'
pfFatTightDiJet = 'cmgPFFatTightDiJet'


# Produce PF Base Jet
allCuts = cms.PSet(
       jetKinematics = kinematics.clone(),
       looseJetId = looseJetId.clone(),
       )

cmgPFJet = cmgPFJet.clone()
cmgPFJet.cfg.inputCollection = patJet
cmgPFJet.cuts = allCuts
cmgPFJet.cuts.jetKinematics.eta = cms.string('abs(eta()) < 2.5')

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

cmgPFDiJet = cmgDiPFJet.clone()
cmgPFDiJet.cfg = diPFJetFactory


# Produce PFFatJets
cmgFatJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(pfLeadJet),
    leg2Collection = cms.InputTag(pfLeadJet),
    metCollection = cms.InputTag("cmgPFMET")
    )

cmgPFFatJet = cmgFatJet.clone()
cmgPFFatJet.cfg.inputCollection = cms.InputTag(pfJet)


cmgPFFatDiJet = cmgDiFatJet.clone()
cmgPFFatDiJet.cfg.leg1Collection = cms.InputTag(pfFatJet)
cmgPFFatDiJet.cfg.leg2Collection = cms.InputTag(pfFatJet)




# ---------- Plots producing and skimming

# General Jet histograms

cmgPFLorentzVector = pfJetLorentzVector.clone()
cmgPFLorentzVector.inputCollection = pfJet

cmgPFLeadLorentzVector = pfJetLorentzVector.clone()
cmgPFLeadLorentzVector.inputCollection = pfLeadJet

cmgPFHistograms = pfJetHistograms.clone()
cmgPFHistograms.inputCollection = pfJet

cmgPFLeadHistograms = pfJetHistograms.clone()
cmgPFLeadHistograms.inputCollection = pfLeadJet

# Skimmed dijet histograms for updates

cmgPFTightDiJet = cmgDiPFJetSel.clone()
cmgPFTightDiJet.src = pfDiJet

cmgPFFatTightDiJet = cmgDiFatJetSel.clone()
cmgPFFatTightDiJet.src = pfFatDiJet


cmgPFDiJetLorentzVector = cmgDiJetLorentzVector.clone()
cmgPFDiJetLorentzVector.inputCollection = pfTightDiJet

cmgPFDiJetHistograms = cmgDiPFJetHistograms.clone()
cmgPFDiJetHistograms.inputCollection = pfTightDiJet

cmgPFFatDiJetLorentzVector = cmgDiJetLorentzVector.clone()
cmgPFFatDiJetLorentzVector.inputCollection = pfFatTightDiJet

cmgPFFatDiJetHistograms = cmgDiFatJetHistograms.clone()
cmgPFFatDiJetHistograms.inputCollection = pfFatTightDiJet

cmgPFDiJetHistograms_FatJets = cmgDiFatJetHistograms_FatJets
cmgPFDiJetHistograms_FatJets.inputCollection = pfFatTightDiJet

pfJetSequence = cms.Sequence(
    cmgPFJet +
    cmgPFLeadJet +
    cmgPFDiJet+
    cmgPFFatJet+
    cmgPFFatDiJet+

    cmgPFLorentzVector +
    cmgPFLeadLorentzVector +
    cmgPFHistograms +
    cmgPFLeadHistograms +

    cmgPFTightDiJet+
    cmgPFDiJetLorentzVector+
    cmgPFDiJetHistograms+
    
    cmgPFFatTightDiJet+
    cmgPFFatDiJetLorentzVector+
    cmgPFFatDiJetHistograms+
    cmgPFDiJetHistograms_FatJets
    
    )
