import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
    nJetsBin   = cms.int32(0),
    evCategories = cms.vint32(1,2),
    input      = cms.string("EventSummaries.root"),
    output     = cms.string("CutOptimization_0jets.root"),
    methodList = cms.vstring('CutsD'),
    varsList   = cms.vstring( 'redmetL','redmetT' ),
    selCut     = cms.string ( '' ),
    procList   = cms.vstring( 'VBFtoH200toZZto2L2Nu', 'GGtoH200toZZto2L2Nu', 'GGtoH200toWWto2L2Nu', 'DYJetsToLL'),
    procType   = cms.vint32 ( 1                     , 1                    ,  1                   , 2           ),
    procWeight = cms.vdouble( 1                     , 1                    ,  1                   , 1           )
    )
