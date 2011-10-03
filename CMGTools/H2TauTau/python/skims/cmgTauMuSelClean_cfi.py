import FWCore.ParameterSet.Config as cms


##Basic Selections on the TauMu
cmgTauMuSelMass = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSel" ),
                               cut = cms.string( " mass() > 1.0 " ))

cmgTauMuSelCharge = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelMass" ),
                                 cut = cms.string( " abs(charge()) == 0.0 " ))


##Selections on the legs
cmgTauMuSelPt = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelCharge" ),
                           cut = cms.string( " leg1().pt() > 20.0 && leg2().pt() > 15.0" ))

cmgTauMuSelEta = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelPt" ),
                            cut = cms.string( " abs(leg1().eta()) < 2.3 && abs(leg2().eta()) < 2.1" ))

cmgTauMuSelAgainstMuon = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelEta" ),
                                         cut = cms.string( " leg1().tauID(\"againstMuonLoose\")==1 " ))

cmgTauMuSelAgainstElectron = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelAgainstMuon" ),
                                             cut = cms.string( " leg1().tauID(\"againstElectronLoose\")==1 " ))


cmgTauMuSelIsolation = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelAgainstElectron" ),
                                    cut = cms.string( " leg1().tauID(\"byVLooseCombinedIsolationDeltaBetaCorr\")==1" ))


######
##last selector makes no cuts, just to create a final list with always the same name.
## need to add Clean tag because cmgTauMuSel already exists in CMGTools/Common
######
cmgTauMuSelClean = cms.EDFilter("CmgTauMuSelector",src = cms.InputTag( "cmgTauMuSelIsolation" ),
                                cut = cms.string( "" ))





