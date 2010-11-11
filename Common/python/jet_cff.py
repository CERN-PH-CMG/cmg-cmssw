import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFJet_cfi import cmgPFJet
from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector

cmgPFBaseJet = cmgBaseJet.clone()
cmgPFBaseJet.cfg.inputCollection = 'selectedPatJetsPFlow'

cmgCaloBaseJet = cmgBaseJet.clone()
cmgCaloBaseJet.cfg.inputCollection = 'selectedPatJets'

caloJetLorentzVector = baseJetLorentzVector.clone()
caloJetLorentzVector.inputCollection = 'cmgCaloBaseJet'

jetHistograms = cms.Sequence(
    pfJetLorentzVector +
    pfJetHistograms +
    caloJetLorentzVector
    )

pfJetSequence = cms.Sequence(
    cmgPFJet +
    cmgPFBaseJet +
    pfJetLorentzVector +
    pfJetHistograms 
    )

caloJetSequence = cms.Sequence(
    cmgCaloBaseJet + 
    caloJetLorentzVector
    )

jetSequence = cms.Sequence(
    pfJetSequence +
    caloJetSequence 
    ) 
