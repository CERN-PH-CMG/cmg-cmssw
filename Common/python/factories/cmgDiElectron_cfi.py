import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

dimuonFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("cmgElectron"),
       leg2Collection = cms.InputTag("cmgElectron"),
)
from CMGTools.Common.selections.razorbeta_cfi import razorbeta
from CMGTools.Common.selections.zee_cfi import zee
cmgDiElectron = cms.EDFilter(
    "DiElectronPOProducer",
    cfg = dimuonFactory.clone(),
    cuts = cms.PSet(
       razorbeta = razorbeta.clone(),             
       zee = zee.clone()
      ),
    )


