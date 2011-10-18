from CMGTools.Production.dataset import *

def datasetToSource( user, dataset, pattern):
    
    import FWCore.ParameterSet.Config as cms
    
    data = Dataset( user, dataset, pattern )
    
    source = cms.Source(
	"PoolSource",
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
        )
    
    source.fileNames.extend( data.listOfFiles() )

    return source
