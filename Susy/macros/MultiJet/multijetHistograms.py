#!/usr/bin/env python

import ROOT as rt
import sys, math, glob
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile

def listDirectory(dir):
    return glob.glob('%s/*.root' % dir)

def getFiles(datasets, user, pattern):
    
    files = []
    
    for d in datasets:
        ds = datasetToSource(
                             user,
                             d,
                             pattern
                             )
        files.extend(ds.fileNames)

    #root://eoscms//eos/cms/store/cmst3/user/wreece/CMG/MultiJet/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim/wreece_051211/susy_tree_CMG_0.root
    return ['root://eoscms//eos/cms%s' % f for f in files]

if __name__ == '__main__':

    from CMGTools.Production.datasetToSource import *
#    files = getFiles(
#                     [#'/MultiJet/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim/wreece_051211',
#                      #'/MultiJet/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim/wreece_051211',
#                      #'/MultiJet/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim/wreece_051211',
#                      '/MultiJet/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim/wreece_090112',
#                      #'/MultiJet/Run2011B-PromptReco-v1/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim/wreece_051211'
#                      ],
#                      'wreece',
#                      'susy_tree_CMG_[0-9]+.root'
#                     )
    files = getFiles(
                     ['/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0/wreece_060112',
                      ],
                      'wreece',
                      'susy_tree_CMG_[0-9]+.root'
                     )
    #

    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = 'multijetHistograms.root'
    
    options.register ('inputDirectory',
                  None, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "A directory to read root files from")
    
    options.parseArguments()
    #print options.inputFiles
    options.inputFiles = files

    # use Varparsing object
    events = Events(options)
    
    #make some handles
    muonH = Handle("std::vector<cmg::Muon>")
    allPFBJetsH = Handle("std::vector<cmg::PFJet>")
    muonPFBJetsH = Handle("std::vector<cmg::PFJet>")
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")
    
    store = RootFile.RootFile(options.outputFile)
    ptAllBJets = rt.TH1D('ptAllBJets','pT of all tagged jets',200,0,2000)
    store.add(ptAllBJets)
    
    ptMuBJets = rt.TH1D('ptMuBJets','pT of tagged jets with a muon',200,0,2000)
    store.add(ptMuBJets)
    
    ptMu = rt.TH1D('ptMu','pT of selected muons',29,10,300)
    store.add(ptMu)
    
    ptMuJ = rt.TH1D('ptMuJ','pT of selected muons within jets',29,10,300)
    store.add(ptMuJ)
    
    deltaRMuJ = rt.TH1D('deltaRMuJ','deltaR between the muon and the b jet',40,0,4)
    store.add(deltaRMuJ)
    
    ptMuJet = rt.TH2D('ptMuJet','pt of muon and jet',15,50,600,15,50,600)
    store.add(ptMuJet)
    
    tcheAllBJets = rt.TH1D('tcheAllBJets','TCHE',50,0,50)
    store.add(tcheAllBJets)
    ssvhpAllBJets = rt.TH1D('ssvhpAllBJets','SSVHP',50,0,50)
    store.add(ssvhpAllBJets)
    
    tcheMuBJets = rt.TH1D('tcheMuBJets','TCHE',50,0,50)
    store.add(tcheMuBJets)
    ssvhpMuBJets = rt.TH1D('ssvhpMuBJets','SSVHP',50,0,50)
    store.add(ssvhpMuBJets)
    
#    ssvhptAllBJets = rt.TH1D('ssvhptAllBJets','TCHE',50,0,50)
    
    def deltaR(a,b):
        deta = a.eta() - b.eta()
        dphi = a.phi() - b.phi()
        dr = deta*deta + dphi*dphi
        return math.sqrt(dr)

    event_count = 0

    # loop over events
    for event in events:
        
        if not event_count % 1000:
            print 'Event %d' % event_count
        event_count += 1
        
#        if event_count > 10000:
#            break
        
        #start by testing that the Razor path fired this event
        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()
        
        #start by vetoing events that didn't pass the MultiJet path
        pathTriggerNames = event._event.triggerNames(pathTrigger)
        path = pathTrigger.wasrun(pathTriggerNames.triggerIndex('multijetPath')) and pathTrigger.accept(pathTriggerNames.triggerIndex('multijetPath'))
        if not path: continue
        
        #fill the handles
        event.getByLabel(('multiMuonLoose'), muonH)
        muons = muonH.product()
        
        event.getByLabel(('cmgPFJetSel'), allPFBJetsH)
        #allPFBJets = [j for j in allPFBJetsH.product() if (j.btag(0) >= 3.3 and j.pt() >= 40 and abs(j.eta()) <= 2.5)]
        allPFBJets = [j for j in allPFBJetsH.product() if (j.btag(5) >= 2.0 and j.pt() >= 40 and abs(j.eta()) <= 2.5)]
        if not allPFBJets: continue
        
        event.getByLabel(('multiPFBJetsMuonRequiredSSVHPT'), muonPFBJetsH)
        #event.getByLabel(('multiPFBJetsMuonRequiredTCHEM'), muonPFBJetsH)
        muonPFBJets = muonPFBJetsH.product()
        
        for m in muons:
            ptMu.Fill(m.pt())
        
        for b in allPFBJets:
            ptAllBJets.Fill(b.pt())
            tcheAllBJets.Fill(b.btag(0))
            ssvhpAllBJets.Fill(b.btag(5))
            
        for b in muonPFBJets:
            ptMuBJets.Fill(b.pt())
            tcheMuBJets.Fill(b.btag(0))
            ssvhpAllBJets.Fill(b.btag(5))
            
            for m in muons:
                dr = deltaR(b,m)
                deltaRMuJ.Fill(dr)
                if dr < 0.5:
                    ptMuJet.Fill(m.pt(),b.pt())
                    ptMuJ.Fill(m.pt())
                
    ptMuEff = ptMuBJets.Clone('ptJetEff')
    ptMuEff.Sumw2()
    ptMuEff.Divide(ptAllBJets)
    store.add(ptMuEff)
    
    ptMuJEff = ptMuJ.Clone('ptMuJEff')
    ptMuJEff.Sumw2()
    ptMuJEff.Divide(ptMu)
    store.add(ptMuJEff)

    
    store.write()
