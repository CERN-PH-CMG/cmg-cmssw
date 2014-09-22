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
       #genJets                                                                                                                                                                     
        self.handles['genJets'] = AutoHandle( 'slimmedGenJets','std::vector<reco::GenJet>')

    def beginLoop(self):
        super(ttHTopoVarAnalyzer,self).beginLoop()
        self.counters.addCounter('pairs')
        count = self.counters.counter('pairs')
        count.register('all events')

#### MM
#    def makeGammaObjects(self, event):
#
#        import ROOT
#
#        ## with Central Jets                                                                                                                                                                                                                                       
#        gamma_objects25 = [ j for j in event.gamma_cleanJets if j.pt() > 25 ] + event.selectedLeptons
#        gamma_objects30 = [ j for j in event.gamma_cleanJets if j.pt() > 30 ] + event.selectedLeptons
#        gamma_objects40  = [ j for j in event.gamma_cleanJetsAll if j.pt() > 40 and abs(j.eta()) < 2.5 ] + [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
#        gamma_objects40j = [ j for j in event.gamma_cleanJets if j.pt() > 40 ]
#
#        event.gamma_htJet25 = sum([x.pt() for x in gamma_objects25])
#        event.gamma_mhtJet25vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects25])) , -1.*(sum([x.py() for x in gamma_objects25])), 0, 0 )
#        event.gamma_mhtPhiJet25 = event.mhtJet25vec.phi()
#        event.gamma_mhtJet25 = event.mhtJet25vec.pt()
#
#        event.gamma_htJet30 = sum([x.pt() for x in gamma_objects30])
#        event.gamma_mhtJet30vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects30])) , -1.*(sum([x.py() for x in gamma_objects30])), 0, 0 )
#        event.gamma_mhtJet30 = event.mhtJet30vec.pt()
#        event.gamma_mhtPhiJet30 = event.mhtJet30vec.phi()
#
#        event.gamma_htJet40 = sum([x.pt() for x in gamma_objects40])
#        event.gamma_mhtJet40vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects40])) , -1.*(sum([x.py() for x in gamma_objects40])), 0, 0 )
#        event.gamma_mhtJet40 = event.gamma_mhtJet40vec.pt()
#        event.gamma_mhtPhiJet40 = event.gamma_mhtJet40vec.phi()
#
#        event.gamma_htJet40j = sum([x.pt() for x in gamma_objects40j])
#        event.gamma_mhtJet40jvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects40j])) , -1.*(sum([x.py() for x in gamma_objects40j])), 0, 0 )
#        event.gamma_mhtJet40j = event.gamma_mhtJet40jvec.pt()
#        event.gamma_mhtPhiJet40j = event.gamma_mhtJet40jvec.phi()
#
#        # MET + photon
#        event.gamma_met = ROOT.reco.Particle.LorentzVector( event.met.px(), event.met.py(), 0, 0 )
#        event.gamma_metNoPU = ROOT.reco.Particle.LorentzVector( event.metNoPU.px(), event.metNoPU.py(), 0, 0 )
#        for gamma in event.loosePhotonsCentral:
#            event.gamma_met = ROOT.reco.Particle.LorentzVector( event.gamma_met.px() + gamma.px(), event.gamma_met.py() + gamma.py() , 0, 0 )
#            event.gamma_metNoPU = ROOT.reco.Particle.LorentzVector( event.gamma_metNoPU.px() + gamma.px(), event.gamma_metNoPU.py() + gamma.py() , 0, 0 )
#
#        # look for minimal deltaPhi between MET and four leading jets with pt>40 and eta<2.4
#        event.gamma_deltaPhiMin = 999.
#        for n,j in enumerate(gamma_objects40j):
#            if n>3:  break
#            thisDeltaPhi = abs( deltaPhi( j.phi(), event.gamma_met.phi() ) )
#            if thisDeltaPhi < event.gamma_deltaPhiMin : event.gamma_deltaPhiMin = thisDeltaPhi
#
#        # absolute value of the vectorial difference between met and mht 
#        gamma_diffMetMht_had_vec = ROOT.reco.Particle.LorentzVector(event.gamma_mhtJet40jvec.px()-event.gamma_met.px(), event.gamma_mhtJet40jvec.py()-event.gamma_met.py(), 0, 0 )
#        event.gamma_diffMetMht_had = sqrt( gamma_diffMetMht_had_vec.px()*gamma_diffMetMht_had_vec.px() + gamma_diffMetMht_had_vec.py()*gamma_diffMetMht_had_vec.py() )
#
#        gamma_diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.gamma_mhtJet40vec.px()-event.gamma_met.px(), event.gamma_mhtJet40vec.py()-event.gamma_met.py(), 0, 0 )
#        event.gamma_diffMetMht = sqrt( gamma_diffMetMht_vec.px()*gamma_diffMetMht_vec.px() + gamma_diffMetMht_vec.py()*gamma_diffMetMht_vec.py() )
#
#    def makeZllObjects(self, event):
#
#        import ROOT
#
#        vetoLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
#
#        # MET + zll
#        event.zll_deltaPhiMin = -999.
#        event.zll_met_pt = -999.
#        event.zll_met_phi = -999.
#        event.zll_diffMetMht = -999.
#        event.zll_mhtJet40j = -999.
#        event.zll_mhtPhiJet40j = -999.
#        event.zll_invmass = -999.
#
#        if len(vetoLeptons)==2:
#            event.zll_met = ROOT.reco.Particle.LorentzVector( event.met.px(), event.met.py(), 0, 0 )
#            for l in vetoLeptons:
#                event.zll_met = ROOT.reco.Particle.LorentzVector( event.zll_met.px() + l.px(), event.zll_met.py() + l.py() , 0, 0 )
#    
#            event.zll_met_pt = event.zll_met.pt()
#            event.zll_met_phi = event.zll_met.phi()
#
#            # defining mht as hadronic mht
#            event.zll_mhtJet40j = event.mhtJet40j
#            event.zll_mhtPhiJet40j = event.mhtPhiJet40j
#
#            # look for minimal deltaPhi between MET and four leading jets with pt>40 and |eta|<2.4
#            event.zll_deltaPhiMin = 999.
#            objects40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]
#            for n,j in enumerate(objects40jc):
#                if n>3:  break
#                thisDeltaPhi = abs( deltaPhi( j.phi(), event.zll_met.phi() ) )
#                if thisDeltaPhi < event.zll_deltaPhiMin : event.zll_deltaPhiMin = thisDeltaPhi
#
#            # absolute value of the vectorial difference between met and mht
#            zll_diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.mhtJet40jvec.px()-event.zll_met.px(), event.mhtJet40jvec.py()-event.zll_met.py(), 0, 0 )
#            event.zll_diffMetMht = sqrt( zll_diffMetMht_vec.px()*zll_diffMetMht_vec.px() + zll_diffMetMht_vec.py()*zll_diffMetMht_vec.py() )
#
#            # di-lepton invariant mass
#            zll_p4 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
#            for l in vetoLeptons:
#                zll_p4 += l.p4() 
#                
#            event.zll_invmass = zll_p4.M()
####

        
    def makeMT(self, event):
#        print '==> INSIDE THE PRINT MT'
#        print 'MET=',event.met.pt() 

        if len(event.selectedLeptons)>0:
            for lepton in event.selectedLeptons:
                event.mtw = mtw(lepton, event.met)

        if len(event.selectedTaus)>0:
            for myTau in event.selectedTaus:
                event.mtwTau = mtw(myTau, event.met)
                foundTau = True
                
        if len(event.selectedIsoTrack)>0:
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

            ### MM
            # Ordering pseudoJets in pT 
                    
            pseudoJet1pt2 = pseudoJet1px*pseudoJet1px + pseudoJet1py*pseudoJet1py
            pseudoJet2pt2 = pseudoJet2px*pseudoJet2px + pseudoJet2py*pseudoJet2py

            if pseudoJet1pt2 >= pseudoJet2pt2:
                event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)
            else:
                event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
            metVector =numpy.asarray(metVector,dtype='double')
            jetVector1 = TVectorD(3,array.array('d',[0.,event.pseudoJet1.px(), event.pseudoJet1.py()]))
            jetVector1 =numpy.asarray(jetVector1,dtype='double')
            jetVector2 = TVectorD(3,array.array('d',[0.,event.pseudoJet2.px(), event.pseudoJet2.py()]))        
            jetVector2 =numpy.asarray(jetVector2,dtype='double')
            
            davismt2.set_momenta(jetVector1,jetVector2,metVector);
            davismt2.set_mn(0);
            
            event.mt2 = davismt2.get_mt2()  

#### do same things for GEN

        allGenJets = map( Jet, self.handles['genJets'].product() ) 
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

            metGenVector = TVectorD(3,array.array('d',[0.,event.met.genMET().px(), event.met.genMET().py()]))
            metGenVector =numpy.asarray(metGenVector,dtype='double')
            jetGenVector1 = TVectorD(3,array.array('d',[0.,pseudoGenJet1.px(), pseudoGenJet1.py()]))
            jetGenVector1 =numpy.asarray(jetGenVector1,dtype='double')
            jetGenVector2 = TVectorD(3,array.array('d',[0.,pseudoGenJet2.px(), pseudoGenJet2.py()]))        
            jetGenVector2 =numpy.asarray(jetGenVector2,dtype='double')
            
            davismt2.set_momenta(jetGenVector1,jetGenVector2,metGenVector);
            davismt2.set_mn(0);
            
            event.mt2_gen = davismt2.get_mt2()  

            
### MM
## ===> full MT2 (jets + leptons)                                                                                                                                                                                             

        leptons_fullmt2 = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta())<2.5 ]

        objects_fullmt2 = objects40jc + leptons_fullmt2
        #objects_fullmt2 = leptons_fullmt2 + objects40jc

        objects_fullmt2.sort(key = lambda obj : obj.pt(), reverse = True)

        if len(objects_fullmt2)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()

            for obj in objects_fullmt2:
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

            #event.full_pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            #event.full_pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)
            
            ### MM
            # Ordering pseudoJets in pT

            pseudoJet1pt2 = pseudoJet1px*pseudoJet1px + pseudoJet1py*pseudoJet1py
            pseudoJet2pt2 = pseudoJet2px*pseudoJet2px + pseudoJet2py*pseudoJet2py

            if pseudoJet1pt2 >= pseudoJet2pt2:
                event.full_pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.full_pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)
            else:
                event.full_pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
                event.full_pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            ###

            full_metVector = TVectorD(3,array.array('d',[0.,event.met.px(), event.met.py()]))
            full_metVector =numpy.asarray(full_metVector,dtype='double')
            full_jetVector1 = TVectorD(3,array.array('d',[0.,event.full_pseudoJet1.px(), event.full_pseudoJet1.py()]))
            full_jetVector1 =numpy.asarray(full_jetVector1,dtype='double')
            full_jetVector2 = TVectorD(3,array.array('d',[0.,event.full_pseudoJet2.px(), event.full_pseudoJet2.py()]))
            full_jetVector2 =numpy.asarray(full_jetVector2,dtype='double')
            
            davismt2.set_momenta(full_jetVector1,full_jetVector2,full_metVector);
            davismt2.set_mn(0);

            event.fullmt2 = davismt2.get_mt2()

###


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
### MM

## ===> full gamma_MT2

        gamma_objects40jc = [ j for j in event.gamma_cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]
        gamma_leptons_fullmt2 = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta())<2.5 ]

        gamma_objects_fullmt2 = gamma_objects40jc + gamma_leptons_fullmt2
    
        gamma_objects_fullmt2.sort(key = lambda obj : obj.pt(), reverse = True)

        if len(gamma_objects_fullmt2)>=2:

            pxvec  = ROOT.std.vector(float)()
            pyvec  = ROOT.std.vector(float)()
            pzvec  = ROOT.std.vector(float)()
            Evec  = ROOT.std.vector(float)()
            grouping  = ROOT.std.vector(int)()

            for obj in gamma_objects_fullmt2:
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

            gamma_metVector = TVectorD(3,array.array('d',[0.,event.gamma_met.px(), event.gamma_met.py()]))
            gamma_metVector = numpy.asarray(gamma_metVector,dtype='double')
            gamma_jetVector1 = TVectorD(3,array.array('d',[0.,event.gamma_pseudoJet1.px(), event.gamma_pseudoJet1.py()]))
            gamma_jetVector1 =numpy.asarray(gamma_jetVector1,dtype='double')
            gamma_jetVector2 = TVectorD(3,array.array('d',[0.,event.gamma_pseudoJet2.px(), event.gamma_pseudoJet2.py()]))
            gamma_jetVector2 =numpy.asarray(gamma_jetVector2,dtype='double')

            davismt2.set_momenta(gamma_jetVector1,gamma_jetVector2,gamma_metVector);
            davismt2.set_mn(0);

            event.gamma_mt2 = davismt2.get_mt2()


## ===> zll_MT2
        
        vetoLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
            
        if len(vetoLeptons)==2 and len(objects40jc)>=2:

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

            event.zll_pseudoJet1 = ROOT.reco.Particle.LorentzVector( pseudoJet1px, pseudoJet1py, pseudoJet1pz, pseudoJet1energy)
            event.zll_pseudoJet2 = ROOT.reco.Particle.LorentzVector( pseudoJet2px, pseudoJet2py, pseudoJet2pz, pseudoJet2energy)

            zll_metVector = TVectorD(3,array.array('d',[0.,event.zll_met.px(), event.zll_met.py()]))
            zll_metVector = numpy.asarray(zll_metVector,dtype='double')
            zll_jetVector1 = TVectorD(3,array.array('d',[0.,event.zll_pseudoJet1.px(), event.zll_pseudoJet1.py()]))
            zll_jetVector1 =numpy.asarray(zll_jetVector1,dtype='double')
            zll_jetVector2 = TVectorD(3,array.array('d',[0.,event.zll_pseudoJet2.px(), event.zll_pseudoJet2.py()]))
            zll_jetVector2 =numpy.asarray(zll_jetVector2,dtype='double')

            davismt2.set_momenta(zll_jetVector1,zll_jetVector2,zll_metVector);
            davismt2.set_mn(0);

            event.zll_mt2 = davismt2.get_mt2()

        else:
            event.zll_mt2 = -999.
###

    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.mtw=-999
        event.mtwTau=-999
        event.mtwIsoTrack=-999

        event.mt2_gen=-999
        event.mt2=-999
        event.mt2lept=-999        
        event.mt2w=-999
        event.pseudoJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.pseudoJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        
        ### MM
        #event.fullmt2_gen=-999
        #event.full_pseudoGenJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        #event.full_pseudoGenJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.fullmt2=-999
        event.full_pseudoJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.full_pseudoJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.gamma_mt2=-999
        event.gamma_pseudoJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.gamma_pseudoJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        event.zll_mt2=-999
        event.zll_pseudoJet1 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
        event.zll_pseudoJet2 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )

        ###
        
        #### MM
        #self.makeGammaObjects(event)
        #self.makeZllObjects(event)
        ####

        self.makeMT(event)
        self.makeMT2(event)

#        print 'variables computed: MT=',event.mtw,'MT2=',event.mt2,'MT2W=',event.mt2w
#        print 'pseudoJet1 px=',event.pseudoJet1.px(),' py=',event.pseudoJet1.py(),' pz=',event.pseudoJet1.pz()
#        print 'pseudoJet2 px=',event.pseudoJet2.px(),' py=',event.pseudoJet2.py(),' pz=',event.pseudoJet2.pz()   

        return True
