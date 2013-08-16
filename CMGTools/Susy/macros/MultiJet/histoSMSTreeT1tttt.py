import ROOT as rt
import os, pickle

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

from histoSMSTree import getBins
                                                        
if __name__ == '__main__':

    import glob
    fileList = glob.glob('SMS-T1tttt_Mgluino-450to1200_mLSP-50to800_7TeV-Pythia6Z-Summer11-PU_START42_V11_FSIM-v2-wreece_080612-BPOG_*.root')
    #fileList = fileList[:2]
    oldTree = rt.TChain('RMRTree')
    for f in fileList:
        oldTree.Add(f)
    outputFile = rt.TFile.Open('sms-histos-T1tttt-rb.root','recreate')

    def label(hist, zlabel = None):
        hist.GetXaxis().SetTitle('gluino mass [GeV]')
        hist.GetYaxis().SetTitle('LSP mass [GeV]')
        if zlabel is not None:
            hist.GetZaxis().SetTitle(zlabel)

    #norms = mergeNorms(fileList)
    norms = pickle.load(file("/afs/cern.ch/user/w/wreece/work/LimitSetting/RazorMultiJet2011/SMS-T1tttt_Mgluino-450to1200_mLSP-50to800_7TeV-Pythia6Z-Summer11-PU_START42_V11_FSIM-v2-wreece_080612-ByPoint-3.pkl"))
    print norms
    bins = getBins(norms)

    effHad = rt.TH2D("effHad",'effHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effBJet = rt.TH2D("effBJet",'effBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effLep = rt.TH2D("effLep",'effLep',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    label(effHad)
    label(effBJet)
    label(effLep)

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

        #choose between Had and BJet boxes
        if oldTree.nBJet > 0 and oldTree.nLepton == 0:
            #BJet box
            effBJet.Fill(point[0],point[1],weight)
            mrBJet.Fill(point[0],point[1],oldTree.MR)
            rsqBJet.Fill(point[0],point[1],oldTree.RSQ)

        elif oldTree.nBJetLoose == 0 and oldTree.nLepton == 0:
            #Had CR
            effHad.Fill(point[0],point[1],weight)
            mrHad.Fill(point[0],point[1],oldTree.MR)
            rsqHad.Fill(point[0],point[1],oldTree.RSQ)

        elif oldTree.nBJetLoose == 0 and oldTree.nLepton == 1:
            #the Single Lepton CR
            effLep.Fill(point[0],point[1],weight)

    effTau.Sumw2()
    effTauAll.Sumw2()
    effTau.Divide(effTauAll)

    effHad.Write()
    effBJet.Write()
    effLep.Write()

    effTau.Write()
    effTauAll.Write()

    mrHad.Write()
    mrBJet.Write()

    rsqHad.Write()
    rsqBJet.Write()
    
    outputFile.Close()
    
