import operator
import numpy as my_n
import copy
import math, os
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Jet, GenParticle
# from CMGTools.RootTools.utils.TriggerMatching import triggerMatched
from CMGTools.RootTools.utils.DeltaR import bestMatch, deltaR, deltaR2
from CMGTools.WMass.analyzers.common_functions import *
# from CMGTools.Utilities.mvaMET.mvaMet import MVAMet

class JPsiAnalyzer( Analyzer ):

    ZMuonClass = Muon 
    ZJetClass = Jet 
    
    def beginLoop(self):
        super(JPsiAnalyzer,self).beginLoop()
        self.counters.addCounter('JPsiAna')
        count = self.counters.counter('JPsiAna')
        count.register('Z all triggered events')
        count.register('Z >= 2 lepton')
        count.register('Z at least 1 lep trig matched')
        count.register('Z good muon pair found')
        count.register('Z non trg leading extra muon pT < 10 GeV')
        count.register('Z Inv Mass>50')
        count.register('Z pos Mu is MuIsTightAndIso')
        count.register('Z pos Mu_eta<2.1 && Mu_pt>30*MZ/MW')
        count.register('Z neg Mu_eta<2.4 && Mu_pt>10')
        count.register('Z pfmet>25*MZ/MW')
        count.register('Z pt<20*MZ/MW')
        count.register('Z Jet_leading_pt<30')

    def buildLeptons(self, cmgMuons, event):
        '''Creates python Leptons from the muons read from the disk.
        to be overloaded if needed.'''
        return map( self.__class__.ZMuonClass, cmgMuons )

    def buildJets(self, cmgJets, event):
        '''Creates python Jets from the Jets read from the disk.
        to be overloaded if needed.'''
        return map( self.__class__.ZJetClass, cmgJets )
        
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
        event.Zmuons = self.buildLeptons( self.handles['Zmuons'].product(), event )
        # access jets
        event.Zjets = self.buildJets( self.handles['Zjets'].product(), event )
        # access MET
        event.pfmet = self.handles['pfmet'].product()[0]
        event.ZElectrons = self.buildLeptons( self.handles['ZElectrons'].product(), event )

        if hasattr(self.cfg_ana,'computeMVAmet'):
          event.tkmet = self.handles['tkmet'].product()[0]
          event.nopumet = self.handles['nopumet'].product()[0]
          event.pumet = self.handles['pumet'].product()[0]
          event.pucmet = self.handles['pucmet'].product()[0]
          event.pfMetForRegression = self.handles['pfMetForRegression'].product()[0]
          event.nJetsPtGt1H = self.handles['nJetsPtGt1'].product()[0]
        
        if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
          event.cmgPFcands = self.handles['cmgCandidates'].product()

        # access genP
        event.genParticles = []
        event.LHEweights = []
        if self.cfg_comp.isMC and self.cfg_ana.savegenp :
          event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )
          # event.LHEweights = self.mchandles['LHEweights'].product()
        # define good event bool
        event.ZGoodEvent = False
          # select event
        return self.selectionSequence(event, fillCounter=True)
        
    def selectionSequence(self, event, fillCounter):

        if fillCounter: self.counters.counter('JPsiAna').inc('Z all triggered events')
        
        # retrieve collections of interest (muons and jets)
        
        # retrieve LHE weights if requested
        # if hasattr(self.cfg_ana,'hasLHEweights') and self.cfg_comp.isMC and self.cfg_ana.savegenp :
          # print event.LHEweights.comments_size()
          # for i in range(0,event.LHEweights.comments_size()):
            # print i, event.LHEweights.getComment(i).split()

        # loop over electrons 
        event.ZselElectrons = [electron for electron in event.ZElectrons if \
                            electron.pt()>20 and \
                               testElectronIDMedium(self,electron) and \
                               testElectronVtxMedium(self,electron,event)]
                           # self.testElectronVtxMediumTight(electron,event)   
                            #and electron
                         
        event.ZElTightID = []
        event.ZElTightIso = []
        event.ZElMediumID = []
        event.ZElMediumIso = []
        event.ZElIsPromt = []
        for i in range(0, min(len(event.ZselElectrons),9)):
            if testElectronIDTight(self,event.ZselElectrons[i]):
                event.ZElTightID.append(1)
            else:
                event.ZElTightID.append(0)
            if testElectronIDMedium(self,event.ZselElectrons[i]):
                event.ZElMediumID.append(1)
            else:
                event.ZElMediumID.append(0)

            if testElectronVtxTight(self,event.ZselElectrons[i],event):
                event.ZElTightIso.append(1)
            else:
                event.ZElTightIso.append(0)
            if testElectronVtxMedium(self,event.ZselElectrons[i],event):
                event.ZElMediumIso.append(1)
            else:
                event.ZElMediumIso.append(0)
           
        
     #   print 'looping on muons'
        event.ZallMuons = copy.copy(event.Zmuons)
        event.ZallMuonsTrig = my_n.zeros(10, dtype=int);
        event.ZallMuonsID = []
        event.ZallMuonsID_8TeV = []
        event.ZallMuonsMatched = []
        event.ZallMuonsSelMatched = []
        if len(event.ZallMuons)>0:
            for i in range(0, min(len(event.ZallMuons),9)):
            #print self.cfg_comp.triggers
            # if(event.passedTriggerAnalyzer):
                # print 'trigMatched(self,event,event.ZallMuons[',i,'])',trigMatched(self,event,event.ZallMuons[i])
                event.ZallMuonsTrig[i]=0.
                for T, TL in self.cfg_ana.triggerBits.iteritems():
                  # muon object trigger matching
                  if(trigMatched(self, event, event.ZallMuons[i], TL)):
                # if(trigMatched(self,event,event.ZallMuons[i])):
                    event.ZallMuonsTrig[i]=1.
            # else:
                # event.ZallMuonsTrig[i]=0.
                if((event.ZallMuons[i].isGlobalMuon() or event.ZallMuons[i].isTrackerMuon()) and len(event.goodVertices)>0):
                    event.ZallMuons[i].associatedVertex = event.goodVertices[0]
                    if testLegID(self, event.ZallMuons[i]):
                        event.ZallMuonsID.append(1.)
                    else:
                        event.ZallMuonsID.append(0.)
                else:
                    event.ZallMuonsID.append(0.)


                if((event.ZallMuons[i].isGlobalMuon() or event.ZallMuons[i].isTrackerMuon()) and len(event.goodVertices)>0):
                  #  event.ZallMuons[i].associatedVertex = event.goodVertices[0] aready in if above
                    if testLegID_8TeV(self, event.ZallMuons[i]):
                        event.ZallMuonsID_8TeV.append(1.)
                    else:
                        event.ZallMuonsID_8TeV.append(0.)
                else:
                    event.ZallMuonsID_8TeV.append(0.)


        event.ZselTriggeredMuons = []
        event.ZselNoTriggeredMuons = []
        event.ZselNoTriggeredExtraMuonsLeadingPt = []
        event.ZallJets = copy.copy(event.Zjets)
        event.ZselJets = []
                                                     
        # check if the event is MC and if genp must be saved
        event.savegenpZ=True
        if not (self.cfg_ana.savegenp and self.cfg_comp.isMC):
            event.savegenpZ=False


        if self.cfg_comp.isMC and self.cfg_ana.savegenp:
            if len(event.ZallMuons)>0:
                for i in range(0, min(len(event.ZallMuons),9)):
                    if( (matchPromt(self,event,event.ZallMuons[i],-13*event.ZallMuons[i].charge())+ matchPromtTau(self,event,event.ZallMuons[i],-13*event.ZallMuons[i].charge()))>0):
                        
                        event.ZallMuonsMatched.append(1.)
                    else:
                        event.ZallMuonsMatched.append(0.)  
            for i in range(0, min(len(event.ZselElectrons),9)):
                if  (matchPromt(self,event,event.ZselElectrons[i],-11*event.ZselElectrons[i].charge())+ matchPromtTau(self,event,event.ZselElectrons[i],-11*event.ZselElectrons[i].charge()))>0:
                    event.ZElIsPromt.append(1)
                    #print 'promt dude'
                else:
                    event.ZElIsPromt.append(0)
                    #print 'just kidding'

        # save genp only for signal events
        # i.e. only one Z is present and daughters are muons
        genZ_dummy = [ genp for genp in event.genParticles if \
                             (math.fabs(genp.pdgId())==443 # J/PSI!!!!
                             or math.fabs(genp.pdgId())==553)
                             and genp.numberOfDaughters()>0
                             and not ( math.fabs(genp.daughter(0).pdgId())== 443 or math.fabs(genp.daughter(0).pdgId())== 553)
                             ]
        
        # print 'number of genZ_dummy',len(genZ_dummy)
        # for igenZ in range(0,len(genZ_dummy)): 
          # print 'genZ_dummy',igenZ, genZ_dummy[igenZ], 'daughters',genZ_dummy[igenZ].numberOfDaughters(); 
          # for jgenZdau in range(0,genZ_dummy[igenZ].numberOfDaughters()): 
            # print 'daughter',jgenZdau,genZ_dummy[igenZ].daughter(jgenZdau).pdgId(),jgenZdau,genZ_dummy[igenZ].daughter(jgenZdau).pt(),jgenZdau,genZ_dummy[igenZ].daughter(jgenZdau).eta(),
        
        # genMu_dummy = [ genp for genp in event.genParticles if \
                     # math.fabs(genp.pdgId())==13 # Mu!!!! 
                     # ]
        # for i in genMu_dummy: print i, i.mother(0).pdgId()
        
        # print event.genParticles
        # print genZ_dummy
        if len(genZ_dummy)==1:
          event.genZ = [ genp for genp in genZ_dummy if \
                               math.fabs(genp.daughter(0).pdgId())==13
                               ]
          event.genMuPos = []
          event.genMuNeg = []
          event.genMuPosStatus1 = []
          event.genMuNegStatus1 = []
          
          # print event.genZ[0].daughter(0).pdgId(), event.genZ[0].daughter(1).pdgId()
          
          if len(event.genZ)==1:
          # if the genp event is selected, associate gen muons
            # if(event.genZ[0].daughter(0).pdgId()==13):
            if(event.genZ[0].daughter(0).charge()>0):
              event.genMuPos.append(event.genZ[0].daughter(0))
              event.genMuNeg.append(event.genZ[0].daughter(1))
            else:
              event.genMuPos.append(event.genZ[0].daughter(1))
              event.genMuNeg.append(event.genZ[0].daughter(0))
            
            if(len(event.genMuNeg) >0):
              event.genMuNegStatus1.append(returnMuonDaughterStatus1(self,event.genMuNeg[0]))
            if(len(event.genMuPos) >0):
              event.genMuPosStatus1.append(returnMuonDaughterStatus1(self,event.genMuPos[0]))
            
            event.genZ_PostFSR = event.genMuNegStatus1[0].p4() + event.genMuPosStatus1[0].p4()
            event.genZ_mt = mT(self,event.genZ[0].daughter(0).p4() , event.genZ[0].daughter(1).p4())
            event.muPosGenDeltaRgenP=1e6
            event.muNegGenDeltaRgenP=1e6
            
          else:
            # if the genp is not signal, don't save genp but do not exit 
            # -----> events which will pass the reconstruction but are not signal
            # can be considered as background (for example, in Z+Jets, from Z decaying into electrons, taus)
            event.savegenpZ=False
        else:
          event.savegenpZ=False

        # store event MET and jets in all gen events (necessary to make cuts in genp studies...)
        event.ZpfmetNoMu = event.pfmet.p4()
        # clean jets by removing muons
        event.ZselJets = [ jet for jet in event.ZallJets if ( jet.looseJetId() and jet.pt()>10 )
                                #not (bestMatch( jet , event.ZallMuons ))[1] <0.01 \
                                #and jet.looseJetId() and jet.pt()>10 \
                                #jet.looseJetId() and jet.pt()>10 \
                              #  )
                          ]
        
        # reco events must have good reco vertex and trigger fired...                          
        if not (event.passedVertexAnalyzer):
          if hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents:
            return False
          else:
            return True
        # ...and at lest two reco muons...
        if len(event.ZallMuons) < 2 :
            if hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents:
              return False
            else:
              return True
        if fillCounter : self.counters.counter('JPsiAna').inc('Z >= 2 lepton')
        
        # check if the event is triggered according to cfg_ana
        # store separately muons that fired the trigger
        # if len(self.cfg_comp.triggers)>0:
            # # muon object trigger matching
            # event.ZselTriggeredMuons = [lep for lep in event.ZallMuons if \
                           # trigMatched(self,event, lep)]
        if hasattr(self.cfg_ana, 'triggerBits'):
            for lep in event.ZallMuons:
              for T, TL in self.cfg_ana.triggerBits.iteritems():
                  # muon object trigger matching
                  if(trigMatched(self, event, lep, TL)):
                    event.ZselTriggeredMuons.append(lep)
                    continue                           
           # if len(event.ZselTriggeredMuons) == 0 :
               # return True, 'trigger matching failed'
           # else:
               # if fillCounter : self.counters.counter('JPsiAna').inc('Z at least 1 lep trig matched')

        # store muons that did not fire the trigger
        # event.ZselNoTriggeredMuons = [lep for lep in event.ZallMuons if \
                        # not trigMatched(self,event, lep)]
        
        # check wether there are muons that did not fire the trigger, if so print some info
        # if len(event.ZselNoTriggeredMuons)>0:
        # print 'len(event.ZallMuons)= ',len(event.ZallMuons),' len(event.ZselTriggeredMuons)= ',len(event.ZselTriggeredMuons),' len(event.ZselNoTriggeredMuons)= ', len(event.ZselNoTriggeredMuons)
        
        # check for muon pair (with at least 1 triggering muon) which give invariant mass closest to Z pole
        # event.BestZMuonPairList = self.BestZMuonPair(event.ZselTriggeredMuons,event.ZselNoTriggeredMuons)
        # event.BestZMuonPairList = BestZMuonPair(self,event.ZselNoTriggeredMuons) # FROM MARKUS: THIS IS WRONG !!!
        event.BestZMuonPairList = BestJPsiMuonPair(self,event.ZallMuons) # JPSI !!!
        #                                            mZ                   mu1 (always firing trigger)              mu2                     mu2 has fired trigger?
        # print 'event.BestZMuonPairList= ',event.BestZMuonPairList[0],' ',event.BestZMuonPairList[1],' ',event.BestZMuonPairList[2],' ',event.BestZMuonPairList[3]
                  
        # check that a good muon pair exists, otherwise reject reco event
        if event.BestZMuonPairList[0] > 1e6 :
          # return True, 'good muon pair not found (probably same charge)'
          if hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents:
            return False
          else:
            return True, 'good muon pair not found'
        else:
            if fillCounter : self.counters.counter('JPsiAna').inc('Z good muon pair found')          
        
        # associate properly positive and negative muons
        if(event.BestZMuonPairList[1].charge()>0):
          event.BestZPosMuon = event.BestZMuonPairList[1]  
          event.BestZNegMuon = event.BestZMuonPairList[2]
        else:
          event.BestZPosMuon = event.BestZMuonPairList[2]
          event.BestZNegMuon = event.BestZMuonPairList[1]
        
        event.BestZNegMuonHasTriggered = 0
        event.BestZPosMuonHasTriggered = 0
        
          # check the triggers 
        # if(trigMatched(self,event,event.BestZPosMuon)):
            # event.BestZPosMuonHasTriggered = 1
        # else:
            # event.BestZPosMuonHasTriggered = 0
        # if(trigMatched(self,event,event.BestZNegMuon)):
            # event.BestZNegMuonHasTriggered = 1
        # else:
            # event.BestZNegMuonHasTriggered = 0
        if hasattr(self.cfg_ana, 'triggerBits'):
            for T, TL in self.cfg_ana.triggerBits.iteritems():
              if(trigMatched(self, event, event.BestZPosMuon, TL)):
                event.BestZPosMuonHasTriggered = 1
                break
        else:
            event.BestZPosMuonHasTriggered = 0
        
        if hasattr(self.cfg_ana, 'triggerBits'):
            for T, TL in self.cfg_ana.triggerBits.iteritems():
              if(trigMatched(self, event, event.BestZNegMuon, TL)):
                event.BestZNegMuonHasTriggered = 1
                break
        else:
            event.BestZNegMuonHasTriggered = 0

        event.ZselNoTriggeredExtraMuonsLeadingPt = [lep for lep in event.ZallMuons if \
                        lep !=event.BestZMuonPairList[2] and lep !=event.BestZMuonPairList[1]]
            
#        if(len(event.ZselNoTriggeredExtraMuonsLeadingPt)>0  and lep.pt()>10):
#          return True, 'rejecting, non triggering leading extra muon has pT > 10 GeV'
          # print 'len(event.ZallMuons)= ',len(event.ZallMuons),' len(event.ZselTriggeredMuons)= ',len(event.ZselTriggeredMuons),' len(event.ZselNoTriggeredMuons)= ', len(event.ZselNoTriggeredMuons)
          # print 'event.BestZMuonPairList= ',event.BestZMuonPairList[0],' ',event.BestZMuonPairList[1],' ',event.BestZMuonPairList[2],' ',event.BestZMuonPairList[3],' ',event.ZselNoTriggeredExtraMuonsLeadingPt[0].pt()
#        else:
#            if fillCounter : self.counters.counter('JPsiAna').inc('Z non trg leading extra muon pT < 10 GeV')
            
        # if the genp are saved, compute dR between gen and reco muons
        if event.savegenpZ :
          event.muPosGenDeltaRgenP = deltaR( event.BestZPosMuon.eta(), event.BestZPosMuon.phi(), event.genMuPos[0].eta(), event.genMuPos[0].phi() ) 
          event.muNegGenDeltaRgenP = deltaR( event.BestZNegMuon.eta(), event.BestZNegMuon.phi(), event.genMuNeg[0].eta(), event.genMuNeg[0].phi() ) 
            
        # associate lepton to good vertex to muons to compute dxy
        event.BestZPosMuon.associatedVertex = event.goodVertices[0]
        event.BestZNegMuon.associatedVertex = event.goodVertices[0]
        
        # print 'event.firstVtxIsGoodVertices', event.firstVtxIsGoodVertices
        
        # testing offline muon cuts (tight+iso, no kinematical cuts)
        event.BestZPosMuonIsTightAndIso=0
        if testLeg(self, event.BestZPosMuon ):
            event.BestZPosMuonIsTightAndIso=1
        event.BestZNegMuonIsTightAndIso=0
        if testLeg(self, event.BestZNegMuon ):
            event.BestZNegMuonIsTightAndIso=1
        event.BestZPosMuonIsTight=0
        if testLegID( self, event.BestZPosMuon ):
            event.BestZPosMuonIsTight=1
        event.BestZNegMuonIsTight=0
        if testLegID( self, event.BestZNegMuon ):
            event.BestZNegMuonIsTight=1

        event.covMatrixPosMuon = []
        RetrieveMuonMatrixIntoVector(self,event.BestZPosMuon,event.covMatrixPosMuon)
        event.covMatrixNegMuon = []
        RetrieveMuonMatrixIntoVector(self,event.BestZNegMuon,event.covMatrixNegMuon)
        
        # print event.BestZPosMuon.covarianceMatrix().Print("")
        # print event.covMatrixPosMuon
        # print event.BestZNegMuon.covarianceMatrix().Print("")
        # print event.covMatrixNegMuon
        
        event.BestZNegMatchIndex = matchCMGmuon(self,event,event.BestZNegMuon)
        event.BestZPosMatchIndex = matchCMGmuon(self,event,event.BestZPosMuon)
        # assign negative lepton to MET to build W+
        event.ZpfmetWpos = event.ZpfmetNoMu + event.BestZNegMuon.p4()
        # assign positive lepton to MET to build W-
        event.ZpfmetWneg = event.ZpfmetNoMu + event.BestZPosMuon.p4()        

        # define a positive W from positive lepton and MET
        event.Wpos4VfromZ = event.BestZPosMuon.p4() + event.ZpfmetWpos
        event.Wpos4VfromZ_mt = mT(self,event.BestZPosMuon.p4() , event.ZpfmetWpos)
        # define a negative W from negative lepton and MET
        event.Wneg4VfromZ = event.BestZNegMuon.p4() + event.ZpfmetWneg
        event.Wneg4VfromZ_mt = mT(self,event.BestZNegMuon.p4() , event.ZpfmetWneg)
        # define a Z from the two leptons
        event.Z4V = event.BestZPosMuon.p4() + event.BestZNegMuon.p4()
        event.Z4V_mt = mT(self,event.BestZPosMuon.p4() , event.BestZNegMuon.p4())
        
        # Code to study the Z recoil
        metVect = event.ZpfmetNoMu.Vect()
        metVect.SetZ(0.) # use only transverse info
        ZVect = event.Z4V.Vect()
        ZVect.SetZ(0.) # use only transverse info
        recoilVect = - copy.deepcopy(metVect)  ## FIXED (met sign inverted) vU = - vMET - vZ
        recoilVect -= ZVect
        #print 'What must be true'
        uZVect = ZVect.Unit()
        zAxis = type(ZVect)(0,0,1)
        uZVectPerp = ZVect.Cross(zAxis).Unit()

        u1 = recoilVect.Dot(uZVect) # recoil parallel to Z pt
        u2 = - recoilVect.Dot(uZVectPerp) # recoil perpendicular to Z pt

        event.Zu1 = u1
        event.Zu2 = u2
        
        if fillCounter:
          if event.Wpos4VfromZ.M() > 2.8 and event.Wpos4VfromZ.M() < 15.: 
            self.counters.counter('JPsiAna').inc('Z Inv Mass>50')
            if event.BestZPosMuonIsTight : 
              self.counters.counter('JPsiAna').inc('Z pos Mu is MuIsTightAndIso')
              if testLegKine(self, event.BestZPosMuon , 4 , 2.1 ) : 
                self.counters.counter('JPsiAna').inc('Z pos Mu_eta<2.1 && Mu_pt>30*MZ/MW')
                if testLegKine(self, event.BestZNegMuon , 4 , 2.4 ) : 
                  self.counters.counter('JPsiAna').inc('Z neg Mu_eta<2.4 && Mu_pt>10')
                  if event.ZpfmetWpos.Pt() >25: 
                    self.counters.counter('JPsiAna').inc('Z pfmet>25*MZ/MW')
                    if event.Wpos4VfromZ.Pt() < 20*91.1876/80.385: 
                      self.counters.counter('JPsiAna').inc('Z pt<20*MZ/MW')
                      if len(event.ZselJets) > 0: 
                        if event.ZselJets[0].pt() < 30: 
                          self.counters.counter('JPsiAna').inc('Z Jet_leading_pt<30')
                      else:
                        self.counters.counter('JPsiAna').inc('Z Jet_leading_pt<30')

        
        # event is fully considered as good
        # self.counters.counter('JPsiAna').inc('Z pass')
        event.ZGoodEvent = True
        
        if hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents:
          if( \
            not event.passedVertexAnalyzer 
            # or not event.passedTriggerAnalyzer 
            # or not ((event.Z4V.M()>2.8 and event.Z4V.M()<3.35) 
            # or (event.Z4V.M()>8.5 and event.Z4V.M()<11.5)
            or not ((event.Z4V.M()>2.8 and event.Z4V.M()<11.5) 
            ) \
            or not event.BestZPosMuon.charge() != event.BestZNegMuon.charge() \
            or not event.BestZPosMuonIsTight == 1 or not event.BestZNegMuonIsTight == 1 \
            or not event.BestZPosMuon.pt() > 4 or not math.fabs(event.BestZNegMuon.pt()) > 4 \
            # or not event.BestZPosMuon.relIso(0.5) < 0.5 or not event.BestZNegMuon.relIso(0.5) < 0.5 \
            or not math.fabs(event.BestZPosMuon.eta()) < 2.4 or not math.fabs(event.BestZNegMuon.eta()) < 2.4 \
            or not math.fabs(event.BestZPosMuon.dxy()) < 0.2 or not math.fabs(event.BestZNegMuon.dxy()) < 0.2 \
            or not math.fabs(event.BestZPosMuon.dz()) < 0.5 or not math.fabs(event.BestZNegMuon.dz()) < 0.5 \
          ):
            return False
          # && 
          # (
            # (
              # MuPos_pt>25 && MuPosRelIso<0.12 && MuPosTrg
              # && MuNegRelIso<0.5
            # )
            # ||
            # (
              # MuNeg_pt>25 && MuNegRelIso<0.12 && MuNegTrg
              # && MuPosIsTight
            # )

      #  print 'must be true'
       # if event.BestZNegMuonHasTriggered+event.BestZPosMuonHasTriggered==0 :
       #     print 'must be true' 
        
        # # case h:     return thecharge*211; // pi+
        # # case h0:    return 130; // K_L0
        # # case gamma: return 22;
        # # case h_HF:         return 1; // dummy pdg code 
        # # case egamma_HF:    return 2;  // dummy pdg code
        # # case e:     return thecharge*(-11);
        # # case mu:    return thecharge*(-13);
        # # case X: 
        # # default:    return 0;
        
        # # fromPV: 0 (doesn't check) ; 1 (selectedFromPV) ; 2 (selectedFromPUPV) ; 
        # # event.customMetPdgId = [[211],[130],[22],[1,2],[11],[13]]
        
        # # event.customMetFlavor =        [[211], [130],  [22] ,[1,2],[11] ,[13]]
        # # event.customMetFlavor_str =    [ 'h' ,  'h0','gamma','hf' ,'ele','mu']
        # # event.customMetPtMin =     [0.0,  0.5,  1.0,  1.5,  2.0]
        # # event.customMetPtMin_str = ['0p0','0p5','1p0','1p5','2p0']
        # # event.customMetEtaMin =     [ 0.0,  1.4,  2.1,  2.5,  3.0]
        # # event.customMetEtaMin_str = ['0p0','1p4','2p1','2p5','3p0']
        # # event.customMetEtaMax =     [ 1.4,  2.1,  2.5,  3.0,  5.0]
        # # event.customMetEtaMax_str = ['1p4','2p1','2p5','3p0','5p0']
        # event.customMetFlavor =        [[130],  [22] ,[1,2]]
        # event.customMetFlavor_str =    ['h0','gamma','hf']
        # event.customMetPtMin =     [0.0,  1.0, 2.0]
        # event.customMetPtMin_str = ['0p0','1p0','2p0']
        # event.customMetEtaMin =     [ 0.0,  1.4,  2.5,  3.0]
        # event.customMetEtaMin_str = ['0p0','1p4','2p5','3p0']
        # event.customMetEtaMax =     [ 1.4,  2.5,  3.0,  5.0]
        # event.customMetEtaMax_str = ['1p4','2p5','3p0','5p0']
        
        # # event.meth = [[0 for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))]
        # # event.meth0 = [[0 for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))]
        # # event.metgamma = [[0 for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))]
        # # event.methf = [[0 for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))]
        # # event.metele = [[0 for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))]
        # # event.metmu = [[0 for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))]
        # event.customMet = [[[[ROOT.ROOT.Math.LorentzVector('ROOT::Math::PxPyPzM4D<double>')(),0] for x in xrange(len(event.customMetEtaMax))] for x in xrange(len(event.customMetPtMin))] for x in xrange(len(event.customMetFlavor))]
        
        # for iFlavor in xrange(len(event.customMetFlavor)):
          # for iPtMin in xrange(len(event.customMetPtMin)):
            # for jEta in xrange(len(event.customMetEtaMax)):

              # if event.customMetFlavor[iFlavor]=='ele' or event.customMetFlavor[iFlavor]=='mu':
                # met4v = ROOT.ROOT.Math.LorentzVector('ROOT::Math::PxPyPzM4D<double>')()
                # event.customMet[iFlavor][iPtMin][jEta] = [met4v,0]
              # else:
                # # print event.customMetFlavor_str[iFlavor], event.customMetPtMin[iPtMin], event.customMetEtaMin[jEta], event.customMetEtaMax[jEta]
                # event.customMet[iFlavor][iPtMin][jEta]  = ComputeMetFromCMGCandidatesWithCuts(self,\
                                                          # event.cmgPFcands,event.customMetFlavor[iFlavor],\
                                                          # event.customMetPtMin[iPtMin],event.customMetEtaMin[jEta],event.customMetEtaMax[jEta],\
                                                          # event.goodVertices[0],1,0.1\
                                                          # )
            
        
        # # event.customMet  = computeMetFromCMGCandidatesWithCuts_FAST(self,\
                                                  # # event.cmgPFcands,event.customMetFlavor,\
                                                        # # event.customMetPtMin,event.customMetEtaMin,event.customMetEtaMax,\
                                                        # # event.goodVertices[0],1,0.1\
                                                  # # )
            
        
        return True
         
        
    def declareHandles(self):        
        super(JPsiAnalyzer, self).declareHandles()
        # self.handles['cmgTriggerObjectSel'] =  AutoHandle('cmgTriggerObjectSel','std::vector<cmg::TriggerObject>')
        # self.handles['Zmuons'] = AutoHandle('cmgMuonSel','std::vector<cmg::Muon>')
        self.handles['Zmuons'] = AutoHandle('jpsiMuons','std::vector<cmg::Muon>')
        self.handles['Zjets'] = AutoHandle('cmgPFJetSel','std::vector<cmg::PFJet>')
        # self.handles['kt6PFJets'] = AutoHandle('kt6PFJets','std::vector<reco::PFJet>')
        if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
          self.handles['cmgCandidates'] = AutoHandle('cmgCandidates','std::vector<cmg::Candidate>')
        self.handles['pfmet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>' )
        self.handles['ZElectrons'] = AutoHandle('cmgElectronSel','std::vector<cmg::Electron>')
        self.mchandles['genpart'] =  AutoHandle('genParticlesPruned','std::vector<reco::GenParticle>')
        # self.mchandles['LHEweights'] =  AutoHandle('source','LHEEventProduct')


