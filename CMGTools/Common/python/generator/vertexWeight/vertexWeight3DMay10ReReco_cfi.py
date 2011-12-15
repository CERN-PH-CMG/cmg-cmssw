import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data'
centraldir = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp'

vertexWeight3DMay10ReReco = cms.EDProducer(
    "VertexWeight3DProducer",
    verbose = cms.untracked.bool( False ),
    inputHistMC = cms.string( rootfile_dir + '/Pileup3D_Summer11MC.root'),
    inputHistData = cms.string( centraldir + '/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileupTruth_v2_finebin.root'),
    )

