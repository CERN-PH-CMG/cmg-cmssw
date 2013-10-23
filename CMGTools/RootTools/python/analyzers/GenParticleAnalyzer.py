import operator 
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle, PhysicsObject, printOut
from CMGTools.RootTools.physicsobjects.genutils import *

from ROOT import FSRWeightAlgo

class GenParticleAnalyzer( Analyzer ):
    '''Base analyzer for GenParticle analysis.
    Puts the collection of GenParticles in the event, as event.genParticles.
    Prints the list if verbose is True in the configuration.'''
    

    def declareHandles(self):
        '''Reads genParticlesPruned.'''
        super(GenParticleAnalyzer, self).declareHandles()
        self.mchandles['genpart'] =  AutoHandle(
            self.cfg_ana.src,
            'std::vector<reco::GenParticle>'
            )

    def beginLoop(self):
        super(GenParticleAnalyzer,self).beginLoop()        
        self.fsrWeightAlgo = FSRWeightAlgo()

    def buildGenParticles(self, cmgGenParticles, event):
        '''Creates python GenParticles from the di-leptons read from the disk.
        to be overloaded if needed.'''
        return map( GenParticle, cmgGenParticles )
    
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        if not self.cfg_comp.isMC:
            return True
        
        event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )

        self.fsrWeightAlgo.clear()
        # print '-'*50
        for gen in self.mchandles['genpart'].product():
            if gen.status()!=1:
                pass
                # print gen.pdgId(), gen.status(), gen.pt()
            self.fsrWeightAlgo.addGenParticle(gen)
        event.fsrWeight = self.fsrWeightAlgo.weight()
        
        if self.cfg_ana.verbose:
            print self.name
            print printOut(event.genParticles)
        
        return True

