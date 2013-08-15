import os
import re 

# dirReprocessing = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing'
# dirPrompt = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt'
defaultDict = '/'.join( [ os.environ['CMSSW_BASE'],
                          'src/CMGTools/H2TauTau/json/officialJSONS.txt' ] )

def lfnToDataset( lfn ):
    '''If lfn contains A/CMG/B, returns /B. Otherwise, returns lfn.'''
    pattern = re.compile( '.*/CMG(\S+)' )
    match = pattern.match( lfn )
    if match is not None:
        dataset = match.group(1)
        # print dataset
        return dataset
    return lfn

def jsonPick( dataset, samplesAndJSONs=None):

    dataset = lfnToDataset(dataset)
    # find dictionary, if not provided by user
    if samplesAndJSONs is None:
        samplesAndJSONs = defaultDict 
    # reading dictionary
    jsonDictFile = open( samplesAndJSONs )
    jsons = {}
    for line in jsonDictFile:
        try:
            sample, json = line.split()
        except ValueError:
            continue
        jsons[sample] = json
    # dir = dirReprocessing
    # if 'Prompt' in dataset:
    #     dir = dirPrompt
    # stripping out the last part of the dataset name
    # to keep only the base official dataset name
    dsfields = dataset.lstrip('/').split('/')[0:3]
    # print dsfields
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
    print 
    samples = ['/TauPlusX/Run2011A-May10ReReco-v1/AOD/foo/bar',
               '/TauPlusX/Run2011B-PromptReco-v1/AOD/blah',
               '/store/cmst3/user/cmgtools/CMG/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_0/tree_CMG_648.root',
               'should_fail_for_this_sample_name']

    for sample in samples:
        print 'Sample', sample
        print '\tJSON =', jsonPick( sample )


    
