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

import ROOT

from ROOT import Hemisphere

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

    def declareHandles(self):
        super(ttHTopoVarAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHTopoVarAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')
        
    def printMT(self, event):
#        print '==> INSIDE THE PRINT MT'
#        print 'MET=',event.met.pt() 

        for lepton in event.selectedLeptons:
            event.mtw = mtw(lepton, event.met)

#        for myTau in self.handles['taus'].product():
        for myTau in event.selectedTaus:
            event.mtwTau = mtw(myTau, event.met)
            foundTau = True

        for myTrack in event.selectedIsoTrack:
            event.mtwIsoTrack = mtw(myTrack, event.met)

    def printMT2(self, event):
#        print '==> INSIDE THE PRINT MT2'
#        print 'MET=',event.met.pt() 

        import array
        import numpy

## ===> hadronic MT2 (as used in the SUS-13-019) below place holder

        pxvec  = []
        pyvec  = []
        pzvec  = []
        Evec  = []
        
        for jet in event.cleanJetsAll:
            pxvec.append(jet.px())
            pyvec.append(jet.py())
            pzvec.append(jet.pz())
            Evec.append(jet.energy())

#        hemisphere = Hemisphere(pxvec, pyvec, pzvec, Evec, 2, 2)
#        grouping=hemisphere.getGrouping()
#        print 'grouping ',len(grouping)

#        event.pseudojet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
#        event.pseudojet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
        metVector =numpy.asarray(metVector,dtype='double')
        jetVector1 = TVectorD(3,array.array('d',[0.,event.pseudojet1.px(), event.pseudojet1.py()]))
        jetVector1 =numpy.asarray(jetVector1,dtype='double')
        jetVector2 = TVectorD(3,array.array('d',[0.,event.pseudojet2.px(), event.pseudojet2.py()]))        
        jetVector2 =numpy.asarray(jetVector2,dtype='double')
        
        davismt2.set_momenta(jetVector1,jetVector2,metVector);
        davismt2.set_mn(100);
        
        event.mt2 = davismt2.get_mt2()  

## ===> leptonic MT2 (as used in the SUS-13-025 ) below just a placeHolder

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


## ===> hadronic MT2w (as used in the SUS-13-011) below just a placeHolder
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
        event.mtwIsoTrack=-999
        event.mt2=-999

        event.mt2lept=-999        
        event.mt2w=-999
        event.pseudojet1 = ROOT.reco.Particle.LorentzVector( -999, -999, -999, -999 )
        event.pseudojet2 = ROOT.reco.Particle.LorentzVector( -999, -999, -999, -999 )

        self.printMT(event)
        self.printMT2(event)

#        print 'variables computed: MT=',event.mtw,'MT2=',event.mt2,'MT2W=',event.mt2w

#        self.printMT2(event)
        return True
