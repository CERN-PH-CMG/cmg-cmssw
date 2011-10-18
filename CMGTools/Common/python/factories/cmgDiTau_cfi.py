import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

ditauFactory = diObjectFactory.clone(
    leg1Collection = cms.InputTag("cmgTauSel"),
    leg2Collection = cms.InputTag("cmgTauSel"),
    metCollection = cms.InputTag('cmgPFMET')       
    )


cmgDiTau = cms.EDFilter("DiTauPOProducer",
                        cfg = ditauFactory.clone(),
                        cuts =  cms.PSet( pt = cms.string("pt() > 0"),
                                          mass = cms.string(' 0 < mass() && mass() < 500'),             
                                          )
                        )

