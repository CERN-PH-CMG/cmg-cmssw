import os
import re 

from CMGTools.H2TauTau.officialJSONS import jsonMap
from CMGTools.H2TauTau.dcsJSONS import dcsJsonMap

def lfnToDataset( lfn ):
    '''If lfn contains A/CMG/B, returns /B. Otherwise, returns lfn.'''
    pattern = re.compile( '.*/CMG(\S+)' )
    match = pattern.match( lfn )
    if match is not None:
        dataset = match.group(1)
        # print dataset
        return dataset
    return lfn

def jsonPick( dataset, useDCS=False):

    dataset = lfnToDataset(dataset)

    # stripping out the last part of the dataset name
    # to keep only the base official dataset name
    dsfields = dataset.lstrip('/').split('/')[0:3]
    # print dsfields
    baseDataSet = '/'+'/'.join( dsfields )

    jmap = jsonMap
    if useDCS:
        jmap = dcsJsonMap
        
    # jsonFile = jmap[ baseDataSet ]
    jsonFiles = []
    jsonFile = None
    for key, value in  jmap.iteritems():
        pat = re.compile(key)
        if pat.match(baseDataSet):
            jsonFiles.append(value)
    if len(jsonFiles)==0:
        raise ValueError('No json file found')
    elif len(jsonFiles)>1:
        raise ValueError('Too many json files found')
    else:
        jsonFile = jsonFiles[0]
            
    jsonAbsPath = jsonFile
    if not os.path.isfile(jsonAbsPath):
        raise ValueError( ' '.join([jsonAbsPath,
                                    'does not exist.']) )
    return jsonAbsPath

if __name__ == '__main__':

    samples = ['/TauPlusX/Run2011A-May10ReReco-v1/AOD/foo/bar',
               '/TauPlusX/Run2011B-PromptReco-v1/AOD/blah',
               '/TauPlusX/Run2012B-PromptReco-v1/AOD/blah',
               '/store/cmst3/user/cmgtools/CMG/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_0/tree_CMG_648.root',
               'should_fail_for_this_sample_name']

    for sample in samples:
        print 'Sample', sample
        print '\tJSON (DCS)     =', jsonPick( sample, True)
        print '\tJSON (cert)=', jsonPick( sample, False)


    
