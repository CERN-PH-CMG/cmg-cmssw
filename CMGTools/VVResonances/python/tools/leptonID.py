


def muonID(mu):
    decision =  mu.isGlobalMuon() and mu.isTrackerMuon() and mu.segmentCompatibility()>0.3 \
            and mu.innerTrack().validFraction()>0.7 and mu.miniRelIso<0.1
    return decision


def electronID(e):
    decisionBarrel = abs(e.superCluster().eta())<1.4442 and \
        e.ecalDriven() and abs(e.deltaEtaSeedClusterTrackAtVtx())<0.004 and \
       abs( e.deltaPhiSuperClusterTrackAtVtx())< 0.06 and (e.hadronicOverEm()<2.0/e.superCluster().energy()+0.05) and \
       (e.e2x5Max()/e.e5x5()>0.94 or e.e1x5()/e.e5x5()>0.83) and abs(e.dxy())<0.02 and e.miniRelIso<0.1


    decisionEndcap = abs(e.superCluster().eta())>1.566 and \
        e.ecalDriven() and abs(e.deltaEtaSeedClusterTrackAtVtx())<0.006 and \
       abs( e.deltaPhiSuperClusterTrackAtVtx())< 0.06 and (e.hadronicOverEm()<12.5/e.superCluster().energy()+0.05) and \
       abs(e.dxy())<0.05 and e.full5x5_sigmaIetaIeta()<0.03 and e.miniRelIso<0.1

    return decisionBarrel or decisionEndcap


