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

from CMGTools.HToZZTo4Leptons.analyzers.DiObject import DiObject
        
class FourLeptonAnalyzer( Analyzer ):

    LeptonClass1 = Lepton 
    LeptonClass2 = Lepton

    def declareHandles(self):
        super(FourLeptonAnalyzer, self).declareHandles()

        self.handles['rho'] = AutoHandle( ('kt6PFJetsForIso', 'rho'),
                                          'double')

    def beginLoop(self):
        super(FourLeptonAnalyzer,self).beginLoop()
        self.counters.addCounter('FourLepton')
        count = self.counters.counter('FourLepton')
        count.register('all events')
        # count.register('>={nL} leptons'.format(nL=self.cfg_ana.nLeptonsMin))
        # count.register('>={nL} good leptons'.format(nL=self.cfg_ana.nLeptonsMin))
        count.register('Z1 found')
        count.register('Z1+l')
        # count.register('2nd lepton pair')
        count.register('Z2 found (Presel)')
        count.register('Iso')
        count.register('SIP')
        # count.register('4-lepton selected')
        count.register('passing')
        

    def buildLeptonList(self, event):
        event.leptons1 = map( self.__class__.LeptonClass1,
                              self.handles['leptons1'].product() )
        if self.__class__.LeptonClass1 != self.__class__.LeptonClass2: 
            event.leptons2 = map( self.__class__.LeptonClass2,
                                  self.handles['leptons2'].product() )
        else:
            event.leptons2 = event.leptons1

       
    def process(self, iEvent, event):
        self.readCollections( iEvent )

        # creating a "sub-event" for this analyzer
        myEvent = Event(event.iEv)
        setattr(event, self.name, myEvent)
        event = myEvent
        
        self.counters.counter('FourLepton').inc('all events')
        event.step = 0

        self.buildLeptonList( event )   
        event.allLeptons = set( event.leptons1 )
        event.allLeptons.update( event.leptons2 )
    
        nL = len( event.allLeptons )
        # first preselection, just to speed up the process
        if nL < self.cfg_ana.nLeptonsMin:
            return self.cfg_ana.keep
        else:
            # self.counters.counter('FourLepton').inc('>={nL} leptons'.format(nL=self.cfg_ana.nLeptonsMin))        
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
            # self.counters.counter('FourLepton').inc('>={nL} good leptons'.format(nL=self.cfg_ana.nLeptonsMin))
            event.step += 1

        event.leptonPairs = self.findPairs( event.allSelLeptons )
        event.selZBosons1 = filter( self.testZ1, event.leptonPairs )
        if len(event.selZBosons1)<1:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('Z1 found')
            event.step += 1
        
        event.zBoson1 = self.bestZBoson1( event.selZBosons1 )
        
        event.leptonsAfterZ1 = copy.copy(event.allSelLeptons)
        event.leptonsAfterZ1.remove( event.zBoson1.leg1 ) 
        event.leptonsAfterZ1.remove( event.zBoson1.leg2 )

        if len(event.leptonsAfterZ1)==0:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('Z1+l')
            event.step += 1
        
        event.leptonPairs2 = self.findPairs( event.leptonsAfterZ1 )
        if len(event.leptonPairs2)==0:
            return self.cfg_ana.keep
        else:
            # self.counters.counter('FourLepton').inc('2nd lepton pair')
            event.step += 1
        
        event.selZBosons2 = filter( self.testZ2, event.leptonPairs2 )
        if len(event.selZBosons2)==0:
            return self.cfg_ana.keep
        else:
            self.counters.counter('FourLepton').inc('Z2 found (Presel)')
            event.step += 1

        event.zBoson2 = self.bestZBoson2( event.selZBosons2 )

        event.higgsCand = DiObject( event.zBoson1, event.zBoson2 )

        #FIXME: need to apply isolation cut and SIP cut

        event.theLeptons = [
            event.zBoson1.leg1,
            event.zBoson1.leg2,
            event.zBoson2.leg1,
            event.zBoson2.leg2
            ]

        # leptons sorted by decreasing pT 
        event.theLeptons_ptSorted = sorted( event.theLeptons,
                                            key = lambda x: 1/x.pt() )


        for l1, l2 in itertools.combinations( event.theLeptons, 2 ):
            event.leptonPairIso = self.testLeptonPairIso(l1, l2, self.cfg_ana.pair_iso)
            if not event.leptonPairIso:
                return self.cfg_ana.keep
        self.counters.counter('FourLepton').inc('Iso')


        event.theLeptonsSIP = filter( lambda x: x.sip3D()<self.cfg_ana.h_sip,
                                      event.theLeptons)
        if len(event.theLeptonsSIP)<4:
            return self.cfg_ana.keep
        self.counters.counter('FourLepton').inc('SIP')
        
        self.counters.counter('FourLepton').inc('passing')
        event.step += 1

        # import pdb; pdb.set_trace()
        
        return True
 
    def bestZBoson2(self, zBosons):
        '''To be implemented!'''
        best = max(zBosons, key=lambda x: x.leg1.pt() + x.leg2.pt() )        
        return best

    def bestZBoson1(self, zBosons):
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
        if fourLepton.M()<self.cfg_ana.h_m:
            return False

        
        assert( abs( fourLepton.leg1.leg1.pdgId() ) == \
                abs( fourLepton.leg1.leg2.pdgId() ) ) 

        assert( abs( fourLepton.leg2.leg1.pdgId() ) == \
                abs( fourLepton.leg2.leg2.pdgId() ) ) 

        if abs( fourLepton.leg1.leg1.pdgId() ) != \
           abs( fourLepton.leg2.leg1.pdgId() ):
            # 2e 2mu we're all set
            return True

        
        # check that at least 3 of the 4 combinations of 2 leptons
        # have a mass larger than self.cfg_ana.pair_mass
        leptons = [fourLepton.leg1.leg1,
                   fourLepton.leg1.leg2,
                   fourLepton.leg2.leg1,
                   fourLepton.leg2.leg2]
        nLowMass = 0
        nPairs = 0
        for l1, l2 in itertools.combinations(leptons, 2):
            if l1.charge()+l2.charge()!=0:
                continue
            if abs(l1.pdgId()) != abs(l2.pdgId()):
                continue
            # considering only the pairs of SF OS leptons (possible Zs)
            nPairs += 1
            pair = DiObject( l1, l2 )
            if pair.M()<self.cfg_ana.pair_mass:
                nLowMass += 1
        if nPairs!=4:
            raise ValueError('number of pairs should be equal to 4!')
        if nLowMass>1:
            return False
        
        return True

    def testLeptonPairIso(self, lep1, lep2, max):
        '''Combined isolation for a lepton pair.'''
        return self.leptonIso(lep1) + self.leptonIso(lep2) < max
    
    def testZ1(self, zCand):
        '''Still missing the SIP3D cut'''
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
        #FIXME: I should use charged hadron iso as a loose iso!
        if sel is not None and \
           not lepton.getSelection(sel):
            # a cut string has to be tested, and the lepton does not pass
            return False
        # patLepton = lepton.sourcePtr()
        if lepton.pt() > pt and \
           abs(lepton.eta()) < eta and \
           self.leptonIso( lepton ) < iso and \
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
        # print muon.sourcePtr().track().pt()
        if self.cfg_ana.PF:
            return True
        else:
            return muon.numberOfValidTrackerHits() > 10
        # return True
        
    
    def testElectron(self, electron):
        '''Returns True if an electron passes a set of cuts.
        Can be used in testLepton1 and testLepton2, in child classes.'''
        return electron.getSelection('cuts_tightID')


    def leptonIso( self, lepton ):
        # delta beta corrected isolation 
        # return lepton.relIso(0.5)
        # if abs(lepton.pdgId()) == 13:
        if self.cfg_ana.PFIso is True:
            return lepton.relIso( 0.5 )
        else:
            rho = self.handles['rho'].product()[0]
            return lepton.detIso( rho )


##     def muDetIso(self, patMuon):
##         ''' from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Torino/ZZAnalysis/AnalysisStep/plugins/ZZ4lAnalyzer.cc?revision=1.287&view=markup
##         '''
##         isoEcal = patMuon.ecalIso()
##         isoHcal = patMuon.hcalIso()
##         isoTk   = patMuon.userIsolation( 7 )
##         isoEcal, isoHcal = self.rhoCorrMu(patMuon.eta(),isoEcal, isoHcal)
##         return (isoEcal + isoHcal + isoTk)/patMuon.pt()


##     def rhoCorrMu(self, eta, ecalIso, hcalIso):
##         AreaEcal = [0.074, 0.045] # barrel/endcap 
##         AreaHcal = [0.022 , 0.030] # barrel/endcap
##         ifid = 1 
##         if abs( eta ) < 1.479:
##             ifid = 0 # selecting barrel settings
##         rho = self.handles['rho'].product()[0]
##         ecalIso = ecalIso - AreaEcal[ifid] * rho
##         hcalIso = hcalIso - AreaHcal[ifid] * rho
##         return ecalIso, hcalIso

