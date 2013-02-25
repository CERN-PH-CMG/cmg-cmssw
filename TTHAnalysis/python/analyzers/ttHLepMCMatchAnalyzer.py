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

def matchObjectCollection3 ( objects, matchCollection, deltaRMax = 0.3, filter = lambda x,y : True ):
    '''Univoque association of an element from matchCollection to an element of objects.
    Reco and Gen objects get the "matched" attribute, true is they are re part of a matched tulpe.
    By default, the matching is true only if delta R is smaller than 0.3.
    '''
    # 
    pairs = {}
    if len(objects)==0:
            return pairs
    if len(matchCollection)==0:
            return dict( zip(objects, [None]*len(objects)) )
    # build all possible combinations
    allPairs = [(deltaR2 (object.eta(), object.phi(), match.eta(), match.phi()), (object, match)) for object in objects for match in matchCollection if filter(object,match) ]
    allPairs.sort ()
    #
    # to flag already matched objects
    # FIXME this variable remains appended to the object, I do not like it
    for object in objects:
        object.matched = False
    for match in matchCollection:
        match.matched = False
    #
    deltaR2Max = deltaRMax * deltaRMax
    for dR2, (object, match) in allPairs:
        if dR2 > deltaR2Max:
            break
        if dR2 < deltaR2Max and object.matched == False and match.matched == False:
            object.matched = True
            match.matched = True
            pairs[object] = match
    #
    for object in objects:
       if object.matched == False:
           pairs[object] = None
    #
    return pairs
    # by now, the matched attribute remains in the objects, for future usage
    # one could remove it with delattr (object, attrname)

        
class ttHLepMCMatchAnalyzer( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHLepMCMatchAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

    def declareHandles(self):
        super(ttHLepMCMatchAnalyzer, self).declareHandles()

    def beginLoop(self):
        super(ttHLepMCMatchAnalyzer,self).beginLoop()

    def matchLeptons(self, event):
        def plausible(rec,gen):
            if abs(rec.pdgId()) == 11 and abs(gen.pdgId()) != 11:   return False
            if abs(rec.pdgId()) == 13 and abs(gen.pdgId()) != 13:   return False
            dr = deltaR(rec.eta(),rec.phi(),gen.eta(),gen.phi())
            if dr < 0.3: return True
            if rec.pt() < 10 and abs(rec.pdgId()) == 13 and gen.pdgId() != rec.pdgId(): return False
            if dr < 0.7: return True
            if min(rec.pt(),gen.pt())/max(rec.pt(),gen.pt()) < 0.3: return False
            return True

        match = matchObjectCollection3(event.selectedLeptons, 
                                       event.genleps + event.gentauleps, 
                                       deltaRMax = 1.2, filter = plausible)
        for lep in event.selectedLeptons:
            gen = match[lep]
            lep.mcMatchId = (gen.sourceId if gen != None else 0)

    def isFromB(self,particle):
        for i in xrange( particle.numberOfMothers() ): 
            mom  = particle.mother(i)
            momid = abs(mom.pdgId())
            if momid / 1000 == 5 or momid / 100 == 5 or momid == 5: 
                return True
            elif mom.status() == 2 and self.isFromB(mom):
                return True
        return False

    def isFromHard(self,particle):
        for i in xrange( particle.numberOfMothers() ): 
            mom  = particle.mother(i)
            momid = abs(mom.pdgId())
            if mom.status() == 3 and momid >= 11 and momid <= 25 and momid != 21: 
                return True
            elif momid > 25 or momid == 21: # hadrons or gluons
                return False
            if self.isFromHard(mom):
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
        match = matchObjectCollection3(event.selectedLeptons, event.anyLeptons, deltaRMax = 0.3, filter = lambda x,y : abs(x.pdgId()) == abs(y.pdgId()))
        for lep in event.selectedLeptons:
            gen = match[lep]
            lep.mcMatchAny = ((1 + self.isFromB(gen)) if gen != None else 0)
            if gen != None and lep.mcMatchId == 0:
                if self.isFromHard(gen): lep.mcMatchId = 100
            lep.mcDeltaRB  = 999
            if gen != None:
                bgen = self.sourceBQuark(gen,event)
                if bgen != None:
                    lep.mcDeltaRB = deltaR(bgen.eta(),bgen.phi(),lep.eta(),lep.phi())

    def process(self, iEvent, event):

        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        self.matchLeptons(event)

        self.matchAnyLeptons(event)

        return True
