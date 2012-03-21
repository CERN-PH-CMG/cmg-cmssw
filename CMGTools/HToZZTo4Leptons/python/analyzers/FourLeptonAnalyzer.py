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


class DiObject( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, lepton1, lepton2):
        lv1 = TLorentzVector( lepton1.px(), lepton1.py(), lepton1.pz(), lepton1.energy() )
        lv2 = TLorentzVector( lepton2.px(), lepton2.py(), lepton2.pz(), lepton2.energy() )
        lv1 += lv2 
        super( DiObject, self).__init__( lv1 )
        self.leg1 = lepton1
        self.leg2 = lepton2

    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        return getattr( super(DiObject, self), name.capitalize() )
        
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
        count.register('enough good leptons')
        count.register('Z1 found')
        count.register('3rd lepton present')
        count.register('2nd lepton pair')
        count.register('Z2 found')
        count.register('4-lepton selected')
        count.register('passing')

        
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        self.counters.counter('FourLepton').inc('all events')

        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )
        nL = len(event.leptons1)
        if self.__class__.LeptonClass2 != self.__class__.LeptonClass1:
            event.leptons2 = map( self.__class__.LeptonClass2,
                                  self.handles['leptons2'].product() )
            nL += len(event.leptons2)

        if nL < self.cfg_ana.nLeptonsMin:
            return False

        self.counters.counter('FourLepton').inc('enough leptons')        

        event.selLeptons1 = filter( self.testLepton1,
                                    event.leptons1 )
        event.allSelLeptons = copy.copy( event.selLeptons1 )        
        if self.__class__.LeptonClass2 != self.__class__.LeptonClass1:
            event.selLeptons2 = filter( self.testLepton2,
                                        event.leptons2 )
            event.allSelLeptons.extend( event.selLeptons2 ) 

        nL = len( event.allSelLeptons )
        if nL < self.cfg_ana.nLeptonsMin:
            return False

        self.counters.counter('FourLepton').inc('enough good leptons')

        event.leptonPairs = self.findPairs( event.allSelLeptons )
        event.selZBosons1 = filter( self.testZ1, event.leptonPairs )
        if len(event.selZBosons1)<1:
            return False
        self.counters.counter('FourLepton').inc('Z1 found')
        
        event.zBoson1 = self.bestZBoson( event.selZBosons1 )
        
        event.leptonsAfterZ1 = copy.copy(event.allSelLeptons)
        event.leptonsAfterZ1.remove( event.zBoson1.leg1 ) 
        event.leptonsAfterZ1.remove( event.zBoson1.leg2 )

        if len(event.leptonsAfterZ1)==0:
            return False

        self.counters.counter('FourLepton').inc('3rd lepton present')
        
        event.leptonPairs2 = self.findPairs( event.leptonsAfterZ1 )
        if len(event.leptonPairs2)==0:
            return False
        self.counters.counter('FourLepton').inc('2nd lepton pair')
        
        event.selZBosons2 = filter( self.testZ2, event.leptonPairs2 )
        if len(event.selZBosons2)==0:
            return False
        self.counters.counter('FourLepton').inc('Z2 found')

        
        event.allLeptonPairs = copy.copy( event.selZBosons2 )
        event.allLeptonPairs.append( event.zBoson1 ) 

        event.fourLeptons = self.findPairs( event.allLeptonPairs )
        event.selFourLeptons = filter( self.testFourLepton, event.fourLeptons)
        if len( event.selFourLeptons )==0:
            return False 
        self.counters.counter('FourLepton').inc('4-lepton selected')
        
        event.higgsCand = self.bestFourLepton( event.selFourLeptons ) 
        event.zBoson2 = event.higgsCand.leg1
        if event.zBoson2 == event.zBoson1:
            event.zBoson2 = event.higgsCand.leg2

        event.theLeptons = [
            event.zBoson1.leg1,
            event.zBoson1.leg2,
            event.zBoson2.leg1,
            event.zBoson2.leg2
            ]

        event.theLeptons_ptSorted = sorted( event.theLeptons,
                                            key = lambda x: 1/x.pt() )
        
        self.counters.counter('FourLepton').inc('passing')

        return True


    def bestFourLepton( self, fourLeptons ):
        if len( fourLeptons ) == 0:
            return None
        elif len( fourLeptons ) == 1:
            return fourLeptons[0]
        else:
            return max( fourLeptons, key = lambda x: x.leg1.pt() + x.leg2.pt() )
            

    def bestZBoson(self, zBosons):
        if len( zBosons ) == 0:
            return None
        elif len( zBosons ) == 1:
            return zBosons[0]
        else:
            # taking the closest from the Z mass
            zMass = 91.2
            return min(zBosons, key = lambda x: abs( x.mass() - zMass) )
             
    
    def findPairs(self, leptons):
        out = []
        for l1, l2 in itertools.combinations(leptons, 2):
            out.append( DiObject(l1, l2) )
        return out


    def testFourLepton(self, fourLepton):
        '''Need to overload this function for the 4e and 4mu analyzers:
        see pas p3, point 4
        '''
        return fourLepton.M()>self.cfg_ana.h_m

    
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
        # return muon.getSelection('cuts_vbtfmuon')
        return True
    
    def testElectron(self, electron):
        '''Returns True if an electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        return True

