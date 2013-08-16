import FWCore.ParameterSet.Config as cms

import os

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
electronFactory = cms.PSet(
       inputCollection = cms.InputTag("patElectronsWithTrigger"),
       primaryVertexCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
       leptonFactory = leptonFactory.clone(),
       electronMVAFile = cms.FileInPath("CMGTools/Common/data/TMVA_BDTSimpleCat.weights.xml")
       )

#from CMGTools.Common.selections.vbtfelectron_cfi import *
#from CMGTools.Common.selections.cicelectron_cfi import *
#from CMGTools.Common.selections.isolation_cfi import *

# see mva pre-selection id : https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification
# see cut-based id working point : https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification
# NOTE: for cut-based id, vtx and isolation cut are NOT included.

from CMGTools.Common.selections.cutidelectron_cfi import *

cmgElectron = cms.EDFilter("ElectronPOProducer",
    cfg = electronFactory.clone(),
    cuts = cms.PSet(
       ecalDriven = cms.string('ecalDriven() == 1'),
       isEB = cms.string('sourcePtr().isEB()'),
       isEE = cms.string('sourcePtr().isEE()'),
       # requires that the id variables have been filled
       vetoNoVtx          = vetoNoVtx.clone(),
       looseNoVtx         = looseNoVtx.clone(),
       mediumNoVtx        = mediumNoVtx.clone(),
       tightNoVtx         = tightNoVtx.clone(),
       premvaTrig             = premvaTrig.clone(),
       )
)
