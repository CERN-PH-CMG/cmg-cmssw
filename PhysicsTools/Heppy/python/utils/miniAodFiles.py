from __future__ import print_function
from PhysicsTools.Heppy.utils.cmsswRelease import cmsswRelease, releaseNumber

def miniAodFiles():
    relnum = releaseNumber(cmsswRelease())
    files = []
    big, medium = relnum[:2] # e.g. 7, 3 for CMSSW_7_3_X
    if (big, medium)==(7,2):
        files = [
            '/store/relval/CMSSW_7_2_2_patch1/RelValZMM_13/MINIAODSIM/PU25ns_MCRUN2_72_V1-v1/00000/5C64D172-8D73-E411-9C77-0025905A60AA.root',
            '/store/relval/CMSSW_7_2_2_patch1/RelValZMM_13/MINIAODSIM/PU25ns_MCRUN2_72_V1-v1/00000/6CA2B9CC-8973-E411-8920-002618943904.root'
            ]
    elif (big,medium)==(7,1):
        files = [
            '/store/relval/CMSSW_7_1_10_patch2/RelValZMM_13/MINIAODSIM/PU25ns_MCRUN2_71_V1-v1/00000/3E4EC015-AF53-E411-8889-0025905A6076.root',
            '/store/relval/CMSSW_7_1_10_patch2/RelValZMM_13/MINIAODSIM/PU25ns_MCRUN2_71_V1-v1/00000/C483E714-AF53-E411-9B9A-0025905B855E.root'
            ]
    elif (big,medium)==(7,0):
        files = [
            '/store/relval/CMSSW_7_0_9_patch3/RelValZMM_13/MINIAODSIM/PU25ns_PLS170_V7AN2-v1/00000/E0D7EDE4-0660-E411-B655-02163E00EF88.root',
            '/store/relval/CMSSW_7_0_9_patch3/RelValZMM_13/MINIAODSIM/PU25ns_PLS170_V7AN2-v1/00000/F0F88B6E-905F-E411-8080-0025904B0FBE.root'
            ]
    elif (big,medium)==(7,3):
        files = [
            '/store/relval/CMSSW_7_3_0_pre1/RelValZMM_13/MINIAODSIM/PU25ns_PRE_LS172_V15-v1/00000/582D0582-355F-E411-9F30-02163E006D72.root'
            ]
    elif (big,medium)==(7,4):
        files = [
            '/store/relval/CMSSW_7_4_0_pre2/RelValZMM_13/MINIAODSIM/PU25ns_MCRUN2_73_V7-v1/00000/1ACF00C3-E09A-E411-BB34-0025905AA9F0.root',
            '/store/relval/CMSSW_7_4_0_pre2/RelValZMM_13/MINIAODSIM/PU25ns_MCRUN2_73_V7-v1/00000/4E1864C4-E09A-E411-98F8-0025905B861C.root'
            ]
    elif (big,medium)==(7,5):
        files = [
            '/store/relval/CMSSW_7_5_1/RelValZMM_13/MINIAODSIM/PU50ns_75X_mcRun2_startup_v3_gs7118-v1/00000/44F0B567-C33F-E511-93A2-0025905B8562.root',
            '/store/relval/CMSSW_7_5_1/RelValZMM_13/MINIAODSIM/PU50ns_75X_mcRun2_startup_v3_gs7118-v1/00000/AC4D6070-C33F-E511-97C2-0025905A6094.root'
            ]
    elif (big,medium)==(7,6):
        files = [
            '/store/relval/CMSSW_7_6_0/RelValZMM_13/MINIAODSIM/76X_mcRun2_asymptotic_v11-v1/00000/82DD73F0-B17F-E511-9397-00261894397A.root',
            '/store/relval/CMSSW_7_6_0/RelValZMM_13/MINIAODSIM/76X_mcRun2_asymptotic_v11-v1/00000/AE6938F6-B17F-E511-937B-0025905938A4.root'
            ]
    elif (big,medium)==(8,0):
        files=[
            '/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0693E0E7-97BE-E611-B32F-0CC47A78A3D8.root',
            '/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/0806AB92-99BE-E611-9ECD-0025905A6138.root',
            '/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/165F54A0-A3BE-E611-B3F7-0025905A606A.root',
            '/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/18E31463-B3BE-E611-B6A3-0CC47A4D7678.root',
            ]
    elif (big,medium)==(9,2):
        files=[
            '/store/relval/CMSSW_9_2_4/RelValTTbar_13/MINIAODSIM/PU25ns_92X_upgrade2017_realistic_v2-v1/00000/A6E29287-535C-E711-B579-0CC47A4D7668.root',
            '/store/relval/CMSSW_9_2_4/RelValTTbar_13/MINIAODSIM/PU25ns_92X_upgrade2017_realistic_v2-v1/00000/B6BE639D-535C-E711-8689-0CC47A4D7606.root',
            ]
    elif (big,medium)==(9,4):
        files=[
            '/store/relval/CMSSW_9_4_0/RelValTTbar_13/MINIAODSIM/PU25ns_94X_mc2017_realistic_v10-v1/10000/6A3683FD-A7CA-E711-AC02-0CC47A7C361E.root',
            '/store/relval/CMSSW_9_4_0/RelValTTbar_13/MINIAODSIM/PU25ns_94X_mc2017_realistic_v10-v1/10000/BAE308FD-A7CA-E711-8DC4-0CC47A4C8E16.root',
            ]
    elif (big,medium) in ( (10,2), (10,4) ):
        files=[
            '/store/relval/CMSSW_10_2_4/RelValTTbar_13/MINIAODSIM/PU25ns_102X_upgrade2018_realistic_v12-v1/20000/EB67E894-1B22-0C4F-940B-B1A113F84B92.root',
            ]
 
 
    else:
        raise ValueError('no mini AOD file defined for release '+cmsswRelease())
    eosfiles = [''.join(['root://cms-xrd-global.cern.ch/', lfn]) for lfn in files]
    return eosfiles


if __name__ == '__main__':
    print(miniAodFiles())
