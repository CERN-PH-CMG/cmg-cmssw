from PhysicsTools.HeppyCore.utils.deltar import *


#MUON ID used in both Z and W : For Z we dont ask for a PF muon due to the boost
def muonIDCommon(mu):
    decision =  mu.isGlobalMuon() and mu.isTrackerMuon() and mu.segmentCompatibility()>0.3 \
            and mu.innerTrack().validFraction()>0.7
    return decision


#Additional MUON ID used in W case. Here we need PF muon for the MET
def leptonIDW(W):
    if abs(W.leg1.pdgId())==11:
        return True
    else:
        return W.leg1.isPFMuon()


#Muon isolation for Z:footprint removal of both! 

def electronID(e):
    decisionBarrel = abs(e.superCluster().eta())<1.4442 and \
        e.ecalDriven() and abs(e.deltaEtaSeedClusterTrackAtVtx())<0.004 and \
       abs( e.deltaPhiSuperClusterTrackAtVtx())< 0.06 and (e.hadronicOverEm()<2.0/e.superCluster().energy()+0.05) and \
       (e.e2x5Max()/e.e5x5()>0.94 or e.e1x5()/e.e5x5()>0.83) and abs(e.dxy())<0.02


    decisionEndcap = abs(e.superCluster().eta())>1.566 and \
        e.ecalDriven() and abs(e.deltaEtaSeedClusterTrackAtVtx())<0.006 and \
       abs( e.deltaPhiSuperClusterTrackAtVtx())< 0.06 and (e.hadronicOverEm()<12.5/e.superCluster().energy()+0.05) and \
       abs(e.dxy())<0.05 and e.full5x5_sigmaIetaIeta()<0.03

    return decisionBarrel or decisionEndcap




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


    
