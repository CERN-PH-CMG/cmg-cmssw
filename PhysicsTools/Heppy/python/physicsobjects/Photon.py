from PhysicsTools.Heppy.physicsobjects.PhysicsObject import *

import ROOT

class Photon(PhysicsObject ):

    '''                                                                                                                                                                                                                                                                return object from the photon 
    '''
    def hOVERe(self):
        #return self.physObj.hadronicOverEm() 
        hadTowDepth1O = self.physObj.hadTowDepth1OverEm() * (self.physObj.superCluster().energy()/self.physObj.full5x5_e5x5() if self.physObj.full5x5_e5x5() else 1)
        hadTowDepth2O = self.physObj.hadTowDepth2OverEm() * (self.physObj.superCluster().energy()/self.physObj.full5x5_e5x5() if self.physObj.full5x5_e5x5() else 1)
        return hadTowDepth1O + hadTowDepth2O

    def r9(self):
        return self.physObj.r9()

    def sigmaIetaIeta(self):
        return self.physObj.sigmaIetaIeta()

    def full5x5_r9(self):
        return self.physObj.full5x5_r9()

    def full5x5_sigmaIetaIeta(self):
        return self.physObj.full5x5_sigmaIetaIeta()

    def chargedHadronIso(self):
        return self.physObj.chargedHadronIso()

    def neutralHadronIso(self):
        return self.physObj.neutralHadronIso()

    def photonIso(self):
        return self.physObj.photonIso()

    def photonIDCSA14(self, name, sidebands=False):
        keepThisPhoton = True
        sigmaThresh  = 999
        hovereThresh = 999
        if name == "PhotonCutBasedIDLoose_CSA14":
            if abs(self.physObj.eta())<1.479 :
                sigmaThresh  = 0.010
                hovereThresh = 0.0559
            else :
                sigmaThresh  = 0.030
                hovereThresh = 0.049
        elif name == "PhotonCutBasedIDLoose_PHYS14":
            if abs(self.physObj.eta())<1.479 :
                sigmaThresh  = 0.0106
                hovereThresh = 0.048
            else :
                sigmaThresh  = 0.0266
                hovereThresh = 0.069
        else :
          print "WARNING! Unkown photon ID! Will return true!" 
          return True

        if sidebands:
          if abs(self.physObj.eta())<1.479 :
            sigmaThresh = 0.015
          else :
            sigmaThresh = 0.035

        if self.full5x5_sigmaIetaIeta() > sigmaThresh  : keepThisPhoton = False
        if self.hOVERe()                > hovereThresh : keepThisPhoton = False

        return keepThisPhoton
                
    pass

setattr(ROOT.pat.Photon, "recoPhotonIso", ROOT.reco.Photon.photonIso)
setattr(ROOT.pat.Photon, "recoNeutralHadronIso", ROOT.reco.Photon.neutralHadronIso)
setattr(ROOT.pat.Photon, "recoChargedHadronIso", ROOT.reco.Photon.chargedHadronIso)
