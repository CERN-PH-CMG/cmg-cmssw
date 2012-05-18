from copy import copy
from math import pi,acos,asin
from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi  


class FSRConfig(object):
    def __init__(self):
        self.minPhotonPt = 2.0
        self.maxPhotonEta=2.4
        self.electronCleaningDR=0.01
        self.vetoElectronDR=0.15
        self.vetoElectronDEta=0.05
        self.vetoElectronDPhi=2.0
        self.maxLeptonPhotonDR=999.
        self.maxLeptonPhotonAngle=10.
        self.maxThetaStar = 20.
        self.leptonIsoCone=0.4
        
        





class FSRRecovery(object):
    def __init__(self,fsrCfg):
        self.cfg = fsrCfg
        self.photons=None
        
    def setPhotons(self,photons):
        self.photons = copy(photons)
        self.photons = filter(lambda x: x.pt()>self.cfg.minPhotonPt and \
                             abs(x.eta())<self.cfg.maxPhotonEta,photons)



    def setZ(self,zCand):
        self.z = zCand


    def recover(self):
        # filter wrt Z candidate
        self.photons = filter(self.photonPreFilter,self.photons)

        #Filter by theta star
        self.photons = filter(self.filterThetaStar,self.photons)
        #Sort the photons by thetaStar
#        self.photons=sorted(self.photons,key=self.thetaStar)

        #Sort the photons by pt
        self.photons=sorted(self.photons,key=lambda x: x.pt(),reverse=True)

                               
        if len(self.photons)>0:
            #associate it to the Z
            photon=self.photons[0]
            self.z.setFSR(photon)
            #check if it in the isolation cone of the leptons
            #and associate it with them
            if self.z.fsrDR1()<self.cfg.leptonIsoCone:
                self.z.leg1.fsrPhoton=photon
            if self.z.fsrDR2()<self.cfg.leptonIsoCone:
                self.z.leg2.fsrPhoton=photon
            #remove photons from the list so thet we can next go to
            #the other Z
            self.photons.remove(photon)
            

    def photonPreFilter(self,photon):
        '''Ask minimum distance from Z legs. In case photon is an electron
           cross clean the photon . In case that the lepton is an electron
           account for the fact that the energy will be absorbed by the
           Supercluster'''
        for leg in [self.z.leg1,self.z.leg2]:
            #Cross clean electrons from electrons
            if abs(photon.pdgId())==11 and abs(leg.pdgId())==11:
                if deltaR(photon.eta(),leg.eta(),photon.phi(),leg.phi())<self.cfg.electronCleaningDR:
                    return False
            #if the lepton is an electron and the photon a photon account for supercluster absorption
            if abs(photon.pdgId())!=11 and abs(leg.pdgId())==11:
                if deltaR(photon.eta(),leg.eta(),photon.phi(),leg.phi())<self.cfg.vetoElectronDR:
                    return False
                if abs(photon.eta()-leg.eta())<self.cfg.vetoElectronDEta and  deltaPhi(photon.phi(),leg.phi())<self.cfg.vetoElectronDPhi:
                    return False
                
        #Now ask that the photon is in the vicinity of the lepton
        dr1 = deltaR(photon.eta(),self.z.leg1.eta(),photon.phi(),self.z.leg1.phi())
        dr2 = deltaR(photon.eta(),self.z.leg2.eta(),photon.phi(),self.z.leg2.phi())

        theta1 = acos(photon.p4().Vect().Dot(self.z.leg1.p4().Vect())/(photon.p4().P()*self.z.leg1.p4().P()))*180/pi
        theta2 = acos(photon.p4().Vect().Dot(self.z.leg2.p4().Vect())/(photon.p4().P()*self.z.leg2.p4().P()))*180/pi

        if min(dr1,dr2) > self.cfg.maxLeptonPhotonDR:
            return False

        if min(theta1,theta2) > self.cfg.maxLeptonPhotonAngle:
            return False

        return True


    def thetaStar(self,photon):
        plane = (self.z.leg1.p4().Vect().Cross(self.z.leg2.p4().Vect())).unit()
        angle = asin(plane.Dot(photon.p4().Vect().unit()))
        return angle*180/pi


    def filterThetaStar(self,photon):
        if self.thetaStar(photon)<self.cfg.maxThetaStar:
            return True
        else:
            return False
        
