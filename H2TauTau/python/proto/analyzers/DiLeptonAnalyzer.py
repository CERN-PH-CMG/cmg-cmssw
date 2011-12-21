import operator 
from CMGTools.H2TauTau.proto.framework.Analyzer import Analyzer
from CMGTools.H2TauTau.proto.statistics.Counter import Counter, Counters
from CMGTools.H2TauTau.proto.framework.AutoHandle import AutoHandle
from CMGTools.H2TauTau.proto.physicsobjects.DiObject import DiObject
from CMGTools.H2TauTau.proto.physicsobjects.PhysicsObjects import Lepton

class DiLeptonAnalyzer( Analyzer ):

    DiObjectClass = DiObject
    LeptonClass = Lepton 

    def beginLoop(self):
        super(DiLeptonAnalyzer,self).beginLoop()
        self.counters.addCounter('DiLepton')
        
    def process(self, iEvent, event):
        # access di-object collection
        # test first leg
        # test second leg
        # test di-lepton
        # apply lepton veto
        # choose best di-lepton
        # put in the event
        self.readCollections( iEvent )
        event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        event.diLeptons = map( self.__class__.DiObjectClass, self.handles['diLeptons'].product() )
        event.leptons = map( self.__class__.LeptonClass, self.handles['leptons'].product() ) 

        self.counters.counter('DiLepton').inc('all events ')
        if not self.triggerList.triggerPassed(event.triggerObject):
            return False
        self.counters.counter('DiLepton').inc('trigger passed ')
            
        if len(event.diLeptons) == 0:
            return False
        self.counters.counter('DiLepton').inc('> 0 di-lepton ')
        if not self.leptonAccept( event.leptons ):
            return False
        self.counters.counter('DiLepton').inc('lepton accept ')
        selDiLeptons = []
        for diLepton in event.diLeptons:
            if self.testLeg1( diLepton.leg1() ) and \
               self.testLeg2( diLepton.leg2() ) :
                selDiLeptons.append( diLepton )
        if len(selDiLeptons) == 0:
            return False
        self.counters.counter('DiLepton').inc('legs 1 & 2 ok ')
        event.diLepton = self.bestDiLepton( selDiLeptons )

        mass = event.diLepton.mass()
        if self.cfg_ana.m_min < mass and mass < self.cfg_ana.m_max:
            self.counters.counter('DiLepton').inc(
                '{min:3.1f} < m < {max:3.1f}'.format( min = self.cfg_ana.m_min,
                                                          max = self.cfg_ana.m_max )
                )
        else:
            return False
        return True

    def declareHandles(self):        
        super(DiLeptonAnalyzer, self).declareHandles()
        self.handles['cmgTriggerObjectSel'] =  AutoHandle(
            'cmgTriggerObjectSel',
            'std::vector<cmg::TriggerObject>'
            )
    
    def leptonAccept(self, leptons):
        '''Should implement a default version running on event.leptons.'''
        return True

    def testLeg(self, leg, pt, eta, iso):
        if leg.pt()>pt and \
           abs(leg.eta())<eta and \
           leg.relIso( 0.5 ) < iso:
            return True
        else:
            return False
    
    def testLeg1(self, leg):
        '''Overload according to type.
        Could provide a default test of pt,eta,charge...'''
        return self.testLeg( leg,
                             pt = self.cfg_ana.pt1,
                             eta = self.cfg_ana.eta1,
                             iso = self.cfg_ana.iso1 )

    def testLeg2(self, leg):
        '''Overload according to type.
        Could provide a default test of pt,eta,charge...'''
        return self.testLeg( leg,
                             pt = self.cfg_ana.pt2,
                             eta = self.cfg_ana.eta2,
                             iso = self.cfg_ana.iso2 )
        

    def testMuon(self, muon):
        '''Returns True if a muon passes a set of cuts.
        Can be used in testLeg1 and testLeg2, in child classes.'''
        return True
    
    def testElectron(self, electron):
        '''Returns True if an electron passes a set of cuts.
        Can be used in testLeg1 and testLeg2, in child classes.'''
        return True

    def testTau(self, tau):
        '''Returns True if a tau passes a set of cuts.
        Can be used in testLeg1 and testLeg2, in child classes.'''
        return True
    
    def bestDiLepton(self, diLeptons):
        '''Returns the best diLepton (the one with highest pt1 + pt2).'''
        return max( diLeptons, key=operator.methodcaller( 'sumPt' ) )
    
