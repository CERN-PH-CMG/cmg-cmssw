import os
import glob

import pickle

def makeTreeList(step):
    """Make a list of trees to look at with a toy"""

    import pickle
    fileName = "SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_080212-SMS-NoSkim.pkl"
    SMSPoints = pickle.load(file(fileName))
    keys = sorted(SMSPoints.keys())

    massStop = set()
    massLSP = set()
    for k in keys:
        massStop.add(k[0])
        massLSP.add(k[1])

    massPointsStop = sorted([m for m in massStop])
    massPointsLSP = sorted([m for m in massLSP])
    
    treeList = []
    for i in xrange(len(massPointsStop)):
        for j in xrange(len(massPointsLSP)):
            point = (massPointsStop[i],massPointsLSP[j])
            if SMSPoints.has_key(point):
                if ((point[0]-massPointsStop[0]) % step) == 0 and ((point[1]-massPointsLSP[0]) % step) == 0:
                    treeList.append('RMRTree_%i_%i' % point)
    return treeList


if __name__ == "__main__":

    xsec = 0.001
    index = 3001

    config = "config_winter2012/MultiJet_All_fR1fR2fR3fR4_2012.cfg"
    fitName = "root://eoscms//eos/cms/store/cmst3/user/wreece/Razor2011/MultiJetAnalysis/FitResults/razorMJ_Penalties_Staircase_3_All.root"
    SMSTree = "root://eoscms//eos/cms/store/cmst3/user/wreece/Razor2011/SMST2tt/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_080212-SMS-ByPoint.root"

    treeList = makeTreeList(50)
    for tree in treeList:
        cmd = "python runMDLimit_SMS.py --multijet -t 50 -i %i -n 100 -q 8nh --xsec %f --input %s --tree_name %s --config %s %s" % (index,xsec,fitName,tree,config,SMSTree)
        print cmd
