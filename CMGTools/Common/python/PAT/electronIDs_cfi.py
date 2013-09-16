import FWCore.ParameterSet.Config as cms

electronIDs = cms.PSet(
    #SimpleCutsBasedEleID
    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"), 
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"), 
    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"), 
    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"), 
    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"), 
    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"), 
    #Cuts in Categories (CiC) -non exhaustive (5 flavors here, up to 9 available)
    eidVeryLoose = cms.InputTag("eidVeryLoose"), 
    eidLoose = cms.InputTag("eidLoose"), 
    eidMedium = cms.InputTag("eidMedium"), 
    eidTight = cms.InputTag("eidTight"),
    eidSuperTight = cms.InputTag("eidSuperTight"), 
    #MVA
    mvaTrigV0 = cms.InputTag("mvaTrigV0"),
    mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0"),
  )

