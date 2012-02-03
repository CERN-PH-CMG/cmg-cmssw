#!/usr/bin/env python

import ROOT as rt
import os, sys, math, glob
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
        name = '%s-%s-%s-SMS.root' % (names[0],names[1],names[-1])
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
struct Arrays{\
    std::vector<double> CTEQ66_Wi;\
    std::vector<double> MRST2006NNLO_Wi;\
};""")

    rt.gROOT.ProcessLine("""
struct Info{\
    Int_t event;\
    Int_t run;\
    Int_t lumi;\
    Int_t nJet;\
    Int_t nBJet;\
    Int_t nMuonLoose;\
    Int_t nMuonTight;\
    Int_t nElectronLoose;\
    Int_t nElectronTight;\
    Int_t nTauLoose;\
    Int_t nTauTight;\
    Int_t nVertex;\
};""")
    
    rt.gROOT.ProcessLine("""
struct Filters{\
    Bool_t triggerFilter;\
    Bool_t quadTriggerFilter;\
    Bool_t sixTriggerFilter;\
    Bool_t eightTriggerFilter;\
    Bool_t selectionFilter;\
};""")

    from ROOT import Variables, Info, Filters

    tree = rt.TTree('RMRTree','Multijet events')
    tree.SetDirectory(0)
    def setAddress(obj, flag):
        for branch in dir(obj):
            if branch.startswith('__'): continue
            if flag:
                tree.Branch(branch,rt.AddressOf(obj,branch),'%s/%s' % (branch,flag) )
            else:
                tree.Branch(branch,rt.AddressOf(obj,branch),branch)
    
    filters = Filters()
    info = Info()
    vars = Variables()
    CTEQ66_Wi = std.vector('double')()
    MRST2006NNLO_Wi = std.vector('double')()
    
    setAddress(filters,'O')
    setAddress(info,'I')
    setAddress(vars,'D')
    tree.Branch('CTEQ66_Wi',CTEQ66_Wi)
    tree.Branch('MRST2006NNLO_Wi',MRST2006NNLO_Wi)

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

    store = RootFile.RootFile(options.outputFile)
    store.add(tree)

    count = 0

    # loop over events
    for event in events:

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()
        
        CTEQ66_Wi.clear()
        MRST2006NNLO_Wi.clear()
        
        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
            tree.AutoSave()
        count += 1

        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()

        #start by vetoing events that didn't pass the MultiJet path
        pathTriggerNames = event.object().triggerNames(pathTrigger)
        path = pathTrigger.wasrun(pathTriggerNames.triggerIndex('razorMJPath')) and \
               pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJPath'))
        filters.selectionFilter = path
        #if not path: continue

        event.getByLabel(('razorMJPFJetSel30'),jetSel30H)
        if not jetSel30H.isValid(): continue
        jets = jetSel30H.product()
        info.nJet = len(jets)

        event.getByLabel(('cmgTriggerObjectSel'),triggerH)
        hlt = triggerH.product()[0]

        filters.quadTriggerFilter = hlt.getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$")
        filters.sixTriggerFilter = hlt.getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$")
        filters.eightTriggerFilter = hlt.getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$")
        filters.l1MultiJetFilter = hlt.getSelectionRegExp("^HLT_L1MultiJet_v[0-9]+$")
        filters.triggerFilter = filters.quadTriggerFilter or filters.sixTriggerFilter or filters.eightTriggerFilter
            
        tche = sorted([(j.pt(),j) for j in jets if abs(j.eta()) <= 2.4 and j.btag(0) >= 3.3])
        if tche:
            vars.maxTCHE = tche[-1][1].btag(0)
            vars.maxTCHE_PT = tche[-1][1].pt()
            vars.maxTCHE_ETA = tche[-1][1].eta()
        info.nBJet = len(tche)
        
        #also get the old definition
        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ = hemi.Rsq()
            vars.MR = hemi.mR()

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
        info.nElectronTight = len(ele_sel_tight)

        mu_sel_loose = [m for m in muons if m.pt() >= 10 and abs(m.eta()) < 2.4 and m.getSelection("cuts_vbtfmuon_isGlobal") and m.getSelection("cuts_vbtfmuon_numberOfValidTrackerHits")]
        mu_sel_tight = [m for m in mu_sel_loose if m.pt() >= 15 and abs(m.eta()) < 2.1 and m.getSelection("cuts_vbtfmuon") and m.relIso(0.5) < 0.15]
        info.nMuonLoose = len(mu_sel_loose)
        info.nMuonTight = len(mu_sel_tight)
        
        tau_sel_loose = [t for t in taus if t.pt() >= 15 and abs(t.tauID("byLooseCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3]
        tau_sel_tight = [t for t in tau_sel_loose if abs(t.tauID("byMediumCombinedIsolationDeltaBetaCorr") - 1.0) < 1e-3]
        info.nTauLoose = len(tau_sel_loose)
        info.nTauTight = len(tau_sel_tight)
        info.nVertex = countH.product()[0]

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

        #dump the PDF weights
        event.getByLabel(('dumpPdfWeights','cteq66'),pdfH)
        for w in pdfH.product():
            CTEQ66_Wi.push_back(w)
        event.getByLabel(('dumpPdfWeights','MRST2006nnlo'),pdfH)
        for w in pdfH.product():
            MRST2006NNLO_Wi.push_back(w)

        tree.Fill()
        if count > 1000: break

    store.write()
