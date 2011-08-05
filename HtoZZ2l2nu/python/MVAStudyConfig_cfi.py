import FWCore.ParameterSet.Config as cms

# simple study
redMETstudy = cms.PSet(  evCategories = cms.vint32(0,1,2,3),
                         input      = cms.string("EventSummaries.root"),
                         studyTag   = cms.string("redMetLT"),
                         weightsDir = cms.string("${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/weights"),
                         methodList = cms.vstring('Likelihood'), #'Cuts'),
                         varsList   = cms.vstring( 'redMetL','redMetT' ), #'redMet'),
                         procList   = cms.vstring( 'VBFtoH200toZZto2L2Nu', 'GGtoH200toZZto2L2Nu', 'GGtoH200toWWto2L2Nu', 'DYJetsToLL'),
                         procType   = cms.vint32 ( 1                     , 1                    ,  1                   , 2           ),
                         procWeight = cms.vdouble( 1                     , 1                    ,  1                   , 1           ),
                         )
