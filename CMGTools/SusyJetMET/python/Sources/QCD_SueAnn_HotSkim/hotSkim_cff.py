import FWCore.ParameterSet.Config as cms

def hotSkim( source, label ):

    sourceAll = source.clone()

    # If the label equals the empty string, no selection is done.
    if len(label):

        source.fileNames = []

        print "Selecting all files that sound like '%s'" % label

        for sourceItem in sourceAll.fileNames:
            print "  %s" % sourceItem
            if sourceItem.find( label ) != -1:
                source.fileNames.append( sourceItem )

        print "  Selected %d of %d files" % \
              (len(sourceAll.fileNames), len(source.fileNames))
