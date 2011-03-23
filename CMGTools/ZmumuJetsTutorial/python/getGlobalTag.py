import os

def getGlobalTag(runOnMC):
    """Return the global tag depending on the release"""
    
    data = {'CMSSW_3_8_7':'GR_R_38X_V15::All',
            'CMSSW_4_1_3':'GR_R_311_V2::All',
            }
    
    mc = {'CMSSW_3_8_7':'START38_V14::All',
          'CMSSW_4_1_3':'START311_V2::All'
          }
    
    if not os.environ.has_key('CMSSW_VERSION'):
        raise KeyError("Looking for CMSSW_VERSION, but not found. Did you do 'cmsenv'?")
    version = os.environ.get('CMSSW_VERSION')

    tag = None
    if runOnMC:
        if not mc.has_key(version):
            raise KeyError("Unknown CMSSW version '%s'. Global tag for MC not found." % version)
        tag = mc[version]
    else:
        if not data.has_key(version):
            raise KeyError("Unknown CMSSW version '%s'. Global tag for data not found." % version)
        tag = data[version]
    return tag