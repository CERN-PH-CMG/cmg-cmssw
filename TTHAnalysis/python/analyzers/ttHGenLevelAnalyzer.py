import operator 
import itertools
import copy

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
from CMGTools.RootTools.physicsobjects.PhysicsObjects import GenParticle

from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi
from CMGTools.RootTools.physicsobjects.genutils import *

        
class ttHGenLevelAnalyzer( Analyzer ):
    """Do generator-level analysis of a ttH->leptons decay:

       Creates in a subevent:
         event.genParticles   = the gen particles (pruned, as default)
         event.higgsDecayMode =   0  for non-Higgs
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
        super(ttHGenLevelAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)


    #---------------------------------------------
    # DECLARATION OF HANDLES OF GEN LEVEL OBJECTS 
    #---------------------------------------------
        

    def declareHandles(self):
        super(ttHGenLevelAnalyzer, self).declareHandles()

        #mc information
        self.mchandles['genParticles'] = AutoHandle( 'genParticlesPruned',
                                                     'std::vector<reco::GenParticle>' )

    def beginLoop(self):
        super(ttHGenLevelAnalyzer,self).beginLoop()

    def fillGenLeptons(self, event, particle, isTau=False):
        """Get the gen level light leptons (prompt and/or from tau decays)"""

        for i in xrange( particle.numberOfDaughters() ):
            dau = particle.daughter(i)
            id = abs(dau.pdgId())
            if id in [11,13]:
                if isTau: event.gentauleps.append(GenParticle(dau))
                else:     event.genleps.append(GenParticle(dau))
            elif id == 15:
                self.fillGenLeptons(event, dau, True)
            elif id in [22,23,24]:
                self.fillGenLeptons(event, dau, False)

    def fillWZQuarks(self, event, particle, isWZ=False):
        """Descend daughters of 'particle', and add quarks from W,Z to event.genwzquarks
           isWZ is set to True if already processing daughters of W,Z's, to False before it"""

        for i in xrange( particle.numberOfDaughters() ):
            dau = particle.daughter(i)
            id = abs(dau.pdgId())
            if id <= 5 and isWZ:
                event.genwzquarks.append(GenParticle(dau))
            elif id in [22,23,24]:
                self.fillWZQuarks(event, dau, True)

    def fillTopQuarks(self, event):
        """Get the b quarks from top decays into event.genbquarks"""

        topQuarks = [ p for p in event.genParticles if (p.status() == 3 and abs(p.pdgId()) == 6) ]
        if len(topQuarks) != 2:
            print "Not two top quarks? \n%s\n" % topQuarks

        for tq in topQuarks:
            for i in xrange( tq.numberOfDaughters() ):
                if abs(tq.daughter(i).pdgId()) == 5:
                    event.genbquarks.append( GenParticle(tq.daughter(i)) )
        
    def makeMCInfo(self, event):
        event.genParticles = map( GenParticle, self.mchandles['genParticles'].product() )

        event.genleps    = []
        event.gentauleps = []
        event.genbquarks  = []
        event.genwzquarks = []

        higgsBosons = [ p for p in event.genParticles if (p.status() == 3 and p.pdgId() == 25) ]

        if len(higgsBosons) == 0:
            event.higgsDecayMode = 0
        else:
            if len(higgsBosons) > 1: 
                print "More than one higgs? \n%s\n" % higgsBosons

            event.higgsDecayMode = abs( higgsBosons[-1].daughter(0).pdgId() )
            self.fillGenLeptons( event, higgsBosons[-1] )
            self.fillWZQuarks(   event, higgsBosons[-1] )
            self.fillTopQuarks( event )

            if self.cfg_ana.verbose:
                print "Higgs boson decay mode: ", event.higgsDecayMode
                print "Generator level prompt light leptons:\n", "\n".join(["\t%s" % p for p in event.genleps])
                print "Generator level light leptons from taus:\n", "\n".join(["\t%s" % p for p in event.gentauleps])
                print "Generator level b quarks from top:\n", "\n".join(["\t%s" % p for p in event.genbquarks])
                print "Generator level quarks from W, Z decays:\n", "\n".join(["\t%s" % p for p in event.genwzquarks])

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent

        # if not MC, nothing to do
        if not self.cfg_comp.isMC: 
            return True

        # do MC level analysis
        self.makeMCInfo(event)

        # if MC and filtering on the Higgs decay mode, 
        # them do filter events
        if self.cfg_ana.filterHiggsDecays:
            if event.higgsDecayMode not in self.cfg_ana.filterHiggsDecays:
                return False

        return True
