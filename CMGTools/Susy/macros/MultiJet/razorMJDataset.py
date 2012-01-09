#!/usr/bin/env python

import ROOT as rt
import os, sys, math, glob
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile

def listDirectory(dir, inputFiles, maxFiles):
    if not os.path.exists(dir):
        raise Exception("The directory '%s' does not exist" % dir)
    files = glob.glob('%s/*.root' % dir)
    if len(files) > maxFiles:
        files = files[:maxFiles]
    inputFiles.extend(files)
    return len(files)

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
    options.outputFile = 'razorMJDatasets.root'
    options.register ('inputDirectory',
                  None, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "A directory to read root files from")
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
    if options.inputDirectory is not None:
        listDirectory(options.inputDirectory, options.inputFiles, options.maxFiles)
        
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
    Double_t mR;\
    Double_t R;\
    Double_t Rsq;\
    Double_t maxTCHE;\
    Double_t maxSSVP;\
    Double_t nextTCHE;\
    Double_t nextSSVP;\
    Double_t met;\
    Double_t jet1Pt;\
    Double_t jet2Pt;\
    Double_t jet3Pt;\
    Double_t jet4Pt;\
    Double_t jet5Pt;\
    Double_t jet6Pt;\
    Double_t jet1Eta;\
    Double_t jet2Eta;\
    Double_t jet3Eta;\
    Double_t jet4Eta;\
    Double_t jet5Eta;\
    Double_t jet6Eta;\
    Double_t hemi1Mass;\
    Double_t hemi2Mass;\
};""")
    
    rt.gROOT.ProcessLine("""
struct Info{\
    Int_t event;\
    Int_t run;\
    Int_t lumi;\
    Int_t nJet;\
    Int_t nMuonLoose;\
    Int_t nMuonTight;\
    Int_t nElectronLoose;\
    Int_t nElectronTight;\
    Int_t nTauLoose;\
    Int_t nTauTight;\
    Int_t nVertex;\
    Int_t hemisphereBalance;\
};""")
    
    filter_tags = [('ecalDeadCellTPfilter',''),
                   ('HBHENoiseFilterResultProducer2010','HBHENoiseFilterResult'),
                   ('HBHENoiseFilterResultProducer2011IsoDefault','HBHENoiseFilterResult'),
                   ('HBHENoiseFilterResultProducer2011NonIsoRecommended','HBHENoiseFilterResult'),
                   ('eeNoiseFilter','Result'),
                   ('goodPrimaryVertexFilter','Result'),
                   ('greedyMuonsTagging','Result'),
                   ('inconsistentMuonsTagging','Result'),
                   ('recovRecHitFilter','Result'),
                   ('scrapingFilter','Result'),
                   ('trackingFailureFilter','Result')]

    rt.gROOT.ProcessLine("""
struct Filters{\
    Bool_t triggerFilter;\
    Bool_t quadTriggerFilter;\
    Bool_t sixTriggerFilter;\
    Bool_t eightTriggerFilter;\
    Bool_t ecalDeadCellTPfilter;\
    Bool_t HBHENoiseFilterResultProducer2010;\
    Bool_t HBHENoiseFilterResultProducer2011IsoDefault;\
    Bool_t HBHENoiseFilterResultProducer2011NonIsoRecommended;\
    Bool_t eeNoiseFilter;\
    Bool_t goodPrimaryVertexFilter;\
    Bool_t greedyMuonsTagging;\
    Bool_t inconsistentMuonsTagging;\
    Bool_t recovRecHitFilter;\
    Bool_t scrapingFilter;\
    Bool_t trackingFailureFilter;\
};""")

    from ROOT import Variables, Info, Filters

#    output = rt.TFile.Open(options.outputFile,'recreate')
    tree = rt.TTree('RMRTree','Multijet events')
    tree.SetDirectory(0)
    def setAddress(obj, flag):
        for branch in dir(obj):
            if branch.startswith('__'): continue
            tree.Branch(branch,rt.AddressOf(obj,branch),'%s/%s' % (branch,flag) )
    
    filters = Filters()
    info = Info()
    vars = Variables()
    
    setAddress(filters,'O')
    setAddress(info,'I')
    setAddress(vars,'D')

    # use Varparsing object
    events = Events(options)
    
    #make some handles
    jetSel30H = Handle("std::vector<cmg::PFJet>")
    hemiHadH = Handle("std::vector<cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> >")
    metH = Handle("std::vector<cmg::BaseMET>")

    electronH = Handle("std::vector<cmg::Electron>")
    muonH = Handle("std::vector<cmg::Muon>")
    tauH = Handle("std::vector<cmg::Tau>")
    
    triggerH = Handle('std::vector<cmg::TriggerObject>')
    countH = Handle('int')
    filterH = Handle('bool')
    
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")

    store = RootFile.RootFile(options.outputFile)
    store.add(tree)

    count = 0

    # loop over events
    for event in events:
        
        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()

        #start by vetoing events that didn't pass the MultiJet path
        pathTriggerNames = event.object().triggerNames(pathTrigger)
        path = pathTrigger.wasrun(pathTriggerNames.triggerIndex('razorMJPath')) and \
               pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJPath'))
        if not path: continue

        event.getByLabel(('razorMJPFJetSel30'),jetSel30H)
        jets = jetSel30H.product()
        info.nJet = len(jets)

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
            tree.AutoSave()

        for f in filter_tags:
            event.getByLabel(f,filterH)
            print f,filterH,filterH.product()

        for i in xrange(len(jets)):
            name = 'jet%iPt' % (i + 1)
            if hasattr(vars,name):
                jet = jets[i]
                setattr(vars,name,jet.pt())
                setattr(vars,'jet%iEta'%(i+1),jet.eta())

        tche = sorted([j.btag(0) for j in jets if abs(j.eta()) <= 2.4])
        ssvp = sorted([j.btag(5) for j in jets if abs(j.eta()) <= 2.4])

        if len(tche) < 2:
            continue

        vars.maxTCHE = tche[-1]
        vars.maxSSVP = ssvp[-1]
        vars.nextTCHE = tche[-2]
        vars.nextSSVP = ssvp[-2]

        event.getByLabel(('cmgPFMET'),metH)
        met = metH.product()[0]
        vars.met = met.et()

        event.getByLabel(('cmgTriggerObjectSel'),triggerH)
        hlt = triggerH.product()[0]

        filters.quadTriggerFilter = hlt.getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$")
        filters.sixTriggerFilter = hlt.getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$")
        filters.eightTriggerFilter = hlt.getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$")
        filters.triggerFilter = filters.quadTriggerFilter or filters.sixTriggerFilter or filters.eightTriggerFilter
        
        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if not len(hemiHadH.product()): continue
        hemiHad = hemiHadH.product()[0]

        vars.R = hemiHad.R()
        vars.Rsq = hemiHad.Rsq()
        vars.mR = hemiHad.mR()
        vars.hemi1Mass = hemiHad.leg1().mass()
        vars.hemi2Mass = hemiHad.leg2().mass()
        info.hemisphereBalance = (10*hemiHad.leg1().numConstituents()) + hemiHad.leg2().numConstituents()

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

        tree.Fill()
        count += 1

#    tree.SetDirectory(output)
#    tree.Write()
#    output.Close()
    store.write()
