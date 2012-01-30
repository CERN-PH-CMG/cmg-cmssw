from CMGTools.Production.dataset import Dataset, CMSDataset
import FWCore.ParameterSet.Config as cms

def createDataset( user, dataset, pattern):
    if user == 'CMS':
        data = CMSDataset( dataset )
    else:
        data = Dataset( user, dataset, pattern )
    return data

def datasetToSource( user, dataset, pattern='.*root'):

    data = createDataset(user, dataset, pattern)
    
    source = cms.Source(
	"PoolSource",
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
        )
    
    source.fileNames.extend( data.listOfGoodFiles() )

    return source
