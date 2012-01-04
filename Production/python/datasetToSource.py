from CMGTools.Production.dataset import Dataset, CMSDataset
import FWCore.ParameterSet.Config as cms

def datasetToSource( user, dataset, pattern='.*root'):

    if user == 'CMS':
        data = CMSDataset( dataset )
    else:
        data = Dataset( user, dataset, pattern )
    
    source = cms.Source(
	"PoolSource",
	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
        )
    
    source.fileNames.extend( data.listOfGoodFiles() )

    return source
