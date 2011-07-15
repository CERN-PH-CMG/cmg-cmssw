import FWCore.ParameterSet.Config as cms


##Selections
cmgMuESelCharge = cms.EDFilter("CmgMuESelector",src = cms.InputTag( "cmgMuESel" ),
                                 cut = cms.string( " charge() == 0.0 " ))

cmgMuESelMass = cms.EDFilter("CmgMuESelector",src = cms.InputTag( "cmgMuESelCharge" ),
                               cut = cms.string( " mass() > 1 " ))



##last selector makes no cuts, just to create a final list with always the same name.
cmgMuESelClean = cms.EDFilter("CmgMuESelector",src = cms.InputTag( "cmgMuESelMass" ),
                              cut = cms.string( "" ))




