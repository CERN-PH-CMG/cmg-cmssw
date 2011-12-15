import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data'

vertexWeight3D2011AB = cms.EDProducer(
    "VertexWeight3DProducer",
    verbose = cms.untracked.bool( False ),
    inputHistMC = cms.string( rootfile_dir + '/Pileup3D_Summer11MC.root'),
    inputHistData = cms.string( rootfile_dir + '/Pileup3D_160404-180252_4.6invfb.pileup.root' ),
    )
