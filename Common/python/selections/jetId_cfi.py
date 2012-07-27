import FWCore.ParameterSet.Config as cms

# see for details https://twiki.cern.ch/twiki/bin/view/CMS/JetID

veryLooseJetId99 = cms.PSet(
    h0Fraction = cms.string('component(5).fraction() < 0.99'),                      
    gammaFraction = cms.string('component(4).fraction() < 0.99')  
    )

veryLooseJetId95 = cms.PSet(
    h0Fraction = cms.string('component(5).fraction() < 0.95'),                      
    gammaFraction = cms.string('component(4).fraction() < 0.95')  
    )

veryLooseJetId95h0 = cms.PSet(
    h0Fraction = cms.string('component(5).fraction() < 0.95')  
    )

veryLooseJetId95gamma = cms.PSet(
    gammaFraction = cms.string('component(4).fraction() < 0.95')  
    )


#Selection thanks to Maxime
looseJetId = cms.PSet(
    h0Fraction = cms.string('component(5).fraction() < 0.99'),                      
    gammaFraction = cms.string('component(4).fraction() < 0.99'),
    nConstituents = cms.string('nConstituents() > 1'),
    #these only valid for abs(eta) < 2.4
    hFraction = cms.string('abs(eta()) < 2.4 || component(1).fraction() > 0'),
    hChargedMultiplicity = cms.string('abs(eta()) < 2.4 || component(1).number() > 0'),
    eFraction = cms.string('abs(eta()) < 2.4 || component(2).fraction() < 0.99'),
    muFraction = cms.string('abs(eta()) < 2.4 || component(3).fraction() < 0.99'),
    )

mediumJetId = looseJetId.clone()
mediumJetId.gammaFraction  = cms.string('component(4).fraction() < 0.95')
mediumJetId.h0Fraction  = cms.string('component(5).fraction() < 0.95')

tightJetId = looseJetId.clone()
tightJetId.gammaFraction  = cms.string('component(4).fraction() < 0.90')
tightJetId.h0Fraction  = cms.string('component(5).fraction() < 0.90')

