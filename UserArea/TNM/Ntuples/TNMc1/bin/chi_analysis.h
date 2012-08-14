#ifndef ANALYZER_H
#define ANALYZER_H
//-----------------------------------------------------------------------------
// File:        analyzer.h
// Description: Analyzer header for ntuples created by TheNtupleMaker
// Created:     Thu May 31 21:49:25 2012 by mkanalyzer.py
// Author:      Andreas Hinzmann
//-----------------------------------------------------------------------------
// -- System

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#ifdef PROJECT_NAME
#include "PhysicsTools/TheNtupleMaker/interface/treestream.h"
#include "PhysicsTools/TheNtupleMaker/interface/pdg.h"
#else
#include "treestream.h"
#include "pdg.h"
#endif

// -- Root

#include "TROOT.h"
#include "TApplication.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"
//-----------------------------------------------------------------------------
// -- Declare variables to be read
//-----------------------------------------------------------------------------
std::vector<float>	electronhelper_caloIso(10,0);
std::vector<int>	electronhelper_charge(10,0);
std::vector<float>	electronhelper_deltaEtaSuperClusterTrackAtVtx(10,0);
std::vector<float>	electronhelper_deltaPhiSuperClusterTrackAtVtx(10,0);
std::vector<double>	electronhelper_dxywrtPV(10,0);
std::vector<double>	electronhelper_dzwrtPV(10,0);
std::vector<float>	electronhelper_eSuperClusterOverP(10,0);
std::vector<float>	electronhelper_ecalIso(10,0);
std::vector<double>	electronhelper_energy(10,0);
std::vector<double>	electronhelper_et(10,0);
std::vector<double>	electronhelper_eta(10,0);
std::vector<unsigned short>	electronhelper_gsfTrack_trackerExpectedHitsInner_numberOfHits(10,0);
std::vector<float>	electronhelper_hadronicOverEm(10,0);
std::vector<float>	electronhelper_hcalIso(10,0);
std::vector<int>	electronhelper_isEB(10,0);
std::vector<int>	electronhelper_isEE(10,0);
std::vector<double>	electronhelper_phi(10,0);
std::vector<double>	electronhelper_pt(10,0);
std::vector<float>	electronhelper_scSigmaIEtaIEta(10,0);
std::vector<float>	electronhelper_simpleEleId80relIso(10,0);
std::vector<float>	electronhelper_simpleEleId95relIso(10,0);
std::vector<double>	electronhelper_superCluster_energy(10,0);
std::vector<float>	electronhelper_trackIso(10,0);
int	eventhelper_bunchCrossing;
int	eventhelper_event;
int	eventhelper_isRealData;
int	eventhelper_luminosityBlock;
int	eventhelper_orbitNumber;
int	eventhelper_run;
double	eventhelperextra_dijetCHS_invmass;
double	eventhelperextra_dijetCHSpruned_invmass;
double	eventhelperextra_dijet_invmass;
double	eventhelperextra_wj1CHS_energy;
double	eventhelperextra_wj1CHS_eta;
double	eventhelperextra_wj1CHS_mass;
double	eventhelperextra_wj1CHS_nconst;
double	eventhelperextra_wj1CHS_phi;
double	eventhelperextra_wj1CHS_pt;
double	eventhelperextra_wj1_energy;
double	eventhelperextra_wj1_eta;
double	eventhelperextra_wj1_mass;
double	eventhelperextra_wj1_nconst;
double	eventhelperextra_wj1_phi;
double	eventhelperextra_wj1_pt;
double	eventhelperextra_wj1wj2CHS_invmass;
double	eventhelperextra_wj1wj2CHS_nconst;
double	eventhelperextra_wj1wj2_invmass;
double	eventhelperextra_wj1wj2_nconst;
double	eventhelperextra_wj2CHS_energy;
double	eventhelperextra_wj2CHS_eta;
double	eventhelperextra_wj2CHS_mass;
double	eventhelperextra_wj2CHS_nconst;
double	eventhelperextra_wj2CHS_phi;
double	eventhelperextra_wj2CHS_pt;
double	eventhelperextra_wj2_energy;
double	eventhelperextra_wj2_eta;
double	eventhelperextra_wj2_mass;
double	eventhelperextra_wj2_nconst;
double	eventhelperextra_wj2_phi;
double	eventhelperextra_wj2_pt;
double	geneventinfoproduct_alphaQCD;
double	geneventinfoproduct_alphaQED;
int	geneventinfoproduct_hasBinningValues;
int	geneventinfoproduct_hasPDF;
double	geneventinfoproduct_qScale;
unsigned int	geneventinfoproduct_signalProcessID;
double	geneventinfoproduct_weight;
std::vector<int>	genparticlehelper_charge(200,0);
std::vector<double>	genparticlehelper_eta(200,0);
std::vector<int>	genparticlehelper_firstDaughter(200,0);
std::vector<int>	genparticlehelper_firstMother(200,0);
std::vector<int>	genparticlehelper_lastDaughter(200,0);
std::vector<int>	genparticlehelper_lastMother(200,0);
std::vector<double>	genparticlehelper_mass(200,0);
std::vector<int>	genparticlehelper_pdgId(200,0);
std::vector<double>	genparticlehelper_phi(200,0);
std::vector<double>	genparticlehelper_pt(200,0);
std::vector<int>	genparticlehelper_status(200,0);
double	genruninfoproduct_crossSection;
double	genruninfoproduct_filterEfficiency;
std::vector<float>	jethelper2_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelper2_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelper2_chargedMultiplicity(10,0);
std::vector<float>	jethelper2_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelper2_combinedSecondaryVertexMVABJetTags(10,0);
std::vector<double>	jethelper2_energy(10,0);
std::vector<double>	jethelper2_et(10,0);
std::vector<double>	jethelper2_eta(10,0);
std::vector<double>	jethelper2_rapidity(10,0);
std::vector<double>	jethelper2_genJet_energy(10,0);
std::vector<double>	jethelper2_genJet_eta(10,0);
std::vector<double>	jethelper2_genJet_phi(10,0);
std::vector<double>	jethelper2_genJet_pt(10,0);
std::vector<int>	jethelper2_genParton_pdgId(10,0);
std::vector<float>	jethelper2_jetArea(10,0);
std::vector<float>	jethelper2_jetBProbabilityBJetTags(10,0);
std::vector<float>	jethelper2_jetProbabilityBJetTags(10,0);
std::vector<double>	jethelper2_mass(10,0);
std::vector<float>	jethelper2_muonEnergyFraction(10,0);
std::vector<int>	jethelper2_nConstituents(10,0);
std::vector<float>	jethelper2_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelper2_neutralHadronEnergyFraction(10,0);
std::vector<int>	jethelper2_partonFlavour(10,0);
std::vector<double>	jethelper2_phi(10,0);
std::vector<float>	jethelper2_photonEnergyFraction(10,0);
std::vector<double>	jethelper2_pt(10,0);
std::vector<float>	jethelper2_simpleSecondaryVertexHighEffBJetTags(10,0);
std::vector<float>	jethelper2_simpleSecondaryVertexHighPurBJetTags(10,0);
std::vector<float>	jethelper2_trackCountingHighEffBJetTags(10,0);
std::vector<float>	jethelper2_trackCountingHighPurBJetTags(10,0);
std::vector<double>	jethelper2_uncor_energy(10,0);
std::vector<double>	jethelper2_uncor_et(10,0);
std::vector<double>	jethelper2_uncor_pt(10,0);
std::vector<float>	jethelper3_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelper3_combinedSecondaryVertexMVABJetTags(10,0);
std::vector<double>	jethelper3_energy(10,0);
std::vector<double>	jethelper3_et(10,0);
std::vector<double>	jethelper3_eta(10,0);
std::vector<double>	jethelper3_rapidity(10,0);
std::vector<double>	jethelper3_genJet_energy(10,0);
std::vector<double>	jethelper3_genJet_eta(10,0);
std::vector<double>	jethelper3_genJet_phi(10,0);
std::vector<double>	jethelper3_genJet_pt(10,0);
std::vector<int>	jethelper3_genParton_pdgId(10,0);
std::vector<float>	jethelper3_jetArea(10,0);
std::vector<float>	jethelper3_jetBProbabilityBJetTags(10,0);
std::vector<float>	jethelper3_jetProbabilityBJetTags(10,0);
std::vector<double>	jethelper3_mass(10,0);
std::vector<int>	jethelper3_nConstituents(10,0);
std::vector<int>	jethelper3_partonFlavour(10,0);
std::vector<double>	jethelper3_phi(10,0);
std::vector<double>	jethelper3_pt(10,0);
std::vector<float>	jethelper3_simpleSecondaryVertexHighEffBJetTags(10,0);
std::vector<float>	jethelper3_simpleSecondaryVertexHighPurBJetTags(10,0);
std::vector<float>	jethelper3_trackCountingHighEffBJetTags(10,0);
std::vector<float>	jethelper3_trackCountingHighPurBJetTags(10,0);
std::vector<double>	jethelper3_uncor_energy(10,0);
std::vector<double>	jethelper3_uncor_et(10,0);
std::vector<double>	jethelper3_uncor_pt(10,0);
std::vector<float>	jethelper_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelper_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelper_chargedMultiplicity(10,0);
std::vector<float>	jethelper_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelper_combinedSecondaryVertexMVABJetTags(10,0);
std::vector<float>	jethelper_daughter_0_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelper_daughter_0_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelper_daughter_0_chargedMultiplicity(10,0);
std::vector<double>	jethelper_daughter_0_energy(10,0);
std::vector<double>	jethelper_daughter_0_eta(10,0);
std::vector<double>	jethelper_daughter_0_mass(10,0);
std::vector<float>	jethelper_daughter_0_muonEnergyFraction(10,0);
std::vector<int>	jethelper_daughter_0_nConstituents(10,0);
std::vector<float>	jethelper_daughter_0_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelper_daughter_0_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelper_daughter_0_phi(10,0);
std::vector<float>	jethelper_daughter_0_photonEnergyFraction(10,0);
std::vector<double>	jethelper_daughter_0_pt(10,0);
std::vector<float>	jethelper_daughter_1_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelper_daughter_1_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelper_daughter_1_chargedMultiplicity(10,0);
std::vector<double>	jethelper_daughter_1_energy(10,0);
std::vector<double>	jethelper_daughter_1_eta(10,0);
std::vector<double>	jethelper_daughter_1_mass(10,0);
std::vector<float>	jethelper_daughter_1_muonEnergyFraction(10,0);
std::vector<int>	jethelper_daughter_1_nConstituents(10,0);
std::vector<float>	jethelper_daughter_1_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelper_daughter_1_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelper_daughter_1_phi(10,0);
std::vector<float>	jethelper_daughter_1_photonEnergyFraction(10,0);
std::vector<double>	jethelper_daughter_1_pt(10,0);
std::vector<double>	jethelper_energy(10,0);
std::vector<double>	jethelper_et(10,0);
std::vector<double>	jethelper_eta(10,0);
std::vector<double>	jethelper_rapidity(10,0);
std::vector<double>	jethelper_genJet_energy(10,0);
std::vector<double>	jethelper_genJet_eta(10,0);
std::vector<double>	jethelper_genJet_phi(10,0);
std::vector<double>	jethelper_genJet_pt(10,0);
std::vector<int>	jethelper_genParton_pdgId(10,0);
std::vector<float>	jethelper_jetArea(10,0);
std::vector<float>	jethelper_jetBProbabilityBJetTags(10,0);
std::vector<float>	jethelper_jetProbabilityBJetTags(10,0);
std::vector<double>	jethelper_mass(10,0);
std::vector<float>	jethelper_muonEnergyFraction(10,0);
std::vector<int>	jethelper_nConstituents(10,0);
std::vector<float>	jethelper_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelper_neutralHadronEnergyFraction(10,0);
std::vector<size_t>	jethelper_numberOfDaughters(10,0);
std::vector<int>	jethelper_partonFlavour(10,0);
std::vector<double>	jethelper_phi(10,0);
std::vector<float>	jethelper_photonEnergyFraction(10,0);
std::vector<double>	jethelper_pt(10,0);
std::vector<float>	jethelper_simpleSecondaryVertexHighEffBJetTags(10,0);
std::vector<float>	jethelper_simpleSecondaryVertexHighPurBJetTags(10,0);
std::vector<float>	jethelper_trackCountingHighEffBJetTags(10,0);
std::vector<float>	jethelper_trackCountingHighPurBJetTags(10,0);
std::vector<double>	jethelper_uncor_energy(10,0);
std::vector<double>	jethelper_uncor_et(10,0);
std::vector<double>	jethelper_uncor_pt(10,0);
double	met2_energy;
double	met2_et;
double	met2_mEtSig;
double	met2_phi;
double	met2_pt;
double	met2_significance;
double	met2_sumEt;
double	met_energy;
double	met_et;
double	met_mEtSig;
double	met_phi;
double	met_pt;
double	met_significance;
double	met_sumEt;
std::vector<float>	muonhelper_TMOneStationTight(10,0);
std::vector<int>	muonhelper_charge(10,0);
std::vector<double>	muonhelper_dB(10,0);
std::vector<double>	muonhelper_dxywrtPV(10,0);
std::vector<double>	muonhelper_dzwrtPV(10,0);
std::vector<double>	muonhelper_energy(10,0);
std::vector<double>	muonhelper_et(10,0);
std::vector<double>	muonhelper_eta(10,0);
std::vector<unsigned short>	muonhelper_globalTrack_hitPattern_numberOfValidMuonHits(10,0);
std::vector<double>	muonhelper_globalTrack_normalizedChi2(10,0);
std::vector<unsigned short>	muonhelper_innerTrack_hitPattern_numberOfValidPixelHits(10,0);
std::vector<unsigned short>	muonhelper_innerTrack_hitPattern_pixelLayersWithMeasurement(10,0);
std::vector<double>	muonhelper_innerTrack_normalizedChi2(10,0);
std::vector<int>	muonhelper_isGlobalMuon(10,0);
std::vector<int>	muonhelper_isPFMuon(10,0);
std::vector<int>	muonhelper_isTrackerMuon(10,0);
std::vector<int>	muonhelper_numberOfMatchedStations(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumChargedHadronPt(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumChargedParticlePt(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumNeutralHadronEt(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumNeutralHadronEtHighThreshold(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumPUPt(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumPhotonEt(10,0);
std::vector<float>	muonhelper_pfIsolationR04_sumPhotonEtHighThreshold(10,0);
std::vector<double>	muonhelper_phi(10,0);
std::vector<double>	muonhelper_pt(10,0);
std::vector<unsigned short>	muonhelper_track_hitPattern_trackerLayersWithMeasurement(10,0);
int	nelectronhelper;
int	ngenparticlehelper;
int	njethelper;
int	njethelper2;
int	njethelper3;
int	nmuonhelper;
int	ntau;
int	nvertex;
int	pileupsummaryinfo_getBunchCrossing;
int	pileupsummaryinfo_getPU_NumInteractions;
float	pileupsummaryinfo_getTrueNumInteractions;
double	sdouble_kt6PFJets_rho_value;
std::vector<float>	tau_byLooseCombinedIsolationDeltaBetaCorr(10,0);
std::vector<float>	tau_byMediumCombinedIsolationDeltaBetaCorr(10,0);
std::vector<float>	tau_caloIso(10,0);
std::vector<float>	tau_ecalIso(10,0);
std::vector<double>	tau_energy(10,0);
std::vector<double>	tau_et(10,0);
std::vector<double>	tau_eta(10,0);
std::vector<float>	tau_hcalIso(10,0);
std::vector<double>	tau_phi(10,0);
std::vector<double>	tau_pt(10,0);
std::vector<float>	tau_trackIso(10,0);
int	triggerresultshelper_CSCTightHaloFilterPath;
int	triggerresultshelper_EcalDeadCellBoundaryEnergyFilterPath;
int	triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath;
int	triggerresultshelper_HBHENoiseFilterPath;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9;
int	triggerresultshelper_HLT_HT200_v1;
int	triggerresultshelper_HLT_HT200_v2;
int	triggerresultshelper_HLT_HT200_v3;
int	triggerresultshelper_HLT_HT250_v1;
int	triggerresultshelper_HLT_HT250_v2;
int	triggerresultshelper_HLT_HT250_v3;
int	triggerresultshelper_HLT_HT300_v1;
int	triggerresultshelper_HLT_HT300_v2;
int	triggerresultshelper_HLT_HT300_v3;
int	triggerresultshelper_HLT_HT350_v1;
int	triggerresultshelper_HLT_HT350_v2;
int	triggerresultshelper_HLT_HT350_v3;
int	triggerresultshelper_HLT_HT400_v1;
int	triggerresultshelper_HLT_HT400_v2;
int	triggerresultshelper_HLT_HT400_v3;
int	triggerresultshelper_HLT_HT450_v1;
int	triggerresultshelper_HLT_HT450_v2;
int	triggerresultshelper_HLT_HT450_v3;
int	triggerresultshelper_HLT_HT500_v1;
int	triggerresultshelper_HLT_HT500_v2;
int	triggerresultshelper_HLT_HT500_v3;
int	triggerresultshelper_HLT_HT550_v1;
int	triggerresultshelper_HLT_HT550_v2;
int	triggerresultshelper_HLT_HT550_v3;
int	triggerresultshelper_HLT_HT650_v1;
int	triggerresultshelper_HLT_HT650_v2;
int	triggerresultshelper_HLT_HT650_v3;
int	triggerresultshelper_HLT_HT650_v4;
int	triggerresultshelper_HLT_HT650_v5;
int	triggerresultshelper_HLT_HT650_v6;
int	triggerresultshelper_HLT_HT650_v7;
int	triggerresultshelper_HLT_HT650_v8;
int	triggerresultshelper_HLT_HT650_v9;
int	triggerresultshelper_HLT_HT750_v1;
int	triggerresultshelper_HLT_HT750_v2;
int	triggerresultshelper_HLT_HT750_v3;
int	triggerresultshelper_HLT_HT750_v4;
int	triggerresultshelper_HLT_HT750_v5;
int	triggerresultshelper_HLT_HT750_v6;
int	triggerresultshelper_HLT_HT750_v7;
int	triggerresultshelper_HLT_HT750_v8;
int	triggerresultshelper_HLT_HT750_v9;
int	triggerresultshelper_HLT_PFHT350_v1;
int	triggerresultshelper_HLT_PFHT350_v2;
int	triggerresultshelper_HLT_PFHT350_v3;
int	triggerresultshelper_HLT_PFHT350_v4;
int	triggerresultshelper_HLT_PFHT350_v5;
int	triggerresultshelper_HLT_PFHT350_v6;
int	triggerresultshelper_HLT_PFHT650_v1;
int	triggerresultshelper_HLT_PFHT650_v2;
int	triggerresultshelper_HLT_PFHT650_v3;
int	triggerresultshelper_HLT_PFHT650_v4;
int	triggerresultshelper_HLT_PFHT650_v5;
int	triggerresultshelper_HLT_PFHT650_v6;
int	triggerresultshelper_HLT_PFHT650_v7;
int	triggerresultshelper_HLT_PFHT650_v8;
int	triggerresultshelper_HLT_PFHT650_v9;
int	triggerresultshelper_HLT_PFHT700_v1;
int	triggerresultshelper_HLT_PFHT700_v2;
int	triggerresultshelper_HLT_PFHT700_v3;
int	triggerresultshelper_HLT_PFHT700_v4;
int	triggerresultshelper_HLT_PFHT700_v5;
int	triggerresultshelper_HLT_PFHT700_v6;
int	triggerresultshelper_HLT_PFHT700_v7;
int	triggerresultshelper_HLT_PFHT700_v8;
int	triggerresultshelper_HLT_PFHT700_v9;
int	triggerresultshelper_HLT_PFHT750_v1;
int	triggerresultshelper_HLT_PFHT750_v2;
int	triggerresultshelper_HLT_PFHT750_v3;
int	triggerresultshelper_HLT_PFHT750_v4;
int	triggerresultshelper_HLT_PFHT750_v5;
int	triggerresultshelper_HLT_PFHT750_v6;
int	triggerresultshelper_HLT_PFHT750_v7;
int	triggerresultshelper_HLT_PFHT750_v8;
int	triggerresultshelper_HLT_PFHT750_v9;
int	triggerresultshelper_HLT_PFJet140_v1;
int	triggerresultshelper_HLT_PFJet140_v2;
int	triggerresultshelper_HLT_PFJet140_v3;
int	triggerresultshelper_HLT_PFJet200_v1;
int	triggerresultshelper_HLT_PFJet200_v2;
int	triggerresultshelper_HLT_PFJet200_v3;
int	triggerresultshelper_HLT_PFJet260_v1;
int	triggerresultshelper_HLT_PFJet260_v2;
int	triggerresultshelper_HLT_PFJet260_v3;
int	triggerresultshelper_HLT_PFJet320_v1;
int	triggerresultshelper_HLT_PFJet320_v2;
int	triggerresultshelper_HLT_PFJet320_v3;
int	triggerresultshelper_HLT_PFJet320_v4;
int	triggerresultshelper_HLT_PFJet320_v5;
int	triggerresultshelper_HLT_PFJet400_v1;
int	triggerresultshelper_HLT_PFJet400_v2;
int	triggerresultshelper_HLT_PFJet400_v3;
int	triggerresultshelper_HLT_PFJet400_v4;
int	triggerresultshelper_HLT_PFJet400_v5;
int	triggerresultshelper_HLT_PFJet40_v1;
int	triggerresultshelper_HLT_PFJet40_v2;
int	triggerresultshelper_HLT_PFJet40_v3;
int	triggerresultshelper_HLT_PFJet80_v1;
int	triggerresultshelper_HLT_PFJet80_v2;
int	triggerresultshelper_HLT_PFJet80_v3;
int	triggerresultshelper_greedyMuonPFCandidateFilterPath;
int	triggerresultshelper_hcalLaserEventFilterPath;
int	triggerresultshelper_hcalLaserFilterFromAODPath;
int	triggerresultshelper_inconsistentMuonPFCandidateFilterPath;
int	triggerresultshelper_noscrapingFilterPath;
int	triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1;
int	triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2;
int	triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3;
int	triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4;
int	triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5;
int	triggerresultshelper_prescaleHLT_HT200_v1;
int	triggerresultshelper_prescaleHLT_HT200_v2;
int	triggerresultshelper_prescaleHLT_HT200_v3;
int	triggerresultshelper_prescaleHLT_HT250_v1;
int	triggerresultshelper_prescaleHLT_HT250_v2;
int	triggerresultshelper_prescaleHLT_HT250_v3;
int	triggerresultshelper_prescaleHLT_HT300_v1;
int	triggerresultshelper_prescaleHLT_HT300_v2;
int	triggerresultshelper_prescaleHLT_HT300_v3;
int	triggerresultshelper_prescaleHLT_HT350_v1;
int	triggerresultshelper_prescaleHLT_HT350_v2;
int	triggerresultshelper_prescaleHLT_HT350_v3;
int	triggerresultshelper_prescaleHLT_HT400_v1;
int	triggerresultshelper_prescaleHLT_HT400_v2;
int	triggerresultshelper_prescaleHLT_HT400_v3;
int	triggerresultshelper_prescaleHLT_HT450_v1;
int	triggerresultshelper_prescaleHLT_HT450_v2;
int	triggerresultshelper_prescaleHLT_HT450_v3;
int	triggerresultshelper_prescaleHLT_HT500_v1;
int	triggerresultshelper_prescaleHLT_HT500_v2;
int	triggerresultshelper_prescaleHLT_HT500_v3;
int	triggerresultshelper_prescaleHLT_HT550_v1;
int	triggerresultshelper_prescaleHLT_HT550_v2;
int	triggerresultshelper_prescaleHLT_HT550_v3;
int	triggerresultshelper_prescaleHLT_HT650_v1;
int	triggerresultshelper_prescaleHLT_HT650_v2;
int	triggerresultshelper_prescaleHLT_HT650_v3;
int	triggerresultshelper_prescaleHLT_HT750_v1;
int	triggerresultshelper_prescaleHLT_HT750_v2;
int	triggerresultshelper_prescaleHLT_HT750_v3;
int	triggerresultshelper_prescaleHLT_PFHT350_v1;
int	triggerresultshelper_prescaleHLT_PFHT350_v2;
int	triggerresultshelper_prescaleHLT_PFHT350_v3;
int	triggerresultshelper_prescaleHLT_PFHT350_v4;
int	triggerresultshelper_prescaleHLT_PFHT350_v5;
int	triggerresultshelper_prescaleHLT_PFHT350_v6;
int	triggerresultshelper_prescaleHLT_PFHT650_v1;
int	triggerresultshelper_prescaleHLT_PFHT650_v2;
int	triggerresultshelper_prescaleHLT_PFHT650_v3;
int	triggerresultshelper_prescaleHLT_PFHT650_v4;
int	triggerresultshelper_prescaleHLT_PFHT650_v5;
int	triggerresultshelper_prescaleHLT_PFHT650_v6;
int	triggerresultshelper_prescaleHLT_PFHT650_v7;
int	triggerresultshelper_prescaleHLT_PFHT650_v8;
int	triggerresultshelper_prescaleHLT_PFHT700_v1;
int	triggerresultshelper_prescaleHLT_PFHT700_v2;
int	triggerresultshelper_prescaleHLT_PFHT700_v3;
int	triggerresultshelper_prescaleHLT_PFHT700_v4;
int	triggerresultshelper_prescaleHLT_PFHT700_v5;
int	triggerresultshelper_prescaleHLT_PFHT700_v6;
int	triggerresultshelper_prescaleHLT_PFHT750_v1;
int	triggerresultshelper_prescaleHLT_PFHT750_v2;
int	triggerresultshelper_prescaleHLT_PFHT750_v3;
int	triggerresultshelper_prescaleHLT_PFHT750_v4;
int	triggerresultshelper_prescaleHLT_PFHT750_v5;
int	triggerresultshelper_prescaleHLT_PFHT750_v6;
int	triggerresultshelper_prescaleHLT_PFJet140_v1;
int	triggerresultshelper_prescaleHLT_PFJet140_v2;
int	triggerresultshelper_prescaleHLT_PFJet140_v3;
int	triggerresultshelper_prescaleHLT_PFJet200_v1;
int	triggerresultshelper_prescaleHLT_PFJet200_v2;
int	triggerresultshelper_prescaleHLT_PFJet200_v3;
int	triggerresultshelper_prescaleHLT_PFJet260_v1;
int	triggerresultshelper_prescaleHLT_PFJet260_v2;
int	triggerresultshelper_prescaleHLT_PFJet260_v3;
int	triggerresultshelper_prescaleHLT_PFJet320_v1;
int	triggerresultshelper_prescaleHLT_PFJet320_v2;
int	triggerresultshelper_prescaleHLT_PFJet320_v3;
int	triggerresultshelper_prescaleHLT_PFJet320_v4;
int	triggerresultshelper_prescaleHLT_PFJet320_v5;
int	triggerresultshelper_prescaleHLT_PFJet400_v1;
int	triggerresultshelper_prescaleHLT_PFJet400_v2;
int	triggerresultshelper_prescaleHLT_PFJet400_v3;
int	triggerresultshelper_prescaleHLT_PFJet400_v4;
int	triggerresultshelper_prescaleHLT_PFJet400_v5;
int	triggerresultshelper_prescaleHLT_PFJet40_v1;
int	triggerresultshelper_prescaleHLT_PFJet40_v2;
int	triggerresultshelper_prescaleHLT_PFJet40_v3;
int	triggerresultshelper_prescaleHLT_PFJet80_v1;
int	triggerresultshelper_prescaleHLT_PFJet80_v2;
int	triggerresultshelper_prescaleHLT_PFJet80_v3;
int	triggerresultshelper_primaryVertexFilterPath;
int	triggerresultshelper_simpleDRfilterPath;
int	triggerresultshelper_totalKinematicsFilterPath;
int	triggerresultshelper_trackingFailureFilterPath;
std::vector<double>	vertex_chi2(200,0);
std::vector<int>	vertex_isFake(200,0);
std::vector<double>	vertex_ndof(200,0);
std::vector<double>	vertex_position_Rho(200,0);
std::vector<double>	vertex_x(200,0);
std::vector<double>	vertex_xError(200,0);
std::vector<double>	vertex_y(200,0);
std::vector<double>	vertex_yError(200,0);
std::vector<double>	vertex_z(200,0);
std::vector<double>	vertex_zError(200,0);

//-----------------------------------------------------------------------------
// --- Structs can be filled by calling fillObjects()
// --- after the call to stream.read(...)
//-----------------------------------------------------------------------------
struct electronhelper_s
{
  bool	selected;
  double	energy;
  double	et;
  double	pt;
  double	phi;
  double	eta;
  int	charge;
  float	eSuperClusterOverP;
  float	deltaEtaSuperClusterTrackAtVtx;
  float	deltaPhiSuperClusterTrackAtVtx;
  int	isEB;
  int	isEE;
  float	scSigmaIEtaIEta;
  float	hadronicOverEm;
  double	superCluster_energy;
  unsigned short	gsfTrack_trackerExpectedHitsInner_numberOfHits;
  float	simpleEleId80relIso;
  float	simpleEleId95relIso;
  float	trackIso;
  float	ecalIso;
  float	hcalIso;
  float	caloIso;
  double	dxywrtPV;
  double	dzwrtPV;
};
std::vector<electronhelper_s> electronhelper(10);

std::ostream& operator<<(std::ostream& os, const electronhelper_s& o)
{
  char r[1024];
  os << "electronhelper" << std::endl;
  sprintf(r, "  %-32s: %f\n", "energy", (double)o.energy); os << r;
  sprintf(r, "  %-32s: %f\n", "et", (double)o.et); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "charge", (double)o.charge); os << r;
  sprintf(r, "  %-32s: %f\n", "eSuperClusterOverP", (double)o.eSuperClusterOverP); os << r;
  sprintf(r, "  %-32s: %f\n", "deltaEtaSuperClusterTrackAtVtx", (double)o.deltaEtaSuperClusterTrackAtVtx); os << r;
  sprintf(r, "  %-32s: %f\n", "deltaPhiSuperClusterTrackAtVtx", (double)o.deltaPhiSuperClusterTrackAtVtx); os << r;
  sprintf(r, "  %-32s: %f\n", "isEB", (double)o.isEB); os << r;
  sprintf(r, "  %-32s: %f\n", "isEE", (double)o.isEE); os << r;
  sprintf(r, "  %-32s: %f\n", "scSigmaIEtaIEta", (double)o.scSigmaIEtaIEta); os << r;
  sprintf(r, "  %-32s: %f\n", "hadronicOverEm", (double)o.hadronicOverEm); os << r;
  sprintf(r, "  %-32s: %f\n", "superCluster_energy", (double)o.superCluster_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "gsfTrack_trackerExpectedHitsInner_numberOfHits", (double)o.gsfTrack_trackerExpectedHitsInner_numberOfHits); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleEleId80relIso", (double)o.simpleEleId80relIso); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleEleId95relIso", (double)o.simpleEleId95relIso); os << r;
  sprintf(r, "  %-32s: %f\n", "trackIso", (double)o.trackIso); os << r;
  sprintf(r, "  %-32s: %f\n", "ecalIso", (double)o.ecalIso); os << r;
  sprintf(r, "  %-32s: %f\n", "hcalIso", (double)o.hcalIso); os << r;
  sprintf(r, "  %-32s: %f\n", "caloIso", (double)o.caloIso); os << r;
  sprintf(r, "  %-32s: %f\n", "dxywrtPV", (double)o.dxywrtPV); os << r;
  sprintf(r, "  %-32s: %f\n", "dzwrtPV", (double)o.dzwrtPV); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct genparticlehelper_s
{
  bool	selected;
  int	firstMother;
  int	lastMother;
  int	firstDaughter;
  int	lastDaughter;
  int	charge;
  int	pdgId;
  int	status;
  double	pt;
  double	eta;
  double	phi;
  double	mass;
};
std::vector<genparticlehelper_s> genparticlehelper(200);

std::ostream& operator<<(std::ostream& os, const genparticlehelper_s& o)
{
  char r[1024];
  os << "genparticlehelper" << std::endl;
  sprintf(r, "  %-32s: %f\n", "firstMother", (double)o.firstMother); os << r;
  sprintf(r, "  %-32s: %f\n", "lastMother", (double)o.lastMother); os << r;
  sprintf(r, "  %-32s: %f\n", "firstDaughter", (double)o.firstDaughter); os << r;
  sprintf(r, "  %-32s: %f\n", "lastDaughter", (double)o.lastDaughter); os << r;
  sprintf(r, "  %-32s: %f\n", "charge", (double)o.charge); os << r;
  sprintf(r, "  %-32s: %f\n", "pdgId", (double)o.pdgId); os << r;
  sprintf(r, "  %-32s: %f\n", "status", (double)o.status); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "mass", (double)o.mass); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct jethelper_s
{
  bool	selected;
  double	energy;
  double	uncor_energy;
  double	et;
  double	uncor_et;
  double	pt;
  double	uncor_pt;
  double	phi;
  double	eta;
  double	mass;
  float	jetArea;
  float	chargedHadronEnergyFraction;
  float	neutralHadronEnergyFraction;
  float	chargedEmEnergyFraction;
  float	neutralEmEnergyFraction;
  float	photonEnergyFraction;
  float	muonEnergyFraction;
  int	chargedMultiplicity;
  int	nConstituents;
  double	genJet_energy;
  double	genJet_pt;
  double	genJet_phi;
  double	genJet_eta;
  int	genParton_pdgId;
  int	partonFlavour;
  float	trackCountingHighEffBJetTags;
  float	trackCountingHighPurBJetTags;
  float	jetProbabilityBJetTags;
  float	jetBProbabilityBJetTags;
  float	simpleSecondaryVertexHighEffBJetTags;
  float	simpleSecondaryVertexHighPurBJetTags;
  float	combinedSecondaryVertexBJetTags;
  float	combinedSecondaryVertexMVABJetTags;
  size_t	numberOfDaughters;
  double	daughter_0_energy;
  double	daughter_0_pt;
  double	daughter_0_eta;
  double	daughter_0_phi;
  double	daughter_0_mass;
  float	daughter_0_chargedHadronEnergyFraction;
  float	daughter_0_neutralHadronEnergyFraction;
  float	daughter_0_chargedEmEnergyFraction;
  float	daughter_0_neutralEmEnergyFraction;
  float	daughter_0_photonEnergyFraction;
  float	daughter_0_muonEnergyFraction;
  int	daughter_0_chargedMultiplicity;
  int	daughter_0_nConstituents;
  double	daughter_1_energy;
  double	daughter_1_pt;
  double	daughter_1_eta;
  double	daughter_1_phi;
  double	daughter_1_mass;
  float	daughter_1_chargedHadronEnergyFraction;
  float	daughter_1_neutralHadronEnergyFraction;
  float	daughter_1_chargedEmEnergyFraction;
  float	daughter_1_neutralEmEnergyFraction;
  float	daughter_1_photonEnergyFraction;
  float	daughter_1_muonEnergyFraction;
  int	daughter_1_chargedMultiplicity;
  int	daughter_1_nConstituents;
};
std::vector<jethelper_s> jethelper(10);

std::ostream& operator<<(std::ostream& os, const jethelper_s& o)
{
  char r[1024];
  os << "jethelper" << std::endl;
  sprintf(r, "  %-32s: %f\n", "energy", (double)o.energy); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_energy", (double)o.uncor_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "et", (double)o.et); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_et", (double)o.uncor_et); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_pt", (double)o.uncor_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "mass", (double)o.mass); os << r;
  sprintf(r, "  %-32s: %f\n", "jetArea", (double)o.jetArea); os << r;
  sprintf(r, "  %-32s: %f\n", "chargedHadronEnergyFraction", (double)o.chargedHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "neutralHadronEnergyFraction", (double)o.neutralHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "chargedEmEnergyFraction", (double)o.chargedEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "neutralEmEnergyFraction", (double)o.neutralEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "photonEnergyFraction", (double)o.photonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "muonEnergyFraction", (double)o.muonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "chargedMultiplicity", (double)o.chargedMultiplicity); os << r;
  sprintf(r, "  %-32s: %f\n", "nConstituents", (double)o.nConstituents); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_energy", (double)o.genJet_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_pt", (double)o.genJet_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_phi", (double)o.genJet_phi); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_eta", (double)o.genJet_eta); os << r;
  sprintf(r, "  %-32s: %f\n", "genParton_pdgId", (double)o.genParton_pdgId); os << r;
  sprintf(r, "  %-32s: %f\n", "partonFlavour", (double)o.partonFlavour); os << r;
  sprintf(r, "  %-32s: %f\n", "trackCountingHighEffBJetTags", (double)o.trackCountingHighEffBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "trackCountingHighPurBJetTags", (double)o.trackCountingHighPurBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "jetProbabilityBJetTags", (double)o.jetProbabilityBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "jetBProbabilityBJetTags", (double)o.jetBProbabilityBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleSecondaryVertexHighEffBJetTags", (double)o.simpleSecondaryVertexHighEffBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleSecondaryVertexHighPurBJetTags", (double)o.simpleSecondaryVertexHighPurBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "combinedSecondaryVertexBJetTags", (double)o.combinedSecondaryVertexBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "combinedSecondaryVertexMVABJetTags", (double)o.combinedSecondaryVertexMVABJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "numberOfDaughters", (double)o.numberOfDaughters); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_energy", (double)o.daughter_0_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_pt", (double)o.daughter_0_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_eta", (double)o.daughter_0_eta); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_phi", (double)o.daughter_0_phi); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_mass", (double)o.daughter_0_mass); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_chargedHadronEnergyFraction", (double)o.daughter_0_chargedHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_neutralHadronEnergyFraction", (double)o.daughter_0_neutralHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_chargedEmEnergyFraction", (double)o.daughter_0_chargedEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_neutralEmEnergyFraction", (double)o.daughter_0_neutralEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_photonEnergyFraction", (double)o.daughter_0_photonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_muonEnergyFraction", (double)o.daughter_0_muonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_chargedMultiplicity", (double)o.daughter_0_chargedMultiplicity); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_0_nConstituents", (double)o.daughter_0_nConstituents); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_energy", (double)o.daughter_1_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_pt", (double)o.daughter_1_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_eta", (double)o.daughter_1_eta); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_phi", (double)o.daughter_1_phi); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_mass", (double)o.daughter_1_mass); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_chargedHadronEnergyFraction", (double)o.daughter_1_chargedHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_neutralHadronEnergyFraction", (double)o.daughter_1_neutralHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_chargedEmEnergyFraction", (double)o.daughter_1_chargedEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_neutralEmEnergyFraction", (double)o.daughter_1_neutralEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_photonEnergyFraction", (double)o.daughter_1_photonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_muonEnergyFraction", (double)o.daughter_1_muonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_chargedMultiplicity", (double)o.daughter_1_chargedMultiplicity); os << r;
  sprintf(r, "  %-32s: %f\n", "daughter_1_nConstituents", (double)o.daughter_1_nConstituents); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct jethelper2_s
{
  bool	selected;
  double	energy;
  double	uncor_energy;
  double	et;
  double	uncor_et;
  double	pt;
  double	uncor_pt;
  double	phi;
  double	eta;
  double	mass;
  float	jetArea;
  float	chargedHadronEnergyFraction;
  float	neutralHadronEnergyFraction;
  float	chargedEmEnergyFraction;
  float	neutralEmEnergyFraction;
  float	photonEnergyFraction;
  float	muonEnergyFraction;
  int	chargedMultiplicity;
  int	nConstituents;
  double	genJet_energy;
  double	genJet_pt;
  double	genJet_phi;
  double	genJet_eta;
  int	genParton_pdgId;
  int	partonFlavour;
  float	trackCountingHighEffBJetTags;
  float	trackCountingHighPurBJetTags;
  float	jetProbabilityBJetTags;
  float	jetBProbabilityBJetTags;
  float	simpleSecondaryVertexHighEffBJetTags;
  float	simpleSecondaryVertexHighPurBJetTags;
  float	combinedSecondaryVertexBJetTags;
  float	combinedSecondaryVertexMVABJetTags;
};
std::vector<jethelper2_s> jethelper2(10);

std::ostream& operator<<(std::ostream& os, const jethelper2_s& o)
{
  char r[1024];
  os << "jethelper2" << std::endl;
  sprintf(r, "  %-32s: %f\n", "energy", (double)o.energy); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_energy", (double)o.uncor_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "et", (double)o.et); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_et", (double)o.uncor_et); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_pt", (double)o.uncor_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "mass", (double)o.mass); os << r;
  sprintf(r, "  %-32s: %f\n", "jetArea", (double)o.jetArea); os << r;
  sprintf(r, "  %-32s: %f\n", "chargedHadronEnergyFraction", (double)o.chargedHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "neutralHadronEnergyFraction", (double)o.neutralHadronEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "chargedEmEnergyFraction", (double)o.chargedEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "neutralEmEnergyFraction", (double)o.neutralEmEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "photonEnergyFraction", (double)o.photonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "muonEnergyFraction", (double)o.muonEnergyFraction); os << r;
  sprintf(r, "  %-32s: %f\n", "chargedMultiplicity", (double)o.chargedMultiplicity); os << r;
  sprintf(r, "  %-32s: %f\n", "nConstituents", (double)o.nConstituents); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_energy", (double)o.genJet_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_pt", (double)o.genJet_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_phi", (double)o.genJet_phi); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_eta", (double)o.genJet_eta); os << r;
  sprintf(r, "  %-32s: %f\n", "genParton_pdgId", (double)o.genParton_pdgId); os << r;
  sprintf(r, "  %-32s: %f\n", "partonFlavour", (double)o.partonFlavour); os << r;
  sprintf(r, "  %-32s: %f\n", "trackCountingHighEffBJetTags", (double)o.trackCountingHighEffBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "trackCountingHighPurBJetTags", (double)o.trackCountingHighPurBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "jetProbabilityBJetTags", (double)o.jetProbabilityBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "jetBProbabilityBJetTags", (double)o.jetBProbabilityBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleSecondaryVertexHighEffBJetTags", (double)o.simpleSecondaryVertexHighEffBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleSecondaryVertexHighPurBJetTags", (double)o.simpleSecondaryVertexHighPurBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "combinedSecondaryVertexBJetTags", (double)o.combinedSecondaryVertexBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "combinedSecondaryVertexMVABJetTags", (double)o.combinedSecondaryVertexMVABJetTags); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct jethelper3_s
{
  bool	selected;
  double	energy;
  double	uncor_energy;
  double	et;
  double	uncor_et;
  double	pt;
  double	uncor_pt;
  double	phi;
  double	eta;
  double	mass;
  float	jetArea;
  int	nConstituents;
  double	genJet_energy;
  double	genJet_pt;
  double	genJet_phi;
  double	genJet_eta;
  int	genParton_pdgId;
  int	partonFlavour;
  float	trackCountingHighEffBJetTags;
  float	trackCountingHighPurBJetTags;
  float	jetProbabilityBJetTags;
  float	jetBProbabilityBJetTags;
  float	simpleSecondaryVertexHighEffBJetTags;
  float	simpleSecondaryVertexHighPurBJetTags;
  float	combinedSecondaryVertexBJetTags;
  float	combinedSecondaryVertexMVABJetTags;
};
std::vector<jethelper3_s> jethelper3(10);

std::ostream& operator<<(std::ostream& os, const jethelper3_s& o)
{
  char r[1024];
  os << "jethelper3" << std::endl;
  sprintf(r, "  %-32s: %f\n", "energy", (double)o.energy); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_energy", (double)o.uncor_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "et", (double)o.et); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_et", (double)o.uncor_et); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "uncor_pt", (double)o.uncor_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "mass", (double)o.mass); os << r;
  sprintf(r, "  %-32s: %f\n", "jetArea", (double)o.jetArea); os << r;
  sprintf(r, "  %-32s: %f\n", "nConstituents", (double)o.nConstituents); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_energy", (double)o.genJet_energy); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_pt", (double)o.genJet_pt); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_phi", (double)o.genJet_phi); os << r;
  sprintf(r, "  %-32s: %f\n", "genJet_eta", (double)o.genJet_eta); os << r;
  sprintf(r, "  %-32s: %f\n", "genParton_pdgId", (double)o.genParton_pdgId); os << r;
  sprintf(r, "  %-32s: %f\n", "partonFlavour", (double)o.partonFlavour); os << r;
  sprintf(r, "  %-32s: %f\n", "trackCountingHighEffBJetTags", (double)o.trackCountingHighEffBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "trackCountingHighPurBJetTags", (double)o.trackCountingHighPurBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "jetProbabilityBJetTags", (double)o.jetProbabilityBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "jetBProbabilityBJetTags", (double)o.jetBProbabilityBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleSecondaryVertexHighEffBJetTags", (double)o.simpleSecondaryVertexHighEffBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "simpleSecondaryVertexHighPurBJetTags", (double)o.simpleSecondaryVertexHighPurBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "combinedSecondaryVertexBJetTags", (double)o.combinedSecondaryVertexBJetTags); os << r;
  sprintf(r, "  %-32s: %f\n", "combinedSecondaryVertexMVABJetTags", (double)o.combinedSecondaryVertexMVABJetTags); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct muonhelper_s
{
  bool	selected;
  double	energy;
  double	et;
  double	pt;
  double	phi;
  double	eta;
  int	charge;
  float	TMOneStationTight;
  int	isGlobalMuon;
  int	isTrackerMuon;
  int	isPFMuon;
  unsigned short	track_hitPattern_trackerLayersWithMeasurement;
  unsigned short	innerTrack_hitPattern_pixelLayersWithMeasurement;
  double	innerTrack_normalizedChi2;
  double	globalTrack_normalizedChi2;
  unsigned short	globalTrack_hitPattern_numberOfValidMuonHits;
  int	numberOfMatchedStations;
  double	dB;
  unsigned short	innerTrack_hitPattern_numberOfValidPixelHits;
  float	pfIsolationR04_sumChargedHadronPt;
  float	pfIsolationR04_sumChargedParticlePt;
  float	pfIsolationR04_sumNeutralHadronEt;
  float	pfIsolationR04_sumPhotonEt;
  float	pfIsolationR04_sumNeutralHadronEtHighThreshold;
  float	pfIsolationR04_sumPhotonEtHighThreshold;
  float	pfIsolationR04_sumPUPt;
  double	dxywrtPV;
  double	dzwrtPV;
};
std::vector<muonhelper_s> muonhelper(10);

std::ostream& operator<<(std::ostream& os, const muonhelper_s& o)
{
  char r[1024];
  os << "muonhelper" << std::endl;
  sprintf(r, "  %-32s: %f\n", "energy", (double)o.energy); os << r;
  sprintf(r, "  %-32s: %f\n", "et", (double)o.et); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "charge", (double)o.charge); os << r;
  sprintf(r, "  %-32s: %f\n", "TMOneStationTight", (double)o.TMOneStationTight); os << r;
  sprintf(r, "  %-32s: %f\n", "isGlobalMuon", (double)o.isGlobalMuon); os << r;
  sprintf(r, "  %-32s: %f\n", "isTrackerMuon", (double)o.isTrackerMuon); os << r;
  sprintf(r, "  %-32s: %f\n", "isPFMuon", (double)o.isPFMuon); os << r;
  sprintf(r, "  %-32s: %f\n", "track_hitPattern_trackerLayersWithMeasurement", (double)o.track_hitPattern_trackerLayersWithMeasurement); os << r;
  sprintf(r, "  %-32s: %f\n", "innerTrack_hitPattern_pixelLayersWithMeasurement", (double)o.innerTrack_hitPattern_pixelLayersWithMeasurement); os << r;
  sprintf(r, "  %-32s: %f\n", "innerTrack_normalizedChi2", (double)o.innerTrack_normalizedChi2); os << r;
  sprintf(r, "  %-32s: %f\n", "globalTrack_normalizedChi2", (double)o.globalTrack_normalizedChi2); os << r;
  sprintf(r, "  %-32s: %f\n", "globalTrack_hitPattern_numberOfValidMuonHits", (double)o.globalTrack_hitPattern_numberOfValidMuonHits); os << r;
  sprintf(r, "  %-32s: %f\n", "numberOfMatchedStations", (double)o.numberOfMatchedStations); os << r;
  sprintf(r, "  %-32s: %f\n", "dB", (double)o.dB); os << r;
  sprintf(r, "  %-32s: %f\n", "innerTrack_hitPattern_numberOfValidPixelHits", (double)o.innerTrack_hitPattern_numberOfValidPixelHits); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumChargedHadronPt", (double)o.pfIsolationR04_sumChargedHadronPt); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumChargedParticlePt", (double)o.pfIsolationR04_sumChargedParticlePt); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumNeutralHadronEt", (double)o.pfIsolationR04_sumNeutralHadronEt); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumPhotonEt", (double)o.pfIsolationR04_sumPhotonEt); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumNeutralHadronEtHighThreshold", (double)o.pfIsolationR04_sumNeutralHadronEtHighThreshold); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumPhotonEtHighThreshold", (double)o.pfIsolationR04_sumPhotonEtHighThreshold); os << r;
  sprintf(r, "  %-32s: %f\n", "pfIsolationR04_sumPUPt", (double)o.pfIsolationR04_sumPUPt); os << r;
  sprintf(r, "  %-32s: %f\n", "dxywrtPV", (double)o.dxywrtPV); os << r;
  sprintf(r, "  %-32s: %f\n", "dzwrtPV", (double)o.dzwrtPV); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct tau_s
{
  bool	selected;
  double	energy;
  double	et;
  double	pt;
  double	phi;
  double	eta;
  float	byLooseCombinedIsolationDeltaBetaCorr;
  float	byMediumCombinedIsolationDeltaBetaCorr;
  float	trackIso;
  float	ecalIso;
  float	hcalIso;
  float	caloIso;
};
std::vector<tau_s> tau(10);

std::ostream& operator<<(std::ostream& os, const tau_s& o)
{
  char r[1024];
  os << "tau" << std::endl;
  sprintf(r, "  %-32s: %f\n", "energy", (double)o.energy); os << r;
  sprintf(r, "  %-32s: %f\n", "et", (double)o.et); os << r;
  sprintf(r, "  %-32s: %f\n", "pt", (double)o.pt); os << r;
  sprintf(r, "  %-32s: %f\n", "phi", (double)o.phi); os << r;
  sprintf(r, "  %-32s: %f\n", "eta", (double)o.eta); os << r;
  sprintf(r, "  %-32s: %f\n", "byLooseCombinedIsolationDeltaBetaCorr", (double)o.byLooseCombinedIsolationDeltaBetaCorr); os << r;
  sprintf(r, "  %-32s: %f\n", "byMediumCombinedIsolationDeltaBetaCorr", (double)o.byMediumCombinedIsolationDeltaBetaCorr); os << r;
  sprintf(r, "  %-32s: %f\n", "trackIso", (double)o.trackIso); os << r;
  sprintf(r, "  %-32s: %f\n", "ecalIso", (double)o.ecalIso); os << r;
  sprintf(r, "  %-32s: %f\n", "hcalIso", (double)o.hcalIso); os << r;
  sprintf(r, "  %-32s: %f\n", "caloIso", (double)o.caloIso); os << r;
  return os;
}
//-----------------------------------------------------------------------------
struct vertex_s
{
  bool	selected;
  int	isFake;
  double	chi2;
  double	ndof;
  double	x;
  double	y;
  double	z;
  double	xError;
  double	yError;
  double	zError;
  double	position_Rho;
};
std::vector<vertex_s> vertex(200);

std::ostream& operator<<(std::ostream& os, const vertex_s& o)
{
  char r[1024];
  os << "vertex" << std::endl;
  sprintf(r, "  %-32s: %f\n", "isFake", (double)o.isFake); os << r;
  sprintf(r, "  %-32s: %f\n", "chi2", (double)o.chi2); os << r;
  sprintf(r, "  %-32s: %f\n", "ndof", (double)o.ndof); os << r;
  sprintf(r, "  %-32s: %f\n", "x", (double)o.x); os << r;
  sprintf(r, "  %-32s: %f\n", "y", (double)o.y); os << r;
  sprintf(r, "  %-32s: %f\n", "z", (double)o.z); os << r;
  sprintf(r, "  %-32s: %f\n", "xError", (double)o.xError); os << r;
  sprintf(r, "  %-32s: %f\n", "yError", (double)o.yError); os << r;
  sprintf(r, "  %-32s: %f\n", "zError", (double)o.zError); os << r;
  sprintf(r, "  %-32s: %f\n", "position_Rho", (double)o.position_Rho); os << r;
  return os;
}
//-----------------------------------------------------------------------------

static bool fillObjectsCalled = false;
void fillObjects()
{
  fillObjectsCalled = true;

  electronhelper.resize(electronhelper_energy.size());
  for(unsigned int i=0; i < electronhelper.size(); ++i)
    {
      electronhelper[i].selected	= false;
      electronhelper[i].energy	= electronhelper_energy[i];
      electronhelper[i].et	= electronhelper_et[i];
      electronhelper[i].pt	= electronhelper_pt[i];
      electronhelper[i].phi	= electronhelper_phi[i];
      electronhelper[i].eta	= electronhelper_eta[i];
      electronhelper[i].charge	= electronhelper_charge[i];
      electronhelper[i].eSuperClusterOverP	= electronhelper_eSuperClusterOverP[i];
      electronhelper[i].deltaEtaSuperClusterTrackAtVtx	= electronhelper_deltaEtaSuperClusterTrackAtVtx[i];
      electronhelper[i].deltaPhiSuperClusterTrackAtVtx	= electronhelper_deltaPhiSuperClusterTrackAtVtx[i];
      electronhelper[i].isEB	= electronhelper_isEB[i];
      electronhelper[i].isEE	= electronhelper_isEE[i];
      electronhelper[i].scSigmaIEtaIEta	= electronhelper_scSigmaIEtaIEta[i];
      electronhelper[i].hadronicOverEm	= electronhelper_hadronicOverEm[i];
      electronhelper[i].superCluster_energy	= electronhelper_superCluster_energy[i];
      electronhelper[i].gsfTrack_trackerExpectedHitsInner_numberOfHits	= electronhelper_gsfTrack_trackerExpectedHitsInner_numberOfHits[i];
      electronhelper[i].simpleEleId80relIso	= electronhelper_simpleEleId80relIso[i];
      electronhelper[i].simpleEleId95relIso	= electronhelper_simpleEleId95relIso[i];
      electronhelper[i].trackIso	= electronhelper_trackIso[i];
      electronhelper[i].ecalIso	= electronhelper_ecalIso[i];
      electronhelper[i].hcalIso	= electronhelper_hcalIso[i];
      electronhelper[i].caloIso	= electronhelper_caloIso[i];
      electronhelper[i].dxywrtPV	= electronhelper_dxywrtPV[i];
      electronhelper[i].dzwrtPV	= electronhelper_dzwrtPV[i];
    }

  genparticlehelper.resize(genparticlehelper_firstMother.size());
  for(unsigned int i=0; i < genparticlehelper.size(); ++i)
    {
      genparticlehelper[i].selected	= false;
      genparticlehelper[i].firstMother	= genparticlehelper_firstMother[i];
      genparticlehelper[i].lastMother	= genparticlehelper_lastMother[i];
      genparticlehelper[i].firstDaughter	= genparticlehelper_firstDaughter[i];
      genparticlehelper[i].lastDaughter	= genparticlehelper_lastDaughter[i];
      genparticlehelper[i].charge	= genparticlehelper_charge[i];
      genparticlehelper[i].pdgId	= genparticlehelper_pdgId[i];
      genparticlehelper[i].status	= genparticlehelper_status[i];
      genparticlehelper[i].pt	= genparticlehelper_pt[i];
      genparticlehelper[i].eta	= genparticlehelper_eta[i];
      genparticlehelper[i].phi	= genparticlehelper_phi[i];
      genparticlehelper[i].mass	= genparticlehelper_mass[i];
    }

  jethelper.resize(jethelper_energy.size());
  for(unsigned int i=0; i < jethelper.size(); ++i)
    {
      jethelper[i].selected	= false;
      jethelper[i].energy	= jethelper_energy[i];
      jethelper[i].uncor_energy	= jethelper_uncor_energy[i];
      jethelper[i].et	= jethelper_et[i];
      jethelper[i].uncor_et	= jethelper_uncor_et[i];
      jethelper[i].pt	= jethelper_pt[i];
      jethelper[i].uncor_pt	= jethelper_uncor_pt[i];
      jethelper[i].phi	= jethelper_phi[i];
      jethelper[i].eta	= jethelper_eta[i];
      jethelper[i].mass	= jethelper_mass[i];
      jethelper[i].jetArea	= jethelper_jetArea[i];
      jethelper[i].chargedHadronEnergyFraction	= jethelper_chargedHadronEnergyFraction[i];
      jethelper[i].neutralHadronEnergyFraction	= jethelper_neutralHadronEnergyFraction[i];
      jethelper[i].chargedEmEnergyFraction	= jethelper_chargedEmEnergyFraction[i];
      jethelper[i].neutralEmEnergyFraction	= jethelper_neutralEmEnergyFraction[i];
      jethelper[i].photonEnergyFraction	= jethelper_photonEnergyFraction[i];
      jethelper[i].muonEnergyFraction	= jethelper_muonEnergyFraction[i];
      jethelper[i].chargedMultiplicity	= jethelper_chargedMultiplicity[i];
      jethelper[i].nConstituents	= jethelper_nConstituents[i];
      jethelper[i].genJet_energy	= jethelper_genJet_energy[i];
      jethelper[i].genJet_pt	= jethelper_genJet_pt[i];
      jethelper[i].genJet_phi	= jethelper_genJet_phi[i];
      jethelper[i].genJet_eta	= jethelper_genJet_eta[i];
      jethelper[i].genParton_pdgId	= jethelper_genParton_pdgId[i];
      jethelper[i].partonFlavour	= jethelper_partonFlavour[i];
      jethelper[i].trackCountingHighEffBJetTags	= jethelper_trackCountingHighEffBJetTags[i];
      jethelper[i].trackCountingHighPurBJetTags	= jethelper_trackCountingHighPurBJetTags[i];
      jethelper[i].jetProbabilityBJetTags	= jethelper_jetProbabilityBJetTags[i];
      jethelper[i].jetBProbabilityBJetTags	= jethelper_jetBProbabilityBJetTags[i];
      jethelper[i].simpleSecondaryVertexHighEffBJetTags	= jethelper_simpleSecondaryVertexHighEffBJetTags[i];
      jethelper[i].simpleSecondaryVertexHighPurBJetTags	= jethelper_simpleSecondaryVertexHighPurBJetTags[i];
      jethelper[i].combinedSecondaryVertexBJetTags	= jethelper_combinedSecondaryVertexBJetTags[i];
      jethelper[i].combinedSecondaryVertexMVABJetTags	= jethelper_combinedSecondaryVertexMVABJetTags[i];
      jethelper[i].numberOfDaughters	= jethelper_numberOfDaughters[i];
      jethelper[i].daughter_0_energy	= jethelper_daughter_0_energy[i];
      jethelper[i].daughter_0_pt	= jethelper_daughter_0_pt[i];
      jethelper[i].daughter_0_eta	= jethelper_daughter_0_eta[i];
      jethelper[i].daughter_0_phi	= jethelper_daughter_0_phi[i];
      jethelper[i].daughter_0_mass	= jethelper_daughter_0_mass[i];
      jethelper[i].daughter_0_chargedHadronEnergyFraction	= jethelper_daughter_0_chargedHadronEnergyFraction[i];
      jethelper[i].daughter_0_neutralHadronEnergyFraction	= jethelper_daughter_0_neutralHadronEnergyFraction[i];
      jethelper[i].daughter_0_chargedEmEnergyFraction	= jethelper_daughter_0_chargedEmEnergyFraction[i];
      jethelper[i].daughter_0_neutralEmEnergyFraction	= jethelper_daughter_0_neutralEmEnergyFraction[i];
      jethelper[i].daughter_0_photonEnergyFraction	= jethelper_daughter_0_photonEnergyFraction[i];
      jethelper[i].daughter_0_muonEnergyFraction	= jethelper_daughter_0_muonEnergyFraction[i];
      jethelper[i].daughter_0_chargedMultiplicity	= jethelper_daughter_0_chargedMultiplicity[i];
      jethelper[i].daughter_0_nConstituents	= jethelper_daughter_0_nConstituents[i];
      jethelper[i].daughter_1_energy	= jethelper_daughter_1_energy[i];
      jethelper[i].daughter_1_pt	= jethelper_daughter_1_pt[i];
      jethelper[i].daughter_1_eta	= jethelper_daughter_1_eta[i];
      jethelper[i].daughter_1_phi	= jethelper_daughter_1_phi[i];
      jethelper[i].daughter_1_mass	= jethelper_daughter_1_mass[i];
      jethelper[i].daughter_1_chargedHadronEnergyFraction	= jethelper_daughter_1_chargedHadronEnergyFraction[i];
      jethelper[i].daughter_1_neutralHadronEnergyFraction	= jethelper_daughter_1_neutralHadronEnergyFraction[i];
      jethelper[i].daughter_1_chargedEmEnergyFraction	= jethelper_daughter_1_chargedEmEnergyFraction[i];
      jethelper[i].daughter_1_neutralEmEnergyFraction	= jethelper_daughter_1_neutralEmEnergyFraction[i];
      jethelper[i].daughter_1_photonEnergyFraction	= jethelper_daughter_1_photonEnergyFraction[i];
      jethelper[i].daughter_1_muonEnergyFraction	= jethelper_daughter_1_muonEnergyFraction[i];
      jethelper[i].daughter_1_chargedMultiplicity	= jethelper_daughter_1_chargedMultiplicity[i];
      jethelper[i].daughter_1_nConstituents	= jethelper_daughter_1_nConstituents[i];
    }

  jethelper2.resize(jethelper2_energy.size());
  for(unsigned int i=0; i < jethelper2.size(); ++i)
    {
      jethelper2[i].selected	= false;
      jethelper2[i].energy	= jethelper2_energy[i];
      jethelper2[i].uncor_energy	= jethelper2_uncor_energy[i];
      jethelper2[i].et	= jethelper2_et[i];
      jethelper2[i].uncor_et	= jethelper2_uncor_et[i];
      jethelper2[i].pt	= jethelper2_pt[i];
      jethelper2[i].uncor_pt	= jethelper2_uncor_pt[i];
      jethelper2[i].phi	= jethelper2_phi[i];
      jethelper2[i].eta	= jethelper2_eta[i];
      jethelper2[i].mass	= jethelper2_mass[i];
      jethelper2[i].jetArea	= jethelper2_jetArea[i];
      jethelper2[i].chargedHadronEnergyFraction	= jethelper2_chargedHadronEnergyFraction[i];
      jethelper2[i].neutralHadronEnergyFraction	= jethelper2_neutralHadronEnergyFraction[i];
      jethelper2[i].chargedEmEnergyFraction	= jethelper2_chargedEmEnergyFraction[i];
      jethelper2[i].neutralEmEnergyFraction	= jethelper2_neutralEmEnergyFraction[i];
      jethelper2[i].photonEnergyFraction	= jethelper2_photonEnergyFraction[i];
      jethelper2[i].muonEnergyFraction	= jethelper2_muonEnergyFraction[i];
      jethelper2[i].chargedMultiplicity	= jethelper2_chargedMultiplicity[i];
      jethelper2[i].nConstituents	= jethelper2_nConstituents[i];
      jethelper2[i].genJet_energy	= jethelper2_genJet_energy[i];
      jethelper2[i].genJet_pt	= jethelper2_genJet_pt[i];
      jethelper2[i].genJet_phi	= jethelper2_genJet_phi[i];
      jethelper2[i].genJet_eta	= jethelper2_genJet_eta[i];
      jethelper2[i].genParton_pdgId	= jethelper2_genParton_pdgId[i];
      jethelper2[i].partonFlavour	= jethelper2_partonFlavour[i];
      jethelper2[i].trackCountingHighEffBJetTags	= jethelper2_trackCountingHighEffBJetTags[i];
      jethelper2[i].trackCountingHighPurBJetTags	= jethelper2_trackCountingHighPurBJetTags[i];
      jethelper2[i].jetProbabilityBJetTags	= jethelper2_jetProbabilityBJetTags[i];
      jethelper2[i].jetBProbabilityBJetTags	= jethelper2_jetBProbabilityBJetTags[i];
      jethelper2[i].simpleSecondaryVertexHighEffBJetTags	= jethelper2_simpleSecondaryVertexHighEffBJetTags[i];
      jethelper2[i].simpleSecondaryVertexHighPurBJetTags	= jethelper2_simpleSecondaryVertexHighPurBJetTags[i];
      jethelper2[i].combinedSecondaryVertexBJetTags	= jethelper2_combinedSecondaryVertexBJetTags[i];
      jethelper2[i].combinedSecondaryVertexMVABJetTags	= jethelper2_combinedSecondaryVertexMVABJetTags[i];
    }

  jethelper3.resize(jethelper3_energy.size());
  for(unsigned int i=0; i < jethelper3.size(); ++i)
    {
      jethelper3[i].selected	= false;
      jethelper3[i].energy	= jethelper3_energy[i];
      jethelper3[i].uncor_energy	= jethelper3_uncor_energy[i];
      jethelper3[i].et	= jethelper3_et[i];
      jethelper3[i].uncor_et	= jethelper3_uncor_et[i];
      jethelper3[i].pt	= jethelper3_pt[i];
      jethelper3[i].uncor_pt	= jethelper3_uncor_pt[i];
      jethelper3[i].phi	= jethelper3_phi[i];
      jethelper3[i].eta	= jethelper3_eta[i];
      jethelper3[i].mass	= jethelper3_mass[i];
      jethelper3[i].jetArea	= jethelper3_jetArea[i];
      jethelper3[i].nConstituents	= jethelper3_nConstituents[i];
      jethelper3[i].genJet_energy	= jethelper3_genJet_energy[i];
      jethelper3[i].genJet_pt	= jethelper3_genJet_pt[i];
      jethelper3[i].genJet_phi	= jethelper3_genJet_phi[i];
      jethelper3[i].genJet_eta	= jethelper3_genJet_eta[i];
      jethelper3[i].genParton_pdgId	= jethelper3_genParton_pdgId[i];
      jethelper3[i].partonFlavour	= jethelper3_partonFlavour[i];
      jethelper3[i].trackCountingHighEffBJetTags	= jethelper3_trackCountingHighEffBJetTags[i];
      jethelper3[i].trackCountingHighPurBJetTags	= jethelper3_trackCountingHighPurBJetTags[i];
      jethelper3[i].jetProbabilityBJetTags	= jethelper3_jetProbabilityBJetTags[i];
      jethelper3[i].jetBProbabilityBJetTags	= jethelper3_jetBProbabilityBJetTags[i];
      jethelper3[i].simpleSecondaryVertexHighEffBJetTags	= jethelper3_simpleSecondaryVertexHighEffBJetTags[i];
      jethelper3[i].simpleSecondaryVertexHighPurBJetTags	= jethelper3_simpleSecondaryVertexHighPurBJetTags[i];
      jethelper3[i].combinedSecondaryVertexBJetTags	= jethelper3_combinedSecondaryVertexBJetTags[i];
      jethelper3[i].combinedSecondaryVertexMVABJetTags	= jethelper3_combinedSecondaryVertexMVABJetTags[i];
    }

  muonhelper.resize(muonhelper_energy.size());
  for(unsigned int i=0; i < muonhelper.size(); ++i)
    {
      muonhelper[i].selected	= false;
      muonhelper[i].energy	= muonhelper_energy[i];
      muonhelper[i].et	= muonhelper_et[i];
      muonhelper[i].pt	= muonhelper_pt[i];
      muonhelper[i].phi	= muonhelper_phi[i];
      muonhelper[i].eta	= muonhelper_eta[i];
      muonhelper[i].charge	= muonhelper_charge[i];
      muonhelper[i].TMOneStationTight	= muonhelper_TMOneStationTight[i];
      muonhelper[i].isGlobalMuon	= muonhelper_isGlobalMuon[i];
      muonhelper[i].isTrackerMuon	= muonhelper_isTrackerMuon[i];
      muonhelper[i].isPFMuon	= muonhelper_isPFMuon[i];
      muonhelper[i].track_hitPattern_trackerLayersWithMeasurement	= muonhelper_track_hitPattern_trackerLayersWithMeasurement[i];
      muonhelper[i].innerTrack_hitPattern_pixelLayersWithMeasurement	= muonhelper_innerTrack_hitPattern_pixelLayersWithMeasurement[i];
      muonhelper[i].innerTrack_normalizedChi2	= muonhelper_innerTrack_normalizedChi2[i];
      muonhelper[i].globalTrack_normalizedChi2	= muonhelper_globalTrack_normalizedChi2[i];
      muonhelper[i].globalTrack_hitPattern_numberOfValidMuonHits	= muonhelper_globalTrack_hitPattern_numberOfValidMuonHits[i];
      muonhelper[i].numberOfMatchedStations	= muonhelper_numberOfMatchedStations[i];
      muonhelper[i].dB	= muonhelper_dB[i];
      muonhelper[i].innerTrack_hitPattern_numberOfValidPixelHits	= muonhelper_innerTrack_hitPattern_numberOfValidPixelHits[i];
      muonhelper[i].pfIsolationR04_sumChargedHadronPt	= muonhelper_pfIsolationR04_sumChargedHadronPt[i];
      muonhelper[i].pfIsolationR04_sumChargedParticlePt	= muonhelper_pfIsolationR04_sumChargedParticlePt[i];
      muonhelper[i].pfIsolationR04_sumNeutralHadronEt	= muonhelper_pfIsolationR04_sumNeutralHadronEt[i];
      muonhelper[i].pfIsolationR04_sumPhotonEt	= muonhelper_pfIsolationR04_sumPhotonEt[i];
      muonhelper[i].pfIsolationR04_sumNeutralHadronEtHighThreshold	= muonhelper_pfIsolationR04_sumNeutralHadronEtHighThreshold[i];
      muonhelper[i].pfIsolationR04_sumPhotonEtHighThreshold	= muonhelper_pfIsolationR04_sumPhotonEtHighThreshold[i];
      muonhelper[i].pfIsolationR04_sumPUPt	= muonhelper_pfIsolationR04_sumPUPt[i];
      muonhelper[i].dxywrtPV	= muonhelper_dxywrtPV[i];
      muonhelper[i].dzwrtPV	= muonhelper_dzwrtPV[i];
    }

  tau.resize(tau_energy.size());
  for(unsigned int i=0; i < tau.size(); ++i)
    {
      tau[i].selected	= false;
      tau[i].energy	= tau_energy[i];
      tau[i].et	= tau_et[i];
      tau[i].pt	= tau_pt[i];
      tau[i].phi	= tau_phi[i];
      tau[i].eta	= tau_eta[i];
      tau[i].byLooseCombinedIsolationDeltaBetaCorr	= tau_byLooseCombinedIsolationDeltaBetaCorr[i];
      tau[i].byMediumCombinedIsolationDeltaBetaCorr	= tau_byMediumCombinedIsolationDeltaBetaCorr[i];
      tau[i].trackIso	= tau_trackIso[i];
      tau[i].ecalIso	= tau_ecalIso[i];
      tau[i].hcalIso	= tau_hcalIso[i];
      tau[i].caloIso	= tau_caloIso[i];
    }

  vertex.resize(vertex_isFake.size());
  for(unsigned int i=0; i < vertex.size(); ++i)
    {
      vertex[i].selected	= false;
      vertex[i].isFake	= vertex_isFake[i];
      vertex[i].chi2	= vertex_chi2[i];
      vertex[i].ndof	= vertex_ndof[i];
      vertex[i].x	= vertex_x[i];
      vertex[i].y	= vertex_y[i];
      vertex[i].z	= vertex_z[i];
      vertex[i].xError	= vertex_xError[i];
      vertex[i].yError	= vertex_yError[i];
      vertex[i].zError	= vertex_zError[i];
      vertex[i].position_Rho	= vertex_position_Rho[i];
    }
}

//-----------------------------------------------------------------------------
// --- Call saveSelectedObjects() just before call to addEvent if
// --- you wish to save only the selected objects
//-----------------------------------------------------------------------------
void saveSelectedObjects()
{
  if ( ! fillObjectsCalled ) return;
  int n = 0;

  n = 0;
  for(unsigned int i=0; i < electronhelper.size(); ++i)
    {
      if ( ! electronhelper[i].selected ) continue;
      electronhelper_energy[n]	= electronhelper[i].energy;
      electronhelper_et[n]	= electronhelper[i].et;
      electronhelper_pt[n]	= electronhelper[i].pt;
      electronhelper_phi[n]	= electronhelper[i].phi;
      electronhelper_eta[n]	= electronhelper[i].eta;
      electronhelper_charge[n]	= electronhelper[i].charge;
      electronhelper_eSuperClusterOverP[n]	= electronhelper[i].eSuperClusterOverP;
      electronhelper_deltaEtaSuperClusterTrackAtVtx[n]	= electronhelper[i].deltaEtaSuperClusterTrackAtVtx;
      electronhelper_deltaPhiSuperClusterTrackAtVtx[n]	= electronhelper[i].deltaPhiSuperClusterTrackAtVtx;
      electronhelper_isEB[n]	= electronhelper[i].isEB;
      electronhelper_isEE[n]	= electronhelper[i].isEE;
      electronhelper_scSigmaIEtaIEta[n]	= electronhelper[i].scSigmaIEtaIEta;
      electronhelper_hadronicOverEm[n]	= electronhelper[i].hadronicOverEm;
      electronhelper_superCluster_energy[n]	= electronhelper[i].superCluster_energy;
      electronhelper_gsfTrack_trackerExpectedHitsInner_numberOfHits[n]	= electronhelper[i].gsfTrack_trackerExpectedHitsInner_numberOfHits;
      electronhelper_simpleEleId80relIso[n]	= electronhelper[i].simpleEleId80relIso;
      electronhelper_simpleEleId95relIso[n]	= electronhelper[i].simpleEleId95relIso;
      electronhelper_trackIso[n]	= electronhelper[i].trackIso;
      electronhelper_ecalIso[n]	= electronhelper[i].ecalIso;
      electronhelper_hcalIso[n]	= electronhelper[i].hcalIso;
      electronhelper_caloIso[n]	= electronhelper[i].caloIso;
      electronhelper_dxywrtPV[n]	= electronhelper[i].dxywrtPV;
      electronhelper_dzwrtPV[n]	= electronhelper[i].dzwrtPV;
      n++;
    }
  nelectronhelper = n;

  n = 0;
  for(unsigned int i=0; i < genparticlehelper.size(); ++i)
    {
      if ( ! genparticlehelper[i].selected ) continue;
      genparticlehelper_firstMother[n]	= genparticlehelper[i].firstMother;
      genparticlehelper_lastMother[n]	= genparticlehelper[i].lastMother;
      genparticlehelper_firstDaughter[n]	= genparticlehelper[i].firstDaughter;
      genparticlehelper_lastDaughter[n]	= genparticlehelper[i].lastDaughter;
      genparticlehelper_charge[n]	= genparticlehelper[i].charge;
      genparticlehelper_pdgId[n]	= genparticlehelper[i].pdgId;
      genparticlehelper_status[n]	= genparticlehelper[i].status;
      genparticlehelper_pt[n]	= genparticlehelper[i].pt;
      genparticlehelper_eta[n]	= genparticlehelper[i].eta;
      genparticlehelper_phi[n]	= genparticlehelper[i].phi;
      genparticlehelper_mass[n]	= genparticlehelper[i].mass;
      n++;
    }
  ngenparticlehelper = n;

  n = 0;
  for(unsigned int i=0; i < jethelper.size(); ++i)
    {
      if ( ! jethelper[i].selected ) continue;
      jethelper_energy[n]	= jethelper[i].energy;
      jethelper_uncor_energy[n]	= jethelper[i].uncor_energy;
      jethelper_et[n]	= jethelper[i].et;
      jethelper_uncor_et[n]	= jethelper[i].uncor_et;
      jethelper_pt[n]	= jethelper[i].pt;
      jethelper_uncor_pt[n]	= jethelper[i].uncor_pt;
      jethelper_phi[n]	= jethelper[i].phi;
      jethelper_eta[n]	= jethelper[i].eta;
      jethelper_mass[n]	= jethelper[i].mass;
      jethelper_jetArea[n]	= jethelper[i].jetArea;
      jethelper_chargedHadronEnergyFraction[n]	= jethelper[i].chargedHadronEnergyFraction;
      jethelper_neutralHadronEnergyFraction[n]	= jethelper[i].neutralHadronEnergyFraction;
      jethelper_chargedEmEnergyFraction[n]	= jethelper[i].chargedEmEnergyFraction;
      jethelper_neutralEmEnergyFraction[n]	= jethelper[i].neutralEmEnergyFraction;
      jethelper_photonEnergyFraction[n]	= jethelper[i].photonEnergyFraction;
      jethelper_muonEnergyFraction[n]	= jethelper[i].muonEnergyFraction;
      jethelper_chargedMultiplicity[n]	= jethelper[i].chargedMultiplicity;
      jethelper_nConstituents[n]	= jethelper[i].nConstituents;
      jethelper_genJet_energy[n]	= jethelper[i].genJet_energy;
      jethelper_genJet_pt[n]	= jethelper[i].genJet_pt;
      jethelper_genJet_phi[n]	= jethelper[i].genJet_phi;
      jethelper_genJet_eta[n]	= jethelper[i].genJet_eta;
      jethelper_genParton_pdgId[n]	= jethelper[i].genParton_pdgId;
      jethelper_partonFlavour[n]	= jethelper[i].partonFlavour;
      jethelper_trackCountingHighEffBJetTags[n]	= jethelper[i].trackCountingHighEffBJetTags;
      jethelper_trackCountingHighPurBJetTags[n]	= jethelper[i].trackCountingHighPurBJetTags;
      jethelper_jetProbabilityBJetTags[n]	= jethelper[i].jetProbabilityBJetTags;
      jethelper_jetBProbabilityBJetTags[n]	= jethelper[i].jetBProbabilityBJetTags;
      jethelper_simpleSecondaryVertexHighEffBJetTags[n]	= jethelper[i].simpleSecondaryVertexHighEffBJetTags;
      jethelper_simpleSecondaryVertexHighPurBJetTags[n]	= jethelper[i].simpleSecondaryVertexHighPurBJetTags;
      jethelper_combinedSecondaryVertexBJetTags[n]	= jethelper[i].combinedSecondaryVertexBJetTags;
      jethelper_combinedSecondaryVertexMVABJetTags[n]	= jethelper[i].combinedSecondaryVertexMVABJetTags;
      jethelper_numberOfDaughters[n]	= jethelper[i].numberOfDaughters;
      jethelper_daughter_0_energy[n]	= jethelper[i].daughter_0_energy;
      jethelper_daughter_0_pt[n]	= jethelper[i].daughter_0_pt;
      jethelper_daughter_0_eta[n]	= jethelper[i].daughter_0_eta;
      jethelper_daughter_0_phi[n]	= jethelper[i].daughter_0_phi;
      jethelper_daughter_0_mass[n]	= jethelper[i].daughter_0_mass;
      jethelper_daughter_0_chargedHadronEnergyFraction[n]	= jethelper[i].daughter_0_chargedHadronEnergyFraction;
      jethelper_daughter_0_neutralHadronEnergyFraction[n]	= jethelper[i].daughter_0_neutralHadronEnergyFraction;
      jethelper_daughter_0_chargedEmEnergyFraction[n]	= jethelper[i].daughter_0_chargedEmEnergyFraction;
      jethelper_daughter_0_neutralEmEnergyFraction[n]	= jethelper[i].daughter_0_neutralEmEnergyFraction;
      jethelper_daughter_0_photonEnergyFraction[n]	= jethelper[i].daughter_0_photonEnergyFraction;
      jethelper_daughter_0_muonEnergyFraction[n]	= jethelper[i].daughter_0_muonEnergyFraction;
      jethelper_daughter_0_chargedMultiplicity[n]	= jethelper[i].daughter_0_chargedMultiplicity;
      jethelper_daughter_0_nConstituents[n]	= jethelper[i].daughter_0_nConstituents;
      jethelper_daughter_1_energy[n]	= jethelper[i].daughter_1_energy;
      jethelper_daughter_1_pt[n]	= jethelper[i].daughter_1_pt;
      jethelper_daughter_1_eta[n]	= jethelper[i].daughter_1_eta;
      jethelper_daughter_1_phi[n]	= jethelper[i].daughter_1_phi;
      jethelper_daughter_1_mass[n]	= jethelper[i].daughter_1_mass;
      jethelper_daughter_1_chargedHadronEnergyFraction[n]	= jethelper[i].daughter_1_chargedHadronEnergyFraction;
      jethelper_daughter_1_neutralHadronEnergyFraction[n]	= jethelper[i].daughter_1_neutralHadronEnergyFraction;
      jethelper_daughter_1_chargedEmEnergyFraction[n]	= jethelper[i].daughter_1_chargedEmEnergyFraction;
      jethelper_daughter_1_neutralEmEnergyFraction[n]	= jethelper[i].daughter_1_neutralEmEnergyFraction;
      jethelper_daughter_1_photonEnergyFraction[n]	= jethelper[i].daughter_1_photonEnergyFraction;
      jethelper_daughter_1_muonEnergyFraction[n]	= jethelper[i].daughter_1_muonEnergyFraction;
      jethelper_daughter_1_chargedMultiplicity[n]	= jethelper[i].daughter_1_chargedMultiplicity;
      jethelper_daughter_1_nConstituents[n]	= jethelper[i].daughter_1_nConstituents;
      n++;
    }
  njethelper = n;

  n = 0;
  for(unsigned int i=0; i < jethelper2.size(); ++i)
    {
      if ( ! jethelper2[i].selected ) continue;
      jethelper2_energy[n]	= jethelper2[i].energy;
      jethelper2_uncor_energy[n]	= jethelper2[i].uncor_energy;
      jethelper2_et[n]	= jethelper2[i].et;
      jethelper2_uncor_et[n]	= jethelper2[i].uncor_et;
      jethelper2_pt[n]	= jethelper2[i].pt;
      jethelper2_uncor_pt[n]	= jethelper2[i].uncor_pt;
      jethelper2_phi[n]	= jethelper2[i].phi;
      jethelper2_eta[n]	= jethelper2[i].eta;
      jethelper2_mass[n]	= jethelper2[i].mass;
      jethelper2_jetArea[n]	= jethelper2[i].jetArea;
      jethelper2_chargedHadronEnergyFraction[n]	= jethelper2[i].chargedHadronEnergyFraction;
      jethelper2_neutralHadronEnergyFraction[n]	= jethelper2[i].neutralHadronEnergyFraction;
      jethelper2_chargedEmEnergyFraction[n]	= jethelper2[i].chargedEmEnergyFraction;
      jethelper2_neutralEmEnergyFraction[n]	= jethelper2[i].neutralEmEnergyFraction;
      jethelper2_photonEnergyFraction[n]	= jethelper2[i].photonEnergyFraction;
      jethelper2_muonEnergyFraction[n]	= jethelper2[i].muonEnergyFraction;
      jethelper2_chargedMultiplicity[n]	= jethelper2[i].chargedMultiplicity;
      jethelper2_nConstituents[n]	= jethelper2[i].nConstituents;
      jethelper2_genJet_energy[n]	= jethelper2[i].genJet_energy;
      jethelper2_genJet_pt[n]	= jethelper2[i].genJet_pt;
      jethelper2_genJet_phi[n]	= jethelper2[i].genJet_phi;
      jethelper2_genJet_eta[n]	= jethelper2[i].genJet_eta;
      jethelper2_genParton_pdgId[n]	= jethelper2[i].genParton_pdgId;
      jethelper2_partonFlavour[n]	= jethelper2[i].partonFlavour;
      jethelper2_trackCountingHighEffBJetTags[n]	= jethelper2[i].trackCountingHighEffBJetTags;
      jethelper2_trackCountingHighPurBJetTags[n]	= jethelper2[i].trackCountingHighPurBJetTags;
      jethelper2_jetProbabilityBJetTags[n]	= jethelper2[i].jetProbabilityBJetTags;
      jethelper2_jetBProbabilityBJetTags[n]	= jethelper2[i].jetBProbabilityBJetTags;
      jethelper2_simpleSecondaryVertexHighEffBJetTags[n]	= jethelper2[i].simpleSecondaryVertexHighEffBJetTags;
      jethelper2_simpleSecondaryVertexHighPurBJetTags[n]	= jethelper2[i].simpleSecondaryVertexHighPurBJetTags;
      jethelper2_combinedSecondaryVertexBJetTags[n]	= jethelper2[i].combinedSecondaryVertexBJetTags;
      jethelper2_combinedSecondaryVertexMVABJetTags[n]	= jethelper2[i].combinedSecondaryVertexMVABJetTags;
      n++;
    }
  njethelper2 = n;

  n = 0;
  for(unsigned int i=0; i < jethelper3.size(); ++i)
    {
      if ( ! jethelper3[i].selected ) continue;
      jethelper3_energy[n]	= jethelper3[i].energy;
      jethelper3_uncor_energy[n]	= jethelper3[i].uncor_energy;
      jethelper3_et[n]	= jethelper3[i].et;
      jethelper3_uncor_et[n]	= jethelper3[i].uncor_et;
      jethelper3_pt[n]	= jethelper3[i].pt;
      jethelper3_uncor_pt[n]	= jethelper3[i].uncor_pt;
      jethelper3_phi[n]	= jethelper3[i].phi;
      jethelper3_eta[n]	= jethelper3[i].eta;
      jethelper3_mass[n]	= jethelper3[i].mass;
      jethelper3_jetArea[n]	= jethelper3[i].jetArea;
      jethelper3_nConstituents[n]	= jethelper3[i].nConstituents;
      jethelper3_genJet_energy[n]	= jethelper3[i].genJet_energy;
      jethelper3_genJet_pt[n]	= jethelper3[i].genJet_pt;
      jethelper3_genJet_phi[n]	= jethelper3[i].genJet_phi;
      jethelper3_genJet_eta[n]	= jethelper3[i].genJet_eta;
      jethelper3_genParton_pdgId[n]	= jethelper3[i].genParton_pdgId;
      jethelper3_partonFlavour[n]	= jethelper3[i].partonFlavour;
      jethelper3_trackCountingHighEffBJetTags[n]	= jethelper3[i].trackCountingHighEffBJetTags;
      jethelper3_trackCountingHighPurBJetTags[n]	= jethelper3[i].trackCountingHighPurBJetTags;
      jethelper3_jetProbabilityBJetTags[n]	= jethelper3[i].jetProbabilityBJetTags;
      jethelper3_jetBProbabilityBJetTags[n]	= jethelper3[i].jetBProbabilityBJetTags;
      jethelper3_simpleSecondaryVertexHighEffBJetTags[n]	= jethelper3[i].simpleSecondaryVertexHighEffBJetTags;
      jethelper3_simpleSecondaryVertexHighPurBJetTags[n]	= jethelper3[i].simpleSecondaryVertexHighPurBJetTags;
      jethelper3_combinedSecondaryVertexBJetTags[n]	= jethelper3[i].combinedSecondaryVertexBJetTags;
      jethelper3_combinedSecondaryVertexMVABJetTags[n]	= jethelper3[i].combinedSecondaryVertexMVABJetTags;
      n++;
    }
  njethelper3 = n;

  n = 0;
  for(unsigned int i=0; i < muonhelper.size(); ++i)
    {
      if ( ! muonhelper[i].selected ) continue;
      muonhelper_energy[n]	= muonhelper[i].energy;
      muonhelper_et[n]	= muonhelper[i].et;
      muonhelper_pt[n]	= muonhelper[i].pt;
      muonhelper_phi[n]	= muonhelper[i].phi;
      muonhelper_eta[n]	= muonhelper[i].eta;
      muonhelper_charge[n]	= muonhelper[i].charge;
      muonhelper_TMOneStationTight[n]	= muonhelper[i].TMOneStationTight;
      muonhelper_isGlobalMuon[n]	= muonhelper[i].isGlobalMuon;
      muonhelper_isTrackerMuon[n]	= muonhelper[i].isTrackerMuon;
      muonhelper_isPFMuon[n]	= muonhelper[i].isPFMuon;
      muonhelper_track_hitPattern_trackerLayersWithMeasurement[n]	= muonhelper[i].track_hitPattern_trackerLayersWithMeasurement;
      muonhelper_innerTrack_hitPattern_pixelLayersWithMeasurement[n]	= muonhelper[i].innerTrack_hitPattern_pixelLayersWithMeasurement;
      muonhelper_innerTrack_normalizedChi2[n]	= muonhelper[i].innerTrack_normalizedChi2;
      muonhelper_globalTrack_normalizedChi2[n]	= muonhelper[i].globalTrack_normalizedChi2;
      muonhelper_globalTrack_hitPattern_numberOfValidMuonHits[n]	= muonhelper[i].globalTrack_hitPattern_numberOfValidMuonHits;
      muonhelper_numberOfMatchedStations[n]	= muonhelper[i].numberOfMatchedStations;
      muonhelper_dB[n]	= muonhelper[i].dB;
      muonhelper_innerTrack_hitPattern_numberOfValidPixelHits[n]	= muonhelper[i].innerTrack_hitPattern_numberOfValidPixelHits;
      muonhelper_pfIsolationR04_sumChargedHadronPt[n]	= muonhelper[i].pfIsolationR04_sumChargedHadronPt;
      muonhelper_pfIsolationR04_sumChargedParticlePt[n]	= muonhelper[i].pfIsolationR04_sumChargedParticlePt;
      muonhelper_pfIsolationR04_sumNeutralHadronEt[n]	= muonhelper[i].pfIsolationR04_sumNeutralHadronEt;
      muonhelper_pfIsolationR04_sumPhotonEt[n]	= muonhelper[i].pfIsolationR04_sumPhotonEt;
      muonhelper_pfIsolationR04_sumNeutralHadronEtHighThreshold[n]	= muonhelper[i].pfIsolationR04_sumNeutralHadronEtHighThreshold;
      muonhelper_pfIsolationR04_sumPhotonEtHighThreshold[n]	= muonhelper[i].pfIsolationR04_sumPhotonEtHighThreshold;
      muonhelper_pfIsolationR04_sumPUPt[n]	= muonhelper[i].pfIsolationR04_sumPUPt;
      muonhelper_dxywrtPV[n]	= muonhelper[i].dxywrtPV;
      muonhelper_dzwrtPV[n]	= muonhelper[i].dzwrtPV;
      n++;
    }
  nmuonhelper = n;

  n = 0;
  for(unsigned int i=0; i < tau.size(); ++i)
    {
      if ( ! tau[i].selected ) continue;
      tau_energy[n]	= tau[i].energy;
      tau_et[n]	= tau[i].et;
      tau_pt[n]	= tau[i].pt;
      tau_phi[n]	= tau[i].phi;
      tau_eta[n]	= tau[i].eta;
      tau_byLooseCombinedIsolationDeltaBetaCorr[n]	= tau[i].byLooseCombinedIsolationDeltaBetaCorr;
      tau_byMediumCombinedIsolationDeltaBetaCorr[n]	= tau[i].byMediumCombinedIsolationDeltaBetaCorr;
      tau_trackIso[n]	= tau[i].trackIso;
      tau_ecalIso[n]	= tau[i].ecalIso;
      tau_hcalIso[n]	= tau[i].hcalIso;
      tau_caloIso[n]	= tau[i].caloIso;
      n++;
    }
  ntau = n;

  n = 0;
  for(unsigned int i=0; i < vertex.size(); ++i)
    {
      if ( ! vertex[i].selected ) continue;
      vertex_isFake[n]	= vertex[i].isFake;
      vertex_chi2[n]	= vertex[i].chi2;
      vertex_ndof[n]	= vertex[i].ndof;
      vertex_x[n]	= vertex[i].x;
      vertex_y[n]	= vertex[i].y;
      vertex_z[n]	= vertex[i].z;
      vertex_xError[n]	= vertex[i].xError;
      vertex_yError[n]	= vertex[i].yError;
      vertex_zError[n]	= vertex[i].zError;
      vertex_position_Rho[n]	= vertex[i].position_Rho;
      n++;
    }
  nvertex = n;
  fillObjectsCalled = false;
}

//-----------------------------------------------------------------------------
// -- Select variables to be read
//-----------------------------------------------------------------------------
void selectVariables(itreestream& stream)
{
  stream.select("patElectronHelper_patElectronsWithTrigger.caloIso", electronhelper_caloIso);
  stream.select("patElectronHelper_patElectronsWithTrigger.charge", electronhelper_charge);
  stream.select("patElectronHelper_patElectronsWithTrigger.deltaEtaSuperClusterTrackAtVtx", electronhelper_deltaEtaSuperClusterTrackAtVtx);
  stream.select("patElectronHelper_patElectronsWithTrigger.deltaPhiSuperClusterTrackAtVtx", electronhelper_deltaPhiSuperClusterTrackAtVtx);
  stream.select("patElectronHelper_patElectronsWithTrigger.dxywrtPV", electronhelper_dxywrtPV);
  stream.select("patElectronHelper_patElectronsWithTrigger.dzwrtPV", electronhelper_dzwrtPV);
  stream.select("patElectronHelper_patElectronsWithTrigger.eSuperClusterOverP", electronhelper_eSuperClusterOverP);
  stream.select("patElectronHelper_patElectronsWithTrigger.ecalIso", electronhelper_ecalIso);
  stream.select("patElectronHelper_patElectronsWithTrigger.energy", electronhelper_energy);
  stream.select("patElectronHelper_patElectronsWithTrigger.et", electronhelper_et);
  stream.select("patElectronHelper_patElectronsWithTrigger.eta", electronhelper_eta);
  stream.select("patElectronHelper_patElectronsWithTrigger.gsfTrack_trackerExpectedHitsInner_numberOfHits", electronhelper_gsfTrack_trackerExpectedHitsInner_numberOfHits);
  stream.select("patElectronHelper_patElectronsWithTrigger.hadronicOverEm", electronhelper_hadronicOverEm);
  stream.select("patElectronHelper_patElectronsWithTrigger.hcalIso", electronhelper_hcalIso);
  stream.select("patElectronHelper_patElectronsWithTrigger.isEB", electronhelper_isEB);
  stream.select("patElectronHelper_patElectronsWithTrigger.isEE", electronhelper_isEE);
  stream.select("patElectronHelper_patElectronsWithTrigger.phi", electronhelper_phi);
  stream.select("patElectronHelper_patElectronsWithTrigger.pt", electronhelper_pt);
  stream.select("patElectronHelper_patElectronsWithTrigger.scSigmaIEtaIEta", electronhelper_scSigmaIEtaIEta);
  stream.select("patElectronHelper_patElectronsWithTrigger.simpleEleId80relIso", electronhelper_simpleEleId80relIso);
  stream.select("patElectronHelper_patElectronsWithTrigger.simpleEleId95relIso", electronhelper_simpleEleId95relIso);
  stream.select("patElectronHelper_patElectronsWithTrigger.superCluster_energy", electronhelper_superCluster_energy);
  stream.select("patElectronHelper_patElectronsWithTrigger.trackIso", electronhelper_trackIso);
  stream.select("edmEventHelper_info.bunchCrossing", eventhelper_bunchCrossing);
  stream.select("edmEventHelper_info.event", eventhelper_event);
  stream.select("edmEventHelper_info.isRealData", eventhelper_isRealData);
  stream.select("edmEventHelper_info.luminosityBlock", eventhelper_luminosityBlock);
  stream.select("edmEventHelper_info.orbitNumber", eventhelper_orbitNumber);
  stream.select("edmEventHelper_info.run", eventhelper_run);
  stream.select("edmEventHelperExtra_info.dijetCHS_invmass", eventhelperextra_dijetCHS_invmass);
  stream.select("edmEventHelperExtra_info.dijetCHSpruned_invmass", eventhelperextra_dijetCHSpruned_invmass);
  stream.select("edmEventHelperExtra_info.dijet_invmass", eventhelperextra_dijet_invmass);
  stream.select("edmEventHelperExtra_info.wj1CHS_energy", eventhelperextra_wj1CHS_energy);
  stream.select("edmEventHelperExtra_info.wj1CHS_eta", eventhelperextra_wj1CHS_eta);
  stream.select("edmEventHelperExtra_info.wj1CHS_mass", eventhelperextra_wj1CHS_mass);
  stream.select("edmEventHelperExtra_info.wj1CHS_nconst", eventhelperextra_wj1CHS_nconst);
  stream.select("edmEventHelperExtra_info.wj1CHS_phi", eventhelperextra_wj1CHS_phi);
  stream.select("edmEventHelperExtra_info.wj1CHS_pt", eventhelperextra_wj1CHS_pt);
  stream.select("edmEventHelperExtra_info.wj1_energy", eventhelperextra_wj1_energy);
  stream.select("edmEventHelperExtra_info.wj1_eta", eventhelperextra_wj1_eta);
  stream.select("edmEventHelperExtra_info.wj1_mass", eventhelperextra_wj1_mass);
  stream.select("edmEventHelperExtra_info.wj1_nconst", eventhelperextra_wj1_nconst);
  stream.select("edmEventHelperExtra_info.wj1_phi", eventhelperextra_wj1_phi);
  stream.select("edmEventHelperExtra_info.wj1_pt", eventhelperextra_wj1_pt);
  stream.select("edmEventHelperExtra_info.wj1wj2CHS_invmass", eventhelperextra_wj1wj2CHS_invmass);
  stream.select("edmEventHelperExtra_info.wj1wj2CHS_nconst", eventhelperextra_wj1wj2CHS_nconst);
  stream.select("edmEventHelperExtra_info.wj1wj2_invmass", eventhelperextra_wj1wj2_invmass);
  stream.select("edmEventHelperExtra_info.wj1wj2_nconst", eventhelperextra_wj1wj2_nconst);
  stream.select("edmEventHelperExtra_info.wj2CHS_energy", eventhelperextra_wj2CHS_energy);
  stream.select("edmEventHelperExtra_info.wj2CHS_eta", eventhelperextra_wj2CHS_eta);
  stream.select("edmEventHelperExtra_info.wj2CHS_mass", eventhelperextra_wj2CHS_mass);
  stream.select("edmEventHelperExtra_info.wj2CHS_nconst", eventhelperextra_wj2CHS_nconst);
  stream.select("edmEventHelperExtra_info.wj2CHS_phi", eventhelperextra_wj2CHS_phi);
  stream.select("edmEventHelperExtra_info.wj2CHS_pt", eventhelperextra_wj2CHS_pt);
  stream.select("edmEventHelperExtra_info.wj2_energy", eventhelperextra_wj2_energy);
  stream.select("edmEventHelperExtra_info.wj2_eta", eventhelperextra_wj2_eta);
  stream.select("edmEventHelperExtra_info.wj2_mass", eventhelperextra_wj2_mass);
  stream.select("edmEventHelperExtra_info.wj2_nconst", eventhelperextra_wj2_nconst);
  stream.select("edmEventHelperExtra_info.wj2_phi", eventhelperextra_wj2_phi);
  stream.select("edmEventHelperExtra_info.wj2_pt", eventhelperextra_wj2_pt);
  stream.select("GenEventInfoProduct_generator.alphaQCD", geneventinfoproduct_alphaQCD);
  stream.select("GenEventInfoProduct_generator.alphaQED", geneventinfoproduct_alphaQED);
  stream.select("GenEventInfoProduct_generator.hasBinningValues", geneventinfoproduct_hasBinningValues);
  stream.select("GenEventInfoProduct_generator.hasPDF", geneventinfoproduct_hasPDF);
  stream.select("GenEventInfoProduct_generator.qScale", geneventinfoproduct_qScale);
  stream.select("GenEventInfoProduct_generator.signalProcessID", geneventinfoproduct_signalProcessID);
  stream.select("GenEventInfoProduct_generator.weight", geneventinfoproduct_weight);
  stream.select("recoGenParticleHelper_genParticles.charge", genparticlehelper_charge);
  stream.select("recoGenParticleHelper_genParticles.eta", genparticlehelper_eta);
  stream.select("recoGenParticleHelper_genParticles.firstDaughter", genparticlehelper_firstDaughter);
  stream.select("recoGenParticleHelper_genParticles.firstMother", genparticlehelper_firstMother);
  stream.select("recoGenParticleHelper_genParticles.lastDaughter", genparticlehelper_lastDaughter);
  stream.select("recoGenParticleHelper_genParticles.lastMother", genparticlehelper_lastMother);
  stream.select("recoGenParticleHelper_genParticles.mass", genparticlehelper_mass);
  stream.select("recoGenParticleHelper_genParticles.pdgId", genparticlehelper_pdgId);
  stream.select("recoGenParticleHelper_genParticles.phi", genparticlehelper_phi);
  stream.select("recoGenParticleHelper_genParticles.pt", genparticlehelper_pt);
  stream.select("recoGenParticleHelper_genParticles.status", genparticlehelper_status);
  stream.select("GenRunInfoProduct_generator.crossSection", genruninfoproduct_crossSection);
  stream.select("GenRunInfoProduct_generator.filterEfficiency", genruninfoproduct_filterEfficiency);
  stream.select("patJetHelper_selectedPatJets.chargedEmEnergyFraction", jethelper2_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.chargedHadronEnergyFraction", jethelper2_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.chargedMultiplicity", jethelper2_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJets.combinedSecondaryVertexBJetTags", jethelper2_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJets.combinedSecondaryVertexMVABJetTags", jethelper2_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJets.energy", jethelper2_energy);
  stream.select("patJetHelper_selectedPatJets.et", jethelper2_et);
  stream.select("patJetHelper_selectedPatJets.eta", jethelper2_eta);
  stream.select("patJetHelper_selectedPatJets.genJet_energy", jethelper2_genJet_energy);
  stream.select("patJetHelper_selectedPatJets.genJet_eta", jethelper2_genJet_eta);
  stream.select("patJetHelper_selectedPatJets.genJet_phi", jethelper2_genJet_phi);
  stream.select("patJetHelper_selectedPatJets.genJet_pt", jethelper2_genJet_pt);
  stream.select("patJetHelper_selectedPatJets.genParton_pdgId", jethelper2_genParton_pdgId);
  stream.select("patJetHelper_selectedPatJets.jetArea", jethelper2_jetArea);
  stream.select("patJetHelper_selectedPatJets.jetBProbabilityBJetTags", jethelper2_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJets.jetProbabilityBJetTags", jethelper2_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJets.mass", jethelper2_mass);
  stream.select("patJetHelper_selectedPatJets.muonEnergyFraction", jethelper2_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.nConstituents", jethelper2_nConstituents);
  stream.select("patJetHelper_selectedPatJets.neutralEmEnergyFraction", jethelper2_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.neutralHadronEnergyFraction", jethelper2_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.partonFlavour", jethelper2_partonFlavour);
  stream.select("patJetHelper_selectedPatJets.phi", jethelper2_phi);
  stream.select("patJetHelper_selectedPatJets.photonEnergyFraction", jethelper2_photonEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.pt", jethelper2_pt);
  stream.select("patJetHelper_selectedPatJets.simpleSecondaryVertexHighEffBJetTags", jethelper2_simpleSecondaryVertexHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJets.simpleSecondaryVertexHighPurBJetTags", jethelper2_simpleSecondaryVertexHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJets.trackCountingHighEffBJetTags", jethelper2_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJets.trackCountingHighPurBJetTags", jethelper2_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJets.uncor_energy", jethelper2_uncor_energy);
  stream.select("patJetHelper_selectedPatJets.uncor_et", jethelper2_uncor_et);
  stream.select("patJetHelper_selectedPatJets.uncor_pt", jethelper2_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.combinedSecondaryVertexBJetTags", jethelper3_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.combinedSecondaryVertexMVABJetTags", jethelper3_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.energy", jethelper3_energy);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.et", jethelper3_et);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.eta", jethelper3_eta);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.genJet_energy", jethelper3_genJet_energy);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.genJet_eta", jethelper3_genJet_eta);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.genJet_phi", jethelper3_genJet_phi);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.genJet_pt", jethelper3_genJet_pt);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.genParton_pdgId", jethelper3_genParton_pdgId);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.jetArea", jethelper3_jetArea);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.jetBProbabilityBJetTags", jethelper3_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.jetProbabilityBJetTags", jethelper3_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.mass", jethelper3_mass);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.nConstituents", jethelper3_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.partonFlavour", jethelper3_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.phi", jethelper3_phi);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.pt", jethelper3_pt);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.simpleSecondaryVertexHighEffBJetTags", jethelper3_simpleSecondaryVertexHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.simpleSecondaryVertexHighPurBJetTags", jethelper3_simpleSecondaryVertexHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.trackCountingHighEffBJetTags", jethelper3_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.trackCountingHighPurBJetTags", jethelper3_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.uncor_energy", jethelper3_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.uncor_et", jethelper3_uncor_et);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.uncor_pt", jethelper3_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCHS.chargedEmEnergyFraction", jethelper_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.chargedHadronEnergyFraction", jethelper_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.chargedMultiplicity", jethelper_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCHS.combinedSecondaryVertexBJetTags", jethelper_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.combinedSecondaryVertexMVABJetTags", jethelper_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_chargedEmEnergyFraction", jethelper_daughter_0_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_chargedHadronEnergyFraction", jethelper_daughter_0_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_chargedMultiplicity", jethelper_daughter_0_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_energy", jethelper_daughter_0_energy);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_eta", jethelper_daughter_0_eta);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_mass", jethelper_daughter_0_mass);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_muonEnergyFraction", jethelper_daughter_0_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_nConstituents", jethelper_daughter_0_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_neutralEmEnergyFraction", jethelper_daughter_0_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_neutralHadronEnergyFraction", jethelper_daughter_0_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_phi", jethelper_daughter_0_phi);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_photonEnergyFraction", jethelper_daughter_0_photonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_0_pt", jethelper_daughter_0_pt);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_chargedEmEnergyFraction", jethelper_daughter_1_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_chargedHadronEnergyFraction", jethelper_daughter_1_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_chargedMultiplicity", jethelper_daughter_1_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_energy", jethelper_daughter_1_energy);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_eta", jethelper_daughter_1_eta);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_mass", jethelper_daughter_1_mass);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_muonEnergyFraction", jethelper_daughter_1_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_nConstituents", jethelper_daughter_1_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_neutralEmEnergyFraction", jethelper_daughter_1_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_neutralHadronEnergyFraction", jethelper_daughter_1_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_phi", jethelper_daughter_1_phi);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_photonEnergyFraction", jethelper_daughter_1_photonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.daughter_1_pt", jethelper_daughter_1_pt);
  stream.select("patJetHelper_selectedPatJetsCHS.energy", jethelper_energy);
  stream.select("patJetHelper_selectedPatJetsCHS.et", jethelper_et);
  stream.select("patJetHelper_selectedPatJetsCHS.eta", jethelper_eta);
  stream.select("patJetHelper_selectedPatJetsCHS.genJet_energy", jethelper_genJet_energy);
  stream.select("patJetHelper_selectedPatJetsCHS.genJet_eta", jethelper_genJet_eta);
  stream.select("patJetHelper_selectedPatJetsCHS.genJet_phi", jethelper_genJet_phi);
  stream.select("patJetHelper_selectedPatJetsCHS.genJet_pt", jethelper_genJet_pt);
  stream.select("patJetHelper_selectedPatJetsCHS.genParton_pdgId", jethelper_genParton_pdgId);
  stream.select("patJetHelper_selectedPatJetsCHS.jetArea", jethelper_jetArea);
  stream.select("patJetHelper_selectedPatJetsCHS.jetBProbabilityBJetTags", jethelper_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.jetProbabilityBJetTags", jethelper_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.mass", jethelper_mass);
  stream.select("patJetHelper_selectedPatJetsCHS.muonEnergyFraction", jethelper_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.nConstituents", jethelper_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCHS.neutralEmEnergyFraction", jethelper_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.neutralHadronEnergyFraction", jethelper_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.numberOfDaughters", jethelper_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsCHS.partonFlavour", jethelper_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsCHS.phi", jethelper_phi);
  stream.select("patJetHelper_selectedPatJetsCHS.photonEnergyFraction", jethelper_photonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.pt", jethelper_pt);
  stream.select("patJetHelper_selectedPatJetsCHS.simpleSecondaryVertexHighEffBJetTags", jethelper_simpleSecondaryVertexHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.simpleSecondaryVertexHighPurBJetTags", jethelper_simpleSecondaryVertexHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.trackCountingHighEffBJetTags", jethelper_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.trackCountingHighPurBJetTags", jethelper_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.uncor_energy", jethelper_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsCHS.uncor_et", jethelper_uncor_et);
  stream.select("patJetHelper_selectedPatJetsCHS.uncor_pt", jethelper_uncor_pt);
  stream.select("patMET_patMETsRaw.energy", met2_energy);
  stream.select("patMET_patMETsRaw.et", met2_et);
  stream.select("patMET_patMETsRaw.mEtSig", met2_mEtSig);
  stream.select("patMET_patMETsRaw.phi", met2_phi);
  stream.select("patMET_patMETsRaw.pt", met2_pt);
  stream.select("patMET_patMETsRaw.significance", met2_significance);
  stream.select("patMET_patMETsRaw.sumEt", met2_sumEt);
  stream.select("patMET_patMETs.energy", met_energy);
  stream.select("patMET_patMETs.et", met_et);
  stream.select("patMET_patMETs.mEtSig", met_mEtSig);
  stream.select("patMET_patMETs.phi", met_phi);
  stream.select("patMET_patMETs.pt", met_pt);
  stream.select("patMET_patMETs.significance", met_significance);
  stream.select("patMET_patMETs.sumEt", met_sumEt);
  stream.select("patMuonHelper_patMuonsWithTrigger.TMOneStationTight", muonhelper_TMOneStationTight);
  stream.select("patMuonHelper_patMuonsWithTrigger.charge", muonhelper_charge);
  stream.select("patMuonHelper_patMuonsWithTrigger.dB", muonhelper_dB);
  stream.select("patMuonHelper_patMuonsWithTrigger.dxywrtPV", muonhelper_dxywrtPV);
  stream.select("patMuonHelper_patMuonsWithTrigger.dzwrtPV", muonhelper_dzwrtPV);
  stream.select("patMuonHelper_patMuonsWithTrigger.energy", muonhelper_energy);
  stream.select("patMuonHelper_patMuonsWithTrigger.et", muonhelper_et);
  stream.select("patMuonHelper_patMuonsWithTrigger.eta", muonhelper_eta);
  stream.select("patMuonHelper_patMuonsWithTrigger.globalTrack_hitPattern_numberOfValidMuonHits", muonhelper_globalTrack_hitPattern_numberOfValidMuonHits);
  stream.select("patMuonHelper_patMuonsWithTrigger.globalTrack_normalizedChi2", muonhelper_globalTrack_normalizedChi2);
  stream.select("patMuonHelper_patMuonsWithTrigger.innerTrack_hitPattern_numberOfValidPixelHits", muonhelper_innerTrack_hitPattern_numberOfValidPixelHits);
  stream.select("patMuonHelper_patMuonsWithTrigger.innerTrack_hitPattern_pixelLayersWithMeasurement", muonhelper_innerTrack_hitPattern_pixelLayersWithMeasurement);
  stream.select("patMuonHelper_patMuonsWithTrigger.innerTrack_normalizedChi2", muonhelper_innerTrack_normalizedChi2);
  stream.select("patMuonHelper_patMuonsWithTrigger.isGlobalMuon", muonhelper_isGlobalMuon);
  stream.select("patMuonHelper_patMuonsWithTrigger.isPFMuon", muonhelper_isPFMuon);
  stream.select("patMuonHelper_patMuonsWithTrigger.isTrackerMuon", muonhelper_isTrackerMuon);
  stream.select("patMuonHelper_patMuonsWithTrigger.numberOfMatchedStations", muonhelper_numberOfMatchedStations);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumChargedHadronPt", muonhelper_pfIsolationR04_sumChargedHadronPt);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumChargedParticlePt", muonhelper_pfIsolationR04_sumChargedParticlePt);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumNeutralHadronEt", muonhelper_pfIsolationR04_sumNeutralHadronEt);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumNeutralHadronEtHighThreshold", muonhelper_pfIsolationR04_sumNeutralHadronEtHighThreshold);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumPUPt", muonhelper_pfIsolationR04_sumPUPt);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumPhotonEt", muonhelper_pfIsolationR04_sumPhotonEt);
  stream.select("patMuonHelper_patMuonsWithTrigger.pfIsolationR04_sumPhotonEtHighThreshold", muonhelper_pfIsolationR04_sumPhotonEtHighThreshold);
  stream.select("patMuonHelper_patMuonsWithTrigger.phi", muonhelper_phi);
  stream.select("patMuonHelper_patMuonsWithTrigger.pt", muonhelper_pt);
  stream.select("patMuonHelper_patMuonsWithTrigger.track_hitPattern_trackerLayersWithMeasurement", muonhelper_track_hitPattern_trackerLayersWithMeasurement);
  stream.select("npatElectronHelper_patElectronsWithTrigger", nelectronhelper);
  stream.select("nrecoGenParticleHelper_genParticles", ngenparticlehelper);
  stream.select("npatJetHelper_selectedPatJetsCHS", njethelper);
  stream.select("npatJetHelper_selectedPatJets", njethelper2);
  stream.select("npatJetHelper_selectedPatJetsCHSpruned", njethelper3);
  stream.select("npatMuonHelper_patMuonsWithTrigger", nmuonhelper);
  stream.select("npatTau_selectedPatTaus", ntau);
  stream.select("nrecoVertex_offlinePrimaryVertices", nvertex);
  stream.select("PileupSummaryInfo_addPileupInfo.getBunchCrossing", pileupsummaryinfo_getBunchCrossing);
  stream.select("PileupSummaryInfo_addPileupInfo.getPU_NumInteractions", pileupsummaryinfo_getPU_NumInteractions);
  stream.select("PileupSummaryInfo_addPileupInfo.getTrueNumInteractions", pileupsummaryinfo_getTrueNumInteractions);
  stream.select("sdouble_kt6PFJets_rho.value", sdouble_kt6PFJets_rho_value);
  stream.select("patTau_selectedPatTaus.byLooseCombinedIsolationDeltaBetaCorr", tau_byLooseCombinedIsolationDeltaBetaCorr);
  stream.select("patTau_selectedPatTaus.byMediumCombinedIsolationDeltaBetaCorr", tau_byMediumCombinedIsolationDeltaBetaCorr);
  stream.select("patTau_selectedPatTaus.caloIso", tau_caloIso);
  stream.select("patTau_selectedPatTaus.ecalIso", tau_ecalIso);
  stream.select("patTau_selectedPatTaus.energy", tau_energy);
  stream.select("patTau_selectedPatTaus.et", tau_et);
  stream.select("patTau_selectedPatTaus.eta", tau_eta);
  stream.select("patTau_selectedPatTaus.hcalIso", tau_hcalIso);
  stream.select("patTau_selectedPatTaus.phi", tau_phi);
  stream.select("patTau_selectedPatTaus.pt", tau_pt);
  stream.select("patTau_selectedPatTaus.trackIso", tau_trackIso);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.CSCTightHaloFilterPath", triggerresultshelper_CSCTightHaloFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.EcalDeadCellBoundaryEnergyFilterPath", triggerresultshelper_EcalDeadCellBoundaryEnergyFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.EcalDeadCellTriggerPrimitiveFilterPath", triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.HBHENoiseFilterPath", triggerresultshelper_HBHENoiseFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT200_v1", triggerresultshelper_HLT_HT200_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT200_v2", triggerresultshelper_HLT_HT200_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT200_v3", triggerresultshelper_HLT_HT200_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT250_v1", triggerresultshelper_HLT_HT250_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT250_v2", triggerresultshelper_HLT_HT250_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT250_v3", triggerresultshelper_HLT_HT250_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT300_v1", triggerresultshelper_HLT_HT300_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT300_v2", triggerresultshelper_HLT_HT300_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT300_v3", triggerresultshelper_HLT_HT300_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT350_v1", triggerresultshelper_HLT_HT350_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT350_v2", triggerresultshelper_HLT_HT350_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT350_v3", triggerresultshelper_HLT_HT350_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT400_v1", triggerresultshelper_HLT_HT400_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT400_v2", triggerresultshelper_HLT_HT400_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT400_v3", triggerresultshelper_HLT_HT400_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT450_v1", triggerresultshelper_HLT_HT450_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT450_v2", triggerresultshelper_HLT_HT450_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT450_v3", triggerresultshelper_HLT_HT450_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v1", triggerresultshelper_HLT_HT500_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v2", triggerresultshelper_HLT_HT500_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v3", triggerresultshelper_HLT_HT500_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v1", triggerresultshelper_HLT_HT550_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v2", triggerresultshelper_HLT_HT550_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v3", triggerresultshelper_HLT_HT550_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v1", triggerresultshelper_HLT_HT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v2", triggerresultshelper_HLT_HT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v3", triggerresultshelper_HLT_HT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v1", triggerresultshelper_HLT_HT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v2", triggerresultshelper_HLT_HT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v3", triggerresultshelper_HLT_HT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT350_v1", triggerresultshelper_HLT_PFHT350_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT350_v2", triggerresultshelper_HLT_PFHT350_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT350_v3", triggerresultshelper_HLT_PFHT350_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT350_v4", triggerresultshelper_HLT_PFHT350_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT350_v5", triggerresultshelper_HLT_PFHT350_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT350_v6", triggerresultshelper_HLT_PFHT350_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v1", triggerresultshelper_HLT_PFHT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v2", triggerresultshelper_HLT_PFHT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v3", triggerresultshelper_HLT_PFHT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v4", triggerresultshelper_HLT_PFHT650_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v5", triggerresultshelper_HLT_PFHT650_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v6", triggerresultshelper_HLT_PFHT650_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v7", triggerresultshelper_HLT_PFHT650_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v8", triggerresultshelper_HLT_PFHT650_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v1", triggerresultshelper_HLT_PFHT700_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v2", triggerresultshelper_HLT_PFHT700_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v3", triggerresultshelper_HLT_PFHT700_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v4", triggerresultshelper_HLT_PFHT700_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v5", triggerresultshelper_HLT_PFHT700_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v6", triggerresultshelper_HLT_PFHT700_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v1", triggerresultshelper_HLT_PFHT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v2", triggerresultshelper_HLT_PFHT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v3", triggerresultshelper_HLT_PFHT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v4", triggerresultshelper_HLT_PFHT750_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v5", triggerresultshelper_HLT_PFHT750_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v6", triggerresultshelper_HLT_PFHT750_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet140_v1", triggerresultshelper_HLT_PFJet140_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet140_v2", triggerresultshelper_HLT_PFJet140_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet140_v3", triggerresultshelper_HLT_PFJet140_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet200_v1", triggerresultshelper_HLT_PFJet200_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet200_v2", triggerresultshelper_HLT_PFJet200_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet200_v3", triggerresultshelper_HLT_PFJet200_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet260_v1", triggerresultshelper_HLT_PFJet260_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet260_v2", triggerresultshelper_HLT_PFJet260_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet260_v3", triggerresultshelper_HLT_PFJet260_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet320_v1", triggerresultshelper_HLT_PFJet320_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet320_v2", triggerresultshelper_HLT_PFJet320_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet320_v3", triggerresultshelper_HLT_PFJet320_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet320_v4", triggerresultshelper_HLT_PFJet320_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet320_v5", triggerresultshelper_HLT_PFJet320_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet400_v1", triggerresultshelper_HLT_PFJet400_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet400_v2", triggerresultshelper_HLT_PFJet400_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet400_v3", triggerresultshelper_HLT_PFJet400_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet400_v4", triggerresultshelper_HLT_PFJet400_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet400_v5", triggerresultshelper_HLT_PFJet400_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet40_v1", triggerresultshelper_HLT_PFJet40_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet40_v2", triggerresultshelper_HLT_PFJet40_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet40_v3", triggerresultshelper_HLT_PFJet40_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet80_v1", triggerresultshelper_HLT_PFJet80_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet80_v2", triggerresultshelper_HLT_PFJet80_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFJet80_v3", triggerresultshelper_HLT_PFJet80_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.greedyMuonPFCandidateFilterPath", triggerresultshelper_greedyMuonPFCandidateFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.hcalLaserEventFilterPath", triggerresultshelper_hcalLaserEventFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.hcalLaserFilterFromAODPath", triggerresultshelper_hcalLaserFilterFromAODPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.inconsistentMuonPFCandidateFilterPath", triggerresultshelper_inconsistentMuonPFCandidateFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.noscrapingFilterPath", triggerresultshelper_noscrapingFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1", triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2", triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3", triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4", triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5", triggerresultshelper_prescaleHLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT200_v1", triggerresultshelper_prescaleHLT_HT200_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT200_v2", triggerresultshelper_prescaleHLT_HT200_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT200_v3", triggerresultshelper_prescaleHLT_HT200_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT250_v1", triggerresultshelper_prescaleHLT_HT250_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT250_v2", triggerresultshelper_prescaleHLT_HT250_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT250_v3", triggerresultshelper_prescaleHLT_HT250_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT300_v1", triggerresultshelper_prescaleHLT_HT300_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT300_v2", triggerresultshelper_prescaleHLT_HT300_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT300_v3", triggerresultshelper_prescaleHLT_HT300_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT350_v1", triggerresultshelper_prescaleHLT_HT350_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT350_v2", triggerresultshelper_prescaleHLT_HT350_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT350_v3", triggerresultshelper_prescaleHLT_HT350_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT400_v1", triggerresultshelper_prescaleHLT_HT400_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT400_v2", triggerresultshelper_prescaleHLT_HT400_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT400_v3", triggerresultshelper_prescaleHLT_HT400_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT450_v1", triggerresultshelper_prescaleHLT_HT450_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT450_v2", triggerresultshelper_prescaleHLT_HT450_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT450_v3", triggerresultshelper_prescaleHLT_HT450_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT500_v1", triggerresultshelper_prescaleHLT_HT500_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT500_v2", triggerresultshelper_prescaleHLT_HT500_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT500_v3", triggerresultshelper_prescaleHLT_HT500_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT550_v1", triggerresultshelper_prescaleHLT_HT550_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT550_v2", triggerresultshelper_prescaleHLT_HT550_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT550_v3", triggerresultshelper_prescaleHLT_HT550_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT650_v1", triggerresultshelper_prescaleHLT_HT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT650_v2", triggerresultshelper_prescaleHLT_HT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT650_v3", triggerresultshelper_prescaleHLT_HT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT750_v1", triggerresultshelper_prescaleHLT_HT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT750_v2", triggerresultshelper_prescaleHLT_HT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_HT750_v3", triggerresultshelper_prescaleHLT_HT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT350_v1", triggerresultshelper_prescaleHLT_PFHT350_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT350_v2", triggerresultshelper_prescaleHLT_PFHT350_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT350_v3", triggerresultshelper_prescaleHLT_PFHT350_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT350_v4", triggerresultshelper_prescaleHLT_PFHT350_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT350_v5", triggerresultshelper_prescaleHLT_PFHT350_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT350_v6", triggerresultshelper_prescaleHLT_PFHT350_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v1", triggerresultshelper_prescaleHLT_PFHT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v2", triggerresultshelper_prescaleHLT_PFHT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v3", triggerresultshelper_prescaleHLT_PFHT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v4", triggerresultshelper_prescaleHLT_PFHT650_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v5", triggerresultshelper_prescaleHLT_PFHT650_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v6", triggerresultshelper_prescaleHLT_PFHT650_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v7", triggerresultshelper_prescaleHLT_PFHT650_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT650_v8", triggerresultshelper_prescaleHLT_PFHT650_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT700_v1", triggerresultshelper_prescaleHLT_PFHT700_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT700_v2", triggerresultshelper_prescaleHLT_PFHT700_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT700_v3", triggerresultshelper_prescaleHLT_PFHT700_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT700_v4", triggerresultshelper_prescaleHLT_PFHT700_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT700_v5", triggerresultshelper_prescaleHLT_PFHT700_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT700_v6", triggerresultshelper_prescaleHLT_PFHT700_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT750_v1", triggerresultshelper_prescaleHLT_PFHT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT750_v2", triggerresultshelper_prescaleHLT_PFHT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT750_v3", triggerresultshelper_prescaleHLT_PFHT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT750_v4", triggerresultshelper_prescaleHLT_PFHT750_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT750_v5", triggerresultshelper_prescaleHLT_PFHT750_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFHT750_v6", triggerresultshelper_prescaleHLT_PFHT750_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet140_v1", triggerresultshelper_prescaleHLT_PFJet140_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet140_v2", triggerresultshelper_prescaleHLT_PFJet140_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet140_v3", triggerresultshelper_prescaleHLT_PFJet140_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet200_v1", triggerresultshelper_prescaleHLT_PFJet200_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet200_v2", triggerresultshelper_prescaleHLT_PFJet200_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet200_v3", triggerresultshelper_prescaleHLT_PFJet200_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet260_v1", triggerresultshelper_prescaleHLT_PFJet260_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet260_v2", triggerresultshelper_prescaleHLT_PFJet260_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet260_v3", triggerresultshelper_prescaleHLT_PFJet260_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet320_v1", triggerresultshelper_prescaleHLT_PFJet320_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet320_v2", triggerresultshelper_prescaleHLT_PFJet320_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet320_v3", triggerresultshelper_prescaleHLT_PFJet320_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet320_v4", triggerresultshelper_prescaleHLT_PFJet320_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet320_v5", triggerresultshelper_prescaleHLT_PFJet320_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet400_v1", triggerresultshelper_prescaleHLT_PFJet400_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet400_v2", triggerresultshelper_prescaleHLT_PFJet400_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet400_v3", triggerresultshelper_prescaleHLT_PFJet400_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet400_v4", triggerresultshelper_prescaleHLT_PFJet400_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet400_v5", triggerresultshelper_prescaleHLT_PFJet400_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet40_v1", triggerresultshelper_prescaleHLT_PFJet40_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet40_v2", triggerresultshelper_prescaleHLT_PFJet40_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet40_v3", triggerresultshelper_prescaleHLT_PFJet40_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet80_v1", triggerresultshelper_prescaleHLT_PFJet80_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet80_v2", triggerresultshelper_prescaleHLT_PFJet80_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.prescaleHLT_PFJet80_v3", triggerresultshelper_prescaleHLT_PFJet80_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.primaryVertexFilterPath", triggerresultshelper_primaryVertexFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.simpleDRfilterPath", triggerresultshelper_simpleDRfilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.totalKinematicsFilterPath", triggerresultshelper_totalKinematicsFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.trackingFailureFilterPath", triggerresultshelper_trackingFailureFilterPath);
  stream.select("recoVertex_offlinePrimaryVertices.chi2", vertex_chi2);
  stream.select("recoVertex_offlinePrimaryVertices.isFake", vertex_isFake);
  stream.select("recoVertex_offlinePrimaryVertices.ndof", vertex_ndof);
  stream.select("recoVertex_offlinePrimaryVertices.position_Rho", vertex_position_Rho);
  stream.select("recoVertex_offlinePrimaryVertices.x", vertex_x);
  stream.select("recoVertex_offlinePrimaryVertices.xError", vertex_xError);
  stream.select("recoVertex_offlinePrimaryVertices.y", vertex_y);
  stream.select("recoVertex_offlinePrimaryVertices.yError", vertex_yError);
  stream.select("recoVertex_offlinePrimaryVertices.z", vertex_z);
  stream.select("recoVertex_offlinePrimaryVertices.zError", vertex_zError);

}
//-----------------------------------------------------------------------------
// -- Utilities
//-----------------------------------------------------------------------------
void
error(std::string message)
{
  std::cout << "** error ** " << message << std::endl;
  exit(0);
}

std::string 
strip(std::string line)
{
  int l = line.size();
  if ( l == 0 ) return std::string("");
  int n = 0;
  while (((line[n] == 0)    ||
	  (line[n] == ' ' ) ||
	  (line[n] == '\n') ||
	  (line[n] == '\t')) && n < l) n++;

  int m = l-1;
  while (((line[m] == 0)    ||
	  (line[m] == ' ')  ||
	  (line[m] == '\n') ||
	  (line[m] == '\t')) && m > 0) m--;
  return line.substr(n,m-n+1);
}

std::string
nameonly(std::string filename)
{
  int i = filename.rfind("/");
  int j = filename.rfind(".");
  if ( j < 0 ) j = filename.size();
  return filename.substr(i+1,j-i-1);
}
//-----------------------------------------------------------------------------
struct outputFile
{
  outputFile(std::string filename)
   : filename_(filename),
	 file_(new TFile(filename_.c_str(), "recreate")),
	 tree_(0),
	 b_weight_(0),
	 entry_(0),
	 SAVECOUNT_(50000)
  {
	file_->cd();
	hist_ = new TH1F("counts", "", 1,0,1);
	hist_->SetBit(TH1::kCanRebin);
	hist_->SetStats(0);
  }

  outputFile(std::string filename, itreestream& stream, int savecount=50000) 
   : filename_(filename),
	 file_(new TFile(filename.c_str(), "recreate")),
	 tree_(stream.tree()->CloneTree(0)),
	 b_weight_(tree_->Branch("eventWeight", &weight_, "eventWeight/D")),
	 entry_(0),
	 SAVECOUNT_(savecount)
  {
	std::cout << "events will be skimmed to file "
			  << filename_ << std::endl;
	file_->cd();
	hist_ = new TH1F("counts", "", 1,0,1);
	hist_->SetBit(TH1::kCanRebin);
	hist_->SetStats(0);
  }

  void addEvent(double weight=1)
  {
    if ( tree_ == 0 ) return;
	
    weight_ = weight;
	file_   = tree_->GetCurrentFile();
	file_->cd();
	tree_->Fill();

	entry_++;
	if ( entry_ % SAVECOUNT_ == 0 )
	  tree_->AutoSave("SaveSelf");
  }

  void count(std::string cond, int w=1)
  {
    hist_->Fill(cond.c_str(), w);
  }
  
  void close()
  {
  	std::cout << "==> histograms saved to file " << filename_ << std::endl;
    if ( tree_ != 0 )
	  {
	    std::cout << "==> events skimmed to file " << filename_ << std::endl;
	    file_ = tree_->GetCurrentFile();
	  }
	file_->cd();
	//file_->Write("", TObject::kWriteDelete);
	file_->Write();
	file_->ls();
	file_->Close();
  }

  std::string filename_;  
  TFile* file_;
  TTree* tree_;
  TH1F*  hist_;
  TBranch* b_weight_;
  double     weight_;
  int    entry_;
  int    SAVECOUNT_;
};

struct commandLine
{
  std::string progname;
  std::string filelist;
  std::string outputfilename;
};


void
decodeCommandLine(int argc, char** argv, commandLine& cl)
{
  cl.progname = std::string(argv[0]);

  // 1st (optional) argument
  if ( argc > 1 )
	cl.filelist = std::string(argv[1]);
  else
	cl.filelist = std::string("filelist.txt");

  // 2nd (optional) command line argument
  if ( argc > 2 ) 
	cl.outputfilename = std::string(argv[2]);
  else
	cl.outputfilename = cl.progname + std::string("_histograms");

  // Make sure extension is ".root"
  std::string name = cl.outputfilename;
  if ( name.substr(name.size()-5, 5) != std::string(".root") )
    cl.outputfilename += std::string(".root");
}

// Read ntuple filenames from file list

std::vector<std::string>
getFilenames(std::string filelist)
{
  std::ifstream stream(filelist.c_str());
  if ( !stream.good() ) error("unable to open file: " + filelist);

  // Get list of ntuple files to be processed

  std::vector<std::string> v;
  std::string filename;
  while ( stream >> filename )
	if ( strip(filename) != "" ) v.push_back(filename);
  return v;
}

#endif
