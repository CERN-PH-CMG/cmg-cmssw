import os

# dirReprocessing = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing'
# dirPrompt = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt'
defaultDict = '/'.join( [ os.environ['CMSSW_BASE'],
                          'src/CMGTools/H2TauTau/json/officialJSONS.txt' ] )

def jsonPick( dataset, samplesAndJSONs=None):
    
    # find dictionary, if not provided by user
    if samplesAndJSONs is None:
        samplesAndJSONs = defaultDict 
    # reading dictionary
    jsonDictFile = open( samplesAndJSONs )
    jsons = {}
    for line in jsonDictFile:
        sample, json = line.split()
        jsons[sample] = json
    # dir = dirReprocessing
    # if 'Prompt' in dataset:
    #     dir = dirPrompt
    # stripping out the last part of the dataset name
    # to keep only the base official dataset name
    dsfields = dataset.lstrip('/').split('/')[0:3]
    baseDataSet = '/'+'/'.join( dsfields )
    jsonFile = jsons[ baseDataSet ]
    jsonAbsPath = jsonFile
    if not os.path.isfile(jsonAbsPath):
        raise ValueError( ' '.join([jsonAbsPath,
                                    'does not exist.']) )
    return jsonAbsPath

if __name__ == '__main__':
    print 'reading from:'
    os.system('cat ' + defaultDict)
    print 
    samples = ['/TauPlusX/Run2011A-May10ReReco-v1/AOD/foo/bar',
               '/TauPlusX/Run2011B-PromptReco-v1/AOD/blah',
               'should_fail_for_this_sample_name']

    for sample in samples:
        print sample
        print '\tJSON =', jsonPick( sample )


    
