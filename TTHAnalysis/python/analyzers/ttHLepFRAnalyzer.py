import operator 
import itertools
import copy
from math import *

from ROOT import TLorentzVector, TVectorD

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.Photon import Photon
from CMGTools.RootTools.physicsobjects.Electron import Electron
from CMGTools.RootTools.physicsobjects.Muon import Muon
from CMGTools.RootTools.physicsobjects.Jet import Jet

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi,bestMatch
from CMGTools.TTHAnalysis.leptonMVA import LeptonMVA

from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.TTHAnalysis.analyzers.ntuple import *
import os

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )

class ttHLepFRAnalyzer( TreeAnalyzerNumpy ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepFRAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 
        self.leptonMVA = LeptonMVA("%s/src/CMGTools/TTHAnalysis/data/leptonMVA/%%s_BDTG.weights.xml" % os.environ['CMSSW_BASE'], self.cfg_comp.isMC)

    def declareHandles(self):
        super(ttHLepFRAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'cmgPFMET', 'std::vector<cmg::BaseMET>' )
        self.handles['nopumet'] = AutoHandle( 'nopuMet', 'std::vector<reco::PFMET>' )

    def declareVariables(self):
        tr = self.tree

        isMC = self.cfg_comp.isMC 

        tr = self.tree
        var( tr, 'run', int)
        var( tr, 'lumi', int)
        var( tr, 'evt', int)
        var( tr, 'nVert', int)
        
        bookLepton(tr,"Probe", isMC)

        var( tr, 'tagType', int )

        bookLepton(tr,"TagLepton", isMC)
        bookJet(tr,"TagJet", isMC)
        #bookParticle(tr,"TagPhoton", isMC)

        var(tr,"Trig_Probe_1Mu", int)
        var(tr,"Trig_Probe_1ElT", int)
        var(tr,"Trig_Probe_1ElL", int)
        var(tr,"Trig_Tag_1Mu", int)
        var(tr,"Trig_Pair_2Mu", int)
        var(tr,"Trig_Pair_MuEG", int)

        var(tr, 'dphi_tp')
        var(tr, 'dr_tp')
        var(tr, 'mll')
        var(tr, 'mtw_probe')
        var(tr, 'mtw_tag')
        var(tr, 'mtwNoPU_probe')
        var(tr, 'mtwNoPU_tag')
        var(tr, 'met')
        var(tr, 'metPhi')
        var(tr, 'metNoPU')
        var(tr, 'metPhiNoPU')

    def beginLoop(self):
        super(ttHLepFRAnalyzer,self).beginLoop()

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.met = self.handles['met'].product()[0]
        event.metNoPU = self.handles['nopumet'].product()[0]

        tr = self.tree
        fill( tr, 'run', event.run) 
        fill( tr, 'lumi',event.lumi)
        fill( tr, 'evt', event.eventId)    
        fill( tr, 'nVert', len(event.goodVertices) )

        fill(tr, 'met', event.met.pt())
        fill(tr, 'metPhi', event.met.phi())
        fill(tr, 'metNoPU', event.metNoPU.pt())
        fill(tr, 'metPhiNoPU', event.metNoPU.phi())

        # di-lepton veto: simple, for now
        if len(event.selectedLeptons) > 1: 
            return False

        def mtw(x1,x2):
            return sqrt(2*x1.pt()*x2.pt()*(1-cos(x1.phi()-x2.phi())))

        def passTrigg(lep,triggers):
            for t in triggers:
                if lep.sourcePtr().triggerObjectMatchesByPath('HLT_%s_v*' % t).size() > 0: return True
            return False

        def passSingleMu(lep):
            return passTrigg(lep, "Mu12 Mu24 Mu24_eta2p1 Mu30 Mu30_eta2p1 Mu40 Mu40_eta2p1 Mu17 Mu8".split())

        def passSingleIsoMu(lep):
            return passTrigg(lep, "IsoMu24 IsoMu24_eta2p1".split())

        def passSingleElL(lep):
            return passTrigg(lep, "Ele17_CaloIdL_CaloIsoVL Ele8_CaloIdL_CaloIsoVL".split())

        def passSingleElT(lep):
            return passTrigg(lep, "Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL".split())

        def passDoubleMu(lep1,lep2):
            return passTrigg(lep1, "Mu17_Mu8 Mu17_TkMu8".split()) and passTrigg(lep2, "Mu17_Mu8 Mu17_TkMu8".split())

        def passMuEG(mu,el):
            if (mu.sourcePtr().triggerObjectMatchesByFilter("hltL1Mu12EG7L3MuFiltered17").size() > 0 and
                el.sourcePtr().triggerObjectMatchesByPath("hltMu17Ele8CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter").size()):
                    return True
            if (mu.sourcePtr().triggerObjectMatchesByFilter("hltL1sL1Mu3p5EG12ORL1MuOpenEG12L3Filtered8").size() > 0 and
                el.sourcePtr().triggerObjectMatchesByPath("hltMu8Ele17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLTrackIsoFilter").size()):
                    return True
            return False
        
        # now fill probes
        for lep in event.selectedLeptons: 
            self.leptonMVA.addMVA(lep)
            fillLepton(tr, "Probe", lep)
            fill(tr, 'mtw_probe', mtw(lep, event.met))
            fill(tr, 'mtwNoPU_probe', mtw(lep, event.metNoPU))
            fill(tr, 'Trig_Probe_1Mu', passSingleMu(lep))
            fill(tr, 'Trig_Probe_1ElL', passSingleElL(lep))
            fill(tr, 'Trig_Probe_1ElT', passSingleElT(lep))
            # first search for a jet tag
            fill(tr,'tagType',1) # jet
            for jet in event.cleanJets:
                dr   = deltaR(jet.eta(),jet.phi(),lep.eta(),lep.phi())
                dphi = deltaPhi(jet.phi(),lep.phi())
                if (dr < 0.5): continue
                fillJet(tr, "TagJet", jet)
                fill(tr, 'dr_tp',   dr)
                fill(tr, 'dphi_tp', dphi)
                tr.tree.Fill()
            # then search for a lepton tag
            fill(tr,'tagType',13) # muon
            for tag in event.looseLeptons:
                if abs(tag.pdgId()) != 13: continue
                if tag.pt() < 10: continue
                if tag.sourcePtr().userFloat("isPFMuon") < 0.5: continue
                dr   = deltaR(tag.eta(),tag.phi(),lep.eta(),lep.phi())
                dphi = deltaPhi(tag.phi(),lep.phi())
                if (dr < 0.5): continue
                fillLepton(tr, "TagLepton", tag)
                fill(tr, 'mtw_tag', mtw(tag, event.met))
                fill(tr, 'mtwNoPU_tag', mtw(tag, event.metNoPU))
                fill(tr, 'dr_tp',   dr)
                fill(tr, 'dphi_tp', dphi)
                fill(tr, 'mll', (tag.p4()+lep.p4()).M())
                fill(tr, 'Trig_Tag_1Mu', passSingleMu(tag) or passSingleIsoMu(tag))
                fill(tr, 'Trig_Pair_2Mu',  abs(lep.pdgId()) == 13 and passDoubleMu(tag,lep))
                fill(tr, 'Trig_Pair_MuEG', abs(lep.pdgId()) == 11 and passMuEG(tag,lep))
                tr.tree.Fill()
 
        return True
