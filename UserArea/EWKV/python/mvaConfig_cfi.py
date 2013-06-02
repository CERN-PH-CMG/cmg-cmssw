import FWCore.ParameterSet.Config as cms

ewkzp2j = cms.PSet(  weightsDir = cms.string("/afs/cern.ch/user/p/psilva/work/ewkz/weights"),
                     methodList = cms.vstring('Fisher','LikelihoodD','BDTD'),
                     varsList   = cms.vstring("mjj","detajj","spt")
                     )
