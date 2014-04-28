from CMGTools.RootTools.physicsobjects.Lepton import Lepton

class Muon( Lepton ):

    def looseId( self ):
        '''Loose ID as recommended by mu POG.'''
        return self.physObj.isLooseMuon()

    def tightId( self ):
        '''Tight ID as recommended by mu POG.'''
        return self.muonID("POG_Tight")

    def muonID(self, name, vertex=None):
        if name.startswith("POG_"):
            if name == "POG_Loose": return self.physObj.isLooseMuon()
            if vertex is None:
                vertex = self.associatedVertex
            if name == "POG_Tight":  return self.physObj.isTightMuon(vertex)
            if name == "POG_HighPt": return self.physObj.isHighPtMuon(vertex)
            if name == "POG_Soft":   return self.physObj.isSoftMuon(vertex)
            if name == "POG_TightNoVtx":  return self.looseId() and \
                                                 self.isGlobalMuon() and \
                                                 self.globalTrack().normalizedChi2() < 10 and \
                                                 self.globalTrack().hitPattern().numberOfValidMuonHits() > 0 and \
                                                 self.numberOfMatchedStations()>1 and \
                                                 self.innerTrack().hitPattern().numberOfValidPixelHits()>0 and \
                                                 self.innerTrack().hitPattern().trackerLayersWithMeasurement() > 5
        return self.physObj.muonID(name)
            
    def mvaId(self):
        '''For a transparent treatment of electrons and muons. Returns -99'''
        return -99
    
   
    def detIso( self, rho ):
        '''Rho corrected detector-based isolation, for the H->ZZ->4l baseline analysis'''
        isoEcal = self.ecalIso()
        isoHcal = self.hcalIso()
        isoTk   = self.userIsolation( 7 )
        isoEcal, isoHcal = self.rhoCorrMu(rho, isoEcal, isoHcal)
        return (isoEcal + isoHcal + isoTk)/self.pt()

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
        return self.absIsoFromEA(rho,self.eta(),effectiveAreas.muon)



    def dxy(self, vertex=None):
        '''either pass the vertex, or set associatedVertex before calling the function.
        note: the function does not work with standalone muons as innerTrack
        is not available.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.innerTrack().dxy( vertex.position() )
 

    def dz(self, vertex=None):
        '''either pass the vertex, or set associatedVertex before calling the function.
        note: the function does not work with standalone muons as innerTrack
        is not available.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.innerTrack().dz( vertex.position() )

