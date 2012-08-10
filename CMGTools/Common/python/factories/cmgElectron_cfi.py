import FWCore.ParameterSet.Config as cms

import os

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
electronFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatElectronsAK5"),
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

#obsolete ids : not supported by EGM POG in 2012
#       vbtf60ID      = vbtfelectron60ID.clone(),
#       vbtf70ID      = vbtfelectron70ID.clone(),
#       vbtf80ID      = vbtfelectron80ID.clone(),
#       vbtf90ID      = vbtfelectron90ID.clone(),
#       vbtf95ID      = vbtfelectron95ID.clone(),
#       vbtf60CR      = vbtfelectron60CR.clone(),
#       vbtf70CR      = vbtfelectron70CR.clone(),
#       vbtf80CR      = vbtfelectron80CR.clone(),
#       vbtf90CR      = vbtfelectron90CR.clone(),
#       vbtf95CR      = vbtfelectron95CR.clone(),       
#       veryLooseID   = cicVeryLooseID.clone(),
#       looseID       = cicLooseID.clone(),
#       mediumID      = cicMediumID.clone(),
#       tightID       = cicTightID.clone(),
#       superTightID  = cicSuperTightID.clone(),
#       veryLooseCR   = cicVeryLooseCR.clone(),
#       looseCR       = cicLooseCR.clone(),
#       mediumCR      = cicMediumCR.clone(),
#       tightCR       = cicTightCR.clone(),
#       superTightCR  = cicSuperTightCR.clone(),
#       veryLooseIP   = cicVeryLooseIP.clone(),
#       looseIP       = cicLooseIP.clone(),
#       mediumIP      = cicMediumIP.clone(),
#       tightIP       = cicTightIP.clone(),
#       superTightIP  = cicSuperTightIP.clone(),
#       isoelectron = isolation.clone(),
       )
)
