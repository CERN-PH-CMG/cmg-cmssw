import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
    evCategories = cms.vint32(0,1,2,3),
    input      = cms.string("EventSummaries.root"),
    output     = cms.string("CutOptimization_redMetLT.root"),
    methodList = cms.vstring('Cuts'), #'CutsD'),
    varsList   = cms.vstring( 'redMetL','redMetT' ), '#redMet'),
    selCut     = cms.string ( '' ),
    procList   = cms.vstring( 'VBFtoH200toZZto2L2Nu', 'GGtoH200toZZto2L2Nu', 'GGtoH200toWWto2L2Nu', 'DYJetsToLL'),
    procType   = cms.vint32 ( 1                     , 1                    ,  1                   , 2           ),
    procWeight = cms.vdouble( 1                     , 1                    ,  1                   , 1           )
    )
