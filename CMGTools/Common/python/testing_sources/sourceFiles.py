import os

def testing_sources():
    """Import the testing files in a version specific way"""
    
    version = os.environ['CMSSW_VERSION']
    imp = 'CMGTools.Common.testing_sources.%s' % version
    ts = __import__(imp,globals(),locals(),['*'])
    return ts.sourceFiles
