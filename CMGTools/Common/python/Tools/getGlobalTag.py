
from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X

def getGlobalTag(file, runOnMC):
    """Return the global tag, based on
    - the runOnMC flag
    - the file name
    - the CMSSW release

    Please have a look at the python source for more information
    """

    # we run on a 52 sample if:
    # - we have START52 in the name (52 MC)
    # - we have PromptReco and (2012A or 2012B) in the name (52 data)
    runOn52Sample = file.find('START52') != -1 or \
                    ( (file.find('2012A') != -1 or file.find('2012B') !=-1 ) and file.find('PromptReco') != -1 )
    
    if cmsswIs44X():
        if runOnMC:
            GT = 'START44_V13::All'
        else:
            GT = 'GR_R_44_V15::All'
    else:
        if runOnMC:
            if runOn52Sample:
                GT = 'START52_V10::All'
            else:
                GT = 'START53_V10::All' # for 53X MC in >= 533
                # GT = 'START53_V7E::All' # for 53X MC in < 533
        else:
            if runOn52Sample:
                GT = 'GR_R_52_V8::All'
            else:
                GT = 'GR_P_V41_AN1::All' # for 53X data in >= 533
                # GT = 'GR_P_V40_AN1::All' # for 53X data in < 533

    return GT


if __name__ == '__main__':
    
    files = [
        # prompt reco 53:
        '/store/data/Run2012C/DoubleMu/AOD/PromptReco-v2/000/202/477/EACE0FC9-F7FB-E111-A689-003048CFB40C.root',
        # re-reco 53:
        '/store/data/Run2012A/DoubleElectron/AOD/13Jul2012-v1/00000/FED86C82-EFD9-E111-89EA-848F69FD45B6.root',
        # START53 MC:
        '/store/cmst3/group/cmgtools/CMG/DY2JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PFAOD_1.root',
        # prompt reco 52:
        '/store/data/Run2012B/BTag/RECO/PromptReco-v1/000/197/038/A63A84D9-F7BD-E111-A425-001D09F2905B.root',
        # START52 MC:
        '/store/mc/Summer12/TTH_HToBB_M-135_8TeV-pythia6/AODSIM/PU_S7_START52_V9-v1/0000/F4CA9D4D-80CE-E111-8091-00215E93D738.root'
        ]

    for file in files:
        print file, getGlobalTag(file, True), getGlobalTag(file, False)
