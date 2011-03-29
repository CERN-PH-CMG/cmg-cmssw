import FWCore.ParameterSet.Config as cms

#See CMS-PAS-SUS-10-009
razorbeta = cms.PSet(
    useMR = cms.string('abs(leg1.p()-leg2.p()) <= abs(leg1.pz()-leg2.pz())'),
    useMRPrime = cms.string('abs(leg1.p()-leg2.p()) > abs(leg1.pz()-leg2.pz())'),
    betaR = cms.string('betaR() < 0.99')
    )