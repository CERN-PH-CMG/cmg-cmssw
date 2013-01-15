import operator 
import itertools
import copy
from math import *

from ROOT import TLorentzVector

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


        
class ttHLepMCMatchAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepMCMatchAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHLepMCMatchAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHLepMCMatchAnalyzer,self).beginLoop()

    def matchLeptons(self, event):
        match = matchObjectCollection2(event.selectedLeptons, 
                                       event.genleps + event.gentauleps, 
                                       deltaRMax = 0.3)
        for lep in event.selectedLeptons:
            gen = match[lep]
            lep.mcMatchId = (gen.sourceId if gen != None else 0)

    def isFromB(self,particle):
        maxflavour = 0
        for i in xrange( particle.numberOfMothers() ): 
            mom  = particle.mother(i)
            momid = abs(mom.pdgId())
            if momid / 1000 == 5 or momid / 100 == 5 or momid == 5: 
                return True
            elif mom.status() == 2 and self.isFromB(mom):
                return True
        return False

    def sourceBQuark(self,particle,event):
        for i in xrange( particle.numberOfMothers() ):
            mom  = particle.mother(i)
            if mom.status() == 3 and abs(mom.pdgId()) == 5:
                return mom
            elif mom.status() == 2:
                momB = self.sourceBQuark(mom,event)
                if momB != None: return momB
        return None
                
    def matchAnyLeptons(self, event): 
        event.anyLeptons = [ x for x in event.genParticles if x.status() == 1 and abs(x.pdgId()) in [11,13] ]
        match = matchObjectCollection2(event.selectedLeptons, event.anyLeptons, deltaRMax = 0.3)
        for lep in event.selectedLeptons:
            gen = match[lep]
            lep.mcMatchAny = ((1 + self.isFromB(gen)) if gen != None else 0)
            lep.mcDeltaRB  = 999
            if gen != None:
                bgen = self.sourceBQuark(gen,event)
                if bgen != None:
                    lep.mcDeltaRB = deltaR(bgen.eta(),bgen.phi(),lep.eta(),lep.phi())

    def matchJets(self, event):
        match = matchObjectCollection2(event.cleanJets,
                                       event.genbquarks + event.genwzquarks,
                                       deltaRMax = 0.5)
        for jet in event.cleanJets:
            gen = match[jet]
            jet.mcMatchId   = (gen.sourceId     if gen != None else 0)
            jet.mcMatchFlav = (abs(gen.pdgId()) if gen != None else 0)

    def process(self, iEvent, event):

        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        self.matchLeptons(event)

        self.matchAnyLeptons(event)

        self.matchJets(event)

        return True
