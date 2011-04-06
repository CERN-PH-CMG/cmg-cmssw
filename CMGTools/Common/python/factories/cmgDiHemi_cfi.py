import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory
diHemiFactory = diObjectFactory.clone(
    leg1Collection = cms.InputTag("cmgHemi"),
    leg2Collection = cms.InputTag("cmgHemi")
    )

from CMGTools.Common.selections.razorbeta_cfi import razorbeta
cmgDiHemi = cms.EDFilter("DiHemispherePOProducer",
    cfg = diHemiFactory.clone(),
    cuts = cms.PSet(
        razorbeta = razorbeta.clone()
    )
)
