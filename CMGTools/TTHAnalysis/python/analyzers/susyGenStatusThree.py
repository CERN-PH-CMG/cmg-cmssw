import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle
from CMGTools.RootTools.physicsobjects.genutils import *
        
class susyGenStatusThree( Analyzer ):
    """Save status==3 genParticles    """
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(susyGenStatusThree,self).__init__(cfg_ana,cfg_comp,looperName)
      
    #---------------------------------------------
    # DECLARATION OF HANDLES OF GEN LEVEL OBJECTS 
    #---------------------------------------------
        
    def declareHandles(self):
        super(susyGenStatusThree, self).declareHandles()

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )
    def beginLoop(self):
        super(susyGenStatusThree,self).beginLoop()

    def makeMCInfo(self, event):
        genParticles = map( GenParticle, self.mchandles['genParticles'].product() )
        event.genStatusThree  = [p for p in genParticles if (p.status() == 3)]
        # add the ID of the mother to be able to recreate last decay chains
        for p in event.genStatusThree:
            if(p.numberOfMothers()==0 ):
                p.motherId = 0
            elif (p.numberOfMothers()==1):
                p.motherId = p.mother(0).pdgId()
            else:
                p.motherId = -9999

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        ## creating a "sub-event" for this analyzer
        #myEvent = Event(event.iEv)
        #setattr(event, self.name, myEvent)
        #event = myEvent
        
        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True
        # do MC level analysis
        self.makeMCInfo(event)
        return True
