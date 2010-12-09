import FWCore.ParameterSet.Config as cms


def hotSkim( source, label ):

    sourceAll = source.clone()

    source.fileNames = []
    
    for sourceItem in sourceAll.fileNames:
        if sourceItem.find( label ) != -1:
            source.fileNames.append( sourceItem )


