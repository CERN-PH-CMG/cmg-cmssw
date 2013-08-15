from copy import copy,deepcopy
from math import pi,acos,asin
from CMGTools.RootTools.utils.DeltaR import deltaR,deltaPhi  

from ROOT import TLorentzVector

class FSRRecovery(object):
    def __init__(self,fsrCfg,verbose=False):
        self.cfg = fsrCfg
        self.verbose=verbose
        self.electronID=lambda x: True
        
        
    def setLeptons(self,leptons):
        self.leptons =leptons

    def setBoson(self,boson):
        self.leptons = boson.daughterLeptons()

    def setElectronID(self,electronID):
        self.electronID = electronID
        
    def attachPhotons(self,photons):
        '''Define the collection of photons to be used for the recovery'''
        self.photons = filter(lambda x: x.pt()>self.cfg.minPhotonPt and \
                             abs(x.eta())<self.cfg.maxPhotonEta and self.photonPreFilter(x),photons)
            

        if self.verbose:
            print 'initial photons'
            for g in self.photons:
                print g.pt(),g.eta(),g.phi()

        for lepton in self.leptons:
            lepton.photons=[]

        #attach the photon to the closest lepton
        for photon in self.photons:
            self.attachToClosestLepton(photon)
                
        if self.verbose:
            for lepton in self.leptons:
                print 'LEPTON ',lepton.pt(),lepton.eta(),lepton.phi()
                for photon in lepton.photons:
                    print '--->attached photon ',photon.pt(),photon.eta(),photon.phi()
                    

    def recoverZ(self,z):
        ''' Run the recovewry algorithm. Associates a photon to the Z'''

        #merge the photons
        photons = z.leg1.photons+z.leg2.photons
        #Filter by MassZ
        photons = filter(lambda x: (TLorentzVector(x.px(),x.py(),x.pz(),x.energy())+z).M()<self.cfg.maxZMass,photons)

        #filter by mass improvement
        photons= filter(lambda x: abs((TLorentzVector(x.px(),x.py(),x.pz(),x.energy())+z).M()-91.188)<abs(z.M()-91.188),photons)

        if self.verbose:
            for photon in photons:
                print 'photon for recovery',photon.pt(),photon.eta(),photon.phi()

        #Split the high Pt and low Pt region
        photonsHighPt = filter(lambda x: x.pt()>self.cfg.minPhotonPtTight,photons)
        photonsLowPt  = filter(lambda x: x.pt()<=self.cfg.minPhotonPtTight,photons)

        photonsHighPt=sorted(photonsHighPt,key=lambda x: x.pt(),reverse=True)

        # if there is high Pt photon get it

        photon=None
        if len(photonsHighPt)>0:
            #associate it to the Z
            photon=photonsHighPt[0]
        elif len(photonsLowPt):
            #sort by angle
            photonsLowPt = sorted(photonsLowPt,key=lambda x: min(deltaR(z.leg1.eta(),z.leg1.phi(),x.eta(),x.phi()),deltaR(z.leg2.eta(),z.leg2.phi(),x.eta(),x.phi())))
            photon=photonsLowPt[0]
            
        if photon is not None:    
            z.setFSR(photon)




    def recoverLeg(self,leg):
        ''' Run the recovewry algorithm. Associates a photon to the Z'''
        
        #Prefilter
        photons=leg.photons
        photonsHighPt = filter(lambda x: x.pt()>self.cfg.minPhotonPtTight,photons)
        photonsLowPt  = filter(lambda x: x.pt()<=self.cfg.minPhotonPtTight,photons)
        photonsHighPt=sorted(photonsHighPt,key=lambda x: x.pt(),reverse=True)
        photon=None

        if len(photonsHighPt)>0:
            photon=photonsHighPt[0]
        elif len(photonsLowPt):
            photonsLowPt = sorted(photonsLowPt,key=lambda x: deltaR(x.eta(),x.phi(),leg.eta(),leg.phi()))
            photon=photonsLowPt[0]

            
        if photon is not None:    
            if deltaR(photon.eta(),photon.phi(),leg.eta(),leg.phi())<self.cfg.leptonIsoCone:
                leg.fsrPhoton=photon


            




    def photonPreFilter(self,photon):
        '''Generic cross cleaning of photons''' 
        if self.verbose:
            print 'prefilter gamma',photon.pt(),photon.eta(),photon.phi()

        for leg in self.leptons:
            DR = deltaR(photon.eta(),photon.phi(),leg.eta(),leg.phi())
            Deta = abs(photon.eta()-leg.eta())
            Dphi = abs(deltaPhi(photon.phi(),leg.phi()))
            if self.verbose:
                print '---->vs leg',leg.pdgId(),self.electronID(leg),leg.pt(),leg.eta(),leg.phi(),'|',DR,Deta,Dphi
                
                
            if  abs(leg.pdgId())==11 and self.electronID(leg):
                if DR<self.cfg.vetoElectronDR:
                    return False
                if Deta <self.cfg.vetoElectronDEta and  Dphi<self.cfg.vetoElectronDPhi:
                    return False
                

        return True

    def attachToClosestLepton(self,photon):
        nearestLepton=None
        dr=1000.
        for lepton in self.leptons:
            drNew = deltaR(lepton.eta(),lepton.phi(),photon.eta(),photon.phi()) 
            if drNew<dr:
                dr=drNew
                nearestLepton=lepton
        
        if (nearestLepton is not None) and  dr<self.cfg.maxLeptonPhotonDR:
            if dr<self.cfg.maxLeptonPhotonDRTight or (dr> self.cfg.maxLeptonPhotonDRTight and (photon.chargedHadronIso()+photon.photonIso()+photon.neutralHadronIso()+photon.puChargedHadronIso())/photon.pt()<self.cfg.maxPhotonDBIso and photon.pt()>self.cfg.minPhotonPtTight):
                nearestLepton.photons.append(photon)
                
