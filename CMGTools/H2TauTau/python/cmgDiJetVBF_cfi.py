import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

####Jets for VBF
cmgPFJetVBF = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( " pt() > 30.0" )
    )

###apply lepton cleaning to jets

#### VBF diJet producer
cmgDiJetVBF = cms.EDFilter("DiJetPOProducer",
                           cfg = diObjectFactory.clone(leg1Collection = cms.InputTag("cmgPFJetVBF"),
                                                       leg2Collection = cms.InputTag("cmgPFJetVBF")
                                                       ),
                           cuts = cms.PSet(),
                           verbose = cms.untracked.bool( False )
                           )

#                           cuts = cms.PSet( mass = cms.string(" mass() > 350.0 "),
#                                            deltaEta = cms.string(" abs(leg1.eta()-leg2.eta()) > 3.5"),
#                                            oppositeEta = cms.string(" leg1.eta()*leg2.eta() < 0.0"),
#                                            ),

cmgDiJetVBFSel = cms.EDFilter(
    "CmgDiBaseJetSelector",
    src = cms.InputTag( "cmgDiJetVBF" ),
    cut = cms.string( " mass() > 350.0 && abs(leg1.eta()-leg2.eta()) > 3.5 && leg1.eta()*leg2.eta() < 0.0" )
    )
