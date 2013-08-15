import ROOT as rt
import os, pickle, sys

from BTagSFUtil import *

def getBinContent(histo, x):

    xaxis = histo.GetXaxis()
    xbin = xaxis.FindBin(x)
    
    bin = histo.GetBin(xbin)
    return histo.GetBinContent(bin)

def mergeNorms(files):
    
    bins = {}

    for f in files:
        name = f.replace('.root','.pkl')
        if not os.path.exists(name): continue
        norm = pickle.load(file(name))
        for key in norm.keys():
            if bins.has_key(key):
                bins[key] = bins[key] + norm[key]
            else:
                bins[key] = norm[key]
    return bins

def loadNorms():

    pkl = "/afs/cern.ch/user/w/wreece/work/LimitSetting/RazorMultiJet2011/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_030412-ByPoint.pkl"
    return pickle.load(file(pkl))

def getBox(nB, nEle, nMu, nTau, diLep = False):
    #order is mu, ele, tau, had
    #boxMap = {'MuEle':0,'MuMu':1,'EleEle':2,'Mu':3,'Ele':4,'Had':5, 'BJet':6, 'Tau':7,'TauTau':8}
    BOX_NUM = 5
    if nEle > 0 and nMu > 0 and diLep:
        BOX_NUM = 0
    elif nMu > 1 and diLep:
        BOX_NUM = 1
    elif nEle > 1 and diLep:
        BOX_NUM = 2
    elif nTau > 1 and diLep:
        BOX_NUM = 8
    elif nMu > 0:
        BOX_NUM = 3
    elif nEle > 0:
        BOX_NUM = 4
    elif nTau > 0:
        BOX_NUM = 7
    elif nB > 0:
        BOX_NUM = 6
    return BOX_NUM

def getBoxGenLevel(genInfo):
    #const int result = (nTop*10000) + (nBot*1000) + (nEle*100) + (nMuon*10) + nTau;
    gen = str(genInfo)
    nB = int(gen[-4])
    nEle = int(gen[-3])
    nMu = int(gen[-2])
    nTau = int(gen[-1])
    return getBox(nB,nEle,nMu,nTau, diLep = True)

def quad(a, b):
    return rt.TMath.Sqrt(a**2 + b**2)

def decoratePoint(point):
    return '%i_%i' % point

if __name__ == '__main__':

    model = 'T1tttt'

    rt.gROOT.ProcessLine("""
struct Extras{\
    Int_t PROC;\
    Double_t LEP_W;\
    Double_t W;\
    Double_t W_EFF;\
    Double_t LEP_W_SYS;\
    Double_t LEP_W_OLD;\
    Int_t BOX_NUM;\
    Int_t BOX_NUM_GEN;\
    Double_t ISR_W;\
};""")
    from ROOT import Extras, std
    extras = Extras()
    extras.PROC = 0
    extras.LEP_W = 1.0
    extras.W = 1.0
    extras.W_EFF_SYS = 0.0

    import glob
    fileList = glob.glob('SMS-T1tttt_Mgluino-450to1200_mLSP-50to800_7TeV-Pythia6Z-Summer11-PU_START42_V11_FSIM-v2-wreece_290612-BPOG_*.root')
    #fileList = glob.glob('SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_290612-BPOG_*.root')
    norms = mergeNorms(fileList)
    #norms = loadNorms()
    pickle.dump(norms,file('foo.pkl','wb'))
    print norms

    oldTree = rt.TChain('RMRTree')
    for f in fileList:
        oldTree.Add(f)

    weightsF = rt.TFile.Open('/afs/cern.ch/user/w/wreece/work/LimitSetting/RazorMultiJet2011/ISRWeights_Topology%s.root' % model[0:2])
    weights = {}
    for point in norms.iterkeys():
        weights[point] = weightsF.Get('h_ISRWeight_lastPt_%d_%d' % point) 

    outputFile = rt.TFile.Open('foo.root','recreate')
    def cloneTree(point):
        newTree = oldTree.CloneTree(0)
        newTree.SetName('%s_%s' % (oldTree.GetName(),decoratePoint(point)) )
        newTree.Branch('PROC',rt.AddressOf(extras,'PROC'),'PROC/I')
        newTree.Branch('LEP_W',rt.AddressOf(extras,'LEP_W'),'LEP_W/D')
        newTree.Branch('LEP_W_SYS',rt.AddressOf(extras,'LEP_W_SYS'),'LEP_W_SYS/D')
        newTree.Branch('LEP_W_OLD',rt.AddressOf(extras,'LEP_W_OLD'),'LEP_W_OLD/D')
        newTree.Branch('W_EFF',rt.AddressOf(extras,'W_EFF'),'W_EFF/D')
        newTree.Branch('W',rt.AddressOf(extras,'W'),'W/D')
        newTree.Branch('BOX_NUM',rt.AddressOf(extras,'BOX_NUM'),'BOX_NUM/I')
        newTree.Branch('BOX_NUM_GEN',rt.AddressOf(extras,'BOX_NUM_GEN'),'BOX_NUM_GEN/I')
        newTree.Branch('ISR_W',rt.AddressOf(extras,'ISR_W'),'ISR_W/D')
        return newTree                
        
    #make one tree per point
    trees = {}
    for point in norms.iterkeys():
        trees[point] = cloneTree(point)

        
    #fill the trees by point
    #for i in xrange(10000):
    for i in xrange(oldTree.GetEntries()):
        oldTree.GetEntry(i)
        
        point = (oldTree.mStop,oldTree.mLSP)
        
        #print point
        total = 1.*norms[point]
        extras.W_EFF = 1.0/total

        #extras.BOX_NUM = getBox(1,oldTree.nElectronTight,oldTree.nMuonTight,oldTree.nTauTight)
        extras.BOX_NUM_GEN = getBoxGenLevel(oldTree.genInfo)

        extras.ISR_W = getBinContent(weights[point],oldTree.diTopPt)
        #print extras.ISR_W,point,oldTree.diTopPt

        #choose between Had and BJet boxes
        if oldTree.nBJet > 0 and oldTree.nLepton == 0:
            extras.BOX_NUM = 6
            #take a data/MC scale factor
            extras.LEP_W = oldTree.BTAG_W
            extras.LEP_W_OLD = FullSimCentral.getBTagScaleWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL)*FastSimCentral.getBTagScaleWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL)

            #extras.LEP_W_OLD = getBTagScale(oldTree.maxTCHE_PT)*getBTagScaleFastSimTCHEM(oldTree.maxTCHE_PT)
            LEP_W_SYS_OLD = quad(FullSimError.getBTagScaleErrorWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL), FastSimError.getBTagScaleErrorWithFlavour(oldTree.maxTCHE_PT, oldTree.maxTCHE_ETA, model, oldTree.maxTCHE_FL))

            BTAG_UP = rt.TMath.Sqrt( oldTree.BTAG_W_BC_UP**2 + min( oldTree.BTAG_W_LT_UP, oldTree.BTAG_W_LT_DW )**2 + oldTree.BTAG_W_DT_DW**2)
            BTAG_DW = rt.TMath.Sqrt( oldTree.BTAG_W_BC_DW**2 + min( oldTree.BTAG_W_LT_UP, oldTree.BTAG_W_LT_DW )**2 + oldTree.BTAG_W_DT_UP**2)
            #build in some bounds to kill off the very long error tails from FastSim correction factors
            extras.LEP_W_SYS =  min(BTAG_UP,BTAG_DW, 3*LEP_W_SYS_OLD, 0.2*extras.LEP_W)
            #extras.LEP_W_SYS =  LEP_W_SYS_OLD

            #as we add and subtract, don't let this weight go negative
            if extras.LEP_W_SYS > extras.LEP_W:
                extras.LEP_W_SYS = extras.LEP_W - 1e-6
            
            #print '(%i/%i) first tag/pt/eta=(%f,%f,%f) pog=(%f \pm %f); wr = (%f \pm %f)' % (oldTree.nJet, oldTree.nBJet, oldTree.maxTCHE, oldTree.maxTCHE_PT, oldTree.maxTCHE_FL, extras.LEP_W, extras.LEP_W_SYS, extras.LEP_W_OLD, LEP_W_SYS_OLD)
            
        elif oldTree.nBJetLoose == 0:
            #Had Box
            extras.BOX_NUM = 5
            #ignore the mis-tag here
            extras.LEP_W = 1.0
            extras.LEP_W_OLD = 1.0
        else:
            #this is neither our control channel or the signal box
            continue

        #the number of generator level taus
        nGenTau = int(str(oldTree.genInfo)[-1])
        if nGenTau > 0:
            #the data/MC scale factor for the tau ID
            extras.LEP_W *= (1/0.979) #i.e. tau events are more likely to pass the selection in data than in MC as the veto is worse
        
        #just store the BJet box for now
        if extras.BOX_NUM != 6: continue
        trees[point].Fill()

    #save all of the trees in one file
    for tree in trees.itervalues():
        tree.Write()
    outputFile.Close()
