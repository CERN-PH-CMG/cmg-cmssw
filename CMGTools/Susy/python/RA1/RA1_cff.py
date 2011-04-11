import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgHemi_cfi import *
from CMGTools.Common.factories.cmgDiHemi_cfi import *
from CMGTools.Common.factories.cmgBaseCandMET_cfi import cmgBaseCandMET
from CMGTools.Common.factories.cmgMultiJet_cfi import cmgMultiJet
from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms
from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.skims.cmgPhotonSel_cfi import *
from CMGTools.Common.skims.cmgMuonSel_cfi import *
from CMGTools.Common.skims.cmgPFJetSel_cfi import *
from CMGTools.Common.skims.cmgBaseMETSel_cfi import *
from CMGTools.Common.skims.leadingCMGPFJetSelector_cfi import leadingCMGPFJetSelector

######################################################################
## Cuts and selections and their sequences.
######################################################################

RA1cmgElectron = cmgElectronSel.clone(
    cut = "(pt() > 10.) && (abs(eta()) < 2.5)"
    )

RA1Electron = cms.Sequence(
    RA1cmgElectron
    )

##########

RA1cmgMuon = cmgMuonSel.clone(
    cut = "(pt() > 10.) && (abs(eta()) < 2.5) && " \
    "getSelection('cuts_isomuon') && getSelection('cuts_vbtfmuon')"
    )

RA1Muon = cms.Sequence(
    RA1cmgMuon
    )

##########

RA1cmgPhoton = cmgPhotonSel.clone()

RA1Photon = cms.Sequence(
    RA1cmgPhoton
    )

##########

# NOTE: The loose jet id cuts have already been applied.

RA1cmgPFJetSel30 = cmgPFJetSel.clone(
    src = "cmgPFJetSel",
    cut = "pt() > 30"
    )

RA1cmgPFJetSel50 = cmgPFJetSel.clone(
    src = "cmgPFJetSel",
    cut = "pt() > 50"
    )

RA1cmgPFJetSel = cmgPFJetSel.clone(
    src = "RA1cmgPFJetSel50",
    cut = "(abs(eta) < 3.) && getSelection('cuts_looseJetId')"
    )

RA1cmgPFJetFail = invertSelector(RA1cmgPFJetSel)

# Select the jets from the above that are over 100 GeV.
RA1cmgPFJetSelPt100 = cmgPFJetSel.clone(
    src = "RA1cmgPFJetSel",
    cut = "pt() > 100"
    )

# Pick out the leading selected jet.
RA1cmgPFJetSelLead = leadingCMGPFJetSelector.clone(
    inputCollection = "RA1cmgPFJetSel",
    index = 1
    )

# Select the leading jet if within |eta| < 2.5.
RA1cmgPFJetSelLeadEta25 = cmgPFJetSel.clone(
    src = "RA1cmgPFJetSelLead",
    cut = "abs(eta) < 2.5"
    )

RA1Jet = cms.Sequence(
    RA1cmgPFJetSel30 +
    RA1cmgPFJetSel50 +
    RA1cmgPFJetSel +
    RA1cmgPFJetFail +
    RA1cmgPFJetSelPt100 +
    RA1cmgPFJetSelLead +
    RA1cmgPFJetSelLeadEta25
    )

##########

RA1cmgMHTPFJets30 = cmgBaseCandMET.clone()
RA1cmgMHTPFJets30.cfg.inputCollection = "RA1cmgPFJetSel30"
RA1cmgMHTPFJets30.cfg.ptThreshold = 30.

RA1cmgMHTPFJets50 = cmgBaseCandMET.clone()
RA1cmgMHTPFJets50.cfg.inputCollection = "RA1cmgPFJetSel50"
RA1cmgMHTPFJets50.cfg.ptThreshold = 50.

RA1cmgMHTPFJets50Sel = cmgBaseMETSel.clone(
    src = "RA1cmgMHTPFJets50",
    cut = "sumEt() > 350."
    )

RA1HT = cms.Sequence(
    RA1cmgMHTPFJets50 +
    RA1cmgMHTPFJets50Sel
    )

##########

RA1MHT = cms.Sequence(
    RA1cmgMHTPFJets30
    )

##########

# Object counting.

# Need at least one jet to build hemispheres.

# Need at least two jets with pT over 100 GeV.
RA1cmgPFJetCount = cmgCandCount.clone(
    src = "RA1cmgPFJetSelPt100",
    minNumber = 2
    )

# Need to have the leading jet withing eta range.
RA1cmgPFJetLeadCount = cmgCandCount.clone(
    src = "RA1cmgPFJetSelLeadEta25",
    minNumber = 1
    )

# This one is used as a veto.
RA1cmgPFJetFailCount = cmgCandCount.clone(
    src = "RA1cmgPFJetFail",
    minNumber = 1
    )

RA1cmgElectronCount = cmgCandCount.clone(
    src = "RA1cmgElectron",
    minNumber = 1
    )

RA1cmgMuonCount = cmgCandCount.clone(
    src = "RA1cmgMuon",
    minNumber = 1
    )

RA1cmgPhotonCount = cmgCandCount.clone(
    src = "RA1cmgPhoton",
    minNumber = 1
    )

RA1CountingSequence = cms.Sequence(
    RA1cmgPFJetCount +
    RA1cmgPFJetLeadCount +
    ~RA1cmgPFJetFailCount +
    RA1cmgElectronCount +
    RA1cmgMuonCount +
    ~RA1cmgPhotonCount
    )

##########

RA1cmgHemi = cmgHemi.clone()
RA1cmgHemi.cfg.inputCollection = cms.VInputTag("RA1cmgPFJetSel")
RA1cmgHemi.cfg.maxCand = 100

RA1cmgDiHemi = cmgDiHemi.clone()
RA1cmgDiHemi.cfg.leg1Collection = "RA1cmgHemi"
RA1cmgDiHemi.cfg.leg2Collection = "RA1cmgHemi"

RA1Hemi = cms.Sequence(
    RA1cmgHemi +
    RA1cmgDiHemi
    )

##########

RA1cmgMultiJet = cmgMultiJet.clone()
RA1cmgMultiJet.cfg.inputCollection = "RA1cmgPFJetSel30"

RA1MultiJet = cms.Sequence(
    RA1cmgMultiJet
    )

######################################################################
## Histograms and corresponding sequences.
######################################################################

RA1mhtPTJets30Histograms = baseMETHistograms.clone(
    inputCollection = "RA1cmgMHTPFJets30"
    )

RA1mhtPTJets50Histograms = baseMETHistograms.clone(
    inputCollection = "RA1cmgMHTPFJets50"
    )

RA1HistogramSequence = cms.Sequence(
    RA1mhtPTJets30Histograms +
    RA1mhtPTJets50Histograms
    )

######################################################################
## Complete RA1 sequences.
######################################################################

RA1ObjectSequence = cms.Sequence(
    RA1Electron +
    RA1Muon +
#     RA1Photon +
    RA1Jet +
    RA1HT +
    RA1MHT +
    RA1Hemi +
    RA1MultiJet
    )

RA1Sequence  = cms.Sequence(
    RA1ObjectSequence +
    RA1HistogramSequence
    )

RA1SkimSequence = cms.Sequence(
    RA1ObjectSequence +
    RA1CountingSequence
    )

######################################################################
