import operator
import copy
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Jet, GenParticle
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched
from CMGTools.RootTools.utils.DeltaR import bestMatch, deltaR, deltaR2

class WAnalyzer( Analyzer ):

    MuonClass = Muon 
    JetClass = Jet 
    
    def beginLoop(self):
        super(WAnalyzer,self).beginLoop()
        self.counters.addCounter('WAna')
        count = self.counters.counter('WAna')
        count.register('W all events')
        count.register('W >= 1 lepton')
        count.register('W at least 1 lep trig matched')
        count.register('W only 1 lep trig matched')
        count.register('W pass')

    def buildLeptons(self, cmgMuons, event):
        '''Creates python Leptons from the muons read from the disk.
        to be overloaded if needed.'''
        return map( self.__class__.MuonClass, cmgMuons )

    def buildJets(self, cmgJets, event):
        '''Creates python Jets from the Jets read from the disk.
        to be overloaded if needed.'''
        return map( self.__class__.JetClass, cmgJets )
    
    def buildGenParticles(self, cmgGenParticles, event):
        '''Creates python GenParticles from the di-leptons read from the disk.
        to be overloaded if needed.'''
        return map( GenParticle, cmgGenParticles )

        
    def declareVariables(self):
        tr = self.tree
        var( tr, 'pfmet')
    
    def process(self, iEvent, event):
        # access event
        self.readCollections( iEvent )
        # access muons
        event.muons = self.buildLeptons( self.handles['muons'].product(), event )
        # access jets
        event.jets = self.buildJets( self.handles['jets'].product(), event )
        # access MET
        event.pfmet = self.handles['pfmet'].product()[0]
        # access genP
        event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )        
        # define good event bool
        event.WGoodEvent = False
        # select event
        return self.selectionSequence(event, fillCounter=True )

        
    def selectionSequence(self, event, fillCounter):

        if fillCounter: self.counters.counter('WAna').inc('W all events')

        # retrieve collections of interest (muons and jets)
        event.allMuons = copy.copy(event.muons)
        event.selMuons = copy.copy(event.muons)
        event.allJets = copy.copy(event.jets)
        event.selJets = copy.copy(event.jets)

        # check if the event is MC and if genp must be saved
        event.savegenpW=True
        if not (self.cfg_ana.savegenp and self.cfg_comp.isMC):
          event.savegenpW=False
        
        # save genp only for signal events
        # i.e. only one W is present and daughters are muon plus neutrino
        genW_dummy = [ genp for genp in event.genParticles if \
                              math.fabs(genp.pdgId())==24 ]
        if len(genW_dummy)==1:
          event.genW = [ genp for genp in genW_dummy if \
                               ( \
                                ( math.fabs(genW_dummy[0].daughter(0).pdgId())==13 ) or \
                                ( math.fabs(genW_dummy[0].daughter(1).pdgId())==13 ) \
                                ) ]
          # if the genp event is selected, associate gen muon and neutrino
          event.genMu = []
          event.genNu = []
          if len(event.genW)==1:
            if [ math.fabs(event.genW[0].daughter(0).pdgId())==13 ]:
              event.genMu.append(event.genW[0].daughter(0))
              event.genNu.append(event.genW[0].daughter(1))
            else:
              event.genMu.append(event.genW[0].daughter(1))
              event.genNu.append(event.genW[0].daughter(0))
            
            event.genW_mt = self.mT(event.genW[0].daughter(0).p4() , event.genW[0].daughter(1).p4())
            event.muGenDeltaRgenP=1e6
          
          else:
            # if the genp is not signal, don't save genp but do not exit 
            # -----> events which will pass the reconstruction but are not signal
            # can be considered as background (for example, in W+Jets, from W decaying into electrons, taus)
            event.savegenpW=False
        else:
          event.savegenpW=False
                
        # store event number of muons, MET and jets in all gen events (necessary to make cuts in genp studies...)
        # total number of reco muons
        event.nMuons=len(event.selMuons)
        # clean jets by removing muons
        event.selJets = [ jet for jet in event.allJets if not \
                                (bestMatch( jet , event.selMuons ))[1] <0.5
                        ]

        # reco events must have good reco vertex and trigger fired...
        if not (event.HasGoodVertices and event.HasTriggerFired):
          return True
        # ...and at lest one reco muon...
        if len(event.selMuons) == 0:
            return True
        if fillCounter: self.counters.counter('WAna').inc('W >= 1 lepton')
        
        #check if the event is triggered according to cfg_ana
        if len(self.cfg_comp.triggers)>0:
            # muon object trigger matching
            event.selMuons = [lep for lep in event.allMuons if \
                            self.trigMatched(event, lep)]
            # exit if there are no triggered muons
            if len(event.selMuons) == 0:
                return True, 'trigger matching failed'
            else:
                if fillCounter: self.counters.counter('WAna').inc('W at least 1 lep trig matched')
                
        # to select W impose only 1 triggering lepton in the event:
        # the number of triggering lepton is checked on the whole lepton collection
        # before any cut, otherwise could be a Z!!!
        if len(event.selMuons) != 1:
          return True, 'more than 1 lep trig matched'
        else:
            if fillCounter: self.counters.counter('WAna').inc('W only 1 lep trig matched')

        # if the genp are saved, compute dR between gen and reco muon 
        if event.savegenpW :
          event.muGenDeltaRgenP = deltaR( event.selMuons[0].eta(), event.selMuons[0].phi(), event.genMu[0].eta(), event.genMu[0].phi() ) 

        # associate good vertex to muon to compute dxy
        event.selMuons[0].associatedVertex = event.goodVertices[0]
                
        # testing offline muon cuts (tight+iso, no kinematical cuts)
        event.selMuonIsTightAndIso = self.testLeg( event.selMuons[0] ) 
          
        # define a W from lepton and MET
        event.W4V = event.selMuons[0].p4() + event.pfmet.p4()
        event.W4V_mt = self.mT(event.selMuons[0].p4() , event.pfmet.p4())        

        # Code to study the recoil (not very useful for W...)
        metVect = event.pfmet.p4().Vect()
        metVect.SetZ(0.) # use only transverse info
        WVect = event.W4V.Vect()
        WVect.SetZ(0.) # use only transverse info
        recoilVect = copy.deepcopy(metVect)
        recoilVect -= WVect
        
        uWVect = WVect.Unit()
        zAxis = type(WVect)(0,0,1)
        uWVectPerp = WVect.Cross(zAxis).Unit()

        u1 = - recoilVect.Dot(uWVect) # recoil parallel to W pt
        u2 = recoilVect.Dot(uWVectPerp) # recoil perpendicular to W pt

        event.u1 = u1
        event.u2 = u2

        # event is fully considered as good
        self.counters.counter('WAna').inc('W pass')
        event.WGoodEvent = True
        return True


    def declareHandles(self):        
        super(WAnalyzer, self).declareHandles()
        self.handles['cmgTriggerObjectSel'] =  AutoHandle(
            'cmgTriggerObjectSel',
            'std::vector<cmg::TriggerObject>'
            )
        self.handles['muons'] = AutoHandle(
            'cmgMuonSel',
            'std::vector<cmg::Muon>'
            )
        self.handles['jets'] = AutoHandle(
            'cmgPFJetSel',
            'std::vector<cmg::PFJet>'
            )
        self.handles['pfmet'] = AutoHandle(
          'cmgPFMET',
          'std::vector<cmg::BaseMET>' 
          )
        self.mchandles['genpart'] =  AutoHandle(
            'genParticlesPruned',
            'std::vector<reco::GenParticle>'
            )


    def testJet(self, jet):
        '''returns testjetID && testjetIso && testjetKine for jet'''
        return jet.pt() > self.cfg_ana.jetptcut

    def testLeg(self, leg):
        '''returns testLegID && testLegIso && testLegKine for leg'''
        return self.testLegID(leg) and \
               self.testLegIso(leg, self.cfg_ana.iso) 
               # and self.testLegKine(leg, self.cfg_ana.pt, self.cfg_ana.eta)

    
    def testLegID(self, leg):
        '''Always return true by default, overload in your subclass'''
        # return True
        return ( leg.tightId() and \
                 leg.dxy() < 0.2 and\
                 leg.dz() < 0.5 )

    
    def testLegIso(self, leg, isocut):
        '''If isocut is None, the iso value is taken from the iso1 parameter.
        Checks the standard dbeta corrected isolation.
        '''
        if isocut is None:
            isocut = self.cfg_ana.iso
        return leg.relIso(0.5) < isocut


    def testLegKine(self, leg, ptcut, etacut ):
        '''Tests pt and eta.'''
        return leg.pt() > ptcut and \
               abs(leg.eta()) < etacut 


    def mT(self, leg1, leg2):
        return math.sqrt( 2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) ) )


    def trigMatched(self, event, leg):
        '''Returns true if the leg is matched to a trigger object as defined in the
        triggerMap parameter'''
        if not hasattr( self.cfg_ana, 'triggerMap'):
            return True
        path = event.hltPath
        triggerObjects = event.triggerObjects
        filters = self.cfg_ana.triggerMap[ path ]
        filter = filters[0]
        # if legName == 'leg1':
            # filter = filters[0]
        # elif legName == 'leg2':
            # filter = filters[1]
        # else:
            # raise ValueError( 'legName should be leg1 or leg2, not {leg}'.format(
                # leg=legName )  )
        # the dR2Max value is 0.3^2
        pdgIds = None
        if len(filter) == 1:
            filter, pdgIds = filter[0], filter[1]
        return triggerMatched(leg, triggerObjects, path, filter,
                              # dR2Max=0.089999,
                              dR2Max=0.25,
                              pdgIds=pdgIds )


class WBoson(object):

        def __init__(self, lepton, met):
                self.lepton = lepton
                self.met = met
                self.mt = None
