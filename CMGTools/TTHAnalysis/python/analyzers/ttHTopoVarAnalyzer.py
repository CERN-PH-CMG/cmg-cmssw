import operator 
import itertools
import copy
from math import *

from ROOT import TVectorD, TLorentzVector

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
from ROOT import HemisphereViaKt

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
#        printMT2 '==> INSIDE THE PRINT MT'
#        print 'MET=',event.met.pt() 

        for lepton in event.selectedLeptons:
            event.mtw = mtw(lepton, event.met)

        for myTau in event.selectedTaus:
            event.mtwTau = mtw(myTau, event.met)
            foundTau = True

        for myTrack in event.selectedIsoTrack:
            event.mtwIsoTrack = mtw(myTrack, event.met)

    def computeMT2(self, visaVec, visbVec, metVec):
		
        import array
        import numpy

        metVector = TVectorD(3,array.array('d',[0.,metVec.px(), metVec.py()]))
        visaVector = TVectorD(3,array.array('d',[0.,visaVec.px(), visaVec.py()]))
        visbVector = TVectorD(3,array.array('d',[0.,visbVec.px(), visbVec.py()]))
        
        metVector =numpy.asarray(metVector,dtype='double')
        visaVector =numpy.asarray(visaVector,dtype='double')
        visbVector =numpy.asarray(visbVector,dtype='double')
        
        davismt2.set_momenta(visaVector,visbVector,metVector);
        davismt2.set_mn(0);
        
        return davismt2.get_mt2()  
                    

    def makeMT2(self, event):
#        print '==> INSIDE THE PRINT MT2'
#        print 'MET=',event.met.pt() 


## ===> hadronic MT2 (as used in the SUS-13-019)

        objects40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]

        if len(objects40jc)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()
            groupingViaKt  = ROOT.std.vector(ROOT.std.vector(float))()
            
            for jet in objects40jc:
                pxvec.push_back(jet.px())
                pyvec.push_back(jet.py())
                pzvec.push_back(jet.pz())
                Evec.push_back(jet.energy())
                
#### get hemispheres via AntiKT -1 antikt, 1 kt, 0 CA
            hemisphereViaAKt = HemisphereViaKt(pxvec, pyvec, pzvec, Evec, -1.) 
            groupingViaAKt=hemisphereViaAKt.getGrouping()

            pseudoJet1px = 0 
            pseudoJet1py = 0 
            pseudoJet1pz = 0
            pseudoJet1energy = 0 

            pseudoJet2px = 0 
            pseudoJet2py = 0 
            pseudoJet2pz = 0
            pseudoJet2energy = 0 

            for index in range(0, len(groupingViaAKt[0])):
                if(index==0):
                    pseudoJet1px = groupingViaAKt[0][index]
                    pseudoJet1py = groupingViaAKt[1][index]
                    pseudoJet1pz = groupingViaAKt[2][index]
                    pseudoJet1energy = groupingViaAKt[3][index]
                if(index==1):
                    pseudoJet2px = groupingViaAKt[0][index]
                    pseudoJet2py = groupingViaAKt[1][index]
                    pseudoJet2pz = groupingViaAKt[2][index]
                    pseudoJet2energy = groupingViaAKt[3][index]                    

            event.pseudoViaAKtJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.pseudoViaAKtJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.mt2ViaAKt = self.computeMT2(event.pseudoViaAKtJet1, event.pseudoViaAKtJet2, event.met)

#### get hemispheres via AntiKT -1 antikt, 1 kt, 0 CA
            hemisphereViaKt = HemisphereViaKt(pxvec, pyvec, pzvec, Evec, 1.) 
            groupingViaKt=hemisphereViaKt.getGrouping()

            pseudoJet1px = 0 
            pseudoJet1py = 0 
            pseudoJet1pz = 0
            pseudoJet1energy = 0 

            pseudoJet2px = 0 
            pseudoJet2py = 0 
            pseudoJet2pz = 0
            pseudoJet2energy = 0 

            for index in range(0, len(groupingViaKt[0])):
                if(index==0):
                    pseudoJet1px = groupingViaKt[0][index]
                    pseudoJet1py = groupingViaKt[1][index]
                    pseudoJet1pz = groupingViaKt[2][index]
                    pseudoJet1energy = groupingViaKt[3][index]
                if(index==1):
                    pseudoJet2px = groupingViaKt[0][index]
                    pseudoJet2py = groupingViaKt[1][index]
                    pseudoJet2pz = groupingViaKt[2][index]
                    pseudoJet2energy = groupingViaKt[3][index]                    

            event.pseudoViaKtJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.pseudoViaKtJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.mt2ViaKt = self.computeMT2(event.pseudoViaKtJet1, event.pseudoViaKtJet2, event.met)            

#### get hemispheres (seed 2: max inv mass, association method: default 3 = minimal lund distance)
            hemisphere = Hemisphere(pxvec, pyvec, pzvec, Evec, 2, 3)
            grouping=hemisphere.getGrouping()

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
                    event.multPseudoJet1 += 1
                if(grouping[index]==2):
                    pseudoJet2px += pxvec[index]
                    pseudoJet2py += pyvec[index]
                    pseudoJet2pz += pzvec[index]
                    pseudoJet2energy += Evec[index]                    
                    event.multPseudoJet2 += 1

            event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.mt2 = self.computeMT2(event.pseudoJet1, event.pseudoJet2, event.met)


#### get hemispheres alternatice: 3: two objects who give maximal transverse mass 2: minimal mass squared sum of the hemispheres
            hemisphere32 = Hemisphere(pxvec, pyvec, pzvec, Evec, 3, 2)
            grouping32=hemisphere32.getGrouping()

            pseudoJet1px = 0 
            pseudoJet1py = 0 
            pseudoJet1pz = 0
            pseudoJet1energy = 0 

            pseudoJet2px = 0 
            pseudoJet2py = 0 
            pseudoJet2pz = 0
            pseudoJet2energy = 0 
            
            for index in range(0, len(pxvec)):
                if(grouping32[index]==1):
                    pseudoJet1px += pxvec[index]
                    pseudoJet1py += pyvec[index]
                    pseudoJet1pz += pzvec[index]
                    pseudoJet1energy += Evec[index]
                if(grouping32[index]==2):
                    pseudoJet2px += pxvec[index]
                    pseudoJet2py += pyvec[index]
                    pseudoJet2pz += pzvec[index]
                    pseudoJet2energy += Evec[index]                    

            event.pseudoJet1minmass = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.pseudoJet2minmass = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.mt2minmass = self.computeMT2(event.pseudoJet1minmass, event.pseudoJet2minmass, event.met)
            
#### do same things for GEN
            
##        allGenJets = map( Jet, event.genJetsself.handles['genJets'].product() )
        objects40jc_Gen = [ j for j in event.genJets if j.pt() > 40 and abs(j.eta())<2.5 ]
            
        if len(objects40jc_Gen)>=2:
                
            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()
            
            for jet in objects40jc_Gen:
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
                    
            pseudoGenJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            pseudoGenJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.mt2_gen = self.computeMT2(pseudoGenJet1, pseudoGenJet2, event.met.genMET())
            
            
#### do the mt2 with one or two b jets (medium CSV)

        if len(event.bjetsMedium)>=2:

           event.mt2bb = self.computeMT2(event.bjetsMedium[0], event.bjetsMedium[1], event.met)
#            print 'MT2bb(2b)',event.mt2bb
            
        if len(event.bjetsMedium)==1:

            objects40jcCSV = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 and j.p4()!=event.bjetsMedium[0].p4() ]
            objects40jcCSV.sort(key = lambda l : l.btag('combinedSecondaryVertexBJetTags'), reverse = True)

            if len(objects40jcCSV)>0:
#                for index in range(0, len(objects40jcCSV)):
#                    print 'CSV ',objects40jcCSV[index].btag('combinedSecondaryVertexBJetTags')

                event.mt2bb = self.computeMT2(event.bjetsMedium[0], objects40jcCSV[0], event.met)
##                print 'MT2bb(1b)',event.mt2bb
                
        
## ===> leptonic MT2 (as used in the SUS-13-025 )

        if len(event.selectedLeptons)>=2:
            event.mt2lep = self.computeMT2(event.selectedLeptons[0], event.selectedLeptons[1], event.met)            

## ===> hadronic MT2w (as used in the SUS-13-011) below just a placeHolder to be coded properly

        if len(event.selectedLeptons)>=1:

            import array
            import numpy
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

        event.mt2minmass=-999
        event.mt2_gen=-999
        event.mt2=-999
        event.mt2lept=-999
        event.mt2bb=-999        
        event.mt2w=-999
        event.multPseudoJet1=0
        event.multPseudoJet2=0
        event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.pseudoJet1minmass = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoJet2minmass = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.mt2ViaKt=-999
        event.mt2ViaAKt=-999
        event.pseudoViaKtJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoViaKtJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )                
        event.pseudoViaAKtJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoViaAKtJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        self.makeMT(event)
        self.makeMT2(event)

#        print 'variables computed: MT=',event.mtw,'MT2=',event.mt2,'MT2W=',event.mt2w
#        print 'mt2=',event.mt2, 'mt2(kt)',event.mt2ViaKt, 'mt2(akt)',event.mt2ViaAKt
#        print 'pseudoJet1 px=',event.pseudoJet1.px(),' py=',event.pseudoJet1.py(),' pz=',event.pseudoJet1.pz()
#        print 'pseudoJet2 px=',event.pseudoJet2.px(),' py=',event.pseudoJet2.py(),' pz=',event.pseudoJet2.pz()   

        return True
