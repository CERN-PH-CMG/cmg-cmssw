import math

from CMGTools.RootTools.physicsobjects.TauDecayModes import tauDecayModes



class PhysicsObject(object):
    '''Extends the cmg::PhysicsObject functionalities.'''

    def __init__(self, physObj):
        self.physObj = physObj

    def scaleEnergy( self, scale ):
        p4 = self.physObj.p4()
        p4 *= scale 
        self.physObj.setP4( p4 )  
        
    def __getattr__(self,name):
        '''all accessors  from cmg::DiTau are transferred to this class.'''
        return getattr(self.physObj, name)

    def __str__(self):
        tmp = '{className} : {pdgId:>3}, pt = {pt:5.1f}, eta = {eta:5.2f}, phi = {phi:5.2f}'
        return tmp.format( className = self.__class__.__name__,
                           pdgId = self.pdgId(),
                           pt = self.pt(),
                           eta = self.eta(),
                           phi = self.phi() )


class Jet( PhysicsObject):
    def looseJetId(self):
        '''This jet id is slightly different from the one present in the cmg::PFJet on May 28.
        no cut on the mu fraction.
        '''
        return self.getSelection('cuts_looseJetId_h0Fraction') and \
               self.getSelection('cuts_looseJetId_gammaFraction') and \
               self.getSelection('cuts_looseJetId_nConstituents') and \
               self.getSelection('cuts_looseJetId_hFraction') and \
               self.getSelection('cuts_looseJetId_hChargedMultiplicity') and \
               self.getSelection('cuts_looseJetId_eFraction')
               


class GenJet( PhysicsObject):
    pass


class Lepton( PhysicsObject):
    def sip3D(self):
        patLepton = self.physObj.sourcePtr()
        return abs( patLepton.dB(2) ) / patLepton.edB(2) 

    def absIsoFromEA(self,rho,effectiveArea1 = None,effectiveArea2 = None):
        #First remove FSR
        photonIso = self.photonIso()
        if hasattr(self,'fsrPhoton'):
            photon=self.fsrPhoton
            photonIso=photonIso-photon.pt()                
        ea1 = rho
        ea2 = rho
        if effectiveArea1 is not None:
            for element in effectiveArea1:
                if abs(self.eta())>= element['etaMin'] and \
                   abs(self.eta())< element['etaMax']:
                    ea1 = ea1 * element['area']
                    break
        else:
            return self.chargedHadronIso()+max(0.,photonIso+self.neutralHadronIso()-ea1)
        if effectiveArea2 is not None:
            for element in effectiveArea2:
                if abs(self.eta())>= element['etaMin'] and \
                   abs(self.eta())< element['etaMax']:
                    ea2 = ea2 * element['area']
            return self.chargedHadronIso()+max(0.,photonIso-ea1)+max(0.,self.neutralHadronIso()-ea2)
        else:
            return self.chargedHadronIso()+max(0.,photonIso+self.neutralHadronIso()-ea1)


    #OVerload Delta beta as well
    def absIso(self,dBetaFactor = 0,allCharged=0):
        if dBetaFactor>0 and self.puChargedHadronIso()<0:
            return -1
        neutralIso = self.neutralHadronIso()+self.photonIso()

        #Recover FSR
        if hasattr(self,'fsrPhoton'):
            photon=self.fsrPhoton
            neutralIso=neutralIso-photon.pt()
        
        corNeutralIso = neutralIso - dBetaFactor * self.puChargedHadronIso();
        charged = self.chargedHadronIso();
        if  allCharged:
            charged = self.chargedAllIso();

        return charged + max(corNeutralIso,0)

    def  relIso(self,dBetaFactor=0, allCharged=0):
         abs = self.absIso(dBetaFactor, allCharged)/self.pt();
         # if abs >0:
         return abs
         #else:
         #    return -1

    def relIsoAllChargedDB05(self):
        '''Used in the H2TauTau analysis: rel iso, dbeta=0.5, using all charged particles.'''
        return self.relIso( 0.5, 1 )

    def relEffAreaIso(self,rho):
        return 0

    def relEffAreaIso(self,rho):
        return self.absEffAreaIso(rho)/self.pt()




class Photon(Lepton):
    pass

    
class Muon( Lepton ):

    def looseId( self ):
        '''Loose ID as recommended by mu POG.'''
        return self.sourcePtr().userFloat('isPFMuon') and \
               ( self.isGlobalMuon() or self.isTrackerMuon() )

    def tightId( self ):
        '''Tight ID as recommended by mu POG.'''
        return self.looseId() and \
               self.isGlobalMuon() and \
               self.normalizedChi2() < 10 and \
               self.numberOfValidMuonHits() > 0 and \
               self.numberOfMatches() > 1 and \
               self.sourcePtr().innerTrack().hitPattern().numberOfValidPixelHits()>0 and \
               self.trackerLayersWithMeasurement() > 5 
               # self.dxy() < 0.2 and \
               # self.dz() < 0.5 and \
               # self.numberOfValidPixelHits() > 0 and \

    def mvaId(self):
        '''For a transparent treatment of electrons and muons. Returns -99'''
        return -99
    
    def mvaIso( self ):
        return self.sourcePtr().userFloat('mvaIsoRings')
    
    def detIso( self, rho ):
        '''Rho corrected detector-based isolation, for the H->ZZ->4l baseline analysis'''
        patMuon = self.sourcePtr() 
        isoEcal = patMuon.ecalIso()
        isoHcal = patMuon.hcalIso()
        isoTk   = patMuon.userIsolation( 7 )
        isoEcal, isoHcal = self.rhoCorrMu(rho, isoEcal, isoHcal)
        return (isoEcal + isoHcal + isoTk)/patMuon.pt()

    def rhoCorrMu(self, rho, ecalIso, hcalIso):
        '''rho correction for the ecal and hcal iso. returns the corrected pair'''
        AreaEcal = [0.074, 0.045] # barrel/endcap 
        AreaHcal = [0.022 , 0.030] # barrel/endcap
        ifid = 1 
        if abs( self.eta() ) < 1.479:
            ifid = 0 # selecting barrel settings
        ecalIso = ecalIso - AreaEcal[ifid] * rho
        hcalIso = hcalIso - AreaHcal[ifid] * rho
        return ecalIso, hcalIso

    def absEffAreaIso(self,rho,effectiveAreas):
        return self.absIsoFromEA(rho,effectiveAreas.muon)

    def dxy(self, vertex=None):
        '''either pass the vertex, or set associatedVertex before calling the function.
        note: the function does not work with standalone muons as innerTrack
        is not available.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.sourcePtr().innerTrack().dxy( vertex.position() )
 

    def dz(self, vertex=None):
        '''either pass the vertex, or set associatedVertex before calling the function.
        note: the function does not work with standalone muons as innerTrack
        is not available.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.sourcePtr().innerTrack().dz( vertex.position() )
    

class Electron( Lepton ):

    def __init__(self, *args, **kwargs):
        '''Initializing tightIdResult to None. The user is responsible
        for setting this attribute externally if he wants to use the tightId
        function.'''
        super(Electron, self).__init__(*args, **kwargs)
        self.tightIdResult = None

    def absEffAreaIso(self,rho,effectiveAreas):
        return self.absIsoFromEA(rho,effectiveAreas.eGamma)

    def mvaIso( self ):
        return self.sourcePtr().userFloat('mvaIsoRings')

    def mvaId( self ):
        return self.mvaNonTrigV0()
        #return self.sourcePtr().userFloat('mvaNonTrigV0')
        
    #    def ElectronMVA_MIT( self ):
    #        return self.sourcePtr().userFloat('ElectronMVA_MIT')
    
    def tightId( self ):
        return self.tightIdResult
        
    def mvaIDZZ(self):
        mvaRegions = [{'ptMin':0,'ptMax':10, 'etaMin':0.0, 'etaMax':0.8,'mva':0.47},\
                      {'ptMin':0,'ptMax':10, 'etaMin':0.8 ,'etaMax':1.479,'mva':0.004},\
                      {'ptMin':0,'ptMax':10, 'etaMin':1.479, 'etaMax':2.5,'mva':0.295},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.0, 'etaMax':0.8,'mva':0.5},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.8, 'etaMax':1.479,'mva':0.12},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':1.479, 'etaMax':2.5,'mva':0.6}]

        ID=False 

        for element in mvaRegions:
            if self.pt()>= element['ptMin'] and \
               self.pt()< element['ptMax'] and \
               abs(self.eta())>=element['etaMin'] and \
               abs(self.eta())<element['etaMax'] and \
               self.mvaNonTrigV0()> element['mva']: 
                ID=True

        return ID and (self.numberOfHits()<=1)

    def relIsoAllChargedDB05(self):
        '''Used in the H2TauTau analysis: rel iso, dbeta=0.5, 
        
        using all charged particles with a 0.01 cone veto around electron.'''

        dBetaFactor = 0.5
        if dBetaFactor>0 and self.puChargedHadronIso()<0:
            return -1
        neutralIso = self.neutralHadronIso()+self.photonIso()

        corNeutralIso = neutralIso - dBetaFactor * self.puChargedHadronIso();
        charged = self.chargedAllIsoWithConeVeto()

        abs = (charged + max(corNeutralIso,0)) / self.pt()

        if abs >0:
             return abs
        else:
             return -1

class GenParticle( PhysicsObject):
    def __str__(self):
        base = super(GenParticle, self).__str__()
        theStr = '{base}, status = {status:>2}'.format(base=base, status=self.status())
        return theStr

class GenLepton( GenParticle ):
    def sip3D(self):
        '''Just to make generic code work on GenParticles'''
        return 0
    def relIso(self, dummy):
        '''Just to make generic code work on GenParticles'''
        return 0

    def absIso(self, dummy):
        '''Just to make generic code work on GenParticles'''
        return 0

    def absEffAreaIso(self,rho):
        '''Just to make generic code work on GenParticles'''
        return 0

    def relEffAreaIso(self,rho):
        '''Just to make generic code work on GenParticles'''
        return 0

    
class Tau( Lepton ):
    
    def __init__(self, tau):
        self.tau = tau
        super(Tau, self).__init__(tau)
        self.eOverP = None
        
    def calcEOverP(self):
        if self.eOverP is not None:
            return self.eOverP
        self.leadChargedEnergy = self.tau.leadChargedHadrEcalEnergy() \
                                 + self.tau.leadChargedHadrHcalEnergy()
        # self.leadChargedMomentum = self.tau.leadChargedHadrPt() / math.sin(self.tau.theta())
        self.leadChargedMomentum = self.tau.leadPFChargedHadrCand().energy()
        self.eOverP = self.leadChargedEnergy / self.leadChargedMomentum
        return self.eOverP         

    def dxy(self, vertex=None):
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrVertex();   
        p4 = self.p4();
        return ( - (vtx.x()-vertex.position().x()) *  p4.y()
                 + (vtx.y()-vertex.position().y()) *  p4.x() ) /  p4.pt();    

    def dz(self, vertex=None):
        if vertex is None:
            vertex = self.associatedVertex
        vtx = self.leadChargedHadrVertex();   
        p4 = self.p4();        
        return  (vtx.z()-vertex.position().z()) - ((vtx.x()-vertex.position().x())*p4.x()+(vtx.y()-vertex.position().y())*p4.y())/ p4.pt() *  p4.z()/ p4.pt();
    
    def __str__(self):
        lep = super(Tau, self).__str__()
        spec = '\tTau: decay = {decMode:<15}, eOverP = {eOverP:4.2f}'.format(
            decMode = tauDecayModes.intToName( self.decayMode() ),
            eOverP = self.calcEOverP()
            )
        return '\n'.join([lep, spec])






def isTau(leg):
    '''Duck-typing a tau'''
    try:
        leg.leadChargedHadrPt()
    except AttributeError:
        return False
    return True

