import operator
import ROOT
import numpy as my_n
import copy
import math, os
from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle
from CMGTools.RootTools.physicsobjects.PhysicsObjects import Muon, Jet, GenParticle
from CMGTools.RootTools.utils.TriggerMatching import triggerMatched
from CMGTools.RootTools.utils.DeltaR import bestMatch, deltaR, deltaR2
from CMGTools.WMass.analyzers.common_functions import *

 
# FROM Z 

def testJet(self, jet):
    '''returns testjetID && testjetIso && testjetKine for jet'''
    return jet.pt() > self.cfg_ana.jetptcut

def testJetIsNotSelLepton(self, jet, lep):
    '''returns testjetID && testjetIso && testjetKine for jet'''
    return deltaR( lep.p4().Eta(),lep.p4().Phi(), jet.p4().Eta(),jet.p4().Phi() )  > 0.2

def testLeg(self, leg):
    '''returns testLegID && testLegIso && testLegKine for leg'''
    return testLegID(self, leg) and \
           testLegIso(self, leg, self.cfg_ana.iso)
           # and self.testLegKine(leg, self.cfg_ana.pt, self.cfg_ana.eta)

# from Z
# def testLegID(self, leg):
    # '''Always return true by default, overload in your subclass'''
    # # return True
    # return (leg.tightId() and \
            # leg.dxy() < 0.2 and \
          # #  leg.dz() < 0.5 and \
            # leg.trackerLayersWithMeasurement() > 8)

def testLegID(self, leg):
    '''Always return true by default, overload in your subclass'''
    # return True
    return ( leg.tightId() and \
             math.fabs(leg.dxy()) < 0.2 and\
             math.fabs(leg.dz()) < 0.5 and\
             leg.trackerLayersWithMeasurement() > 8
             )

def testLegID_8TeV(self, leg):
    '''Always return true by default, overload in your subclass'''
    # return True
    return (leg.tightId() and \
            math.fabs(leg.dxy()) < 0.2 and \
            math.fabs(leg.dz()) < 0.5 and \
            leg.trackerLayersWithMeasurement() > 5)
            
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
   

def BestZMuonPair(self, Zmuons):
  mZpole = 91.1876
  mZ=1e10
  bestmu1=0
  bestmu2=0
  for lep1 in Zmuons:
    for lep2 in Zmuons:
      # if( lep1 != lep2 and lep1.charge() != lep2.charge() ):
      if( lep1 != lep2 ):
        if(math.fabs((lep1.p4()+lep2.p4()).M()-mZpole) < math.fabs(mZ-mZpole) ):
          mZ=(lep1.p4()+lep2.p4()).M()
          bestmu1=lep1
          bestmu2=lep2


  return mZ, bestmu1, bestmu2

def BestJPsiMuonPair(self, JPsimuons):
  mJPsipole = 3.096916
  mJPsi=1e10
  bestmu1=0
  bestmu2=0
  for lep1 in JPsimuons:
    for lep2 in JPsimuons:
      # if( lep1 != lep2 and lep1.charge() != lep2.charge() ):
      if( lep1 != lep2 ):
        if(math.fabs((lep1.p4()+lep2.p4()).M()-mJPsipole) < math.fabs(mJPsi-mJPsipole) ):
          mJPsi=(lep1.p4()+lep2.p4()).M()
          bestmu1=lep1
          bestmu2=lep2


  return mJPsi, bestmu1, bestmu2

# def BestZMuonPair(self, ZselTriggeredMuons, ZselNoTriggeredMuons):
    # mZpole = 91.1876
    # mZ=1e10
    # bestmu1=0
    # bestmu2=0
    # mu2hastriggered=0
    # for lep1 in ZselTriggeredMuons:
      # for lep2 in ZselTriggeredMuons:
        # # if( lep1 != lep2 and lep1.charge() != lep2.charge() ):
        # if( lep1 != lep2 ):
          # if(math.fabs((lep1.p4()+lep2.p4()).M()-mZpole) < math.fabs(mZ-mZpole) ):
            # mZ=(lep1.p4()+lep2.p4()).M()
            # bestmu1=lep1
            # bestmu2=lep2
            # mu2hastriggered=1
    # for lep1 in ZselTriggeredMuons:
      # for lep2 in ZselNoTriggeredMuons:
        # # if( lep1 != lep2 and lep1.charge() != lep2.charge() ): # charge check removed to allow same sign pairs for background studies
        # if( lep1 != lep2 ):
          # if(math.fabs((lep1.p4()+lep2.p4()).M()-mZpole) < math.fabs(mZ-mZpole) ):
            # mZ=(lep1.p4()+lep2.p4()).M()
            # bestmu1=lep1
            # bestmu2=lep2
            # mu2hastriggered=0

    # return mZ, bestmu1, bestmu2, mu2hastriggered


def mT(self, leg1, leg2):
    # print 'leg1.Pt() ',leg1.Pt(),' leg2.Pt() ',leg2.Pt(),' leg1.Px() ',leg1.Px(),' leg2.Px() ',leg2.Px(),' leg2.Py() ',leg2.Py(),' leg1.Pt() ',leg1.Pt(),' leg2.Pt() ',leg2.Pt(),' before sqrt ',( 2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) ) )
    if (2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) )) > 0:
        return math.sqrt( 2*leg1.Pt()*leg2.Pt()*( 1 - (leg1.Px()*leg2.Px() + leg1.Py()*leg2.Py()) / ( leg1.Pt()*leg2.Pt() ) ) )
    else:
        return 0

        
def trigMatched(self, event, leg):
    '''Returns true if the leg is matched to a trigger object as defined in the
    triggerMap parameter'''
    # print 'inside trigMatched'
    if not event.passedTriggerAnalyzer:
        # print 'not event.passedTriggerAnalyzer'
        return False
    if not hasattr( self.cfg_ana, 'triggerMap'):
        # print 'not hasattr( self.cfg_ana, \'triggerMap\')'
        return True
    path = event.hltPath
    # print 'path ', path
    triggerObjects = event.triggerObjects
    # print 'triggerObjects ',triggerObjects
    filters = self.cfg_ana.triggerMap[ path ]
    # print 'filters ',filters
    # the dR2Max value is 0.1^2
    # print 'triggerMatched ',triggerMatched(leg, triggerObjects, path, filters,dR2Max=0.01,pdgIds=None )
    return triggerMatched(leg, triggerObjects, path, filters,
                          dR2Max=0.01,
                          pdgIds=None )
                          
# def trigMatched(self, event, leg):
    # '''Returns true if the leg is matched to a trigger object as defined in the
    # triggerMap parameter'''
    # if not hasattr( self.cfg_ana, 'triggerMap'):
        # return True
    # path = event.hltPath
    # triggerObjects = event.triggerObjects
    # filters = self.cfg_ana.triggerMap[ path ]
    # # the dR2Max value is 0.1^2
    # return triggerMatched(leg, triggerObjects, path, filters,
                          # dR2Max=0.01,
                          # pdgIds=None )
                          
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
      return returnMuonDaughterStatus1(self, returnMuonDaughter(self,genp_muon_daughter))

      
def matchPromt(self, event, lep, particleID):

    for genp in event.genParticles:
        if  (deltaR( lep.p4().Eta(),lep.p4().Phi(), genp.p4().Eta(),genp.p4().Phi() )  < 0.2) and genp.status()==3 and genp.pdgId()==particleID: # status three are always promt
            return 1
    return 0


def matchPromtTau(self, event, lep, particleID):

    for genp in event.genParticles:
        if abs(genp.pdgId())==15:
            for k in range(0,genp.numberOfDaughters()):
               if(deltaR(genp.daughter(k).p4().Eta(),genp.daughter(k).p4().Phi(),lep.p4().Eta(),lep.p4().Phi() )  < 0.2) and genp.daughter(k).pdgId()==particleID:
                   return 1 
    return 0 

def matchCMGmuon(self, event, lep):
    for i in range(0, min(len(event.ZallMuons),10)):  
        if(deltaR(event.ZallMuons[i].p4().Eta(),event.ZallMuons[i].p4().Phi(),lep.p4().Eta(),lep.p4().Phi() )  < 0.1):
            return i 
    return -99

def testElectronIDMedium(self, leg):
    return (
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.004 ) or (leg.sourcePtr().isEE() and leg.sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.007) )  and \
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.060 ) or (leg.sourcePtr().isEE() and leg.sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.030) )  and \
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().sigmaIetaIeta() < 0.010                  ) or (leg.sourcePtr().isEE() and leg.sourcePtr().sigmaIetaIeta() < 0.030)                  )  and \
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().hadronicOverEm() < 0.120                 ) or (leg.sourcePtr().isEE() and leg.sourcePtr().hadronicOverEm() < 0.100)                 )  and \
        (                               ( 1.0/leg.sourcePtr().ecalEnergy() - leg.sourcePtr().eSuperClusterOverP()/leg.sourcePtr().ecalEnergy() ) < 0.050                                  )  and \
        (                                                                       leg.sourcePtr().passConversionVeto() == 1                                                                 )  and \
        (                                                            leg.sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() <= 1)
        )


def testElectronVtxMedium(self, electron, event):
    if len(event.goodVertices)>0:
        electron.associatedVertex = event.goodVertices[0]
        return( math.fabs(electron.sourcePtr().gsfTrack().dxy(event.goodVertices[0].position())) < 0.02 and \
                math.fabs(electron.sourcePtr().gsfTrack().dz(event.goodVertices[0].position())) < 0.1 and \
                electron.relIso(0.5) < 0.15 )
        
    else:
        return False

def testElectronVtxTight(self, electron, event):
    if len(event.goodVertices)>0:
        electron.associatedVertex = event.goodVertices[0]
        return( math.fabs(electron.sourcePtr().gsfTrack().dxy(event.goodVertices[0].position())) < 0.02 and \
                math.fabs(electron.sourcePtr().gsfTrack().dz(event.goodVertices[0].position())) < 0.1 and \
                electron.relIso(0.5) < 0.10 )
        
    else:
        return False
    
def testElectronIDTight(self, leg):
    return (
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.004 ) or (leg.sourcePtr().isEE() and leg.sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.007) )  and \
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.03 ) or (leg.sourcePtr().isEE() and leg.sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.030) )  and \
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().sigmaIetaIeta() < 0.010                  ) or (leg.sourcePtr().isEE() and leg.sourcePtr().sigmaIetaIeta() < 0.030)                  )  and \
        ( (leg.sourcePtr().isEB() and leg.sourcePtr().hadronicOverEm() < 0.120                 ) or (leg.sourcePtr().isEE() and leg.sourcePtr().hadronicOverEm() < 0.100)                 )  and \
        (                               ( 1.0/leg.sourcePtr().ecalEnergy() - leg.sourcePtr().eSuperClusterOverP()/leg.sourcePtr().ecalEnergy() ) < 0.050                                  )  and \
        (                                                                       leg.sourcePtr().passConversionVeto() == 1                                                                 )  and \
        (                                                            leg.sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() <= 0)
        )

# simpler would be
#leg.sourcePtr().electronID()
    #print lep.genparticle().pdgId()

                          
def RetrieveMuonMatrixIntoVector(self,muon,matrix):
    for i in range(0,3):
        for j in range(0,3):
            matrix.append(muon.covarianceMatrix()(i+3,j+3))
            # matrix[offset+i][offset+j] = muon.covarianceMatrix()(i+3,j+3)
                            
def ComputeMetFromCMGCandidatesWithCuts(self,collection,pdgId,ptmin,etamin,etamax,selectedPV,fromPV,dZfromPVcut):
    if len(collection) < 1: return [-1,-1,-1]
    met_x = 0
    met_y = 0
    sumet = 0
    met4v = ROOT.ROOT.Math.LorentzVector('ROOT::Math::PxPyPzM4D<double>')()
    # print 'starting ', met_x, met_y, sumet
    # REMINDER fromPV: 0 (doesn't check) ; 1 (selectedFromPV) ; 2 (selectedFromPUPV) ; 
    for candidate in collection:
      if -1 in pdgId or math.fabs(candidate.pdgId()) in pdgId:
        if(math.fabs(candidate.pdgId())==1 or math.fabs(candidate.pdgId())==2):
          etamin_loop = 2.0+etamin
          etamax_loop = 2.0+etamax
        else:
          etamin_loop = etamin
          etamax_loop = etamax
        if(math.fabs(candidate.eta()) > etamin_loop and math.fabs(candidate.eta()) < etamax_loop)\
           and candidate.pt() > ptmin \
           and ( \
                # doesn't check
                fromPV==0 \
                # only from selected PV passing the dz cut
                or (fromPV==1 and candidate.fromPV()==True and math.fabs(candidate.vertex().z()-selectedPV.z()) < dZfromPVcut) \
                # from pileup PV 
                or (fromPV==2 and candidate.fromPV()==False) \
                # plus candidates from selected PV failing the dz cut
                or (fromPV==2 and candidate.fromPV()==True and math.fabs(candidate.vertex().z()-selectedPV.z()) > dZfromPVcut) \
                ):

          # print candidate.pdgId(), candidate.pt(), candidate.eta(), candidate.fromPV(), math.fabs(candidate.vertex().z()-selectedPV.z())
          met_x = met_x - candidate.px()
          met_y = met_y - candidate.py()
          sumet = sumet + candidate.pt()
          # print met_x, met_y, sumet
        # else: print pdgId,etamin_loop,etamax_loop,'NOT USING',candidate.pdgId(), candidate.pt(), candidate.eta(), candidate.fromPV(), math.fabs(candidate.vertex().z()-selectedPV.z())
      # else: print pdgId,(-1 in pdgId or math.fabs(candidate.pdgId()) in pdgId),'NOT USING',candidate.pdgId(), candidate.pt(), candidate.eta(), candidate.fromPV(), math.fabs(candidate.vertex().z()-selectedPV.z())
    met4v.SetCoordinates(met_x,met_y,0,0)
    # print 'ending ', met_x, met_y, sumet, 'met4v ptm phi: ', met4v.Pt(), met4v.Phi()
    return [met4v, sumet]

def computeMetFromCMGCandidatesWithCuts_FAST(self,collection,v_flavor,v_ptmin,v_etamin,v_etamax,selectedPV,fromPV,dZfromPVcut):

    # [iFlavor][iPtMin][jEta][met_i] where met_i = [met_4v, sumet]
    customMet_FAST = [[[[ROOT.ROOT.Math.LorentzVector('ROOT::Math::PxPyPzM4D<double>')(), 0] for x in xrange(len(v_etamax))] for x in xrange(len(v_ptmin))] for x in xrange(len(v_flavor))]
    
    if len(collection) < 1: return
    
    # event.customMetFlavor_str =    [ 'h' ,  'h0','gamma','hf' ,'ele','mu']
    # event.customMetFlavor =        [[211], [130],  [22] ,[1,2],[11] ,[13]]
    # event.customMetPtMin =     [0.0,  0.5,  1.0,  1.5,  2.0]
    # event.customMetEtaMin =     [ 0.0,  1.4,  2.1,  2.5,  3.0]
    # event.customMetEtaMax =     [ 1.4,  2.1,  2.5,  3.0,  5.0]

    # REMINDER fromPV: 0 (doesn't check) ; 1 (selectedFromPV) ; 2 (selectedFromPUPV) ; 
    
    for candidate in collection:
      
      candidate_4v = ROOT.ROOT.Math.LorentzVector('ROOT::Math::PxPyPzM4D<double>')()
      candidate_4v.SetCoordinates(candidate.px(),candidate.py(),0,0)
      # print 'new candidate, pdg %.d, pt %.2f, eta %.2f'%(candidate.pdgId(), candidate.pt(), candidate.eta()), '*************************'
      
      # met definitions
      for iFlavor in xrange(len(v_flavor)):
        # print "probing flavor bin ",v_flavor[iFlavor]
        if( math.fabs(candidate.pdgId()) in v_flavor[iFlavor] or v_flavor[iFlavor] == -1 ):
          # print "passed flavor bin ",v_flavor[iFlavor]
        
          for iPtMin in xrange(len(v_ptmin)):
            # print "probing pt bin ",v_ptmin[iPtMin]
            if( candidate.pt() > v_ptmin[iPtMin]):
              # print "passed pt bin ",v_ptmin[iPtMin]
            
              for jEta in xrange(len(v_etamin)):
                # print "probing eta bin ",v_etamin[jEta],v_etamax[jEta]
                etamin_loop = v_etamin[jEta]
                etamax_loop = v_etamax[jEta]
                if( math.fabs(candidate.pdgId()) == 1 or math.fabs(candidate.pdgId()) == 2 ):
                  etamin_loop = etamin_loop + 2
                  etamax_loop = etamax_loop + 2
                if( math.fabs(candidate.eta()) > etamin_loop and math.fabs(candidate.eta()) < etamax_loop):
                  # print "passed eta bin ",v_etamin[jEta],v_etamax[jEta]

                  customMet_FAST[iFlavor][iPtMin][jEta][0] = customMet_FAST[iFlavor][iPtMin][jEta][0] - candidate_4v
                  customMet_FAST[iFlavor][iPtMin][jEta][1] = customMet_FAST[iFlavor][iPtMin][jEta][1] + candidate_4v.Pt()
                  # print "********** fits bin ",v_flavor[iFlavor], "%.1f"%v_ptmin[iPtMin], "%.1f"%v_etamin[jEta], "%.1f"%v_etamax[jEta], "met %.1f"%customMet_FAST[iFlavor][iPtMin][jEta][0].Pt(), "sumet %.1f"%customMet_FAST[iFlavor][iPtMin][jEta][1]
    
    return customMet_FAST
    
    
                            
                            
# FROM W
                          
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


  

                          
          

# class WBoson(object):

        # def __init__(self, lepton, met):
                # self.lepton = lepton
                # self.met = met
                # self.mt = None
