from PhysicsTools.HeppyCore.utils.deltar import *





#MUON ID used in both Z and W : For Z we dont ask for a PF muon due to the boost

def muonIDHighPt(mu):   
    decision =  mu.isGlobalMuon() and mu.globalTrack().hitPattern().numberOfValidMuonHits() > 0 and mu.numberOfMatchedStations() > 1 and\
        mu.muonBestTrack().ptError()/mu.muonBestTrack().pt()<0.3 and mu.dB()< 0.2 and mu.innerTrack().hitPattern().numberOfValidPixelHits()>0 and\
    mu.innerTrack().hitPattern().trackerLayersWithMeasurement()>5
    return decision

def muonIDTrackerHighPt(mu):    
    decision =  mu.isTrackerMuon() and mu.numberOfMatchedStations() > 1 and\
        mu.muonBestTrack().ptError()/mu.muonBestTrack().pt()<0.3 and mu.dB()< 0.2 and mu.innerTrack().hitPattern().numberOfValidPixelHits() > 0 and\
    mu.innerTrack().hitPattern().trackerLayersWithMeasurement()>5
    return decision





def electronID(e):
    decisionBarrel = abs(e.superCluster().eta())<1.4442 and \
        e.ecalDriven() and abs(e.deltaEtaSeedClusterTrackAtVtx())<0.004 and \
       abs( e.deltaPhiSuperClusterTrackAtVtx())< 0.06 and (e.hadronicOverEm()<1.0/e.superCluster().energy()+0.05) and \
       (e.e2x5Max()/e.e5x5()>0.94 or e.e1x5()/e.e5x5()>0.83) and abs(e.dxy())<0.02


    decisionEndcap = abs(e.superCluster().eta())>1.566 and \
        e.ecalDriven() and abs(e.deltaEtaSeedClusterTrackAtVtx())<0.006 and \
       abs( e.deltaPhiSuperClusterTrackAtVtx())< 0.06 and (e.hadronicOverEm()<5.0/e.superCluster().energy()+0.05) and \
       abs(e.dxy())<0.05 and e.full5x5_sigmaIetaIeta()<0.03

    return decisionBarrel or decisionEndcap



#Additional MUON ID used in W case. Here we need PF muon for the MET
def leptonIDW(W):
    if abs(W.leg1.pdgId())==11:
        return W.leg1.pt()>120 and electronID(W.leg1)
    else:
        return W.leg1.isPFMuon() and muonIDHighPt(W.leg1) and W.leg1.pt()>53 and abs(W.leg1.eta())<2.1


#Muon isolation for Z:footprint removal of both! 


def leptonIDZ(Z):
    if abs(Z.leg1.pdgId())==13:
        return ( (muonIDHighPt(Z.leg1) and muonIDHighPt(Z.leg2)) or (muonIDHighPt(Z.leg1) and muonIDTrackerHighPt(Z.leg2)) or (muonIDTrackerHighPt(Z.leg1) and muonIDHighPt(Z.leg2))) and max(Z.leg1.pt(),Z.leg2.pt())>50 and min(Z.leg1.pt(),Z.leg2.pt())>20  
    else:
        return electronID(Z.leg1) and electronID(Z.leg2) and max(Z.leg1.pt(),Z.leg2.pt())>115 and min(Z.leg1.pt(),Z.leg2.pt())>35  
        
     

#Isolation for Z
def isolationZ(z):

    if abs(z.leg1.pdgId())==13:
        iso1=(z.leg1.cleanedChargedIso+z.leg1.cleanedNeutralIsoDB)/z.leg1.pt()<0.4
        iso2=(z.leg2.cleanedChargedIso+z.leg2.cleanedNeutralIsoDB)/z.leg2.pt()<0.4
        return iso1 and iso2
    if abs(z.leg1.pdgId())==11:
        iso1=(z.leg1.cleanedChargedIso+z.leg1.cleanedNeutralIsoRho)/z.leg1.pt()<0.4
        iso2=(z.leg2.cleanedChargedIso+z.leg2.cleanedNeutralIsoRho)/z.leg2.pt()<0.4
        return iso1 and iso2


#Isolation for Z
def isolationW(z):
    if abs(z.leg1.pdgId())==13:
        return ((z.leg1.cleanedChargedIso+z.leg1.cleanedNeutralIsoDB)/z.leg1.pt()<0.2)
    if abs(z.leg1.pdgId())==11:
        return ((z.leg1.cleanedChargedIso+z.leg1.cleanedNeutralIsoRho)/z.leg1.pt()<0.2)


    
