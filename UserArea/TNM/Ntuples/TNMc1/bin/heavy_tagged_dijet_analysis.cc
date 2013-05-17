//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     Thu May 31 21:49:25 2012 by mkntanalyzer.py
// Author:      Andreas Hinzmann
//-----------------------------------------------------------------------------
#include "heavy_tagged_dijet_analysis.h"

#ifdef PROJECT_NAME
#include "PhysicsTools/TheNtupleMaker/interface/pdg.h"
#else
#include "pdg.h"
#endif

#include "TLorentzVector.h"

using namespace std;

Double_t fnc_dscb(Double_t *xx, Double_t *pp)
{
  Double_t x   = xx[0];
  Double_t N   = pp[0];
  Double_t mu  = pp[1];
  Double_t sig = pp[2];
  Double_t a1  = pp[3];
  Double_t p1  = pp[4];
  Double_t a2  = pp[5];
  Double_t p2  = pp[6];

  Double_t u   = (x-mu)/sig;
  Double_t A1  = TMath::Power(p1/TMath::Abs(a1),p1)*TMath::Exp(-a1*a1/2);
  Double_t A2  = TMath::Power(p2/TMath::Abs(a2),p2)*TMath::Exp(-a2*a2/2);
  Double_t B1  = p1/TMath::Abs(a1) - TMath::Abs(a1);
  Double_t B2  = p2/TMath::Abs(a2) - TMath::Abs(a2);

  Double_t result=N;
  if (u<-a1)
      result *= A1*TMath::Power(B1-u,-p1);
  else if (u<a2)
      result *= TMath::Exp(-u*u/2);
  else
      result *= A2*TMath::Power(B2+u,-p2);
  return result;
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  cout << "Start program" << endl;

  commandLine cmdline;
  decodeCommandLine(argc, argv, cmdline);

  // Get names of ntuple files to be processed and open chain of ntuples

  vector<string> filenames = getFilenames(cmdline.filelist);
  cout << "Open file stream" << endl;
  itreestream stream(filenames, "Events");
  if ( !stream.good() ) error("unable to open ntuple file(s)");

  // Get number of events to be read

  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;

  // Select variables to be read

  //selectVariables(stream);

  stream.select("edmEventHelper_info.bunchCrossing", eventhelper_bunchCrossing);
  stream.select("edmEventHelper_info.event", eventhelper_event);
  stream.select("edmEventHelper_info.isRealData", eventhelper_isRealData);
  stream.select("edmEventHelper_info.luminosityBlock", eventhelper_luminosityBlock);
  stream.select("edmEventHelper_info.orbitNumber", eventhelper_orbitNumber);
  stream.select("edmEventHelper_info.run", eventhelper_run);
  stream.select("edmEventHelperExtra_info.dijetCHS_invmass", eventhelperextra_dijetCHS_invmass);
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
  stream.select("GenEventInfoProduct_generator.weight", geneventinfoproduct_weight);
  
  stream.select("patJetHelper_patJetsWithVar.chargedEmEnergyFraction", jethelperNoCHS_chargedEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.chargedHadronEnergyFraction", jethelperNoCHS_chargedHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.chargedMultiplicity", jethelperNoCHS_chargedMultiplicity);
  stream.select("patJetHelper_patJetsWithVar.energy", jethelperNoCHS_energy);
  stream.select("patJetHelper_patJetsWithVar.eta", jethelperNoCHS_eta);
  stream.select("patJetHelper_patJetsWithVar.rapidity", jethelperNoCHS_rapidity);
  stream.select("patJetHelper_patJetsWithVar.mass", jethelperNoCHS_mass);
  stream.select("patJetHelper_patJetsWithVar.muonEnergyFraction", jethelperNoCHS_muonEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.nConstituents", jethelperNoCHS_nConstituents);
  stream.select("patJetHelper_patJetsWithVar.neutralEmEnergyFraction", jethelperNoCHS_neutralEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.neutralHadronEnergyFraction", jethelperNoCHS_neutralHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVar.phi", jethelperNoCHS_phi);
  stream.select("patJetHelper_patJetsWithVar.pt", jethelperNoCHS_pt);
  stream.select("patJetHelper_patJetsWithVarCHS.chargedEmEnergyFraction", jethelper_chargedEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.chargedHadronEnergyFraction", jethelper_chargedHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.chargedMultiplicity", jethelper_chargedMultiplicity);
  stream.select("patJetHelper_patJetsWithVarCHS.combinedSecondaryVertexBJetTags", jethelper_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.combinedSecondaryVertexMVABJetTags", jethelper_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.energy", jethelper_energy);
  stream.select("patJetHelper_patJetsWithVarCHS.eta", jethelper_eta);
  stream.select("patJetHelper_patJetsWithVarCHS.rapidity", jethelper_rapidity);
  stream.select("patJetHelper_patJetsWithVarCHS.jetBProbabilityBJetTags", jethelper_jetBProbabilityBJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.jetProbabilityBJetTags", jethelper_jetProbabilityBJetTags);
  stream.select("patJetHelper_patJetsWithVarCHS.mass", jethelper_mass);
  stream.select("patJetHelper_patJetsWithVarCHS.muonEnergyFraction", jethelper_muonEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.nConstituents", jethelper_nConstituents);
  stream.select("patJetHelper_patJetsWithVarCHS.neutralEmEnergyFraction", jethelper_neutralEmEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.neutralHadronEnergyFraction", jethelper_neutralHadronEnergyFraction);
  stream.select("patJetHelper_patJetsWithVarCHS.phi", jethelper_phi);
  stream.select("patJetHelper_patJetsWithVarCHS.pt", jethelper_pt);
  
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.combinedSecondaryVertexBJetTags", jethelperCA8_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.combinedSecondaryVertexMVABJetTags", jethelperCA8_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.energy", jethelperCA8_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.et", jethelperCA8_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.eta", jethelperCA8_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetArea", jethelperCA8_jetArea);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetBProbabilityBJetTags", jethelperCA8_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.jetProbabilityBJetTags", jethelperCA8_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.mass", jethelperCA8_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.nConstituents", jethelperCA8_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.partonFlavour", jethelperCA8_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.chargedEmEnergyFraction", jethelperCA8_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.chargedHadronEnergyFraction", jethelperCA8_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.chargedMultiplicity", jethelperCA8_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.muonEnergyFraction", jethelperCA8_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.neutralEmEnergyFraction", jethelperCA8_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.neutralHadronEnergyFraction", jethelperCA8_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.phi", jethelperCA8_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.pt", jethelperCA8_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.trackCountingHighEffBJetTags", jethelperCA8_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.trackCountingHighPurBJetTags", jethelperCA8_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_energy", jethelperCA8_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_et", jethelperCA8_uncor_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_pt", jethelperCA8_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.tau1", jethelperCA8_tau1);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.tau2", jethelperCA8_tau2);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.C2beta17", jethelperCA8_C2beta17);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.mass", jethelperCA8pruned_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.pt", jethelperCA8pruned_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.uncor_pt", jethelperCA8pruned_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.numberOfDaughters", jethelperCA8pruned_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_energy", jethelperCA8pruned_daughter_0_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_eta", jethelperCA8pruned_daughter_0_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_mass", jethelperCA8pruned_daughter_0_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_phi", jethelperCA8pruned_daughter_0_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_pt", jethelperCA8pruned_daughter_0_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_energy", jethelperCA8pruned_daughter_1_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_eta", jethelperCA8pruned_daughter_1_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_mass", jethelperCA8pruned_daughter_1_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_phi", jethelperCA8pruned_daughter_1_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_pt", jethelperCA8pruned_daughter_1_pt);

  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.combinedSecondaryVertexBJetTags", jethelperAK7_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.combinedSecondaryVertexMVABJetTags", jethelperAK7_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.energy", jethelperAK7_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.et", jethelperAK7_et);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.eta", jethelperAK7_eta);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.jetArea", jethelperAK7_jetArea);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.jetBProbabilityBJetTags", jethelperAK7_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.jetProbabilityBJetTags", jethelperAK7_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.mass", jethelperAK7_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.nConstituents", jethelperAK7_nConstituents);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.partonFlavour", jethelperAK7_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.phi", jethelperAK7_phi);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.pt", jethelperAK7_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.trackCountingHighEffBJetTags", jethelperAK7_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.trackCountingHighPurBJetTags", jethelperAK7_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.uncor_energy", jethelperAK7_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.uncor_et", jethelperAK7_uncor_et);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.uncor_pt", jethelperAK7_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.tau1", jethelperAK7_tau1);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.tau2", jethelperAK7_tau2);
  stream.select("patJetHelper_selectedPatJetsAK7CHSwithQjets.C2beta17", jethelperAK7_C2beta17);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.mass", jethelperAK7pruned_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.pt", jethelperAK7pruned_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.uncor_pt", jethelperAK7pruned_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.numberOfDaughters", jethelperAK7pruned_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_energy", jethelperAK7pruned_daughter_0_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_eta", jethelperAK7pruned_daughter_0_eta);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_mass", jethelperAK7pruned_daughter_0_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_phi", jethelperAK7pruned_daughter_0_phi);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_0_pt", jethelperAK7pruned_daughter_0_pt);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_energy", jethelperAK7pruned_daughter_1_energy);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_eta", jethelperAK7pruned_daughter_1_eta);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_mass", jethelperAK7pruned_daughter_1_mass);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_phi", jethelperAK7pruned_daughter_1_phi);
  stream.select("patJetHelper_selectedPatJetsAK7CHSpruned.daughter_1_pt", jethelperAK7pruned_daughter_1_pt);

  stream.select("patMET_patMETsRaw.et", met2_et);
  stream.select("patMET_patMETsRaw.sumEt", met2_sumEt);
  stream.select("nrecoVertex_offlinePrimaryVertices", nvertex);
  stream.select("PileupSummaryInfo_addPileupInfo.getBunchCrossing", pileupsummaryinfo_getBunchCrossing);
  stream.select("PileupSummaryInfo_addPileupInfo.getPU_NumInteractions", pileupsummaryinfo_getPU_NumInteractions);
  stream.select("PileupSummaryInfo_addPileupInfo.getTrueNumInteractions", pileupsummaryinfo_getTrueNumInteractions);
  stream.select("sdouble_kt6PFJets_rho.value", sdouble_kt6PFJets_rho_value);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5);
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
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.hcalLaserEventFilterPath", triggerresultshelper_hcalLaserEventFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.noscrapingFilterPath", triggerresultshelper_noscrapingFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.primaryVertexFilterPath", triggerresultshelper_primaryVertexFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.trackingFailureFilterPath", triggerresultshelper_trackingFailureFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.CSCTightHaloFilterPath", triggerresultshelper_CSCTightHaloFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.EcalDeadCellBoundaryEnergyFilterPath", triggerresultshelper_EcalDeadCellBoundaryEnergyFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.EcalDeadCellTriggerPrimitiveFilterPath", triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.HBHENoiseFilterPath", triggerresultshelper_HBHENoiseFilterPath);
  stream.select("sint_hcallasereventfilter2012.value", triggerresultshelper_hcallasereventfilter2012);

  stream.select("nrecoGenParticleHelper_genParticles", ngenparticlehelper);
  stream.select("recoGenParticleHelper_genParticles.firstDaughter", genparticlehelper_firstDaughter);
  stream.select("recoGenParticleHelper_genParticles.firstMother", genparticlehelper_firstMother);
  stream.select("recoGenParticleHelper_genParticles.lastDaughter", genparticlehelper_lastDaughter);
  stream.select("recoGenParticleHelper_genParticles.lastMother", genparticlehelper_lastMother);
  stream.select("recoGenParticleHelper_genParticles.pdgId", genparticlehelper_pdgId);
/*
  stream.select("cmgPFJet_cmgPFJetSel.energy", jethelperCMG_energy);
  stream.select("cmgPFJet_cmgPFJetSel.eta", jethelperCMG_eta);
  stream.select("cmgPFJet_cmgPFJetSel.rapidity", jethelperCMG_rapidity);
  stream.select("cmgPFJet_cmgPFJetSel.mass", jethelperCMG_mass);
  stream.select("cmgPFJet_cmgPFJetSel.phi", jethelperCMG_phi);
  stream.select("cmgPFJet_cmgPFJetSel.pt", jethelperCMG_pt);
*/
  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the following
  // line

  TApplication app("analyzer", &argc, argv);

  /*
	 Notes:
	
	 1. Use
	   ofile = outputFile(cmdline.outputfile, stream)
	
	 to skim events to output file in addition to writing out histograms.
	
	 2. Use
	   ofile.addEvent(event-weight)
	
	 to specify that the current event is to be added to the output file.
	 If omitted, the event-weight is defaulted to 1.
	
	 3. Use
	    ofile.count(cut-name, event-weight)
	
	 to keep track, in the count histogram, of the number of events
	 passing a given cut. If omitted, the event-weight is taken to be 1.
	 If you want the counts in the count histogram to appear in a given
	 order, specify the order, before entering the event loop, as in
	 the example below
	 
	    ofile.count("NoCuts", 0)
		ofile.count("GoodEvent", 0)
		ofile.count("Vertex", 0)
		ofile.count("MET", 0)
  */
  
  outputFile ofile(cmdline.outputfilename);

  //---------------------------------------------------------------------------
  // Declare histograms
  //---------------------------------------------------------------------------

  const int NBINS = 104;
  Double_t BOUNDARIES[NBINS] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325,
  354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687,
  1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509,
  4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7060, 7320, 7589, 7866, 8152, 8447, 8752, 9067, 9391, 9726, 10072, 10430, 
  10798, 11179, 11571, 11977, 12395, 12827, 13272, 13732, 14000};
/*  
  TH1F* mass_0Wtag=new TH1F("dijet_mass_0Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_0Wtag->Sumw2();
  TH1F* mass_1Wtag=new TH1F("dijet_mass_1Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Wtag->Sumw2();
  TH1F* mass_2Wtag=new TH1F("dijet_mass_2Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Wtag->Sumw2();
  TH1F* massNoCHS_2Wtag=new TH1F("dijet_massNoCHS_2Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  massNoCHS_2Wtag->Sumw2();
  TH1F* massCA8_2Wtag=new TH1F("dijet_massCA8_2Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  massCA8_2Wtag->Sumw2();
  TH1F* mass_2Nsubtag04=new TH1F("dijet_mass_2Nsubtag04","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Nsubtag04->Sumw2();
  TH1F* mass_2Nsubtag05=new TH1F("dijet_mass_2Nsubtag05","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Nsubtag05->Sumw2();
  TH1F* mass_2Nsubtag06=new TH1F("dijet_mass_2Nsubtag06","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Nsubtag06->Sumw2();
  TH1F* mass_1Nsubtag04=new TH1F("dijet_mass_1Nsubtag04","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Nsubtag04->Sumw2();
  TH1F* mass_1Nsubtag05=new TH1F("dijet_mass_1Nsubtag05","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Nsubtag05->Sumw2();
  TH1F* mass_1Nsubtag06=new TH1F("dijet_mass_1Nsubtag06","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Nsubtag06->Sumw2();
  TH1F* mass_2Nsubtag0406=new TH1F("dijet_mass_2Nsubtag0406","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Nsubtag0406->Sumw2();
  TH1F* mass_0Htag=new TH1F("dijet_mass_0Htag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_0Htag->Sumw2();
  TH1F* mass_1Htag=new TH1F("dijet_mass_1Htag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Htag->Sumw2();
  TH1F* mass_2Htag=new TH1F("dijet_mass_2Htag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Htag->Sumw2();
  TH1F* mass_0Wtag04=new TH1F("dijet_mass_0Wtag04","M_{jj}",NBINS-1,BOUNDARIES);
  mass_0Wtag04->Sumw2();
  TH1F* mass_1Wtag04=new TH1F("dijet_mass_1Wtag04","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Wtag04->Sumw2();
  TH1F* mass_2Wtag04=new TH1F("dijet_mass_2Wtag04","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Wtag04->Sumw2();
  TH1F* mass_0Mtag=new TH1F("dijet_mass_0Mtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_0Mtag->Sumw2();
  TH1F* mass_1Mtag=new TH1F("dijet_mass_1Mtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_1Mtag->Sumw2();
  TH1F* mass_2Mtag=new TH1F("dijet_mass_2Mtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Mtag->Sumw2();

  TH1F* mass_0mtag=new TH1F("dijet_mass_0mtag","M_{jj}",8000,0,8000);
  mass_0mtag->Sumw2();
  TH1F* mass_1mtag_0mdtag=new TH1F("dijet_mass_1mtag_0mdtag","M_{jj}",8000,0,8000);
  mass_1mtag_0mdtag->Sumw2();
  TH1F* mass_1mtag_1mdtag=new TH1F("dijet_mass_1mtag_1mdtag","M_{jj}",8000,0,8000);
  mass_1mtag_1mdtag->Sumw2();
  TH1F* mass_2mtag_0mdtag=new TH1F("dijet_mass_2mtag_0mdtag","M_{jj}",8000,0,8000);
  mass_2mtag_0mdtag->Sumw2();
  TH1F* mass_2mtag_1mdtag=new TH1F("dijet_mass_2mtag_1mdtag","M_{jj}",8000,0,8000);
  mass_2mtag_1mdtag->Sumw2();
  TH1F* mass_2mtag_2mdtag=new TH1F("dijet_mass_2mtag_2mdtag","M_{jj}",8000,0,8000);
  mass_2mtag_2mdtag->Sumw2();

  TH2F* mass_eta_0mtag=new TH2F("dijet_mass_eta_0mtag","M_{jj}",7000,0,7000,20,0,2);
  mass_eta_0mtag->Sumw2();
  TH2F* mass_eta_1mtag_0mdtag=new TH2F("dijet_mass_eta_1mtag_0mdtag","M_{jj}",7000,0,7000,20,0,2);
  mass_eta_1mtag_0mdtag->Sumw2();
  TH2F* mass_eta_1mtag_1mdtag=new TH2F("dijet_mass_eta_1mtag_1mdtag","M_{jj}",7000,0,7000,20,0,2);
  mass_eta_1mtag_1mdtag->Sumw2();
  TH2F* mass_eta_2mtag_0mdtag=new TH2F("dijet_mass_eta_2mtag_0mdtag","M_{jj}",7000,0,7000,20,0,2);
  mass_eta_2mtag_0mdtag->Sumw2();
  TH2F* mass_eta_2mtag_1mdtag=new TH2F("dijet_mass_eta_2mtag_1mdtag","M_{jj}",7000,0,7000,20,0,2);
  mass_eta_2mtag_1mdtag->Sumw2();
  TH2F* mass_eta_2mtag_2mdtag=new TH2F("dijet_mass_eta_2mtag_2mdtag","M_{jj}",7000,0,7000,20,0,2);
  mass_eta_2mtag_2mdtag->Sumw2();

  TH1F* mass_0mak7tag=new TH1F("dijet_mass_0mak7tag","M_{jj}",8000,0,8000);
  mass_0mak7tag->Sumw2();
  TH1F* mass_1mak7tag_0mdtag=new TH1F("dijet_mass_1mak7tag_0mdtag","M_{jj}",8000,0,8000);
  mass_1mak7tag_0mdtag->Sumw2();
  TH1F* mass_1mak7tag_1mdtag=new TH1F("dijet_mass_1mak7tag_1mdtag","M_{jj}",8000,0,8000);
  mass_1mak7tag_1mdtag->Sumw2();
  TH1F* mass_2mak7tag_0mdtag=new TH1F("dijet_mass_2mak7tag_0mdtag","M_{jj}",8000,0,8000);
  mass_2mak7tag_0mdtag->Sumw2();
  TH1F* mass_2mak7tag_1mdtag=new TH1F("dijet_mass_2mak7tag_1mdtag","M_{jj}",8000,0,8000);
  mass_2mak7tag_1mdtag->Sumw2();
  TH1F* mass_2mak7tag_2mdtag=new TH1F("dijet_mass_2mak7tag_2mdtag","M_{jj}",8000,0,8000);
  mass_2mak7tag_2mdtag->Sumw2();

  TH1F* mass_0smtag=new TH1F("dijet_mass_0smtag","M_{jj}",8000,0,8000);
  mass_0smtag->Sumw2();
  TH1F* mass_1smtag_0mdtag=new TH1F("dijet_mass_1smtag_0mdtag","M_{jj}",8000,0,8000);
  mass_1smtag_0mdtag->Sumw2();
  TH1F* mass_1smtag_1mdtag=new TH1F("dijet_mass_1smtag_1mdtag","M_{jj}",8000,0,8000);
  mass_1smtag_1mdtag->Sumw2();
  TH1F* mass_2smtag_0mdtag=new TH1F("dijet_mass_2smtag_0mdtag","M_{jj}",8000,0,8000);
  mass_2smtag_0mdtag->Sumw2();
  TH1F* mass_2smtag_1mdtag=new TH1F("dijet_mass_2smtag_1mdtag","M_{jj}",8000,0,8000);
  mass_2smtag_1mdtag->Sumw2();
  TH1F* mass_2smtag_2mdtag=new TH1F("dijet_mass_2smtag_2mdtag","M_{jj}",8000,0,8000);
  mass_2smtag_2mdtag->Sumw2();

  TH1F* mass_0smak7tag=new TH1F("dijet_mass_0smak7tag","M_{jj}",8000,0,8000);
  mass_0smak7tag->Sumw2();
  TH1F* mass_1smak7tag_0mdtag=new TH1F("dijet_mass_1smak7tag_0mdtag","M_{jj}",8000,0,8000);
  mass_1smak7tag_0mdtag->Sumw2();
  TH1F* mass_1smak7tag_1mdtag=new TH1F("dijet_mass_1smak7tag_1mdtag","M_{jj}",8000,0,8000);
  mass_1smak7tag_1mdtag->Sumw2();
  TH1F* mass_2smak7tag_0mdtag=new TH1F("dijet_mass_2smak7tag_0mdtag","M_{jj}",8000,0,8000);
  mass_2smak7tag_0mdtag->Sumw2();
  TH1F* mass_2smak7tag_1mdtag=new TH1F("dijet_mass_2smak7tag_1mdtag","M_{jj}",8000,0,8000);
  mass_2smak7tag_1mdtag->Sumw2();
  TH1F* mass_2smak7tag_2mdtag=new TH1F("dijet_mass_2smak7tag_2mdtag","M_{jj}",8000,0,8000);
  mass_2smak7tag_2mdtag->Sumw2();

  TH1F* mass_0ssmtag=new TH1F("dijet_mass_0ssmtag","M_{jj}",8000,0,8000);
  mass_0ssmtag->Sumw2();
  TH1F* mass_1ssmtag_0mdtag=new TH1F("dijet_mass_1ssmtag_0mdtag","M_{jj}",8000,0,8000);
  mass_1ssmtag_0mdtag->Sumw2();
  TH1F* mass_1ssmtag_1mdtag=new TH1F("dijet_mass_1ssmtag_1mdtag","M_{jj}",8000,0,8000);
  mass_1ssmtag_1mdtag->Sumw2();
  TH1F* mass_2ssmtag_0mdtag=new TH1F("dijet_mass_2ssmtag_0mdtag","M_{jj}",8000,0,8000);
  mass_2ssmtag_0mdtag->Sumw2();
  TH1F* mass_2ssmtag_1mdtag=new TH1F("dijet_mass_2ssmtag_1mdtag","M_{jj}",8000,0,8000);
  mass_2ssmtag_1mdtag->Sumw2();
  TH1F* mass_2ssmtag_2mdtag=new TH1F("dijet_mass_2ssmtag_2mdtag","M_{jj}",8000,0,8000);
  mass_2ssmtag_2mdtag->Sumw2();

  TH1F* mass_0ssmak7tag=new TH1F("dijet_mass_0ssmak7tag","M_{jj}",8000,0,8000);
  mass_0ssmak7tag->Sumw2();
  TH1F* mass_1ssmak7tag_0mdtag=new TH1F("dijet_mass_1ssmak7tag_0mdtag","M_{jj}",8000,0,8000);
  mass_1ssmak7tag_0mdtag->Sumw2();
  TH1F* mass_1ssmak7tag_1mdtag=new TH1F("dijet_mass_1ssmak7tag_1mdtag","M_{jj}",8000,0,8000);
  mass_1ssmak7tag_1mdtag->Sumw2();
  TH1F* mass_2ssmak7tag_0mdtag=new TH1F("dijet_mass_2ssmak7tag_0mdtag","M_{jj}",8000,0,8000);
  mass_2ssmak7tag_0mdtag->Sumw2();
  TH1F* mass_2ssmak7tag_1mdtag=new TH1F("dijet_mass_2ssmak7tag_1mdtag","M_{jj}",8000,0,8000);
  mass_2ssmak7tag_1mdtag->Sumw2();
  TH1F* mass_2ssmak7tag_2mdtag=new TH1F("dijet_mass_2ssmak7tag_2mdtag","M_{jj}",8000,0,8000);
  mass_2ssmak7tag_2mdtag->Sumw2();

  //TH1F* mass_0mtag=new TH1F("dijet_mass_0mtag","M_{jj}",8000,0,8000);
  //mass_0mtag->Sumw2();
  TH1F* mass_1mtag_0nstag=new TH1F("dijet_mass_1mtag_0nstag","M_{jj}",8000,0,8000);
  mass_1mtag_0nstag->Sumw2();
  TH1F* mass_1mtag_1nstag=new TH1F("dijet_mass_1mtag_1nstag","M_{jj}",8000,0,8000);
  mass_1mtag_1nstag->Sumw2();
  TH1F* mass_2mtag_0nstag=new TH1F("dijet_mass_2mtag_0nstag","M_{jj}",8000,0,8000);
  mass_2mtag_0nstag->Sumw2();
  TH1F* mass_2mtag_1nstag=new TH1F("dijet_mass_2mtag_1nstag","M_{jj}",8000,0,8000);
  mass_2mtag_1nstag->Sumw2();
  TH1F* mass_2mtag_2nstag=new TH1F("dijet_mass_2mtag_2nstag","M_{jj}",8000,0,8000);
  mass_2mtag_2nstag->Sumw2();

  //TH1F* mass_0mak7tag=new TH1F("dijet_mass_0mak7tag","M_{jj}",8000,0,8000);
  //mass_0mak7tag->Sumw2();
  TH1F* mass_1mak7tag_0nstag=new TH1F("dijet_mass_1mak7tag_0nstag","M_{jj}",8000,0,8000);
  mass_1mak7tag_0nstag->Sumw2();
  TH1F* mass_1mak7tag_1nstag=new TH1F("dijet_mass_1mak7tag_1nstag","M_{jj}",8000,0,8000);
  mass_1mak7tag_1nstag->Sumw2();
  TH1F* mass_2mak7tag_0nstag=new TH1F("dijet_mass_2mak7tag_0nstag","M_{jj}",8000,0,8000);
  mass_2mak7tag_0nstag->Sumw2();
  TH1F* mass_2mak7tag_1nstag=new TH1F("dijet_mass_2mak7tag_1nstag","M_{jj}",8000,0,8000);
  mass_2mak7tag_1nstag->Sumw2();
  TH1F* mass_2mak7tag_2nstag=new TH1F("dijet_mass_2mak7tag_2nstag","M_{jj}",8000,0,8000);
  mass_2mak7tag_2nstag->Sumw2();

  //TH1F* mass_0mtag=new TH1F("dijet_mass_0mtag","M_{jj}",8000,0,8000);
  //mass_0mtag->Sumw2();
  TH1F* mass_1mtag_0nsltag=new TH1F("dijet_mass_1mtag_0nsltag","M_{jj}",8000,0,8000);
  mass_1mtag_0nsltag->Sumw2();
  TH1F* mass_1mtag_1nsltag=new TH1F("dijet_mass_1mtag_1nsltag","M_{jj}",8000,0,8000);
  mass_1mtag_1nsltag->Sumw2();
  TH1F* mass_2mtag_0nsltag=new TH1F("dijet_mass_2mtag_0nsltag","M_{jj}",8000,0,8000);
  mass_2mtag_0nsltag->Sumw2();
  TH1F* mass_2mtag_1nsltag=new TH1F("dijet_mass_2mtag_1nsltag","M_{jj}",8000,0,8000);
  mass_2mtag_1nsltag->Sumw2();
  TH1F* mass_2mtag_2nsltag=new TH1F("dijet_mass_2mtag_2nsltag","M_{jj}",8000,0,8000);
  mass_2mtag_2nsltag->Sumw2();

  //TH1F* mass_0smtag=new TH1F("dijet_mass_0smtag","M_{jj}",8000,0,8000);
  //mass_0smtag->Sumw2();
  TH1F* mass_1smtag_0nstag=new TH1F("dijet_mass_1smtag_0nstag","M_{jj}",8000,0,8000);
  mass_1smtag_0nstag->Sumw2();
  TH1F* mass_1smtag_1nstag=new TH1F("dijet_mass_1smtag_1nstag","M_{jj}",8000,0,8000);
  mass_1smtag_1nstag->Sumw2();
  TH1F* mass_2smtag_0nstag=new TH1F("dijet_mass_2smtag_0nstag","M_{jj}",8000,0,8000);
  mass_2smtag_0nstag->Sumw2();
  TH1F* mass_2smtag_1nstag=new TH1F("dijet_mass_2smtag_1nstag","M_{jj}",8000,0,8000);
  mass_2smtag_1nstag->Sumw2();
  TH1F* mass_2smtag_2nstag=new TH1F("dijet_mass_2smtag_2nstag","M_{jj}",8000,0,8000);
  mass_2smtag_2nstag->Sumw2();

  //TH1F* mass_0smak7tag=new TH1F("dijet_mass_0smak7tag","M_{jj}",8000,0,8000);
  //mass_0smak7tag->Sumw2();
  TH1F* mass_1smak7tag_0nstag=new TH1F("dijet_mass_1smak7tag_0nstag","M_{jj}",8000,0,8000);
  mass_1smak7tag_0nstag->Sumw2();
  TH1F* mass_1smak7tag_1nstag=new TH1F("dijet_mass_1smak7tag_1nstag","M_{jj}",8000,0,8000);
  mass_1smak7tag_1nstag->Sumw2();
  TH1F* mass_2smak7tag_0nstag=new TH1F("dijet_mass_2smak7tag_0nstag","M_{jj}",8000,0,8000);
  mass_2smak7tag_0nstag->Sumw2();
  TH1F* mass_2smak7tag_1nstag=new TH1F("dijet_mass_2smak7tag_1nstag","M_{jj}",8000,0,8000);
  mass_2smak7tag_1nstag->Sumw2();
  TH1F* mass_2smak7tag_2nstag=new TH1F("dijet_mass_2smak7tag_2nstag","M_{jj}",8000,0,8000);
  mass_2smak7tag_2nstag->Sumw2();

  TH1F* mass_0mtag_1smdtag=new TH1F("dijet_mass_0mtag_1smdtag","M_{jj}",8000,0,8000);
  mass_0mtag_1smdtag->Sumw2();
  TH1F* mass_1mtag_1smdtag=new TH1F("dijet_mass_1mtag_1smdtag","M_{jj}",8000,0,8000);
  mass_1mtag_1smdtag->Sumw2();
  TH1F* mass_2mtag_1smdtag=new TH1F("dijet_mass_2mtag_1smdtag","M_{jj}",8000,0,8000);
  mass_2mtag_1smdtag->Sumw2();

  TH1F* mass8_0mtag=new TH1F("dijet_mass8_0mtag","M_{jj}",8000,0,8000);
  mass8_0mtag->Sumw2();
  TH1F* mass8_1mtag_0mdtag=new TH1F("dijet_mass8_1mtag_0mdtag","M_{jj}",8000,0,8000);
  mass8_1mtag_0mdtag->Sumw2();
  TH1F* mass8_1mtag_1mdtag=new TH1F("dijet_mass8_1mtag_1mdtag","M_{jj}",8000,0,8000);
  mass8_1mtag_1mdtag->Sumw2();
  TH1F* mass8_2mtag_0mdtag=new TH1F("dijet_mass8_2mtag_0mdtag","M_{jj}",8000,0,8000);
  mass8_2mtag_0mdtag->Sumw2();
  TH1F* mass8_2mtag_1mdtag=new TH1F("dijet_mass8_2mtag_1mdtag","M_{jj}",8000,0,8000);
  mass8_2mtag_1mdtag->Sumw2();
  TH1F* mass8_2mtag_2mdtag=new TH1F("dijet_mass8_2mtag_2mdtag","M_{jj}",8000,0,8000);
  mass8_2mtag_2mdtag->Sumw2();

  TH1F* mass8_0smtag=new TH1F("dijet_mass8_0smtag","M_{jj}",8000,0,8000);
  mass8_0smtag->Sumw2();
  TH1F* mass8_1smtag_0mdtag=new TH1F("dijet_mass8_1smtag_0mdtag","M_{jj}",8000,0,8000);
  mass8_1smtag_0mdtag->Sumw2();
  TH1F* mass8_1smtag_1mdtag=new TH1F("dijet_mass8_1smtag_1mdtag","M_{jj}",8000,0,8000);
  mass8_1smtag_1mdtag->Sumw2();
  TH1F* mass8_2smtag_0mdtag=new TH1F("dijet_mass8_2smtag_0mdtag","M_{jj}",8000,0,8000);
  mass8_2smtag_0mdtag->Sumw2();
  TH1F* mass8_2smtag_1mdtag=new TH1F("dijet_mass8_2smtag_1mdtag","M_{jj}",8000,0,8000);
  mass8_2smtag_1mdtag->Sumw2();
  TH1F* mass8_2smtag_2mdtag=new TH1F("dijet_mass8_2smtag_2mdtag","M_{jj}",8000,0,8000);
  mass8_2smtag_2mdtag->Sumw2();

  TH1F* mass7_0mtag=new TH1F("dijet_mass7_0mtag","M_{jj}",8000,0,8000);
  mass7_0mtag->Sumw2();
  TH1F* mass7_1mtag_0mdtag=new TH1F("dijet_mass7_1mtag_0mdtag","M_{jj}",8000,0,8000);
  mass7_1mtag_0mdtag->Sumw2();
  TH1F* mass7_1mtag_1mdtag=new TH1F("dijet_mass7_1mtag_1mdtag","M_{jj}",8000,0,8000);
  mass7_1mtag_1mdtag->Sumw2();
  TH1F* mass7_2mtag_0mdtag=new TH1F("dijet_mass7_2mtag_0mdtag","M_{jj}",8000,0,8000);
  mass7_2mtag_0mdtag->Sumw2();
  TH1F* mass7_2mtag_1mdtag=new TH1F("dijet_mass7_2mtag_1mdtag","M_{jj}",8000,0,8000);
  mass7_2mtag_1mdtag->Sumw2();
  TH1F* mass7_2mtag_2mdtag=new TH1F("dijet_mass7_2mtag_2mdtag","M_{jj}",8000,0,8000);
  mass7_2mtag_2mdtag->Sumw2();

  TH1F* mass7_0smtag=new TH1F("dijet_mass7_0smtag","M_{jj}",8000,0,8000);
  mass7_0smtag->Sumw2();
  TH1F* mass7_1smtag_0mdtag=new TH1F("dijet_mass7_1smtag_0mdtag","M_{jj}",8000,0,8000);
  mass7_1smtag_0mdtag->Sumw2();
  TH1F* mass7_1smtag_1mdtag=new TH1F("dijet_mass7_1smtag_1mdtag","M_{jj}",8000,0,8000);
  mass7_1smtag_1mdtag->Sumw2();
  TH1F* mass7_2smtag_0mdtag=new TH1F("dijet_mass7_2smtag_0mdtag","M_{jj}",8000,0,8000);
  mass7_2smtag_0mdtag->Sumw2();
  TH1F* mass7_2smtag_1mdtag=new TH1F("dijet_mass7_2smtag_1mdtag","M_{jj}",8000,0,8000);
  mass7_2smtag_1mdtag->Sumw2();
  TH1F* mass7_2smtag_2mdtag=new TH1F("dijet_mass7_2smtag_2mdtag","M_{jj}",8000,0,8000);
  mass7_2smtag_2mdtag->Sumw2();

  //TH1F* mass_0mtag=new TH1F("dijet_mass_0mtag","M_{jj}",8000,0,8000);
  //mass_0mtag->Sumw2();
  TH1F* mass_1mtag_0mdltag=new TH1F("dijet_mass_1mtag_0mdltag","M_{jj}",8000,0,8000);
  mass_1mtag_0mdltag->Sumw2();
  TH1F* mass_1mtag_1mdltag=new TH1F("dijet_mass_1mtag_1mdltag","M_{jj}",8000,0,8000);
  mass_1mtag_1mdltag->Sumw2();
  TH1F* mass_2mtag_0mdltag=new TH1F("dijet_mass_2mtag_0mdltag","M_{jj}",8000,0,8000);
  mass_2mtag_0mdltag->Sumw2();
  TH1F* mass_2mtag_1mdltag=new TH1F("dijet_mass_2mtag_1mdltag","M_{jj}",8000,0,8000);
  mass_2mtag_1mdltag->Sumw2();
  TH1F* mass_2mtag_1mdtag_1mdltag=new TH1F("dijet_mass_2mtag_1mdtag_1mdltag","M_{jj}",8000,0,8000);
  mass_2mtag_1mdtag_1mdltag->Sumw2();

  TH1F* jet1_massdrop_0mtag=new TH1F("jet1_massdrop_0mtag","massdrop",100,0,1);
  jet1_massdrop_0mtag->Sumw2();
  TH1F* jet2_massdrop_0mtag=new TH1F("jet2_massdrop_0mtag","massdrop",100,0,1);
  jet2_massdrop_0mtag->Sumw2();
  TH1F* jet1_massdrop_1mtag=new TH1F("jet1_massdrop_1mtag","massdrop",100,0,1);
  jet1_massdrop_1mtag->Sumw2();
  TH1F* jet2_massdrop_1mtag=new TH1F("jet2_massdrop_1mtag","massdrop",100,0,1);
  jet2_massdrop_1mtag->Sumw2();
  TH1F* jet1_massdrop_2mtag=new TH1F("jet1_massdrop_2mtag","massdrop",100,0,1);
  jet1_massdrop_2mtag->Sumw2();
  TH1F* jet2_massdrop_2mtag=new TH1F("jet2_massdrop_2mtag","massdrop",100,0,1);
  jet2_massdrop_2mtag->Sumw2();

  TH1F* jet1_mass_0mdtag=new TH1F("jet1_mass_0mdtag","m_{jet}",1000,0,1000);
  jet1_mass_0mdtag->Sumw2();
  TH1F* jet2_mass_0mdtag=new TH1F("jet2_mass_0mdtag","m_{jet}",1000,0,1000);
  jet2_mass_0mdtag->Sumw2();
  TH1F* jet1_mass_1mdtag=new TH1F("jet1_mass_1mdtag","m_{jet}",1000,0,1000);
  jet1_mass_1mdtag->Sumw2();
  TH1F* jet2_mass_1mdtag=new TH1F("jet2_mass_1mdtag","m_{jet}",1000,0,1000);
  jet2_mass_1mdtag->Sumw2();
  TH1F* jet1_mass_2mdtag=new TH1F("jet1_mass_2mdtag","m_{jet}",1000,0,1000);
  jet1_mass_2mdtag->Sumw2();
  TH1F* jet2_mass_2mdtag=new TH1F("jet2_mass_2mdtag","m_{jet}",1000,0,1000);
  jet2_mass_2mdtag->Sumw2();

  TH1F* njets_0mtag=new TH1F("njets_0mtag","M_{jj}",50,0,50);
  njets_0mtag->Sumw2();
  TH1F* njets_1mtag_0mdtag=new TH1F("njets_1mtag_0mdtag","M_{jj}",50,0,50);
  njets_1mtag_0mdtag->Sumw2();
  TH1F* njets_1mtag_1mdtag=new TH1F("njets_1mtag_1mdtag","M_{jj}",50,0,50);
  njets_1mtag_1mdtag->Sumw2();
  TH1F* njets_2mtag_0mdtag=new TH1F("njets_2mtag_0mdtag","M_{jj}",50,0,50);
  njets_2mtag_0mdtag->Sumw2();
  TH1F* njets_2mtag_1mdtag=new TH1F("njets_2mtag_1mdtag","M_{jj}",50,0,50);
  njets_2mtag_1mdtag->Sumw2();
  TH1F* njets_2mtag_2mdtag=new TH1F("njets_2mtag_2mdtag","M_{jj}",50,0,50);
  njets_2mtag_2mdtag->Sumw2();

  TH1F* njetsCA8_0mtag=new TH1F("njetsCA8_0mtag","M_{jj}",50,0,50);
  njetsCA8_0mtag->Sumw2();
  TH1F* njetsCA8_1mtag_0mdtag=new TH1F("njetsCA8_1mtag_0mdtag","M_{jj}",50,0,50);
  njetsCA8_1mtag_0mdtag->Sumw2();
  TH1F* njetsCA8_1mtag_1mdtag=new TH1F("njetsCA8_1mtag_1mdtag","M_{jj}",50,0,50);
  njetsCA8_1mtag_1mdtag->Sumw2();
  TH1F* njetsCA8_2mtag_0mdtag=new TH1F("njetsCA8_2mtag_0mdtag","M_{jj}",50,0,50);
  njetsCA8_2mtag_0mdtag->Sumw2();
  TH1F* njetsCA8_2mtag_1mdtag=new TH1F("njetsCA8_2mtag_1mdtag","M_{jj}",50,0,50);
  njetsCA8_2mtag_1mdtag->Sumw2();
  TH1F* njetsCA8_2mtag_2mdtag=new TH1F("njetsCA8_2mtag_2mdtag","M_{jj}",50,0,50);
  njetsCA8_2mtag_2mdtag->Sumw2();

  TH1F* deta_0mtag=new TH1F("deta_0mtag","M_{jj}",1000,0,10);
  deta_0mtag->Sumw2();
  TH1F* deta_1mtag_0mdtag=new TH1F("deta_1mtag_0mdtag","M_{jj}",200,0,2);
  deta_1mtag_0mdtag->Sumw2();
  TH1F* deta_1mtag_1mdtag=new TH1F("deta_1mtag_1mdtag","M_{jj}",200,0,2);
  deta_1mtag_1mdtag->Sumw2();
  TH1F* deta_2mtag_0mdtag=new TH1F("deta_2mtag_0mdtag","M_{jj}",200,0,2);
  deta_2mtag_0mdtag->Sumw2();
  TH1F* deta_2mtag_1mdtag=new TH1F("deta_2mtag_1mdtag","M_{jj}",200,0,2);
  deta_2mtag_1mdtag->Sumw2();
  TH1F* deta_2mtag_2mdtag=new TH1F("deta_2mtag_2mdtag","M_{jj}",200,0,2);
  deta_2mtag_2mdtag->Sumw2();
*/

  TH1F* mass_0mtag=new TH1F("dijet_mass_0mtag","M_{jj}",8000,0,8000);
  mass_0mtag->Sumw2();
  TH1F* mass_1mtag_1lptag=new TH1F("dijet_mass_1mtag_1lptag","M_{jj}",8000,0,8000);
  mass_1mtag_1lptag->Sumw2();
  TH1F* mass_1mtag_1hptag=new TH1F("dijet_mass_1mtag_1hptag","M_{jj}",8000,0,8000);
  mass_1mtag_1hptag->Sumw2();
  TH1F* mass_2mtag_2lptag=new TH1F("dijet_mass_2mtag_2lptag","M_{jj}",8000,0,8000);
  mass_2mtag_2lptag->Sumw2();
  TH1F* mass_2mtag_1hptag_1lptag=new TH1F("dijet_mass_2mtag_1hptag_1lptag","M_{jj}",8000,0,8000);
  mass_2mtag_1hptag_1lptag->Sumw2();
  TH1F* mass_2mtag_2hptag=new TH1F("dijet_mass_2mtag_2hptag","M_{jj}",8000,0,8000);
  mass_2mtag_2hptag->Sumw2();

  Double_t mgg;
  Double_t evWeight = 1.0;
  Int_t categories;
  Int_t categoriesNS;
  double weight=1.0;

  TTree *TCVARS = new TTree("TCVARS", "VV selection");
  TCVARS->Branch("mgg",&mgg,"mgg/D");

  TCVARS->Branch("evWeight",&evWeight,"evWeight/D");
  TCVARS->Branch("Weight",&weight,"Weight/D");

  TCVARS->Branch("categories",&categories,"categories/I"); 
//  TCVARS->Branch("categoriesNS",&categoriesNS,"categoriesNS/I"); 

  double DijetMass;
  double DijetMassNoCHS;
  double DijetMassCA8;
  double DijetMassAK7;
  double deta;
  double njets;
  double njetsCA8;
  double njetsAK7;
  double Jet1pt;
  double Jet2pt;
  double Jet1eta;
  double Jet2eta;
  double Jet1phi;
  double Jet2phi;
  double Jet1CA8Mass;
  double Jet2CA8Mass;
  double Jet1CA8MassDrop;
  double Jet2CA8MassDrop;
  double Jet1CA8Nsub;
  double Jet2CA8Nsub;
  double Jet1CA8C2beta17;
  double Jet2CA8C2beta17;
  double Jet1AK7Mass;
  double Jet2AK7Mass;
  double Jet1AK7MassDrop;
  double Jet2AK7MassDrop;
  double Jet1AK7Nsub;
  double Jet2AK7Nsub;
  double Jet1AK7C2beta17;
  double Jet2AK7C2beta17;

  TTree *dijetWtag = new TTree("dijetWtag", "dijetWtag");
  dijetWtag->Branch("DijetMass",&DijetMass,"DijetMass/D");
  //dijetWtag->Branch("DijetMassNoCHS",&DijetMassNoCHS,"DijetMassNoCHS/D");
  dijetWtag->Branch("DijetMassCA8",&DijetMassCA8,"DijetMassCA8/D");
  dijetWtag->Branch("DijetMassAK7",&DijetMassAK7,"DijetMassAK7/D");
  //dijetWtag->Branch("deta",&deta,"deta/D");
  dijetWtag->Branch("Jet1pt",&Jet1pt,"Jet1pt/D");
  dijetWtag->Branch("Jet2pt",&Jet2pt,"Jet2pt/D");
  dijetWtag->Branch("Jet1eta",&Jet1eta,"Jet1eta/D");
  dijetWtag->Branch("Jet2eta",&Jet2eta,"Jet2eta/D");
  dijetWtag->Branch("Jet1phi",&Jet1phi,"Jet1phi/D");
  dijetWtag->Branch("Jet2phi",&Jet2phi,"Jet2phi/D");
  dijetWtag->Branch("Jet1CA8Mass",&Jet1CA8Mass,"Jet1CA8Mass/D");
  dijetWtag->Branch("Jet2CA8Mass",&Jet2CA8Mass,"Jet2CA8Mass/D");
  dijetWtag->Branch("Jet1CA8MassDrop",&Jet1CA8MassDrop,"Jet1CA8MassDrop/D");
  dijetWtag->Branch("Jet2CA8MassDrop",&Jet2CA8MassDrop,"Jet2CA8MassDrop/D");
  dijetWtag->Branch("Jet1CA8Nsub",&Jet1CA8Nsub,"Jet1CA8Nsub/D");
  dijetWtag->Branch("Jet2CA8Nsub",&Jet2CA8Nsub,"Jet2CA8Nsub/D");
  /*dijetWtag->Branch("Jet1CA8C2beta17",&Jet1CA8C2beta17,"Jet1CA8C2beta17/D");
  dijetWtag->Branch("Jet2CA8C2beta17",&Jet2CA8C2beta17,"Jet2CA8C2beta17/D");
  dijetWtag->Branch("Jet1AK7Mass",&Jet1AK7Mass,"Jet1AK7Mass/D");
  dijetWtag->Branch("Jet2AK7Mass",&Jet2AK7Mass,"Jet2AK7Mass/D");
  dijetWtag->Branch("Jet1AK7MassDrop",&Jet1AK7MassDrop,"Jet1AK7MassDrop/D");
  dijetWtag->Branch("Jet2AK7MassDrop",&Jet2AK7MassDrop,"Jet2AK7MassDrop/D");
  dijetWtag->Branch("Jet1AK7Nsub",&Jet1AK7Nsub,"Jet1AK7Nsub/D");
  dijetWtag->Branch("Jet2AK7Nsub",&Jet2AK7Nsub,"Jet2AK7Nsub/D");
  dijetWtag->Branch("Jet1AK7C2beta17",&Jet1AK7C2beta17,"Jet1AK7C2beta17/D");
  dijetWtag->Branch("Jet2AK7C2beta17",&Jet2AK7C2beta17,"Jet2AK7C2beta17/D");
  */
  dijetWtag->Branch("weight",&weight,"weight/D");

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  bool hcallasereventfilter2012active=false;
  bool datafiltersactive=false;

  for(int entry=0; entry < nevents; ++entry)
	{
	  // Read event into memory
	  stream.read(entry);

	  // Uncomment the following line if you wish to copy variables into
	  // structs. See the header file analyzer.h to find out what structs
	  // are available. Each struct contains the field "selected", which
	  // can be set as needed. Call saveSelectedObjects() before a call to
	  // addEvent if you wish to save only the selected objects.
	  
	  // fillObjects();
	  
	  // ---------------------
	  // -- event selection --
	  // ---------------------

	  if(!((jethelperCA8_pt.size()>=2)))
	      continue;

          njets=0;
	  for(unsigned n=0;n<jethelper_pt.size();n++)
	     if(jethelper_pt[n]>100)
	         njets++;
          njetsCA8=0;
	  for(unsigned n=0;n<jethelperCA8_pt.size();n++)
	     if(jethelperCA8_pt[n]>100)
	         njetsCA8++;
          njetsAK7=0;
	  for(unsigned n=0;n<jethelperAK7_pt.size();n++)
	     if(jethelperAK7_pt[n]>100)
	         njetsAK7++;

          TLorentzVector Jet1;
          TLorentzVector Jet2;
	  Jet1.SetPtEtaPhiE(jethelper_pt[0],jethelper_eta[0],jethelper_phi[0],jethelper_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelper_pt[1],jethelper_eta[1],jethelper_phi[1],jethelper_energy[1]);
          DijetMass = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelperNoCHS_pt[0],jethelperNoCHS_eta[0],jethelperNoCHS_phi[0],jethelperNoCHS_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperNoCHS_pt[1],jethelperNoCHS_eta[1],jethelperNoCHS_phi[1],jethelperNoCHS_energy[1]);
          DijetMassNoCHS = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelperCA8_pt[0],jethelperCA8_eta[0],jethelperCA8_phi[0],jethelperCA8_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperCA8_pt[1],jethelperCA8_eta[1],jethelperCA8_phi[1],jethelperCA8_energy[1]);
          DijetMassCA8 = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelperAK7_pt[0],jethelperAK7_eta[0],jethelperAK7_phi[0],jethelperAK7_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperAK7_pt[1],jethelperAK7_eta[1],jethelperAK7_phi[1],jethelperAK7_energy[1]);
          DijetMassAK7 = (Jet1+Jet2).M();

          deta = fabs(jethelperCA8_eta[0]-jethelperCA8_eta[1]);
          Jet1pt = jethelperCA8_pt[0];
          Jet2pt = jethelperCA8_pt[1];
          Jet1eta = jethelperCA8_eta[0];
          Jet2eta = jethelperCA8_eta[1];
          Jet1phi = jethelperCA8_phi[0];
          Jet2phi = jethelperCA8_phi[1];
	  
          Jet1CA8Mass = jethelperCA8pruned_mass[0];
          Jet2CA8Mass = jethelperCA8pruned_mass[1];
          Jet1CA8MassDrop = max(jethelperCA8pruned_daughter_0_mass[0],jethelperCA8pruned_daughter_1_mass[0])/Jet1CA8Mass/jethelperCA8pruned_uncor_pt[0]*jethelperCA8pruned_pt[0];
          Jet2CA8MassDrop = max(jethelperCA8pruned_daughter_0_mass[1],jethelperCA8pruned_daughter_1_mass[1])/Jet2CA8Mass/jethelperCA8pruned_uncor_pt[1]*jethelperCA8pruned_pt[1];
	  if ((jethelperCA8pruned_daughter_0_mass[0]<0.0001)||(jethelperCA8pruned_daughter_1_mass[0]<0.0001)) Jet1CA8MassDrop = 2;
	  if ((jethelperCA8pruned_daughter_0_mass[1]<0.0001)||(jethelperCA8pruned_daughter_1_mass[1]<0.0001)) Jet2CA8MassDrop = 2;
          Jet1CA8Nsub = jethelperCA8_tau2[0]/jethelperCA8_tau1[0];
          Jet2CA8Nsub = jethelperCA8_tau2[1]/jethelperCA8_tau1[1];
          Jet1CA8C2beta17 = jethelperCA8_C2beta17[0];
          Jet2CA8C2beta17 = jethelperCA8_C2beta17[1];
           
          Jet1AK7Mass = jethelperAK7pruned_mass[0];
          Jet2AK7Mass = jethelperAK7pruned_mass[1];
          Jet1AK7MassDrop = max(jethelperAK7pruned_daughter_0_mass[0],jethelperAK7pruned_daughter_1_mass[0])/Jet1AK7Mass/jethelperAK7pruned_uncor_pt[0]*jethelperAK7pruned_pt[0];
          Jet2AK7MassDrop = max(jethelperAK7pruned_daughter_0_mass[1],jethelperAK7pruned_daughter_1_mass[1])/Jet2AK7Mass/jethelperAK7pruned_uncor_pt[1]*jethelperAK7pruned_pt[1];
	  if ((jethelperAK7pruned_daughter_0_mass[0]<0.0001)||(jethelperAK7pruned_daughter_1_mass[0]<0.0001)) Jet1AK7MassDrop = 2;
	  if ((jethelperAK7pruned_daughter_0_mass[1]<0.0001)||(jethelperAK7pruned_daughter_1_mass[1]<0.0001)) Jet2AK7MassDrop = 2;
          Jet1AK7Nsub = jethelperAK7_tau2[0]/jethelperAK7_tau1[0];
          Jet2AK7Nsub = jethelperAK7_tau2[1]/jethelperAK7_tau1[1];
          Jet1AK7C2beta17 = jethelperAK7_C2beta17[0];
          Jet2AK7C2beta17 = jethelperAK7_C2beta17[1];
           
          if (triggerresultshelper_hcallasereventfilter2012!=0)
	     hcallasereventfilter2012active=true;

	  if((triggerresultshelper_primaryVertexFilterPath!=0)&&
	     (triggerresultshelper_noscrapingFilterPath!=0)&&
	     (triggerresultshelper_trackingFailureFilterPath!=0)&&
	     (triggerresultshelper_hcalLaserEventFilterPath!=0)&&
	     (triggerresultshelper_HBHENoiseFilterPath!=0)&&
	     (triggerresultshelper_CSCTightHaloFilterPath!=0)&&
	     (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0))
	     datafiltersactive=true;

          //if(DijetMass>1600)
          //    cout << "every " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << endl;
	  if(!((jethelperCA8_pt[0]>30)&&
	     (jethelperCA8_pt[1]>30)&&
	     (fabs(jethelperCA8_eta[0])<2.5)&&
	     (fabs(jethelperCA8_eta[1])<2.5)&&
	     
	     (jethelperCA8_muonEnergyFraction[0]<0.80)&&
	     //(jethelper_neutralHadronEnergyFraction[0]<0.99)&&
	     (jethelperCA8_neutralHadronEnergyFraction[0]<0.90)&&
	     //(jethelper_neutralEmEnergyFraction[0]<0.99)&&
	     (jethelperCA8_neutralEmEnergyFraction[0]<0.90)&&
	     (jethelperCA8_nConstituents[0]>1)&&
	     ((fabs(jethelper_eta[0])>2.4)||
	      ((jethelperCA8_chargedHadronEnergyFraction[0]>0.01)&&
	       (jethelperCA8_chargedMultiplicity[0]>0)&&
	       (jethelperCA8_chargedEmEnergyFraction[0]<0.99)))&&
	     
	     (jethelperCA8_muonEnergyFraction[1]<0.80)&&
	     //(jethelper_neutralHadronEnergyFraction[1]<0.99)&&
	     (jethelperCA8_neutralHadronEnergyFraction[1]<0.90)&&
	     //(jethelper_neutralEmEnergyFraction[1]<0.99)&&
	     (jethelperCA8_neutralEmEnergyFraction[1]<0.90)&&
	     (jethelperCA8_nConstituents[1]>1)&&
	     ((fabs(jethelperCA8_eta[1])>2.4)||
	      ((jethelperCA8_chargedHadronEnergyFraction[1]>0.01)&&
	       (jethelperCA8_chargedMultiplicity[1]>0)&&
	       (jethelperCA8_chargedEmEnergyFraction[1]<0.99)))
	     
	     &&
	     (((sdouble_kt6PFJets_rho_value<40)&&
	       (triggerresultshelper_primaryVertexFilterPath!=0)&&
	       (triggerresultshelper_noscrapingFilterPath!=0)&&
	       (triggerresultshelper_trackingFailureFilterPath!=0)&&
	       (triggerresultshelper_hcalLaserEventFilterPath!=0)&&
	       (triggerresultshelper_HBHENoiseFilterPath!=0)&&
	       (triggerresultshelper_CSCTightHaloFilterPath!=0)&&
	       (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0))||
	      (datafiltersactive==false))&&
	     ((triggerresultshelper_hcallasereventfilter2012!=0)||(hcallasereventfilter2012active==false))
	    )) continue;

	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

          if(geneventinfoproduct_weight>0)
	      weight=geneventinfoproduct_weight;
	  else
	      weight=1;

	  if(!(deta<1.3))
	      continue;

          if((cmdline.outputfilename.find("Py6")!=std::string::npos)||(cmdline.outputfilename.find("Hpp")!=std::string::npos)||(DijetMassCA8>890))
              dijetWtag->Fill();

          if((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(DijetMassCA8>1600))
              cout << "filtered " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << ":" << DijetMass << ":" << Jet1CA8Mass << ":" << Jet2CA8Mass << ":" << Jet1CA8Nsub << ":" << Jet2CA8Nsub << endl;
/*
	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25))))
	  {
              mass_0Wtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25))))||
 	    ((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))))
	  {
              mass_1Wtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)) &&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass_2Wtag->Fill(DijetMass, weight);
              massNoCHS_2Wtag->Fill(DijetMassNoCHS, weight);
              massCA8_2Wtag->Fill(DijetMassCA8, weight);
	      if(DijetMass>1600)
                cout << "run:lumi:event:mass " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << ":" << DijetMass << ":" << Jet1CA8Mass << ":" << Jet2CA8Mass << ":" << Jet1CA8MassDrop << ":" << Jet2CA8MassDrop << endl;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.4)) &&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.4)))
	  {
              mass_2Nsubtag04->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.5)) &&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.5)))
	  {
              mass_2Nsubtag05->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.6)) &&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.6)))
	  {
              mass_2Nsubtag06->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.4)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.6))))||
 	    ((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.6)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.4)))))
	  {
              mass_2Nsubtag0406->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.4)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.4))))||
 	    ((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.4)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.4)))))
	  {
              mass_1Nsubtag04->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.5)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.5))))||
 	    ((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.5)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.5)))))
	  {
              mass_1Nsubtag05->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.6)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.6))))||
 	    ((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.6)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.6)))))
	  {
              mass_1Nsubtag06->Fill(DijetMass, weight);
	  }
	  if((!((Jet1CA8Mass>100)&&(Jet1CA8Mass<150)&&(Jet1CA8MassDrop<0.25)))&&
	     (!((Jet2CA8Mass>100)&&(Jet2CA8Mass<150)&&(Jet2CA8MassDrop<0.25))))
	  {
              mass_0Htag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>100)&&(Jet1CA8Mass<150)&&(Jet1CA8MassDrop<0.25)))&&
	     (!((Jet2CA8Mass>100)&&(Jet2CA8Mass<150)&&(Jet2CA8MassDrop<0.25))))||
 	    ((!((Jet1CA8Mass>100)&&(Jet1CA8Mass<150)&&(Jet1CA8MassDrop<0.25)))&&
	     (((Jet2CA8Mass>100)&&(Jet2CA8Mass<150)&&(Jet2CA8MassDrop<0.25)))))
	  {
              mass_1Htag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>100)&&(Jet1CA8Mass<150)&&(Jet1CA8MassDrop<0.25)) &&
	     ((Jet2CA8Mass>100)&&(Jet2CA8Mass<150)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass_2Htag->Fill(DijetMass, weight);
	  }
	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.4)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.4))))
	  {
              mass_0Wtag04->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.4)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.4))))||
 	    ((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.4)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.4)))))
	  {
              mass_1Wtag04->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.4)) &&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.4)))
	  {
              mass_2Wtag04->Fill(DijetMass, weight);
	  }
	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  {
              mass_0Mtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	    ((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)))))
	  {
              mass_1Mtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)) &&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)))
	  {
              mass_2Mtag->Fill(DijetMass, weight);
	  }



          categories=-1;
          categoriesNS=-1;
          mgg = DijetMassCA8;


	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  {
              mass_0mtag->Fill(DijetMass, weight);
              mass_eta_0mtag->Fill(DijetMass, deta, weight);
	      categories = 5;
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_0mdtag->Fill(DijetMass, weight);
              mass_eta_1mtag_0mdtag->Fill(DijetMass, deta, weight);
	      categories = 4;
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1mdtag->Fill(DijetMass, weight);
              mass_eta_1mtag_1mdtag->Fill(DijetMass, deta, weight);
	      categories = 3;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass_2mtag_0mdtag->Fill(DijetMass, weight);
              mass_eta_2mtag_0mdtag->Fill(DijetMass, deta, weight);
	      categories = 2;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass_2mtag_1mdtag->Fill(DijetMass, weight);
              mass_eta_2mtag_1mdtag->Fill(DijetMass, deta, weight);
	      categories = 1;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass_2mtag_2mdtag->Fill(DijetMass, weight);
              mass_eta_2mtag_2mdtag->Fill(DijetMass, deta, weight);
	      categories = 0;
	  }


	  if((!((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)))&&
	     (!((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))))
	  {
              mass_0mak7tag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet1AK7MassDrop>0.25)))&&
	      (!((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))))||
 	     ((((Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet2AK7MassDrop>0.25)))&&
	      (!((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)))))
	  {
              mass_1mak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet1AK7MassDrop<0.25)))&&
	      (!((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))))||
 	     ((((Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet2AK7MassDrop<0.25)))&&
	      (!((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)))))
	  {
              mass_1mak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>70)&&(Jet1AK7Mass<100))&&
	     ((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))&&
	     ((Jet1AK7MassDrop>0.25)&&(Jet2AK7MassDrop>0.25)))
	  {
              mass_2mak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>70)&&(Jet1AK7Mass<100))&&
	     ((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))&&
	     (((Jet1AK7MassDrop>0.25)&&(Jet2AK7MassDrop<0.25))||
	      ((Jet1AK7MassDrop<0.25)&&(Jet2AK7MassDrop>0.25))))
	  {
              mass_2mak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>70)&&(Jet1AK7Mass<100))&&
	     ((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))&&
	     ((Jet1AK7MassDrop<0.25)&&(Jet2AK7MassDrop<0.25)))
	  {
              mass_2mak7tag_2mdtag->Fill(DijetMassAK7, weight);
	  }


	  if((!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))&&
	     (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))
	  {
              mass_0smtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass_1smtag_0mdtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass_1smtag_1mdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass_2smtag_0mdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass_2smtag_1mdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass_2smtag_2mdtag->Fill(DijetMass, weight);
	  }


	  if((!((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)))&&
	     (!((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))))
	  {
              mass_0smak7tag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)&&(Jet1AK7MassDrop>0.25)))&&
	      (!((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))))||
 	     ((((Jet2AK7Mass>40)&&(Jet2AK7Mass<70)&&(Jet2AK7MassDrop>0.25)))&&
	      (!((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)))))
	  {
              mass_1smak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)&&(Jet1AK7MassDrop<0.25)))&&
	      (!((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))))||
 	     ((((Jet2AK7Mass>40)&&(Jet2AK7Mass<70)&&(Jet2AK7MassDrop<0.25)))&&
	      (!((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)))))
	  {
              mass_1smak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>40)&&(Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))&&
	     ((Jet1AK7MassDrop>0.25)&&(Jet2AK7MassDrop>0.25)))
	  {
              mass_2smak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>40)&&(Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))&&
	     (((Jet1AK7MassDrop>0.25)&&(Jet2AK7MassDrop<0.25))||
	      ((Jet1AK7MassDrop<0.25)&&(Jet2AK7MassDrop>0.25))))
	  {
              mass_2smak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>40)&&(Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))&&
	     ((Jet1AK7MassDrop<0.25)&&(Jet2AK7MassDrop<0.25)))
	  {
              mass_2smak7tag_2mdtag->Fill(DijetMassAK7, weight);
	  }


	  if((!((Jet1CA8Mass<70)))&&
	     (!((Jet2CA8Mass<70))))
	  {
              mass_0ssmtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass<70)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass<70)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass<70)))))
	  {
              mass_1ssmtag_0mdtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass<70)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass<70)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass<70)))))
	  {
              mass_1ssmtag_1mdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass_2ssmtag_0mdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass<70))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass_2ssmtag_1mdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass_2ssmtag_2mdtag->Fill(DijetMass, weight);
	  }


	  if((!((Jet1AK7Mass<70)))&&
	     (!((Jet2AK7Mass<70))))
	  {
              mass_0ssmak7tag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass<70)&&(Jet1AK7MassDrop>0.25)))&&
	      (!((Jet2AK7Mass<70))))||
 	     ((((Jet2AK7Mass<70)&&(Jet2AK7MassDrop>0.25)))&&
	      (!((Jet1AK7Mass<70)))))
	  {
              mass_1ssmak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass<70)&&(Jet1AK7MassDrop<0.25)))&&
	      (!((Jet2AK7Mass<70))))||
 	     ((((Jet2AK7Mass<70)&&(Jet2AK7MassDrop<0.25)))&&
	      (!((Jet1AK7Mass<70)))))
	  {
              mass_1ssmak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass<70))&&
	     ((Jet1AK7MassDrop>0.25)&&(Jet2AK7MassDrop>0.25)))
	  {
              mass_2ssmak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass<70))&&
	     (((Jet1AK7MassDrop>0.25)&&(Jet2AK7MassDrop<0.25))||
	      ((Jet1AK7MassDrop<0.25)&&(Jet2AK7MassDrop>0.25))))
	  {
              mass_2ssmak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass<70))&&
	     ((Jet1AK7MassDrop<0.25)&&(Jet2AK7MassDrop<0.25)))
	  {
              mass_2ssmak7tag_2mdtag->Fill(DijetMassAK7, weight);
	  }


	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  {
          //    mass_0mtag->Fill(DijetMass, weight);
	      categoriesNS = 5;
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub>0.5)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub>0.5)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_0nstag->Fill(DijetMass, weight);
	      categoriesNS = 4;
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.5)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.5)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1nstag->Fill(DijetMass, weight);
	      categoriesNS = 3;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub>0.5)))
	  {
              mass_2mtag_0nstag->Fill(DijetMass, weight);
	      categoriesNS = 2;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub<0.5))||
	      ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub>0.5))))
	  {
              mass_2mtag_1nstag->Fill(DijetMass, weight);
	      categoriesNS = 1;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5)))
	  {
              mass_2mtag_2nstag->Fill(DijetMass, weight);
	      categoriesNS = 0;
	  }



	  //if((!((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)))&&
	  //   (!((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))))
	  //{
          //    mass_0mak7tag->Fill(DijetMassAK7, weight);
	  //}
	  if(((((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet1AK7Nsub>0.5)))&&
	      (!((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))))||
 	     ((((Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet2AK7Nsub>0.5)))&&
	      (!((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)))))
	  {
              mass_1mak7tag_0nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)&&(Jet1AK7Nsub<0.5)))&&
	      (!((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))))||
 	     ((((Jet2AK7Mass>70)&&(Jet2AK7Mass<100)&&(Jet2AK7Nsub<0.5)))&&
	      (!((Jet1AK7Mass>70)&&(Jet1AK7Mass<100)))))
	  {
              mass_1mak7tag_1nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>70)&&(Jet1AK7Mass<100))&&
	     ((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))&&
	     ((Jet1AK7Nsub>0.5)&&(Jet2AK7Nsub>0.5)))
	  {
              mass_2mak7tag_0nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>70)&&(Jet1AK7Mass<100))&&
	     ((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))&&
	     (((Jet1AK7Nsub>0.5)&&(Jet2AK7Nsub<0.5))||
	      ((Jet1AK7Nsub<0.5)&&(Jet2AK7Nsub>0.5))))
	  {
              mass_2mak7tag_1nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>70)&&(Jet1AK7Mass<100))&&
	     ((Jet2AK7Mass>70)&&(Jet2AK7Mass<100))&&
	     ((Jet1AK7Nsub<0.5)&&(Jet2AK7Nsub<0.5)))
	  {
              mass_2mak7tag_2nstag->Fill(DijetMassAK7, weight);
	  }



	  //if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	  //   (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  //{
          //    mass_0mtag->Fill(DijetMass, weight);
	  //}
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub>0.55)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub>0.55)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_0nsltag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.55)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.55)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1nsltag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8Nsub>0.55)&&(Jet2CA8Nsub>0.55)))
	  {
              mass_2mtag_0nsltag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8Nsub>0.55)&&(Jet2CA8Nsub<0.55))||
	      ((Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub>0.55))))
	  {
              mass_2mtag_1nsltag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8Nsub<0.55)&&(Jet2CA8Nsub<0.55)))
	  {
              mass_2mtag_2nsltag->Fill(DijetMass, weight);
	  }



	  //if((!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))&&
	  //   (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))
	  //{
          //    mass_0smtag->Fill(DijetMass, weight);
	  //}
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8Nsub>0.5)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8Nsub>0.5)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass_1smtag_0nstag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8Nsub<0.5)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8Nsub<0.5)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass_1smtag_1nstag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub>0.5)))
	  {
              mass_2smtag_0nstag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     (((Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub<0.5))||
	      ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub>0.5))))
	  {
              mass_2smtag_1nstag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5)))
	  {
              mass_2smtag_2nstag->Fill(DijetMass, weight);
	  }



	  //if((!((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)))&&
	  //   (!((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))))
	  //{
          //    mass_0smak7tag->Fill(DijetMassAK7, weight);
	  //}
	  if(((((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)&&(Jet1AK7Nsub>0.5)))&&
	      (!((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))))||
 	     ((((Jet2AK7Mass>40)&&(Jet2AK7Mass<70)&&(Jet2AK7Nsub>0.5)))&&
	      (!((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)))))
	  {
              mass_1smak7tag_0nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)&&(Jet1AK7Nsub<0.5)))&&
	      (!((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))))||
 	     ((((Jet2AK7Mass>40)&&(Jet2AK7Mass<70)&&(Jet2AK7Nsub<0.5)))&&
	      (!((Jet1AK7Mass>40)&&(Jet1AK7Mass<70)))))
	  {
              mass_1smak7tag_1nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>40)&&(Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))&&
	     ((Jet1AK7Nsub>0.5)&&(Jet2AK7Nsub>0.5)))
	  {
              mass_2smak7tag_0nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>40)&&(Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))&&
	     (((Jet1AK7Nsub>0.5)&&(Jet2AK7Nsub<0.5))||
	      ((Jet1AK7Nsub<0.5)&&(Jet2AK7Nsub>0.5))))
	  {
              mass_2smak7tag_1nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1AK7Mass>40)&&(Jet1AK7Mass<70))&&
	     ((Jet2AK7Mass>40)&&(Jet2AK7Mass<70))&&
	     ((Jet1AK7Nsub<0.5)&&(Jet2AK7Nsub<0.5)))
	  {
              mass_2smak7tag_2nstag->Fill(DijetMassAK7, weight);
	  }



	  if(((Jet1CA8MassDrop>0.8)||(Jet2CA8MassDrop>0.8))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)))&&
             (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))))
	  {
              mass_0mtag_1smdtag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.8)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.8)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1smdtag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop>0.8)||(Jet2CA8MassDrop>0.8)))
	  {
              mass_2mtag_1smdtag->Fill(DijetMass, weight);
	  }


	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  {
              mass8_0mtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass8_1mtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass8_1mtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass8_2mtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass8_2mtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass8_2mtag_2mdtag->Fill(DijetMassCA8, weight);
	  }


	  if((!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))&&
	     (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))
	  {
              mass8_0smtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass8_1smtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass8_1smtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass8_2smtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass8_2smtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass8_2smtag_2mdtag->Fill(DijetMassCA8, weight);
	  }


	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  {
              mass7_0mtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass7_1mtag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass7_1mtag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass7_2mtag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass7_2mtag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass7_2mtag_2mdtag->Fill(DijetMassAK7, weight);
	  }


	  if((!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))&&
	     (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))
	  {
              mass7_0smtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass7_1smtag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))))||
 	     ((((Jet2CA8Mass>40)&&(Jet2CA8Mass<70)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>40)&&(Jet1CA8Mass<70)))))
	  {
              mass7_1smtag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))
	  {
              mass7_2smtag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))
	  {
              mass7_2smtag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((Jet1CA8Mass>40)&&(Jet1CA8Mass<70))&&
	     ((Jet2CA8Mass>40)&&(Jet2CA8Mass<70))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass7_2smtag_2mdtag->Fill(DijetMassAK7, weight);
	  }


	  //if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	  //   (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  //{
          //    mass_0mtag->Fill(DijetMass, weight);
	  //}
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.4)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.4)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_0mdltag->Fill(DijetMass, weight);
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.4)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.4)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1mdltag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop>0.4)&&(Jet2CA8MassDrop>0.4)))
	  {
              mass_2mtag_0mdltag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8MassDrop>0.4)&&(Jet2CA8MassDrop<0.4))||
	      ((Jet1CA8MassDrop<0.4)&&(Jet2CA8MassDrop>0.4))))
	  {
              mass_2mtag_1mdltag->Fill(DijetMass, weight);
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop<0.4)&&(Jet2CA8MassDrop<0.25)))
	  {
              mass_2mtag_1mdtag_1mdltag->Fill(DijetMass, weight);
	  }



	  if(((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))&&(DijetMass>890))
	  {
              jet1_massdrop_0mtag->Fill(Jet1CA8MassDrop, weight);
              jet2_massdrop_0mtag->Fill(Jet2CA8MassDrop, weight);
	  }
	  if((((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))&&(DijetMass>890))
	  {
              jet1_massdrop_1mtag->Fill(Jet1CA8MassDrop, weight);
              jet2_massdrop_1mtag->Fill(Jet2CA8MassDrop, weight);
	  }
	  if((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)))&&(DijetMass>890))
	  {
              jet1_massdrop_2mtag->Fill(Jet1CA8MassDrop, weight);
              jet2_massdrop_2mtag->Fill(Jet2CA8MassDrop, weight);
	  }

	  if(((!((Jet1CA8MassDrop<0.25)))&&
	     (!((Jet2CA8MassDrop<0.25))))&&(DijetMass>890))
	  {
              jet1_mass_0mdtag->Fill(Jet1CA8Mass, weight);
              jet2_mass_0mdtag->Fill(Jet2CA8Mass, weight);
	  }
	  if((((((Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8MassDrop<0.25))))||
 	     ((((Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8MassDrop<0.25)))))&&(DijetMass>890))
	  {
              jet1_mass_1mdtag->Fill(Jet1CA8Mass, weight);
              jet2_mass_1mdtag->Fill(Jet2CA8Mass, weight);
	  }
	  if((((Jet1CA8MassDrop<0.25))&&
	     ((Jet2CA8MassDrop<0.25)))&&(DijetMass>890))
	  {
              jet1_mass_2mdtag->Fill(Jet1CA8Mass, weight);
              jet2_mass_2mdtag->Fill(Jet2CA8Mass, weight);
	  }

	  if(((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))&&(DijetMass>1600))
	  {
              njets_0mtag->Fill(njets, weight);
              njetsCA8_0mtag->Fill(njetsCA8, weight);
              deta_0mtag->Fill(deta, weight);
	  }
	  if((((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop>0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop>0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))&&(DijetMass>1600))
	  {
              njets_1mtag_0mdtag->Fill(njets, weight);
              njetsCA8_1mtag_0mdtag->Fill(njetsCA8, weight);
              deta_1mtag_0mdtag->Fill(deta, weight);
	  }
	  if((((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8MassDrop<0.25)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8MassDrop<0.25)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))&&(DijetMass>1600))
	  {
              njets_1mtag_1mdtag->Fill(njets, weight);
              njetsCA8_1mtag_1mdtag->Fill(njetsCA8, weight);
              deta_1mtag_1mdtag->Fill(deta, weight);
	  }
	  if((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop>0.25)))&&(DijetMass>1600))
	  {
              njets_2mtag_0mdtag->Fill(njets, weight);
              njetsCA8_2mtag_0mdtag->Fill(njetsCA8, weight);
              deta_2mtag_0mdtag->Fill(deta, weight);
	  }
	  if((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8MassDrop>0.25)&&(Jet2CA8MassDrop<0.25))||
	      ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop>0.25))))&&(DijetMass>1600))
	  {
              njets_2mtag_1mdtag->Fill(njets, weight);
              njetsCA8_2mtag_1mdtag->Fill(njetsCA8, weight);
              deta_2mtag_1mdtag->Fill(deta, weight);
	  }
	  if((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8MassDrop<0.25)&&(Jet2CA8MassDrop<0.25)))&&(DijetMass>1600))
	  {
              njets_2mtag_2mdtag->Fill(njets, weight);
              njetsCA8_2mtag_2mdtag->Fill(njetsCA8, weight);
              deta_2mtag_2mdtag->Fill(deta, weight);
	  }
*/
          categories=-1;
          categoriesNS=-1;
          mgg = DijetMassCA8;


	  if((!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))&&
	     (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))
	  {
              mass_0mtag->Fill(DijetMassCA8, weight);
	      categories = 5;
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.75)&&(Jet1CA8Nsub>0.5)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.75)&&(Jet2CA8Nsub>0.5)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1lptag->Fill(DijetMassCA8, weight);
	      categories = 4;
	  }
	  if(((((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)&&(Jet1CA8Nsub<0.5)))&&
	      (!((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))))||
 	     ((((Jet2CA8Mass>70)&&(Jet2CA8Mass<100)&&(Jet2CA8Nsub<0.5)))&&
	      (!((Jet1CA8Mass>70)&&(Jet1CA8Mass<100)))))
	  {
              mass_1mtag_1hptag->Fill(DijetMassCA8, weight);
	      categories = 3;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8Nsub<0.75)&&(Jet2CA8Nsub<0.75)&&(Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub>0.5)))
	  {
              mass_2mtag_2lptag->Fill(DijetMassCA8, weight);
	      categories = 2;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     (((Jet1CA8Nsub<0.75)&&(Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub<0.5))||
	      ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.75)&&(Jet2CA8Nsub>0.5))))
	  {
              mass_2mtag_1hptag_1lptag->Fill(DijetMassCA8, weight);
	      categories = 1;
	  }
	  if(((Jet1CA8Mass>70)&&(Jet1CA8Mass<100))&&
	     ((Jet2CA8Mass>70)&&(Jet2CA8Mass<100))&&
	     ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5)))
	  {
              mass_2mtag_2hptag->Fill(DijetMassCA8, weight);
	      categories = 0;
	  }

          if(mgg>890) TCVARS->Fill();

	}
  /*
  double y_min=0.5;
  if(weight!=1) y_min=mass_2Wtag->GetMaximum()/1e6;
  
  TCanvas c1("c1","c1",200,200);
  mass_2Wtag->Draw("he");
  mass_2Wtag->GetXaxis()->SetRangeUser(890,2500);
  mass_2Wtag->GetYaxis()->SetRangeUser(y_min,mass_2Wtag->GetMaximum()*1.5);
  gPad->SetLogy(true);
  massNoCHS_2Wtag->SetLineColor(2);
  massNoCHS_2Wtag->Draw("hesame");
  massCA8_2Wtag->SetLineColor(3);
  massCA8_2Wtag->Draw("hesame");
  mass_2Nsubtag04->SetLineColor(4);
  mass_2Nsubtag04->Draw("hesame");
  mass_2Nsubtag05->SetLineColor(4);
  mass_2Nsubtag05->Draw("hesame");
  mass_2Nsubtag06->SetLineColor(4);
  mass_2Nsubtag06->Draw("hesame");
  c1.SaveAs((cmdline.outputfilename.substr(0,cmdline.outputfilename.size()-5)+"_Wtag_mass.root").c_str());
  */
  stream.close();
  ofile.close();

  return 0;
}
