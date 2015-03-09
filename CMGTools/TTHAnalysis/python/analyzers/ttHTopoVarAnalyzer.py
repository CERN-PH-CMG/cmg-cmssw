import operator 
import itertools
import copy
from math import *

from ROOT import std 
from ROOT import TLorentzVector, TVectorD

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

from PhysicsTools.HeppyCore.utils.deltar import deltaR

from ROOT.heppy import Hemisphere
from ROOT.heppy import ReclusterJets

from ROOT.heppy import Davismt2
davismt2 = Davismt2()

from ROOT.heppy import mt2w_bisect 
mt2wSNT = mt2w_bisect.mt2w()

import ROOT

import os

def mtw(x1,x2):
    return sqrt(2*x1.pt()*x2.pt()*(1-cos(x1.phi()-x2.phi())))

class ttHTopoVarAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHTopoVarAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName) 

    def declareHandles(self):
        super(ttHTopoVarAnalyzer, self).declareHandles()
       #genJets                                                                                                                                                                     
        self.handles['genJets'] = AutoHandle( 'slimmedGenJets','std::vector<reco::GenJet>')

    def beginLoop(self, setup):
        super(ttHTopoVarAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')

    def makeMinMT(self,event):

        objectsb40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 and j.btagWP("CSVv2IVFM")]

        if len(objectsb40jc)>0:
            for bjet in objectsb40jc:
                mtTemp = mtw(bjet, event.met)
                event.minMTBMet = min(event.minMTBMet,mtTemp)

    def makeMinMTGamma(self,event):

        gamma_objectsb40jc = [ j for j in event.gamma_cleanJets if j.pt() > 40 and abs(j.eta())<2.5 and j.btagWP("CSVv2IVFM")]

        if len(gamma_objectsb40jc)>0:
            for bjet in gamma_objectsb40jc:
                mtTemp = mtw(bjet, event.gamma_met)
                event.gamma_minMTBMet = min(event.gamma_minMTBMet,mtTemp)

    def computeMT2(self, visaVec, visbVec, metVec):
        
        import array
        import numpy

        metVector = array.array('d',[0.,metVec.px(), metVec.py()])
        visaVector = array.array('d',[0.,visaVec.px(), visaVec.py()])
        visbVector = array.array('d',[0.,visbVec.px(), visbVec.py()])

        davismt2.set_momenta(visaVector,visbVector,metVector);
        davismt2.set_mn(0);

        return davismt2.get_mt2()


    def makeMT2(self, event):
#        print '==> INSIDE THE PRINT MT2'
#        print 'MET=',event.met.pt() 

        import array
        import numpy


## ===> hadronic MT2 (as used in the SUS-13-019) 

        objects40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]


#### get hemispheres via AntiKT -1 antikt, 1 kt, 0 CA                                                                                                                                    
        if len(objects40jc)>=2:

            objects  = ROOT.std.vector(ROOT.reco.Particle.LorentzVector)()
            for jet in objects40jc:
                objects.push_back(jet.p4())

            hemisphereViaKt = ReclusterJets(objects, 1.,50.0)
            groupingViaKt=hemisphereViaKt.getGroupingExclusive(2)

            if len(groupingViaKt)>=2:
                event.pseudoViaKtJet1_had = ROOT.reco.Particle.LorentzVector(groupingViaKt[0])
                event.pseudoViaKtJet2_had = ROOT.reco.Particle.LorentzVector(groupingViaKt[1])
                event.mt2ViaKt_had = self.computeMT2(event.pseudoViaKtJet1_had, event.pseudoViaKtJet2_had, event.met)

            if not self.cfg_ana.doOnlyDefault:
                hemisphereViaAKt = ReclusterJets(objects, -1.,50.0)
                groupingViaAKt=hemisphereViaAKt.getGroupingExclusive(2)

                if len(groupingViaAKt)>=2:
                    event.pseudoViaKtJet1_had = ROOT.reco.Particle.LorentzVector(groupingViaAKt[0])
                    event.pseudoViaKtJet2_had = ROOT.reco.Particle.LorentzVector(groupingViaAKt[1])
                    event.mt2ViaAKt_had = self.computeMT2(event.pseudoViaAKtJet1_had, event.pseudoViaAKtJet2_had, event.met)

#### get hemispheres (seed 2: max inv mass, association method: default 3 = minimal lund distance)

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

            hemisphere = Hemisphere(pxvec, pyvec, pzvec, Evec, 2, 3)
            grouping=hemisphere.getGrouping()
##            print 'grouping ',len(grouping)

            pseudoJet1px = 0 
            pseudoJet1py = 0 
            pseudoJet1pz = 0
            pseudoJet1energy = 0 
            multPSJ1 = 0

            pseudoJet2px = 0 
            pseudoJet2py = 0 
            pseudoJet2pz = 0
            pseudoJet2energy = 0 
            multPSJ2 = 0
                
            for index in range(0, len(pxvec)):
                if(grouping[index]==1):
                    pseudoJet1px += pxvec[index]
                    pseudoJet1py += pyvec[index]
                    pseudoJet1pz += pzvec[index]
                    pseudoJet1energy += Evec[index]
                    multPSJ1 += 1
                if(grouping[index]==2):
                    pseudoJet2px += pxvec[index]
                    pseudoJet2py += pyvec[index]
                    pseudoJet2pz += pzvec[index]
                    pseudoJet2energy += Evec[index]                    
                    multPSJ2 += 1

            pseudoJet1pt2 = pseudoJet1px*pseudoJet1px + pseudoJet1py*pseudoJet1py
            pseudoJet2pt2 = pseudoJet2px*pseudoJet2px + pseudoJet2py*pseudoJet2py

            if pseudoJet1pt2 >= pseudoJet2pt2:
                event.pseudoJet1_had = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.pseudoJet2_had = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)
                event.multPseudoJet1_had = multPSJ1
                event.multPseudoJet2_had = multPSJ2
            else:
                event.pseudoJet2_had = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.pseudoJet1_had = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)
                event.multPseudoJet1_had = multPSJ2
                event.multPseudoJet2_had = multPSJ1

            event.mt2_had = self.computeMT2(event.pseudoJet1_had, event.pseudoJet2_had, event.met)

#### do same things for GEN

        allGenJets = [ x for x in self.handles['genJets'].product() ] 
        objects40jc_Gen = [ j for j in allGenJets if j.pt() > 40 and abs(j.eta())<2.5 ]

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

            
## ===> full MT2 (jets + leptons)                                                                                                                                                                                             
        objects10lc = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta())<2.5 ]
        if hasattr(event, 'selectedIsoCleanTrack'):
            objects10lc = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta())<2.5 ] + [ t for t in event.selectedIsoCleanTrack ]

        objects40j10lc = objects40jc + objects10lc

        objects40j10lc.sort(key = lambda obj : obj.pt(), reverse = True)

        if len(objects40j10lc)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()

            for obj in objects40j10lc:
                pxvec.push_back(obj.px())
                pyvec.push_back(obj.py())
                pzvec.push_back(obj.pz())
                Evec.push_back(obj.energy())

            #for obj in objects_fullmt2:
            #    print "pt: ", obj.pt(), ", eta: ", obj.eta(), ", phi: ", obj.phi(), ", mass: ", obj.mass()

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

            pseudoJet1pt2 = pseudoJet1px*pseudoJet1px + pseudoJet1py*pseudoJet1py
            pseudoJet2pt2 = pseudoJet2px*pseudoJet2px + pseudoJet2py*pseudoJet2py

            if pseudoJet1pt2 >= pseudoJet2pt2:
                event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)
            else:
                event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            ###

            event.mt2 = self.computeMT2(event.pseudoJet1, event.pseudoJet2, event.met)


## ===> full gamma_MT2

        gamma_objects40jc = [ j for j in event.gamma_cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]

        gamma_objects40j10lc = gamma_objects40jc + objects10lc
    
        gamma_objects40j10lc.sort(key = lambda obj : obj.pt(), reverse = True)

##        if len(gamma_objects40j10lc)>=2:
        if len(gamma_objects40jc)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()

##            for obj in objects40j10lc:
            for obj in gamma_objects40jc:
                pxvec.push_back(obj.px())
                pyvec.push_back(obj.py())
                pzvec.push_back(obj.pz())
                Evec.push_back(obj.energy())

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

            event.gamma_pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.gamma_pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.gamma_mt2 = self.computeMT2(event.gamma_pseudoJet1, event.gamma_pseudoJet2, event.gamma_met)

## ===> zll_MT2
        
        csLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
           
        if len(csLeptons)==2 and len(objects40jc)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()

            for obj in objects40jc:
                pxvec.push_back(obj.px())
                pyvec.push_back(obj.py())
                pzvec.push_back(obj.pz())
                Evec.push_back(obj.energy())

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

            zll_pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            zll_pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            event.zll_mt2 = self.computeMT2(zll_pseudoJet1, zll_pseudoJet2, event.zll_met)


#### do the mt2 with one or two b jets (medium CSV)                                                                                                                                                                                                         
        if len(event.bjetsMedium)>=2:

           event.mt2bb = self.computeMT2(event.bjetsMedium[0], event.bjetsMedium[1], event.met)
#            print 'MT2bb(2b)',event.mt2bb                                                                                                                                                                                                                 
        if len(event.bjetsMedium)==1:

            objects40jcCSV = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 and j.p4()!=event.bjetsMedium[0].p4() ]
            objects40jcCSV.sort(key = lambda l : l.btag('combinedInclusiveSecondaryVertexV2BJetTags'), reverse = True)

            if len(objects40jcCSV)>0:
                event.mt2bb = self.computeMT2(event.bjetsMedium[0], objects40jcCSV[0], event.met)
##                print 'MT2bb(1b)',event.mt2bb                                                                                                                                                                                                             

## ===> leptonic MT2 (as used in the SUS-13-025 )                                                                                                                                                                                                           
        if not self.cfg_ana.doOnlyDefault:
            if len(event.selectedLeptons)>=2:
                event.mt2lep = self.computeMT2(event.selectedLeptons[0], event.selectedLeptons[1], event.met)

## ===> hadronic MT2w (as used in the SUS-13-011) below just a placeHolder to be coded properly

        if not self.cfg_ana.doOnlyDefault:
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



###

    def process(self, event):
        self.readCollections( event.input )

        event.mt2_gen=-999
        event.mt2bb=-999
        event.mt2lept=-999        
        event.mt2w=-999

        event.mt2_had=-999
        event.pseudoJet1_had = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoJet2_had = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.multPseudoJet1_had=0
        event.multPseudoJet2_had=0
        
        event.mt2=-999
        event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.gamma_mt2=-999
        event.gamma_pseudoJet1  = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.gamma_pseudoJet2  = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.zll_mt2=-999

        event.mt2ViaKt_had=-999
        event.mt2ViaAKt_had=-999
        event.pseudoViaKtJet1_had = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoViaKtJet2_had = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoViaAKtJet1_had = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoViaAKtJet2_had = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        ###

        self.makeMT2(event)

        event.minMTBMet=999999
        self.makeMinMT(event)

        event.gamma_minMTBMet=999999
        self.makeMinMTGamma(event)

        event.zll_minMTBMet=999999
        csLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
        if len(csLeptons)==2:
            event.zll_minMTBMet=event.minMTBMet
#        print 'variables computed: MT=',event.mtw,'MT2=',event.mt2,'MT2W=',event.mt2w
#        print 'pseudoJet1 px=',event.pseudoJet1.px(),' py=',event.pseudoJet1.py(),' pz=',event.pseudoJet1.pz()
#        print 'pseudoJet2 px=',event.pseudoJet2.px(),' py=',event.pseudoJet2.py(),' pz=',event.pseudoJet2.pz()   

        return True
