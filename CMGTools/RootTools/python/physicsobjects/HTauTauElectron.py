from CMGTools.RootTools.physicsobjects.Electron import Electron
from ROOT import gSystem, AutoLibraryLoader
gSystem.Load("libFWCoreFWLite")
AutoLibraryLoader.enable()
gSystem.Load("libDataFormatsRecoCandidate.so")
 
from ROOT import reco

class HTauTauElectron( Electron ):

    def __init__(self, *args, **kwargs):
        super(HTauTauElectron, self).__init__(*args, **kwargs)
        self.photonIsoCache = None
        self.chargedAllIsoCache = None
        
    def photonIso(self):
        if self.photonIsoCache is None:
            myVetoes = reco.IsoDeposit.Vetos()
            pfGammaIsoType = 6
            iso = self.sourcePtr().isoDeposit(pfGammaIsoType).depositWithin(0.4,myVetoes,False)
            iso_veto = self.sourcePtr().isoDeposit(pfGammaIsoType).depositWithin(0.08,myVetoes,False)
            iso -= iso_veto
            self.photonIsoCache = iso
        return self.photonIsoCache


    def chargedAllIso(self):
        if self.chargedAllIsoCache is None:
            chargedAllIsoType = 13
            myVetoes = reco.IsoDeposit.Vetos()
            iso = self.sourcePtr().isoDeposit(chargedAllIsoType).depositWithin(0.4,
                                                                               myVetoes,False)
            vetoSize = 0.01
            if self.sourcePtr().isEE():
                vetoSize = 0.015
            iso_veto = self.sourcePtr().isoDeposit(chargedAllIsoType).depositWithin(vetoSize,
                                                                                    myVetoes,False)
            iso -= iso_veto
            self.chargedAllIsoCache = iso
        return self.chargedAllIsoCache

            
    def tightIdForEleTau(self):
        """reference numbers form the Htautau twiki

        https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#2012_Baseline_Selection
        """
        # if self.eventId == 428746: 
        #    print 'STOPPING'
        #    import pdb
        #    pdb.set_trace()

        #COLIN: loose id shouldn't be required here (Jose said)
        # if self.looseIdForEleTau() == False : return False
        #BUT: need to apply conversion rejection and missing inner hits cuts
        if self.numberOfHits() != 0: return False
        if not self.passConversionVeto(): return False
        
        # eta = abs( self.eta() )
        eta = abs( self.sourcePtr().superCluster().eta() )
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
        #self.tightIdResult = result
        return result
    

    def tightId( self ):
        return self.tightIdForEleTau()
        

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

