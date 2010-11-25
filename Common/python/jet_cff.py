import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFJet_cfi import cmgPFJet
from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
# dijet is not generic, no need for everybody to define it. Please put that in your cfg, or in a separate cff
# from CMGTools.Common.factories.cmgDiJet_cfi import cmgDiJet
from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms
from CMGTools.Common.histograms.baseJetLorentzVector_cfi import baseJetLorentzVector
from CMGTools.Common.Tools.indexBaseJetSelector_cfi import indexBaseJetSelector
from CMGTools.Common.Tools.indexPFJetSelector_cfi import indexPFJetSelector

# PF jets -------------------------------------------------------
cmgPFJetLead = indexPFJetSelector.clone()

# leading jets 
cmgPFJetLead.inputCollection = 'cmgPFJet'

# PF base jets (light PF jets, do not store constituents) -------
cmgPFBaseJet = cmgBaseJet.clone()
cmgPFBaseJet.cfg.inputCollection = 'selectedPatJetsPFlow'

# leading jets 
cmgPFBaseJetLead = indexBaseJetSelector.clone()
cmgPFBaseJetLead.inputCollection = 'cmgPFBaseJet'

# Light calo jets -----------------------------------------------
cmgCaloBaseJet = cmgBaseJet.clone()
cmgCaloBaseJet.cfg.inputCollection = 'selectedPatJets'

cmgCaloBaseJetLead = indexBaseJetSelector.clone()
cmgCaloBaseJetLead.inputCollection = 'cmgCaloBaseJet'

caloJetLorentzVector = baseJetLorentzVector.clone()
caloJetLorentzVector.inputCollection = 'cmgCaloBaseJetLead'

jetHistograms = cms.Sequence(
    pfJetLorentzVector +
    pfJetHistograms +
    caloJetLorentzVector
    )

pfJetSequence = cms.Sequence(
    cmgPFJet +
    cmgPFJetLead +
    cmgPFBaseJet +
    cmgPFBaseJetLead + 
    pfJetLorentzVector +
    pfJetHistograms 
    )

caloJetSequence = cms.Sequence(
    cmgCaloBaseJet + 
    cmgCaloBaseJetLead + 
    caloJetLorentzVector
    )

# diJetSequence = cms.Sequence(
#    cmgDiJet                         
#    )

jetSequence = cms.Sequence(
    pfJetSequence +
    caloJetSequence 
#    diJetSequence
    ) 
