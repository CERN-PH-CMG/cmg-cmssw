import os
import ROOT
from CMGTools.RootTools.RootTools import *
from ROOT import gSystem

gSystem.Load("libCMGToolsHToZZTo4Leptons")


from ROOT import Mela


# you can even create a python class for high level stuff, like this: 
class MELACalculator(object):
    '''This class computes the mela angles, taking in input the 4-vectors of the leptons coming from the decay of the Zs from the Higgs.'''
    
    def __init__(self):
        self.costheta1=ROOT.Double(0)
        self.costheta2=ROOT.Double(0)
        self.phi=ROOT.Double(0)
        self.costhetastar=ROOT.Double(0)
        self.phistar1=ROOT.Double(0)
        self.phistar2=ROOT.Double(0)
        self.phistar12=ROOT.Double(0)
        self.phi1=ROOT.Double(0)
        self.phi2=ROOT.Double(0)

        self.melaCpp = Mela()
        
    def calculate(self,FL):
        

        self.costheta1=ROOT.Double(0)
        self.costheta2=ROOT.Double(0)
        self.phi=ROOT.Double(0)
        self.costhetastar=ROOT.Double(0)
        self.phistar1=ROOT.Double(0)
        self.phistar2=ROOT.Double(0)
        self.phistar12=ROOT.Double(0)
        self.phi1=ROOT.Double(0)
        self.phi2=ROOT.Double(0)

        self.MELAS = ROOT.Double(0)
        self.MELAB = ROOT.Double(0)
        self.MELA  = ROOT.Double(0) 
        

        l1 = ROOT.TLorentzVector(FL.leg1.leg1.px(),FL.leg1.leg1.py(),FL.leg1.leg1.pz(),FL.leg1.leg1.energy())
        l2 = ROOT.TLorentzVector(FL.leg1.leg2.px(),FL.leg1.leg2.py(),FL.leg1.leg2.pz(),FL.leg1.leg2.energy())
        l3 = ROOT.TLorentzVector(FL.leg2.leg1.px(),FL.leg2.leg1.py(),FL.leg2.leg1.pz(),FL.leg2.leg1.energy())
        l4 = ROOT.TLorentzVector(FL.leg2.leg2.px(),FL.leg2.leg2.py(),FL.leg2.leg2.pz(),FL.leg2.leg2.energy())

        if hasattr(FL.leg1,'fsrPhoton'):
            photon = ROOT.TLorentzVector(FL.leg1.fsrPhoton.px(),FL.leg1.fsrPhoton.py(),FL.leg1.fsrPhoton.pz(),FL.leg1.fsrPhoton.energy())
            if FL.leg1.fsrDR1() < FL.leg1.fsrDR2():
                l1 = l1+photon
            else:
                l2=l2+photon

        if hasattr(FL.leg2,'fsrPhoton'):
            photon = ROOT.TLorentzVector(FL.leg2.fsrPhoton.px(),FL.leg2.fsrPhoton.py(),FL.leg2.fsrPhoton.pz(),FL.leg2.fsrPhoton.energy())
            if FL.leg2.fsrDR1() < FL.leg2.fsrDR2():
                l3 = l3+photon
            else:
                l4=l4+photon
                
            
        self.melaCpp.calculateAngles(FL, FL.leg1 , l1 , l2,
                        FL.leg2 , l3, l4,
                        self.costheta1,  self.costheta2,  self.phi,  self.costhetastar,
                        self.phistar1,  self.phistar2,  self.phistar12,  self.phi1,  self.phi2)



        self.melaCpp.computeLD(FL.mass(), FL.leg1.mass(),
                               FL.leg2.mass(),self.costhetastar,
                               self.costheta1, self.costheta2,
                               self.phi, self.phistar1, 
                               self.MELA,self.MELAS,self.MELAB)
        return self.MELA
    





