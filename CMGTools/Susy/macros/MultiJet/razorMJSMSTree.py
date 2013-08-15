#!/usr/bin/env python

import ROOT as rt
import os, sys, math, glob, copy, pickle
from DataFormats.FWLite import Events, Handle
from CMGTools.RootTools import RootFile
from BTagSFUtil import BTag

import numpy as n
from ROOT import std

def decoratePoint(point):
    return '%i_%i' % point

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
    options.register ('model',
                  None,
                  VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.varType.string,          # string, int, or float
                  "The SMS model to use in the error calculation")
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
    if True:
        names = [f for f in options.datasetName.split('/') if f]
        if options.index == -1:
            name = '%s-%s-%s-BPOG.root' % (names[0],names[1],names[-1])
        else:
            name = '%s-%s-%s-BPOG_%d.root' % (names[0],names[1],names[-1],options.index)
        options.outputFile = os.path.join(options.outputDirectory,name)
        options.model = names[0].split('-')[1].split('_')[0]
    pickleFile = options.outputFile.replace('.root','.pkl')

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
    print options.inputFiles

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
    Double_t maxTCHE_FL;\
    Double_t maxTCHE_ETA;\
    Double_t nextTCHE;\
    Double_t nextTCHE_PT;\
    Double_t nextTCHE_FL;\
    Double_t nextTCHE_ETA;\
    Double_t mStop;\
    Double_t mLSP;\
    Double_t BTAG_W;\
    Double_t BTAG_W_BC_UP;\
    Double_t BTAG_W_BC_DW;\
    Double_t BTAG_W_LT_UP;\
    Double_t BTAG_W_LT_DW;\
    Double_t BTAG_W_DT_UP;\
    Double_t BTAG_W_DT_DW;\
    Double_t diTopPt;\
};""")

    rt.gROOT.ProcessLine("""
struct Info{\
    Int_t event;\
    Int_t run;\
    Int_t lumi;\
    Int_t nJet;\
    Int_t nBJet;\
    Int_t nBJetLoose;\
    Int_t nVertex;\
    Int_t genInfo;\
    Int_t BOX_NUM;\
    Int_t nLepton;\
};""")
    
    from ROOT import Variables, Info

    output = rt.TFile.Open(options.outputFile,"recreate",'SMS',9)
    output.cd()
    tree = rt.TTree('RMRTree','Multijet events')
    def setAddress(obj, flag):
        for branch in dir(obj):
            if branch.startswith('__'): continue
            if flag:
                tree.Branch(branch,rt.AddressOf(obj,branch),'%s/%s' % (branch,flag) )
            else:
                tree.Branch(branch,rt.AddressOf(obj,branch),branch)
    
    info = Info()
    vars = Variables()
    BTAG_TrackCount = std.vector('double')()    
    CTEQ66_W = std.vector('double')()
    MRST2006NNLO_W = std.vector('double')()
    
    setAddress(info,'I')
    setAddress(vars,'D')
    tree.Branch('BTAG_TrackCount',BTAG_TrackCount)
    tree.Branch('CTEQ66_W',CTEQ66_W)
    tree.Branch('MRST2006NNLO_W',MRST2006NNLO_W)

    # use Varparsing object
    events = Events(options)

    #make some handles
    jetSel30H = Handle("std::vector<cmg::PFJet>")
    hemiHadH = Handle("std::vector<cmg::DiObject<cmg::Hemisphere, cmg::Hemisphere> >")
    metH = Handle("std::vector<cmg::BaseMET>")
    lheH = Handle('LHEEventProduct')
    candH = Handle("std::vector<reco::LeafCandidate>")

    electronH = Handle("std::vector<cmg::Electron>")
    muonH = Handle("std::vector<cmg::Muon>")
    tauH = Handle("std::vector<cmg::Tau>")
    
    triggerH = Handle('std::vector<cmg::TriggerObject>')
    countH = Handle('int')
    filterH = Handle('int')
    pdfH = Handle('std::vector<double>')
    
    # path trigger
    pathTriggerH = Handle("edm::TriggerResults")

    count = 0

    #for storing the counts of each model point
    bins = {}

    #for doing all the crap with btags and scale factors
    tagger = BTag(options.model)

    # loop over events
    for event in events:

        CTEQ66_W.clear()
        MRST2006NNLO_W.clear()
        BTAG_TrackCount.clear()

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
        
        #store how many of each model we see
        point = (vars.mStop,vars.mLSP)
        if bins.has_key(point):
            bins[point] = bins[point] + 1
        else:
            bins[point] = 1

        ##selection part of code
        #CRHad=5; BJet=6; CRLep=7
        info.BOX_NUM = 0

        #this is the trigger hack, where I cut on the trigger objects
        event.getByLabel(('emulate2011Trigger'),filterH)
        triggerEMFilter = filterH.product()[0]
        if not triggerEMFilter: continue
              
        if not event.getByLabel(('razorMJPFJetSel30'),jetSel30H):
            continue
        jets = jetSel30H.product()
        info.nJet = len(jets)
        if info.nJet < 6: continue

        event.getByLabel(('razorMJDiHemiHadBox'),hemiHadH)
        if len(hemiHadH.product()):
            hemi = hemiHadH.product()[0]
            vars.RSQ = hemi.Rsq()
            vars.MR = hemi.mR()
            #filter out events failing the analysis cuts
            if vars.MR < 500 or vars.RSQ < 0.03: continue
        else:
            #skip events with no hemi
            continue

        #lepton ID
        event.getByLabel(('razorMJElectronTight'),electronH)
        event.getByLabel(('razorMJMuonTight'),muonH)
        event.getByLabel(('razorMJTauTight'),tauH)
        #
        nElectronTight = len(electronH.product())
        nMuonTight = len(muonH.product())
        nTauTight = len(tauH.product())

        #we don't make a requirement on the Taus at this point so we can study tau id
        #if (nElectronTight + nMuonTight) > 0: continue
        nLepton = nElectronTight + nMuonTight + nTauTight
        info.nLepton = nLepton

        #dump the B-tags
        tche = sorted([(j.btag(0),j) for j in jets if abs(j.eta()) <= 2.4 and j.btag(0) >= 3.3])
        if tche:
            vars.maxTCHE = tche[-1][1].btag(0)
            vars.maxTCHE_PT = tche[-1][1].pt()
            vars.maxTCHE_ETA = tche[-1][1].eta()
            vars.maxTCHE_FL = tche[-1][1].partonFlavour()

            if len(tche) > 1:
                vars.nextTCHE = tche[-2][1].btag(0)
                vars.nextTCHE_PT = tche[-2][1].pt()
                vars.nextTCHE_ETA = tche[-2][1].eta()
                vars.nextTCHE_FL = tche[-2][1].partonFlavour()

        info.nBJet = len(tche)
        if info.nBJet > 1:
            BTAG_TrackCount.push_back(tche[-1][1].btag(0))
            BTAG_TrackCount.push_back(tche[-2][1].btag(0))
        elif info.nBJet > 0:
            BTAG_TrackCount.push_back(tche[-1][1].btag(0))
            BTAG_TrackCount.push_back(-1)
        else:
            BTAG_TrackCount.push_back(-1)
            BTAG_TrackCount.push_back(-1)
        tcheLoose = sorted([(j.pt(),j) for j in jets if abs(j.eta()) <= 2.4 and j.btag(0) >= 1.7])
        info.nBJetLoose = len(tcheLoose)
        
        #events must either have a Btag or pass the tight veto
        if info.nBJet == 0 and info.nBJetLoose > 0: continue

        def findWeight(jets, mcdir = 0, doLight = False, datadir = 0):
            w_data = 1.
            w_mc = 1.
            for j in jets:
                if j.pt() < 30 or abs(j.eta()) > 2.4 or j.btag(0) < 0:
                    continue
                w_data_i, w_dataE_i = tagger.getEfficiencyData(j.btag(0), j.pt(), j.eta(), j.partonFlavour())
                w_mc_i, w_mcE_i = tagger.getEfficiencyFastSim(j.btag(0), j.pt(), j.eta(), j.partonFlavour())
                #use errors
                if mcdir:
                    #b,c are taken correlated and uncorrelated with the light jets
                    if (not doLight and abs(j.partonFlavour()) in [4,5]) or (doLight and not abs(j.partonFlavour()) in [4,5]):
                        w_mc_i = w_mc_i + (mcdir*w_mcE_i)
                        #project against bounds in eff
                        if w_mc_i > 1: w_mc_i = 1.0
                        if w_mc_i < 0: w_mc_i = 0.0
                        #if doLight: print w_data_i, w_dataE_i, w_mc_i, w_mcE_i
                if datadir:
                    #print 'datadir',w_data_i, w_dataE_i, w_mc_i, w_mcE_i, j.pt(), j.eta(), j.partonFlavour(), j.btag(0)
                    w_data_i = w_data_i + (datadir*w_dataE_i)
                    #project against bounds in eff
                    if w_data_i > 1: w_data_i = 1.0
                    if w_data_i < 0: w_data_i = 0.0

                w_data = w_data * (1 - w_data_i)
                w_mc = w_mc * (1 - w_mc_i)

            if w_data > 1: w_data = 1.
            if w_mc > 1: w_mc = 0.0
            
            try:
                tag_weight = (1-w_data)/(1-w_mc)
            except ZeroDivisionError:
                tag_weight = (1-w_data)
            return tag_weight

        jet_set = set()
        for j in jets:
            if j.pt() < 30 or abs(j.eta()) > 2.4 or j.btag(0) < 0:
                continue
            jet_set.add(j)

        jets = [j for j in jet_set]    
        nominal = findWeight(jets)
        bc_mc_up = abs(nominal - findWeight(jets,mcdir=+1))
        bc_mc_dw = abs(nominal - findWeight(jets,mcdir=-1))
        #print 'btag bc mc',nominal, bc_mc_up, bc_mc_dw

        lt_mc_up = abs(nominal - findWeight(jets,mcdir=+1,doLight=True))
        lt_mc_dw = abs(nominal - findWeight(jets,mcdir=-1,doLight=True))
        #print 'btag light mc',nominal, lt_mc_up, lt_mc_dw

        data_up = abs(nominal - findWeight(jets,datadir=+1))
        data_dw = abs(nominal - findWeight(jets,datadir=-1))
        #print 'btag data', nominal, data_up, data_dw
        #print 'final', info.nBJet, nominal, rt.TMath.Sqrt(bc_mc_up**2 + max(lt_mc_up,lt_mc_dw)**2 + data_dw**2 ),rt.TMath.Sqrt(bc_mc_dw**2 + max(lt_mc_up,lt_mc_dw)**2 + data_up**2 )

        #if max(rt.TMath.Sqrt(bc_mc_up**2 + max(lt_mc_up,lt_mc_dw)**2 + data_dw**2 ),rt.TMath.Sqrt(bc_mc_dw**2 + max(lt_mc_up,lt_mc_dw)**2 + data_up**2 ) ) > 0.5:
        #if nominal > 2:
        if False:
            print '\tbtag bc mc',nominal, bc_mc_up, bc_mc_dw
            print '\tbtag light mc',nominal, lt_mc_up, lt_mc_dw
            print '\tbtag data', nominal, data_up, data_dw
               
            
        #set all of the btagging vars
        vars.BTAG_W = nominal
        vars.BTAG_W_BC_UP = bc_mc_up
        vars.BTAG_W_BC_DW = bc_mc_dw
        vars.BTAG_W_LT_UP = lt_mc_up
        vars.BTAG_W_LT_DW = lt_mc_dw
        vars.BTAG_W_DT_UP = data_up
        vars.BTAG_W_DT_DW = data_dw

        #for the tau scaling
        event.getByLabel(('simpleGenInfo'),filterH)
        info.genInfo = filterH.product()[0]
        nGenTau = int(str(info.genInfo)[-1])

        if nLepton == 0 and info.nBJet > 0:
            info.BOX_NUM = 6 #BJet box
        elif nLepton == 0 and info.nBJet == 0 and info.nBJetLoose == 0:
            info.BOX_NUM = 5 #CRHAD
        elif nLepton == 1 and info.nBJet == 0 and info.nBJetLoose == 0:
            info.BOX_NUM = 7 #CRLEP

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

        info.event = event.object().id().event()
        info.lumi = event.object().id().luminosityBlock()
        info.run = event.object().id().run()
              
        #for PU weights
        event.getByLabel(('vertexSize'),countH)
        info.nVertex = countH.product()[0]
        
        #dump the PDF weights
        event.getByLabel(('dumpPdfWeights','cteq66'),pdfH)
        for w in pdfH.product():
            CTEQ66_W.push_back(w)
        event.getByLabel(('dumpPdfWeights','MRST2006nnlo'),pdfH)
        for w in pdfH.product():
            MRST2006NNLO_W.push_back(w)

        event.getByLabel(('topGenInfo'),candH)
        if len(candH.product()):
            if 'T2' in options.model:
                diTop = candH.product()[1]
            elif 'T1' in options.model:
                diTop = candH.product()[2]
            else:
                diTop = candH.product()[0]
            vars.diTopPt = diTop.pt()

        if (count % 1000) == 0:
            print count,'run/lumi/event',info.run,info.lumi,info.event
        count += 1
        tree.Fill()

        #if count > 2000: break
        
    output.cd()    
    tree.Write()
    output.Close()

    sample_counts = file(options.outputFile.replace('.root','.pkl'),'wb')
    print bins
    pickle.dump(bins,sample_counts)
    sample_counts.close()
