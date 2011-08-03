import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
    nJetsBin   = cms.int32(0),
    evCategories = cms.vint32(1),
    input      = cms.string("EventSummaries.root"),
    output     = cms.string("MVA_0jets.root"),
    methodList = cms.vstring('Likelihood'),
    varsList   = cms.vstring( 'lep1pt','lep2pt','dphill','drll','mindrlz','maxdrlz','mll'),
    selCut     = cms.string ( '' ),
    procList   = cms.vstring( 'ZZto2l2Nu','GGtoH200toZZto2L2Nu'),
    procType   = cms.vint32 ( 2,           1                   ),
    procWeight = cms.vdouble( 1,           1                   )
    )
