import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
# from CMGTools.RootTools.physicsobjects.DiObject import DiObject
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched


class DiLepton( TLorentzVector ):
    def __init__(self, lepton1, lepton2):
        lv1 = TLorentzVector( lepton1.px(), lepton1.py(), lepton1.pz(), lepton1.energy() )
        lv2 = TLorentzVector( lepton2.px(), lepton2.py(), lepton2.pz(), lepton2.energy() )
        lv1 += lv2 
        super( DiLepton, self).__init__( lv1 )
        self.leg1 = lepton1
        self.leg2 = lepton2

    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        return getattr( super(DiLepton, self), name.capitalize() )
        
    def charge(self):
        return self.leg1.charge() + self.leg2.charge()
        
    def __str__(self):
        return ', '.join( ['DiObject:', str(self.leg1), str(self.leg2)] )

        
class FourLeptonAnalyzer( Analyzer ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def beginLoop(self):
        super(FourLeptonAnalyzer,self).beginLoop()
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')
        count.register('enough leptons')
        count.register('Z1 found')
        
##         count.register('>= 2 leptons')
##         count.register('>= 3 leptons')
##         count.register('>= 4 leptons')
#         count.register('first Z cuts passed')
#         count.register('>= 2 leptons')
        count.register('passing')
        
    def process(self, iEvent, event):
        self.readCollections( iEvent )
        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )
        event.leptons2 = map( self.__class__.LeptonClass2,
                              self.handles['leptons2'].product() )

        event.selLeptons1 = filter( self.testLepton1,
                                    event.leptons1 )
        
        event.selLeptons2 = filter( self.testLepton2,
                                    event.leptons2 )

        event.allSelLeptons = copy.copy( event.selLeptons1 )
        event.allSelLeptons.extend( event.selLeptons2 ) 

        self.counters.counter('FourLepton').inc('all events')

        nL = len( event.allSelLeptons )
        if nL < self.cfg_ana.nLeptonsMin:
            return False

        self.counters.counter('FourLepton').inc('enough leptons')

        event.leptonPairs = self.findLeptonPairs( event.allSelLeptons )
        event.selZBosons1 = filter( self.testZ1, event.leptonPairs )
        if len(event.selZBosons1)<1:
            return False
        self.counters.counter('FourLepton').inc('Z1 found')
        
        event.zBoson1 = self.bestZBoson( event.selZBosons1 )  
        
##         if nL < 2:
##             return False
##         self.counters.counter('FourLepton').inc('>= 2 leptons')
##         if nL < 3:
##             return False
##         self.counters.counter('FourLepton').inc('>= 3 leptons')
##         if nL < 4:
##             return False
##         self.counters.counter('FourLepton').inc('>= 4 leptons')
        
##         event.zBosons1 = self.findLeptonPairs( event.selLeptons1 )             
##         event.selZBosons1 = filter( self.testZ1, event.zBosons1 ) 
##         if len( event.selZBosons1 )==0:
##             return False
##         self.counters.counter('FourLepton').inc('first Z cuts passed')

##         event.sortedLeptons = self.sortLeptons( [event.zBosons1] ) 
        
##         if nL2 < 2:
##             return False
##         self.counters.counter('FourLepton').inc('>= 2 leptons in col 2')
##         event.zBosons2 = self.findLeptonPairs( event.selLeptons2 )             
##         # event.selZBosons2 = filter( self.testZ2, event.zBosons2 ) 

        self.counters.counter('FourLepton').inc('passing')

        return True

    def bestZBoson(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            # taking the closest from the Z mass
            zMass = 91.2
            return min(zBosons, key = lambda x: abs( x.mass() - zMass) )
             
    
    def findLeptonPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiLepton(l1, l2) )
        return out


    def testZ1(self, zCand):
        return zCand.M() > self.cfg_ana.z1_m and \
               zCand.leg1.pt() > self.cfg_ana.z1_pt1 and \
               zCand.leg2.pt() > self.cfg_ana.z1_pt2 and \
               zCand.charge() == 0

    def testZ2(self, zCand):
        return zCand.M() > self.cfg_ana.z2_m and \
               zCand.leg1.pt() > self.cfg_ana.z2_pt1 and \
               zCand.leg2.pt() > self.cfg_ana.z2_pt2 and \
               zCand.charge() == 0


    def testLepton(self, lepton, pt, eta, iso, sel=None):
        if sel is not None and \
           not lepton.getSelection(sel):
            # a cut string has to be tested, and the lepton does not pass
            return False
        if lepton.pt() > pt and \
           abs(lepton.eta()) < eta and \
           lepton.relIso( 0.5 ) < iso:
            return True
        else:
            return False


    def testLepton1(self, lepton):
        '''Overload according to type, see e.g. TauMuAnalyzer.'''
        sel = None
        if hasattr(self.cfg_ana, 'cutString1'):
            sel = self.cfg_ana.cutString1
        return self.testLepton( lepton,
                             pt = self.cfg_ana.pt1,
                             eta = self.cfg_ana.eta1,
                             iso = self.cfg_ana.iso1,
                             sel = sel )


    def testLepton2(self, lepton):
        '''Overload according to type, see e.g. TauMuAnalyzer.'''
        sel = None
        if hasattr(self.cfg_ana, 'cutString2'):
            sel = self.cfg_ana.cutString2
        return self.testLepton( lepton,
                             pt = self.cfg_ana.pt2,
                             eta = self.cfg_ana.eta2,
                             iso = self.cfg_ana.iso2,
                             sel = sel )


    
    def testMuon(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        return True

    
    def testElectron(self, electron):
        '''Returns True if an electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        return True

