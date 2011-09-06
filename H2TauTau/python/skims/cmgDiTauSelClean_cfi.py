import FWCore.ParameterSet.Config as cms


cmgDiTauSelTrue = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSel" ),
                           cut = cms.string( " leg1().genJetp4().Pt() > 0.0  && leg2().genJetp4().Pt() > 0.0 " ))

##Basic Selections on the DiTau
cmgDiTauSelMass = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSel" ),
                               cut = cms.string( " mass() > 1.0 " ))

cmgDiTauSelCharge = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelMass" ),
                                 cut = cms.string( " charge() == 0.0 " ))


##Selections on the tau legs
cmgDiTauSelPt = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelCharge" ),
                           cut = cms.string( " leg1().pt() > 20.0 && leg2().pt() > 20.0" ))

cmgDiTauSelEta = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelPt" ),
                            cut = cms.string( " abs(leg1().eta()) < 2.3 && abs(leg2().eta()) < 2.3" ))

#this cut is already applied in PF2PAT through the DiscriminationByLooseIsolation selection
#cmgDiTauSelDecayModeFinding = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelEta" ),
#                                         cut = cms.string( " leg1().tauID(\"decayModeFinding\")==1 && leg2().tauID(\"decayModeFinding\")==1 " ))

cmgDiTauSelAgainstMuon = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelEta" ),
                                         cut = cms.string( " leg1().tauID(\"againstMuonLoose\")==1 && leg2().tauID(\"againstMuonLoose\")==1 " ))


cmgDiTauSelAgainstElectron = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelAgainstMuon" ),
                                             cut = cms.string( " leg1().tauID(\"againstElectronLoose\")==1 && leg2().tauID(\"againstElectronLoose\")==1 " ))

#this cut is already applied in PF2PAT pfTausBase
#cmgDiTauSelVertex = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelAgainstElectronLoose" ),
#                               cut = cms.string( " abs(leg1().dz()) < 0.2 &&  abs(leg2().dz()) < 0.2 " ))

#a similar cut is defined in PF2PAT hpsPFTauProducer but was not applied
cmgDiTauSelSumPtIsolation = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelAgainstElectron" ),
                               cut = cms.string( " (leg1().trackIso() + max( leg1().gammaIso() - 0.025*3.14159*leg1().userData(\"rho\") , 0.0 )) <2.0 && (leg2().trackIso() + max( leg2().gammaIso() - 0.025*3.14159*leg2().userData(\"rho\") , 0.0 )) <2.0" ))


######
##last selector makes no cuts, just to create a final list with always the same name.
######
cmgDiTauSelClean = cms.EDFilter("CmgDiTauSelector",src = cms.InputTag( "cmgDiTauSelSumPtIsolation" ),
                                cut = cms.string( "" ))





