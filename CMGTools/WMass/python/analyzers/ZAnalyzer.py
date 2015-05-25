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

class ZAnalyzer( Analyzer ):

    ZMuonClass = Muon 
    ZJetClass = Jet 
    
    def beginLoop(self):
        super(ZAnalyzer,self).beginLoop()
        self.counters.addCounter('ZAna')
        count = self.counters.counter('ZAna')
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

                
#    def declareVariables(self):
#      tr = self.tree
#      var( tr, 'pfmet')
    
    def process(self, iEvent, event):
        # access event
        self.readCollections( iEvent )
        # access muons
        event.Zmuons = self.buildLeptons( self.handles['Zmuons'].product(), event )
        # access jets
        event.Zjets = self.buildJets( self.handles['Zjets'].product(), event )
        # access MET
        event.pfmet = self.handles['pfmet'].product()[0]
        event.tkmet = self.handles['tkmet'].product()[0]
        ## UNUSED
        #event.ZElectrons = self.buildLeptons( self.handles['ZElectrons'].product(), event )

        if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
          event.cmgPFcands = self.handles['cmgCandidates'].product()

        # access genP
        event.genParticles = []
        event.LHE_weights = []
        event.LHE_weights_str = []
        event.newLHE_weights = []
        if self.cfg_comp.isMC and self.cfg_ana.savegenp :
          event.genParticles = self.buildGenParticles( self.mchandles['genpart'].product(), event )
          # import ROOT
          # objectsPF = [ j for j in event.genParticles if (j.status()==1 and math.fabs(j.pdgId())!=12 and math.fabs(j.pdgId())!=14 and math.fabs(j.pdgId())!=16) ]
          # objectsTK = [ j for j in event.genParticles if (j.charge()!=0 and j.status()==1 and math.fabs(j.eta())<2.5) ]
         # # for i in objectsPF:
         # #     print 'charge=',i.charge(),' status',i.status(),' pt',i.pt(),' pdg',i.pdgId() 
          # event.genTkSumEt = sum([x.pt() for x in objectsTK])
          # event.genTkMet = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objectsTK])) , -1.*(sum([x.py() for x in objectsTK])), 0, math.hypot(-1.*sum([x.px() for x in objectsTK]),-1.*sum([x.py() for x in objectsTK])))
          # event.genPfSumEt = sum([x.pt() for x in objectsPF])
          # event.genPfMet = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in objectsPF])) , -1.*(sum([x.py() for x in objectsPF])), 0, math.hypot(-1.*sum([x.px() for x in objectsPF]),-1.*sum([x.py() for x in objectsPF])))
         # # print 'genTkMet=',event.genTkMet.pt(),' genPfMet=',event.genPfMet.pt()
          if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
            event.LHEweights = self.mchandles['LHEweights'].product()
            event.LHEweights_str = []
          if (hasattr(self.cfg_ana,'use_newWeights') and self.cfg_ana.use_newWeights):
            event.newLHEweights_str = self.mchandles['MyLHEProducer'].product()
          # event.LHEweights = []
        # define good event bool
        event.ZGoodEvent = False
          # select event
        return self.selectionSequence(event, fillCounter=True)
        
    def selectionSequence(self, event, fillCounter):

        if fillCounter: self.counters.counter('ZAna').inc('Z all triggered events')
        
        # retrieve collections of interest (muons and jets)
        
        if self.cfg_comp.isMC :
          if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
            for i in range(0,event.LHEweights.comments_size()):
              if not "rwgt" in event.LHEweights.getComment(i).split()[0]:
                event.LHE_weights.append(float(event.LHEweights.getComment(i).split()[1])/float(event.LHEweights.getComment(206).split()[1])) # CHECK THE 216 FOR THE SAMPLE IN USE !!!
                if (hasattr(self.cfg_ana,'use_newWeights') and self.cfg_ana.use_newWeights):
                  event.LHE_weights_str.append(event.LHEweights.getComment(i))
          
          # if (hasattr(self.cfg_ana,'use_newWeights') and self.cfg_ana.use_newWeights):
            # print 'standard weights'
            # for i in range(0,len(event.LHE_weights_str)):
              # print i, event.LHE_weights_str[i].split()[0], float(event.LHE_weights_str[i].split()[1])/float(event.LHEweights.getComment(206).split()[1]), event.LHE_weights_str[i].split()[2], event.LHE_weights_str[i].split()[3], event.LHE_weights_str[i].split()[4], event.LHE_weights_str[i].split()[5], event.LHE_weights_str[i].split()[6]
            
            if (hasattr(self.cfg_ana,'use_newWeights') and self.cfg_ana.use_newWeights):
              # print 'new weights'
              for i in range(0,event.newLHEweights_str.size()):
                if not "rwgt" in event.newLHEweights_str[i].split()[0]:
                  event.newLHE_weights.append(float(event.newLHEweights_str[i].split()[1])/float(event.LHEweights.getComment(206).split()[1])) # CHECK THE 216 FOR THE SAMPLE IN USE !!!
                  # print len(event.newLHE_weights)-1, event.newLHEweights_str[i].split()[0], float(event.newLHEweights_str[i].split()[1])/float(event.LHEweights.getComment(206).split()[1]), event.newLHEweights_str[i].split()[2], event.newLHEweights_str[i].split()[3], event.newLHEweights_str[i].split()[4], event.newLHEweights_str[i].split()[5], event.newLHEweights_str[i].split()[6]
              
              start_transplant_index = 0
              for i in range(0,len(event.newLHEweights_str)):
                if(event.LHE_weights_str[i].split()[6] == event.newLHEweights_str[0].split()[6]): 
                  # print 'i',i,'event.LHE_weights_str[i].split()[6]',event.LHE_weights_str[i].split()[6],'event.newLHEweights_str[0].split()[6]',event.newLHEweights_str[0].split()[6]
                  start_transplant_index = i
              # print 'start_transplant_index',start_transplant_index, 'event.LHEweights.getComment(206)',event.LHEweights.getComment(206)
              for i in range(0,len(event.newLHEweights_str)):
                event.LHE_weights[i+start_transplant_index] = float(event.newLHEweights_str[i].split()[1])/float(event.LHEweights.getComment(206).split()[1])
              
              # print 'standard weights after transplant'
              # for i in range(0,len(event.LHE_weights_str)):
                # print i, event.LHE_weights_str[i].split()[0], float(event.LHE_weights_str[i].split()[1])/float(event.LHEweights.getComment(206).split()[1]), '--->', event.LHE_weights[i], event.LHE_weights_str[i].split()[2], event.LHE_weights_str[i].split()[3], event.LHE_weights_str[i].split()[4], event.LHE_weights_str[i].split()[5], event.LHE_weights_str[i].split()[6]
            

        ##------------------------  Initial declaration of vectors --------------------------------------
          
        event.BestZPosMuonHasTriggered = 0
        event.BestZNegMuonHasTriggered = 0

     #   print 'looping on muons'
        event.ZallMuons = copy.copy(event.Zmuons)
        event.ZallMuonsTrig = my_n.zeros(50, dtype=int);
        event.ZallMuonsID = []
        event.ZallMuonsID_8TeV = []
        event.ZallMuonsMatched = []
        event.ZallMuonsSelMatched = []
        # if len(event.ZallMuons)>0:
            # for i in range(0, len(event.ZallMuons)):
                # if hasattr(self.cfg_ana, 'triggerBits'):
                    # for T, TL in self.cfg_ana.triggerBits.iteritems():
                      # if(trigMatched(self, event, event.ZallMuons[i], TL)):
                          # event.ZallMuonsTrig[i]=1.
                      # else:
                          # event.ZallMuonsTrig[i]=0.
                # else:
                    # event.ZallMuonsTrig[i]=0.
                
                # if((event.ZallMuons[i].isGlobalMuon() or event.ZallMuons[i].isTrackerMuon()) and len(event.goodVertices)>0):
                    # event.ZallMuons[i].associatedVertex = event.goodVertices[0]
                    # if testLegID(self, event.ZallMuons[i]):
                        # event.ZallMuonsID.append(1.)
                    # else:
                        # event.ZallMuonsID.append(0.)
                # else:
                    # event.ZallMuonsID.append(0.)


                # if((event.ZallMuons[i].isGlobalMuon() or event.ZallMuons[i].isTrackerMuon()) and len(event.goodVertices)>0):
                    # if testLegID_8TeV(self, event.ZallMuons[i]):
                        # event.ZallMuonsID_8TeV.append(1.)
                    # else:
                        # event.ZallMuonsID_8TeV.append(0.)
                # else:
                    # event.ZallMuonsID_8TeV.append(0.)


        event.ZselTriggeredMuons = []
        event.ZselNoTriggeredMuons = []
        event.ZselNoTriggeredExtraMuonsLeadingPt = []
        event.ZallJets = copy.copy(event.Zjets)
        event.ZselJets = []
          
        # store event MET and jets in all gen events (necessary to make cuts in genp studies...)
        # event.ZpfmetNoMu = event.pfmet.p4() # not needed
        # clean jets by removing muons
        event.ZselJets = [ jet for jet in event.ZallJets if ( jet.looseJetId() and jet.pt()>10 ) ]
        
                                                     
        ##------------------------  HERE MC related stuff --------------------------------------
        
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
            # for i in range(0, min(len(event.ZselElectrons),9)):
                # if  (matchPromt(self,event,event.ZselElectrons[i],-11*event.ZselElectrons[i].charge())+ matchPromtTau(self,event,event.ZselElectrons[i],-11*event.ZselElectrons[i].charge()))>0:
                    # event.ZElIsPromt.append(1)
                    # #print 'promt dude'
                # else:
                    # event.ZElIsPromt.append(0)
                    # #print 'just kidding'

        # save genp only for signal events
        # i.e. only one Z is present and daughters are muons
        
        if False:
            print "============", event.eventId,"==========================="
            for i,p in enumerate(event.genParticles):
                # print "\n :  %5d: pdgId %+5d status %3d  pt %6.1f  " % (i, p.pdgId(),p.status(),p.pt()),
                if abs(p.pdgId())==13:
                    print "\n muons:  %5d: pdgId %+5d status %3d  pt %6.1f  " % (i, p.pdgId(),p.status(),p.pt()),
                if abs(p.pdgId())==23:
                    print "\n Z: %5d: pdgId %+5d status %3d  pt %6.1f  " % (i, p.pdgId(),p.status(),p.pt()),
                if abs(p.pdgId())==13 or abs(p.pdgId())==23:
                    if p.numberOfMothers() > 0:
                        imom, mom = p.motherRef().key(), p.mother()
                        print " | mother %5d pdgId %+5d status %3d  pt %6.1f  " % (imom, mom.pdgId(),mom.status(),mom.pt()),
                    else:
                        print " | no mother particle                              ",

                        for j in xrange(min(3, p.numberOfDaughters())):
                            idau, dau = p.daughterRef(j).key(), p.daughter(j)
                            print " | dau[%d] %5d pdgId %+5d status %3d  pt %6.1f  " % (j,idau,dau.pdgId(),dau.status(),dau.pt()),
                            print ""
                            print "\n"
            print "\n ========================================================="

        foundZ = False
        for genp in event.genParticles:
          # if math.fabs(genp.pdgId())==23:
          if math.fabs(genp.pdgId())==23 and genp.status()==62:
            foundZ = True
            # print 'genp.pdgId()=',genp.pdgId(), 'genp.status()=',genp.status(), 'genp.numberOfDaughters()=',genp.numberOfDaughters()
            # # if(genp.numberOfDaughters()>0 and genp.status()==62):
            # if(genp.numberOfDaughters()>0):
              # print 'genp.daughter(0)',genp.daughter(0).pdgId(),'status',genp.daughter(0).status()
              # if(genp.numberOfDaughters()>1):
                # print 'genp.daughter(1)',genp.daughter(1).pdgId(),'status',genp.daughter(1).status()

        # if not foundZ: print 'NO Z FOUND!!!'
          
        genZ_dummy = [ genp for genp in event.genParticles if \
                             math.fabs(genp.pdgId())==23 and (self.cfg_ana.doMad or genp.status()==62 ) 
                             ]
        if len(genZ_dummy)==1:
          event.genZ = [ genp for genp in genZ_dummy if \
                               math.fabs(genp.daughter(0).pdgId())==13
                               ]
          event.genMuPos = []
          event.genMuNeg = []
          event.genMuPosStatus1 = []
          event.genMuNegStatus1 = []
          
          if len(event.genZ)==1:
          # if the genp event is selected, associate gen muons
            if(event.genZ[0].daughter(0).charge()>0):
              event.genMuPos.append(event.genZ[0].daughter(0))
              # print event.genZ[0].daughter(0).pdgId(),' event.genZ[0].daughter(0).charge()= ',event.genZ[0].daughter(0).charge()
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

        ##------------------------ HERE THERE is the selection --------------------------------------  
        
        # reco events must have good reco vertex and trigger fired...                       
        if (not event.passedVertexAnalyzer) or (event.vertices[0] != event.goodVertices[0]):
        # if (not event.passedVertexAnalyzer):
          # print 'PROBLEM'
          if not hasattr(self.cfg_ana,'keepFailingEvents') or (hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents):
            return False
          else:
            return True
        # ...and at lest two reco muons...
        if len(event.ZallMuons) < 2 :
            if not hasattr(self.cfg_ana,'keepFailingEvents') or (hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents):
              return False
            else:
              return True
        if fillCounter : self.counters.counter('ZAna').inc('Z >= 2 lepton')
        
        # check if the event is triggered according to cfg_ana
        # store separately muons that fired the trigger
        if hasattr(self.cfg_ana, 'triggerBits'):
            for lep in event.ZallMuons:
              for T, TL in self.cfg_ana.triggerBits.iteritems():
                  # muon object trigger matching
                  if(trigMatched(self, event, lep, TL)):
                    event.ZselTriggeredMuons.append(lep)
                    continue
            # for T, TL in self.cfg_ana.triggerBits.iteritems():
                # # muon object trigger matching
                # event.ZselTriggeredMuons = [lep for lep in event.ZallMuons if \
                                # trigMatched(self, event, lep, TL)]
                # # exit if there are no triggered muons
        if len(event.ZselTriggeredMuons) == 0:
            return (hasattr(self.cfg_ana,'keepFailingEvents') and self.cfg_ana.keepFailingEvents)
        else:
            if fillCounter: self.counters.counter('ZAna').inc('Z at least 1 lep trig matched')

               
        # # store muons that did not fire the trigger
        # event.ZselNoTriggeredMuons = [lep for lep in event.ZallMuons if \
                        # not trigMatched(self, event, lep, TL)]
        
        # check wether there are muons that did not fire the trigger, if so print some info
        # if len(event.ZselNoTriggeredMuons)>0:
        # print 'len(event.ZallMuons)= ',len(event.ZallMuons),' len(event.ZselTriggeredMuons)= ',len(event.ZselTriggeredMuons)
        # for imu in range (0,len(event.ZselTriggeredMuons)):
          # print event.ZselTriggeredMuons[imu].pt(),event.ZselTriggeredMuons[imu].eta()
        # ,' len(event.ZselNoTriggeredMuons)= ', len(event.ZselNoTriggeredMuons)
        
        # check for muon pair (with at least 1 triggering muon) which give invariant mass closest to Z pole
        event.BestZMuonPairList = BestZMuonPair(self,event.ZallMuons)
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
            if fillCounter : self.counters.counter('ZAna').inc('Z good muon pair found')          

        ##------------------------  MAKE THE MUONS  -------------------------------------- 
        
        # associate properly positive and negative muons
        if(event.BestZMuonPairList[1].charge()>0):
          event.BestZPosMuon = event.BestZMuonPairList[1]  
          event.BestZNegMuon = event.BestZMuonPairList[2]
        else:
          event.BestZPosMuon = event.BestZMuonPairList[2]
          event.BestZNegMuon = event.BestZMuonPairList[1]
          # check the triggers 
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
#            if fillCounter : self.counters.counter('ZAna').inc('Z non trg leading extra muon pT < 10 GeV')
            
        # if the genp are saved, compute dR between gen and reco muons
        if event.savegenpZ :
          event.muPosGenDeltaRgenP = deltaR( event.BestZPosMuon.eta(), event.BestZPosMuon.phi(), event.genMuPos[0].eta(), event.genMuPos[0].phi() ) 
          event.muNegGenDeltaRgenP = deltaR( event.BestZNegMuon.eta(), event.BestZNegMuon.phi(), event.genMuNeg[0].eta(), event.genMuNeg[0].phi() ) 
            
        # associate lepton to good vertex to muons to compute dxy
        event.BestZPosMuon.associatedVertex = event.goodVertices[0]
        event.BestZNegMuon.associatedVertex = event.goodVertices[0]
        
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

        ##------------------------  MAKE THE RECOIL variables  -------------------------------------- 

        # print event.BestZPosMuon.covarianceMatrix().Print("")
        # print event.covMatrixPosMuon
        # print event.BestZNegMuon.covarianceMatrix().Print("")
        # print event.covMatrixNegMuon
        
        event.BestZNegMatchIndex = matchCMGmuon(self,event,event.BestZNegMuon)
        event.BestZPosMatchIndex = matchCMGmuon(self,event,event.BestZPosMuon)
        # assign negative lepton to MET to build W+
        event.ZpfmetWpos = event.pfmet.p4() + event.BestZNegMuon.p4()
        # assign positive lepton to MET to build W-
        event.ZpfmetWneg = event.pfmet.p4() + event.BestZPosMuon.p4()        

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
        metVect = event.pfmet.p4().Vect()
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

        ##------------------------  FINAL COUNTERS  --------------------------------------  
        
        if fillCounter:
          if event.Wpos4VfromZ.M() > 50: 
            self.counters.counter('ZAna').inc('Z Inv Mass>50')
            if event.BestZPosMuonIsTightAndIso : 
              self.counters.counter('ZAna').inc('Z pos Mu is MuIsTightAndIso')
              if testLegKine(self, event.BestZPosMuon , 30*91.1876/80.385 , 2.1 ) : 
                self.counters.counter('ZAna').inc('Z pos Mu_eta<2.1 && Mu_pt>30*MZ/MW')
                if testLegKine(self, event.BestZNegMuon , 10 , 2.4 ) : 
                  self.counters.counter('ZAna').inc('Z neg Mu_eta<2.4 && Mu_pt>10')
                  if event.ZpfmetWpos.Pt() >25*91.1876/80.385: 
                    self.counters.counter('ZAna').inc('Z pfmet>25*MZ/MW')
                    if event.Wpos4VfromZ.Pt() < 20*91.1876/80.385: 
                      self.counters.counter('ZAna').inc('Z pt<20*MZ/MW')
                      if len(event.ZselJets) > 0: 
                        if event.ZselJets[0].pt() < 30: 
                          self.counters.counter('ZAna').inc('Z Jet_leading_pt<30')
                      else:
                        self.counters.counter('ZAna').inc('Z Jet_leading_pt<30')

        
        # event is fully considered as good
        event.ZGoodEvent = True
        
        ##------------------------  EXTRA  --------------------------------------  
        
        if not hasattr(self.cfg_ana,'keepFailingEvents') or (hasattr(self.cfg_ana,'keepFailingEvents') and not self.cfg_ana.keepFailingEvents):
          if( \
            not event.passedVertexAnalyzer \
            # or not event.passedTriggerAnalyzer
            # or not event.Z4V.M()>70 or not event.Z4V.M()<110 \
            or not event.Z4V.M()>50 \
            ## commented Zpt 
            ##or not event.Z4V.Pt()<50 \
            or not event.BestZPosMuon.charge() != event.BestZNegMuon.charge() \
            or not event.BestZPosMuonIsTight == 1 or not event.BestZNegMuonIsTight == 1 \
            
            # or not event.BestZPosMuon.pt() > 6 or not math.fabs(event.BestZNegMuon.pt()) > 6 \
            # or not event.BestZPosMuon.relIso(0.5) < 0.5 or not event.BestZNegMuon.relIso(0.5) < 0.5 \
            # or not math.fabs(event.BestZPosMuon.eta()) < 2.4 or not math.fabs(event.BestZNegMuon.eta()) < 2.4 \
            # or not math.fabs(event.BestZPosMuon.dxy()) < 0.2 or not math.fabs(event.BestZNegMuon.dxy()) < 0.2 \
            # or not math.fabs(event.BestZPosMuon.dz()) < 0.5 or not math.fabs(event.BestZNegMuon.dz()) < 0.5 \
            or not ( \
                    ( event.BestZPosMuon.pt() > 30 and event.BestZPosMuon.relIso(0.5) < 0.12 \
                    and math.fabs(event.BestZPosMuon.eta()) < 2.1 and math.fabs(event.BestZPosMuon.dxy()) < 0.2 \
                    # and math.fabs(event.BestZPosMuon.dz()) < 0.5 ) \
                    and math.fabs(event.BestZPosMuon.dz()) < 0.1  \
                    and math.fabs(event.BestZNegMuon.dz()) < 0.1 ) \
                    or\
                    ( event.BestZNegMuon.pt() > 30 and event.BestZNegMuon.relIso(0.5) < 0.12 \
                    and math.fabs(event.BestZNegMuon.eta()) < 2.1 and math.fabs(event.BestZNegMuon.dxy()) < 0.2 \
                    # and math.fabs(event.BestZNegMuon.dz()) < 0.5 ) \
                    and math.fabs(event.BestZNegMuon.dz()) < 0.1  \
                    and math.fabs(event.BestZPosMuon.dz()) < 0.1 ) \
                    )\
          ):
            return False
        
        if(event.tkmet.sumEt()-event.BestZPosMuon.pt()-event.BestZNegMuon.pt()<0):
          print 'event.passedVertexAnalyzer',event.passedVertexAnalyzer,'event.vertices[0].ndof()',event.vertices[0].ndof(),'event.goodVertices[0].ndof()',event.goodVertices[0].ndof(),'PROBLEM?',event.vertices[0] != event.goodVertices[0]
          print 'tkmet_SumEt-mupos-muneg',event.tkmet.sumEt()-event.BestZPosMuon.pt()-event.BestZNegMuon.pt()
          print 'event.BestZPosMuon.dz()',event.BestZPosMuon.dz(),'event.BestZNegMuon.dz()',event.BestZNegMuon.dz()
        return True
         
        
    def declareHandles(self):        
        super(ZAnalyzer, self).declareHandles()
##        self.handles['TriggerResults'] = AutoHandle( ('TriggerResults','','HLT'), 'edm::TriggerResults' )
##        self.handles['ZElectrons'] = AutoHandle('cmgElectronSel','std::vector<cmg::Electron>')
        self.handles['Zmuons'] = AutoHandle('cmgMuonSel','std::vector<cmg::Muon>')
        self.handles['Zjets'] = AutoHandle('cmgPFJetSel','std::vector<cmg::PFJet>')
        if hasattr(self.cfg_ana,'storeNeutralCMGcandidates') or hasattr(self.cfg_ana,'storeCMGcandidates'):
          self.handles['cmgCandidates'] = AutoHandle('cmgCandidates','std::vector<cmg::Candidate>')
        self.handles['pfmet'] = AutoHandle('cmgPFMET','std::vector<cmg::BaseMET>' )
        self.handles['tkmet'] = AutoHandle('tkMet','std::vector<reco::PFMET>' )
        self.mchandles['genpart'] =  AutoHandle('genParticlesPruned','std::vector<reco::GenParticle>')
        if self.cfg_comp.isMC :
          if (hasattr(self.cfg_ana,'storeLHE_weight') and self.cfg_ana.storeLHE_weight):
            self.mchandles['LHEweights'] =  AutoHandle('source','LHEEventProduct')
          if (hasattr(self.cfg_ana,'use_newWeights') and self.cfg_ana.use_newWeights):
            self.mchandles['MyLHEProducer'] =  AutoHandle('MyLHEProducer','std::vector<std::string>')


                                        
