#!/usr/bin/env python

import ROOT as rt
import sys, math
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile

if __name__ == '__main__':

    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = 'multijetHistograms.root'
    options.parseArguments()

    # use Varparsing object
    events = Events(options)
    
    #make some handles
    muonH = Handle("std::vector<cmg::Muon>")
    allPFBJetsH = Handle("std::vector<cmg::PFJet>")
    muonPFBJetsH = Handle("std::vector<cmg::PFJet>")
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")
    
    store = RootFile.RootFile(options.outputFile)
    ptAllBJets = rt.TH1D('ptAllBJets','pT of all tagged jets',15,50,600)
    store.add(ptAllBJets)
    
    ptMuBJets = rt.TH1D('ptMuBJets','pT of tagged jets with a muon',15,50,600)
    store.add(ptMuBJets)
    
    ptMu = rt.TH1D('ptMu','pT of selected muons',15,10,200)
    store.add(ptMu)
    
    ptMuJ = rt.TH1D('ptMuJ','pT of selected muons within jets',15,10,200)
    store.add(ptMuJ)
    
    deltaRMuJ = rt.TH1D('deltaRMuJ','deltaR between the muon and the b jet',15,0,200)
    store.add(ptMuJ)
    
    ptMuJet = rt.TH2D('ptMuJet','pt of muon and jet',15,50,600,15,50,600)
    store.add(ptMuJet)
    
    def deltaR(a,b):
        deta = a.eta() - b.eta()
        dphi = a.phi() - b.phi()
        dr = deta*deta + dphi*dphi
        return math.sqrt(dr)

    # loop over events
    for event in events:
        
        #start by testing that the Razor path fired this event
        event.getByLabel(('TriggerResults','','SUSY'),pathTriggerH)
        pathTrigger = pathTriggerH.product()
        
        #start by vetoing events that didn't pass the MultiJet path
        pathTriggerNames = event._event.triggerNames(pathTrigger)
        path = pathTrigger.wasrun(pathTriggerNames.triggerIndex('multijetPath')) and pathTrigger.accept(pathTriggerNames.triggerIndex('multijetPath'))
        if not path: continue
        
        #fill the handles
        event.getByLabel(('multiMuonLoose'), muonH)
        muons = muonH.product()
        
        event.getByLabel(('multiPFBJetSel60'), allPFBJetsH)
        allPFBJets = allPFBJetsH.product()
        
        event.getByLabel(('multiPFBJetsMuonRequired'), muonPFBJetsH)
        muonPFBJets = muonPFBJetsH.product()
        
        for m in muons:
            ptMu.Fill(m.pt())
        
        for b in allPFBJets:
            ptAllBJets.Fill(b.pt())
            
        for b in muonPFBJets:
            ptMuBJets.Fill(b.pt())
            
            for m in muons:
                if deltaR(b,m) < 0.5:
                    ptMuJet.Fill(m.pt(),b.pt())
                    ptMuJ.Fill(m.pt())
                
        
    store.write()
