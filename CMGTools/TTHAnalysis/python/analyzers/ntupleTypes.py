#!/bin/env python
from math import *
from CMGTools.TTHAnalysis.signedSip import *
from CMGTools.TTHAnalysis.analyzers.ntupleObjects import *
from CMGTools.RootTools.utils.DeltaR import deltaR

twoVectorType = NTupleObjectType("twoVector", variables = [
    NTupleVariable("pt",    lambda x : x.pt()),
    NTupleVariable("phi",   lambda x : x.phi()),
])

fourVectorType = NTupleObjectType("fourVector", variables = [
    NTupleVariable("pt",    lambda x : x.pt()),
    NTupleVariable("eta",   lambda x : x.eta()),
    NTupleVariable("phi",   lambda x : x.phi()),
    NTupleVariable("mass",  lambda x : x.mass()),
    NTupleVariable("p4",    lambda x : x, "TLorentzVector", default=ROOT.reco.Particle.LorentzVector(0.,0.,0.,0.), filler = lambda vector, obj: vector.SetPtEtaPhiM(obj.pt(), obj.eta(), obj.phi(), obj.mass())),
    #               ^^^^------- Note: p4 normally is not saved unless 'saveTLorentzVectors' is enabled in the tree producer
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
    NTupleVariable("tightId",     lambda x : x.tightId(), int, help="POG Tight ID (based on triggering MVA value for electrons, boolean for muons)"),
    NTupleVariable("convVeto",    lambda x : x.sourcePtr().passConversionVeto() if abs(x.pdgId())==11 else 1, int, help="Conversion veto (always true for muons)"),
    NTupleVariable("lostHits",    lambda x : x.numberOfHits() if abs(x.pdgId())==11 else x.sourcePtr().innerTrack().trackerExpectedHitsInner().numberOfLostHits(), int, help="Number of lost hits on inner track")
])
leptonTypeTTH = NTupleObjectType("leptonTTH", baseObjectTypes = [ leptonType ], variables = [
    NTupleVariable("relIso",  lambda x : x.relIso(dBetaFactor=0.5), help="PF Iso, R=0.4, with deltaBeta correction"),
    NTupleVariable("chargedRelIso", lambda x : x.chargedHadronIso()/x.pt(), help="PF Iso from charged hadrons only, R=0.4"),
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
    NTupleVariable("convVetoFull", lambda x : (x.sourcePtr().passConversionVeto() and x.numberOfHits() == 0) if abs(x.pdgId())==11 else 1, int, help="Conv veto + no missing hits for electrons, always true for muons."),
])
leptonTypeSusy = NTupleObjectType("leptonSusy", baseObjectTypes = [ leptonType ], variables = [
    # Loose id 
    NTupleVariable("looseIdSusy", lambda x : x.looseIdSusy if hasattr(x, 'looseIdSusy') else -1, int, help="Loose ID for Susy ntuples (always true on selected leptons)"),
    # Isolations with the two radia
    NTupleVariable("relIso03",  lambda x : x.relIso03, help="PF Rel Iso, R=0.3, with deltaBeta correction for muons and rho corrections for electrons"),
    NTupleVariable("relIso04",  lambda x : x.relIso04, help="PF Rel Iso, R=0.4, with deltaBeta correction for muons and rho corrections for electrons"),
    NTupleVariable("chargedHadRelIso03",  lambda x : x.chargedHadronIso(0.3)/x.pt(), help="PF Rel Iso, R=0.3, charged hadrons only"),
    NTupleVariable("chargedHadRelIso04",  lambda x : x.chargedHadronIso(0.4)/x.pt(), help="PF Rel Iso, R=0.4, charged hadrons only"),
    # Extra electron id variables
    NTupleVariable("convVetoFull", lambda x : (x.sourcePtr().passConversionVeto() and x.numberOfHits() == 0) if abs(x.pdgId())==11 else 1, int, help="Conv veto + no missing hits for electrons, always true for muons."),
    NTupleVariable("eleCutId",     lambda x : (1*x.electronID("POG_Cuts_ID_Veto") + 1*x.electronID("POG_Cuts_ID_Loose") + 1*x.electronID("POG_Cuts_ID_Medium") + 1*x.electronID("POG_Cuts_ID_Tight")) if abs(x.pdgId()) == 11 else -1, int, help="Electron cut-based id: 0=none, 1=veto, 2=loose, 3=medium, 4=tight"),
    NTupleVariable("eleMVAId",     lambda x : (x.electronID("POG_MVA_ID_NonTrig") + 2*x.electronID("POG_MVA_ID_Trig")) if abs(x.pdgId()) == 11 else -1, int, help="Electron mva id working point: 0=none, 1=non-trig, 2=trig, 3=both"),
    NTupleVariable("tightCharge",  lambda lepton : ( lepton.sourcePtr().isGsfCtfScPixChargeConsistent() + lepton.sourcePtr().isGsfScPixChargeConsistent() ) if abs(lepton.pdgId()) == 11 else 2*(lepton.sourcePtr().innerTrack().ptError()/lepton.sourcePtr().innerTrack().pt() < 0.2), int, help="Tight charge criteria"),
    NTupleVariable("mvaId",        lambda lepton : lepton.mvaNonTrigV0() if abs(lepton.pdgId()) == 11 else 1, help="EGamma POG MVA ID for non-triggering electrons (as HZZ); 1 for muons"),
    NTupleVariable("mvaIdTrig",    lambda lepton : lepton.mvaTrigV0()    if abs(lepton.pdgId()) == 11 else 1, help="EGamma POG MVA ID for triggering electrons; 1 for muons"),
    # Muon-speficic info
    NTupleVariable("nStations",    lambda lepton : lepton.sourcePtr().numberOfMatchedStations() if abs(lepton.pdgId()) == 13 else 4, help="Number of matched muons stations (4 for electrons)"),
    NTupleVariable("trkKink",      lambda lepton : lepton.sourcePtr().combinedQuality().trkKink if abs(lepton.pdgId()) == 13 else 0, help="Tracker kink-finder"), 
    # Extra tracker-related id variables
    NTupleVariable("trackerLayers", lambda x : (x.sourcePtr().track() if abs(x.pdgId())==13 else x.sourcePtr().gsfTrack()).hitPattern().trackerLayersWithMeasurement(), int, help="Tracker Layers"),
    NTupleVariable("pixelLayers", lambda x : (x.sourcePtr().track() if abs(x.pdgId())==13 else x.sourcePtr().gsfTrack()).hitPattern().pixelLayersWithMeasurement(), int, help="Pixel Layers"),
    # TTH-id related variables
    NTupleVariable("mvaTTH",     lambda lepton : lepton.mvaValue if hasattr(lepton,'mvaValue') else -1, help="Lepton MVA (ttH version)"),
    NTupleVariable("jetPtRatio", lambda lepton : lepton.pt()/lepton.jet.pt() if hasattr(lepton,'jet') else -1, help="pt(lepton)/pt(nearest jet)"),
    NTupleVariable("jetBTagCSV", lambda lepton : lepton.jet.btag('combinedSecondaryVertexBJetTags') if hasattr(lepton,'jet') and hasattr(lepton.jet, 'btag') else -99, help="btag of nearest jet"),
    NTupleVariable("jetDR",      lambda lepton : deltaR(lepton.eta(),lepton.phi(),lepton.jet.eta(),lepton.jet.phi()) if hasattr(lepton,'jet') else -1, help="deltaR(lepton, nearest jet)"),
    # MC-match info
    NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
    NTupleVariable("mcMatchAny",  lambda x : x.mcMatchAny, int, mcOnly=True, help="Match to any final state leptons: -mcMatchId if prompt, 0 if unmatched, 1 if light flavour, 2 if heavy flavour (b)"),
    NTupleVariable("mcMatchTau",  lambda x : x.mcMatchTau, int, mcOnly=True, help="True if the leptons comes from a tau"),
])

leptonTypeSusyFR = NTupleObjectType("leptonSusyFR", baseObjectTypes = [ leptonTypeTTH ], variables = [
    NTupleVariable("relIso03",  lambda x : x.relIso03, help="PF Rel Iso, R, with deltaBeta correction"),
    NTupleVariable("looseFakeId", lambda x : x.looseFakeId, int, help="Loose ID for Susy Fake Rate exercise"),
    NTupleVariable("tightFakeId", lambda x : x.tightFakeId, int, help="Tight ID for Susy Fake Rate exercise"),
    #NTupleVariable("rho",  lambda x : x.rho if hasattr(x, 'rho') else -99.0, help="PF Rel Iso, R, with deltaBeta correction"),
    #NTupleVariable("EA",  lambda x : x.EffectiveArea if hasattr(x, 'EffectiveArea') else -99.0, help="PF Rel Iso, R, with deltaBeta correction"),
    #NTupleVariable("relIso03_ch",  lambda x : x.chargedHadronIso(0.3) if abs(x.pdgId())==11 else 0, help="PF Rel Iso, R, with deltaBeta correction"),
    #NTupleVariable("relIso03_nh",  lambda x : x.neutralHadronIso(0.3) if abs(x.pdgId())==11 else 0, help="PF Rel Iso, R, with deltaBeta correction"),
    #NTupleVariable("relIso03_ph",  lambda x : x.photonIso(0.3)        if abs(x.pdgId())==11 else 0, help="PF Rel Iso, R, with deltaBeta correction"),
])
leptonTypeFull = NTupleObjectType("leptonFull", baseObjectTypes = [ leptonTypeSusy ], variables = [
    NTupleVariable("pfMuonId",    lambda x : x.muonID("POG_ID_Loose") if abs(x.pdgId())==13 else 1, int, help="Muon POG Loose id"),
    NTupleVariable("softMuonId",    lambda x : x.muonID("POG_ID_Soft") if abs(x.pdgId())==13 else 1, int, help="Muon POG Soft id"),
])
 

tauType = NTupleObjectType("tau",  baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("charge",   lambda x : x.charge(), int),
    NTupleVariable("dxy",   lambda x : x.dxy(), help="d_{xy} of lead track with respect to PV, in cm (with sign)"),
    NTupleVariable("dz",    lambda x : x.dz() , help="d_{z} of lead track with respect to PV, in cm (with sign)"),
    NTupleVariable("idMVA2", lambda x : x.idMVA2, int, help="1,2,3 if the tau passes the loose, medium, tight WP of the By<X>IsolationMVA2 discriminator"),
    NTupleVariable("idCI3hit", lambda x : x.idCI3hit, int, help="1,2,3 if the tau passes the loose, medium, tight WP of the By<X>CombinedIsolationDBSumPtCorr3Hits discriminator"),
    NTupleVariable("isoMVA2",  lambda x : x.tauID("byIsolationMVA2raw"), help="ByIsolationMVA2 raw output discriminator"),
])
tauTypeTTH = NTupleObjectType("tauTTH", baseObjectTypes = [ tauType ], variables = [
    NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
])
tauTypeSusy = NTupleObjectType("tauSusy", baseObjectTypes = [ tauType ], variables = [
    NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
])


jetType = NTupleObjectType("jet",  baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("btagCSV",   lambda x : x.btag('combinedSecondaryVertexBJetTags'), help="CSV discriminator"),
    NTupleVariable("rawPt",     lambda x : x.pt() * x.rawFactor(), help="p_{T} before JEC"),
    NTupleVariable("mcPt",      lambda x : x.mcJet.pt() if x.mcJet else 0., mcOnly=True, help="p_{T} of associated gen jet"),
    NTupleVariable("mcFlavour", lambda x : abs(x.partonFlavour()), int,     mcOnly=True, help="parton flavour (physics definition, i.e. including b's from shower)"),
    NTupleVariable("quarkGluonID", lambda x : x.QG,  mcOnly=False),
])
jetTypeTTH = NTupleObjectType("jetTTH",  baseObjectTypes = [ jetType ], variables = [
    NTupleVariable("mcMatchId",    lambda x : x.mcMatchId,   int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
    NTupleVariable("mcMatchFlav",  lambda x : x.mcMatchFlav, int, mcOnly=True, help="Flavour of associated parton from hard scatter (if any)"),
])
jetTypeSusy = NTupleObjectType("jetSusy",  baseObjectTypes = [ jetType ], variables = [
    NTupleVariable("mcMatchId",    lambda x : x.mcMatchId,   int, mcOnly=True, help="Match to source from hard scatter (25 for H, 6 for t, 23/24 for W/Z)"),
    NTupleVariable("mcMatchFlav",  lambda x : x.mcMatchFlav, int, mcOnly=True, help="Flavour of associated parton from hard scatter (if any)"),
    NTupleVariable("PuId_full", lambda x : x.puId("full"), int,     mcOnly=False, help="puId full: returns an integeger containing 3 bits, one for each working point (loose-bit2, medium-bit1, tight-bit0)"),
    NTupleVariable("PuId_simple", lambda x : x.puId("simple"), int,    mcOnly=False, help="puId simple: returns an integeger containing 3 bits, one for each working point (loose-bit2, medium-bit1, tight-bit0)"),
    NTupleVariable("PuId_cut_based", lambda x : x.puId("cut-based"), int,    mcOnly=False, help="puId cut-based: returns an integeger containing 3 bits, one for each working point (loose-bit2, medium-bit1, tight-bit0)"),
    NTupleVariable("Id",    lambda x : x.jetID("POG_PFID_Loose") , int, mcOnly=False,help="POG Loose jet ID"),
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
genParticleWithMotherId = NTupleObjectType("genParticleWithMotherId", baseObjectTypes = [ genParticleType ], mcOnly=True, variables = [
    NTupleVariable("motherId", lambda x : x.motherId, int, help="pdgId of the mother of the particle")
])

