#!/usr/bin/env python

import ROOT as rt
import sys, copy
from DataFormats.FWLite import Events, Handle, Lumis, Runs
from CMGTools.RootTools import RootFile, RootTools
from CMGTools.Susy.plots import TriggerHistogram
from CMGTools.Susy.plots import TriggerEfficiencyHistogram
from CMGTools.Susy.plots import StackManager

if __name__ == '__main__':

    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = 'leptonicStopHistograms.root'
    options.parseArguments()

    lumis = Lumis(options)
    
    totalSimInput = 0
    totalSimOutput = 0
    totalPatInput = 0
    totalPatOutput = 0

    simFilterInfoH = Handle('GenFilterInfo')
    patFilterInfoH = Handle('GenFilterInfo')
    
    weights = []
    for lumi in lumis:
        lumi.getByLabel(('genFilterEfficiencyProducer','','SIM'),simFilterInfoH)
        lumi.getByLabel(('preSelFilterEfficiencyProducer','','PAT'),patFilterInfoH)

        totalSimInput += simFilterInfoH.product().numEventsTried()
        totalSimOutput += simFilterInfoH.product().numEventsPassed()
        
        totalPatInput += patFilterInfoH.product().numEventsTried()
        totalPatOutput += patFilterInfoH.product().numEventsPassed()
        
        fe = simFilterInfoH.product().filterEfficiency()
        sc = patFilterInfoH.product().numEventsTried()/(1.*simFilterInfoH.product().numEventsPassed())
        
        w = fe/(sc*simFilterInfoH.product().numEventsTried())
        weights.append(w)
        print w
    print 'Mean',sum(weights)/(1.*len(weights))
    
    genFilterEfficiency = totalSimInput/(1.*totalSimOutput)
    scalingFraction = totalPatInput/(1.*totalSimOutput)    
    print totalSimInput,totalSimOutput,totalPatInput,totalPatOutput,genFilterEfficiency,scalingFraction

    options.parseArguments()
    runs = Runs(options)
    
    genInfoH = Handle('GenRunInfoProduct')
    cross_sections = []
    for run in runs:
        run.getByLabel('generator',genInfoH)
        cross_sections.append(genInfoH.product().crossSection())
    genCrossSection = sum(cross_sections)/(1.*len(cross_sections))
    print genCrossSection
    
    intLumi = 2005.2
    #eventWeight = intLumi*(genCrossSection*genFilterEfficiency)/(scalingFraction*totalSimInput)
    eventWeight = (genFilterEfficiency)/(scalingFraction*totalSimInput)
    print 'Event Weight',eventWeight

    # use Varparsing object
    options.parseArguments()
    events = Events(options)
      
    store = RootFile.RootFile(options.outputFile)

    hltFired = TriggerHistogram.TriggerHistogram('hltFired','Paths fired in the HLT')
    triggerEff = TriggerEfficiencyHistogram.TriggerEfficiencyHistogram(store, dir='TriggerEfficiencyHistogram')
    triggerEff.AddTrigger('leptonicStopTriggerPath')
    triggerEff.Add1D('LeadingJetPt', 100, 0, 1000)
    
    
    pathSelectHistograms = StackManager.MultiStack()
    pathSelectHistograms['NumberOfJets'] = StackManager.StackManager('NumberOfJets',store, dir='PathSelections')
    pathSelectHistograms['NumberOfJets'].Add1D('OfflineSelected',15,0,15).SetFillColor(rt.kRed)
    pathSelectHistograms['NumberOfJets'].Add1D('OnlineSelected',15,0,15).SetFillColor(rt.kBlue)
    pathSelectHistograms['NumberOfJets'].Add1D('Selected',15,0,15).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['LeadingJetPt'] = StackManager.StackManager('LeadingJetPt',store, dir='PathSelections')
    pathSelectHistograms['LeadingJetPt'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['LeadingJetPt'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['LeadingJetPt'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['SecondJetPt'] = StackManager.StackManager('SecondJetPt',store, dir='PathSelections')
    pathSelectHistograms['SecondJetPt'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['SecondJetPt'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['SecondJetPt'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['ThirdJetPt'] = StackManager.StackManager('ThirdJetPt',store, dir='PathSelections')
    pathSelectHistograms['ThirdJetPt'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['ThirdJetPt'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['ThirdJetPt'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['FourthJetPt'] = StackManager.StackManager('FourthJetPt',store, dir='PathSelections')
    pathSelectHistograms['FourthJetPt'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['FourthJetPt'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['FourthJetPt'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['LeadingJetEta'] = StackManager.StackManager('LeadingJetEta',store, dir='PathSelections')
    pathSelectHistograms['LeadingJetEta'].Add1D('OfflineSelected',64,-3.2,3.2).SetFillColor(rt.kRed)
    pathSelectHistograms['LeadingJetEta'].Add1D('OnlineSelected',64,-3.2,3.2).SetFillColor(rt.kBlue)
    pathSelectHistograms['LeadingJetEta'].Add1D('Selected',64,-3.2,3.2).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['SecondJetEta'] = StackManager.StackManager('SecondJetEta',store, dir='PathSelections')
    pathSelectHistograms['SecondJetEta'].Add1D('OfflineSelected',64,-3.2,3.2).SetFillColor(rt.kRed)
    pathSelectHistograms['SecondJetEta'].Add1D('OnlineSelected',64,-3.2,3.2).SetFillColor(rt.kBlue)
    pathSelectHistograms['SecondJetEta'].Add1D('Selected',64,-3.2,3.2).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['ThirdJetEta'] = StackManager.StackManager('ThirdJetEta',store, dir='PathSelections')
    pathSelectHistograms['ThirdJetEta'].Add1D('OfflineSelected',64,-3.2,3.2).SetFillColor(rt.kRed)
    pathSelectHistograms['ThirdJetEta'].Add1D('OnlineSelected',64,-3.2,3.2).SetFillColor(rt.kBlue)
    pathSelectHistograms['ThirdJetEta'].Add1D('Selected',64,-3.2,3.2).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['FourthJetEta'] = StackManager.StackManager('FourthJetEta',store, dir='PathSelections')
    pathSelectHistograms['FourthJetEta'].Add1D('OfflineSelected',64,-3.2,3.2).SetFillColor(rt.kRed)
    pathSelectHistograms['FourthJetEta'].Add1D('OnlineSelected',64,-3.2,3.2).SetFillColor(rt.kBlue)
    pathSelectHistograms['FourthJetEta'].Add1D('Selected',64,-3.2,3.2).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['NumberOfMuons'] = StackManager.StackManager('NumberOfMuons',store, dir='PathSelections')
    pathSelectHistograms['NumberOfMuons'].Add1D('OfflineSelected',15,0,15).SetFillColor(rt.kRed)
    pathSelectHistograms['NumberOfMuons'].Add1D('OnlineSelected',15,0,15).SetFillColor(rt.kBlue)
    pathSelectHistograms['NumberOfMuons'].Add1D('Selected',15,0,15).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['NumberOfElectrons'] = StackManager.StackManager('NumberOfElectrons',store, dir='PathSelections')
    pathSelectHistograms['NumberOfElectrons'].Add1D('OfflineSelected',15,0,15).SetFillColor(rt.kRed)
    pathSelectHistograms['NumberOfElectrons'].Add1D('OnlineSelected',15,0,15).SetFillColor(rt.kBlue)
    pathSelectHistograms['NumberOfElectrons'].Add1D('Selected',15,0,15).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['PFMET'] = StackManager.StackManager('PFMET',store, dir='PathSelections')
    pathSelectHistograms['PFMET'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['PFMET'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['PFMET'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['WENuMT'] = StackManager.StackManager('WENuMT',store, dir='PathSelections')
    pathSelectHistograms['WENuMT'].Add1D('OfflineSelected',100,0,500).SetFillColor(rt.kRed)
    pathSelectHistograms['WENuMT'].Add1D('OnlineSelected',100,0,500).SetFillColor(rt.kBlue)
    pathSelectHistograms['WENuMT'].Add1D('Selected',100,0,500).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['WMuNuMT'] = StackManager.StackManager('WMuNuMT',store, dir='PathSelections')
    pathSelectHistograms['WMuNuMT'].Add1D('OfflineSelected',100,0,500).SetFillColor(rt.kRed)
    pathSelectHistograms['WMuNuMT'].Add1D('OnlineSelected',100,0,500).SetFillColor(rt.kBlue)
    pathSelectHistograms['WMuNuMT'].Add1D('Selected',100,0,500).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['TCHELeadingJet'] = StackManager.StackManager('TCHELeadingJet',store, dir='PathSelections')
    pathSelectHistograms['TCHELeadingJet'].Add1D('OfflineSelected',100,0,100).SetFillColor(rt.kRed)
    pathSelectHistograms['TCHELeadingJet'].Add1D('OnlineSelected',100,0,100).SetFillColor(rt.kBlue)
    pathSelectHistograms['TCHELeadingJet'].Add1D('Selected',100,0,100).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['TCHESecondJet'] = StackManager.StackManager('TCHESecondJet',store, dir='PathSelections')
    pathSelectHistograms['TCHESecondJet'].Add1D('OfflineSelected',100,0,100).SetFillColor(rt.kRed)
    pathSelectHistograms['TCHESecondJet'].Add1D('OnlineSelected',100,0,100).SetFillColor(rt.kBlue)
    pathSelectHistograms['TCHESecondJet'].Add1D('Selected',100,0,100).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['TCHEThirdJet'] = StackManager.StackManager('TCHEThirdJet',store, dir='PathSelections')
    pathSelectHistograms['TCHEThirdJet'].Add1D('OfflineSelected',100,0,100).SetFillColor(rt.kRed)
    pathSelectHistograms['TCHEThirdJet'].Add1D('OnlineSelected',100,0,100).SetFillColor(rt.kBlue)
    pathSelectHistograms['TCHEThirdJet'].Add1D('Selected',100,0,100).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['TCHEFourthJet'] = StackManager.StackManager('TCHEFourthJet',store, dir='PathSelections')
    pathSelectHistograms['TCHEFourthJet'].Add1D('OfflineSelected',100,0,100).SetFillColor(rt.kRed)
    pathSelectHistograms['TCHEFourthJet'].Add1D('OnlineSelected',100,0,100).SetFillColor(rt.kBlue)
    pathSelectHistograms['TCHEFourthJet'].Add1D('Selected',100,0,100).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['NumberOfBJetsLoose'] = StackManager.StackManager('NumberOfBJetsLoose',store, dir='PathSelections')
    pathSelectHistograms['NumberOfBJetsLoose'].Add1D('OfflineSelected',15,0,15).SetFillColor(rt.kRed)
    pathSelectHistograms['NumberOfBJetsLoose'].Add1D('OnlineSelected',15,0,15).SetFillColor(rt.kBlue)
    pathSelectHistograms['NumberOfBJetsLoose'].Add1D('Selected',15,0,15).SetFillColor(rt.kGreen)

    pathSelectHistograms['NumberOfBJetsMedium'] = StackManager.StackManager('NumberOfBJetsMedium',store, dir='PathSelections')
    pathSelectHistograms['NumberOfBJetsMedium'].Add1D('OfflineSelected',15,0,15).SetFillColor(rt.kRed)
    pathSelectHistograms['NumberOfBJetsMedium'].Add1D('OnlineSelected',15,0,15).SetFillColor(rt.kBlue)
    pathSelectHistograms['NumberOfBJetsMedium'].Add1D('Selected',15,0,15).SetFillColor(rt.kGreen)    

    pathSelectHistograms['LeadingBJetPtLoose'] = StackManager.StackManager('LeadingBJetPtLoose',store, dir='PathSelections')
    pathSelectHistograms['LeadingBJetPtLoose'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['LeadingBJetPtLoose'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['LeadingBJetPtLoose'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['LeadingBJetPtMedium'] = StackManager.StackManager('LeadingBJetPtMedium',store, dir='PathSelections')
    pathSelectHistograms['LeadingBJetPtMedium'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['LeadingBJetPtMedium'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['LeadingBJetPtMedium'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)    
    
    pathSelectHistograms['SecondBJetPtLoose'] = StackManager.StackManager('SecondBJetPtLoose',store, dir='PathSelections')
    pathSelectHistograms['SecondBJetPtLoose'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['SecondBJetPtLoose'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['SecondBJetPtLoose'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen)
    
    pathSelectHistograms['SecondBJetPtMedium'] = StackManager.StackManager('SecondBJetPtMedium',store, dir='PathSelections')
    pathSelectHistograms['SecondBJetPtMedium'].Add1D('OfflineSelected',100,0,1000).SetFillColor(rt.kRed)
    pathSelectHistograms['SecondBJetPtMedium'].Add1D('OnlineSelected',100,0,1000).SetFillColor(rt.kBlue)
    pathSelectHistograms['SecondBJetPtMedium'].Add1D('Selected',100,0,1000).SetFillColor(rt.kGreen) 
    
    #make some handles
    jetH = Handle("std::vector<cmg::PFJet>")

    looseElectronH = Handle("std::vector<cmg::Electron>")    
    tightElectronH = Handle("std::vector<cmg::Electron>")
    
    looseMuonH = Handle("std::vector<cmg::Muon>")    
    tightMuonH = Handle("std::vector<cmg::Muon>")
    
    hemiHadH = Handle("std::vector<cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> >")
    metH = Handle("std::vector<cmg::BaseMET>")
    
    wENuH = Handle("std::vector<cmg::DiObject<cmg::Electron, cmg::BaseMET> >")
    wMuNuH = Handle("std::vector<cmg::DiObject<cmg::Muon, cmg::BaseMET> >")
    
    #HLT Skim
    hltH = Handle('std::vector<cmg::TriggerObject>')
    # path trigger
    pathselectH = Handle("std::vector<cmg::TriggerObject>")
    
    # loop over events
    for event in events:
        
        #get the HLT result
        event.getByLabel('leptonicStopTriggerSel',hltH)
        hlt = hltH.product()
        #the CMSSW skim path
        event.getByLabel('pathTriggerSel',pathselectH)
        pathtrigger = pathselectH.product()[0]
        
        event.getByLabel('leptonicStopPFJetsLeptonVeto',jetH)
        jets = jetH.product()
        
        event.getByLabel('leptonicStopElectronLoose',looseElectronH)
        event.getByLabel('leptonicStopElectronTight',tightElectronH)
        looseElectrons = looseElectronH.product()
        tightElectrons = tightElectronH.product()
        
        event.getByLabel('leptonicStopMuonLoose',looseMuonH)
        event.getByLabel('leptonicStopMuonTight',tightMuonH)
        looseMuons = looseMuonH.product()
        tightMuons = tightMuonH.product()
        
        event.getByLabel('cmgPFMET',metH)
        met = metH.product()
        
        event.getByLabel('leptonicStopWENuSel',wENuH)
        wENu = wENuH.product()
        event.getByLabel('leptonicStopWMuNuSel',wMuNuH)
        wMuNu = wMuNuH.product()        
        
        looseBJets = [jets[i] for i in xrange(jets.size()) if jets[i].getSelection('cuts_btag_loose')]
        mediumBJets = [jets[i] for i in xrange(jets.size()) if jets[i].getSelection('cuts_btag_medium')]
                
        if pathtrigger.getSelection('leptonicStopPath') and pathtrigger.getSelection('leptonicStopTriggerPath'):

            pathSelectHistograms['NumberOfJets']['Selected'].Fill(jets.size(),eventWeight)
            pathSelectHistograms['NumberOfBJetsLoose']['Selected'].Fill(len(looseBJets),eventWeight)
            pathSelectHistograms['NumberOfBJetsMedium']['Selected'].Fill(len(mediumBJets),eventWeight)            
            pathSelectHistograms['NumberOfElectrons']['Selected'].Fill(looseElectrons.size(),eventWeight)
            pathSelectHistograms['NumberOfMuons']['Selected'].Fill(looseMuons.size(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['LeadingJetPt']['Selected'].Fill(jets[0].pt(),eventWeight)
            if jets.size() > 1: pathSelectHistograms['SecondJetPt']['Selected'].Fill(jets[1].pt(),eventWeight)
            if jets.size() > 2: pathSelectHistograms['ThirdJetPt']['Selected'].Fill(jets[2].pt(),eventWeight)
            if jets.size() > 3: pathSelectHistograms['FourthJetPt']['Selected'].Fill(jets[3].pt(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['LeadingJetEta']['Selected'].Fill(jets[0].eta(),eventWeight)
            if jets.size() > 1: pathSelectHistograms['SecondJetEta']['Selected'].Fill(jets[1].eta(),eventWeight)
            if jets.size() > 2: pathSelectHistograms['ThirdJetEta']['Selected'].Fill(jets[2].eta(),eventWeight)
            if jets.size() > 3: pathSelectHistograms['FourthJetEta']['Selected'].Fill(jets[3].eta(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['TCHELeadingJet']['Selected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 1: pathSelectHistograms['TCHESecondJet']['Selected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 2: pathSelectHistograms['TCHEThirdJet']['Selected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 3: pathSelectHistograms['TCHEFourthJet']['Selected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            
            pathSelectHistograms['PFMET']['Selected'].Fill(met[0].et(),eventWeight)
            if wENu.size() > 0: pathSelectHistograms['WENuMT']['Selected'].Fill(wENu[0].mT(),eventWeight)
            if wMuNu.size() > 0: pathSelectHistograms['WMuNuMT']['Selected'].Fill(wMuNu[0].mT(),eventWeight)
            
            if len(looseBJets) > 0: pathSelectHistograms['LeadingBJetPtLoose']['Selected'].Fill(looseBJets[0].pt(),eventWeight)
            if len(mediumBJets) > 0: pathSelectHistograms['LeadingBJetPtMedium']['Selected'].Fill(mediumBJets[0].pt(),eventWeight)
            if len(looseBJets) > 1: pathSelectHistograms['SecondBJetPtLoose']['Selected'].Fill(looseBJets[1].pt(),eventWeight)
            if len(mediumBJets) > 1: pathSelectHistograms['SecondBJetPtMedium']['Selected'].Fill(mediumBJets[1].pt(),eventWeight)            
            
        elif pathtrigger.getSelection('leptonicStopPath'):
            
            pathSelectHistograms['NumberOfJets']['OfflineSelected'].Fill(jets.size(),eventWeight)
            pathSelectHistograms['NumberOfBJetsLoose']['OfflineSelected'].Fill(len(looseBJets),eventWeight)
            pathSelectHistograms['NumberOfBJetsMedium']['OfflineSelected'].Fill(len(mediumBJets),eventWeight)             
            pathSelectHistograms['NumberOfElectrons']['OfflineSelected'].Fill(looseElectrons.size(),eventWeight)
            pathSelectHistograms['NumberOfMuons']['OfflineSelected'].Fill(looseMuons.size(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['LeadingJetPt']['OfflineSelected'].Fill(jets[0].pt(),eventWeight)
            if jets.size() > 1: pathSelectHistograms['SecondJetPt']['OfflineSelected'].Fill(jets[1].pt(),eventWeight)
            if jets.size() > 2: pathSelectHistograms['ThirdJetPt']['OfflineSelected'].Fill(jets[2].pt(),eventWeight)
            if jets.size() > 3: pathSelectHistograms['FourthJetPt']['OfflineSelected'].Fill(jets[3].pt(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['LeadingJetEta']['OfflineSelected'].Fill(jets[0].eta(),eventWeight)
            if jets.size() > 1: pathSelectHistograms['SecondJetEta']['OfflineSelected'].Fill(jets[1].eta(),eventWeight)
            if jets.size() > 2: pathSelectHistograms['ThirdJetEta']['OfflineSelected'].Fill(jets[2].eta(),eventWeight)
            if jets.size() > 3: pathSelectHistograms['FourthJetEta']['OfflineSelected'].Fill(jets[3].eta(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['TCHELeadingJet']['OfflineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 1: pathSelectHistograms['TCHESecondJet']['OfflineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 2: pathSelectHistograms['TCHEThirdJet']['OfflineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 3: pathSelectHistograms['TCHEFourthJet']['OfflineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            
            pathSelectHistograms['PFMET']['OfflineSelected'].Fill(met[0].et(),eventWeight)
            if wENu.size() > 0: pathSelectHistograms['WENuMT']['OfflineSelected'].Fill(wENu[0].mT(),eventWeight)
            if wMuNu.size() > 0: pathSelectHistograms['WMuNuMT']['OfflineSelected'].Fill(wMuNu[0].mT(),eventWeight)
            
            if len(looseBJets) > 0: pathSelectHistograms['LeadingBJetPtLoose']['OfflineSelected'].Fill(looseBJets[0].pt(),eventWeight)
            if len(mediumBJets) > 0: pathSelectHistograms['LeadingBJetPtMedium']['OfflineSelected'].Fill(mediumBJets[0].pt(),eventWeight)
            if len(looseBJets) > 1: pathSelectHistograms['SecondBJetPtLoose']['OfflineSelected'].Fill(looseBJets[1].pt(),eventWeight)
            if len(mediumBJets) > 1: pathSelectHistograms['SecondBJetPtMedium']['OfflineSelected'].Fill(mediumBJets[1].pt(),eventWeight)              
            
        elif pathtrigger.getSelection('leptonicStopTriggerPath'):
            
            pathSelectHistograms['NumberOfJets']['OnlineSelected'].Fill(jets.size(),eventWeight)
            pathSelectHistograms['NumberOfBJetsLoose']['OnlineSelected'].Fill(len(looseBJets),eventWeight)
            pathSelectHistograms['NumberOfBJetsMedium']['OnlineSelected'].Fill(len(mediumBJets),eventWeight)             
            pathSelectHistograms['NumberOfElectrons']['OnlineSelected'].Fill(looseElectrons.size(),eventWeight)
            pathSelectHistograms['NumberOfMuons']['OnlineSelected'].Fill(looseMuons.size(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['LeadingJetPt']['OnlineSelected'].Fill(jets[0].pt(),eventWeight)
            if jets.size() > 1: pathSelectHistograms['SecondJetPt']['OnlineSelected'].Fill(jets[1].pt(),eventWeight)
            if jets.size() > 2: pathSelectHistograms['ThirdJetPt']['OnlineSelected'].Fill(jets[2].pt(),eventWeight)
            if jets.size() > 3: pathSelectHistograms['FourthJetPt']['OnlineSelected'].Fill(jets[3].pt(),eventWeight)

            if jets.size() > 0: pathSelectHistograms['LeadingJetEta']['OnlineSelected'].Fill(jets[0].eta(),eventWeight)
            if jets.size() > 1: pathSelectHistograms['SecondJetEta']['OnlineSelected'].Fill(jets[1].eta(),eventWeight)
            if jets.size() > 2: pathSelectHistograms['ThirdJetEta']['OnlineSelected'].Fill(jets[2].eta(),eventWeight)
            if jets.size() > 3: pathSelectHistograms['FourthJetEta']['OnlineSelected'].Fill(jets[3].eta(),eventWeight)
            
            if jets.size() > 0: pathSelectHistograms['TCHELeadingJet']['OnlineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 1: pathSelectHistograms['TCHESecondJet']['OnlineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 2: pathSelectHistograms['TCHEThirdJet']['OnlineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            if jets.size() > 3: pathSelectHistograms['TCHEFourthJet']['OnlineSelected'].Fill(jets[0].btag("trackCountingHighEffBJetTags"),eventWeight)
            
            pathSelectHistograms['PFMET']['OnlineSelected'].Fill(met[0].et(),eventWeight)
            if wENu.size() > 0: pathSelectHistograms['WENuMT']['OnlineSelected'].Fill(wENu[0].mT(),eventWeight)
            if wMuNu.size() > 0: pathSelectHistograms['WMuNuMT']['OnlineSelected'].Fill(wMuNu[0].mT(),eventWeight)
            
            if len(looseBJets) > 0: pathSelectHistograms['LeadingBJetPtLoose']['OnlineSelected'].Fill(looseBJets[0].pt(),eventWeight)
            if len(mediumBJets) > 0: pathSelectHistograms['LeadingBJetPtMedium']['OnlineSelected'].Fill(mediumBJets[0].pt(),eventWeight)
            if len(looseBJets) > 1: pathSelectHistograms['SecondBJetPtLoose']['OnlineSelected'].Fill(looseBJets[1].pt(),eventWeight)
            if len(mediumBJets) > 1: pathSelectHistograms['SecondBJetPtMedium']['OnlineSelected'].Fill(mediumBJets[1].pt(),eventWeight)             

        #from now on only look at fully selected events
        if not pathtrigger.getSelection('leptonicStopPath') or not pathtrigger.getSelection('leptonicStopTriggerPath'):
            continue
        
        paths = hlt[0].getSelectionNames()
        for path in RootTools.stliter(paths):
            if 'Jet' in path and ('Central' in path or 'Quad' in path) and hlt[0].getSelection(path):
                if pathtrigger.getSelection('leptonicStopPath'):
                    hltFired.FillTrigger(path)
        
        #look only at offline selected events        
        if pathtrigger.getSelection('leptonicStopPath'):
            if jets.size() > 0: triggerEff.Fill(pathtrigger, 'LeadingJetPt', jets[0].pt(),eventWeight)
        
        
#        #start by testing that the Razor path fired this event
#        event.getByLabel(('TriggerResults','','SUSY'),pathTriggerH)
#        
#        #fill the handles
#        pathTrigger = pathTriggerH.product()
        
#        #start by vetoing events that didn't pass the Razor path
#        pathTriggerNames = event._event.triggerNames(pathTrigger)
#        razorPath = pathTrigger.wasrun(pathTriggerNames.triggerIndex('razorPath')) and pathTrigger.accept(pathTriggerNames.triggerIndex('razorPath'))
#        if not razorPath: continue
        

    store.add(hltFired.buildHistogram())
    triggerEff.Scale()
    store.write()
