import FWCore.ParameterSet.Config as cms


##
cmgElectronSelPt = cms.EDFilter("CmgElectronSelector",src = cms.InputTag( "cmgElectronSel" ),
                           cut = cms.string( " pt() > 15.0" ))
cmgElectronSelEta = cms.EDFilter("CmgElectronSelector",src = cms.InputTag( "cmgElectronSelPt" ),
                            cut = cms.string( " -2.1 < eta() && eta() < 2.1 " ))


##last selector makes no cuts, just to create a final list with always the same name.
cmgElectronSelClean = cms.EDFilter("CmgElectronSelector",src = cms.InputTag( "cmgElectronSelEta" ),
                              cut = cms.string( "" ))




