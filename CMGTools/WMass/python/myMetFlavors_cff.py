import FWCore.ParameterSet.Config as cms

from CMGTools.External.puJetIDAlgo_cff import *
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X, isNewerThan


jetPtMin = 1.
puJetIdAlgo = PhilV1
wpId = 2 # see MetUtilies.cc
if isNewerThan('CMSSW_5_3_0'):
    jetPtMin = 0.
    puJetIdAlgo = met_53x
    wpId = 3

myMetFlavor   = cms.EDProducer(
    "MyMetFlavorProducer",
    MetFlavor       = cms.int32(0),  # 0 PF  1 TK  2 No PU 3 PU  4 PUC
    CorrJetName = cms.InputTag("cmgPFJetSel"),
    PFCandidateName = cms.InputTag("cmgCandidates"),
    VertexName      = cms.InputTag("offlinePrimaryVertices"),
    RhoName         = cms.InputTag('kt6PFJets','rho'),
    JetPtMin        = cms.double(jetPtMin), # should be 0 for 5X and 1 for 4X
    dZMin           = cms.double(0.1),
    candEtaMin = cms.double(0.0),
    candEtaMax = cms.double(100.0),
    chargedCandPtMin = cms.double(0.0),
    neutralCandPMin = cms.double(0.0),
    candFlavor = cms.string(''),    
    WorkingPointId  = cms.uint32(wpId),
    PUJetId         = puJetIdAlgo,
    PUJetIdLowPt    = puJetIdAlgo,
    )
   
tkMet     =  myMetFlavor.clone(MetFlavor = cms.int32(1))
nopuMet   =  myMetFlavor.clone(MetFlavor = cms.int32(2))
puMet     =  myMetFlavor.clone(MetFlavor = cms.int32(3),dZMin = 0.2)
pcMet     =  myMetFlavor.clone(MetFlavor = cms.int32(4))

                          
MetRegressionSequence  = cms.Sequence (
    myMetFlavor + 
    nopuMet +
    puMet + 
    pcMet +
    tkMet
    )
