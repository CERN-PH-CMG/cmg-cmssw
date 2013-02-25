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

    def fillGenLeptons(self, event, particle, isTau=False, sourceId=25):
        """Get the gen level light leptons (prompt and/or from tau decays)"""

        for i in xrange( particle.numberOfDaughters() ):
            dau = GenParticle(particle.daughter(i))
            dau.sourceId = sourceId
            id = abs(dau.pdgId())
            if id in [11,13]:
                if isTau: event.gentauleps.append(dau)
                else:     event.genleps.append(dau)
            elif id == 15:
                self.fillGenLeptons(event, dau, True, sourceId)
            elif id in [22,23,24]:
                self.fillGenLeptons(event, dau, False, sourceId)

    def fillWZQuarks(self, event, particle, isWZ=False, sourceId=25):
        """Descend daughters of 'particle', and add quarks from W,Z to event.genwzquarks
           isWZ is set to True if already processing daughters of W,Z's, to False before it"""

        for i in xrange( particle.numberOfDaughters() ):
            dau = GenParticle(particle.daughter(i))
            dau.sourceId = sourceId
            id = abs(dau.pdgId())
            if id <= 5 and isWZ:
                event.genwzquarks.append(dau)
            elif id in [22,23,24]:
                self.fillWZQuarks(event, dau, True, sourceId)

    def fillTopQuarks(self, event):
        """Get the b quarks from top decays into event.genbquarks"""

        event.gentopquarks = [ p for p in event.genParticles if (p.status() == 3 and abs(p.pdgId()) == 6) ]
        #if len(event.gentopquarks) != 2:
        #    print "Not two top quarks? \n%s\n" % event.gentopquarks

        for tq in event.gentopquarks:
            for i in xrange( tq.numberOfDaughters() ):
                dau = GenParticle(tq.daughter(i))
                if abs(dau.pdgId()) == 5:
                    dau.sourceId = 6
                    event.genbquarks.append( dau )
                elif abs(dau.pdgId()) == 24:
                    self.fillGenLeptons( event, dau, sourceId=6 )
                    self.fillWZQuarks(   event, dau, True, sourceId=6 )

        
    def makeMCInfo(self, event):
        event.genParticles = map( GenParticle, self.mchandles['genParticles'].product() )

        event.genHiggsBoson = None
        event.genleps    = []
        event.gentauleps = []
        event.genbquarks  = []
        event.genwzquarks = []
        event.gentopquarks  = []

        higgsBosons = [ p for p in event.genParticles if (p.status() == 3 and p.pdgId() == 25) ]

        if len(higgsBosons) == 0:
            event.genHiggsDecayMode = 0

            ## Matching that can be done also on non-Higgs events
            ## First, top quarks
            self.fillTopQuarks( event )

            ## Then W,Z,gamma from hard scattering and that don't come from a top and don't rescatter
            def hasAncestor(particle, filter):
                for i in xrange(particle.numberOfMothers()):
                    mom = particle.mother(i)
                    if filter(mom) or hasAncestor(mom, filter): 
                        return True
                return False
            def hasDescendent(particle, filter):
                for i in xrange(particle.numberOfDaughters()):
                    dau = particle.daughter(i)
                    if filter(dau) or hasAncestor(dau, filter):
                        return True
                return False

            bosons = [ gp for gp in event.genParticles if gp.status() == 3 and abs(gp.pdgId()) in [22,23,34] ]
            for b in bosons:
                if hasAncestor(b,   lambda gp : abs(gp.pdgId()) == 6): continue
                if hasDescendent(b, lambda gp : abs(gp.pdgId()) in [22,23,34] and gp.status() == 3): continue
                self.fillGenLeptons(event, b, sourceId=abs(b.pdgId()))
                self.fillWZQuarks(event, b, isWZ=True, sourceId=abs(b.pdgId()))
        else:
            if len(higgsBosons) > 1: 
                print "More than one higgs? \n%s\n" % higgsBosons

            event.genHiggsBoson = GenParticle(higgsBosons[-1])
            event.genHiggsDecayMode = abs( event.genHiggsBoson.daughter(0).pdgId() )
            self.fillTopQuarks( event )
            self.fillWZQuarks(   event, event.genHiggsBoson )
            self.fillGenLeptons( event, event.genHiggsBoson, sourceId=25 )
            if self.cfg_ana.verbose:
                print "Higgs boson decay mode: ", event.genHiggsDecayMode
                print "Generator level prompt light leptons:\n", "\n".join(["\t%s" % p for p in event.genleps])
                print "Generator level light leptons from taus:\n", "\n".join(["\t%s" % p for p in event.gentauleps])
                print "Generator level b quarks from top:\n", "\n".join(["\t%s" % p for p in event.genbquarks])
                print "Generator level quarks from W, Z decays:\n", "\n".join(["\t%s" % p for p in event.genwzquarks])

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

        # if MC and filtering on the Higgs decay mode, 
        # them do filter events
        if self.cfg_ana.filterHiggsDecays:
            if event.genHiggsDecayMode not in self.cfg_ana.filterHiggsDecays:
                return False

        return True
