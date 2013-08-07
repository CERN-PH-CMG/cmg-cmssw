#!/usr/bin/env python
import ROOT as rt
import sys

# Had(0), Ele(1), Mu(2), Tau(3), EleEle(4), EleMu(5), EleTau(6), MuMu(7), MuTau(8), TauTau(9)
def getGenBox(genInfo):

    #e, mu, tau
    info = str(genInfo)
    e = int(info[-3])
    m = int(info[-2])
    t = int(info[-1])
    
    box = 0
    if e == 2:
        #ele-ele
        box = 4
    elif e == 1 and m == 1:
        #ele-ele
        box = 5
    elif e == 1 and t == 1:
        #ele-tau
        box = 6
    elif m == 2:
        #mu-mu
        box = 7
    elif m == 1 and t == 1:
        #ele-ele
        box = 8
    elif t == 2:
        #tau-tau
        box = 9
    elif e == 1:
        #ele
        box = 1
    elif m == 1:
        #mu
        box = 2
    elif t == 1:
        #tau
        box = 3

    print genInfo,box,e,m,t
    return box

def getRecoBox(nEleTight,nMuTight,nTauTight,nEleLoose,nMuLoose,nTauLoose):

    nLooseLeptons = nEleLoose+nMuLoose+nTauLoose
    nTightLeptons = nEleTight+nMuTight+nTauTight
    # Had(0), Ele(1), Mu(2), Tau(3), EleEle(4), EleMu(5), EleTau(6), MuMu(7), MuTau(8), TauTau(9)
    box = -1
    if (nEleTight == 1 and nMuLoose > 0) or (nMuTight == 1 and nEleLoose > 0):
        #ele-mu
        box = 5
    elif (nEleTight == 1 and nTauLoose > 0) or (nTauTight == 1 and nEleLoose > 0):
        #ele-tau
        box = 6
    elif (nMuTight == 1 and nTauLoose > 0) or (nTauTight == 1 and nMuLoose > 0):
        #mu-tau
        box = 8
    elif nEleTight == 1 and nEleLoose > 1:
        #ele-ele
        box = 4
    elif nMuTight == 1 and nMuLoose > 1:
        #mu-mu
        box = 7
    elif nTauTight == 1 and nTauLoose > 1:
        #tau-tau
        box = 9
    elif nEleTight == 1 and nLooseLeptons == 1:
        #ele
        box = 1
    elif nMuTight == 1 and nLooseLeptons == 1:
        #mu
        box = 2
    elif nTauTight == 1 and nLooseLeptons == 1:
        #tau
        box = 3
    elif nTightLeptons == 0:
        box = 0
    
    return box

def setAxisLabels(axis):
    labels={0:'Had', 1:'Ele', 2:'Mu', 3:'Tau', 4:'EleEle', 5:'EleMu', 6:'EleTau', 7:'MuMu', 8:'MuTau', 9:'TauTau'}
    for i in xrange(10):
        axis.SetBinLabel(i+1,labels[i])

def getFullSelectionBox(tree):

    box = -1
    if tree.muBoxFilter and tree.muTriggerFilter:
        box = 2
    elif tree.eleBoxFilter and tree.eleTriggerFilter:
        box = 1
    elif tree.tauBoxFilter and tree.hadTriggerFilter:
        box = 3
    elif tree.hadBoxFilter and tree.hadTriggerFilter:
        box = 0
    return box


if __name__ == '__main__':

    rootFileName = sys.argv[1]
    inputFile = rt.TFile.Open(rootFileName)
    
    tree = inputFile.Get('RMRTree')
    print 'Running on %d entries' % tree.GetEntries()

    genBoxes = rt.TH1F('genBoxes','genBoxes',10,0,10)
    setAxisLabels(genBoxes.GetXaxis())
    genBoxesTrigger = genBoxes.Clone('genBoxesTrigger')
    genBoxesBoxes = genBoxes.Clone('genBoxesBoxes')
    genBoxesJets = genBoxes.Clone('genBoxesJets')
    genBoxesSelection = genBoxes.Clone('genBoxesSelection')

    genRecoBoxes = rt.TH2F('genRecoBoxes','genRecoBoxes',10,0,10,10,0,10)
    genRecoBoxes.GetXaxis().SetTitle('Gen box')
    genRecoBoxes.GetYaxis().SetTitle('Reco box')
    setAxisLabels(genRecoBoxes.GetXaxis())
    setAxisLabels(genRecoBoxes.GetYaxis())
    genRecoBoxesNoCuts = genRecoBoxes.Clone('genRecoBoxesNoCuts')
    genRecoBoxesSel = genRecoBoxes.Clone('genRecoBoxesSel')

    genBoxesJets2D = rt.TH2F('genBoxesJets2D','genBoxesJets',10,0,10,10,0,10)
    genBoxesJets2D.GetXaxis().SetTitle('Gen box')
    genBoxesJets2D.GetYaxis().SetTitle('Num Jets')
    setAxisLabels(genBoxesJets2D.GetXaxis())
    setAxisLabels(genBoxesJets2D.GetYaxis())

    mrRsqHad = rt.TH2F('mrRsqHad','mrRsqHad',20,0,2000,10,0,1)
    mrRsqHad.GetXaxis().SetTitle('M_{R} [GeV]')
    mrRsqHad.GetYaxis().SetTitle('R^{2}')
    
    mrRsqEle = mrRsqHad.Clone('mrRsqEle')
    mrRsqMu = mrRsqHad.Clone('mrRsqMu')
    mrRsqTau = mrRsqHad.Clone('mrRsqTau')

    for i in xrange(tree.GetEntries()):
    #for i in xrange(1000):
        tree.GetEntry(i)
        
        genBox = getGenBox(tree.genInfo)
        genBoxes.Fill(genBox)

        selectionBox = getFullSelectionBox(tree)
        if selectionBox >= 0:
            genRecoBoxesSel.Fill(genBox,selectionBox)

        #we require that the 
        if not tree.hadBoxFilter and not tree.eleBoxFilter and not tree.muBoxFilter and not tree.tauBoxFilter:
            continue
        genBoxesSelection.Fill(genBox)
        genBoxesTrigger.Fill(genBox)
        genBoxesJets2D.Fill(genBox,tree.nJetCleaned)

        recoBox = getRecoBox(tree.nElectronTight,tree.nMuonTight,tree.nTauTight,tree.nElectronLoose,tree.nMuonLoose,tree.nTauLoose)
        genRecoBoxesNoCuts.Fill(genBox,recoBox)

        if (recoBox == 0 and tree.nJet < 6) or (recoBox > 0 and tree.nJetCleaned < 4):
            continue
        genBoxesJets.Fill(genBox)

        if recoBox not in [0,1,2,3,5]:
            continue
        
        mr = tree.mR
        rsq = tree.Rsq
        if mr < 500 or rsq < 0.03:
            continue

        genRecoBoxes.Fill(genBox,recoBox)
        genBoxesBoxes.Fill(genBox)

    
        if recoBox == 0: mrRsqHad.Fill(mr,rsq)
        if recoBox == 1: mrRsqEle.Fill(mr,rsq)
        if recoBox == 2: mrRsqMu.Fill(mr,rsq)
        if recoBox == 3: mrRsqTau.Fill(mr,rsq)

    genBoxesTrigger.Divide(genBoxes)
    genBoxesBoxes.Divide(genBoxes)
    genBoxesJets.Divide(genBoxes)
    genBoxesSelection.Divide(genBoxes)

    outputFile = rt.TFile.Open('makeLeptonPlots.root','recreate')
    genBoxes.Write()
    genBoxesTrigger.Write()
    genBoxesBoxes.Write()
    genBoxesJets.Write()
    genRecoBoxes.Write()
    genBoxesJets2D.Write()
    genRecoBoxesNoCuts.Write()
    mrRsqHad.Write()
    mrRsqEle.Write()
    mrRsqMu.Write()
    mrRsqTau.Write()
    genBoxesSelection.Write()
    genRecoBoxesSel.Write()
    outputFile.Close()
