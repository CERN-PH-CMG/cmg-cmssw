#!/bin/env python
from math import *
from CMGTools.TTHAnalysis.signedSip import *
from CMGTools.TTHAnalysis.analyzers.ntupleObjects import *
from CMGTools.RootTools.utils.DeltaR import deltaR


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
    NTupleVariable("edxy",  lambda x : x.sourcePtr().edB(), help="#sigma(d_{xy}) with respect to PV, in cm"),
    NTupleVariable("edz",   lambda x : x.sourcePtr().gsfTrack().dzError() if abs(x.pdgId())==11 else x.sourcePtr().innerTrack().dzError() , help="#sigma(d_{z}) with respect to PV, in cm"),
    NTupleVariable("ip3d",  lambda x : abs(signedIp3D(x)) , help="d_{3d} with respect to PV, in cm (absolute value)"),
    NTupleVariable("sip3d",  lambda x : x.sip3D(), help="S_{ip3d} with respect to PV (absolute value)"),
    NTupleVariable("relIso",  lambda x : x.relIso(dBetaFactor=0.5), help="PF Iso, R=0.4, with deltaBeta correction"),
    NTupleVariable("chargedRelIso", lambda x : x.chargedHadronIso()/x.pt(), help="PF Iso from charged hadrons only, R=0.4"),
    NTupleVariable("tightId",     lambda x : x.tightId(), int, help="POG Tight ID (based on triggering MVA value for electrons, boolean for muons)"),
    NTupleVariable("convVeto",    lambda x : x.sourcePtr().passConversionVeto() if abs(x.pdgId())==11 else 1, int, help="Conversion veto (always true for muons)"),
])
leptonTypeTTH = NTupleObjectType("leptonTTH", baseObjectTypes = [ leptonType ], variables = [
    NTupleVariable("tightCharge", lambda lepton : ( lepton.sourcePtr().isGsfCtfScPixChargeConsistent() + lepton.sourcePtr().isGsfScPixChargeConsistent() ) if abs(lepton.pdgId()) == 11 else 2*(lepton.sourcePtr().innerTrack().ptError()/lepton.sourcePtr().innerTrack().pt() < 0.2), int, help="Tight charge criteria"),
    NTupleVariable("mvaId",      lambda lepton : lepton.mvaNonTrigV0() if abs(lepton.pdgId()) == 11 else 1, help="EGamma POG MVA ID for non-triggering electrons (as HZZ); 1 for muons"),
    NTupleVariable("mvaIdTrig",  lambda lepton : lepton.mvaTrigV0()    if abs(lepton.pdgId()) == 11 else 1, help="EGamma POG MVA ID for triggering electrons; 1 for muons"),
    NTupleVariable("mva",        lambda lepton : lepton.mvaValue, help="Lepton MVA (ttH version)"),
    NTupleVariable("jetPtRatio", lambda lepton : lepton.pt()/lepton.jet.pt()),
    NTupleVariable("jetBTagCSV", lambda lepton : lepton.jet.btag('combinedSecondaryVertexBJetTags') if hasattr(lepton.jet, 'btag') else -99),
    NTupleVariable("jetDR",      lambda lepton : deltaR(lepton.eta(),lepton.phi(),lepton.jet.eta(),lepton.jet.phi())),
    NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
    NTupleVariable("mcMatchAny",  lambda x : x.mcMatchAny, int, mcOnly=True, help="Match to any final state leptons: -mcMatchId if prompt, 0 if unmatched, 1 if light flavour, 2 if heavy flavour (b)"),
    NTupleVariable("mcMatchTau",  lambda x : x.mcMatchTau, int, mcOnly=True, help="True if the leptons comes from a tau"),
])

tauType = NTupleObjectType("tau",  baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("charge",   lambda x : x.charge(), int),
    NTupleVariable("dxy",   lambda x : x.dxy(), help="d_{xy} of lead track with respect to PV, in cm (with sign)"),
    NTupleVariable("dz",    lambda x : x.dz() , help="d_{z} of lead track with respect to PV, in cm (with sign)"),
    ## more to be filled in: ID, Iso
])
tauTypeTTH = NTupleObjectType("tauTTH", baseObjectTypes = [ tauType ], variables = [
    NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
])

jetType = NTupleObjectType("jet",  baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("btagCSV",   lambda x : x.btag('combinedSecondaryVertexBJetTags'), help="CSV discriminator"),
    NTupleVariable("rawPt",     lambda x : x.pt() * x.rawFactor(), help="p_{T} before JEC"),
    NTupleVariable("mcPt",      lambda x : x.mcJet.pt() if x.mcJet else 0., mcOnly=True, help="p_{T} of associated gen jet"),
    NTupleVariable("mcFlavour", lambda x : abs(x.partonFlavour()), int,     mcOnly=True, help="parton flavour (physics definition, i.e. including b's from shower)"),
])
jetTypeTTH = NTupleObjectType("jetTTH",  baseObjectTypes = [ jetType ], variables = [
    NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
])
        
metType = NTupleObjectType("met", baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("sumEt", lambda x : x.sumEt() ),
    NTupleVariable("genPt",  lambda x : x.genMET().pt() , mcOnly=True ),
    NTupleVariable("genPhi", lambda x : x.genMET().phi(), mcOnly=True ),
    NTupleVariable("genEta", lambda x : x.genMET().eta(), mcOnly=True ),
])    

genParticleType = NTupleObjectType("genParticle", baseObjectTypes = [ particleType ], mcOnly=True, variables = [
    NTupleVariable("charge",   lambda x : x.threeCharge()/3.0, float),
])
genParticleWithSourceType = NTupleObjectType("genParticleWithSource", baseObjectTypes = [ genParticleType ], mcOnly=True, variables = [
    NTupleVariable("sourceId", lambda x : x.sourceId, int, help="origin of the particle: 6=t, 25=h, 23/24=W/Z")
])

