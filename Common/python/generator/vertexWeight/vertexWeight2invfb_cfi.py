import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data'
centraldir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp'

#weight covers May10ReReco + PromptReco-v4 + 05AugReReco + Prompt-v6

vertexWeight2invfb = cms.EDProducer(
    "VertexWeightProducer",
    verbose = cms.untracked.bool( False ),
    src = cms.InputTag('addPileupInfo'),
    inputHistMC = cms.string( rootfile_dir + '/Pileup_Summer11MC.root'),
    inputHistData = cms.string( rootfile_dir + '/Pileup_160404-173692_2.1invfb.pileup.root' ),
    )

