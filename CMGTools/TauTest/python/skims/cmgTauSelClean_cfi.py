import FWCore.ParameterSet.Config as cms



cmgTauSelPt = cms.EDFilter("CmgTauSelector",
                         src = cms.InputTag( "cmgTauSel" ),
                         cut = cms.string( " pt() > 20.0 " )
                         )

cmgTauSelEta = cms.EDFilter("CmgTauSelector",
                         src = cms.InputTag( "cmgTauSelPt" ),
                         cut = cms.string( " -2.3 < eta() && eta() < 2.3 " )
                         )



##last selector makes no cuts, just to create a final list with always the same name.
cmgTauSelClean = cms.EDFilter("CmgTauSelector",
                         src = cms.InputTag( "cmgTauSelEta" ),
                         cut = cms.string( " " )
                         )




