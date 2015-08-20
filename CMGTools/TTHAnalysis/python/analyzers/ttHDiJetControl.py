import operator
import itertools
import copy
import types

from math import *

from ROOT import TLorentzVector

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle

from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi

def mtw(x1,x2):
    return sqrt(2*x1.pt()*x2.pt()*(1-cos(x1.phi()-x2.phi())))

class ttHDiJetControl( Analyzer ):


    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHDiJetControl,self).__init__(cfg_ana,cfg_comp,looperName)
        self.jetPt = cfg_ana.jetPt

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

        if hasattr(event, 'selectedIsoCleanTrack'):
            if len(event.selectedIsoTrack)>0:
                for myTrack in event.selectedIsoTrack:
                    event.mtwIsoTrack = mtw(myTrack, event.met)      
##number of PF leptons (e,mu) with pt > 5, reliso < 0.2, MT < 100 
#number of PF hadrons with pt > 10, reliso < 0.1, MT < 100                        
                    if event.mtwIsoTrack < 100:
                        if abs(myTrack.pdgId()) == 11 or abs(myTrack.pdgId()) == 13:
                            if myTrack.pt()>5 and myTrack.absIso/myTrack.pt()<0.2:
                                event.nPFLep5LowMT += 1                           
                        if abs(myTrack.pdgId()) == 211:
                            if myTrack.pt()>10 and myTrack.absIso/myTrack.pt()<0.1:
                                event.nPFHad10LowMT += 1                            

        leptons = []
        if hasattr(event, 'selectedLeptons'):
            leptons = [ l for l in event.selectedLeptons if l.pt() > 10]
        if hasattr(event, 'selectedIsoCleanTrack'):
            leptons = leptons[:] + event.selectedIsoCleanTrack
        if len(leptons)>0:
            for lepton in leptons:
                mtwLep = mtw(lepton, event.met)
                if mtwLep < 100:
                    event.nLepLowMT +=1

    def makeZllObjects(self, event):

        import ROOT

        vetoLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]

        # MET + zll                                                                                                                                                                                                               
        event.zll_ht = -999.
        event.zll_deltaPhiMin = -999.
        event.zll_met_pt = -999.
        event.zll_met_phi = -999.
        event.zll_diffMetMht = -999.
        event.zll_mhtJet40j = -999.
        event.zll_mhtPhiJet40j = -999.
        event.zll_p4 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
                      
        event.zll_ht_Xj = -999.
        event.zll_deltaPhiMin_Xj = -999.
        event.zll_diffMetMht_Xj = -999.
        event.zll_mhtJetXj = -999.
        event.zll_mhtPhiJetXj = -999.

        if len(vetoLeptons)==2:
            event.zll_met = ROOT.reco.Particle.LorentzVector( event.met.px(), event.met.py(), 0, 0 )
            for l in vetoLeptons:
                event.zll_met = ROOT.reco.Particle.LorentzVector( event.zll_met.px() + l.px(), event.zll_met.py() + l.py() , 0, 0 )

            event.zll_met_pt = event.zll_met.pt()
            event.zll_met_phi = event.zll_met.phi()

            # defining mht as hadronic mht                                                                                                                                                                                        
            event.zll_mhtJet40j = event.mhtJet40j
            event.zll_mhtPhiJet40j = event.mhtPhiJet40j

            event.zll_mhtJetXj = event.mhtJetXj
            event.zll_mhtPhiJetXj = event.mhtPhiJetXj

            # look for minimal deltaPhi between MET and four leading jets with pt>40 and |eta|<2.4                                                                                                                                
            event.zll_deltaPhiMin = 999.
            objects40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]
            objects40ja = [ j for j in event.cleanJets if j.pt() > 40]
            event.zll_ht = sum([x.pt() for x in objects40jc])
            for n,j in enumerate(objects40ja):
                if n>2:  break
                thisDeltaPhi = abs( deltaPhi( j.phi(), event.zll_met.phi() ) )
                if thisDeltaPhi < event.zll_deltaPhiMin : event.zll_deltaPhiMin = thisDeltaPhi

            event.zll_deltaPhiMin_Xj = 999.
            objectsXjc = [ j for j in event.cleanJets if j.pt() > self.jetPt and abs(j.eta())<2.5 ]
            objectsXja = [ j for j in event.cleanJets if j.pt() > self.jetPt]
            event.zll_ht_Xj = sum([x.pt() for x in objectsXjc])
            for n,j in enumerate(objectsXja):
                if n>2:  break
                thisDeltaPhi = abs( deltaPhi( j.phi(), event.zll_met.phi() ) )
                if thisDeltaPhi < event.zll_deltaPhiMin_Xj : event.zll_deltaPhiMin_Xj = thisDeltaPhi

            # absolute value of the vectorial difference between met and mht                                                                                                                                                      
            zll_diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.mhtJet40jvec.px()-event.zll_met.px(), event.mhtJet40jvec.py()-event.zll_met.py(), 0, 0 )
            event.zll_diffMetMht = sqrt( zll_diffMetMht_vec.px()*zll_diffMetMht_vec.px() + zll_diffMetMht_vec.py()*zll_diffMetMht_vec.py() )

            zll_diffMetMht_Xj_vec = ROOT.reco.Particle.LorentzVector(event.mhtJetXjvec.px()-event.zll_met.px(), event.mhtJetXjvec.py()-event.zll_met.py(), 0, 0 )
            event.zll_diffMetMht_Xj = sqrt( zll_diffMetMht_Xj_vec.px()*zll_diffMetMht_Xj_vec.px() + zll_diffMetMht_Xj_vec.py()*zll_diffMetMht_Xj_vec.py() )

            # di-lepton invariant mass                                                                                                                                                                                            
            for l in vetoLeptons:
                event.zll_p4 += l.p4()

            ##event.zll_invmass = zll_p4.M()

    def process(self, event):

        self.readCollections( event.input )
        self.makeZllObjects(event)

        event.nLepLowMT =0
        event.nPFLep5LowMT = 0
        event.nPFHad10LowMT = 0


        self.makeMT(event)

        return True
