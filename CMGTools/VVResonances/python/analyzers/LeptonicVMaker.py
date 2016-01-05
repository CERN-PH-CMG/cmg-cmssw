import ROOT
import random
import math
from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.HeppyCore.utils.deltar import *
import PhysicsTools.HeppyCore.framework.config as cfg
from  itertools import combinations
from CMGTools.VVResonances.tools.Pair import *


class LeptonicVMaker( Analyzer ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(LeptonicVMaker,self).__init__(cfg_ana, cfg_comp, looperName)
        self.selectLNuPair = cfg_ana.selectLNuPair
        self.selectLLPair = cfg_ana.selectLLPair

    def declareHandles(self):
        super(LeptonicVMaker, self).declareHandles()


    def makeDiLeptons(self,leptonList):
        output=[]
        for l1,l2 in combinations(leptonList,2):
            if  (l1.pdgId() == -l2.pdgId()):
                pair = Pair(l1,l2,23)
                if abs(l1.pdgId())==11:                  
                    self.eeIsolationFootPrint(pair)
                elif abs(l1.pdgId())==13:                      
                    self.mumuIsolationFootPrint(pair)
                m=pair.p4().mass()
                if self.selectLLPair(pair):
#                    print 'New Z with mass ',m
                    output.append(pair)
        return output            


    def muIsolationFootPrint(self,z):#does nothing / for common interface
        footPrintLeg1=0.0
        z.leg1.cleanedChargedIso = max(z.leg1.pfIsolationR04().sumChargedHadronPt,0.0)                    
        z.leg1.cleanedNeutralIsoDB=max( z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-z.leg1.puChargedHadronIsoR(0.4)/2, 0.0)
        z.leg1.cleanedNeutralIsoRho=max(z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-z.leg1.rho*z.leg1.EffectiveArea04,0)        



    def mumuIsolationFootPrint(self,z):
        footPrintLeg1=0.0
        footPrintLeg2=0.0
        dr=deltaR(z.leg1.eta(),z.leg1.phi(),z.leg2.eta(),z.leg2.phi())
        if dr<0.4 and not z.leg2.isPFMuon():
            footPrintLeg1=z.leg2.pt()
        if dr<0.4 and not z.leg1.isPFMuon():
            footPrintLeg2=z.leg2.pt()

        z.leg1.cleanedChargedIso = max(z.leg1.pfIsolationR04().sumChargedHadronPt-footPrintLeg1,0.0)                    
        z.leg2.cleanedChargedIso = max(z.leg2.pfIsolationR04().sumChargedHadronPt-footPrintLeg2,0.0)                    
        z.leg1.cleanedNeutralIsoDB=max( z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-z.leg1.puChargedHadronIsoR(0.4)/2, 0.0)
        z.leg1.cleanedNeutralIsoRho=max(z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-z.leg1.rho*z.leg1.EffectiveArea04,0)        
        z.leg2.cleanedNeutralIsoDB=max( z.leg2.neutralHadronIsoR(0.4)+z.leg2.photonIsoR(0.4)-z.leg2.puChargedHadronIsoR(0.4)/2, 0.0)
        z.leg2.cleanedNeutralIsoRho=max(z.leg2.neutralHadronIsoR(0.4)+z.leg2.photonIsoR(0.4)-z.leg2.rho*z.leg2.EffectiveArea04,0)        



    def eIsolationFootPrint(self,z):
        footPrintChargedLeg1=0.0
        footPrintNeutralLeg1=0.0

        if not z.leg1.isPF():

            for i in range(0,z.leg1.associatedPackedPFCandidates().size()):
                c=z.leg1.associatedPackedPFCandidates()[i]
                dr = deltaR(z.leg1.eta(),z.leg1.phi(),c.eta(),c.phi())
                if z.leg1.isEB() or (dr>0.08 and dr<0.4):
                    if c.charge()>0:
                        footPrintChargedLeg1=footPrintChargedLeg1+c.pt()
                    else:    
                        footPrintNeutralLeg1=footPrintNeutralLeg1+c.pt()

        z.leg1.cleanedChargedIso = max(z.leg1.chargedHadronIsoR(0.4)-footPrintChargedLeg1,0.0)                    
        z.leg1.cleanedNeutralIsoDB=max( z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-footPrintNeutralLeg1-z.leg1.puChargedHadronIsoR(0.4)/2, 0.0)
        z.leg1.cleanedNeutralIsoRho=max(z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-footPrintNeutralLeg1-z.leg1.rho*z.leg1.EffectiveArea04,0)        

    def eeIsolationFootPrint(self,z):
        footPrintChargedLeg1=0.0
        footPrintChargedLeg2=0.0
        footPrintNeutralLeg1=0.0
        footPrintNeutralLeg2=0.0

        if not z.leg1.isPF():

            for i in range(0,z.leg1.associatedPackedPFCandidates().size()):
                c=z.leg1.associatedPackedPFCandidates()[i]
                dr = deltaR(z.leg1.eta(),z.leg1.phi(),c.eta(),c.phi())
                if z.leg1.isEB() or (dr>0.08 and dr<0.4):
                    if c.charge()>0:
                        footPrintChargedLeg1=footPrintChargedLeg1+c.pt()
                    else:    
                        footPrintNeutralLeg1=footPrintNeutralLeg1+c.pt()
                dr = deltaR(z.leg2.eta(),z.leg2.phi(),c.eta(),c.phi())
                if z.leg2.isEB() or (dr>0.08 and dr<0.4):
                    if c.charge()>0:
                        footPrintChargedLeg2=footPrintChargedLeg2+c.pt()
                    else:    
                        footPrintNeutralLeg2=footPrintNeutralLeg2+c.pt()

        if not z.leg2.isPF():
            for i in range(0,z.leg2.associatedPackedPFCandidates().size()):
                c=z.leg2.associatedPackedPFCandidates()[i]
                dr = deltaR(z.leg2.eta(),z.leg2.phi(),c.eta(),c.phi())
                if z.leg2.isEB() or (dr>0.08 and dr<0.4):
                    if c.charge()>0:
                        footPrintChargedLeg2=footPrintChargedLeg2+c.pt()
                    else:    
                        footPrintNeutralLeg2=footPrintNeutralLeg2+c.pt()
                dr = deltaR(z.leg1.eta(),z.leg1.phi(),c.eta(),c.phi())
                if z.leg1.isEB() or (dr>0.08 and dr<0.4):
                    if c.charge()>0:
                        footPrintChargedLeg1=footPrintChargedLeg1+c.pt()
                    else:    
                        footPrintNeutralLeg1=footPrintNeutralLeg1+c.pt()


        z.leg1.cleanedChargedIso = max(z.leg1.chargedHadronIsoR(0.4)-footPrintChargedLeg1,0.0)                    
        z.leg2.cleanedChargedIso = max(z.leg1.chargedHadronIsoR(0.4)-footPrintChargedLeg2,0.0)                    
        z.leg1.cleanedNeutralIsoDB=max( z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-footPrintNeutralLeg1-z.leg1.puChargedHadronIsoR(0.4)/2, 0.0)
        z.leg1.cleanedNeutralIsoRho=max(z.leg1.neutralHadronIsoR(0.4)+z.leg1.photonIsoR(0.4)-footPrintNeutralLeg1-z.leg1.rho*z.leg1.EffectiveArea04,0)        
        z.leg2.cleanedNeutralIsoDB=max( z.leg2.neutralHadronIsoR(0.4)+z.leg2.photonIsoR(0.4)-footPrintNeutralLeg2-z.leg2.puChargedHadronIsoR(0.4)/2, 0.0)
        z.leg2.cleanedNeutralIsoRho=max(z.leg2.neutralHadronIsoR(0.4)+z.leg2.photonIsoR(0.4)--footPrintNeutralLeg2-z.leg2.rho*z.leg2.EffectiveArea04,0)        


    def simpleWKinematicFit(self,pair):
        MW=80.390

        muonLV = ROOT.TLorentzVector(pair.leg1.px(),pair.leg1.py(),pair.leg1.pz(),pair.leg1.energy())
        metLV = ROOT.TLorentzVector(pair.leg2.px(),pair.leg2.py(),pair.leg2.pz(),pair.leg2.energy())

        #go to the rest frame of a muon
        muonBoost = ROOT.TVector3(0.0,0.0,-muonLV.BoostVector().Z())
        
        muonLV.Boost(muonBoost)
        metLV.Boost(muonBoost)

        u = (MW*MW+2*muonLV.Px()*metLV.Px()+2*muonLV.Py()*metLV.Py())/(2*muonLV.Energy())
        u=u*u-metLV.Px()*metLV.Px()-metLV.Py()*metLV.Py()
        if u<0.0:
            pair.alternateLV=pair.LV
            return


        #First solution
        metLV2 = ROOT.TLorentzVector(metLV)

        metLV.SetPxPyPzE(metLV.Px(),metLV.Py(),-math.sqrt(u),math.sqrt(metLV.Px()*metLV.Px()+metLV.Py()*metLV.Py()+u))
        metLV2.SetPxPyPzE(metLV.Px(),metLV.Py(),math.sqrt(u),math.sqrt(metLV.Px()*metLV.Px()+metLV.Py()*metLV.Py()+u))

        muonLV.Boost(-muonBoost)
        metLV.Boost(-muonBoost)
        metLV2.Boost(-muonBoost)

#        print 'Muon Z',muonLV.Pz() , 'METz 1',metLV.Pz(),'METz2',metLV2.Pz(),'Delta1',abs(muonLV.Pz()-metLV.Pz()),'Delta2',abs(muonLV.Pz()-metLV2.Pz()),math.cos(muonLV.Angle(metLV.Vect())),'Angles',math.cos(muonLV.Angle(metLV.Vect())),math.cos(muonLV.Angle(metLV2.Vect()))

        
#        W1=metLV+muonLV
#        W2=metLV2+muonLV
        
        p2 =pair.leg2.p4()
        p2.SetPxPyPzE(metLV.Px(),metLV.Py(),metLV.Pz(),metLV.Energy())
        if abs(metLV2.Pz())>abs(metLV.Pz()):
            pair.LV = pair.leg1.p4()+p2
            p2.SetPxPyPzE(metLV2.Px(),metLV2.Py(),metLV2.Pz(),metLV2.Energy())
            pair.alternateLV = pair.leg1.p4()+p2
            p2.SetPxPyPzE(metLV.Px(),metLV.Py(),0.0,math.sqrt(metLV.Px()*metLV.Px()+metLV.Py()*metLV.Py()))
        else:    
            pair.alternateLV = pair.leg1.p4()+p2
            p2.SetPxPyPzE(metLV2.Px(),metLV2.Py(),metLV2.Pz(),metLV2.Energy())
            pair.LV = pair.leg1.p4()+p2
            p2.SetPxPyPzE(metLV.Px(),metLV.Py(),0.0,math.sqrt(metLV.Px()*metLV.Px()+metLV.Py()*metLV.Py()))



    def defaultWKinematicFit(self,pair):
        MW=80.390

        muLonLV = ROOT.TLorentzVector(pair.leg1.px(),pair.leg1.py(),pair.leg1.pz(),pair.leg1.energy())
        metLV = ROOT.TLorentzVector(pair.leg2.px(),pair.leg2.py(),pair.leg2.pz(),pair.leg2.energy())



        alpha = MW*MW+2*muonLV.Px()*metLV.Px()+2*muonLV.Py()*metLV.Py()
        A = 4*(muonLV.Energy()*muonLV.Energy()-muonLV.Pz()*muonLV.Pz()) 
        B = -4* alpha*muonLV.Pz()
        C = 4*muonLV.Energy()*muonLV.Energy()*(metLV.Px()*metLV.Px()+metLV.Py()*metLV.Py())-alpha*alpha
        D = B*B-4*A*C

        if D>0:
            pz1=(-B+math.sqrt(D))/(2*A)
            pz2=(-B-math.sqrt(D))/(2*A)
            if abs(pz1)<abs(pz2):
                pp1 =pair.leg2.p4()
                pp1.SetPxPyPzE(metLV.Px(),metLV.Py(),pz1,metLV.Energy())
                pair.LV=pair.leg1.p4()+pp1
                pp2 =pair.leg2.p4()
                pp2.SetPxPyPzE(metLV.Px(),metLV.Py(),pz2,metLV.Energy())
                pair.alternateLV=pair.leg1.p4()+pp2
            else:    
                pp1 =pair.leg2.p4()
                pp1.SetPxPyPzE(metLV.Px(),metLV.Py(),pz2,metLV.Energy())
                pair.LV=pair.leg1.p4()+pp1
                pp2 =pair.leg2.p4()
                pp2.SetPxPyPzE(metLV.Px(),metLV.Py(),pz1,metLV.Energy())
                pair.alternateLV=pair.leg1.p4()+pp2
        else:
            pz=-B/(2*A)
            pp =pair.leg2.p4()
            pp.SetPxPyPzE(metLV.Px(),metLV.Py(),pz,metLV.Energy())
            pair.LV=pair.leg1.p4()+pp
            pair.alternateLV=pair.LV

    def makeLeptonsMET(self,leptonList,MET):
        output=[]
        for l1 in leptonList:
            pair = Pair(l1,MET,l1.charge()*24)
            mt=pair.mt()
            if abs(l1.pdgId())==11:
                self.eIsolationFootPrint(pair)
            elif     abs(l1.pdgId())==13:
                self.muIsolationFootPrint(pair)
            if self.selectLNuPair(pair):
#                self.defaultWKinematicFit(pair)
                self.simpleWKinematicFit(pair)
                output.append(pair)
        return output            

    
    def beginLoop(self, setup):
        super(LeptonicVMaker,self).beginLoop(setup)
        
    def process(self, event):
        self.readCollections( event.input )
        

        #make all first
        event.allLL=self.makeDiLeptons(event.selectedLeptons)
        event.LL = event.allLL
        event.allLNu=self.makeLeptonsMET(event.selectedLeptons,event.met)
        
        
        #now make Z first . for the remaining leptons after Z make W
        leptons=list(event.selectedLeptons)
        used = []


        for z in event.LL:
            used.extend([z.leg1,z.leg2])

        for u in used:
            leptons.remove(u)


        event.LNu = self.makeLeptonsMET(leptons,event.met)
        return True



        
            

        


                
                
