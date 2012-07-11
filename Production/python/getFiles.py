def getFiles(dataset, user, pattern, useCache=True):
    from CMGTools.Production.datasetToSource import datasetToSource
    # print 'getting files for', dataset,user,pattern
    ds = datasetToSource( user, dataset, pattern, useCache )
    files = ds.fileNames
    return ['root://eoscms//eos/cms%s' % f for f in files]
