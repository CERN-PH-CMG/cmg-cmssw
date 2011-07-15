import FWCore.ParameterSet.Config as cms


##Selections
cmgDiESelCharge = cms.EDFilter("CmgDiElectronSelector",src = cms.InputTag( "cmgDiElectronSel" ),
                                 cut = cms.string( " charge() == 0.0 " ))

cmgDiESelMass = cms.EDFilter("CmgDiElectronSelector",src = cms.InputTag( "cmgDiESelCharge" ),
                               cut = cms.string( " mass() > 1 " ))



##last selector makes no cuts, just to create a final list with always the same name.
cmgDiESelClean = cms.EDFilter("CmgDiElectronSelector",src = cms.InputTag( "cmgDiESelMass" ),
                              cut = cms.string( "" ))




