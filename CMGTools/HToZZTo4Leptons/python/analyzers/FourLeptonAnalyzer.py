import operator 
import itertools
import copy

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched


class DiObject( TLorentzVector ):
    '''Class used for Zs, and also for Higgs candidates'''
    def __init__(self, leg1, leg2):
        lv1 = TLorentzVector( leg1.px(), leg1.py(), leg1.pz(), leg1.energy() )
        lv2 = TLorentzVector( leg2.px(), leg2.py(), leg2.pz(), leg2.energy() )
        lv1 += lv2 
        super( DiObject, self).__init__( lv1 )
        self.leg1 = leg1
        self.leg2 = leg2

    def __getattr__(self, name):
        '''Trick to preserve the interface in use in CMSSW.'''
        if name.lower() == 'mass':
            name = 'M'
        # changing the first letter of the function name to upper case. 
        capName = ''.join( [name[0].capitalize(), name[1:]] ) 
        # return getattr( super(DiObject, self), capName )
        return getattr( self, capName )

    def PdgId(self):
        '''Dummy, needed to fill the tree'''
        return 23

    def Sip3D(self):
        '''Dummy, needed to fill the tree'''
        return -1

    def RelIso(self, dBetaCor):
        '''Sum of the relative isolation (dbeta corrected) of the 2 legs'''
        return self.leg1.relIso( dBetaCor ) + self.leg2.relIso(dBetaCor )
        
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

        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.counters.counter('FourLepton').inc('all events')
        event.step = 0

        # creating a Lepton for each lepton in the first input collection
        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )
        # nL = len(event.leptons1)

        # if self.__class__.LeptonClass2 != self.__class__.LeptonClass1:
        # .. and in the second input collection, if it is different
        # from the first one
        event.leptons2 = map( self.__class__.LeptonClass2,
                              self.handles['leptons2'].product() )
        # nL += len(event.leptons2)

        event.allLeptons = set( event.leptons1 )
        event.allLeptons.update( event.leptons2 )

        nL = len( event.allLeptons )
        # first preselection, just to speed up the process
        if nL < self.cfg_ana.nLeptonsMin:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('enough leptons')        
            event.step += 1
            
        # loose lepton selection
        event.selLeptons1 = filter( self.testLepton1,
                                    event.leptons1 )
        event.selLeptons2 = filter( self.testLepton2,
                                    event.leptons2 )
        
        event.allSelLeptons = set( event.selLeptons1 )
        event.allSelLeptons.update( event.selLeptons2 )

        nL = len( event.allSelLeptons )
        if nL < self.cfg_ana.nLeptonsMin:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('enough good leptons')
            event.step += 1

        event.leptonPairs = self.findPairs( event.allSelLeptons )
        event.selZBosons1 = filter( self.testZ1, event.leptonPairs )
        if len(event.selZBosons1)<1:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('Z1 found')
            event.step += 1
        
        event.zBoson1 = self.bestZBoson( event.selZBosons1 )
        
        event.leptonsAfterZ1 = copy.copy(event.allSelLeptons)
        event.leptonsAfterZ1.remove( event.zBoson1.leg1 ) 
        event.leptonsAfterZ1.remove( event.zBoson1.leg2 )

        if len(event.leptonsAfterZ1)==0:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('3rd lepton present')
            event.step += 1
        
        event.leptonPairs2 = self.findPairs( event.leptonsAfterZ1 )
        if len(event.leptonPairs2)==0:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('2nd lepton pair')
            event.step += 1
        
        event.selZBosons2 = filter( self.testZ2, event.leptonPairs2 )
        if len(event.selZBosons2)==0:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('Z2 found')
            event.step += 1

        # putting together all selected lepton pairs:
        # Z1
        # selected Z2s
        event.allLeptonPairs = copy.copy( event.selZBosons2 )
        event.allLeptonPairs.append( event.zBoson1 ) 

        # looking for 2-Z combinations
        event.fourLeptons = self.findPairs( event.allLeptonPairs )
        event.selFourLeptons = filter( self.testFourLepton, event.fourLeptons)
        if len( event.selFourLeptons )==0:
            return self.cfg_ana.keep 
        else:
            self.counters.counter('FourLepton').inc('4-lepton selected')
            event.step += 1
        
        event.higgsCand = self.bestFourLepton( event.selFourLeptons ) 
        event.zBoson2 = event.higgsCand.leg1
        if event.zBoson2 == event.zBoson1:
            event.zBoson2 = event.higgsCand.leg2

        # leptons organized by the Z they belong to
        event.theLeptons = [
            event.zBoson1.leg1,
            event.zBoson1.leg2,
            event.zBoson2.leg1,
            event.zBoson2.leg2
            ]

        # leptons sorted by decreasing pT 
        event.theLeptons_ptSorted = sorted( event.theLeptons,
                                            key = lambda x: 1/x.pt() )
        
        self.counters.counter('FourLepton').inc('passing')
        event.step += 1

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

    def testLeptonPairIso(self, lep1, lep2, max):
        '''Combined isolation for a lepton pair.'''
        dBetaCor = 0.5
        return lep1.relIso(dBetaCor) + lep2.relIso(dBetaCor) < max
    
    def testZ1(self, zCand):
        '''Still missing the SIP3d cut'''
        return zCand.M() > self.cfg_ana.z1_m[0] and \
               zCand.M() < self.cfg_ana.z1_m[1] and \
               zCand.leg1.pt() > self.cfg_ana.z1_pt1 and \
               zCand.leg2.pt() > self.cfg_ana.z1_pt2 and \
               abs(zCand.leg1.pdgId()) == abs(zCand.leg2.pdgId()) and \
               self.testLeptonPairIso( zCand.leg1, zCand.leg2, self.cfg_ana.pair_iso ) and \
               zCand.charge() == 0 


    def testZ2(self, zCand):
        return zCand.M() > self.cfg_ana.z2_m and \
               zCand.leg1.pt() > self.cfg_ana.z2_pt1 and \
               zCand.leg2.pt() > self.cfg_ana.z2_pt2 and \
               abs(zCand.leg1.pdgId()) == abs(zCand.leg2.pdgId()) and \
               zCand.charge() == 0


    def testLepton(self, lepton, pt, eta, iso, sip, sel=None):
        if sel is not None and \
           not lepton.getSelection(sel):
            # a cut string has to be tested, and the lepton does not pass
            return False
        # patLepton = lepton.sourcePtr()
        if lepton.pt() > pt and \
           abs(lepton.eta()) < eta and \
           lepton.relIso( 0.5 ) < iso and \
           lepton.sip3D() < sip:
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
                                sip = self.cfg_ana.sip1,
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
                                sip = self.cfg_ana.sip2,
                                sel = sel )


    
    def testMuon(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        # this is the id of the HZZ baseline:
        return muon.getSelection('cuts_vbtfmuon_numberOfValidTrackerHits')
        # return muon.getSelection('cuts_vbtfmuon')
        # return True
    
    def testElectron(self, electron):
        '''Returns True if an electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        return electron.getSelection('cuts_tightID')

