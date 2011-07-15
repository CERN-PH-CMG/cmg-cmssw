import FWCore.ParameterSet.Config as cms


##Selections
cmgTauMuSelCharge = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSel" ),
                                 cut = cms.string( " charge() == 0.0 " ))

cmgTauMuSelMass = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelCharge" ),
                               cut = cms.string( " mass() > 1 " ))



##last selector makes no cuts, just to create a final list with always the same name.
cmgTauMuSelClean = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelMass" ),
                              cut = cms.string( "" ))




