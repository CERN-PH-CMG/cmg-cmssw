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
                m=pair.p4().mass()
                if self.selectLLPair(pair):
#                    print 'New Z with mass ',m
                    output.append(pair)
        return output            


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

        muonLV = ROOT.TLorentzVector(pair.leg1.px(),pair.leg1.py(),pair.leg1.pz(),pair.leg1.energy())
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
            if self.selectLNuPair(pair):
                self.defaultWKinematicFit(pair)
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
        leptonsSet = set(event.selectedLeptons)
        used = []
        for z in event.LL:
            used.extend([z.leg1,z.leg2])
        usedSet = set(used)

        remaining = leptonsSet-usedSet
        event.LNu = self.makeLeptonsMET(list(remaining),event.met)
        return True



        
            

        


                
                
