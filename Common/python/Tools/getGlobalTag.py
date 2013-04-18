
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X, isNewerThan

def getGlobalTag(runOnMC, runOld5XGT):
    """Return the global tag, based on
    - the runOnMC flag
    - the file name
    - the CMSSW release

    Please have a look at the python source for more information
    """

    # we run on a 52 sample if:
    # - we have START52 in the name (52 MC)
    # - we have PromptReco and (2012A or 2012B) in the name (52 data)
##     runOld5XGT = file.find('START52') != -1 or \
##                     ( (file.find('2012A') != -1 or file.find('2012B') !=-1 ) and file.find('PromptReco') != -1 )
    
    if cmsswIs44X():
        if runOnMC:
            GT = 'START44_V13::All'
        else:
            GT = 'GR_R_44_V15::All'
    else:
        if runOnMC:
            if runOld5XGT:
                GT = 'START53_V10::All' # for 53X MC in >= 533
                # GT = 'START53_V7E::All' # for 53X MC in < 533
            else:
                GT = 'START53_V15::All' # Moriond calibration
        else:
            if runOld5XGT:
                GT = 'GR_P_V41_AN1::All' # for 53X data in >= 533
                # GT = 'GR_P_V40_AN1::All' # for 53X data in < 533
            else:
                GT = 'GR_P_V39_AN3::All' # Moriond calibration
    return GT

def getGlobalTagByDataset(runOnMC, dataset):
    """
    Uses the dataset name to select the correct global tag

    Information from here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions?redirectedfrom=CMS.SWGuideFrontierConditions#Summary_of_Global_Tags_used_in_o
    Last updated: wreece 14/01/13 for 53 data, MC, parking
    """
    GT = None
    if runOnMC:
        if cmsswIs44X():
            GT = 'START44_V13::All'
        elif 'START53' in dataset:
            GT = 'START53_V7G::All' 
        elif 'START52' in dataset:
            GT = 'START52_V9F::All'
    else:
        if cmsswIs44X():
            GT = 'GR_R_44_V15::All'
        elif '13Jul2012' in dataset:
            GT = 'FT_53_V6C_AN4::All'
        elif '06Aug2012' in dataset:
            GT = 'FT_53_V6C_AN4::All'
        elif '24Aug2012' in dataset:
            GT = 'FT_53_V10A_AN4::All'
        elif 'Run2012C-PromptReco-v1' in dataset:#shouldn't be used...
            GT = 'GR_P_V40_AN3::All'
        elif 'Run2012C-PromptReco-v2' in dataset:
            GT = 'GR_P_V42_AN4::All'
        elif 'Run2012D' in dataset:
            GT = 'GR_P_V42_AN4::All'
        elif 'Run2012B-05Nov2012' in dataset:
            GT = 'FT_53_V6C_AN4::All'
        elif 'Run2012C-part1_05Nov2012' in dataset:
            GT = 'FT53_V10A_AN4::All'
        elif 'Run2012C-part2_05Nov2012' in dataset:
            GT = 'FT_P_V42C_AN4::All'
        elif 'Run2012D-part1_10Dec2012-v1' in dataset:
            GT = 'FT_P_V42_AN4::All'
        elif 'Run2012D-part2_17Jan2013-v1' in dataset:
            GT = 'FT_P_V42D_AN4::All'

    if GT is None:
        raise Exception("No global tag found for dataset '%s'. Check getGlobalTagByDataset" % dataset)
    return GT


if __name__ == '__main__':

    print '52, data', getGlobalTag(False, True)
    print '52, MC', getGlobalTag(True, True)
    print 'the following depends on the release you are running in'
    print '44X or 53X, data', getGlobalTag(False, False)
    print '44X or 53X, MC', getGlobalTag(True, False)
