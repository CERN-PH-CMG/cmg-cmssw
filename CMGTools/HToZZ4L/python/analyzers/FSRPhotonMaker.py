from math import *
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi, bestMatch, inConeCollection
from PhysicsTools.Heppy.physicsobjects.PhysicsObject import PhysicsObject


from PhysicsTools.HeppyCore.framework.event import Event
from PhysicsTools.HeppyCore.statistics.counter import Counter, Counters
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle


from ROOT import heppy

import os
import itertools
        
class FSRPhotonMaker( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(FSRPhotonMaker,self).__init__(cfg_ana,cfg_comp,looperName)
        self.leptonTag  = cfg_ana.leptons
        self.electronID = cfg_ana.electronID
        self.IsolationComputer = heppy.IsolationComputer(0.3)

    def declareHandles(self):
        super(FSRPhotonMaker, self).declareHandles()
        self.handles['pf'] = AutoHandle( "packedPFCandidates",'std::vector<pat::PackedCandidate>')

    def beginLoop(self, setup):
        super(FSRPhotonMaker,self).beginLoop(setup)

    def process(self, event):
        self.readCollections( event.input )
        pf = self.handles['pf'].product()
        leptons = getattr(event,self.leptonTag)
        self.IsolationComputer.setPackedCandidates(self.handles['pf'].product())

        forIso = []
        for p in pf:
            if p.pdgId() != 22 or not( p.pt() > 2.0 and abs(p.eta()) < 2.4 ):
                continue
            scVetoed = False
            for l in leptons:
                if abs(l.pdgId())==11 and self.electronID(l):
                    #print "Testing photon pt %5.1f eta %+7.4f phi %+7.4f vs ele pt %.1f eta %+7.4f  phi %+7.4f sc  eta %+7.4f  phi %+7.4f" % ( p.pt(), p.eta(), p.phi(), l.pt(), l.eta(), l.phi(), l.superCluster().eta(), l.superCluster().phi() )
                    #print "Testing                                                       deta %+7.4f dphi %+7.4f sc deta %+7.4f dphi %+7.4f" % ( abs(p.eta()-l.eta()), deltaPhi(p.phi(),l.phi()), abs(p.eta()-l.superCluster().eta()), deltaPhi(p.phi(),l.superCluster().phi()))
                    if self.cfg_ana.electronVeto == "superclusterEta":
                        if (abs(p.eta()-l.superCluster().eta())<0.05 and abs(deltaPhi(p.phi(),l.superCluster().phi()))<2) or deltaR(p.eta(),p.phi(),l.superCluster().eta(),l.superCluster().phi())<0.15: 
                            scVetoed = True; break
                    elif self.cfg_ana.electronVeto == "electronEta":
                        if (abs(p.eta()-l.eta())<0.05 and abs(deltaPhi(p.phi(),l.phi()))<2) or deltaR(p.eta(),p.phi(),l.eta(),l.phi())<0.15: 
                            scVetoed = True; break
                    else: 
                        raise RuntimeError, "electronVeto option %r not implemented" % self.cfg_ana.electronVeto
            if scVetoed: continue
            closestLepton, minDR2 = bestMatch(p, leptons)
            if minDR2 >= 0.5*0.5: continue
            p.globalClosestLepton = closestLepton
            forIso.append(p)

        isolatedPhotons = []
        relIsoCut = self.cfg_ana.relIsoCut        
        for g in forIso:
            g.absIsoCH = self.IsolationComputer.chargedAbsIso(g,0.3,0.0001,0.2)
            g.absIsoPU = self.IsolationComputer.puAbsIso(g,0.3,0.0001,0.2)
            g.absIsoNH = self.IsolationComputer.neutralHadAbsIsoRaw(g,0.3,0.01,0.5)
            g.absIsoPH = self.IsolationComputer.photonAbsIsoRaw(g,0.3,0.01,0.5)
            g.relIso   = (g.absIsoCH+g.absIsoPU+g.absIsoNH+g.absIsoPH)/g.pt()
            if g.relIso < relIsoCut:
                isolatedPhotons.append(g)

        event.fsrPhotonsNoIso = forIso
        event.fsrPhotonsIso   = isolatedPhotons

        event.attachedFsrPhotons = []
        drOverET2Cut = self.cfg_ana.drOverET2Cut
        for l in leptons:
            best = None
            for p in event.fsrPhotonsIso:
                if p.globalClosestLepton != l: continue
                drOverET2 = deltaR(p.eta(),p.phi(),l.eta(),l.phi())/p.et2()
                if drOverET2 >= drOverET2Cut: continue
                p.drOverET2 = drOverET2
                if best == None or best.drOverET2 > drOverET2:
                    best = p
            if best:
                event.attachedFsrPhotons.append(best)
                l.ownFsrPhotons = [ best ]
            else: 
                l.ownFsrPhotons = [ ]

        #now attach FSR photons to all leptons (for isolation)
        for l in leptons:
            l.fsrPhotons = inConeCollection(l, event.attachedFsrPhotons, 0.5, -1)
            l.relIsoAfterFSR = l.absIsoWithFSR()/l.pt()

        #define list of FSR photons attached to photons that pass post-FSR iso, for jet cleaning
        event.selectedPhotons = []
        for l in leptons:
            if l.tightId() and l.relIsoAfterFSR < (0.4 if abs(l.pdgId()) == 13 else 0.5):
                event.selectedPhotons += l.ownFsrPhotons

        return True
        
