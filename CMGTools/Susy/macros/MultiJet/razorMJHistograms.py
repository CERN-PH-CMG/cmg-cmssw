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

def deltaR(a,b):
    deta = a.eta() - b.eta()
    dphi = a.phi() - b.phi()
    dr = deta*deta + dphi*dphi
    return math.sqrt(dr)

if __name__ == '__main__':

    runOnMC = False

    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = None
    options.register ('inputDirectory',
                  None, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "A directory to read root files from")
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
    if options.inputDirectory is not None:
        listDirectory(options.inputDirectory, options.inputFiles, options.maxFiles)
        
    if True:
        names = [f for f in options.datasetName.split('/') if f]
        name = '%s-%s-%s.root' % (names[0],names[1],names[-1])
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
    Int_t genInfo;\
};""")
    
    filter_tags = [('ecalDeadCellTPfilter'),
                   ('HBHENoiseFilterResultProducer2010'),
                   ('HBHENoiseFilterResultProducer2011IsoDefault'),
                   ('HBHENoiseFilterResultProducer2011NonIsoRecommended'),
                   ('eeNoiseFilter'),
                   ('goodPrimaryVertexFilter'),
                   ('greedyMuonsTagging'),
                   ('inconsistentMuonsTagging'),
                   ('recovRecHitFilter'),
                   ('scrapingFilter'),
                   ('trackingFailureFilter')]

    rt.gROOT.ProcessLine("""
struct Filters{\
    Bool_t triggerFilter;\
    Bool_t quadTriggerFilter;\
    Bool_t sixTriggerFilter;\
    Bool_t eightTriggerFilter;\
    Bool_t l1MultiJetFilter;\
    Bool_t selectionFilter;\
    Bool_t ht250TriggerFilter;\
    Bool_t ht300TriggerFilter;\
    Bool_t razorTriggerFilter;\
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

    filters = Filters()
    info = Info()
    vars = Variables()
    
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
    
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")

    store = RootFile.RootFile(options.outputFile)

    #fucking stupid plots for ARC
    #could you point me to the kinematic distributions of these merged jets (megajets) like pT, eta, mass, deltaR(j1,j2). Number of jets in merged jets, Number of b-jets in merged jet. 
    
    #pt
    hemi1Pt = rt.TH1D('hemi1Pt','hemi1Pt',350,0.,700.)
    hemi2Pt = rt.TH1D('hemi2Pt','hemi2Pt',350,0.,700.)
    store.add(hemi1Pt)
    store.add(hemi2Pt)

    #eta
    hemi1Eta = rt.TH1D('hemi1Eta','hemi1Eta',32,-3.2,3.2)
    hemi2Eta = rt.TH1D('hemi2Eta','hemi2Eta',32,-3.2,3.2)
    store.add(hemi1Eta)
    store.add(hemi2Eta)
    
    #mass
    hemi1Mass = rt.TH1D('hemi1Mass','hemi1Mass',350,0.,700.)
    hemi2Mass = rt.TH1D('hemi2Mass','hemi2Mass',350,0.,700.)
    store.add(hemi1Mass)
    store.add(hemi2Mass)

    #deltaR
    deltaRHemi = rt.TH1D('deltaRHemi','deltaRHemi',64,0,6.4)
    store.add(deltaRHemi)

    #number of merged jets
    hemi1NJet = rt.TH1D('hemi1NJet','hemi1NJet',10,0.,10)
    hemi2NJet = rt.TH1D('hemi2NJet','hemi2NJet',10,0.,10)
    store.add(hemi1NJet)
    store.add(hemi2NJet)

    #number of merged b-jets
    hemi1NBJet = rt.TH1D('hemi1NBJet','hemi1NBJet',10,0.,10)
    hemi2NBJet = rt.TH1D('hemi2NBJet','hemi2NBJet',10,0.,10)
    store.add(hemi1NBJet)
    store.add(hemi2NBJet)


    count = 0

    # loop over events
    for event in events:

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
        count += 1    

        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()

        #start by vetoing events that didn't pass the offline selection
        pathTriggerNames = event.object().triggerNames(pathTrigger)
        path = pathTrigger.wasrun(pathTriggerNames.triggerIndex('multijetPathNoTrigger')) and \
               pathTrigger.accept(pathTriggerNames.triggerIndex('multijetPathNoTrigger'))
        filters.selectionFilter = path
        if not path: continue

        event.getByLabel(('razorMJPFJetSel30'),jetSel30H)
        if not jetSel30H.isValid(): continue
        jets = jetSel30H.product()
        info.nJet = len(jets)
        if info.nJet < 6: continue

        event.getByLabel(('cmgTriggerObjectSel'),triggerH)
        hlt = triggerH.product()[0]

        filters.quadTriggerFilter = hlt.getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$")
        filters.sixTriggerFilter = hlt.getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$")
        filters.eightTriggerFilter = hlt.getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$")
        filters.l1MultiJetFilter = hlt.getSelectionRegExp("^HLT_L1MultiJet_v[0-9]+$")
        filters.ht250TriggerFilter = hlt.getSelectionRegExp("^HLT_HT250_v[0-9]+$")
        filters.ht300TriggerFilter = hlt.getSelectionRegExp("^HLT_HT300_v[0-9]+$")
        filters.razorTriggerFilter = hlt.getSelectionRegExp("^HLT_R0[0-9]+_MR[0-9]+_v[0-9]+$")
        filters.triggerFilter = filters.quadTriggerFilter or filters.sixTriggerFilter or filters.eightTriggerFilter
        if not filters.triggerFilter: continue
        #if not filters.l1MultiJetFilter: continue
        
        if not runOnMC:
            for f in filter_tags:
                event.getByLabel(f,filterH)
                if filterH.isValid():
                    result = filterH.product()[0]
                    setattr(filters,f,result)

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

        #loose lepton ID
        event.getByLabel(('razorMJElectronLoose'),electronH)
        event.getByLabel(('razorMJMuonLoose'),muonH)
        event.getByLabel(('razorMJTauLoose'),tauH)
        #
        info.nElectronLoose = len(electronH.product())
        info.nMuonLoose = len(muonH.product())
        info.nTauLoose = len(tauH.product())

        #tight lepton ID
        event.getByLabel(('razorMJElectronTight'),electronH)
        event.getByLabel(('razorMJMuonTight'),muonH)
        event.getByLabel(('razorMJTauTight'),tauH)
        #
        info.nElectronTight = len(electronH.product())
        info.nMuonTight = len(muonH.product())
        info.nTauTight = len(tauH.product())
        if (info.nElectronTight + info.nMuonTight + info.nTauTight) > 0:
            continue

        event.getByLabel(('vertexSize'),countH)
        info.nVertex = countH.product()[0]

        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.Rsq = hemi.Rsq()
            vars.mR = hemi.mR()
            vars.hemi1Mass = hemi.leg1().mass()
            vars.hemi2Mass = hemi.leg2().mass()
            info.hemisphereBalance = (10*hemi.leg1().numConstituents()) + hemi.leg2().numConstituents()

            if vars.Rsq < 0.03 or vars.mR < 500:
                continue

            hemi1Pt.Fill(hemi.leg1().pt())
            hemi2Pt.Fill(hemi.leg2().pt())

            hemi1Eta.Fill(hemi.leg1().eta())
            hemi2Eta.Fill(hemi.leg2().eta())
            
            hemi1Mass.Fill(hemi.leg1().mass())
            hemi2Mass.Fill(hemi.leg2().mass())

            deltaRHemi.Fill(deltaR(hemi.leg1(),hemi.leg2()))

            hemi1NJet.Fill(hemi.leg1().numConstituents())
            hemi2NJet.Fill(hemi.leg2().numConstituents())

            jets1 = []
            jets2 = []
            for j in jets:
                if abs(j.eta()) > 2.4 or j.btag(0) < 3.3:
                    continue

                dr1 = deltaR(j, hemi.leg1())
                dr2 = deltaR(j, hemi.leg2())
                
                if dr1 < dr2:
                    jets1.append(j)
                else:
                    jets2.append(j)

            hemi1NBJet.Fill(len(jets1))
            hemi2NBJet.Fill(len(jets2))

        
        if runOnMC:
            event.getByLabel(('simpleGenInfo'),filterH)
            if filterH.isValid():
                info.genInfo = filterH.product()[0]

    store.write()
