#!/usr/bin/env python

import ROOT as rt
import os, sys, math, glob
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile
import math

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

def deltaPhi(phi1, phi2):
    M_PI = rt.TMath.Pi()
    result = phi1 - phi2
    while (result > M_PI):
        result -= 2*M_PI
    while (result <= -M_PI):
        result += 2*M_PI
    return result

def transverseMass(l1, l2):
    return math.sqrt( math.pow(l1.pt()+l2.pt(), 2) - 
                    math.pow(l1.px()+l2.px(), 2) - 
                    math.pow(l1.py()+l2.py(), 2) )

if __name__ == '__main__':

    runOnMC = True

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
    
    options.parseArguments()
    if options.inputDirectory is not None:
        listDirectory(options.inputDirectory, options.inputFiles, options.maxFiles)
        
    if True:
        names = [f for f in options.datasetName.split('/') if f]
        name = '%s-%s-%s-%d-AllSMS.root' % (names[0],names[1],names[-1],options.index)
        options.outputFile = os.path.join(options.outputDirectory,name)
        
    files = getFiles(
                      [options.datasetName],
                      'wreece',
                      'susy_tree_CMG_[0-9]+.root'

                     )

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
    elif options.maxFiles > 0:
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
    Double_t mStop;\
    Double_t mLSP;\
    Double_t leadingLeptonPt;\
    Double_t leadingLeptonEta;\
    Double_t leadingLeptonIso;\
    Double_t minDeltaPhi;\
    Double_t transverseMass;\
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
    Int_t nUniqueLeptons;\
    Int_t nVertex;\
    Int_t hemisphereBalance;\
    Int_t genInfo;\
};""")
    

    rt.gROOT.ProcessLine("""
struct Filters{\
    Bool_t triggerFilter;\
    Bool_t quadTriggerFilter;\
    Bool_t sixTriggerFilter;\
    Bool_t eightTriggerFilter;\
    Bool_t l1MultiJetFilter;\
    Bool_t ht250TriggerFilter;\
    Bool_t ht300TriggerFilter;\
    Bool_t razorTriggerFilter;\
    Bool_t eleHadTriggerFilter;\
    Bool_t muHadTriggerFilter;\
};""")

    from ROOT import Variables, Info, Filters

    output = rt.TFile.Open(options.outputFile,"recreate",'SMS',9)
    output.cd()
    tree = rt.TTree('RMRTree','Multijet events')
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
    lheH = Handle('LHEEventProduct')

    electronH = Handle("std::vector<cmg::Electron>")
    muonH = Handle("std::vector<cmg::Muon>")
    tauH = Handle("std::vector<cmg::Tau>")
    
    triggerH = Handle('std::vector<cmg::TriggerObject>')
    countH = Handle('int')
    filterH = Handle('int')
    
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")

    count = 0

    # loop over events
    for event in events:

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()
        
        if runOnMC:
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

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
        count += 1    

        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.Rsq = hemi.Rsq()
            vars.mR = hemi.mR()
            vars.hemi1Mass = hemi.leg1().mass()
            vars.hemi2Mass = hemi.leg2().mass()
            info.hemisphereBalance = (10*hemi.leg1().numConstituents()) + hemi.leg2().numConstituents()

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
        filters.ht250TriggerFilter = hlt.getSelectionRegExp("^HLT_HT250_v[0-9]+$")
        filters.ht300TriggerFilter = hlt.getSelectionRegExp("^HLT_HT300_v[0-9]+$")
        filters.razorTriggerFilter = hlt.getSelectionRegExp("^HLT_R0[0-9]+_MR[0-9]+_v[0-9]+$")
        filters.muHadTriggerFilter = hlt.getSelectionRegExp("^HLT_.*Mu[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") or hlt.getSelectionRegExp("^HLT_.*Mu[0-9]+_TriCentral.*Jet[0-9]+_v[0-9]+$")
        filters.eleHadTriggerFilter = hlt.getSelectionRegExp("^HLT_Ele[0-9]+.*Quad.*Jet[0-9]+.*_v[0-9]+$") or hlt.getSelectionRegExp("^HLT_Ele[0-9]+.*TriCentral.*Jet[0-9]+_v[0-9]+$") or\
            hlt.getSelectionRegExp("^HLT_Ele[0-9]+.*CentralTriJet[0-9]+_v[0-9]+$")
        filters.triggerFilter = filters.quadTriggerFilter or filters.sixTriggerFilter or filters.eightTriggerFilter

        event.getByLabel(('cmgPFMET'),metH)
        met = metH.product()[0]
        vars.met = met.et()

        vars.minDeltaPhi = 1e6
        for i in xrange(len(jets)):
            
            name = 'jet%iPt' % (i + 1)
            jet = jets[i]
            
            dp = deltaPhi(met.phi(),jet.phi())
            vars.minDeltaPhi = min(vars.minDeltaPhi,dp)

            if hasattr(vars,name):
                
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

        

        #study the leading lepton
        info.nUniqueLeptons = 0
        if (info.nElectronTight + info.nMuonTight + info.nTauTight) > 0:
            leptons = [(l.pt(),l) for l in electronH.product()]
            leptons.extend([(l.pt(),l) for l in muonH.product()])
            if info.nTauTight > 0:
                for t in tauH.product():
                    can = (t.pt(),t)
                    if can not in leptons:
                        leptons.append(can)
            leptons.sort()
            info.nUniqueLeptons = len(leptons)

            leading = leptons[-1][1]
            vars.leadingLeptonPt = leading.pt()
            vars.leadingLeptonEta = leading.pt()
            vars.leadingLeptonIso = leading.relIso()
            vars.transverseMass = transverseMass(leading, met)

        event.getByLabel(('vertexSize'),countH)
        info.nVertex = countH.product()[0]
        
        if runOnMC:
            event.getByLabel(('simpleGenInfo'),filterH)
            if filterH.isValid():
                info.genInfo = filterH.product()[0]

        tree.Fill()
        #if count > 2000: break

    output.cd()    
    tree.Write()
    output.Close()

