import operator
import copy
import math
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Jet, GenParticle
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched
from CMGTools.RootTools.utils.DeltaR import bestMatch, deltaR, deltaR2
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
        if self.cfg_comp.isMC :
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
        event.NoTriggeredMuonsLeadingPt = copy.copy(event.muons)
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
          event.genMuStatus1 = []
          event.genNu = []
          
          if len(event.genW)==1:
            if [ math.fabs(event.genW[0].daughter(0).pdgId())==13 ]:
              event.genMu.append(event.genW[0].daughter(0))
              event.genNu.append(event.genW[0].daughter(1))
            else:
              event.genMu.append(event.genW[0].daughter(1))
              event.genNu.append(event.genW[0].daughter(0))
            
            if(len(event.genMu) >0):
              event.genMuStatus1.append(self.returnMuonDaughterStatus1(event.genMu[0]))
              
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
        if not (event.passedVertexAnalyzer and event.passedTriggerAnalyzer):
          return True
        # ...and at lest one reco muon...
        if len(event.selMuons) == 0:
            return True
        if fillCounter: self.counters.counter('WAna').inc('W ev trig, good vertex and >= 1 lepton')
        
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

        # store muons that did not fire the trigger
        event.NoTriggeredMuonsLeadingPt = [lep for lep in event.allMuons if \
                        not self.trigMatched(event, lep) ]
        
        # print "len(event.NoTriggeredMuonsLeadingPt)= ",len(event.NoTriggeredMuonsLeadingPt)
        # if len(event.NoTriggeredMuonsLeadingPt)>0 : print "event.NoTriggeredMuonsLeadingPt[0].pt() = ",event.NoTriggeredMuonsLeadingPt[0].pt()

        if len(event.NoTriggeredMuonsLeadingPt) > 0:
          if event.NoTriggeredMuonsLeadingPt[0].pt()>10:
            # if (event.NoTriggeredMuonsLeadingPt[0].pt()<10): print "ESISTE UN LEPTONE NON TRIGGERING WITH PT>10, event.NoTriggeredMuonsLeadingPt[0].pt() = ",event.NoTriggeredMuonsLeadingPt[0].pt()
            return True, 'rejecting event with non triggering lepton with pT > 10 GeV'
          else:
              if fillCounter: self.counters.counter('WAna').inc('W non trg leading lepton pT < 10 GeV')
        else:
            if fillCounter: self.counters.counter('WAna').inc('W non trg leading lepton pT < 10 GeV')

        
        # if the genp are saved, compute dR between gen and reco muon 
        if event.savegenpW :
          event.muGenDeltaRgenP = deltaR( event.selMuons[0].eta(), event.selMuons[0].phi(), event.genMu[0].eta(), event.genMu[0].phi() ) 

        # associate good vertex to muon to compute dxy
        event.selMuons[0].associatedVertex = event.goodVertices[0]
                
        # testing offline muon cuts (tight+iso, no kinematical cuts)
        event.selMuonIsTightAndIso = self.testLeg( event.selMuons[0] ) 
        event.selMuonIsTight = self.testLegID( event.selMuons[0] ) 
          
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
        
        if fillCounter:
          if event.selMuonIsTightAndIso : 
            self.counters.counter('WAna').inc('W lep is MuIsTightAndIso')
            if self.testLegKine( event.selMuons[0] , 30 , 2.1 ) : 
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
        # print 'leg1.Pt() ',leg1.Pt(),' leg2.Pt() ',leg2.Pt(),' leg1.Px() ',leg1.Px(),' leg2.Px() ',leg2.Px(),' leg2.Py() ',leg2.Py(),' leg1.Pt() ',leg1.Pt(),' leg2.Pt() ',leg2.Pt(),' before sqrt ',( 2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) ) )
        if (2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) )) > 0:
            return math.sqrt( 2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) ) )
        else:
            return 0


    def trigMatched(self, event, leg):
        '''Returns true if the leg is matched to a trigger object as defined in the
        triggerMap parameter'''
        if not hasattr( self.cfg_ana, 'triggerMap'):
            return True
        path = event.hltPath
        triggerObjects = event.triggerObjects
        filters = self.cfg_ana.triggerMap[ path ]
        # the dR2Max value is 0.1^2
        return triggerMatched(leg, triggerObjects, path, filters,
                              dR2Max=0.01,
                              pdgIds=None )

                              
    # def prepareObjectsForMVAMET(self,event):
    
        # event.NJetsGt30 = 0 # Number of jets above 30 Gev in cmgPFJetSel after cleaning
        # event.nJetsPtGt1Clean = event.nJetsPtGt1H # Number of jets above 1 Gev in cmgPFJetSel after cleaning
    
        # for jet in event.allJets:
            # if jet.pt>=1:
                # if deltaR(jet.eta(),jet.phi(),event.selMuons[0].eta(),event.selMuons[0].phi())<0.5 : 
                    # event.nJetsPtGt1Clean = event.nJetsPtGt1Clean - 1
                # else:
                    # if jet.pt>=30:
                        # event.NJetsGt30 = event.NJetsGt30 + 1
        # if(event.nJetsPtGt1Clean < 0): event.nJetsPtGt1Clean = 0
        
        # # Clean various METs (but PUMET)
        
        # dummyVertex = ROOTmath.XYZPointD()
        
        # cleanpfmetp4 = event.pfMetForRegression.p4() + event.selMuons[0].p4()
        # cleanpfmetsumet = event.pfMetForRegression.sumEt() - event.selMuons[0].pt()
        # event.cleanpfmetForRegression = PFMET(event.pfMetForRegression.getSpecific(),cleanpfmetsumet,cleanpfmetp4,dummyVertex)
        
        # cleanpucmetp4 = event.pucmet.p4() + event.selMuons[0].p4()
        # cleanpucmetsumet = event.pucmet.sumEt() - event.selMuons[0].pt()
        # event.cleanpucmet = PFMET(event.pucmet.getSpecific(),cleanpucmetsumet,cleanpucmetp4,dummyVertex)
        
        # cleantkmetp4 = event.tkmet.p4() + event.selMuons[0].p4()
        # cleantkmetsumet = event.tkmet.sumEt() - event.selMuons[0].pt()
        # event.cleantkmet = PFMET(event.tkmet.getSpecific(),cleantkmetsumet,cleantkmetp4,dummyVertex)
        
        # cleannopumetp4 = event.nopumet.p4() + event.selMuons[0].p4()
        # cleannopumetsumet = event.nopumet.sumEt() - event.selMuons[0].pt()
        # event.cleannopumet = PFMET(event.nopumet.getSpecific(),cleannopumetsumet,cleannopumetp4,dummyVertex)
        
        # event.iLeadJet = 0 # Leading jet (if any) from cmgPFBaseJetLead after cleaning
        # event.i2ndJet = 0 # Second leading jet (if any) from cmgPFBaseJetLead collection after cleaning
        
        # event.jetLeadClean = [jet for jet in event.jetLead if \
                                          # deltaR(jet.eta(),jet.phi(),event.selMuons[0].eta(),event.selMuons[0].phi())<0.5 ]
                        
        # if(len(event.jetLeadClean)>0): event.iLeadJet = event.jetLeadClean[0].p4()
        # if(len(event.jetLeadClean)>1): event.i2ndJet = event.jetLeadClean[1].p4()

        # event.visObjectP4s_array = [event.selMuons[0].p4()] # Visual part of the signal (single muon for the W, dimuon for the Z)
        
        # # iJets struct from cmgPFJetSel without cleaning (will be probably done afterwards)
        # event.iJets_p4 = []
        # event.iJets_mva = []
        # event.iJets_neutFrac = []
        # for jet in event.allJets:
            # if jet.looseJetId():
                # event.iJets_p4.append(jet.p4())
                # event.iJets_mva.append(jet.puMva('philv1'))
                # neutFrac=1
                # # DOES THIS HOLD ALSO IN 44X ???
                # if(math.fabs(jet.eta())<2.5): 
                    # neutFrac = jet.component(reco.PFCandidate.gamma).fraction() + jet.component(reco.PFCandidate.h0).fraction() + jet.component(reco.PFCandidate.egamma_HF).fraction()
                # event.iJets_neutFrac.append( neutFrac )
                
        # print ' event.pfmet.pt() ',event.pfmet.pt()
        # print ' event.pfMetForRegression.pt() ',event.pfMetForRegression.pt(),' event.tkmet.pt() ',event.tkmet.pt(),\
              # ' event.nopumet.pt() ',event.nopumet.pt(),' event.pumet.pt() ',event.pumet.pt(),\
              # ' event.pucmet.pt() ',event.pucmet.pt()

        # print ' event.cleanpfmetForRegression.pt() ',event.cleanpfmetForRegression.pt(),' event.cleantkmet.pt() ',event.cleantkmet.pt(),\
              # ' event.cleannopumet.pt() ',event.cleannopumet.pt(),' event.pumet.pt() ',event.pumet.pt(),\
              # ' event.cleanpucmet.pt() ',event.cleanpucmet.pt()


    def returnMuonDaughter(self,genp_muon):
        if genp_muon.numberOfDaughters()>0:
           for k in range(0,genp_muon.numberOfDaughters()):
             if genp_muon.daughter(k).pdgId()==13:
               return genp_muon.daughter(k)
             elif genp_muon.daughter(k).pdgId()==-13:
               return genp_muon.daughter(k)
             # else:
               # print 'no muon daughter?'  
        else:
          # print 'Satus > 1 muon had no daughter'
          return genp_muon
      
    def returnMuonDaughterStatus1(self,genp_muon_daughter):
        if genp_muon_daughter.status()==1:
          return genp_muon_daughter
        else:
          #print 'Calling again'
          return self.returnMuonDaughterStatus1(self.returnMuonDaughter(genp_muon_daughter))

              

class WBoson(object):

        def __init__(self, lepton, met):
                self.lepton = lepton
                self.met = met
                self.mt = None
