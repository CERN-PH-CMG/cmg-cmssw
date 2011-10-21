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
    
    source.fileNames.extend( data.listOfGoodFiles() )

    return source


if __name__ == '__main__':
    import sys,pprint 
    source = datasetToSource( sys.argv[1], sys.argv[2], sys.argv[3])
    dump = source.dumpPython()
    dump = dump.replace("'/store","\n'/store")
    print dump
