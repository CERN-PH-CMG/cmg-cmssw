import ROOT as rt
import os, pickle, array

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

if __name__ == '__main__':

    import glob
    fileList = glob.glob('SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z-Summer12-START52_V9_FSIM-v1-wreece_231112_*.root')
    fileList = fileList[:10]
    oldTree = rt.TChain('RMRTree')
    for f in fileList:
        oldTree.Add(f)
    outputFile = rt.TFile.Open('sms-histos-T2tt-BDT-231112.root','recreate')

    def label(hist, zlabel = None):
        hist.GetXaxis().SetTitle("Stop mass [GeV]")
        hist.GetYaxis().SetTitle('LSP mass [GeV]')
        if zlabel is not None:
            hist.GetZaxis().SetTitle(zlabel)

    #norms = mergeNorms(fileList)
    norms = pickle.load(file('/afs/cern.ch/user/w/wreece/work/CMGTools/V5_6_0/CMGTools/CMSSW_5_3_3_patch3/src/CMGTools/Susy/macros/MultiJet/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z-Summer12-START52_V9_FSIM-v1-PAT_CMG_V5_6_0_B.pkl'))
    print norms
    bins = getBins(norms)

    effHad = rt.TH2D("effHad",'effHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effBJet = rt.TH2D("effBJet",'effBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effBJetSel = rt.TH2D("effBJetSel",'effBJetSel',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effBJetSelBDT = rt.TH2D("effBJetSelBDT",'effBJetSelBDT',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effEle = rt.TH2D("effEle",'effEle',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effMu = rt.TH2D("effMu",'effMu',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effTauBox = rt.TH2D("effTauBox",'effTauBox',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    label(effHad)
    label(effBJet)
    label(effEle)
    label(effMu)
    label(effTauBox)
    label(effBJetSel)
    label(effBJetSelBDT)

    mrHad = rt.TH3D("mrHad",'mrHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],35,500,4000)
    mrBJet = rt.TH3D("mrBJet",'mrBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],35,500,4000)
    label(mrHad,'M_{R} [GeV]')
    label(mrBJet,'M_{R} [GeV]')

    rsqHad = rt.TH3D("rsqHad",'rsqHad',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],10,0.,1.)
    rsqBJet = rt.TH3D("rsqBJet",'rsqBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],10,0.,1.)
    label(rsqHad,'R^{2}')
    label(rsqBJet,'R^{2}')

    bdtBJet = rt.TH3D("bdtBJet",'bdtBJet',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2],10,-1.,1.)
    label(bdtBJet,'BDT Output')

    effTau = rt.TH2D("effTau",'effTau',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    effTauAll = rt.TH2D("effTauAll",'effTauAll',bins[0][0],bins[0][1],bins[0][2],bins[1][0],bins[1][1],bins[1][2])
    label(effTau)
    label(effTauAll)

    sel = BranchDumper(oldTree)
    tr = sel.make_tree()

    reader = rt.TMVA.Reader()
    bdt_vars = {}
    for h in sel.headers_for_MVA():
        bdt_vars['%s_var'%h] = array.array('f',[0])
        reader.AddVariable(h,bdt_vars['%s_var'%h])
        #reader.AddVariable(rt.TString(h),rt.AddressOf(sel.vars,h))
    mr_var = array.array('f',[0])
    rsq_var = array.array('f',[0])
    nvertex_var = array.array('f',[0])
    reader.AddSpectator('MR',mr_var)
    reader.AddSpectator('RSQ',rsq_var)
    reader.AddSpectator('nVertex',nvertex_var)
    reader.BookMVA('BDT','/afs/cern.ch/user/w/wreece/work/CMGTools/V5_6_0/CMGTools/CMSSW_5_3_3_patch3/src/CMGTools/Susy/prod/MultiJet/TMVAClassification_BDT.weights.xml')

    nSig = 0
    nSigBDT = 0

    #fill the trees by point
    for i in xrange(10000):
    #for i in xrange(oldTree.GetEntries()):
        oldTree.GetEntry(i)
        
        point = (oldTree.mStop,oldTree.mLSP)
        weight = 1/(1.*norms[point])

        nGenTau = int(str(oldTree.genInfo)[-1])
        effTauAll.Fill(point[0],point[1],weight)
        if nGenTau > 0:
            effTau.Fill(point[0],point[1],weight)

        # we no longer use the PFtaus, so the tight taus go into the HAD box
        BOX_NUM = getBox(oldTree.nCSVM,oldTree.nElectronTight,oldTree.nMuonTight,0)

        #must pass the selection and the triggers
        if not ( (oldTree.hadBoxFilter and oldTree.hadTriggerFilter and BOX_NUM in [5,6]) or (oldTree.tauBoxFilter and oldTree.hadTriggerFilter and BOX_NUM in [7]) or\
                (oldTree.eleBoxFilter and oldTree.eleTriggerFilter and BOX_NUM in [0,2,4]) or (oldTree.muBoxFilter and oldTree.muTriggerFilter and BOX_NUM in [0,1,3]) ):
            continue

        #impose some razor cuts
        if oldTree.MR < 450 or oldTree.RSQ < 0.03:
            continue

        nLoose = oldTree.nElectronLoose + oldTree.nMuonLoose
        nTight = oldTree.nElectronTight + oldTree.nMuonTight

        #veto extra leptons in the single lepton boxes
        if BOX_NUM in [3,4,7] and (nLoose > 1 or nTight > 1): continue

        if BOX_NUM in [5,6] and not sel.select(): continue
        if BOX_NUM in [5,6]:
            nSig += 1
            for h in sel.headers_for_MVA():
                bdt_vars['%s_var'%h][0] = getattr(sel,h)()
            bdt = reader.EvaluateMVA('BDT')

            if BOX_NUM == 6:
                bdtBJet.Fill(point[0],point[1],bdt)
                effBJetSel.Fill(point[0],point[1],weight)
            if bdt < -0.093: continue

            if BOX_NUM == 6:
                effBJetSelBDT.Fill(point[0],point[1],weight)
            nSigBDT += 1

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

    effBJetSelBDT.Divide(effBJetSel)

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

    bdtBJet.Write()
    effBJetSelBDT.Write()
    effBJetSel.Write()
    
    outputFile.Close()
    
    print nSig, nSigBDT
