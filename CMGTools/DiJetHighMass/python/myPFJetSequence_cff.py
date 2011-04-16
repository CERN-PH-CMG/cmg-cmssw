import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
# --------------------------------------------------------------
# PF jets (PF jets with constituents) --------------------------
# --------------------------------------------------------------

def myPFJetSequence( process, postfix ): 

    patJet = 'selectedPatJets' + postfix
    pfJet = 'cmgPFJet' + postfix
    pfLeadJet = 'cmgPFLeadJet' + postfix
   
    cloneProcessingSnippet(process, process.pfJetSequence, postfix)

    getattr(process, "cmgPFJet" + postfix).cfg.inputCollection     = patJet
    getattr(process, "cmgPFLeadJet" + postfix).inputCollection = pfJet
    getattr(process, "cmgPFDiJet" + postfix).cfg.leg1Collection = cms.InputTag(pfLeadJet)
    getattr(process, "cmgPFDiJet" + postfix).cfg.leg2Collection = cms.InputTag(pfLeadJet)
    getattr(process, "jetPFLorentzVector" + postfix).inputCollection = pfJet
    getattr(process, "jetPFLeadLorentzVector" + postfix).inputCollection = pfLeadJet
    getattr(process, "jetPFHistograms" + postfix).inputCollection = pfJet
    getattr(process, "jetPFLeadHistograms" + postfix).inputCollection = pfLeadJet

