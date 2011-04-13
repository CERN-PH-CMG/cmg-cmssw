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

RA1Electron = cmgElectronSel.clone(
    cut = "(pt() > 10.) && (abs(eta()) < 2.5)", 
    src = 'cmgElectronSel'
    )

RA1ElectronSequence = cms.Sequence(
    RA1Electron
    )

##########

RA1Muon = cmgMuonSel.clone(
    cut = "(pt() > 10.) && (abs(eta()) < 2.5) && " \
    "getSelection('cuts_isomuon') && getSelection('cuts_vbtfmuon')",
    src = 'cmgMuonSel'
    )

RA1MuonSequence = cms.Sequence(
    RA1Muon
    )

##########

RA1Photon = cmgPhotonSel.clone()

RA1PhotonSequence = cms.Sequence(
    RA1Photon
    )

##########

# NOTE: The loose jet id cuts have already been applied.

RA1PFJet30 = cmgPFJetSel.clone(
    src = "cmgPFJetSel",
    cut = "pt() > 30"
    )

RA1PFJet50 = cmgPFJetSel.clone(
    src = "cmgPFJetSel",
    cut = "pt() > 50"
    )

RA1PFJetSel = cmgPFJetSel.clone(
    src = "RA1PFJet50",
    cut = "(abs(eta) < 3.) && getSelection('cuts_looseJetId')"
    )

RA1PFJetFail = invertSelector(RA1PFJetSel)

# Select the jets from the above that are over 100 GeV.
RA1PFJetSel100 = cmgPFJetSel.clone(
    src = "RA1PFJetSel",
    cut = "pt() > 100"
    )

# Pick out the leading selected jet.
RA1PFJetSelLead = leadingCMGPFJetSelector.clone(
    inputCollection = "RA1PFJetSel",
    index = 1
    )

# Select the leading jet if within |eta| < 2.5.
RA1PFJetSelLeadEta25 = cmgPFJetSel.clone(
    src = "RA1PFJetSelLead",
    cut = "abs(eta) < 2.5"
    )

RA1JetSequence = cms.Sequence(
    RA1PFJet30 +
    RA1PFJet50 +
    RA1PFJetSel +
    RA1PFJetFail +
    RA1PFJetSel100 +
    RA1PFJetSelLead +
    RA1PFJetSelLeadEta25
    )

##########

#COLIN this product is already available from the common analysis sequence
# RA1MHTPFJet30 = cmgBaseCandMET.clone()
# RA1MHTPFJet30.cfg.inputCollection = "RA1PFJet30"
# RA1MHTPFJet30.cfg.ptThreshold = 30.

RA1MHTPFJet50 = cmgBaseCandMET.clone()
RA1MHTPFJet50.cfg.inputCollection = "RA1PFJet50"
RA1MHTPFJet50.cfg.ptThreshold = 50.

RA1MHTPFJet50Sel = cmgBaseMETSel.clone(
    src = "RA1MHTPFJet50",
    cut = "sumEt() > 250."
    )

RA1HTSequence = cms.Sequence(
    RA1MHTPFJet50 +
    RA1MHTPFJet50Sel
    )

##########

# RA1MHTSequence = cms.Sequence(
#     RA1MHTPFJet30
#    )

##########

# Object counting.

# Need at least one jet to build hemispheres.

# Need at least two jets with pT over 100 GeV.
RA1PFJetCount = cmgCandCount.clone(
    src = "RA1PFJetSel100",
    minNumber = 2
    )

# Need to have the leading jet withing eta range.
RA1PFJetLeadCount = cmgCandCount.clone(
    src = "RA1PFJetSelLeadEta25",
    minNumber = 1
    )

# This one is used as a veto.
RA1PFJetFailCount = cmgCandCount.clone(
    src = "RA1PFJetFail",
    minNumber = 1
    )

RA1ElectronCount = cmgCandCount.clone(
    src = "RA1Electron",
    minNumber = 1
    )

RA1MuonCount = cmgCandCount.clone(
    src = "RA1Muon",
    minNumber = 1
    )

RA1PhotonCount = cmgCandCount.clone(
    src = "RA1Photon",
    minNumber = 1
    )

RA1CountingSequence = cms.Sequence(
    RA1PFJetCount +
    RA1PFJetLeadCount +
    ~RA1PFJetFailCount +
    ~RA1ElectronCount +
    ~RA1MuonCount
    # +    ~RA1PhotonCount
    )

##########

RA1Hemi = cmgHemi.clone()
RA1Hemi.cfg.inputCollection = cms.VInputTag( cms.InputTag("RA1PFJetSel") )
                                             
RA1Hemi.cfg.maxCand = 100

RA1DiHemi = cmgDiHemi.clone()
RA1DiHemi.cfg.leg1Collection = "RA1Hemi"
RA1DiHemi.cfg.leg2Collection = "RA1Hemi"

RA1HemiSequence = cms.Sequence(
    RA1Hemi +
    RA1DiHemi
    )

##########

RA1MultiJet = cmgMultiJet.clone()
RA1MultiJet.cfg.inputCollection = cms.InputTag("RA1PFJet30")

RA1MultiJetSequence = cms.Sequence(
    RA1MultiJet
    )

######################################################################
## Histograms and corresponding sequences.
######################################################################

#RA1MHTPFJet30Histograms = baseMETHistograms.clone(
#    inputCollection = "RA1MHTPFJet30"
#    )

RA1MHTPFJet50Histograms = baseMETHistograms.clone(
    inputCollection = "RA1MHTPFJet50"
    )

RA1HistogramSequence = cms.Sequence(
#   RA1MHTPFJet30Histograms +
    RA1MHTPFJet50Histograms
    )

######################################################################
## Complete RA1 sequences.
######################################################################

RA1ObjectSequence = cms.Sequence(
    RA1ElectronSequence +
    RA1MuonSequence +
#    RA1PhotonSequence +
    RA1JetSequence +
    RA1HTSequence +
#    RA1MHTSequence +
    RA1HemiSequence +
    RA1MultiJetSequence
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
