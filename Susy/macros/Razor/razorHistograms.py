#!/usr/bin/env python

import ROOT as rt
import sys
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile

if __name__ == '__main__':

    # https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#VarParsing_Example
    from FWCore.ParameterSet.VarParsing import VarParsing
    options = VarParsing ('python')
    options.outputFile = 'razorHistograms.root'
    options.parseArguments()

    # use Varparsing object
    events = Events(options)
    
    #make some handles
    electronH = Handle("std::vector<cmg::Electron>")
    hemiHadH = Handle("std::vector<cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> >")
    metH = Handle("std::vector<cmg::BaseMET>")
    muonH = Handle("std::vector<cmg::Muon>")
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")
    
    hMass = rt.TH1F('hMass','Mass',400,0,2000)
    hMet = rt.TH1F('hMet','MET',850,150,1000)
    hMR = rt.TH1F('hMR','MR',200,0,2000)
    hRsq = rt.TH1F('hRsq','R^{2}',150,0,1.5)
    hMRRsq = rt.TH2F('hMRRsq','MR vs R^{2}',200,0,2000,150,0,1.5)
    hMRMet = rt.TH2F('hMRMet','MR vs Met',200,0,2000,850,150,1000)
    hMassMet = rt.TH2F('hMassMet','Mass vs Met',200,0,2000,850,150,1000)
    hRsqMet = rt.TH2F('hRsqMet','R^{2} vs Met',150,0,1.5,850,150,1000)
    hMassRsq = rt.TH2F('hMassRsq','Mass vs R^{2}',200,0,2000,150,0,1.5)
    
    store = RootFile.RootFile(options.outputFile)
    store.add(hMet)
    store.add(hMR)
    store.add(hRsq)
    store.add(hMassRsq)
    store.add(hMRRsq)
    store.add(hMRMet)
    store.add(hMassMet)
    store.add(hRsqMet)
    store.add(hMass)

    # loop over events
    for event in events:
        
        #start by testing that the Razor path fired this event
        event.getByLabel(('TriggerResults','','SUSY'),pathTriggerH)
        
        #fill the handles
        event.getByLabel(('susyElectron'), electronH)
        event.getByLabel(('razorDiHemiHadBox'), hemiHadH)
        event.getByLabel(('cmgPFMET'), metH)
        event.getByLabel(('susyMuon'), muonH)

        electrons = electronH.product()        
        hemiHad = hemiHadH.product()
        met = metH.product()
        muons = muonH.product()
        pathTrigger = pathTriggerH.product()
        
        #start by vetoing events that didn't pass the Razor path
        pathTriggerNames = event._event.triggerNames(pathTrigger)
        razorPath = pathTrigger.wasrun(pathTriggerNames.triggerIndex('razorPath')) and pathTrigger.accept(pathTriggerNames.triggerIndex('razorPath'))
        if not razorPath: continue
        
        hMet.Fill(met[0].et())
        hMR.Fill(hemiHad[0].mR())
        hRsq.Fill(hemiHad[0].R()**2)
        hMassRsq.Fill(hemiHad[0].mass(),hemiHad[0].R()**2)
        hMRRsq.Fill(hemiHad[0].mR(),hemiHad[0].R()**2)
        hMRMet.Fill(hemiHad[0].mR(),met[0].et())
        hMassMet.Fill(hemiHad[0].mass(),met[0].et())
        hRsqMet.Fill(hemiHad[0].R()**2,met[0].et())
        hMass.Fill(hemiHad[0].mass())
        
    store.write()
