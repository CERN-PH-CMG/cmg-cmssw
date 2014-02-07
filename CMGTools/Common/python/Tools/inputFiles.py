import FWCore.ParameterSet.Config as cms

def restrictInput( fileNames, numberOfFiles) :

    tmpFiles =  fileNames
    files = cms.untracked.vstring()
    for file in tmpFiles:
        files.append( file )
        if len(files)== numberOfFiles:
            break
    print 'restricting input to:'
    print files 
    return files
