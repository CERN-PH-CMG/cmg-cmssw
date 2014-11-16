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

from CMGTools.RootTools.utils.DeltaR import * 

import ROOT

import os
        
class ttHAlphaTMetAnalyzer( Analyzer ):
    """To construct MET for AlphaT mu control sample"""

    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHAlphaTMetAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHAlphaTMetAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )
        self.handles['nopumet'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )

    def beginLoop(self):
        super(ttHAlphaTMetAnalyzer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    def makeMETNoMu(self, event):
        event.metNoMu = copy.deepcopy(self.handles['met'].product()[0])
        event.metNoMuNoPU = copy.deepcopy(self.handles['nopumet'].product()[0])
        
        mupx = 0
        mupy = 0
        #sum muon momentum
        for mu in event.selectedMuons:
            mupx += mu.px()
            mupy += mu.py()

        #subtract muon momentum and construct met
        if hasattr(event, 'deltaMetFromJetSmearing'):
            import ROOT
            px,py = event.metNoMu.px()+event.deltaMetFromJetSmearing[0]-mupx, event.metNoMu.py()+event.deltaMetFromJetSmearing[1]-mupy
            event.metNoMu.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            px,py = event.metNoMuNoPU.px()+event.deltaMetFromJetSmearing[0]-mupx, event.metNoMuNoPU.py()+event.deltaMetFromJetSmearing[1]-mupy
            event.metNoMuNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
        if hasattr(event, 'deltaMetFromJEC') and event.deltaMetFromJEC[0] != 0 and event.deltaMetFromJEC[1] != 0:
            import ROOT
            px,py = event.metNoMu.px()+event.deltaMetFromJEC[0]-mupx, event.metNoMu.py()+event.deltaMetFromJEC[1]-mupy
            event.met.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))
            px,py = event.metNoMuNoPU.px()+event.deltaMetFromJEC[0]-mupx, event.metNoMuNoPU.py()+event.deltaMetFromJEC[1]-mupy
            event.metNoMuNoPU.setP4(ROOT.reco.Particle.LorentzVector(px,py, 0, hypot(px,py)))


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

        self.makeMETNoMu(event)

        return True
