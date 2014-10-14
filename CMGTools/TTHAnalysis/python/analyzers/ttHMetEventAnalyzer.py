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
        
class ttHMetEventAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHMetEventAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHMetEventAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )
        self.handles['nopumet'] = AutoHandle( 'slimmedMETs', 'std::vector<pat::MET>' )
        self.handles['cmgCand'] = AutoHandle( self.cfg_ana.candidates, self.cfg_ana.candidatesTypes )
        self.handles['vertices'] =  AutoHandle( "offlineSlimmedPrimaryVertices", 'std::vector<reco::Vertex>', fallbackLabel="offlinePrimaryVertices" )

    def beginLoop(self):
        super(ttHMetEventAnalyzer,self).beginLoop()
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')

    def makeTkMETs(self, event):
        event.tkMet = 0  

        charged = []
        pfcands = self.handles['cmgCand'].product()

        for i in xrange(pfcands.size()):

## ===> require the Track Candidate charge and with a  minimum dz 
            
            if (pfcands.at(i).charge()!=0 and (abs(pfcands.at(i).dz())<=self.cfg_ana.dzMax )):
                
                charged.append(pfcands.at(i))
                
        import ROOT
        event.tkMet = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in charged])) , -1.*(sum([x.py() for x in charged])), 0, 0 )
##        print 'tkmet',event.tkMet.pt(),'tkmetphi',event.tkMet.phi()


    def makeMETs(self, event):
        event.met = self.handles['met'].product()[0]
        event.metNoPU = self.handles['nopumet'].product()[0]
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


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.counters.counter('events').inc('all events')

        self.makeMETs(event)

        if self.cfg_ana.doTkMet: 
            self.makeTkMETs(event);

        return True
