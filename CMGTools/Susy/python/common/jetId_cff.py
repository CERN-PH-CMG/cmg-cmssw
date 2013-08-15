import FWCore.ParameterSet.Config as cms

cmgPFJet30VeryLooseJetId95Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetVeryLooseJetId95Failed"),
    cut = cms.string( 'pt()>30' )
    )

cmgPFJet50VeryLooseJetId95Failed = cmgPFJet30VeryLooseJetId95Failed.clone( cut = cms.string('pt()>50'))


cmgPFJet30VeryLooseJetIdNH95Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( 'pt()>30 && component(5).fraction()>0.95' )
    )

cmgPFJet30VeryLooseJetIdNH90Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( 'pt()>30 && component(5).fraction()>0.90' )
    )

cmgPFJet30VeryLooseJetIdNH80Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( 'pt()>30 && component(5).fraction()>0.80' )
    )

cmgPFJet30VeryLooseJetIdG95Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( 'pt()>30 && component(4).fraction()>0.95' )
    )

jetIdSequence = cms.Sequence(
    cmgPFJet30VeryLooseJetId95Failed  +
    cmgPFJet50VeryLooseJetId95Failed  +
    cmgPFJet30VeryLooseJetIdNH95Failed +
    cmgPFJet30VeryLooseJetIdNH90Failed +
    cmgPFJet30VeryLooseJetIdNH80Failed +
    cmgPFJet30VeryLooseJetIdG95Failed
    )
