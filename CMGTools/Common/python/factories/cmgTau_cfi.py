import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
tauFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatTausAK5"),
       leptonFactory = leptonFactory.clone(),
       rhoPFJetsCollection = cms.InputTag("kt6PFJetsAK5")
       )

cmgTau = cms.EDFilter("TauPOProducer",
                      cfg = tauFactory.clone(),
                      cuts = cms.PSet(pt_default = cms.string('pt() > 0.0 '),
                                      )    
                      
                      )
