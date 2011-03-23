import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
electronFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatElectronsPFlow"),
       leptonFactory = leptonFactory.clone()
       )

from CMGTools.Common.selections.vbtfelectron_cfi import *

cmgElectron = cms.EDFilter("ElectronPOProducer",
    cfg = electronFactory.clone(),
    cuts = cms.PSet(
       # the following needs the electron core to be in the input file
       # ecalDriven = cms.string('sourcePtr().ecalDriven()'),
       # but this one is ok: 
       isEB = cms.string('sourcePtr().isEB()'),
       # requires that the id variables have been filled
       #wp80 = vbtfelectron80.clone(),
       #wp95 = vbtfelectron95.clone()
       )
)
