import operator
import copy
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Lepton
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched

class WAnalyzer( Analyzer ):

    LeptonClass = Lepton 

    def beginLoop(self):
        super(WAnalyzer,self).beginLoop()
        self.counters.addCounter('WAna')
        count = self.counters.counter('WAna')
        count.register('all events')
        count.register('>= 1 lepton')
        count.register('leg offline cuts passed')
        count.register('leg trig matched')
        count.register('pass')
        

    def buildLeptons(self, cmgLeptons, event):
        '''Creates python Leptons from the leptons read from the disk.
        to be overloaded if needed.'''
        return map( self.__class__.LeptonClass, cmgLeptons )
    
        
    def process(self, iEvent, event):
        # access di-object collection
        # test first leg
        # test second leg
        # test di-lepton
        # apply lepton veto
        # choose best di-lepton
        # put in the event
        self.readCollections( iEvent )
        # trigger stuff could be put in a separate analyzer
        # event.triggerObject = self.handles['cmgTriggerObjectSel'].product()[0]
        event.leptons = self.buildLeptons( self.handles['leptons'].product(), event )
        # import pdb; pdb.set_trace()
        # self.shiftEnergyScale(event)
        return self.selectionSequence(event, fillCounter=True)


##     def shiftEnergyScale(self, event):
##         scaleShift1 = None
##         scaleShift2 = None
##         if hasattr( self.cfg_ana, 'scaleShift1'):
##             scaleShift1 = self.cfg_ana.scaleShift1
##         if hasattr( self.cfg_ana, 'scaleShift2'):
##             scaleShift2 = self.cfg_ana.scaleShift2
##         if scaleShift1:
##             # import pdb; pdb.set_trace()
##             map( lambda x: x.leg1().scaleEnergy(scaleShift1), event.diLeptons )
##         if scaleShift2:
##             map( lambda x: x.leg2().scaleEnergy(scaleShift2), event.diLeptons )
##             map( lambda x: x.scaleEnergy(scaleShift2), event.leptons )
        

    def selectionSequence(self, event, fillCounter, legIsoCut=None):

        if fillCounter: self.counters.counter('WAna').inc('all events')
        # if not self.triggerList.triggerPassed(event.triggerObject):
        #    return False
        # self.counters.counter('WAna').inc('trigger passed ')

        # if event.eventId == 155035:
        #    import pdb; pdb.set_trace()

        event.selLeptons = copy.copy(event.leptons)
            
        if len(event.selLeptons) == 0:
            return False
        if fillCounter: self.counters.counter('WAna').inc('>= 1 lepton')
  
        # testing offline cuts
        event.selLeptons = [ lep for lep in event.selLeptons if \
                             self.testLeg( lep, legIsoCut ) ]
        if len(event.selLeptons) == 0:
            return False
        else:
            if fillCounter: self.counters.counter('WAna').inc('leg offline cuts passed')

        self.counters.counter('WAna').inc('pass')
        return True
            
##         if len(self.cfg_comp.triggers)>0:
##             # trigger matching leg1
##             selDiLeptons = [diL for diL in selDiLeptons if \
##                             self.trigMatched(event, diL.leg1(), 'leg1')]
##             if len(selDiLeptons) == 0:
##                 return False, 'leg1 trigger matching failed'
##             else:
##                 if fillCounter: self.counters.counter('WAna').inc('leg1 trig matched')

##         # testing leg2 
##         selDiLeptons = [ diL for diL in selDiLeptons if \
##                          self.testLeg2( diL.leg2(), leg2IsoCut ) ]
##         if len(selDiLeptons) == 0:
##             return False, 'leg2 offline cuts failed'
##         else:
##             if fillCounter: self.counters.counter('WAna').inc('leg2 offline cuts passed')

##         if len(self.cfg_comp.triggers)>0:
##             # trigger matching leg2
##             selDiLeptons = [diL for diL in selDiLeptons if \
##                             self.trigMatched(event, diL.leg2(), 'leg2')]
##             if len(selDiLeptons) == 0:
##                 return False, 'leg2 trigger matching failed'
##             else:
##                 if fillCounter: self.counters.counter('WAna').inc('leg2 trig matched')

##         # mass cut 
##         selDiLeptons = [ diL for diL in selDiLeptons if \
##                          self.testMass(diL) ]
##         if len(selDiLeptons)==0:
##             return False, 'mass cut failed'
##         else:
##             if fillCounter: self.counters.counter('WAna').inc(
##                 '{min:3.1f} < m < {max:3.1f}'.format( min = self.cfg_ana.m_min,
##                                                       max = self.cfg_ana.m_max )
##                 )

##         # exactly one? 
##         if len(selDiLeptons)==0:
##             return False
##         elif len(selDiLeptons)==1:
##             if fillCounter: self.counters.counter('WAna').inc('exactly 1 di-lepton')
        
##         event.diLepton = self.bestDiLepton( selDiLeptons )
##         event.leg1 = event.diLepton.leg1()
##         event.leg2 = event.diLepton.leg2()
            
    

    def declareHandles(self):        
        super(WAnalyzer, self).declareHandles()
        self.handles['cmgTriggerObjectSel'] =  AutoHandle(
            'cmgTriggerObjectSel',
            'std::vector<cmg::TriggerObject>'
            )
        self.handles['leptons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )


    def testLeg(self, leg, isocut=None):
        '''returns testLegID && testLegIso && testLegKine for leg'''
        return self.testLegID(leg) and \
               self.testLegIso(leg, isocut) and \
               self.testLegKine(leg, self.cfg_ana.pt, self.cfg_ana.eta)


    def testLegKine(self, leg, ptcut, etacut ):
        '''Tests pt and eta.'''
        return leg.pt() > ptcut and \
               abs(leg.eta()) < etacut 

    
    def testLegID(self, leg):
        '''Always return true by default, overload in your subclass'''
        return True

    
    def testLegIso(self, leg, isocut):
        '''If isocut is None, the iso value is taken from the iso1 parameter.
        Checks the standard dbeta corrected isolation.
        '''
        if isocut is None:
            isocut = self.cfg_ana.iso
        return leg.relIso(0.5) < isocut


    def trigMatched(self, event, leg, legName):
        '''Returns true if the leg is matched to a trigger object as defined in the
        triggerMap parameter'''
        if not hasattr( self.cfg_ana, 'triggerMap'):
            return True
        path = event.hltPath
        triggerObjects = event.triggerObjects
        filters = self.cfg_ana.triggerMap[ path ]
        filter = None
        if legName == 'leg1':
            filter = filters[0]
        elif legName == 'leg2':
            filter = filters[1]
        else:
            raise ValueError( 'legName should be leg1 or leg2, not {leg}'.format(
                leg=legName )  )
        # the dR2Max value is 0.3^2
        pdgIds = None
        if len(filter) == 2:
            filter, pdgIds = filter[0], filter[1]
        return triggerMatched(leg, triggerObjects, path, filter,
                              # dR2Max=0.089999,
                              dR2Max=0.25,
                              pdgIds=pdgIds )
