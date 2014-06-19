from CMGTools.RootTools.physicsobjects.Lepton import Lepton

class Muon( Lepton ):

    def muonID( self, id, vertex=None ):
        if id is None or id == "": return True
        if vertex == None and hasattr(self,'associatedVertex') and self.associatedVertex != None: vertex = self.associatedVertex
        return self.muonID_cpp_(id,vertex)

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
               self.numberOfMatchedStations()>1 and \
               self.sourcePtr().innerTrack().hitPattern().numberOfValidPixelHits()>0 and \
               self.trackerLayersWithMeasurement() > 5 

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
        return self.absIsoFromEA(rho,self.eta(),effectiveAreas.muon)



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

