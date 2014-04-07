import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
tauFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatTaus"),
       leptonFactory = leptonFactory.clone()
       )

cmgTau = cms.EDFilter("TauPOProducer",
                      cfg = tauFactory.clone(),
                      cuts = cms.PSet(pt_default = cms.string('pt() > 0.0 '),
                                      )    
                      
                      )
