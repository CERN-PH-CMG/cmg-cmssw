import FWCore.ParameterSet.Config as cms

import os

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
electronFactory = cms.PSet(
       inputCollection = cms.InputTag("patElectronsWithTrigger"),
       primaryVertexCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
       leptonFactory = leptonFactory.clone(),
       electronMVAFile = cms.FileInPath("CMGTools/Common/data/TMVA_BDTSimpleCat.weights.xml")
       )

from CMGTools.Common.selections.cutidelectron_cfi import *

cmgElectron = cms.EDFilter("ElectronPOProducer",
    cfg = electronFactory.clone(),
    cuts = cms.PSet(
       ecalDriven = cms.string('ecalDriven() == 1'),
       isEB = cms.string('sourcePtr().isEB()'),
       isEE = cms.string('sourcePtr().isEE()'),
       )
)
