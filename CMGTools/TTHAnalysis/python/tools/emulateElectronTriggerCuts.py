def _susy2lss_idEmu_cuts(lep):
    if (abs(lep.pdgId())!=11): return True
    if (lep.full5x5_sigmaIetaIeta()>=(0.011 if abs(lep.superCluster().eta())<1.479 else 0.031)): return False
    if (lep.hadronicOverEm()>=0.08): return False
    if (abs(lep.deltaEtaSuperClusterTrackAtVtx())>=0.01): return False
    if (abs(lep.deltaPhiSuperClusterTrackAtVtx())>=(0.04 if abs(lep.superCluster().eta())<1.479 else 0.08)): return False
    if (abs((1.0/lep.ecalEnergy() - lep.eSuperClusterOverP()/lep.ecalEnergy()) if lep.ecalEnergy()>0. else 9e9)>=0.01): return False
    return True

def _susy2lss_idIsoEmu_cuts(lep):
    if (abs(lep.pdgId())!=11): return True
    if not _susy2lss_idEmu_cuts(lep): return False
    if (lep.ecalPFClusterIso()>=0.45*lep.pt()): return False
    if (lep.hcalPFClusterIso()>=0.25*lep.pt()): return False
    if (lep.dr03TkSumPt()>=0.2*lep.pt()): return False
    return True

