from copy import copy
from math import pi,acos,asin
from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi  
from ROOT import TLorentzVector


class FSRConfig(object):
    def __init__(self):
        self.minPhotonPt = 2.0
        self.minPhotonPtTight = 4.0

        self.maxPhotonEta=2.4
        self.maxPhotonDBIso=1
        self.electronCleaningDR=0.01
        self.vetoElectronDR=0.15
        self.vetoElectronDEta=0.05
        self.vetoElectronDPhi=0.2
        self.maxLeptonPhotonDR=0.5
        self.maxLeptonPhotonAngle=9999.
        self.maxLeptonPhotonDRTight=0.07
        self.maxLeptonPhotonAngleTight=1000.

        self.maxThetaStar = 30.
        self.leptonIsoCone=0.4
        self.maxZMass=100.


class FSRRecovery(object):
    def __init__(self,fsrCfg):
        self.cfg = fsrCfg

        
    def setPhotons(self,photons):
        '''Define the collection of photons to be used for the recovery'''
        self.photons = filter(lambda x: x.pt()>self.cfg.minPhotonPt and \
                             abs(x.eta())<self.cfg.maxPhotonEta,photons)


    def setZ(self,zCand):
        '''Set a Z to recover'''
        self.z = zCand

    def setZZ(self,zzCand):
        '''Set a ZZ to recover'''
        self.zz = zzCand


    def recoverZ(self,isZ1 = False):
        ''' Run the recovewry algorithm. Associates a photon to the Z'''
        
        #Prefilter
        photons = filter(self.photonPreFilter,self.photons)

#        print "Photons after Prefilter = %d\n" % len(self.photons)
        # filter wrt Z candidate
        photons = filter(self.photonFilter,photons)
#        print "Photons after filter = %d\n" % len(self.photons)


        #Filter by theta star
        photons = filter(self.filterThetaStar,photons)
#        print "Photons after theta star = %d\n" % len(self.photons)

        #Filter by MassZ
        photons = filter(self.filterMass,photons)
#        print "Photons after z mass = %d\n" % len(self.photons)

        #if the Z is Z1 check if FSR improves the Z measurement
        

        #Sort the photons by thetaStar
#        self.photons=sorted(self.photons,key=self.thetaStar)
        if isZ1:
            photons= filter(self.filterMassImprovement,photons)
#            print "Photons after  mass improvement test= %d\n" % len(self.photons)



        #Split the high Pt and low Pt region
        photonsHighPt = filter(lambda x: x.pt()>self.cfg.minPhotonPtTight,photons)
        photonsLowPt  = filter(lambda x: x.pt()<=self.cfg.minPhotonPtTight,photons)
            
        #Sort the high photons by pt
        photonsHighPt=sorted(photonsHighPt,key=lambda x: x.pt(),reverse=True)
#        photonsHighPt = sorted(photonsHighPt,key=self.DRLGamma)


        # if there is high Pt photon get it

        photon=None
        if len(photonsHighPt)>0:
            print "There is high Pt photon"

            #associate it to the Z
            photon=photonsHighPt[0]
        elif len(photonsLowPt):
            print "There is low Pt photon"

            #sort by angle
            photonsLowPt = sorted(photonsLowPt,key=self.DRLGamma)
            photon=photonsLowPt[0]
            
            
        if photon is not None:    
            self.z.setFSR(photon)
            #and associate it with them
            if self.z.fsrDR1()<self.cfg.leptonIsoCone:
                self.z.leg1.fsrPhoton=photon
            if self.z.fsrDR2()<self.cfg.leptonIsoCone:
                self.z.leg2.fsrPhoton=photon
            #remove photons from the list so thet we can next go to
            #the other Z
            self.photons.remove(photon)

            


    def recoverZZ(self):
        '''Run the recovery algorithm for four leptons'''
        self.setZ(self.zz.leg1)
        self.recoverZ(True)
        self.setZ(self.zz.leg2)
        self.recoverZ(False)
        

    def photonPreFilter(self,photon):
        '''Generic cross cleaning of photons''' 

        if not hasattr(self,'zz'):
            legs=[self.z.leg1,self.z.leg2]
        else:    
            legs=[self.zz.leg1.leg1, \
                  self.zz.leg1.leg2, \
                  self.zz.leg2.leg1, \
                  self.zz.leg2.leg2]


        for leg in legs:
            #Check that the photon is none of the legs
            #Cross clean electrons from electrons
            if abs(photon.pdgId())==11 and abs(leg.pdgId())==11:
                if deltaR(photon.eta(),photon.phi(),leg.eta(),leg.phi())<self.cfg.electronCleaningDR:
                    return False
            #if the lepton is an electron and the photon a photon account for supercluster absorption
            if abs(photon.pdgId())!=11 and abs(leg.pdgId())==11:
                if deltaR(photon.eta(),photon.phi(),leg.eta(),leg.phi())<self.cfg.vetoElectronDR:
                    return False
                if abs(photon.eta()-leg.eta())<self.cfg.vetoElectronDEta and  deltaPhi(photon.phi(),leg.phi())<self.cfg.vetoElectronDPhi:
                    return False
                

        return True

    def photonFilter(self,photon):
        '''Ask minimum distance from Z legs. In case photon is an electron
           cross clean the photon . In case that the lepton is an electron
           account for the fact that the energy will be absorbed by the
           Supercluster'''

        #Now ask that the photon is in the vicinity of the lepton
        dr1 = deltaR(photon.eta(),photon.phi(),self.z.leg1.eta(),self.z.leg1.phi())
        dr2 = deltaR(photon.eta(),photon.phi(),self.z.leg2.eta(),self.z.leg2.phi())

        theta1 = acos(round(photon.p4().Vect().Dot(self.z.leg1.p4().Vect())/(photon.p4().P()*self.z.leg1.p4().P()),5))*180/pi
        theta2 = acos(round(photon.p4().Vect().Dot(self.z.leg2.p4().Vect())/(photon.p4().P()*self.z.leg2.p4().P()),5))*180/pi

        if min(dr1,dr2) > self.cfg.maxLeptonPhotonDR: 
            return False

        if min(theta1,theta2) > self.cfg.maxLeptonPhotonAngle:
            return False

        if min(dr1,dr2) > self.cfg.maxLeptonPhotonDRTight and photon.relIso(0.5)>self.cfg.maxPhotonDBIso:
            return False

        if photon.pt() < self.cfg.minPhotonPtTight and min(dr1,dr2) > self.cfg.maxLeptonPhotonDRTight:
            return False

        if photon.pt() < self.cfg.minPhotonPtTight and min(theta1,theta2) > self.cfg.maxLeptonPhotonAngleTight:
            return False

        return True

    def thetaStar(self,photon):
        plane = (self.z.leg1.p4().Vect().Cross(self.z.leg2.p4().Vect())).unit()
        angle = asin(round(plane.Dot(photon.p4().Vect().unit()),5))
        return abs(angle)*180/pi


    def filterThetaStar(self,photon):
        if self.thetaStar(photon)<self.cfg.maxThetaStar:
            return True
        else:
            return False

    def DRLGamma(self,photon):
        dr1 = deltaR(photon.eta(),photon.phi(),self.z.leg1.eta(),self.z.leg1.phi())
        dr2 = deltaR(photon.eta(),photon.phi(),self.z.leg2.eta(),self.z.leg2.phi())
        return min(dr1,dr2)



    def filterMass(self,photon):
        p=TLorentzVector(photon.px(),photon.py(),photon.pz(),photon.energy())
        if (p+self.z).M()<self.cfg.maxZMass:
            return True
        else:
            return False


    def filterMassImprovement(self,photon):
        p=TLorentzVector(photon.px(),photon.py(),photon.pz(),photon.energy())
        if abs((p+self.z).M()-91.2)<abs(self.z.M()-91.2):
            return True
        else:
            return False



    def compareDistance(self,photon):
        return min(deltaR(photon.eta(),photon.phi(),self.z.leg1.eta(),self.z.leg1.phi()), \
                   deltaR(photon.eta(),photon.phi(),self.z.leg2.eta(),self.z.leg2.phi()))
