import FWCore.ParameterSet.Config as cms

# see for details https://twiki.cern.ch/twiki/bin/view/CMS/JetID

#Selection thanks to Maxime
looseJetId = cms.PSet(
    h0Fraction = cms.string('component(5).fraction() < 0.99'),                      
    gammaFraction = cms.string('component(4).fraction() < 0.99'),
    nConstituents = cms.string('nConstituents() > 1'),
    #these only valid for abs(eta) < 2.4
    hFraction = cms.string('component(1).fraction() > 0 || abs(eta()) > 2.4'),
    hChargedMultiplicity = cms.string('component(1).number() > 0 || abs(eta()) > 2.4'),
    eFraction = cms.string('component(2).fraction() < 0.99 || abs(eta()) > 2.4'),
    muFraction = cms.string('component(3).fraction() < 0.99 || abs(eta()) > 2.4'),
    )

mediumJetId = looseJetId.clone()
mediumJetId.gammaFraction  = cms.string('component(4).fraction() < 0.95')
mediumJetId.h0Fraction  = cms.string('component(5).fraction() < 0.95')

tightJetId = looseJetId.clone()
tightJetId.gammaFraction  = cms.string('component(4).fraction() < 0.90')
tightJetId.h0Fraction  = cms.string('component(5).fraction() < 0.90')

