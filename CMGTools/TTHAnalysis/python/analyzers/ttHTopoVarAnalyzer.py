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
        
    def makeMT(self, event):
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

    def makeMT2(self, event):
#        print '==> INSIDE THE PRINT MT2'
#        print 'MET=',event.met.pt() 

        import array
        import numpy

## ===> hadronic MT2 (as used in the SUS-13-019)

        objects40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]

        if len(objects40jc)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()
            
            for jet in objects40jc:
                pxvec.push_back(jet.px())
                pyvec.push_back(jet.py())
                pzvec.push_back(jet.pz())
                Evec.push_back(jet.energy())

#### get hemispheres (seed 2: max inv mass, association method: default 3 = minimal lund distance)
            hemisphere = Hemisphere(pxvec, pyvec, pzvec, Evec, 2, 3)
            grouping=hemisphere.getGrouping()
##            print 'grouping ',len(grouping)

            pseudoJet1px = 0 
            pseudoJet1py = 0 
            pseudoJet1pz = 0
            pseudoJet1energy = 0 

            pseudoJet2px = 0 
            pseudoJet2py = 0 
            pseudoJet2pz = 0
            pseudoJet2energy = 0 
                
            for index in range(0, len(pxvec)):
                if(grouping[index]==1):
                    pseudoJet1px += pxvec[index]
                    pseudoJet1py += pyvec[index]
                    pseudoJet1pz += pzvec[index]
                    pseudoJet1energy += Evec[index]
                if(grouping[index]==2):
                    pseudoJet2px += pxvec[index]
                    pseudoJet2py += pyvec[index]
                    pseudoJet2pz += pzvec[index]
                    pseudoJet2energy += Evec[index]                    

            event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
            metVector =numpy.asarray(metVector,dtype='double')
            jetVector1 = TVectorD(3,array.array('d',[0.,event.pseudoJet1.px(), event.pseudoJet1.py()]))
            jetVector1 =numpy.asarray(jetVector1,dtype='double')
            jetVector2 = TVectorD(3,array.array('d',[0.,event.pseudoJet2.px(), event.pseudoJet2.py()]))        
            jetVector2 =numpy.asarray(jetVector2,dtype='double')
            
            davismt2.set_momenta(jetVector1,jetVector2,metVector);
            davismt2.set_mn(0);
            
            event.mt2 = davismt2.get_mt2()  
            
## ===> leptonic MT2 (as used in the SUS-13-025 )

        if len(event.selectedLeptons)>=2:

            metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
            visaVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[0].px(), event.selectedLeptons[0].py()]))
            visbVector = TVectorD(3,array.array('d',[0.,event.selectedLeptons[1].px(), event.selectedLeptons[1].py()]))
            
            metVector =numpy.asarray(metVector,dtype='double')
            visaVector =numpy.asarray(visaVector,dtype='double')
            visbVector =numpy.asarray(visbVector,dtype='double')
            
            davismt2.set_momenta(visaVector,visbVector,metVector);
            davismt2.set_mn(0);
            
            event.mt2lep = davismt2.get_mt2()  


## ===> hadronic MT2w (as used in the SUS-13-011) below just a placeHolder to be coded properly

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
        event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( -999, -999, -999, -999 )
        event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( -999, -999, -999, -999 )

        self.makeMT(event)
        self.makeMT2(event)

#        print 'variables computed: MT=',event.mtw,'MT2=',event.mt2,'MT2W=',event.mt2w
#        print 'pseudoJet1 px=',event.pseudoJet1.px(),' py=',event.pseudoJet1.py(),' pz=',event.pseudoJet1.pz()
#        print 'pseudoJet2 px=',event.pseudoJet2.px(),' py=',event.pseudoJet2.py(),' pz=',event.pseudoJet2.pz()   

        return True
