#!/bin/env python
from math import *
import ROOT
from PhysicsTools.Heppy.analyzers.core.autovars import *
from PhysicsTools.Heppy.analyzers.objects.autophobj  import *

lorentzVectorType = NTupleObjectType("lorentzVector", baseObjectTypes = [ fourVectorType ], variables = [])

particleType = NTupleObjectType("particle", baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("pdgId",   lambda x : x.pdgId(), int, help="particle Id"),
])

candidateType = NTupleObjectType("candidate", baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("tmass",   lambda x : getattr(x, "mT", -1.), int, help="transverse mass"),
    NTupleVariable("charge",   lambda x : getattr(x, "charge", -9.), int, help="charge"),
    NTupleVariable("dR",   lambda x : getattr(x, "deltaR", -1.), float, help="delta R"),
    NTupleVariable("dEta",   lambda x : getattr(x, "deltaEta", -9.), float, help="delta Eta"),
    NTupleVariable("dPhi",   lambda x : getattr(x, "deltaPhi", -9.), float, help="delta Phi"),
    #NTupleVariable("dPhi_met",   lambda x : getattr(x, "deltaPhi_met", -9.), float, help="delta Phi with met"),
    #NTupleVariable("dPhi_jet1",   lambda x : getattr(x, "deltaPhi_jet1", -9.), float, help="delta Phi with leading jet"),
])


candidateFullType = NTupleObjectType("candidate", baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("tmass",   lambda x : getattr(x, "mT", -1.), int, help="transverse mass"),
    NTupleVariable("cmass",   lambda x : getattr(x, "mC", -1.), int, help="collinear mass"),
    NTupleVariable("kmass",   lambda x : getattr(x, "mK", -1.), int, help="Mass with kinematic fit"),
    NTupleVariable("charge",   lambda x : getattr(x, "charge", -9.), int, help="charge"),
    NTupleVariable("dR",   lambda x : getattr(x, "deltaR", -1.), float, help="delta R"),
    NTupleVariable("dEta",   lambda x : getattr(x, "deltaEta", -9.), float, help="delta Eta"),
    NTupleVariable("dPhi",   lambda x : getattr(x, "deltaPhi", -9.), float, help="delta Phi"),
    #NTupleVariable("dPhi_met",   lambda x : getattr(x, "deltaPhi_met", -9.), float, help="delta Phi with met"),
    #NTupleVariable("dPhi_jet1",   lambda x : getattr(x, "deltaPhi_jet1", -9.), float, help="delta Phi with leading jet"),
])


leptonType = NTupleObjectType("lepton", baseObjectTypes = [ particleType ], variables = [
    # Is Muon/Electron
    NTupleVariable("isElectron",   lambda x : True if x.isElectron() else False, int, help="Electron flag" ),
    NTupleVariable("isMuon",   lambda x : True if x.isMuon() else False, int, help="Muon flag" ),
    # Charge
    NTupleVariable("charge",   lambda x : x.charge(), int, help="Lepton charge"),
    # Impact parameters
    NTupleVariable("ip2d", lambda x : x.dB(1), float, help="Lepton 2D impact parameter"),
    NTupleVariable("ip3d", lambda x : x.dB(2), float, help="Lepton 3D impact parameter"),
    NTupleVariable("dz",  lambda x : x.dz(), float, help="Lepton dz"),
    # Isolations with the two radia
    NTupleVariable("relIso03",  lambda x : x.relIso03, float, help="PF Rel Iso, R=0.3, pile-up corrected"),
    NTupleVariable("relIso04",  lambda x : x.relIso04, float, help="PF Rel Iso, R=0.4, pile-up corrected"),
    NTupleVariable("miniIso", lambda x : getattr(x, "miniRelIso", -1.), float, help="Rel Mini-Iso, pile-up corrected"),
    # Cut Based Identification
    #NTupleVariable("vetoId",   lambda x : x.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto") if x.isElectron() else -99., int, help="Cut Based Veto id" ),
    NTupleVariable("looseId",   lambda x : x.muonID("POG_ID_Loose") if x.isMuon() else x.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Loose"), int, help="Cut Based Loose id" ),
    NTupleVariable("mediumId",   lambda x : x.muonID("POG_ID_Medium") if x.isMuon() else x.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Medium"), int, help="Cut Based Medium id"),
    NTupleVariable("tightId",   lambda x : x.muonID("POG_ID_Tight") if x.isMuon() else x.electronID("POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Tight"), int, help="Cut Based Tight id"),
    NTupleVariable("highptId",   lambda x : x.muonID("POG_ID_HighPt") if x.isMuon() else getattr(x, "isHEEP", 0), int, help="Cut Based Tight id"),
])

muonType = NTupleObjectType("muon", baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("charge", lambda x : x.charge(), int, help="Muon charge"),
    # Identification
    NTupleVariable("looseId",  lambda x : x.muonID("POG_ID_Loose") if x.isMuon() else -9, int, help="Muon POG Loose id" ),
    NTupleVariable("mediumId", lambda x : x.muonID("POG_ID_Medium") if x.isMuon() else -9, int, help="Muon POG Medium id"),
    NTupleVariable("tightId",  lambda x : x.muonID("POG_ID_Tight") if x.isMuon() else -9, int, help="Muon POG Tight id"),
    # Relative Isolations fixed ratia / variable ratio
    NTupleVariable("relIso03",   lambda x : x.relIso03, help="Muon PF Rel Iso, R=0.3, pile-up corrected"),
    NTupleVariable("relIso04",   lambda x : x.relIso04, help="Muon PF Rel Iso, R=0.4, pile-up corrected"),
    NTupleVariable("miniIso",    lambda x : x.miniRelIso, help="Muon Rel Mini-Iso, pile-up corrected"),
    # Impact parameters
    NTupleVariable("ip2d", lambda x : x.dB(1), float, help="Lepton 2D impact parameter"),
    NTupleVariable("ip3d", lambda x : x.dB(2), float, help="Lepton 3D impact parameter"),
    NTupleVariable("dz",  lambda x : x.dz(), float, help="Lepton dz"),

])

electronType = NTupleObjectType("electron", baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("charge",   lambda x : x.charge(), int, help="Electron charge"),
    # Identification
    NTupleVariable("vetoId",   lambda x : x.cutBasedId("POG_CSA14_25ns_v1_Veto") if abs(x.pdgId())==11 else -9, int, help="Electron POG Cut-based Veto id"),
    NTupleVariable("looseId",  lambda x : x.cutBasedId("POG_CSA14_25ns_v1_Loose") if abs(x.pdgId())==11 else -9, int, help="Electron POG Cut-based Loose id"),
    NTupleVariable("mediumId", lambda x : x.cutBasedId("POG_CSA14_25ns_v1_Medium") if abs(x.pdgId())==11 else -9, int, help="Electron POG Cut-based Medium id"),
    NTupleVariable("tightId",  lambda x : x.cutBasedId("POG_CSA14_25ns_v1_Tight") if abs(x.pdgId())==11 else -9, int, help="Electron POG Cut-based Tight id"),
    # Isolations with the two radia
    NTupleVariable("relIso03",  lambda x : x.relIso03, help="Electron PF Rel Iso, R=0.3, pile-up corrected"),
    NTupleVariable("relIso04",  lambda x : x.relIso04, help="Electron PF Rel Iso, R=0.4, pile-up corrected"),
    # Impact parameters
    NTupleVariable("ip2d", lambda x : x.dB(1), float, help="Lepton 2D impact parameter"),
    NTupleVariable("ip3d", lambda x : x.dB(2), float, help="Lepton 3D impact parameter"),
    NTupleVariable("dz",  lambda x : x.dz(), float, help="Lepton dz"),
    ### ELECTRON SPECIFIC VARS
    #NTupleVariable("MvaIdNonTrig",   lambda x : x.electronID("POG_MVA_ID_NonTrig") if x.isElectron() else -99., int, help="MVA Electron id NonTrig" ),
    #NTupleVariable("MvaIdTrig",   lambda x : x.electronID("POG_MVA_ID_Trig") if x.isElectron() else -99., int, help="MVA Electron id Trig" ),
    #NTupleVariable("MvaIdNonTrig5x5",   lambda x : x.electronID("POG_MVA_ID_NonTrig_full5x5") if x.isElectron() else -99., int, help="MVA Electron id NonTrig5x5" ),
    #NTupleVariable("MvaIdTrig5x5",   lambda x : x.electronID("POG_MVA_ID_Trig_full5x5") if x.isElectron() else -99., int, help="MVA Electron id Trig5x5" ),
    #NTupleVariable("MvaIdNonTrigVLoose",   lambda x : x.electronID("POG_MVA_ID_Run2_NonTrig_VLoose") if x.isElectron() else -99., int, help="MVA Electron id VLoose" ),
    #NTupleVariable("MvaIdNonTrigLoose",   lambda x : x.electronID("POG_MVA_ID_Run2_NonTrig_Loose") if x.isElectron() else -99., int, help="MVA Electron id Loose" ),
    #NTupleVariable("MvaIdNonTrigMedium",   lambda x : x.electronID("POG_MVA_ID_Run2_NonTrig_Medium") if x.isElectron() else -99., int, help="MVA Electron id Medium" ),
    #NTupleVariable("MvaIdNonTrigTight",   lambda x : x.electronID("POG_MVA_ID_Run2_NonTrig_Tight") if x.isElectron() else -99., int, help="MVA Electron id Tight" ),

])

jetType = NTupleObjectType("jet",  baseObjectTypes = [ fourVectorType ], variables = [
    NTupleVariable("dPhi_met",   lambda x : getattr(x, "deltaPhi_met", -9.), float, help="dPhi between jet and met"),
    NTupleVariable("dPhi_jet1",   lambda x : getattr(x, "deltaPhi_jet1", -9.), float, help="dPhi between jet and leading jet"),
    #NTupleVariable("puId", lambda x : getattr(x, 'puJetIdPassed', -999.), int,     mcOnly=False, help="puId (full MVA, loose WP, 5.3.X training on AK5PFchs: the only thing that is available now)"),
    NTupleVariable("CSV",   lambda x : x.bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags'), float, help="Jet CSV-IVF v2 discriminator"),
    NTupleVariable("flavour", lambda x : x.hadronFlavour(), int,     mcOnly=False, help="flavour of the ghost hadron clustered inside the jet"),
#    NTupleVariable("motherPdgId", lambda x : x.mother().pdgId() if x.mother() else 0, int,     mcOnly=False, help="parton flavour (physics definition, i.e. including b's from shower)"),
#    NTupleVariable("mcMatchPdgId",  lambda x : getattr(x, 'mcMatchId', -999.), int, mcOnly=False, help="Match to source from hard scatter (pdgId of heaviest particle in chain, 25 for H, 6 for t, 23/24 for W/Z), zero if non-prompt or fake"),
    #NTupleVariable("mcPt",   lambda x : x.mcJet.pt() if getattr(x,"mcJet",None) else 0., mcOnly=True, help="p_{T} of associated gen jet"),
    #NTupleVariable("rawPt",  lambda x : x.pt()*x.rawFactor(), help="p_{T} before JEC"),
    #NTupleVariable("ptUnc",  lambda x : x.pt()*x.jetEnergyCorrUncertainty(), help="JEC uncertainty"),
    NTupleVariable("chf",    lambda x : x.chargedHadronEnergyFraction() , float, mcOnly=False,help="Jet charged hadron energy fraction"),
    NTupleVariable("nhf",    lambda x : x.neutralHadronEnergyFraction() , float, mcOnly=False,help="Jet neutral hadron energy fraction"),
    NTupleVariable("phf",    lambda x : x.neutralEmEnergyFraction() , float, mcOnly=False,help="Jet neutral electromagnetic energy fraction"),
    NTupleVariable("elf",    lambda x : x.chargedEmEnergyFraction() , float, mcOnly=False,help="Jet charged electromagnetic energy fraction"),
    NTupleVariable("muf",    lambda x : x.muonEnergyFraction() , float, mcOnly=False,help="Jet muon energy fraction"),
    NTupleVariable("chm",    lambda x : x.chargedHadronMultiplicity() , int, mcOnly=False,help="Jet charged hadron multiplicity"),
    NTupleVariable("npr",    lambda x : x.chargedMultiplicity()+x.neutralMultiplicity() , int, mcOnly=False,help="Jet constituents multiplicity"),
    #NTupleVariable("looseId",    lambda x : x.jetID("POG_PFID_Loose") , int, mcOnly=False,help="Jet POG Loose id"),
    #NTupleVariable("mediumId",    lambda x : x.jetID("POG_PFID_Medium") , int, mcOnly=False,help="Jet POG Medium id"),
    #NTupleVariable("tightId",    lambda x : x.jetID("POG_PFID_Tight") , int, mcOnly=False,help="Jet POG Tight id"),
])

fatjetType = NTupleObjectType("jet",  baseObjectTypes = [ fourVectorType ], variables = [
    #NTupleVariable("trimmedMass",   lambda x : x.userFloat("ak8PFJetsCHSTrimmedMass") if x.hasUserFloat("ak8PFJetsCHSTrimmedMass") else -1., float, help="Jet trimmed mass"),
    #NTupleVariable("filteredMass",   lambda x : x.userFloat("ak8PFJetsCHSFilteredMass") if x.hasUserFloat("ak8PFJetsCHSFilteredMass") else -1., float, help="Jet filtered mass"),
    NTupleVariable("prunedMass",   lambda x : x.userFloat("ak8PFJetsCHSPrunedMass") if x.hasUserFloat("ak8PFJetsCHSPrunedMass") else -1., float, help="Jet pruned mass"),
    NTupleVariable("softdropMass",   lambda x : x.userFloat("ak8PFJetsCHSSoftDropMass") if x.hasUserFloat("ak8PFJetsCHSSoftDropMass") else -1., float, help="Jet SoftDrop mass"),
    NTupleVariable("prunedMassCorr",   lambda x : x.userFloat("ak8PFJetsCHSPrunedMassCorr") if x.hasUserFloat("ak8PFJetsCHSPrunedMassCorr") else -1., float, help="Jet pruned mass corrected"),
    NTupleVariable("softdropMassCorr",   lambda x : x.userFloat("ak8PFJetsCHSSoftDropMassCorr") if x.hasUserFloat("ak8PFJetsCHSSoftDropMassCorr") else -1., float, help="Jet SoftDrop mass corrected"),
    
    NTupleVariable("dR",   lambda x : getattr(x, "dR_subjets", -1.), float, help="dR between the two subjets"),
    NTupleVariable("dPhi_met",   lambda x : getattr(x, "deltaPhi_met", -9.), float, help="dPhi between jet and met"),
    #NTupleVariable("dPhi_jet1",   lambda x : getattr(x, "deltaPhi_jet1", -9.), float, help="dPhi between jet and leading jet"),
    #NTupleVariable("puId", lambda x : getattr(x, 'puJetIdPassed', -999.), int,     mcOnly=False, help="puId (full MVA, loose WP, 5.3.X training on AK5PFchs: the only thing that is available now)"),
    NTupleVariable("tau21",   lambda x : x.userFloat("NjettinessAK8:tau2")/x.userFloat("NjettinessAK8:tau1") if not x.userFloat("NjettinessAK8:tau1") == 0 else -1., float, help="n-subjettiness 2/1"),
    NTupleVariable("CSV",   lambda x : x.bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags'), float, help="Jet CSV-IVF v2 discriminator"),
    NTupleVariable("CSV1",   lambda x : x.subjets('SoftDrop')[0].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') if len(x.subjets('SoftDrop')) > 0 else -1., float, help="subJet 1 CSV-IVF v2 discriminator"),
    NTupleVariable("CSV2",   lambda x : x.subjets('SoftDrop')[1].bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags') if len(x.subjets('SoftDrop')) > 1 else -1., float, help="subJet 2 CSV-IVF v2 discriminator"),
    #NTupleVariable("nSubJetTags",   lambda x : getattr(x, "nSubJetTags", -999.), int, help="Number of b-tagged subjets"),
    NTupleVariable("flavour", lambda x : x.hadronFlavour(), int,     mcOnly=False, help="flavour of the ghost hadron clustered inside the jet"),
    NTupleVariable("flavour1", lambda x : x.subjets('SoftDrop')[0].hadronFlavour() if len(x.subjets('SoftDrop')) > 0 else -9, int,     mcOnly=False, help="flavour of the ghost hadron clustered inside the subjet 1"),
    NTupleVariable("flavour2", lambda x : x.subjets('SoftDrop')[1].hadronFlavour() if len(x.subjets('SoftDrop')) > 1 else -9, int,     mcOnly=False, help="flavour of the ghost hadron clustered inside the subjet 2"),
#    NTupleVariable("motherPdgId", lambda x : x.mother().pdgId() if x.mother() else 0, int,     mcOnly=False, help="parton flavour (physics definition, i.e. including b's from shower)"),
#    NTupleVariable("mcMatchPdgId",  lambda x : getattr(x, 'mcMatchId', -999.), int, mcOnly=False, help="Match to source from hard scatter (pdgId of heaviest particle in chain, 25 for H, 6 for t, 23/24 for W/Z), zero if non-prompt or fake"),
    #NTupleVariable("mcPt",   lambda x : x.mcJet.pt() if getattr(x,"mcJet",None) else 0., mcOnly=True, help="p_{T} of associated gen jet"),
    #NTupleVariable("rawPt",  lambda x : x.pt()*x.rawFactor(), help="p_{T} before JEC"),
    #NTupleVariable("ptUnc",  lambda x : x.pt()*x.jetEnergyCorrUncertainty(), help="JEC uncertainty"),
    NTupleVariable("chf",    lambda x : x.chargedHadronEnergyFraction() , float, mcOnly=False,help="Jet charged hadron energy fraction"),
    NTupleVariable("nhf",    lambda x : x.neutralHadronEnergyFraction() , float, mcOnly=False,help="Jet neutral hadron energy fraction"),
    NTupleVariable("phf",    lambda x : x.neutralEmEnergyFraction() , float, mcOnly=False,help="Jet neutral electromagnetic energy fraction"),
    NTupleVariable("elf",    lambda x : x.chargedEmEnergyFraction() , float, mcOnly=False,help="Jet charged electromagnetic energy fraction"),
    NTupleVariable("muf",    lambda x : x.muonEnergyFraction() , float, mcOnly=False,help="Jet muon energy fraction"),
    NTupleVariable("chm",    lambda x : x.chargedHadronMultiplicity() , int, mcOnly=False,help="Jet charged hadron multiplicity"),
    NTupleVariable("npr",    lambda x : x.chargedMultiplicity()+x.neutralMultiplicity() , int, mcOnly=False,help="Jet constituents multiplicity"),
    #NTupleVariable("looseId",    lambda x : x.jetID("POG_PFID_Loose") , int, mcOnly=False,help="Jet POG Loose id"),
    #NTupleVariable("mediumId",    lambda x : x.jetID("POG_PFID_Medium") , int, mcOnly=False,help="Jet POG Medium id"),
    #NTupleVariable("tightId",    lambda x : x.jetID("POG_PFID_Tight") , int, mcOnly=False,help="Jet POG Tight id"),
])

#subjetType = NTupleObjectType("subjet",  baseObjectTypes = [ fourVectorType ], variables = [
#    NTupleVariable("CSV",   lambda x : x.bDiscriminator('pfCombinedInclusiveSecondaryVertexV2BJetTags'), float, help="Jet CSV-IVF v2 discriminator"),
#    NTupleVariable("flavour", lambda x : x.hadronFlavour(), int,     mcOnly=False, help="flavour of the ghost hadron clustered inside the jet"),
#])

tauType = NTupleObjectType("tau",  baseObjectTypes = [ particleType ], variables = [
    NTupleVariable("charge",   lambda x : x.charge(), int),
    #NTupleVariable("decayMode",   lambda x : x.decayMode(), int),
    #NTupleVariable("idDecayMode",   lambda x : x.idDecayMode, int),
    #NTupleVariable("idDecayModeNewDMs",   lambda x : x.idDecayModeNewDMs, int),
    #NTupleVariable("dxy",   lambda x : x.dxy(), help="d_{xy} of lead track with respect to PV, in cm (with sign)"),
    #NTupleVariable("dz",    lambda x : x.dz() , help="d_{z} of lead track with respect to PV, in cm (with sign)"),
    #NTupleVariable("idMVA", lambda x : x.idMVA, int, help="-99.,2,3,4,5,6 if the tau passes the very loose to very very tight WP of the MVA3oldDMwLT discriminator"),
    #NTupleVariable("idMVANewDM", lambda x : x.idMVANewDM, int, help="-99.,2,3,4,5,6 if the tau passes the very loose to very very tight WP of the MVA3newDMwLT discriminator"),
    #NTupleVariable("idCI3hit", lambda x : x.idCI3hit, int, help="-99.,2,3 if the tau passes the loose, medium, tight WP of the By<X>CombinedIsolationDBSumPtCorr3Hits discriminator"),
    #NTupleVariable("idAntiMu", lambda x : x.idAntiMu, int, help="-99.,2 if the tau passes the loose/tight WP of the againstMuon<X>3 discriminator"),
    #NTupleVariable("idAntiE", lambda x : x.idAntiE, int, help="-99.,2,3,4,5 if the tau passes the v loose, loose, medium, tight, v tight WP of the againstElectron<X>MVA5 discriminator"),
    #NTupleVariable("isoCI3hit",  lambda x : x.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits"), help="byCombinedIsolationDeltaBetaCorrRaw3Hits raw output discriminator"),
    # MC-match info
    #NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (pdgId of heaviest particle in chain, 25 for H, 6 for t, 23/24 for W/Z), zero if non-prompt or fake"),
])

photonType = NTupleObjectType("photon", baseObjectTypes = [ particleType ], variables = [
    #NTupleVariable("idCutBased", lambda x : x.idCutBased, int, help="-99.,2,3 if the photon passes the loose, medium, tight WP of PhotonCutBasedID"),
    NTupleVariable("looseId",    lambda x : x.photonID("PhotonCutBasedIDLoose") , int, mcOnly=False,help="Photon POG Cut-based Loose id"),
#    NTupleVariable("mediumId",    lambda x : x.photonID("PhotonCutBasedIDMedium") , int, mcOnly=False,help="Photon POG Cut-based Medium id"),
    NTupleVariable("tightId",    lambda x : x.photonID("PhotonCutBasedIDTight") , int, mcOnly=False,help="Photon POG Cut-based Tight id"),
    #NTupleVariable("hOverE",  lambda x : x.hOVERe(), float, help="hoverE for photons"),
    #NTupleVariable("r9",  lambda x : x.full5x5_r9(), float, help="r9 for photons"),
    #NTupleVariable("sigmaIetaIeta",  lambda x : x.full5x5_sigmaIetaIeta(), float, help="sigmaIetaIeta for photons"),
    #NTupleVariable("chHadIso",  lambda x : x.chargedHadronIso(), float, help="chargedHadronIsolation for photons"),
    #NTupleVariable("neuHadIso",  lambda x : x.neutralHadronIso(), float, help="neutralHadronIsolation for photons"),
    #NTupleVariable("phIso",  lambda x : x.photonIso(), float, help="gammaIsolation for photons"),
    #NTupleVariable("chHadIso",  lambda x : x.recoChargedHadronIso(), float, help="chargedHadronIsolation for photons"),
    #NTupleVariable("neuHadIso",  lambda x : x.recoNeutralHadronIso(), float, help="neutralHadronIsolation for photons"),
    #NTupleVariable("phIso",  lambda x : x.recoPhotonIso(), float, help="gammaIsolation for photons"),
    #NTupleVariable("mcMatchId",  lambda x : x.mcMatchId, int, mcOnly=True, help="Match to source from hard scatter (pdgId of heaviest particle in chain, 25 for H, 6 for t, 23/24 for W/Z), zero if non-prompt or fake"),
])

metType = NTupleObjectType("met",  baseObjectTypes = [ twoVectorType ], variables = [])

metFullType = NTupleObjectType("met",  baseObjectTypes = [ twoVectorType ], variables = [
#    NTupleVariable("calo_pt",    lambda x : x.caloMETPt(), float, mcOnly=False, help="CaloMET pt"), #does not work if not CaloMET
#    NTupleVariable("calo_phi",    lambda x : x.caloMETPhi(), float, mcOnly=False, help="CaloMET phi"), #does not work if not CaloMET
#    NTupleVariable("sign",    lambda x : x.metSignificance() if x.isCaloMET() else -1., float, mcOnly=False, help="missing energy significance"), #does not work if not CaloMET
#    NTupleVariable("uncorrected",    lambda x : x.uncorrectedPt(), float, mcOnly=False, help="missing energy significance"), #uncorrected met
#    NTupleVariable("phf",     lambda x : x.NeutralEMFraction(), float, mcOnly=False, help="neutral electromagnetic energy fraction"),
#    NTupleVariable("nhf",     lambda x : x.NeutralHadEtFraction(), float, mcOnly=False, help="neutral hadron energy fraction"),
#    NTupleVariable("elf",     lambda x : x.ChargedEMEtFraction(), float, mcOnly=False, help="charged electromagnetic energy fraction"),
#    NTupleVariable("chf",     lambda x : x.ChargedHadEtFraction(), float, mcOnly=False, help="charged hadron energy fraction"),
#    NTupleVariable("muf",     lambda x : x.MuonEtFraction(), float, mcOnly=False, help="muon energy fraction"),
])
