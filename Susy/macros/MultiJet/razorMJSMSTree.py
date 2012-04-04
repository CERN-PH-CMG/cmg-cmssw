#!/usr/bin/env python

import ROOT as rt
import os, sys, math, glob, copy, pickle
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile

import numpy as n
from ROOT import std

def getFiles(datasets, user, pattern):
    
    from CMGTools.Production.datasetToSource import datasetToSource

    files = []
    for d in datasets:
        ds = datasetToSource(
                             user,
                             d,
                             pattern
                             )
        files.extend(ds.fileNames)

    return ['root://eoscms//eos/cms%s' % f for f in files]

if __name__ == '__main__':

    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = None
    options.register ('outputDirectory',
                  '.', # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "A directory to write root files to")
    options.register ('datasetName',
                  None,
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "A directory to read root files from")
    options.register ('maxFiles',
                  -1, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.int,          # string, int, or float
                  "The maximum number of files to read")
    
    options.parseArguments()
    if True:
        names = [f for f in options.datasetName.split('/') if f]
        name = '%s-%s-%s-SMS-NoSkim.root' % (names[0],names[1],names[-1])
        options.outputFile = os.path.join(options.outputDirectory,name)
        
    files = getFiles(
                      [options.datasetName],
                      'wreece',
                      'susy_tree_CMG_[0-9]+.root'

                     )
    if options.maxFiles > 0:
        options.inputFiles = files[0:options.maxFiles]
    else:
        options.inputFiles = files

    rt.gROOT.ProcessLine("""
struct Variables{\
    Double_t MR;\
    Double_t MR_JES_UP;\
    Double_t MR_JES_DOWN;\
    Double_t RSQ;\
    Double_t RSQ_JES_UP;\
    Double_t RSQ_JES_DOWN;\
    Double_t maxTCHE;\
    Double_t maxTCHE_PT;\
    Double_t maxTCHE_ETA;\
    Double_t mStop;\
    Double_t mLSP;\
};""")

    rt.gROOT.ProcessLine("""
struct Info{\
    Int_t event;\
    Int_t run;\
    Int_t lumi;\
    Int_t nJet;\
    Int_t nBJet;\
    Int_t nMuonLoose;\
    Int_t nElectronLoose;\
    Int_t nTauLoose;\
    Int_t nVertex;\
};""")
    
    from ROOT import Variables, Info

    output = rt.TFile.Open(options.outputFile,"recreate")
    output.cd()
    tree = rt.TTree('RMRTree','Multijet events')
    def setAddress(obj, flag):
        for branch in dir(obj):
            if branch.startswith('__'): continue
            if flag:
                tree.Branch(branch,rt.AddressOf(obj,branch),'%s/%s' % (branch,flag) )
            else:
                tree.Branch(branch,rt.AddressOf(obj,branch),branch)
    
    info = Info()
    vars = Variables()
    BTAG_TrackCount = std.vector('double')()    
    CTEQ66_W = std.vector('double')()
    MRST2006NNLO_W = std.vector('double')()
    
    setAddress(info,'I')
    setAddress(vars,'D')
    tree.Branch('BTAG_TrackCount',BTAG_TrackCount)
    tree.Branch('CTEQ66_W',CTEQ66_W)
    tree.Branch('MRST2006NNLO_W',MRST2006NNLO_W)

    # use Varparsing object
    events = Events(options)

    #make some handles
    jetSel30H = Handle("std::vector<cmg::PFJet>")
    hemiHadH = Handle("std::vector<cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> >")
    metH = Handle("std::vector<cmg::BaseMET>")
    lheH = Handle('LHEEventProduct')

    electronH = Handle("std::vector<cmg::Electron>")
    muonH = Handle("std::vector<cmg::Muon>")
    tauH = Handle("std::vector<cmg::Tau>")
    
    triggerH = Handle('std::vector<cmg::TriggerObject>')
    countH = Handle('int')
    filterH = Handle('int')
    pdfH = Handle('std::vector<double>')
    
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")

    count = 0

    bins = {}

    # loop over events
    for event in events:

        #get the LHE product info
        event.getByLabel(('source'),lheH)
        lhe = lheH.product()
        for i in xrange(lhe.comments_size()):
            comment = lhe.getComment(i)
            if 'model' not in comment: continue
            comment = comment.replace('\n','')
            parameters = comment.split(' ')[-1]
            masses = map(float,parameters.split('_')[-2:])
            vars.mStop = masses[0]
            vars.mLSP = masses[1]

            #keep track of how many points are in each model
            point = (vars.mStop,vars.mLSP)
            if bins.has_key(point):
                bins[point] = bins[point]+1
            else:
                bins[point] = 1

        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()

        #start by vetoing events that didn't pass the MultiJet path
        pathTriggerNames = event.object().triggerNames(pathTrigger)
        path = pathTrigger.wasrun(pathTriggerNames.triggerIndex('razorMJPath')) and \
               pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJPath'))
        #event must pass the full selection
        if not path: continue

        event.getByLabel(('cmgTriggerObjectSel'),triggerH)
        hlt = triggerH.product()[0]

        quadTriggerFilter = hlt.getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$")
        sixTriggerFilter = hlt.getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$")
        eightTriggerFilter = hlt.getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$")
        triggerFilter = quadTriggerFilter or sixTriggerFilter or eightTriggerFilter
        #event must have fired the trigger
        if not triggerFilter: continue

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()
        
        CTEQ66_W.clear()
        MRST2006NNLO_W.clear()
        BTAG_TrackCount.clear()
        
        event.getByLabel(('razorMJPFJetSel30'),jetSel30H)
        if not jetSel30H.isValid(): continue
        jets = jetSel30H.product()
        info.nJet = len(jets)
        
        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ = hemi.Rsq()
            vars.MR = hemi.mR()
            #filter out events failing the analysis cuts
            if vars.MR < 500 or vars.RSQ < 0.03: continue
        else:
            #skip events with no hemi
            continue

        event.getByLabel(('razorMJDiHemiHadBoxUp'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ_JES_UP = hemi.Rsq()
            vars.MR_JES_UP = hemi.mR()

        event.getByLabel(('razorMJDiHemiHadBoxDown'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ_JES_DOWN = hemi.Rsq()
            vars.MR_JES_DOWN = hemi.mR()

        event.getByLabel(('cmgElectronSel'),electronH)
        event.getByLabel(('cmgMuonSel'),muonH)
        event.getByLabel(('cmgTauSel'),tauH)
        event.getByLabel(('vertexSize'),countH)

        electrons = electronH.product()
        muons = muonH.product()
        taus = tauH.product()

        #count leptons
        ele_sel_loose = [e for e in electrons if e.pt() >= 10 and abs(e.eta()) < 2.5 and (abs(e.eta()) < 1.442 or abs(e.eta()) > 1.556) and e.getSelection("cuts_vbtf95ID")]
        ele_sel_tight = [e for e in ele_sel_loose if e.pt() >= 20 and e.getSelection("cuts_vbtf80ID") and abs(e.dxy()) < 0.02 and e.relIso() < 0.2]
        info.nElectronLoose = len(ele_sel_loose)
        nElectronTight = len(ele_sel_tight)

        mu_sel_loose = [m for m in muons if m.pt() >= 10 and abs(m.eta()) < 2.4 and m.getSelection("cuts_vbtfmuon_isGlobal") and m.getSelection("cuts_vbtfmuon_numberOfValidTrackerHits")]
        mu_sel_tight = [m for m in mu_sel_loose if m.pt() >= 15 and abs(m.eta()) < 2.1 and m.getSelection("cuts_vbtfmuon") and m.relIso(0.5) < 0.15]
        info.nMuonLoose = len(mu_sel_loose)
        nMuonTight = len(mu_sel_tight)
        
        tau_sel_loose = [t for t in taus if t.pt() >= 15 and abs(t.tauID("byLooseCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3]
        tau_sel_tight = [t for t in tau_sel_loose if abs(t.tauID("byMediumCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3]
        info.nTauLoose = len(tau_sel_loose)
        nTauTight = len(tau_sel_tight)
        info.nVertex = countH.product()[0]

        nLepton = nElectronTight + nMuonTight + nTauTight
        #veto events with a tight lepton
        if nLepton > 0: continue

        #dump the B-tags
        tche = sorted([(j.btag(0),j) for j in jets if abs(j.eta()) <= 2.4 and j.btag(0) >= 3.3])
        if tche:
            vars.maxTCHE = tche[-1][1].btag(0)
            vars.maxTCHE_PT = tche[-1][1].pt()
            vars.maxTCHE_ETA = tche[-1][1].eta()
            
        info.nBJet = len(tche)
        if info.nBJet > 1:
            BTAG_TrackCount.push_back(tche[-1][1].btag(0))
            BTAG_TrackCount.push_back(tche[-2][1].btag(0))
        elif info.nBJet > 0:
            BTAG_TrackCount.push_back(tche[-1][1].btag(0))
            BTAG_TrackCount.push_back(-1)
        else:
            BTAG_TrackCount.push_back(-1)
            BTAG_TrackCount.push_back(-1)

        #dump the PDF weights
        event.getByLabel(('dumpPdfWeights','cteq66'),pdfH)
        for w in pdfH.product():
            CTEQ66_W.push_back(w)
        event.getByLabel(('dumpPdfWeights','MRST2006nnlo'),pdfH)
        for w in pdfH.product():
            MRST2006NNLO_W.push_back(w)

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
            if (count % 100000) == 0:
                tree.AutoSave()
        count += 1
        tree.Fill()
        #if count > 2000: break

    output.cd()    
    tree.Write()
    output.Close()

    sample_counts = file(options.outputFile.replace('.root','.pkl'),'wb')
    print bins
    pickle.dump(bins,sample_counts)
    sample_counts.close()
    

    
