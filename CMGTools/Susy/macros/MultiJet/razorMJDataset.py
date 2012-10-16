#!/usr/bin/env python

import ROOT as rt
from ROOT import std
import os, sys, math, glob, pickle
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile
from razorMJTopTag import topTag

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

    skimEvents = True
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
    options.register ('index',
                  -1, # default value
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.int,          # string, int, or float
                  "The file index to run on")
    options.register ('model',
                  None,
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "The SMS model to use in the error calculation")

    
    options.parseArguments()
    if not options.inputFiles:
        if options.inputDirectory is not None:
            listDirectory(options.inputDirectory, options.inputFiles, options.maxFiles)
        
        if True:
            names = [f for f in options.datasetName.split('/') if f]
            if runOnMC:
                options.model = names[0].split('-')[1].split('_')[0]
            if options.index < 0:
                name = '%s-%s-%s.root' % (names[0],names[1],names[-1])
            else:
                name = '%s-%s-%s_%d.root' % (names[0],names[1],names[-1],options.index)
            options.outputFile = os.path.join(options.outputDirectory,name)
            print options.outputFile
        
        files = getFiles(
            [options.datasetName],
            'wreece',
            'susy_tree_CMG_[0-9]+.root'                
            )
        if options.maxFiles > 0:
            options.inputFiles = files[0:options.maxFiles]
        else:
            options.inputFiles = files
        print 'The number of files to run on is: %d' % len(files)

    if options.index > -1:
        chunks = []
        chunk = []
        for f in files:
            if len(chunk) <= options.maxFiles:
                chunk.append(f)
            if len(chunk) == options.maxFiles:
                chunks.append(chunk)
                chunk = []
        print 'Created %d chunks of length %s' % (len(chunks),options.maxFiles)
        options.inputFiles = chunks[options.index]
    
    pickleFile = options.outputFile.replace('.root','.pkl')

    rt.gROOT.ProcessLine("""
struct Variables{\
    Double_t MR;\
    Double_t MR_JES_UP;\
    Double_t MR_JES_DOWN;\
    Double_t RSQ;\
    Double_t RSQ_JES_UP;\
    Double_t RSQ_JES_DOWN;\
    Double_t hemi1Mass;\
    Double_t hemi2Mass;\
    Double_t hemi1TopMass;\
    Double_t hemi1WMass;\
    Double_t hemi1ThetaH;\
    Double_t hemi2TopMass;\
    Double_t hemi2WMass;\
    Double_t hemi2ThetaH;\
    Double_t mStop;\
    Double_t mLSP;\
    Double_t diTopPt;\
};""")
    
    rt.gROOT.ProcessLine("""
struct Info{\
    Int_t event;\
    Int_t run;\
    Int_t lumi;\
    Int_t nJet;\
    Int_t nJetNoLeptons;\
    Int_t nCSVL;\
    Int_t nCSVM;\
    Int_t nCSVT;\
    Int_t nMuonLoose;\
    Int_t nMuonTight;\
    Int_t nElectronLoose;\
    Int_t nElectronTight;\
    Int_t nTauVeto;\
    Int_t nTauLoose;\
    Int_t nTauTight;\
    Int_t nLepton;\
    Int_t hemisphereBalance;\
    Int_t hemi1Count;\
    Int_t hemi2Count;\
    Int_t genInfo;\
};""")
    
    rt.gROOT.ProcessLine("""
struct Filters{\
    Bool_t hadBoxFilter;\
    Bool_t eleBoxFilter;\
    Bool_t muBoxFilter;\
    Bool_t tauBoxFilter;\
    Bool_t eleTriggerFilter;\
    Bool_t hadTriggerFilter;\
    Bool_t muTriggerFilter;\
    Bool_t metFilter;\
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

    CTEQ66_W = std.vector('double')()
    MRST2006NNLO_W = std.vector('double')()
    tree.Branch('CTEQ66_W',CTEQ66_W)
    tree.Branch('MRST2006NNLO_W',MRST2006NNLO_W)

    jet_pt = std.vector('double')()
    tree.Branch('jet_pt',jet_pt)
    jet_eta = std.vector('double')()
    tree.Branch('jet_eta',jet_eta)
    jet_csv = std.vector('double')()
    tree.Branch('jet_csv',jet_csv)
    jet_fl = std.vector('double')()
    tree.Branch('jet_fl',jet_fl)

    # use Varparsing object
    events = Events(options)
    
    #make some handles
    jetSel30H = Handle("std::vector<cmg::PFJet>")
    hemiHadH = Handle("std::vector<cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> >")
    metH = Handle("std::vector<cmg::BaseMET>")
    lheH = Handle('LHEEventProduct')
    pdfH = Handle('std::vector<double>')
    candH = Handle("std::vector<reco::LeafCandidate>")

    electronH = Handle("std::vector<cmg::Electron>")
    muonH = Handle("std::vector<cmg::Muon>")
    tauH = Handle("std::vector<cmg::Tau>")
    
    triggerH = Handle('std::vector<cmg::TriggerObject>')
    countH = Handle('int')
    filterH = Handle('int')
    
    pathTriggerH = Handle("edm::TriggerResults")
    lheH = Handle('LHEEventProduct')

    store = RootFile.RootFile(options.outputFile)
    store.add(tree)

    count = 0
    #for storing the counts of each model point
    bins = {}

    # loop over events
    for event in events:

        CTEQ66_W.clear()
        MRST2006NNLO_W.clear()
        
        jet_pt.clear()
        jet_eta.clear()
        jet_csv.clear()
        jet_fl.clear()

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
            tree.AutoSave()
        count += 1    

        #get the LHE product info
        vars.mStop = -1
        vars.mLSP = -1
        if runOnMC:
            event.getByLabel(('source'),lheH)
            if lheH.isValid():
                lhe = lheH.product()
                for i in xrange(lhe.comments_size()):
                    comment = lhe.getComment(i)
                    if 'model' not in comment: continue
                    comment = comment.replace('\n','')
                    parameters = comment.split(' ')[-1]
                    masses = map(float,parameters.split('_')[-2:])
                    vars.mStop = masses[0]
                    vars.mLSP = masses[1]

        #store how many of each model we see
        point = (vars.mStop,vars.mLSP)
        if bins.has_key(point):
            bins[point] = bins[point] + 1
        else:
            bins[point] = 1

        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()
        pathTriggerNames = event.object().triggerNames(pathTrigger)

        #start by vetoing events that didn't pass the offline selection
        filters.hadBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceHadPath'))
        filters.eleBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceElePath'))
        filters.muBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceMuPath'))
        filters.tauBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceTauPath'))  
        path = filters.hadBoxFilter or filters.muBoxFilter or filters.eleBoxFilter or filters.tauBoxFilter
        if skimEvents and not path: continue

        #also get the MET filter
        event.getByLabel(('TriggerResults','','PAT'),pathTriggerH)
        pathTrigger = pathTriggerH.product()
        pathTriggerNames = event.object().triggerNames(pathTrigger)
        filters.metFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('metNoiseCleaningPath'))

        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if hemiHadH.isValid() and len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ = hemi.Rsq()
            vars.MR = hemi.mR()
            vars.hemi1Mass = hemi.leg1().mass()
            vars.hemi2Mass = hemi.leg2().mass()
            info.hemisphereBalance = (10*hemi.leg1().numConstituents()) + hemi.leg2().numConstituents()

            #run the hadronic top tagger
            info.hemi1Count, vars.hemi1TopMass, vars.hemi1WMass, vars.hemi1ThetaH = topTag( hemi.leg1() )
            info.hemi2Count, vars.hemi2TopMass, vars.hemi2WMass, vars.hemi2ThetaH = topTag( hemi.leg2() )
            
            #TODO: Place some cut here
            if skimEvents and vars.RSQ < 0.03:
                continue

            jets = [j for j in hemi.leg1().sourcePtrs()]
            jets.extend([j for j in hemi.leg2().sourcePtrs()])
            info.nJet = len(jets)

            for jet in jets:
                jet_pt.push_back(jet.pt())
                jet_eta.push_back(jet.eta())
                jet_csv.push_back(jet.btag(6))
                jet_fl.push_back(jet.partonFlavour())

            #store the number of btags at each working point
            csv = sorted([j.btag(6) for j in jets], reverse=True)
            info.nCSVL = len([c for c in csv if c >= 0.244])
            info.nCSVM = len([c for c in csv if c >= 0.679])
            info.nCSVT = len([c for c in csv if c >= 0.898])
            

        event.getByLabel(('razorMJDiHemiHadBoxUp'),hemiHadH)
        if hemiHadH.isValid() and len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ_JES_UP = hemi.Rsq()
            vars.MR_JES_UP = hemi.mR()

        event.getByLabel(('razorMJDiHemiHadBoxDown'),hemiHadH)
        if hemiHadH.isValid() and len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ_JES_DOWN = hemi.Rsq()
            vars.MR_JES_DOWN = hemi.mR()

        event.getByLabel(('razorMJHadTriggerSel'),triggerH)
        filters.hadTriggerFilter = len(triggerH.product()) > 0
        
        event.getByLabel(('razorMJEleTriggerSel'),triggerH)
        filters.eleTriggerFilter = len(triggerH.product()) > 0

        event.getByLabel(('razorMJMuTriggerSel'),triggerH)
        filters.muTriggerFilter = len(triggerH.product()) > 0
        
        #the number of lepton cleaned jets
        event.getByLabel(('razorMJJetCleanedLoose'),jetSel30H)
        info.nJetNoLeptons = len(jetSel30H.product())

        #event.getByLabel(('razorMJPFJetSel30'),jetSel30H)
        #if not jetSel30H.isValid(): continue
        #jets = jetSel30H.product()

        #tau veto
        #event.getByLabel(('razorMJTauVeto'),jetSel30H)
        #if jetSel30H.isValid():
        #    info.nTauVeto = len(jetSel30H.product())
        #else:
        #    info.nTauVeto = -1

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
        info.nLepton = info.nElectronTight + info.nMuonTight + info.nTauTight

        #event.getByLabel(('vertexSize'),countH)
        #info.nVertex = countH.product()[0]

##        #dump the PDF weights
##        Event.getByLabel(('dumpPdfWeights','cteq66'),pdfH)
##        if pdfH.isValid():
##            for w in pdfH.product():
##                CTEQ66_W.push_back(w)
##        event.getByLabel(('dumpPdfWeights','MRST2006nnlo'),pdfH)
##        if pdfH.isValid():
##            for w in pdfH.product():
##                MRST2006NNLO_W.push_back(w)
        
        if runOnMC:
            event.getByLabel(('topGenInfo'),candH)
            if options.model is not None and candH.isValid() and len(candH.product()):
                if 'T2' in options.model:
                    diTop = candH.product()[1]
                elif 'T1' in options.model:
                    diTop = candH.product()[2]
                else:
                    diTop = candH.product()[0]
                vars.diTopPt = diTop.pt()

        
            event.getByLabel(('simpleGenInfo'),filterH)
            if filterH.isValid():
                info.genInfo = filterH.product()[0]

        tree.Fill()

#    tree.SetDirectory(output)
#    tree.Write()
#    output.Close()
    store.write()

    sample_counts = file(options.outputFile.replace('.root','.pkl'),'wb')
    pickle.dump(bins,sample_counts)
    sample_counts.close()
    
