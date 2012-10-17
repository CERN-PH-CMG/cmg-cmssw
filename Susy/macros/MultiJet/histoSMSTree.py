import ROOT as rt
import os, pickle

from writeOptimFile import BranchDumper

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
    if len(gen) < 4:
        return -1
    nB = int(gen[-4])
    nEle = int(gen[-3])
    nMu = int(gen[-2])
    nTau = int(gen[-1])
    return getBox(nB,nEle,nMu,nTau, diLep = True)

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

def getBins(points):

    binsX = set([p[0] for p in points.keys()])
    binsY = set([p[1] for p in points.keys()])

    binsX = sorted([x for x in binsX])
    binsY = sorted([y for y in binsY])

    widthX = abs(binsX[0]-binsX[-1])/(1.*len(binsX)-1)
    widthY = abs(binsY[0]-binsY[-1])/(1.*len(binsY)-1)
    print widthX,binsX[0],binsX[-1],len(binsX)

    return [ (len(binsX),binsX[0]-(0.5*widthX),binsX[-1]+(0.5*widthX)), (len(binsY),binsY[0]-(0.5*widthY),binsY[-1]+(0.5*widthY)) ]

class Selector(object):

    def __init__(self, tree):

        self.tree = tree
        self.hemi = 1

    def chi2(self):
        chi1 = (abs(self.tree.hemi1TopMass-173.5)/57.)+(abs(self.tree.hemi1WMass-80.385)/44.)
        chi2 = (abs(self.tree.hemi2TopMass-173.5)/57.)+(abs(self.tree.hemi2WMass-80.385)/44.)
        if chi1 <= chi2:
            self.hemi = 1
        else:
            self.hemi = 2
        return min(chi1,chi2)

    def thetaH(self):
        if self.hemi == 1:
            return self.tree.hemi1ThetaH
        return self.tree.hemi2ThetaH

    def topMass(self):
        if self.hemi == 1:
            return self.tree.hemi1TopMass
        return self.tree.hemi2TopMass

    def jet1pt(self):
        return self.tree.jet_pt[0]

    def jet2pt(self):
        return self.tree.jet_pt[1]

    def jet3pt(self):
        return self.tree.jet_pt[2]

    def jet4pt(self):
        return self.tree.jet_pt[3]

    def jet5pt(self):
        return self.tree.jet_pt[4]

    def jet6pt(self):
        return self.tree.jet_pt[5]

    def headers(self):
        return ['chi2','thetaH','jet1pt','jet2pt','jet3pt','jet4pt','jet5pt','jet6pt']

    def values(self):
        values = []
        for h in self.headers():
            values.append(getattr(self,h)())
        return values

    def select(self):
        c = self.chi2()
        result = self.topMass() >= 80. and self.topMass() <= 270. and self.thetaH() < 0.7 and self.tree.hemi1Count > 1 and self.tree.hemi2Count > 1
        return result

        
if __name__ == '__main__':

    import glob
    fileList = glob.glob('SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z-Summer12-START52_V9_FSIM-v1-wreece_011012_*.root')
    #fileList = fileList[:3]
    oldTree = rt.TChain('RMRTree')
    for f in fileList:
        oldTree.Add(f)
    outputFile = rt.TFile.Open('sms-histos-T2tt.root','recreate')

    def label(hist, zlabel = None):
        hist.GetXaxis().SetTitle("Stop mass [GeV]")
        hist.GetYaxis().SetTitle('LSP mass [GeV]')
        if zlabel is not None:
            hist.GetZaxis().SetTitle(zlabel)

    norms = mergeNorms(fileList)
    #norms = pickle.load(file('/afs/cern.ch/user/w/wreece/work/LimitSetting/RazorMultiJet2011/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_030412-ByPoint-3.pkl'))
    print norms
    bins = getBins(norms)

    effHad = rt.TH2D("effHad",'effHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effBJet = rt.TH2D("effBJet",'effBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effEle = rt.TH2D("effEle",'effEle',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effMu = rt.TH2D("effMu",'effMu',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effTauBox = rt.TH2D("effTauBox",'effTauBox',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    label(effHad)
    label(effBJet)
    label(effEle)
    label(effMu)
    label(effTauBox)

    mrHad = rt.TH3D("mrHad",'mrHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],35,500,4000)
    mrBJet = rt.TH3D("mrBJet",'mrBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],35,500,4000)
    label(mrHad,'M_{R} [GeV]')
    label(mrBJet,'M_{R} [GeV]')

    rsqHad = rt.TH3D("rsqHad",'rsqHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],10,0.,1.)
    rsqBJet = rt.TH3D("rsqBJet",'rsqBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],10,0.,1.)
    label(rsqHad,'R^{2}')
    label(rsqBJet,'R^{2}')

    effTau = rt.TH2D("effTau",'effTau',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effTauAll = rt.TH2D("effTauAll",'effTauAll',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    label(effTau)
    label(effTauAll)

    sel = Selector(oldTree)

    #fill the trees by point
    #for i in xrange(10000):
    for i in xrange(oldTree.GetEntries()):
        oldTree.GetEntry(i)
        
        point = (oldTree.mStop,oldTree.mLSP)
        weight = 1/(1.*norms[point])

        nGenTau = int(str(oldTree.genInfo)[-1])
        effTauAll.Fill(point[0],point[1],weight)
        if nGenTau > 0:
            effTau.Fill(point[0],point[1],weight)

        #set the number of btags
        if oldTree.nCSVL == 0:
            NBJET = 0 #bjet veto
        elif oldTree.nCSVM > 1:
            NBJET = oldTree.nCSVL #loose-loose etc
        else:
            NBJET = oldTree.nCSVM #minimum is one medium

        BOX_NUM = getBox(NBJET,oldTree.nElectronTight,oldTree.nMuonTight,oldTree.nTauTight)

        #must pass the selection and the triggers
        if not ( (oldTree.hadBoxFilter and oldTree.hadTriggerFilter and BOX_NUM in [5,6]) or (oldTree.tauBoxFilter and oldTree.hadTriggerFilter and BOX_NUM in [7]) or\
                (oldTree.eleBoxFilter and oldTree.eleTriggerFilter and BOX_NUM in [0,2,4]) or (oldTree.muBoxFilter and oldTree.muTriggerFilter and BOX_NUM in [0,1,3]) ):
            continue

        #impose some razor cuts
        if oldTree.MR < 450 or oldTree.RSQ < 0.03:
            continue

        nLoose = oldTree.nElectronLoose + oldTree.nMuonLoose + oldTree.nTauLoose
        nTight = oldTree.nElectronTight + oldTree.nMuonTight + oldTree.nTauTight

        #veto extra leptons in the Had boxes
        if BOX_NUM in [5,6] and (nLoose > 0 or nTight > 0): continue
        #veto extra leptons in the single lepton boxes
        if BOX_NUM in [3,4,7] and (nLoose > 1 or nTight > 1): continue

        if BOX_NUM in [5,6] and not sel.select(): continue

        #choose between Had and BJet boxes
        if BOX_NUM == 6:
            #BJet box
            effBJet.Fill(point[0],point[1],weight)
            mrBJet.Fill(point[0],point[1],oldTree.MR)
            rsqBJet.Fill(point[0],point[1],oldTree.RSQ)

        elif BOX_NUM == 5:
            #Had CR
            effHad.Fill(point[0],point[1],weight)
            mrHad.Fill(point[0],point[1],oldTree.MR)
            rsqHad.Fill(point[0],point[1],oldTree.RSQ)

        elif BOX_NUM == 3:
            #the Single Lepton CR
            effMu.Fill(point[0],point[1],weight)

        elif BOX_NUM == 4:
            #the Single Lepton CR
            effEle.Fill(point[0],point[1],weight)

        elif BOX_NUM == 7:
            #the Single Lepton CR
            effTauBox.Fill(point[0],point[1],weight)

    effTau.Sumw2()
    effTauAll.Sumw2()
    effTau.Divide(effTauAll)

    #effHad.Divide(effTauAll)
    #effBJet.Divide(effTauAll)
    #effEle.Divide(effTauAll)
    #effMu.Divide(effTauAll)
    #effTauBox.Divide(effTauAll)

    effHad.Write()
    effBJet.Write()
    effTauBox.Write()
    effMu.Write()
    effEle.Write()


    effTau.Write()
    effTauAll.Write()

    mrHad.Write()
    mrBJet.Write()

    rsqHad.Write()
    rsqBJet.Write()
    
    outputFile.Close()
    
