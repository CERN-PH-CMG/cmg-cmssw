from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

import ROOT as root

# loadLibs()

gSystem.Load("libCMGToolsHToZZTo4Leptons")
gSystem.Load("libJHUMELA")
root.setTemplate("../../JHU/MELA/src/datafiles/my8DTemplateNotNorm.root"); # need a better way to point to the input file

from ROOT import MELAAngles
from ROOT import calculateAngles
# need to make a MELAEstimator on the same model

# you can even create a python class for high level stuff
class LvMELAAngles(MELAAngles):
    def __init__(self, lv1, lv2, lv3, lv4): # convention is lepton(Z1), antilepton(Z1), lepton(Z2), antilepton(Z2)
        #print lv1.Pt()
        # here you could pass the variables necessary to the base class,
        # to its constructor.
        super(LvMELAAngles, self).__init__()

        self.costheta1=root.Double(0)
        self.costheta2=root.Double(0)
        self.phi=root.Double(0)
        self.costhetastar=root.Double(0)
        self.phistar1=root.Double(0)
        self.phistar2=root.Double(0)
        self.phistar12=root.Double(0)
        self.phi1=root.Double(0)
        self.phi2=root.Double(0)
        
        calculateAngles(lv1+lv2+lv3+lv4, lv1+lv2 , lv1 , lv2,
                        lv3+lv4 , lv3, lv4,
                        self.costheta1,  self.costheta2,  self.phi,  self.costhetastar,  self.phistar1,  self.phistar2,  self.phistar12,  self.phi1,  self.phi2)

        self.mZZ = (lv1+lv2+lv3+lv4).M()
        self.m1  = (lv1+lv2).M()
        self.m2  = (lv3+lv4).M()
        

        prob = root.likelihoodDiscriminant( self.mZZ, self.m1, self.m2, self.costhetastar, self.costheta1, self.costheta2, self.phi, self.phistar1)
        self.MELAS = prob.first
        self.MELAB = prob.second
        self.MELA  = self.MELAS / (self.MELAS  + self.MELAB)
       

if __name__ == '__main__':

    from ROOT import TLorentzVector
    lv1 = TLorentzVector()
    lv2 = TLorentzVector()
    lv3 = TLorentzVector()
    lv4 = TLorentzVector()
    lv1.SetPxPyPzE(-0.768853944346720E+01,  0.419702017182753E+01, -0.980948214278878E+01,  0.131512188410873E+02)
    lv4.SetPxPyPzE( 0.163114374963135E+02,  0.152798039261140E+02, -0.393458453717676E+02,  0.452507563388247E+02)
    lv3.SetPxPyPzE(-0.943151158126501E+01, -0.986925894022945E+01,  0.240202927591857E+01,  0.138609316923216E+02)
    lv2.SetPxPyPzE( 0.808613528418701E+00, -0.960756515771212E+01,  0.467532982386379E+02,  0.477370931277664E+02)
    melaAngles = LvMELAAngles(lv1, lv2, lv3, lv4)
    
    print melaAngles.costheta1 , melaAngles.costheta2,  melaAngles.phi,  melaAngles.costhetastar,  melaAngles.phistar1,  melaAngles.phistar2,  melaAngles.phistar12,  melaAngles.phi1,  melaAngles.phi2
    print melaAngles.MELA, melaAngles.MELAS , melaAngles.MELAB
