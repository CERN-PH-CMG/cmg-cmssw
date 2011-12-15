import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data'
centraldir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp'

vertexWeight3DPromptRecov4 = cms.EDProducer(
    "VertexWeight3DProducer",
    verbose = cms.untracked.bool( False ),
    inputHistMC = cms.string( rootfile_dir + '/Pileup3D_Summer11MC.root'),
    inputHistData = cms.string( centraldir + '/Cert_165088-167913_7TeV_PromptReco_JSON.pileupTruth_v2_finebin.root'),
    )


