import operator 
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle, PhysicsObject, printOut
from CMGTools.RootTools.physicsobjects.genutils import *


class GenParticleAnalyzer( Analyzer ):
    '''Base analyzer for GenParticle analysis.
    Puts the collection of GenParticles in the event, as event.genParticles.
    Prints the list if verbose is True in the configuration.'''
    
    def beginLoop(self):
        super(GenParticleAnalyzer,self).beginLoop()        

    def buildGenParticles(self, cmgGenParticles, event):
        '''Creates python GenParticles from the di-leptons read from the disk.
        to be overloaded if needed.'''
        return map( GenParticle, cmgGenParticles )
    
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        if not self.cfg_comp.isMC:
            return True
        
        event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )
        
        if self.cfg_ana.verbose:
            print self.name
            print printOut(event.genParticles)
        
        return True


    def declareHandles(self):
        '''Reads genParticlesPruned.'''
        super(GenParticleAnalyzer, self).declareHandles()
        self.mchandles['genpart'] =  AutoHandle(
            'genParticlesPruned',
            'std::vector<reco::GenParticle>'
            )
    
