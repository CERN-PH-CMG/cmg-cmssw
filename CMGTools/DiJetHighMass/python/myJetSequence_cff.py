import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

def myPFJetSequence( process, postfix ): 

    patJet = 'selectedPatJets' + postfix
    pfJet = 'cmgPFJet' + postfix
    pfLeadJet = 'cmgPFLeadJet' + postfix
    pfFatJet = 'cmgPFFatJet' + postfix
   
    cloneProcessingSnippet(process, process.pfJetSequence, postfix)

    getattr(process, "cmgPFJet" + postfix).cfg.inputCollection     = patJet
    getattr(process, "cmgPFLeadJet" + postfix).inputCollection = pfJet
    getattr(process, "cmgPFDiJet" + postfix).cfg.leg1Collection = cms.InputTag(pfLeadJet)
    getattr(process, "cmgPFDiJet" + postfix).cfg.leg2Collection = cms.InputTag(pfLeadJet)

    getattr(process, "cmgPFFatJet" + postfix).cfg.inputCollection = cms.InputTag(pfJet)
    getattr(process, "cmgPFDiFatJet" + postfix).cfg.leg1Collection = cms.InputTag(pfFatJet)
    getattr(process, "cmgPFDiFatJet" + postfix).cfg.leg2Collection = cms.InputTag(pfFatJet)
    
    getattr(process, "jetPFLorentzVector" + postfix).inputCollection = pfJet
    getattr(process, "jetPFLeadLorentzVector" + postfix).inputCollection = pfLeadJet
    getattr(process, "jetPFHistograms" + postfix).inputCollection = pfJet
    getattr(process, "jetPFLeadHistograms" + postfix).inputCollection = pfLeadJet

def myBaseJetSequence( process, postfix ): 

    patJet = 'selectedPatJets' + postfix
    baseJet = 'cmgBaseJet' + postfix
    baseLeadJet = 'cmgBaseLeadJet' + postfix
    baseFatJet = 'cmgBaseFatJet' + postfix

    cloneProcessingSnippet(process, process.baseJetSequence, postfix)

    getattr(process, "cmgBaseJet" + postfix).cfg.inputCollection = patJet
    getattr(process, "cmgBaseLeadJet" + postfix).inputCollection = baseJet
    getattr(process, "cmgBaseDiJet" + postfix).cfg.leg1Collection = cms.InputTag(baseLeadJet)
    getattr(process, "cmgBaseDiJet" + postfix).cfg.leg2Collection = cms.InputTag(baseLeadJet)

    getattr(process, "cmgBaseFatJet" + postfix).cfg.inputCollection = cms.InputTag(baseJet)
    getattr(process, "cmgBaseDiFatJet" + postfix).cfg.leg1Collection = cms.InputTag(baseFatJet)
    getattr(process, "cmgBaseDiFatJet" + postfix).cfg.leg2Collection = cms.InputTag(baseFatJet)
    
    getattr(process, "jetBaseLorentzVector" + postfix).inputCollection = baseJet
    getattr(process, "jetBaseLeadLorentzVector" + postfix).inputCollection = baseLeadJet
    
    getattr(process, "cmgBaseJet" + postfix).cuts.jetKinematics.eta = cms.string('abs(eta()) < 2.5')
    idx = postfix.find('Calo', 0)
    if idx > -1:
        getattr(process, "cmgBaseJet" + postfix).cuts.jetKinematics.pt = cms.string('pt > 30')    

def myJetSequence( process ): 

    postfixAK5PF = "AK5PF"
    postfixAK7PF = "AK7PF"
    postfixAK7Calo = "AK7Calo"

    myBaseJetSequence( process, postfixAK7Calo )
    myPFJetSequence( process, postfixAK5PF )
    myPFJetSequence( process, postfixAK7PF )

    process.cmgPFMET = cmgBaseMET.clone()
    process.cmgPFMET.cfg.inputCollection = "patMETs"

    process.jetSequence = cms.Sequence(
        process.cmgPFMET +
        getattr(process, "baseJetSequence" + postfixAK7Calo) +
        getattr(process, "pfJetSequence" + postfixAK5PF) +
        getattr(process, "pfJetSequence" + postfixAK7PF)
        )
