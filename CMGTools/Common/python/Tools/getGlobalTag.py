import os

from Configuration.PyReleaseValidation.autoCond import autoCond

def getGlobalTag(runOnMC):
    """Return the global tag depending on the release"""
    
    if runOnMC:
        #default to startup, but use mc otherwise
        #return autoCond.get('startup',autoCond['mc'])
        return 'START42_V13::All'
    else:
        #return autoCond['com10']
        return 'GR_R_42_V19::All'