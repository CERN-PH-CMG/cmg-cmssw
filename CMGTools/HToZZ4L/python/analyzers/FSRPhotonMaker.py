from math import *
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.HeppyCore.utils.deltar import deltaR, deltaPhi
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
        pf = map( PhysicsObject, self.handles['pf'].product() )
        leptons = getattr(event,self.leptonTag)
        self.IsolationComputer.setPackedCandidates(self.handles['pf'].product())


        #first trim the photons that are only near leptons
        direct=[]
        forIso=[]


        for p in pf:
            if p.pdgId() !=22 or (p.pt()>2.0 and abs(p.eta())<2.4):
                continue
            for l in leptons:
                if abs(l.pdgId())==11 and self.electronID(l):
                    if (abs(p.eta()-l.eta())<0.05 or deltaPhi(p.phi(),l.phi())<0.2) or deltaR(p.eta(),p.phi(),l.eta(),l.phi())<0.15: 
                        continue;

                DR =deltaR(l.eta(),l.phi(),p.eta(),p.phi())     
                if DR<0.07 and p.pt()>2.0:
                    direct.append(p)
                    break;
                elif  DR<0.5 and p.pt()>4.0:   
                    forIso.append(p)
                    break;
                    

        isolatedPhotons=[]        
        for g in forIso:
            iso = (self.IsolationComputer.chargedAbsIso(g.physObj,0.3,0,0.2)+self.IsolationComputer.puAbsIso(g.physObj,0.3,0,0.2)+self.IsolationComputer.neutralAbsIsoRaw(g.physObj,0.3,0,0.5))/g.pt()
            if iso<1.0:
                isolatedPhotons.append(g)

        event.fsrPhotons = isolatedPhotons+direct


        return True
        
