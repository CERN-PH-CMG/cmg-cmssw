from CMGTools.TTHAnalysis.treeReAnalyzer import *

def friendJetLepAwareJEC(lep,additionalJEC=1):
    l = ROOT.TLorentzVector()
    l.SetPtEtaPhiM(lep.pt,lep.eta,lep.phi,lep.mass)
    j = ROOT.TLorentzVector()
    j.SetPtEtaPhiE(lep.jetPt,lep.jetEta,lep.jetPhi,lep.jetE)
    if ((j*(lep.jetRawPt/lep.jetPt)-l).Rho()<1e-4): return l # matched to jet containing only the lepton
    j = (j*(lep.jetRawPt/lep.jetPt)-l*(1.0/lep.jetCorrFactor_L1))*lep.jetCorrFactor_L1L2L3Res*additionalJEC+l
    return j
def friendPtRatiov2(lep,additionalJEC=1):
    return lep.pt/(friendJetLepAwareJEC(lep,additionalJEC).Pt())
def friendPtRelv2(lep,additionalJEC=1):
    m = friendJetLepAwareJEC(lep,additionalJEC)
    l = ROOT.TLorentzVector()
    l.SetPtEtaPhiM(lep.pt,lep.eta,lep.phi,lep.mass)
    if ((m-l).Rho()<1e-4): return 0 # lep.jet==lep (no match) or jet containing only the lepton
    return l.Perp((m-l).Vect())
