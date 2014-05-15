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
# from CMGTools.Utilities.mvaMET.mvaMet import MVAMet, PFMET
# from WMass.analyzers.GetMVAMET import GetMVAMET

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
        
        # self.mvamet = MVAMet() # SHOULD BE MVAMet(0.1)

        # void    Initialize(const edm::ParameterSet &iConfig, 
        # TString iU1Weights      ="$CMSSW_BASE/src/pharris/data/gbrmet_52.root",
        # TString iPhiWeights     ="$CMSSW_BASE/src/pharris/data/gbrmetphi_52.root",
        # TString iCovU1Weights   ="$CMSSW_BASE/src/pharris/data/gbrcovu1_52.root",
        # TString iCovU2Weights   ="$CMSSW_BASE/src/pharris/data/gbrcovu2_52.root",
        # MVAMet::MVAType  iType=kBaseline);

        # basePath = os.environ['CMSSW_BASE']+"/src/CMGTools/Utilities/data/mvaMET/";
        # print  ("Inputs are",
                          # basePath+'gbrmet_42.root',
                          # basePath+'gbrmetphi_42.root',
                          # basePath+'gbru1cov_42.root',
                          # basePath+'gbru2cov_42.root',
                # )
        # self.mvamet.Initialize(0,
                          # basePath+'gbrmet_42.root',
                          # basePath+'gbrmetphi_42.root',
                          # basePath+'gbrmetu1cov_42.root',
                          # basePath+'gbrmetu2cov_42.root',
                          # 0 # useless
                          # )

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
        event.genParticles = []
        event.LHEweights = []
        event.LHE_weights = []
        if self.cfg_comp.isMC :
          event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )        
          event.LHEweights = self.mchandles['LHEweights'].product()
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
            
        
        # retrieve collections of interest (muons and jets)
        event.allMuons = copy.copy(event.muons)
        event.selMuons = copy.copy(event.muons)
        event.NoTriggeredMuonsLeadingPt = copy.copy(event.muons)
        event.allJets = copy.copy(event.jets)
        event.selJets = copy.copy(event.jets)

        # check if the event is MC and if genp must be saved
        event.savegenpW=False
        # event.savegenpW=True
        # if not (self.cfg_ana.savegenp and self.cfg_comp.isMC):
          # event.savegenpW=False
        # print 'event.savegenpW 1 ',event.savegenpW
        
        # save genp only for signal events
        # i.e. only one W is present and daughters are muon plus neutrino
        genW_dummy = [ genp for genp in event.genParticles if \
                       math.fabs(genp.pdgId())==24 if \
                       ( \
                         math.fabs(genp.daughter(0).pdgId())==11 or 
                         math.fabs(genp.daughter(1).pdgId())==11 or 
                         math.fabs(genp.daughter(0).pdgId())==13 or 
                         math.fabs(genp.daughter(1).pdgId())==13 or 
                         math.fabs(genp.daughter(0).pdgId())==15 or 
                         math.fabs(genp.daughter(1).pdgId())==15
                         ) ]

#        if len(genW_dummy)>0:
          # if the genp event is selected, associate gen muon and neutrino
        event.genMu = []
        event.genMuStatus1 = []
        event.genNu = []
        event.genWLept = []

        if self.cfg_ana.savegenp and self.cfg_comp.isMC:
        
          if len(genW_dummy)==1:
  #        if len(genW_dummy)>0:
            event.genW = [ genp for genp in genW_dummy if \
                                 ( \
                                  ( math.fabs(genW_dummy[0].daughter(0).pdgId())==13 ) or \
                                  ( math.fabs(genW_dummy[0].daughter(1).pdgId())==13 ) \
                                  ) ]
            if len(event.genW)==1:

              event.savegenpW=True
              event.genW_mt = mT(self,event.genW[0].daughter(0).p4() , event.genW[0].daughter(1).p4())
              event.muGenDeltaRgenP=1e6

              event.genWLept.append(event.genW[0])
              if [ math.fabs(event.genW[0].daughter(0).pdgId())==13 ]:
                event.genMu.append(event.genW[0].daughter(0))
                event.genNu.append(event.genW[0].daughter(1))
              else:
                event.genMu.append(event.genW[0].daughter(1))
                event.genNu.append(event.genW[0].daughter(0))
              
              if(len(event.genMu) >0):
                if(math.fabs(event.genW[0].mother(0).pdgId())!=6):
                  event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                else:
                  event.genMuStatus1.append(event.genMu[0])


          if len(genW_dummy)>1:
            event.genW = [ genp for genp in genW_dummy if \
                                 ( \
                                  ( math.fabs(genW_dummy[0].daughter(0).pdgId())==11 ) or \
                                  ( math.fabs(genW_dummy[0].daughter(1).pdgId())==11 ) or \
                                  ( math.fabs(genW_dummy[0].daughter(0).pdgId())==13 ) or \
                                  ( math.fabs(genW_dummy[0].daughter(1).pdgId())==13 ) or \
                                  ( math.fabs(genW_dummy[0].daughter(0).pdgId())==15 ) or \
                                  ( math.fabs(genW_dummy[0].daughter(1).pdgId())==15 ) or \
                                  ( math.fabs(genW_dummy[1].daughter(0).pdgId())==11 ) or \
                                  ( math.fabs(genW_dummy[1].daughter(1).pdgId())==11 ) or \
                                  ( math.fabs(genW_dummy[1].daughter(0).pdgId())==13 ) or \
                                  ( math.fabs(genW_dummy[1].daughter(1).pdgId())==13 ) or \
                                  ( math.fabs(genW_dummy[1].daughter(0).pdgId())==15 ) or \
                                  ( math.fabs(genW_dummy[1].daughter(1).pdgId())==15 ) \
                                  ) ]

            if len(event.genW)==2:
              if ( math.fabs(event.genW[0].daughter(0).pdgId())==13. or math.fabs(event.genW[0].daughter(0).pdgId())==15. or math.fabs(event.genW[0].daughter(0).pdgId())==11. ):
                  # print 'event.savegenpW 2 ',event.savegenpW
                  event.savegenpW=False
  #                print 'found leptonic W0 a'
                  event.genWLept.append(event.genW[0])
                  if ( math.fabs(event.genW[0].daughter(0).pdgId())==13 ):
                      event.genMu.append(event.genW[0].daughter(0))
                      event.genNu.append(event.genW[0].daughter(1))
                      event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())
                      event.muGenDeltaRgenP=1e6
                      if(len(event.genMu) >0):
                          event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      event.savegenpW=True

              if ( math.fabs(event.genW[0].daughter(1).pdgId())==13. or math.fabs(event.genW[0].daughter(1).pdgId())==15. or math.fabs(event.genW[0].daughter(1).pdgId())==11. ):
                  # print 'event.savegenpW 3 ',event.savegenpW
                  event.savegenpW=False
  #                print 'found leptonic W0 b'
                  event.genWLept.append(event.genW[0])
                  if ( math.fabs(event.genW[0].daughter(1).pdgId())==13 ):
                      event.genMu.append(event.genW[0].daughter(1))
                      event.genNu.append(event.genW[0].daughter(0))
                      event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())
                      event.muGenDeltaRgenP=1e6
                      if(len(event.genMu) >0):
                          event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      # print 'event.savegenpW 4 ',event.savegenpW
                      event.savegenpW=True
                      
              if ( math.fabs(event.genW[1].daughter(0).pdgId())==13. or math.fabs(event.genW[1].daughter(0).pdgId())==15. or math.fabs(event.genW[1].daughter(0).pdgId())==11. ):
                  # print 'event.savegenpW 5 ',event.savegenpW
                  event.savegenpW=False
  #                print 'found leptonic W1 c'
                  event.genWLept.append(event.genW[1])
                  if ( math.fabs(event.genW[1].daughter(0).pdgId())==13 ):
                      event.genMu.append(event.genW[1].daughter(0))
                      event.genNu.append(event.genW[1].daughter(1))
                      event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())                
                      event.muGenDeltaRgenP=1e6
                      if(len(event.genMu) >0):
                          event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      event.savegenpW=True
                      
              if ( math.fabs(event.genW[1].daughter(1).pdgId())==13. or math.fabs(event.genW[1].daughter(1).pdgId())==15. or math.fabs(event.genW[1].daughter(1).pdgId())==11. ):
                  # print 'event.savegenpW 6 ',event.savegenpW
                  event.savegenpW=False
  #                print 'found leptonic W1 d'
                  event.genWLept.append(event.genW[1])
                  if ( math.fabs(event.genW[1].daughter(1).pdgId())==13 ):
                      event.genMu.append(event.genW[1].daughter(1))
                      event.genNu.append(event.genW[1].daughter(0))
                      event.genW_mt = mT(self,event.genMu[0].p4() , event.genNu[0].p4())                
                      event.muGenDeltaRgenP=1e6
                      if(len(event.genMu) >0):
                          event.genMuStatus1.append(returnMuonDaughterStatus1(self,event.genMu[0]))
                      event.savegenpW=True
                      
              # if the genp is not signal, don't save genp but do not exit 
              # -----> events which will pass the reconstruction but are not signal
              # can be considered as background (for example, in W+Jets, from W decaying into electrons, taus)

          # else:
            # ## here put false for fully hadronic WW  
            # print 'event.savegenpW 7 ',event.savegenpW
            # event.savegenpW=False
                
#        print 'genW found ', len(genW_dummy)
#        print 'genWLeptonic found ', len(event.genWLept)

        # store event number of muons, MET and jets in all gen events (necessary to make cuts in genp studies...)
        # total number of reco muons
        event.nMuons=len(event.selMuons)
        # clean jets by removing muons
        event.selJets = [ jet for jet in event.allJets if ( \
                                        not (bestMatch( jet , event.selMuons ))[1] <0.5 \
                                        and jet.looseJetId() and jet.pt()>30 \
                                        )
                        ]
        keepFailingEvents = True
        if hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents:
            # print 'hasattr(self.cfg_ana,\'keepFailingEvents\') and not self.cfg_ana.keepFailingEvents'
            keepFailingEvents = False
        # print 'keepFailingEvents',keepFailingEvents
        
            
        # reco events must have good reco vertex and trigger fired...
        if not (event.passedVertexAnalyzer and event.passedTriggerAnalyzer):
          return keepFailingEvents
        # ...and at lest one reco muon...
        if len(event.selMuons) == 0:
            return keepFailingEvents
        if fillCounter: self.counters.counter('WAna').inc('W ev trig, good vertex and >= 1 lepton')
        
        #check if the event is triggered according to cfg_ana
        if len(self.cfg_comp.triggers)>0:
            # muon object trigger matching
            event.selMuons = [lep for lep in event.allMuons if \
                            trigMatched(self, event, lep)]
            # exit if there are no triggered muons
            if len(event.selMuons) == 0:
                return keepFailingEvents, 'trigger matching failed'
            else:
                if fillCounter: self.counters.counter('WAna').inc('W at least 1 lep trig matched')
                
        # to select W impose only 1 triggering lepton in the event:
        # the number of triggering lepton is checked on the whole lepton collection
        # before any cut, otherwise could be a Z!!!
        if len(event.selMuons) != 1:
          # print 'len(event.selMuons) != 1, returning ', keepFailingEvents
          return keepFailingEvents, 'more than 1 lep trig matched'
        else:
            if fillCounter: self.counters.counter('WAna').inc('W only 1 lep trig matched')

        # print 'len(event.selMuons) = ',len(event.selMuons)
        # if len(event.selMuons)!= 1: print 'BUT CONTINUING!'
        
        # store muons that did not fire the trigger
        event.NoTriggeredMuonsLeadingPt = [lep for lep in event.allMuons if \
                        not trigMatched(self, event, lep) ]
        
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

        
        # if the genp are saved, compute dR between gen and reco muon 
        if (event.savegenpW and len(event.genW)==1):
          event.muGenDeltaRgenP = deltaR( event.selMuons[0].eta(), event.selMuons[0].phi(), event.genMu[0].eta(), event.genMu[0].phi() ) 

        # associate good vertex to muon to compute dxy
        event.selMuons[0].associatedVertex = event.goodVertices[0]
                
        # testing offline muon cuts (tight+iso, no kinematical cuts)
        event.selMuonIsTightAndIso = testLeg(self, event.selMuons[0] ) 
        event.selMuonIsTight = testLegID( self,event.selMuons[0] ) 
          
        # START RETRIEVING MVAMET
        
        # INPUT DEFINITIONS AS OF HTT
                # mvaMETTauMu = cms.EDProducer(
                  # "MVAMETProducerTauMu",
                  # pfmetSrc = cms.InputTag('pfMetForRegression'),
                  # tkmetSrc = cms.InputTag('tkMet'),
                  # nopumetSrc = cms.InputTag('nopuMet'),
                  # pucmetSrc = cms.InputTag('pcMet'),
                  # pumetSrc = cms.InputTag('puMet'),
                  # recBosonSrc = cms.InputTag('cmgTauMuSel'),
                  # jetSrc = cms.InputTag('cmgPFJetSel'),
                  # leadJetSrc = cms.InputTag('cmgPFBaseJetLead'),
                  # vertexSrc = cms.InputTag('goodPVFilter'),
                  # nJetsPtGt1Src = cms.InputTag('nJetsPtGt1'),
                  # rhoSrc = cms.InputTag('kt6PFJets','rho'),
                  # enable = cms.bool(True),
                  # verbose = cms.untracked.bool( False ),
                  # weights_gbrmet = cms.string(weights_gbrmet),
                  # weights_gbrmetphi = cms.string(weights_gbrmetphi),
                  # weights_gbrmetu1cov = cms.string(weights_gbrmetu1cov),
                  # weights_gbrmetu2cov = cms.string(weights_gbrmetu2cov),
                  
                  # #COLIN: make delta R a parameter
                  # )

        # self.prepareObjectsForMVAMET(event)
        
        # self.mvamet.getMet(
                           # event.cleanpfmetForRegression, #iPFMet,
                           # event.cleantkmet, #iTKMet,
                           # event.cleannopumet, #iNoPUMet,
                           # event.pumet, #iPUMet,
                           # event.cleanpucmet, #iPUCMet,
                           # event.iLeadJet, #event.iLeadJet,
                           # event.i2ndJet,  #event.i2ndJet,
                           # event.NJetsGt30, #iNJetsGt30,
                           # event.nJetsPtGt1Clean, #iNJetsGt1,
                           # len(event.goodVertices), #iNGoodVtx,
                           # event.iJets_p4, #iJets,
                           # event.iJets_mva, #iJets,
                           # event.iJets_neutFrac, #iJets,
                           # False, #iPrintDebug,
                           # event.visObjectP4s_array #visObjectP4s
                          # )
                          
        # event.mvamet = self.mvamet.GetMet_first();
        # event.GetMVAMet_second = self.mvamet.GetMet_second();
        
        # print 'AFTER MVAmet_test'
        # print 'event.pfmet.pt() ', event.pfmet.pt()
        # print 'event.selMuons[0].pt() ',event.selMuons[0].pt(),' event.mvamet.Pt() ',event.mvamet.Pt()
        # print ''
        # print 'event.GetMVAMet_second ',event.GetMVAMet_second,' event.GetMVAMet_second.significance() ',event.GetMVAMet_second.significance().Print()
        
        # define a W from lepton and MET
        event.W4V = event.selMuons[0].p4() + event.pfmet.p4()
        event.W4V_mt = mT(self,event.selMuons[0].p4() , event.pfmet.p4())        
        
        event.covMatrixMuon = []
        RetrieveMuonMatrixIntoVector(self,event.selMuons[0],event.covMatrixMuon)
        # print event.covMatrixMuon

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
        return True


    def declareHandles(self):        
        super(WAnalyzer, self).declareHandles()
        self.handles['cmgTriggerObjectSel'] =  AutoHandle('cmgTriggerObjectSel','std::vector<cmg::TriggerObject>')
        self.handles['muons'] = AutoHandle('cmgMuonSel','std::vector<cmg::Muon>')
        self.handles['jets'] = AutoHandle('cmgPFJetSel','std::vector<cmg::PFJet>')
        self.handles['jetLead'] = AutoHandle('cmgPFBaseJetLead','vector<cmg::BaseJet>')
        self.handles['pfmet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>' )
        self.handles['pfMetForRegression'] = AutoHandle('pfMetForRegression','std::vector<reco::PFMET>' )
        self.handles['tkmet'] = AutoHandle('tkMet','std::vector<reco::PFMET>' )
        self.handles['nopumet'] = AutoHandle('nopuMet','std::vector<reco::PFMET>' )
        self.handles['pumet'] = AutoHandle('puMet','std::vector<reco::PFMET>' )
        self.handles['pucmet'] = AutoHandle('pcMet','std::vector<reco::PFMET>' )
        self.mchandles['genpart'] =  AutoHandle('genParticlesPruned','std::vector<reco::GenParticle>')
        self.handles['vertices'] =  AutoHandle('offlinePrimaryVertices','std::vector<reco::Vertex>')
        self.handles['nJetsPtGt1'] =  AutoHandle('nJetsPtGt1','int')
        self.handles['cmgTriggerObjectSel'] =  AutoHandle('cmgTriggerObjectSel','std::vector<cmg::TriggerObject>')
        self.handles['muons'] = AutoHandle('cmgMuonSel','std::vector<cmg::Muon>')
        self.handles['jets'] = AutoHandle('cmgPFJetSel','std::vector<cmg::PFJet>')
        self.handles['pfmet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>' )
        self.mchandles['genpart'] =  AutoHandle('genParticlesPruned','std::vector<reco::GenParticle>')
        self.mchandles['LHEweights'] =  AutoHandle('source','LHEEventProduct')

