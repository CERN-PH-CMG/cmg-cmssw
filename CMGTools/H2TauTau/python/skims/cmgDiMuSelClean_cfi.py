import FWCore.ParameterSet.Config as cms


##Selections
cmgDiMuSelCharge = cms.EDFilter("CmgDiMuonSelector",src = cms.InputTag( "cmgDiMuonSel" ),
                                 cut = cms.string( " charge() == 0.0 " ))

cmgDiMuSelMass = cms.EDFilter("CmgDiMuonSelector",src = cms.InputTag( "cmgDiMuSelCharge" ),
                               cut = cms.string( " mass() > 1 " ))



##last selector makes no cuts, just to create a final list with always the same name.
cmgDiMuSelClean = cms.EDFilter("CmgDiMuonSelector",src = cms.InputTag( "cmgDiMuSelMass" ),
                              cut = cms.string( "" ))




