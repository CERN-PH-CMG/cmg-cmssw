import itertools

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Electron


class ZSkim( Analyzer ):
    '''This analyzer tags the event if it passes a Z skim.

     Have a look at SkimOperator.'''

    def declareHandles(self):
        super(ZSkim, self).declareHandles()
        
        self.handles['leptons'] = AutoHandle(
            self.cfg_ana.col_label,
            self.cfg_ana.col_type
            )
        
        self.hasLeptons2 = False
        if hasattr( self.cfg_ana, 'col2_label'):
            self.hasLeptons2 = True
            self.handles['leptons2'] = AutoHandle(
                self.cfg_ana.col2_label,
                self.cfg_ana.col2_type
                )
            

    def beginLoop(self):
        super(ZSkim,self).beginLoop()
        self.counters.addCounter('ZSkim')
        self.count = self.counters.counter('ZSkim')
        self.count.register('all events')
        self.count.register('>=2 leptons')
        self.count.register('>=2 good leptons')
        self.count.register('Z found')
        self.count.register('good Z found')
        self.count.register('passing')

    def getLeptons(self, idstr ):
        leps = self.handles[ idstr ].product()
        if len(leps)==0:
            return []
        LeptonClass = Muon
        if abs(leps[0].pdgId())==11:
            LeptonClass = Electron
        leptons = map(LeptonClass, leps)
        return leptons
    

    def process(self, iEvent, event):
        self.readCollections( iEvent )

        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent

        self.count.inc('all events')
     
        event.leptons = self.getLeptons('leptons')
        if self.hasLeptons2 is True:
            event.leptons.extend( self.getLeptons('leptons2') )
        
        if len(event.leptons)<2:
            event.passing = False
            return True 
        else:
            self.count.inc('>=2 leptons')

        event.selLeptons = filter( self.testLepton, event.leptons )
        if len(event.selLeptons)<2:
            event.passing = False
            return True
        else:
            self.count.inc('>=2 good leptons')

        event.zBosons = self.findPairs(event.selLeptons)
        if len( event.zBosons ) == 0:
            event.passing = False
            return True
        else:
            self.count.inc('Z found')

        event.selZBosons = filter( self.testZ, event.zBosons )
        if len( event.selZBosons ) == 0:
            event.passing = False
            return True
        else:
            self.count.inc('good Z found')

        self.count.inc('passing')
        event.passing = True
        return True

            
    def testLepton(self, lepton):
        pdgId = lepton.pdgId()
        ptMin = 7 # for muons 
        etaMax = 2.5
        if abs(pdgId)==11:
            ptMin = 10 # higher pt cut for electrons
        elif abs(pdgId)==13:
            if self.testMuon( lepton ) is False: return False
        else:
            raise ValueError('lepton is neither an e nor a mu. Its pdgid is {p}'.format(p=lepton.pdgId()))
        # either an electron or a loosely id'ed muon. checking pt and eta.
        return lepton.pt() > ptMin and \
               abs(lepton.eta()) < etaMax

 
    def testMuon(self, muon):
        '''Very loose muon id'''
        return muon.isGlobalMuon() or muon.isTrackerMuon()


    def findPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiObject(l1, l2) )
        return out


    def testZ(self, zCand):

        # if two input collections, exclude SF combinations
        if self.hasLeptons2 and \
           abs( zCand.leg1.pdgId() ) == abs( zCand.leg2.pdgId() ):
            return False

        # sorting legs by pT
        p1 = zCand.leg1
        p2 = zCand.leg2
        if p2.pt() > p1.pt():
            p1, p2 = p2, p1

        z_m = 40
        z_pt1 = 20
        # the cut on the second leg depends on the particle type
        z_pt2 = 7 # for muons
        if abs(p2.pdgId())==11:
            z_pt2 = 10 # for electrons

        return zCand.M() > z_m and \
               p1.pt() > z_pt1 and \
               p2.pt() > z_pt2 
