import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgBaseMET_cfi import cmgBaseMET
from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

def myPFJetSequence( process, postfix ): 

    patJet = 'selectedPatJets' + postfix
    pfJet = 'cmgPFJet' + postfix
    pfDiJet = 'cmgPFDiJet' + postfix
    pfLeadJet = 'cmgPFLeadJet' + postfix
    pfFatJet = 'cmgPFFatJet' + postfix
    pfFatDiJet = 'cmgPFFatDiJet' + postfix
    pfTightDiJet = 'cmgPFTightDiJet' + postfix
    pfFatTightDiJet = 'cmgPFFatTightDiJet' + postfix
   
    cloneProcessingSnippet(process, process.pfJetSequence, postfix)

    getattr(process, "cmgPFJet" + postfix).cfg.inputCollection = cms.InputTag(patJet)
    getattr(process, "cmgPFLeadJet" + postfix).inputCollection = cms.untracked.InputTag(pfJet)
    getattr(process, "cmgPFDiJet" + postfix).cfg.leg1Collection = cms.InputTag(pfLeadJet)
    getattr(process, "cmgPFDiJet" + postfix).cfg.leg2Collection = cms.InputTag(pfLeadJet)

    getattr(process, "cmgPFFatJet" + postfix).cfg.inputCollection = cms.InputTag(pfJet)

    getattr(process, "cmgPFFatDiJet" + postfix).cfg.leg1Collection = cms.InputTag(pfFatJet)
    getattr(process, "cmgPFFatDiJet" + postfix).cfg.leg2Collection = cms.InputTag(pfFatJet)


    # Histogramming

    getattr(process, "cmgPFLorentzVector" + postfix).inputCollection = cms.InputTag(pfJet)
    getattr(process, "cmgPFLeadLorentzVector" + postfix).inputCollection = cms.InputTag(pfLeadJet)
    getattr(process, "cmgPFHistograms" + postfix).inputCollection = cms.InputTag(pfJet)
    getattr(process, "cmgPFLeadHistograms" + postfix).inputCollection = cms.InputTag(pfLeadJet)

    getattr(process, "cmgPFTightDiJet" + postfix).src = cms.InputTag(pfDiJet)
    getattr(process, "cmgPFDiJetLorentzVector" + postfix).inputCollection = cms.InputTag(pfTightDiJet)
    getattr(process, "cmgPFDiJetHistograms" + postfix).inputCollection = cms.InputTag(pfTightDiJet)

    
    getattr(process, "cmgPFFatTightDiJet" + postfix).src = cms.InputTag(pfFatDiJet)
    getattr(process, "cmgPFFatDiJetLorentzVector" + postfix).inputCollection = cms.InputTag(pfFatTightDiJet)
    getattr(process, "cmgPFFatDiJetHistograms" + postfix).inputCollection = cms.InputTag(pfFatTightDiJet)
    getattr(process, "cmgPFDiJetHistograms_FatJets" + postfix).inputCollection = cms.InputTag(pfFatTightDiJet)
    


def myBaseJetSequence( process, postfix ): 

    patJet = 'selectedPatJets' + postfix
    baseJet = 'cmgBaseJet' + postfix
    baseLeadJet = 'cmgBaseLeadJet' + postfix
    baseFatJet = 'cmgBaseFatJet' + postfix
    baseDiJet = 'cmgBaseDiJet' + postfix
    baseTightDiJet = 'cmgBaseTightDiJet' + postfix


    cloneProcessingSnippet(process, process.baseJetSequence, postfix)

    getattr(process, "cmgBaseJet" + postfix).cfg.inputCollection = cms.InputTag(patJet)
    getattr(process, "cmgBaseLeadJet" + postfix).inputCollection = cms.untracked.InputTag(baseJet)
    getattr(process, "cmgBaseDiJet" + postfix).cfg.leg1Collection = cms.InputTag(baseLeadJet)
    getattr(process, "cmgBaseDiJet" + postfix).cfg.leg2Collection = cms.InputTag(baseLeadJet)

    # Histogramming
    
    getattr(process, "cmgBaseLorentzVector" + postfix).inputCollection = cms.InputTag(baseJet)
    getattr(process, "cmgBaseLeadLorentzVector" + postfix).inputCollection = cms.InputTag(baseLeadJet)
    
    getattr(process, "cmgBaseTightDiJet" + postfix).src = cms.InputTag(baseDiJet)        
    getattr(process, "cmgBaseDiJetLorentzVector" + postfix).inputCollection = cms.InputTag(baseTightDiJet)
    getattr(process, "cmgBaseDiJetHistograms" + postfix).inputCollection = cms.InputTag(baseTightDiJet)


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
