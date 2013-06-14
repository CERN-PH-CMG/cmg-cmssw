
import os
import ROOT
from CMGTools.RootTools.RootTools import *
from ROOT import gSystem

gSystem.Load('CMGToolsHToZZTo4Leptons')


from ROOT import KD


# you can even create a python class for high level stuff, like this: 
class KDCalculator(object):
    '''This class Calculates the KDs .'''
    
    def __init__(self):

        self.KD = KD()
        
    def calculate(self,FL):

        if FL.M()<0:
            FL.KD = -99.
            FL.pseudoKD = -99.
            FL.graviKD  = -99. 
            return
        
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
                

        self.KD.computeAll(l1,FL.leg1.leg1.pdgId(),
                           l2,FL.leg1.leg2.pdgId(),
                           l3,FL.leg2.leg1.pdgId(),
                           l4,FL.leg2.leg2.pdgId())

        
        
        FL.KD          = self.KD.getKD()
        FL.gg0KD       = self.KD.getGG0KD()
        FL.gg0HKD      = self.KD.getGG0HKD()
        FL.qq1MinusKD  = self.KD.getQQ1MinusKD()
        FL.qq1PlusKD   = self.KD.getQQ1PlusKD()
        FL.gg2PlusKD   = self.KD.getGG2PlusKD()
        FL.qq2PlusKD   = self.KD.getQQ2PlusKD()
        FL.weight      = self.KD.getInterferenceWeight()

        



