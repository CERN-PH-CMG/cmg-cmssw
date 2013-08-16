import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
from CMGTools.Common.factories.cmgDiJet_cfi import cmgDiJet
from CMGTools.Common.factories.cmgFatJet_cfi import cmgFatJet
from CMGTools.Common.factories.cmgDiFatJet_cfi import cmgDiFatJet
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector
from CMGTools.Common.skims.leadingCMGBaseJetSelector_cfi import leadingCMGBaseJetSelector
from CMGTools.DiJetHighMass.skims.cmgDiJetSelector_cfi import *
from CMGTools.DiJetHighMass.histograms.cmgDiJetHistograms_cfi import *
from CMGTools.DiJetHighMass.histograms.diJetLorentzVector_cfi import *

from CMGTools.Common.selections.kinematics_cfi import kinematics


patJet = 'selectedPatJets'
baseJet = 'cmgBaseJet'
baseLeadJet = 'cmgBaseLeadJet'
baseDiJet = 'cmgBaseDiJet'
baseTightDiJet = 'cmgTightBaseDiJet'



# Produce Base Jet
allCuts = cms.PSet(
       jetKinematics = kinematics.clone()
       )

cmgBaseJet.cfg.inputCollection = patJet
cmgBaseJet.cuts = allCuts
cmgBaseJet.cuts.jetKinematics.eta = cms.string('abs(eta()) < 2.5')
cmgBaseJet.cuts.jetKinematics.pt = cms.string('pt > 30')

# Select Base Leading Jet
cmgBaseLeadJet = leadingCMGBaseJetSelector.clone()
cmgBaseLeadJet.inputCollection = baseJet
cmgBaseLeadJet.index = cms.int32(2)

# Produce Base Dijet
diBaseJetFactory = cms.PSet(            
    leg1Collection = cms.InputTag(baseLeadJet),
    leg2Collection = cms.InputTag(baseLeadJet),
    metCollection = cms.InputTag("cmgPFMET")
    )

cmgBaseDiJet = cmgDiJet.clone()
cmgBaseDiJet.cfg = diBaseJetFactory


# ---------- Plots producing and skimming

# General Jet histograms

cmgBaseLorentzVector = baseJetLorentzVector.clone()
cmgBaseLorentzVector.inputCollection = baseJet

cmgBaseLeadLorentzVector = baseJetLorentzVector.clone()
cmgBaseLeadLorentzVector.inputCollection = baseLeadJet


# Skimmed dijet histograms for updates

cmgBaseTightDiJet = cmgDiBaseJetSel.clone()
cmgBaseTightDiJet.src = baseDiJet

cmgBaseDiJetLorentzVector = cmgDiJetLorentzVector.clone()
cmgBaseDiJetLorentzVector.inputCollection = baseTightDiJet

cmgBaseDiJetHistograms = cmgDiJetHistograms.clone()
cmgBaseDiJetHistograms.inputCollection = baseTightDiJet


baseJetSequence = cms.Sequence(
    cmgBaseJet +
    cmgBaseLeadJet +
    cmgBaseDiJet +

    cmgBaseLorentzVector +
    cmgBaseLeadLorentzVector +

    cmgBaseTightDiJet+
    cmgBaseDiJetLorentzVector+
    cmgBaseDiJetHistograms
    
    )
