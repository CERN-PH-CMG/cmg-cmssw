import operator
import copy
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Jet, GenParticle
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched
from CMGTools.RootTools.utils.DeltaR import bestMatch, deltaR, deltaR2
from CMGTools.WMass.analyzers.common_functions import *

class WAnalyzer( Analyzer ):

    MuonClass = Muon 
    JetClass = Jet 
    
    def beginLoop(self):
        super(WAnalyzer,self).beginLoop()
        self.counters.addCounter('WAna')
        count = self.counters.counter('WAna')
        count.register('W all events')
        count.register('W ev trig, good vertex and >= 1 lepton')
        count.register('W at least 1 lep trig matched')
        count.register('W only 1 lep trig matched')
        count.register('W non trg leading lepton pT < 10 GeV')
        count.register('W lep is MuIsTightAndIso')
        count.register('W Mu_eta<2.1 && Mu_pt>30')
        count.register('W pfmet>25')
        count.register('W pt<20')
        count.register('W Jet_leading_pt<30')
        
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

        
#    def declareVariables(self):
#        tr = self.tree
#        var( tr, 'pfmet')
    
    def process(self, iEvent, event):
        # access event
        self.readCollections( iEvent )
        # access muons
        event.muons = self.buildLeptons( self.handles['muons'].product(), event )
        # access jets
        event.jets = self.buildJets( self.handles['jets'].product(), event )
        # access MET
        event.pfmet = self.handles['pfmet'].product()[0]
        event.tkmet = self.handles['tkmet'].product()[0]
        # access genP
        event.genParticles = []
        event.LHEweights = []
        event.LHE_weights = []
        if self.cfg_comp.isMC :
          event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )        
          # import ROOT
          # objectsPF = [ j for j in event.genParticles if (j.status()==1 and math.fabs(j.pdgId())!=12 and math.fabs(j.pdgId())!=14 and math.fabs(j.pdgId())!=16) ]
          # objectsTK = [ j for j in event.genParticles if (j.charge()!=0 and j.status()==1 and math.fabs(j.eta())<2.5) ]
          # #for i in objectsPF:
          # #    print 'charge=',i.charge(),' status',i.status(),' pt',i.pt(),' pdg',i.pdgId()
          # event.genTkSumEt = sum([x.pt() for x in objectsTK])
          # event.genTkMet = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objectsTK])) , -1.*(sum([x.py() for x in objectsTK])), 0, math.hypot(-1.*sum([x.px() for x in objectsTK]),-1.*sum([x.py() for x in objectsTK])))
          # event.genPfSumEt = sum([x.pt() for x in objectsPF])
          # event.genPfMet = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objectsPF])) , -1.*(sum([x.py() for x in objectsPF])), 0, math.hypot(-1.*sum([x.px() for x in objectsPF]),-1.*sum([x.py() for x in objectsPF])))
          # #print 'genTkMet=',event.genTkMet.pt(),' genPfMet=',event.genPfMet.pt()
          event.LHEweights = []
          if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
            event.LHEweights = self.mchandles['LHEweights'].product()
            event.LHEweights_str = []
          # event.LHEweights = self.mchandles['LHEweights'].product()
        # define good event bool
        event.WGoodEvent = False
        # select event
        return self.selectionSequence(event, fillCounter=True )

        
    def selectionSequence(self, event, fillCounter):

        if fillCounter: self.counters.counter('WAna').inc('W all events')

        if self.cfg_comp.isMC :
          if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
            # print event.LHEweights.comments_size()
            for i in range(0,event.LHEweights.comments_size()):
              # print 'i',i,event.LHEweights.getComment(i).split()[0]
              if not "rwgt" in event.LHEweights.getComment(i).split()[0]:
                event.LHE_weights.append(float(event.LHEweights.getComment(i).split()[1])/float(event.LHEweights.getComment(206).split()[1])) # CHECK THE 216 FOR THE SAMPLE IN USE !!!
                # print len(event.LHE_weights)-1, event.LHEweights.getComment(i).split()[0], float(event.LHEweights.getComment(i).split()[1])/float(event.LHEweights.getComment(206).split()[1]), event.LHEweights.getComment(i).split()[2], event.LHEweights.getComment(i).split()[3], event.LHEweights.getComment(i).split()[4], event.LHEweights.getComment(i).split()[5]
        
        # print len(event.LHE_weights)

        ##------------------------  Initial declaration of vectors --------------------------------------  
            
        event.allMuonsTrgBit=[]
        # retrieve collections of interest (muons and jets)
        event.allMuons = copy.copy(event.muons)
        for i in range(0,len(event.allMuons)):
          event.allMuonsTrgBit.append(0)

        event.selMuons = []
        event.NoTriggeredMuonsLeadingPt = copy.copy(event.muons)
        event.allJets = copy.copy(event.jets)
        event.selJets = copy.copy(event.jets)

        event.selJets = [ jet for jet in event.allJets if ( \
                                        not (bestMatch( jet , event.muons ))[1] <0.5 \
                                        and jet.looseJetId() and jet.pt()>30 \
                                        )
                        ]

        ##------------------------  HERE MC related stuff --------------------------------------

        # check if the event is MC and if genp must be saved
        event.savegenpW=False
        
        # for genp in event.genParticles:
          # if math.fabs(genp.pdgId())==24 and genp.numberOfDaughters()>1:
            # print 'genp.pdgId()=',genp.pdgId(), 'genp.status()=',genp.status(), 'genp.numberOfDaughters()=',genp.numberOfDaughters()
            # if(genp.numberOfDaughters()>0):
              # print 'genp.daughter(0)',genp.daughter(0).pdgId(),'status',genp.daughter(0).status()
              # if(genp.numberOfDaughters()>1):
                # print 'genp.daughter(1)',genp.daughter(1).pdgId(),'status',genp.daughter(1).status()
        
        # return False
        
        # save genp only for signal events
        # i.e. only one W is present and daughters are muon plus neutrino
        genW_dummy = [ genp for genp in event.genParticles if \
                       (math.fabs(genp.pdgId())==24 and (self.cfg_ana.doMad or genp.status()==62))if \
                       ((  genp.numberOfDaughters()>1 and (\
                         math.fabs(genp.daughter(0).pdgId())==11 or 
                         math.fabs(genp.daughter(1).pdgId())==11 or 
                         math.fabs(genp.daughter(0).pdgId())==13 or 
                         math.fabs(genp.daughter(1).pdgId())==13 or 
                         math.fabs(genp.daughter(0).pdgId())==15 or 
                         math.fabs(genp.daughter(1).pdgId())==15
                         ) ) or (genp.numberOfDaughters()==1 and \
                         math.fabs(genp.daughter(0).pdgId())==13
                         ) ) ]

          # if the genp event is selected, associate gen muon and neutrino
        event.genMu = []
        event.genMuStatus1 = []
        event.genNu = []
        event.genNu_p4 = []
        event.genWLept = []

        if self.cfg_ana.savegenp and self.cfg_comp.isMC:
        
          if len(genW_dummy)==1:
            event.genW = [ genp for genp in genW_dummy if \
                                 ( \
                                  ( math.fabs(genW_dummy[0].daughter(0).pdgId())==13 ) or \
                                  ( genW_dummy[0].numberOfDaughters()>1 and math.fabs(genW_dummy[0].daughter(1).pdgId())==13 ) \
                                  ) ]
            if len(event.genW)==1:

              event.savegenpW=True
              event.genNu_p4 = genW_dummy[0].p4() - event.genW[0].daughter(0).p4()
              
              event.genWLept.append(event.genW[0])

              if [ math.fabs(event.genW[0].daughter(0).pdgId())==13 ]:
                event.genMu.append(event.genW[0].daughter(0))
                if event.genW[0].numberOfDaughters()>1:
                  event.genNu.append(event.genW[0].daughter(1))
              elif event.genW[0].numberOfDaughters()>1:
                event.genMu.append(event.genW[0].daughter(1))
                event.genNu.append(event.genW[0].daughter(0))
              
              if(len(event.genMu) >0):
                if(math.fabs(event.genW[0].mother(0).pdgId())!=6):
                  event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                else:
                  event.genMuStatus1.append(event.genMu[0])

              if event.genW[0].numberOfDaughters()>1:
                event.genW_mt = mT(self,event.genW[0].daughter(0).p4() , event.genW[0].daughter(1).p4())
              else: 
                event.genW_mt = mT(self,event.genW[0].daughter(0).p4() , event.genNu_p4)
              event.muGenDeltaRgenP=1e6

          # if len(genW_dummy)>1:
            # event.genW = [ genp for genp in genW_dummy if \
                                 # ( \
                                  # ( math.fabs(genW_dummy[0].daughter(0).pdgId())==11 ) or \
                                  # ( math.fabs(genW_dummy[0].daughter(1).pdgId())==11 ) or \
                                  # ( math.fabs(genW_dummy[0].daughter(0).pdgId())==13 ) or \
                                  # ( math.fabs(genW_dummy[0].daughter(1).pdgId())==13 ) or \
                                  # ( math.fabs(genW_dummy[0].daughter(0).pdgId())==15 ) or \
                                  # ( math.fabs(genW_dummy[0].daughter(1).pdgId())==15 ) or \
                                  # ( math.fabs(genW_dummy[1].daughter(0).pdgId())==11 ) or \
                                  # ( math.fabs(genW_dummy[1].daughter(1).pdgId())==11 ) or \
                                  # ( math.fabs(genW_dummy[1].daughter(0).pdgId())==13 ) or \
                                  # ( math.fabs(genW_dummy[1].daughter(1).pdgId())==13 ) or \
                                  # ( math.fabs(genW_dummy[1].daughter(0).pdgId())==15 ) or \
                                  # ( math.fabs(genW_dummy[1].daughter(1).pdgId())==15 ) \
                                  # ) ]

            # if len(event.genW)==2:
              # if ( math.fabs(event.genW[0].daughter(0).pdgId())==13. or math.fabs(event.genW[0].daughter(0).pdgId())==15. or math.fabs(event.genW[0].daughter(0).pdgId())==11. ):
                  # # print 'event.savegenpW 2 ',event.savegenpW
                  # event.savegenpW=False
  # #                print 'found leptonic W0 a'
                  # event.genWLept.append(event.genW[0])
                  # if ( math.fabs(event.genW[0].daughter(0).pdgId())==13 ):
                      # event.genMu.append(event.genW[0].daughter(0))
                      # event.genNu.append(event.genW[0].daughter(1))
                      # event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())
                      # event.muGenDeltaRgenP=1e6
                      # if(len(event.genMu) >0):
                          # event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      # event.savegenpW=True

              # if ( math.fabs(event.genW[0].daughter(1).pdgId())==13. or math.fabs(event.genW[0].daughter(1).pdgId())==15. or math.fabs(event.genW[0].daughter(1).pdgId())==11. ):
                  # # print 'event.savegenpW 3 ',event.savegenpW
                  # event.savegenpW=False
  # #                print 'found leptonic W0 b'
                  # event.genWLept.append(event.genW[0])
                  # if ( math.fabs(event.genW[0].daughter(1).pdgId())==13 ):
                      # event.genMu.append(event.genW[0].daughter(1))
                      # event.genNu.append(event.genW[0].daughter(0))
                      # event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())
                      # event.muGenDeltaRgenP=1e6
                      # if(len(event.genMu) >0):
                          # event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      # # print 'event.savegenpW 4 ',event.savegenpW
                      # event.savegenpW=True
                      
              # if ( math.fabs(event.genW[1].daughter(0).pdgId())==13. or math.fabs(event.genW[1].daughter(0).pdgId())==15. or math.fabs(event.genW[1].daughter(0).pdgId())==11. ):
                  # # print 'event.savegenpW 5 ',event.savegenpW
                  # event.savegenpW=False
  # #                print 'found leptonic W1 c'
                  # event.genWLept.append(event.genW[1])
                  # if ( math.fabs(event.genW[1].daughter(0).pdgId())==13 ):
                      # event.genMu.append(event.genW[1].daughter(0))
                      # event.genNu.append(event.genW[1].daughter(1))
                      # event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())                
                      # event.muGenDeltaRgenP=1e6
                      # if(len(event.genMu) >0):
                          # event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      # event.savegenpW=True
                      
              # if ( math.fabs(event.genW[1].daughter(1).pdgId())==13. or math.fabs(event.genW[1].daughter(1).pdgId())==15. or math.fabs(event.genW[1].daughter(1).pdgId())==11. ):
                  # # print 'event.savegenpW 6 ',event.savegenpW
                  # event.savegenpW=False
  # #                print 'found leptonic W1 d'
                  # event.genWLept.append(event.genW[1])
                  # if ( math.fabs(event.genW[1].daughter(1).pdgId())==13 ):
                      # event.genMu.append(event.genW[1].daughter(1))
                      # event.genNu.append(event.genW[1].daughter(0))
                      # event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())                
                      # event.muGenDeltaRgenP=1e6
                      # if(len(event.genMu) >0):
                          # event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      # event.savegenpW=True
                      
              # if the genp is not signal, don't save genp but do not exit 
              # -----> events which will pass the reconstruction but are not signal
              # can be considered as background (for example, in W+Jets, from W decaying into electrons, taus)

          # else:
            # ## here put false for fully hadronic WW  
            # print 'event.savegenpW 7 ',event.savegenpW
            # event.savegenpW=False
                
#        print 'genW found ', len(genW_dummy)
#        print 'genWLeptonic found ', len(event.genWLept)

        ##------------------------  HERE THERE is the selection --------------------------------------
        
        keepFailingEvents = True
        if not hasattr(self.cfg_ana,'keepFailingEvents') \
            or (hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents):
            keepFailingEvents = False
        # print 'keepFailingEvents',keepFailingEvents
        # if not (event.passedVertexAnalyzer and event.passedTriggerAnalyzer):
        ##------------
        # reco events must have good reco vertex
        # if not (event.passedVertexAnalyzer):
        if (not event.passedVertexAnalyzer) or (event.vertices[0] != event.goodVertices[0]):
          return keepFailingEvents
        ##------------
        # ...and at lest one reco muon...
        if len(event.muons) == 0:
            return keepFailingEvents
        if fillCounter: self.counters.counter('WAna').inc('W ev trig, good vertex and >= 1 lepton')

        #check if the event is triggered according to cfg_ana
        if hasattr(self.cfg_ana, 'triggerBits'):
          for imu in range(0,len(event.allMuons)):
            for T, TL in self.cfg_ana.triggerBits.iteritems():
                # muon object trigger matching
                if trigMatched(self, event, event.allMuons[imu], TL):
                  # print event.allMuons[imu].pt(), event.allMuons[imu].eta(), TL
                  event.allMuonsTrgBit[imu]=1
                  # continue
            if(event.allMuonsTrgBit[imu]==1): event.selMuons.append(event.allMuons[imu])
            # for T, TL in self.cfg_ana.triggerBits.iteritems():
                # # muon object trigger matching
                # event.selMuons = [lep for lep in event.allMuons if \
                                # trigMatched(self, event, lep, TL)]
          # print 'len(event.selMuons) = ',len(event.selMuons)
          # for i in range(0,len(event.selMuons)):
            # print i,event.selMuons[i].pt(), event.selMuons[i].eta()
                # exit if there are no triggered muons
          ##------------
          # reco events must have trigger fired...
          if len(event.selMuons) == 0:
              return keepFailingEvents, 'trigger matching failed'
          else:
              if fillCounter: self.counters.counter('WAna').inc('W at least 1 lep trig matched')
                
        ##------------
        # to select W impose only 1 triggering lepton in the event:
        # the number of triggering lepton is checked on the whole lepton collection
        # before any cut, otherwise could be a Z!!!
        if len(event.selMuons) != 1:
          return keepFailingEvents, 'more than 1 lep trig matched'
        else:
            if fillCounter: self.counters.counter('WAna').inc('W only 1 lep trig matched')

        if len(event.selMuons)!= 1: print 'BUT CONTINUING!'
        
        # print len(event.selMuons), event.selMuons[0].pt()
        if not (event.selMuons[0].pt()>0): return keepFailingEvents
        
        # store muons that did not fire the trigger
        if hasattr(self.cfg_ana, 'triggerBits'):
            for T, TL in self.cfg_ana.triggerBits.iteritems():
                event.NoTriggeredMuonsLeadingPt = [lep for lep in event.allMuons if \
                                                    (not trigMatched(self, event, lep, TL) \
                                                    and lep.pt()>10)]
        
        # print "len(event.NoTriggeredMuonsLeadingPt)= ",len(event.NoTriggeredMuonsLeadingPt)
        # if len(event.NoTriggeredMuonsLeadingPt)>0 : print "event.NoTriggeredMuonsLeadingPt[0].pt() = ",event.NoTriggeredMuonsLeadingPt[0].pt()

        if len(event.NoTriggeredMuonsLeadingPt) > 0:
          if event.NoTriggeredMuonsLeadingPt[0].pt()>10:
            # if (event.NoTriggeredMuonsLeadingPt[0].pt()<10): print "ESISTE UN LEPTONE NON TRIGGERING WITH PT>10, event.NoTriggeredMuonsLeadingPt[0].pt() = ",event.NoTriggeredMuonsLeadingPt[0].pt()
            return keepFailingEvents, 'rejecting event with non triggering lepton with pT > 10 GeV'
          else:
              if fillCounter: self.counters.counter('WAna').inc('W non trg leading lepton pT < 10 GeV')
        else:
            if fillCounter: self.counters.counter('WAna').inc('W non trg leading lepton pT < 10 GeV')


        ##------------------------  MAKE THE MUON  --------------------------------------
            
        # if the genp are saved, compute dR between gen and reco muon 
        if (event.savegenpW and len(event.genW)==1):
          event.muGenDeltaRgenP = deltaR( event.selMuons[0].eta(), event.selMuons[0].phi(), event.genMu[0].eta(), event.genMu[0].phi() ) 

        # associate good vertex to muon to compute dxy
        event.selMuons[0].associatedVertex = event.goodVertices[0]
                
        # testing offline muon cuts (tight+iso, no kinematical cuts)
        event.selMuonIsTightAndIso = testLeg(self, event.selMuons[0] ) 
        event.selMuonIsTight = testLegID( self,event.selMuons[0] ) 
          
        # define a W from lepton and MET
        event.W4V = event.selMuons[0].p4() + event.pfmet.p4()
        event.W4V_mt = mT(self,event.selMuons[0].p4() , event.pfmet.p4())        
        
        event.covMatrixMuon = []
        RetrieveMuonMatrixIntoVector(self,event.selMuons[0],event.covMatrixMuon)
        # print event.covMatrixMuon

        ##------------------------  MAKE THE JETS and RECOIL variables  --------------------------------------

        # Code to study the recoil (not very useful for W...)
        metVect = event.pfmet.p4().Vect()
        metVect.SetZ(0.) # use only transverse info
        WVect = event.W4V.Vect()
        WVect.SetZ(0.) # use only transverse info
        recoilVect = - copy.deepcopy(metVect) ## FIXED (met sign inverted)
        # recoilVect -= WVect
        temp_recoil = event.selMuons[0].p4().Vect()
        temp_recoil.SetZ(0.) # use only transverse info
        recoilVect -= temp_recoil ## FIXED (subtract only lepton for consistent recoil definition)
        
        uWVect = WVect.Unit()
        zAxis = type(WVect)(0,0,1)
        uWVectPerp = WVect.Cross(zAxis).Unit()

        u1 = recoilVect.Dot(uWVect) # recoil parallel to W pt
        u2 = - recoilVect.Dot(uWVectPerp) # recoil perpendicular to W pt

        event.u1 = u1
        event.u2 = u2

        ##------------------------  FINAL COUNTERS  --------------------------------------       
        
        if fillCounter:
          if event.selMuonIsTightAndIso : 
            self.counters.counter('WAna').inc('W lep is MuIsTightAndIso')
            if testLegKine( self, event.selMuons[0] , 30 , 2.1 ) : 
              self.counters.counter('WAna').inc('W Mu_eta<2.1 && Mu_pt>30')
              if event.pfmet.pt() >25: 
                self.counters.counter('WAna').inc('W pfmet>25')
                if event.W4V.Pt() < 20: 
                  self.counters.counter('WAna').inc('W pt<20')
                  if len(event.selJets) > 0: 
                    if event.selJets[0].pt()<30: 
                      self.counters.counter('WAna').inc('W Jet_leading_pt<30')
                  else:
                    self.counters.counter('WAna').inc('W Jet_leading_pt<30')

        # event is fully considered as good
        # if fillCounter: self.counters.counter('WAna').inc('W pass')
        event.WGoodEvent = True
        return (event.WGoodEvent or keepFailingEvents)


    def declareHandles(self):        
        super(WAnalyzer, self).declareHandles()
        self.handles['muons'] = AutoHandle('cmgMuonSel','std::vector<cmg::Muon>')
        self.handles['jets'] = AutoHandle('cmgPFJetSel','std::vector<cmg::PFJet>')
        self.handles['pfmet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>' )
        self.handles['tkmet'] = AutoHandle('tkMet','std::vector<reco::PFMET>' )
        self.mchandles['genpart'] =  AutoHandle('genParticlesPruned','std::vector<reco::GenParticle>')
        if self.cfg_comp.isMC :
          if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
            self.mchandles['LHEweights'] =  AutoHandle('source','LHEEventProduct')

## UNUSED
#        self.handles['jetLead'] = AutoHandle('cmgPFBaseJetLead','vector<cmg::BaseJet>')
#        self.handles['nJetsPtGt1'] =  AutoHandle('nJetsPtGt1','int')
#        self.handles['pfMetForRegression'] = AutoHandle('pfMetForRegression','std::vector<reco::PFMET>' )
#        self.handles['pucmet'] = AutoHandle('pcMet','std::vector<reco::PFMET>' )
#        self.handles['nopumet'] = AutoHandle('nopuMet','std::vector<reco::PFMET>' )
#        self.handles['pumet'] = AutoHandle('puMet','std::vector<reco::PFMET>' )
#        self.handles['TriggerResults'] = AutoHandle( ('TriggerResults','','HLT'), 'edm::TriggerResults' )
#        self.handles['vertices'] =  AutoHandle('slimmedPrimaryVertices','std::vector<reco::Vertex>')

