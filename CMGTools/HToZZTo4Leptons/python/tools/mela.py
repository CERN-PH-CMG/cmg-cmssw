import os

from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

import ROOT as root


# loadLibs()

gSystem.Load("libCMGToolsHToZZTo4Leptons")
gSystem.Load("libJHUMELA")

templatePath = '/'.join([os.environ['CMSSW_BASE'],'src/JHU/MELA/src/datafiles/my8DTemplateNotNorm.root']) 
root.setTemplate(templatePath); # need a better way to point to the input file

# from ROOT import MELAAngles
from ROOT import calculateAngles

# you can even create a python class for high level stuff, like this: 
class LvMELAAngles(object):
    '''This class computes the mela angles, taking in input the 4-vectors of the leptons coming from the decay of the Zs from the Higgs.'''
    
    def __init__(self,
                 lepZ1, antiLepZ1,
                 lepZ2, antiLepZ2): 
        '''
        lepZ1: lepton from Z1
        antiLepZ1: anti-lepton from Z1
        lepZ2: lepton from Z2
        antiLepZ2: anti-lepton from Z2

        Computes the following angles (if you know the missing definitions add them!):
        costheta1 : langle of the Z1 leptons w/r to the Z1 direction, in the Z1 rest frame   
        costheta2 : same for Z2
        phi 
        costhetastar : angle of the Zs with respect to the Higgs propagation direction, in the Higgs rest frame
        phistar1
        phistar2
        phistar12
        phi1
        phi2 
        '''

        self.costheta1=root.Double(0)
        self.costheta2=root.Double(0)
        self.phi=root.Double(0)
        self.costhetastar=root.Double(0)
        self.phistar1=root.Double(0)
        self.phistar2=root.Double(0)
        self.phistar12=root.Double(0)
        self.phi1=root.Double(0)
        self.phi2=root.Double(0)
        
        calculateAngles(lepZ1+antiLepZ1+lepZ2+antiLepZ2, lepZ1+antiLepZ1 , lepZ1 , antiLepZ1,
                        lepZ2+antiLepZ2 , lepZ2, antiLepZ2,
                        self.costheta1,  self.costheta2,  self.phi,  self.costhetastar,
                        self.phistar1,  self.phistar2,  self.phistar12,  self.phi1,  self.phi2)

        self.mZZ = (lepZ1+antiLepZ1+lepZ2+antiLepZ2).M()
        self.m1  = (lepZ1+antiLepZ1).M()
        self.m2  = (lepZ2+antiLepZ2).M()
        

        prob = root.likelihoodDiscriminant( self.mZZ, self.m1, self.m2, self.costhetastar,
                                            self.costheta1, self.costheta2, self.phi, self.phistar1)
        self.MELAS = prob.first
        self.MELAB = prob.second
        self.MELA  = self.MELAS / (self.MELAS  + self.MELAB)
       

if __name__ == '__main__':

    from ROOT import TLorentzVector
    lepZ1 = TLorentzVector()
    antiLepZ1 = TLorentzVector()
    lepZ2 = TLorentzVector()
    antiLepZ2 = TLorentzVector()
    lepZ1.SetPxPyPzE(-0.768853944346720E+01,  0.419702017182753E+01, -0.980948214278878E+01,  0.131512188410873E+02)
    antiLepZ2.SetPxPyPzE( 0.163114374963135E+02,  0.152798039261140E+02, -0.393458453717676E+02,  0.452507563388247E+02)
    lepZ2.SetPxPyPzE(-0.943151158126501E+01, -0.986925894022945E+01,  0.240202927591857E+01,  0.138609316923216E+02)
    antiLepZ1.SetPxPyPzE( 0.808613528418701E+00, -0.960756515771212E+01,  0.467532982386379E+02,  0.477370931277664E+02)
    melaAngles = LvMELAAngles(lepZ1, antiLepZ1, lepZ2, antiLepZ2)
    
    print melaAngles.costheta1 , melaAngles.costheta2,  melaAngles.phi,  melaAngles.costhetastar,  melaAngles.phistar1,  melaAngles.phistar2,  melaAngles.phistar12,  melaAngles.phi1,  melaAngles.phi2
    print melaAngles.MELA, melaAngles.MELAS , melaAngles.MELAB
