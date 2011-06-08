import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFJet_cfi import cmgPFJet
from CMGTools.Common.factories.cmgBaseJet_cfi import cmgBaseJet
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms
from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel
from CMGTools.Common.skims.cmgBaseJetSel_cfi import cmgBaseJetSel
from CMGTools.Common.skims.leadingCMGBaseJetSelector_cfi import leadingCMGBaseJetSelector
from CMGTools.Common.skims.leadingCMGPFJetSelector_cfi import leadingCMGPFJetSelector

# PF jets -------------------------------------------------------

# leading jets
cmgPFJetLead = leadingCMGPFJetSelector.clone()
cmgPFJetLead.inputCollection = 'cmgPFJet'

# PF base jets (light PF jets, do not store constituents) -------
cmgPFBaseJet = cmgBaseJet.clone()
cmgPFBaseJet.cfg.inputCollection = 'selectedPatJetsAK5'

cmgPFBaseJetSel = cmgBaseJetSel.clone( src = 'cmgPFBaseJet' )

cmgPFBaseJetLead = leadingCMGBaseJetSelector.clone()
cmgPFBaseJetLead.inputCollection = 'cmgPFBaseJet'

# Light calo jets -----------------------------------------------
cmgCaloBaseJet = cmgBaseJet.clone()
cmgCaloBaseJet.cfg.inputCollection = 'selectedPatJetsAK5'

cmgCaloBaseJetSel = cmgBaseJetSel.clone( src = 'cmgCaloBaseJet' )

cmgCaloBaseJetLead = leadingCMGBaseJetSelector.clone()
cmgCaloBaseJetLead.inputCollection = 'cmgCaloBaseJet'

cmgPFJetSel.cut = 'pt()>15'
cmgPFBaseJetSel.cut = 'pt()>15'
cmgCaloBaseJetSel.cut = 'pt()>15'

pfJetSequence = cms.Sequence(
    cmgPFJet +
    cmgPFJetSel +
    cmgPFJetLead +
    cmgPFBaseJet +
    cmgPFBaseJetSel +
    # pfJetLorentzVector +
    # pfJetHistograms +
    cmgPFBaseJetLead
    )

caloJetSequence = cms.Sequence(
    cmgCaloBaseJet +
    cmgCaloBaseJetSel +
    cmgCaloBaseJetLead
#    caloJetLorentzVector
    )

jetSequence = cms.Sequence(
    pfJetSequence
    # + caloJetSequence
    )
