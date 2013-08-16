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
#include "TF1.h"
#include "TMath.h"
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
std::vector<float>	jethelperNoCHS_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelperNoCHS_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelperNoCHS_chargedMultiplicity(10,0);
std::vector<float>	jethelperNoCHS_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelperNoCHS_combinedSecondaryVertexMVABJetTags(10,0);
std::vector<double>	jethelperNoCHS_energy(10,0);
std::vector<double>	jethelperNoCHS_et(10,0);
std::vector<double>	jethelperNoCHS_eta(10,0);
std::vector<double>	jethelperNoCHS_rapidity(10,0);
std::vector<double>	jethelperNoCHS_genJet_energy(10,0);
std::vector<double>	jethelperNoCHS_genJet_eta(10,0);
std::vector<double>	jethelperNoCHS_genJet_phi(10,0);
std::vector<double>	jethelperNoCHS_genJet_pt(10,0);
std::vector<int>	jethelperNoCHS_genParton_pdgId(10,0);
std::vector<float>	jethelperNoCHS_jetArea(10,0);
std::vector<float>	jethelperNoCHS_jetBProbabilityBJetTags(10,0);
std::vector<float>	jethelperNoCHS_jetProbabilityBJetTags(10,0);
std::vector<double>	jethelperNoCHS_mass(10,0);
std::vector<float>	jethelperNoCHS_muonEnergyFraction(10,0);
std::vector<int>	jethelperNoCHS_nConstituents(10,0);
std::vector<float>	jethelperNoCHS_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelperNoCHS_neutralHadronEnergyFraction(10,0);
std::vector<int>	jethelperNoCHS_partonFlavour(10,0);
std::vector<double>	jethelperNoCHS_phi(10,0);
std::vector<float>	jethelperNoCHS_photonEnergyFraction(10,0);
std::vector<double>	jethelperNoCHS_pt(10,0);
std::vector<float>	jethelperNoCHS_simpleSecondaryVertexHighEffBJetTags(10,0);
std::vector<float>	jethelperNoCHS_simpleSecondaryVertexHighPurBJetTags(10,0);
std::vector<float>	jethelperNoCHS_trackCountingHighEffBJetTags(10,0);
std::vector<float>	jethelperNoCHS_trackCountingHighPurBJetTags(10,0);
std::vector<double>	jethelperNoCHS_uncor_energy(10,0);
std::vector<double>	jethelperNoCHS_uncor_et(10,0);
std::vector<double>	jethelperNoCHS_uncor_pt(10,0);

std::vector<float>	jethelperCA8_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelperCA8_combinedSecondaryVertexMVABJetTags(10,0);
std::vector<double>	jethelperCA8_energy(10,0);
std::vector<double>	jethelperCA8_et(10,0);
std::vector<double>	jethelperCA8_eta(10,0);
std::vector<double>	jethelperCA8_rapidity(10,0);
std::vector<double>	jethelperCA8_genJet_energy(10,0);
std::vector<double>	jethelperCA8_genJet_eta(10,0);
std::vector<double>	jethelperCA8_genJet_phi(10,0);
std::vector<double>	jethelperCA8_genJet_pt(10,0);
std::vector<int>	jethelperCA8_genParton_pdgId(10,0);
std::vector<float>	jethelperCA8_jetArea(10,0);
std::vector<float>	jethelperCA8_jetBProbabilityBJetTags(10,0);
std::vector<float>	jethelperCA8_jetProbabilityBJetTags(10,0);
std::vector<double>	jethelperCA8_mass(10,0);
std::vector<int>	jethelperCA8_nConstituents(10,0);
std::vector<int>	jethelperCA8_partonFlavour(10,0);
std::vector<float>	jethelperCA8_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelperCA8_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelperCA8_chargedMultiplicity(10,0);
std::vector<float>	jethelperCA8_muonEnergyFraction(10,0);
std::vector<float>	jethelperCA8_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelperCA8_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelperCA8_phi(10,0);
std::vector<double>	jethelperCA8_pt(10,0);
std::vector<float>	jethelperCA8_simpleSecondaryVertexHighEffBJetTags(10,0);
std::vector<float>	jethelperCA8_simpleSecondaryVertexHighPurBJetTags(10,0);
std::vector<float>	jethelperCA8_trackCountingHighEffBJetTags(10,0);
std::vector<float>	jethelperCA8_trackCountingHighPurBJetTags(10,0);
std::vector<double>	jethelperCA8_uncor_energy(10,0);
std::vector<double>	jethelperCA8_uncor_et(10,0);
std::vector<double>	jethelperCA8_uncor_pt(10,0);
std::vector<double>	jethelperCA8_tau1(10,0);
std::vector<double>	jethelperCA8_tau2(10,0);
std::vector<double>	jethelperCA8_C2beta17(10,0);
std::vector<double>	jethelperCA8pruned_mass(10,0);
std::vector<double>	jethelperCA8pruned_pt(10,0);
std::vector<double>	jethelperCA8pruned_uncor_pt(10,0);
std::vector<size_t>	jethelperCA8pruned_numberOfDaughters(10,0);
std::vector<float>	jethelperCA8pruned_daughter_0_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelperCA8pruned_daughter_0_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelperCA8pruned_daughter_0_chargedMultiplicity(10,0);
std::vector<double>	jethelperCA8pruned_daughter_0_energy(10,0);
std::vector<double>	jethelperCA8pruned_daughter_0_eta(10,0);
std::vector<double>	jethelperCA8pruned_daughter_0_mass(10,0);
std::vector<float>	jethelperCA8pruned_daughter_0_muonEnergyFraction(10,0);
std::vector<int>	jethelperCA8pruned_daughter_0_nConstituents(10,0);
std::vector<float>	jethelperCA8pruned_daughter_0_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelperCA8pruned_daughter_0_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelperCA8pruned_daughter_0_phi(10,0);
std::vector<float>	jethelperCA8pruned_daughter_0_photonEnergyFraction(10,0);
std::vector<double>	jethelperCA8pruned_daughter_0_pt(10,0);
std::vector<float>	jethelperCA8pruned_daughter_1_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelperCA8pruned_daughter_1_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelperCA8pruned_daughter_1_chargedMultiplicity(10,0);
std::vector<double>	jethelperCA8pruned_daughter_1_energy(10,0);
std::vector<double>	jethelperCA8pruned_daughter_1_eta(10,0);
std::vector<double>	jethelperCA8pruned_daughter_1_mass(10,0);
std::vector<float>	jethelperCA8pruned_daughter_1_muonEnergyFraction(10,0);
std::vector<int>	jethelperCA8pruned_daughter_1_nConstituents(10,0);
std::vector<float>	jethelperCA8pruned_daughter_1_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelperCA8pruned_daughter_1_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelperCA8pruned_daughter_1_phi(10,0);
std::vector<float>	jethelperCA8pruned_daughter_1_photonEnergyFraction(10,0);
std::vector<double>	jethelperCA8pruned_daughter_1_pt(10,0);

std::vector<float>	jethelperAK7_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelperAK7_combinedSecondaryVertexMVABJetTags(10,0);
std::vector<double>	jethelperAK7_energy(10,0);
std::vector<double>	jethelperAK7_et(10,0);
std::vector<double>	jethelperAK7_eta(10,0);
std::vector<double>	jethelperAK7_rapidity(10,0);
std::vector<double>	jethelperAK7_genJet_energy(10,0);
std::vector<double>	jethelperAK7_genJet_eta(10,0);
std::vector<double>	jethelperAK7_genJet_phi(10,0);
std::vector<double>	jethelperAK7_genJet_pt(10,0);
std::vector<int>	jethelperAK7_genParton_pdgId(10,0);
std::vector<float>	jethelperAK7_jetArea(10,0);
std::vector<float>	jethelperAK7_jetBProbabilityBJetTags(10,0);
std::vector<float>	jethelperAK7_jetProbabilityBJetTags(10,0);
std::vector<double>	jethelperAK7_mass(10,0);
std::vector<int>	jethelperAK7_nConstituents(10,0);
std::vector<int>	jethelperAK7_partonFlavour(10,0);
std::vector<double>	jethelperAK7_phi(10,0);
std::vector<double>	jethelperAK7_pt(10,0);
std::vector<float>	jethelperAK7_simpleSecondaryVertexHighEffBJetTags(10,0);
std::vector<float>	jethelperAK7_simpleSecondaryVertexHighPurBJetTags(10,0);
std::vector<float>	jethelperAK7_trackCountingHighEffBJetTags(10,0);
std::vector<float>	jethelperAK7_trackCountingHighPurBJetTags(10,0);
std::vector<double>	jethelperAK7_uncor_energy(10,0);
std::vector<double>	jethelperAK7_uncor_et(10,0);
std::vector<double>	jethelperAK7_uncor_pt(10,0);
std::vector<double>	jethelperAK7_tau1(10,0);
std::vector<double>	jethelperAK7_tau2(10,0);
std::vector<double>	jethelperAK7_C2beta17(10,0);
std::vector<double>	jethelperAK7pruned_mass(10,0);
std::vector<double>	jethelperAK7pruned_pt(10,0);
std::vector<double>	jethelperAK7pruned_uncor_pt(10,0);
std::vector<size_t>	jethelperAK7pruned_numberOfDaughters(10,0);
std::vector<float>	jethelperAK7pruned_daughter_0_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelperAK7pruned_daughter_0_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelperAK7pruned_daughter_0_chargedMultiplicity(10,0);
std::vector<double>	jethelperAK7pruned_daughter_0_energy(10,0);
std::vector<double>	jethelperAK7pruned_daughter_0_eta(10,0);
std::vector<double>	jethelperAK7pruned_daughter_0_mass(10,0);
std::vector<float>	jethelperAK7pruned_daughter_0_muonEnergyFraction(10,0);
std::vector<int>	jethelperAK7pruned_daughter_0_nConstituents(10,0);
std::vector<float>	jethelperAK7pruned_daughter_0_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelperAK7pruned_daughter_0_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelperAK7pruned_daughter_0_phi(10,0);
std::vector<float>	jethelperAK7pruned_daughter_0_photonEnergyFraction(10,0);
std::vector<double>	jethelperAK7pruned_daughter_0_pt(10,0);
std::vector<float>	jethelperAK7pruned_daughter_1_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelperAK7pruned_daughter_1_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelperAK7pruned_daughter_1_chargedMultiplicity(10,0);
std::vector<double>	jethelperAK7pruned_daughter_1_energy(10,0);
std::vector<double>	jethelperAK7pruned_daughter_1_eta(10,0);
std::vector<double>	jethelperAK7pruned_daughter_1_mass(10,0);
std::vector<float>	jethelperAK7pruned_daughter_1_muonEnergyFraction(10,0);
std::vector<int>	jethelperAK7pruned_daughter_1_nConstituents(10,0);
std::vector<float>	jethelperAK7pruned_daughter_1_neutralEmEnergyFraction(10,0);
std::vector<float>	jethelperAK7pruned_daughter_1_neutralHadronEnergyFraction(10,0);
std::vector<double>	jethelperAK7pruned_daughter_1_phi(10,0);
std::vector<float>	jethelperAK7pruned_daughter_1_photonEnergyFraction(10,0);
std::vector<double>	jethelperAK7pruned_daughter_1_pt(10,0);

std::vector<double>	jethelperCMG_energy(10,0);
std::vector<double>	jethelperCMG_pt(10,0);
std::vector<double>	jethelperCMG_eta(10,0);
std::vector<double>	jethelperCMG_rapidity(10,0);
std::vector<double>	jethelperCMG_phi(10,0);
std::vector<double>	jethelperCMG_mass(10,0);
std::vector<float>	jethelper_chargedEmEnergyFraction(10,0);
std::vector<float>	jethelper_chargedHadronEnergyFraction(10,0);
std::vector<int>	jethelper_chargedMultiplicity(10,0);
std::vector<float>	jethelper_combinedSecondaryVertexBJetTags(10,0);
std::vector<float>	jethelper_combinedSecondaryVertexMVABJetTags(10,0);
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
int	njethelperNoCHS;
int	njethelperCA8pruned;
int	njethelperAK7pruned;
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
int	triggerresultshelper_hcallasereventfilter2012;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4;
int	triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5;
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
int	triggerresultshelper_HLT_HT750_v1;
int	triggerresultshelper_HLT_HT750_v2;
int	triggerresultshelper_HLT_HT750_v3;
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
int	triggerresultshelper_HLT_PFHT700_v1;
int	triggerresultshelper_HLT_PFHT700_v2;
int	triggerresultshelper_HLT_PFHT700_v3;
int	triggerresultshelper_HLT_PFHT700_v4;
int	triggerresultshelper_HLT_PFHT700_v5;
int	triggerresultshelper_HLT_PFHT700_v6;
int	triggerresultshelper_HLT_PFHT750_v1;
int	triggerresultshelper_HLT_PFHT750_v2;
int	triggerresultshelper_HLT_PFHT750_v3;
int	triggerresultshelper_HLT_PFHT750_v4;
int	triggerresultshelper_HLT_PFHT750_v5;
int	triggerresultshelper_HLT_PFHT750_v6;
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
