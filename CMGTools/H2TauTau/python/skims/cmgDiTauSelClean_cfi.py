import FWCore.ParameterSet.Config as cms


##Selections
cmgDiTauSelCharge = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSel" ),
                                 cut = cms.string( " charge() == 0.0 " ))

cmgDiTauSelMass = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelCharge" ),
                               cut = cms.string( " mass() > 1 " ))



##last selector makes no cuts, just to create a final list with always the same name.
cmgDiTauSelClean = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelMass" ),
                              cut = cms.string( "" ))




