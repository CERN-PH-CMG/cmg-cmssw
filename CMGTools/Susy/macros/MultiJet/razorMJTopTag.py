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

def massDrop(hemi):
    subjets = hemi.sourcePtrs()
    
    csv = sorted([(j.btag(6),j) for j in subjets], reverse=True)
    return csv[0][1].mass()/hemi.mass()

def unboost(jet, prest):
    #transform jet into lab frame
    #See http://fastjet.fr/repo/doxygen-3.0.2/PseudoJet_8cc_source.html

    if (prest.px() == 0.0 and prest.py() == 0.0 and prest.pz() == 0.0):
        return jet
            
    m_local = prest.M()
    pf4  = ( -jet.px()*prest.px() - jet.py()*prest.py()
              - jet.pz()*prest.pz() + jet.E()*prest.E() )/m_local
    fn   = (pf4 + jet.E()) / (prest.E() + m_local)
    _px = jet.px() - fn*prest.px()
    _py =  jet.py() - fn*prest.py()
    _pz =  jet.pz() - fn*prest.pz()
    _E = pf4;

    jet.SetPx(_px)
    jet.SetPy(_py)
    jet.SetPz(_pz)
    jet.SetE(_E)
            
    return jet

def calCHelicity(t, wj1, wj2):
    #see http://fastjet.fr/repo/doxygen-3.0.2/TopTaggerBase_8cc_source.html

    #w and t
    w = wj1 + wj2
    top = t

    #softer subjet
    w2 = wj1
    if wj2.pt() < wj1.pt():
        w2 = wj2
            
    W2 = unboost(w2, w)
    top = unboost(top, w)

    def modp2(j):
        return j.px()**2+j.py()**2+j.pz()**2 
            
    return (W2.px()*top.px() + W2.py()*top.py() + W2.pz()*top.pz())/math.sqrt(modp2(W2) * modp2(top))


def topTag(hemi):
    #looks at the various combinations to find the best W candidate

    mw = 80.385

    #sort the jets into one btag jet and the rest as light jets
    subjets = hemi.sourcePtrs()
    csv = sorted([(j.btag(6),j) for j in subjets], reverse=True)
    tag = csv[0][1]
    light = [j[1] for j in csv[1:]]

    if len(light) < 1: return (hemi.numConstituents(),-1,-1,-1)

    if len(light) == 1:
        w = light[0].p4()
        t = w + tag.p4()
        #boost b and W back to top rest frame
        b = unboost(tag.p4(), light[0].p4()+tag.p4())
        wrf = unboost(light[0].p4(), light[0].p4()+tag.p4())
        thetaH = math.cos(wrf.eta() - b.eta())
    else:
        w_cands = []
        for j1 in light:
            for j2 in light:
                if j1 == j2: continue
                #search for the best W candidate
                w = j1.p4() + j2.p4()
                w_cands.append( ( abs(w.mass()-mw)/abs( (1.0/j2.uncOnFourVectorScale()) + (1.0/j2.uncOnFourVectorScale()) ), j1, j2) )

        w_cands.sort()
        w = w_cands[0][1].p4() + w_cands[0][2].p4()
        thetaH = calCHelicity(w + tag.p4(), w_cands[0][1].p4(), w_cands[0][2].p4() )
        
    t = w + tag.p4()
    return (hemi.numConstituents(),t.mass(), w.mass(), thetaH)


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
    
    options.parseArguments()
    if not options.inputFiles:
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
    Double_t hemi1TopMass;\
    Double_t hemi1WMass;\
    Double_t hemi1ThetaH;\
    Double_t hemi2TopMass;\
    Double_t hemi2WMass;\
    Double_t hemi2ThetaH;\
};""")
    
    rt.gROOT.ProcessLine("""
struct Info{\
    Int_t event;\
    Int_t run;\
    Int_t lumi;\
    Int_t nJet;\
    Int_t nJetCleaned;\
    Int_t nBJet;\
    Int_t nBJetLoose;\
    Int_t nMuonLoose;\
    Int_t nMuonTight;\
    Int_t nElectronLoose;\
    Int_t nElectronTight;\
    Int_t nTauVeto;\
    Int_t nTauLoose;\
    Int_t nTauTight;\
    Int_t nLepton;\
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
    Bool_t hadBoxFilter;\
    Bool_t eleBoxFilter;\
    Bool_t muBoxFilter;\
    Bool_t tauBoxFilter;\
    Bool_t eleTriggerFilter;\
    Bool_t hadTriggerFilter;\
    Bool_t muTriggerFilter;\
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
    store.add(tree)

    count = 0

    # loop over events
    for event in events:

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
            tree.AutoSave()
        count += 1    

        event.getByLabel(('TriggerResults','','MJSkim'),pathTriggerH)
        pathTrigger = pathTriggerH.product()
        pathTriggerNames = event.object().triggerNames(pathTrigger)

        filters.hadBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceHadPath'))
        filters.eleBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceElePath'))
        filters.muBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceMuPath'))
        filters.tauBoxFilter = pathTrigger.accept(pathTriggerNames.triggerIndex('razorMJSkimSequenceTauPath'))        

        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if not hemiHadH.isValid(): continue
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.Rsq = hemi.Rsq()
            vars.mR = hemi.mR()
            vars.hemi1Mass = hemi.leg1().mass()
            vars.hemi2Mass = hemi.leg2().mass()
            info.hemisphereBalance = (10*hemi.leg1().numConstituents()) + hemi.leg2().numConstituents()

            #if vars.Rsq < 0.03 or vars.mR < 500:
            #    continue

        event.getByLabel(('razorMJPFJetSel30'),jetSel30H)
        if not jetSel30H.isValid(): continue
        jets = jetSel30H.product()
        info.nJet = len(jets)
        #if info.nJet < 6:
        #    continue

        event.getByLabel(('razorMJHadTriggerSel'),triggerH)
        filters.hadTrigger = len(triggerH.product()) > 0
        
        event.getByLabel(('razorMJEleTriggerSel'),triggerH)
        filters.eleTrigger = len(triggerH.product()) > 0

        event.getByLabel(('razorMJMuTriggerSel'),triggerH)
        filters.muTrigger = len(triggerH.product()) > 0

        if not (filters.hadBoxFilter and filters.hadTrigger) and not (filters.eleBoxFilter and filters.eleTrigger)\
                and not (filters.muBoxFilter and filters.muTrigger) and not (filters.tauBoxFilter and filters.hadTrigger):
            continue

        if runOnMC:
            event.getByLabel(('simpleGenInfo'),filterH)
            if filterH.isValid():
                info.genInfo = filterH.product()[0]
                #if info.genInfo != 22000:
                #    continue

        for i in xrange(len(jets)):
            name = 'jet%iPt' % (i + 1)
            if hasattr(vars,name):
                jet = jets[i]
                setattr(vars,name,jet.pt())
                setattr(vars,'jet%iEta'%(i+1),jet.eta())

        def massDrop(hemi):
            subjets = hemi.sourcePtrs()

            csv = sorted([(j.btag(6),j) for j in subjets], reverse=True)
            return csv[0][1].mass()/hemi.mass()

        def unboost(jet, prest):
            #transform jet into lab frame
            #See http://fastjet.fr/repo/doxygen-3.0.2/PseudoJet_8cc_source.html

            if (prest.px() == 0.0 and prest.py() == 0.0 and prest.pz() == 0.0):
                return jet
            
            m_local = prest.M()
            pf4  = ( -jet.px()*prest.px() - jet.py()*prest.py()
                      - jet.pz()*prest.pz() + jet.E()*prest.E() )/m_local
            fn   = (pf4 + jet.E()) / (prest.E() + m_local)
            _px = jet.px() - fn*prest.px()
            _py =  jet.py() - fn*prest.py()
            _pz =  jet.pz() - fn*prest.pz()
            _E = pf4;

            jet.SetPx(_px)
            jet.SetPy(_py)
            jet.SetPz(_pz)
            jet.SetE(_E)
            
            return jet

        def calCHelicity(t, wj1, wj2):
            #see http://fastjet.fr/repo/doxygen-3.0.2/TopTaggerBase_8cc_source.html

            #w and t
            w = wj1 + wj2
            top = t

            #softer subjet
            w2 = wj1
            if wj2.pt() < wj1.pt():
                w2 = wj2
            
            W2 = unboost(w2, w)
            top = unboost(top, w)

            def modp2(j):
                return j.px()**2+j.py()**2+j.pz()**2 
            
            return (W2.px()*top.px() + W2.py()*top.py() + W2.pz()*top.pz())/math.sqrt(modp2(W2) * modp2(top))


        def topTag(hemi):
            #looks at the various combinations to find the best W candidate

            mw = 80.385

            #sort the jets into one btag jet and the rest as light jets
            subjets = hemi.sourcePtrs()
            csv = sorted([(j.btag(6),j) for j in subjets], reverse=True)
            tag = csv[0][1]
            light = [j[1] for j in csv[1:]]

            if len(light) < 1: return (hemi.numConstituents(),-1,-1,-1)

            if len(light) == 1:
                w = light[0].p4()
                t = w + tag.p4()
                #boost b and W back to top rest frame
                b = unboost(tag.p4(), light[0].p4()+tag.p4())
                wrf = unboost(light[0].p4(), light[0].p4()+tag.p4())
                thetaH = math.cos(wrf.eta() - b.eta())
            else:
                w_cands = []
                for j1 in light:
                    for j2 in light:
                        if j1 == j2: continue
                        #search for the best W candidate
                        w = j1.p4() + j2.p4()
                        w_cands.append( ( abs(w.mass()-mw)/abs( (1.0/j2.uncOnFourVectorScale()) + (1.0/j2.uncOnFourVectorScale()) ), j1, j2) )

                w_cands.sort()
                w = w_cands[0][1].p4() + w_cands[0][2].p4()
                thetaH = calCHelicity(w + tag.p4(), w_cands[0][1].p4(), w_cands[0][2].p4() )

            t = w + tag.p4()
            return (hemi.numConstituents(),t.mass(), w.mass(), thetaH)


        #csv = sorted([(j.btag(6),j) for j in jets if abs(j.eta()) <= 2.4 and j.pt() >= 30.])
        #md = max( massDrop( hemi.leg1() ), massDrop( hemi.leg2() ) )
        #print md
        _, vars.hemi1TopMass, vars.hemi1WMass, vars.hemi1ThetaH = topTag( hemi.leg1() )
        _, vars.hemi2TopMass, vars.hemi2WMass, vars.hemi2ThetaH = topTag( hemi.leg2() )

        #event.getByLabel(('cmgPFMET'),metH)
        #met = metH.product()[0]
        #vars.met = met.et()

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
        #
        event.getByLabel(('razorMJTauVeto'),jetSel30H)
        info.nTauVeto = len(jetSel30H.product())

        #event.getByLabel(('vertexSize'),countH)
        #info.nVertex = countH.product()[0]
        
        #razorMJJetCleanedLoose
        event.getByLabel(('razorMJJetCleanedLoose'),jetSel30H)
        if not jetSel30H.isValid(): continue
        jets = jetSel30H.product()
        info.nJetCleaned = len(jets)


        tree.Fill()

#    tree.SetDirectory(output)
#    tree.Write()
#    output.Close()
    store.write()
