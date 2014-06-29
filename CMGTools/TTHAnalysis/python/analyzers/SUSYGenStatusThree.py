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
        
class SUSYGenStatusThree( Analyzer ):
    """Do generator-level analysis of a ttH->leptons decay:

       Creates in the event:
         event.genParticles   = the gen particles (pruned, as default)
         event.genHiggsDecayMode =   0  for non-Higgs
                                 15  for H -> tau tau
                                 23  for H -> Z Z
                                 24  for H -> W W
                                 xx  for H -> xx yy zzz 

          event.genleps    = [ status 3 gen electrons and muons not from tau decays ]
          event.gentauleps = [ gen electrons and muons from decays of status 3 taus ]
          event.genbquarks  = [ gen b quarks (status = 3) from to quark decays ]
          event.genwzquarks = [ gen quarks from hadronic W,Z decays ]

       If filterHiggsDecays is set to a list of Higgs decay modes,
       it will filter events that have those decay modes.
       e.g. [0, 15, 23, 24] will keep data, non-Higgs MC and Higgs decays to (tau, Z, W) 
       but will drop Higgs decays to other particles (e.g. bb).
      
    """
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(SUSYGenStatusThree,self).__init__(cfg_ana,cfg_comp,looperName)
      
    #---------------------------------------------
    # DECLARATION OF HANDLES OF GEN LEVEL OBJECTS 
    #---------------------------------------------
        
    def declareHandles(self):
        super(SUSYGenStatusThree, self).declareHandles()

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )
    def beginLoop(self):
        super(SUSYGenStatusThree,self).beginLoop()

    def makeMCInfo(self, event):
        event.genParticles = map( GenParticle, self.mchandles['genParticles'].product() )
        event.genStatusThree  = [p for p in event.genParticles if (p.status() == 3)]
        # add the ID of the mother to be able to recreate last decay chains, use sourceId variable as used in ntupleTypes
        for p in event.genStatusThree:
            if(p.numberOfMothers()==0 ):
                p.sourceId = 0
            elif (p.numberOfMothers()==1):
                p.sourceId = p.mother(0).pdgId()
            else:
                p.sourceId = -9999

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
