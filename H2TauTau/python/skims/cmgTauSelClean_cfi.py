import FWCore.ParameterSet.Config as cms


##Selections for Tau-Mu channel

cmgTauSelTrue = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSel" ),
                           cut = cms.string( " genJetp4().Pt() > 0.0" ))

cmgTauSelPt = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSel" ),
                           cut = cms.string( " pt() > 20.0" ))
cmgTauSelEta = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelPt" ),
                            cut = cms.string( " -2.3 < eta() && eta() < 2.3 " ))

#this cut is already applied in PF2PAT through the DiscriminationByLooseIsolation selection
cmgTauSelDecayModeFinding = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelEta" ),
                                         cut = cms.string( " tauID(\"decayModeFinding\")==1 " ))

cmgTauSelAgainstMuonTight = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelDecayModeFinding" ),
                                         cut = cms.string( " tauID(\"againstMuonTight\")==1 " ))


cmgTauSelAgainstElectronLoose = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelAgainstMuonTight" ),
                                             cut = cms.string( " tauID(\"againstElectronLoose\")==1 " ))

#this cut is already applied in PF2PAT pfTausBase
cmgTauSelVertex = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelAgainstElectronLoose" ),
                               cut = cms.string( " abs(dz()) < 0.2 " ))

#a similar cut is defined in PF2PAT hpsPFTauProducer but was not applied
cmgTauSelSumPtIsolation = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelVertex" ),
                               cut = cms.string( " (trackIso() + max( gammaIso() - 0.025*3.14159*userData(\"rho\") , 0.0 )) <2" ))

##last selector makes no cuts, just to create a final list with always the same name.
cmgTauSelClean = cms.EDFilter("CmgTauSelector",src = cms.InputTag( "cmgTauSelSumPtIsolation" ),
                              cut = cms.string( "" ))


