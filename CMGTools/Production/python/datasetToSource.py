from CMGTools.Production.dataset import Dataset
import FWCore.ParameterSet.Config as cms

def datasetToSource( user, dataset, pattern):
    
    data = Dataset( user, dataset, pattern )
    
    source = cms.Source(
	"PoolSource",
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
        )
    
    source.fileNames.extend( data.listOfGoodFiles() )

    return source
