import FWCore.ParameterSet.Config as cms


##Selections for Tau-E channel
cmgTauESelCharge = cms.EDFilter("CmgTauESelector",src = cms.InputTag( "cmgTauESel" ),
                                 cut = cms.string( " charge() == 0.0 " ))

cmgTauESelMass = cms.EDFilter("CmgTauESelector",src = cms.InputTag( "cmgTauESelCharge" ),
                               cut = cms.string( " mass() > 1 " ))



##last selector makes no cuts, just to create a final list with always the same name.
cmgTauESelClean = cms.EDFilter("CmgTauESelector",src = cms.InputTag( "cmgTauESelMass" ),
                              cut = cms.string( "" ))




