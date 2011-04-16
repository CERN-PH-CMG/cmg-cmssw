import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

# --------------------------------------------------------------
# PF base jets (light PF jets, do not store constituents) ------
# --------------------------------------------------------------

def myBaseJetSequence( process, postfix ): 

    patJet = 'selectedPatJets' + postfix
    baseJet = 'cmgBaseJet' + postfix
    baseLeadJet = 'cmgBaseLeadJet' + postfix

    cloneProcessingSnippet(process, process.baseJetSequence, postfix)

    getattr(process, "cmgBaseJet" + postfix).cfg.inputCollection     = patJet
    getattr(process, "cmgBaseLeadJet" + postfix).inputCollection = baseJet
    getattr(process, "cmgBaseDiJet" + postfix).cfg.leg1Collection = cms.InputTag(baseLeadJet)
    getattr(process, "cmgBaseDiJet" + postfix).cfg.leg2Collection = cms.InputTag(baseLeadJet)
    getattr(process, "jetBaseLorentzVector" + postfix).inputCollection = baseJet
    getattr(process, "jetBaseLeadLorentzVector" + postfix).inputCollection = baseLeadJet
    
    getattr(process, "cmgBaseJet" + postfix).cuts.jetKinematics.eta = cms.string('abs(eta()) < 2.5')
    idx = postfix.find('Calo', 0)
    if idx > -1:
        getattr(process, "cmgBaseJet" + postfix).cuts.jetKinematics.pt = cms.string('pt > 30')    
