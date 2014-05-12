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
from CMGTools.RootTools.physicsobjects.Tau import Tau
from CMGTools.RootTools.physicsobjects.Jet import Jet

from CMGTools.RootTools.utils.DeltaR import * 

from CMGTools.TTHAnalysis.analyzers.ntuple import *



from ROOT import Davismt2
davismt2 = Davismt2()

from ROOT import mt2w_bisect 
mt2wSNT = mt2w_bisect.mt2w()

import os

def mtw(x1,x2):
    return sqrt(2*x1.pt()*x2.pt()*(1-cos(x1.phi()-x2.phi())))

class ttHTopoVarAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHTopoVarAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 
        import ROOT

    def declareHandles(self):
        super(ttHTopoVarAnalyzer, self).declareHandles()
        self.handles['met'] = AutoHandle( 'cmgPFMET', 'std::vector<cmg::BaseMET>' )
        self.handles['nopumet'] = AutoHandle( 'nopuMet', 'std::vector<reco::PFMET>' )
#        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<cmg::Muon>")
#        self.handles['electrons'] = AutoHandle( self.cfg_ana.electrons,"std::vector<cmg::Electron>")
#        self.handles['taus'] = AutoHandle( self.cfg_ana.taus,'std::vector<cmg::Tau>')
        self.handles['jets'] = AutoHandle( self.cfg_ana.jetCol, 'std::vector<cmg::PFJet>' )

    def beginLoop(self):
        super(ttHTopoVarAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')
        
    def printMT(self, event):
#        print '==> INSIDE THE PRINT MT'
        event.met = self.handles['met'].product()[0]
        event.metNoPU = self.handles['nopumet'].product()[0]
#        print 'MET=',event.met.pt() 


        for lepton in event.selectedLeptons:
            event.mtw = mtw(lepton, event.met)

#        for myTau in self.handles['taus'].product():
        for myTau in event.selectedTaus:
            event.mtwTau = mtw(myTau, event.met)
            foundTau = True

#        for myTrack in event.selectedIsoTrack:
#            event.mtwIsoTrack = mtw(myTrack, event.met)

    def printMT2(self, event):
#        print '==> INSIDE THE PRINT MT2'
        event.met = self.handles['met'].product()[0]
        event.metNoPU = self.handles['nopumet'].product()[0]
#        print 'MET=',event.met.pt() 

        import array
        import numpy

        if len(event.selectedLeptons)>=2:

            metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
            visaVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[0].px(), event.selectedLeptons[0].py()]))
            visbVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[1].px(), event.selectedLeptons[1].py()]))
            
            metVector =numpy.asarray(metVector,dtype='double')
            visaVector =numpy.asarray(visaVector,dtype='double')
            visbVector =numpy.asarray(visbVector,dtype='double')
            
            davismt2.set_momenta(visaVector,visbVector,metVector);
            davismt2.set_mn(100);
            
            event.mt2 = davismt2.get_mt2()  

        if len(event.selectedLeptons)>=1:

            metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
            lVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[0].px(), event.selectedLeptons[0].py()]))
            #placeholder for visaVector and visbVector  need to get the jets
            visaVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[0].px(), event.selectedLeptons[0].py()]))
            visbVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[0].px(), event.selectedLeptons[0].py()]))
            
            metVector =numpy.asarray(metVector,dtype='double')
            lVector =numpy.asarray(lVector,dtype='double')
            visaVector =numpy.asarray(visaVector,dtype='double')
            visbVector =numpy.asarray(visbVector,dtype='double')

            mt2wSNT.set_momenta(lVector, visaVector,visbVector,metVector);
            event.mt2w = mt2wSNT.get_mt2w() 


    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.mtw=-999
        event.mtwTau=-999
        event.mt2=-999
        event.mt2w=-999

        self.printMT(event)
        self.printMT2(event)

        print 'variables computed: MT=',event.mtw,'MT2=',event.mt2,'MT2W=',event.mt2w


#        self.printMT2(event)
        return True
