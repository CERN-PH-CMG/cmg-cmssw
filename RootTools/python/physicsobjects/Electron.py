from CMGTools.RootTools.physicsobjects.Lepton import *

class Electron( Lepton ):

    def __init__(self, *args, **kwargs):
        '''Initializing tightIdResult to None. The user is responsible
        for setting this attribute externally if he wants to use the tightId
        function.'''
        super(Electron, self).__init__(*args, **kwargs)
        self.tightIdResult = None

    def absEffAreaIso(self,rho,effectiveAreas):
        '''MIKE, missing doc.
        Should have the same name as the function in the mother class.
        Can call the mother class function with super.
        '''
        return self.absIsoFromEA(rho,self.sourcePtr().superCluster().eta(),effectiveAreas.eGamma)

    def mvaIso( self ):
        return self.sourcePtr().userFloat('mvaIsoRings')

    def mvaId( self ):
        return self.mvaNonTrigV0()
        #return self.sourcePtr().userFloat('mvaNonTrigV0')
        
    def tightId( self ):
        return self.tightIdResult
        
    def mvaIDZZ(self):
        '''missing doc, who is using this function?.'''
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
               abs(self.sourcePtr().superCluster().eta())>=element['etaMin'] and \
               abs(self.sourcePtr().superCluster().eta())<element['etaMax'] and \
               self.mvaNonTrigV0()> element['mva']: 
                ID=True

        return ID and (self.numberOfHits()<=1)

    def chargedAllIso(self):
        '''This function is used in the isolation, see Lepton class.
        Here, we replace the all charged isolation by the all charged isolation with cone veto'''
        return self.chargedAllIsoWithConeVeto()


    def dxy(self, vertex=None):
        '''Returns dxy.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.sourcePtr().gsfTrack().dxy( vertex.position() )
 

    def dz(self, vertex=None):
        '''Returns dz.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.sourcePtr().gsfTrack().dz( vertex.position() )



class HTauTauElectron( Electron ):

    def __init__(self, *args, **kwargs):
        super(ElectronForHTauTau, self).__init__(*args, **kwargs)
        self.photonIsoCache = None
    
    def photonIso(self):
        if self.photonIsoCache is None:
            myVetoes = ROOT.reco.IsoDeposit.Vetos()
            iso = self.sourcePtr().isoDeposit(6).depositWithin(0.4,myVetoes,False)
            if self.sourcePtr().isEE() :
                # Why 13?? 
                iso_veto = self.sourcePtr().isoDeposit(13).depositWithin(0.08,myVetoes,False)
                iso -= iso_veto
            self.photonIsoCache = iso
        return self.photonIsoCache
   
    def tightIdForEleTau(self):
        """reference numbers form the Htautau twiki

        https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#2012_Baseline_Selection
        """
        if self.looseIdForEleTau() == False : return False
        eta = abs( self.eta() )
        if eta > 2.1 : return False
        lmvaID = -99999 # identification
        if self.pt() < 20 :
            if   eta<0.8:   lmvaID = 0.925
            elif eta<1.479: lmvaID = 0.915
            else :          lmvaID = 0.965
        else:
            if   eta<0.8:   lmvaID = 0.925
            elif eta<1.479: lmvaID = 0.975
            else :          lmvaID = 0.985
        result = self.mvaNonTrigV0()  > lmvaID
        return result        

    def looseIdForEleTau(self):
        """Loose electron selection, for the lepton veto, 
        according to Phil sync prescription for the sync exercise 18/06/12
        """
        nInnerHits = self.numberOfHits()
        if nInnerHits != 0 : return False
        if self.passConversionVeto() == False   : return False
        #COLIN : we might want to keep the vertex constraints separated
        if abs(self.dxy())             >= 0.045 : return False
        if abs(self.dz())              >= 0.2   : return False
        # Below, part of WP95 without vertex constraints (applied above)
        hoe = self.hadronicOverEm()
        deta = self.deltaEtaSuperClusterTrackAtVtx()
        dphi = self.deltaPhiSuperClusterTrackAtVtx()
        sihih = self.sigmaIetaIeta() 
        if self.sourcePtr().isEB() :
            if sihih >= 0.010     : return False
            if dphi  >= 0.80      : return False 
            if deta  >= 0.007     : return False
            if hoe   >= 0.15      : return False
        elif self.sourcePtr().isEE() :
            if sihih >= 0.030     : return False
            if dphi  >= 0.70      : return False 
            if deta  >= 0.010     : return False
    #            if hoe   >= 0.07      : return False
        else : return False #PG is this correct? does this take cracks into consideration?
        return True    
