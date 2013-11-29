#!/bin/env python
from math import *
from CMGTools.TTHAnalysis.signedSip import *
from CMGTools.TTHAnalysis.analyzers.ntupleObjects import *

fourVectorType = NTupleObjectType("fourVector", variables = [
    NTupleVariable("pt",    lambda x : x.pt()),
    NTupleVariable("eta",   lambda x : x.eta()),
    NTupleVariable("phi",   lambda x : x.phi()),
    NTupleVariable("mass",  lambda x : x.mass()),
])
particleType = NTupleObjectType("particle", baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("pdgId",   lambda x : x.pdgId(), int),
])
leptonType = NTupleObjectType("lepton", baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("charge",   lambda x : x.charge(), int),
    NTupleVariable("dxy",   lambda x : x.dxy(), help="d_{xy} with respect to PV, in cm (with sign)"),
    NTupleVariable("dz",    lambda x : x.dz() , help="d_{z} with respect to PV, in cm (with sign)"),
    NTupleVariable("ip3d",  lambda x : abs(signedIp3D(x)) , help="d_{3d} with respect to PV, in cm (absolute value)"),
    NTupleVariable("sip3d",  lambda x : x.sip3D(), help="S_{ip3d} with respect to PV (absolute value)"),
    NTupleVariable("relIso",  lambda x : x.relIso(dBetaFactor=0.5), help="PF Iso, R=0.4, with deltaBeta correction"),
    NTupleVariable("mcMatchId",   lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
])
leptonTypeTTH = NTupleObjectType("leptonTTH", baseObjectTypes = [ leptonType ], variables = [
    NTupleVariable("mva",   lambda x : x.mvaValue, help="Lepton MVA"),
])

jetType = NTupleObjectType("jet",  baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("btagCSV",   lambda x : x.btag('combinedSecondaryVertexBJetTags'), help="CSV discriminator"),
    NTupleVariable("rawPt",     lambda x : x.pt() * x.rawFactor(), help="p_{T} before JEC"),
    NTupleVariable("mcPt",      lambda x : x.mcJet.pt() if x.mcJet else 0., mcOnly=True, help="p_{T} of associated gen jet"),
    NTupleVariable("mcFlavour", lambda x : abs(x.partonFlavour()), int,     mcOnly=True, help="parton flavour (physics definition, i.e. including b's from shower)"),
])
        
metType = NTupleObjectType("met", baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("sumEt", lambda x : x.sumEt() ),
    NTupleVariable("genPt",  lambda x : x.genMET().pt() , mcOnly=True ),
    NTupleVariable("genPhi", lambda x : x.genMET().phi(), mcOnly=True ),
    NTupleVariable("genEta", lambda x : x.genMET().eta(), mcOnly=True ),
])    
