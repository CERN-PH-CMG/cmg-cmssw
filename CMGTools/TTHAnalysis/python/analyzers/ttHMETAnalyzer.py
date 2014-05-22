import operator 
import itertools
import copy
from math import *

from ROOT import TLorentzVector, TVectorD

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

from CMGTools.RootTools.utils.DeltaR import * 

import os


class ttHMETAnalyzer( Analyzer ):

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHMETAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)
        import ROOT
        
    def declareHandles(self):
        super(ttHMETAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'cmgPFMET', 'std::vector<cmg::BaseMET>' )
        self.handles['nopumet'] = AutoHandle( 'nopuMet', 'std::vector<reco::PFMET>' )

    def beginLoop(self):
        super(ttHMETAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')

    def makeMETs(self, event):
        #    event.met = self.handles['met'].product()[0]
        #    event.metNoPU = self.handles['nopumet'].product()[0]
        if hasattr(event, 'deltaMetFromJetSmearing'):
            import ROOT
            px,py = event.met.px()+event.deltaMetFromJetSmearing[0], event.met.py()+event.deltaMetFromJetSmearing[1]
            event.met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            px,py = event.metNoPU.px()+event.deltaMetFromJetSmearing[0], event.metNoPU.py()+event.deltaMetFromJetSmearing[1]
            event.metNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            if hasattr(event, 'deltaMetFromJEC') and event.deltaMetFromJEC[0] != 0 and event.deltaMetFromJEC[1] != 0:
                import ROOT
                px,py = event.met.px()+event.deltaMetFromJEC[0], event.met.py()+event.deltaMetFromJEC[1]
                event.met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
                px,py = event.metNoPU.px()+event.deltaMetFromJEC[0], event.metNoPU.py()+event.deltaMetFromJEC[1]
                event.metNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
                
                metMatrix = self.handles['metSignificance'].product().significance()
                metMatrix.Invert();
                import array
                metVector = TVectorD(2,array.array('d',[event.met.px(), event.met.py()]))
                event.metSignificance = metMatrix.Similarity(metVector)
                event.projMetAll1S  = self.jetProjectedMET(event.met, event.jets,True)
                event.projMetAll2S  = self.jetProjectedMET(event.met, event.jets,False)
                event.projMetJets1S = self.jetProjectedMET(event.met, event.cleanJets,True)
                event.projMetJets2S = self.jetProjectedMET(event.met, event.cleanJets,False)
               #print "MET value:  ", event.met.pt()
               #print "MET sumET:  ", event.met.sumEt()
               #print "MET signif: ", event.metSignificance
               #print "PrMETAll 1S:", event.projMetAll1S
               #print "PrMETAll 2S:", event.projMetAll2S
               #print "PrMETJet 1S:", event.projMetJets1S
               #print "PrMETJet 2S:", event.projMetJets2S                      
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.makeMETs(event);    

        return True
