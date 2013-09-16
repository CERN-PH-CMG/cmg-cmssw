import os
from CMGTools.Production.dataset import Dataset, CMSDataset, LocalDataset, createDataset

import FWCore.ParameterSet.Config as cms

def datasetToSource( user, dataset, pattern='.*root', readCache=False):

    # print user, dataset, pattern
    data = createDataset(user, dataset, pattern, readCache)
    
    source = cms.Source(
	"PoolSource",
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
        )
    
    source.fileNames.extend( data.listOfGoodFiles() )

    return source
