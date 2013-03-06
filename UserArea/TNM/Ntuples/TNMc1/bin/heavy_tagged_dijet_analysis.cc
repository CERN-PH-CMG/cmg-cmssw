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
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.phi", jethelperCA8_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.pt", jethelperCA8_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.trackCountingHighEffBJetTags", jethelperCA8_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.trackCountingHighPurBJetTags", jethelperCA8_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_energy", jethelperCA8_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_et", jethelperCA8_uncor_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.uncor_pt", jethelperCA8_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.tau1", jethelperCA8_tau1);
  stream.select("patJetHelper_selectedPatJetsCA8CHSwithQjets.tau2", jethelperCA8_tau2);
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
  stream.select("npatJetHelper_patJetsWithVarCHS", njethelper);
  stream.select("npatJetHelper_patJetsWithVar", njethelperNoCHS);
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

  Double_t mgg;
  Double_t evWeight;
  Double_t Weight;
  Int_t categories;

  TTree *TCVARS = new TTree("TCVARS", "VV selection");
  TCVARS->Branch("mgg",&mgg,"mgg/D");

  TCVARS->Branch("evWeight",&evWeight,"evWeight/D");
  TCVARS->Branch("Weight",&Weight,"Weight/D");

  TCVARS->Branch("categories",&categories,"categories/I"); 

  evWeight = 1.0;
  Weight = 1.0;

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  double weight=1;
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


          TLorentzVector Jet1;
          TLorentzVector Jet2;
	  Jet1.SetPtEtaPhiE(jethelper_pt[0],jethelper_eta[0],jethelper_phi[0],jethelper_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelper_pt[1],jethelper_eta[1],jethelper_phi[1],jethelper_energy[1]);
          double DijetMass = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelperNoCHS_pt[0],jethelperNoCHS_eta[0],jethelperNoCHS_phi[0],jethelperNoCHS_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperNoCHS_pt[1],jethelperNoCHS_eta[1],jethelperNoCHS_phi[1],jethelperNoCHS_energy[1]);
          double DijetMassNoCHS = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelperCA8_pt[0],jethelperCA8_eta[0],jethelperCA8_phi[0],jethelperCA8_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperCA8_pt[1],jethelperCA8_eta[1],jethelperCA8_phi[1],jethelperCA8_energy[1]);
          double DijetMassCA8 = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelperAK7_pt[0],jethelperAK7_eta[0],jethelperAK7_phi[0],jethelperAK7_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperAK7_pt[1],jethelperAK7_eta[1],jethelperAK7_phi[1],jethelperAK7_energy[1]);
          double DijetMassAK7 = (Jet1+Jet2).M();
	  
          double JetCA8Mass1 = jethelperCA8pruned_mass[0];
          double JetCA8Mass2 = jethelperCA8pruned_mass[1];
          double JetCA8MassDrop1 = max(jethelperCA8pruned_daughter_0_mass[0],jethelperCA8pruned_daughter_1_mass[0])/JetCA8Mass1/jethelperCA8pruned_uncor_pt[0]*jethelperCA8pruned_pt[0];
          double JetCA8MassDrop2 = max(jethelperCA8pruned_daughter_0_mass[1],jethelperCA8pruned_daughter_1_mass[1])/JetCA8Mass2/jethelperCA8pruned_uncor_pt[1]*jethelperCA8pruned_pt[1];
	  if ((jethelperCA8pruned_daughter_0_mass[0]<0.0001)||(jethelperCA8pruned_daughter_1_mass[0]<0.0001)) JetCA8MassDrop1 = 2;
	  if ((jethelperCA8pruned_daughter_0_mass[1]<0.0001)||(jethelperCA8pruned_daughter_1_mass[1]<0.0001)) JetCA8MassDrop2 = 2;
          double JetCA8Nsub1 = jethelperCA8_tau2[0]/jethelperCA8_tau1[0];
          double JetCA8Nsub2 = jethelperCA8_tau2[1]/jethelperCA8_tau1[1];
           
          double JetAK7Mass1 = jethelperAK7pruned_mass[0];
          double JetAK7Mass2 = jethelperAK7pruned_mass[1];
          double JetAK7MassDrop1 = max(jethelperAK7pruned_daughter_0_mass[0],jethelperAK7pruned_daughter_1_mass[0])/JetAK7Mass1/jethelperAK7pruned_uncor_pt[0]*jethelperAK7pruned_pt[0];
          double JetAK7MassDrop2 = max(jethelperAK7pruned_daughter_0_mass[1],jethelperAK7pruned_daughter_1_mass[1])/JetAK7Mass2/jethelperAK7pruned_uncor_pt[1]*jethelperAK7pruned_pt[1];
	  if ((jethelperAK7pruned_daughter_0_mass[0]<0.0001)||(jethelperAK7pruned_daughter_1_mass[0]<0.0001)) JetAK7MassDrop1 = 2;
	  if ((jethelperAK7pruned_daughter_0_mass[1]<0.0001)||(jethelperAK7pruned_daughter_1_mass[1]<0.0001)) JetAK7MassDrop2 = 2;
          double JetAK7Nsub1 = jethelperAK7_tau2[0]/jethelperAK7_tau1[0];
          double JetAK7Nsub2 = jethelperAK7_tau2[1]/jethelperAK7_tau1[1];
           
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
	  if(!((jethelper_pt.size()>=2)&&
	     (jethelper_pt[0]>30)&&
	     (jethelper_pt[1]>30)&&
	     (fabs(jethelper_eta[0])<2.5)&&
	     (fabs(jethelper_eta[1])<2.5)&&
	     (fabs(jethelper_eta[0]-jethelper_eta[1])<1.3)&&
	     
	     (jethelper_muonEnergyFraction[0]<0.80)&&
	     //(jethelper_neutralHadronEnergyFraction[0]<0.99)&&
	     (jethelper_neutralHadronEnergyFraction[0]<0.90)&&
	     //(jethelper_neutralEmEnergyFraction[0]<0.99)&&
	     (jethelper_neutralEmEnergyFraction[0]<0.90)&&
	     (jethelper_nConstituents[0]>1)&&
	     ((fabs(jethelper_eta[0])>2.4)||
	      ((jethelper_chargedHadronEnergyFraction[0]>0.01)&&
	       (jethelper_chargedMultiplicity[0]>0)&&
	       (jethelper_chargedEmEnergyFraction[0]<0.99)))&&
	     
	     (jethelper_muonEnergyFraction[1]<0.80)&&
	     //(jethelper_neutralHadronEnergyFraction[1]<0.99)&&
	     (jethelper_neutralHadronEnergyFraction[1]<0.90)&&
	     //(jethelper_neutralEmEnergyFraction[1]<0.99)&&
	     (jethelper_neutralEmEnergyFraction[1]<0.90)&&
	     (jethelper_nConstituents[1]>1)&&
	     ((fabs(jethelper_eta[1])>2.4)||
	      ((jethelper_chargedHadronEnergyFraction[1]>0.01)&&
	       (jethelper_chargedMultiplicity[1]>0)&&
	       (jethelper_chargedEmEnergyFraction[1]<0.99)))
	     
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

          if(DijetMass>1600)
              cout << "filtered " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << ":" << DijetMass << ":" << JetCA8Mass1 << ":" << JetCA8Mass2 << ":" << JetCA8MassDrop1 << ":" << JetCA8MassDrop2 << endl;
	  if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.25)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.25))))
	  {
              mass_0Wtag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.25)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.25))))||
 	    ((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.25)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.25)))))
	  {
              mass_1Wtag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.25)) &&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.25)))
	  {
              mass_2Wtag->Fill(DijetMass, weight);
              massNoCHS_2Wtag->Fill(DijetMassNoCHS, weight);
              massCA8_2Wtag->Fill(DijetMassCA8, weight);
	      if(DijetMass>1600)
                cout << "run:lumi:event:mass " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << ":" << DijetMass << ":" << JetCA8Mass1 << ":" << JetCA8Mass2 << ":" << JetCA8MassDrop1 << ":" << JetCA8MassDrop2 << endl;
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.4)) &&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.4)))
	  {
              mass_2Nsubtag04->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.5)) &&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.5)))
	  {
              mass_2Nsubtag05->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.6)) &&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.6)))
	  {
              mass_2Nsubtag06->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.4)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.6))))||
 	    ((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.6)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.4)))))
	  {
              mass_2Nsubtag0406->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.4)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.4))))||
 	    ((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.4)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.4)))))
	  {
              mass_1Nsubtag04->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.5)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.5))))||
 	    ((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.5)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.5)))))
	  {
              mass_1Nsubtag05->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.6)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.6))))||
 	    ((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.6)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.6)))))
	  {
              mass_1Nsubtag06->Fill(DijetMass, weight);
	  }
	  if((!((JetCA8Mass1>100)&&(JetCA8Mass1<150)&&(JetCA8MassDrop1<0.25)))&&
	     (!((JetCA8Mass2>100)&&(JetCA8Mass2<150)&&(JetCA8MassDrop2<0.25))))
	  {
              mass_0Htag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>100)&&(JetCA8Mass1<150)&&(JetCA8MassDrop1<0.25)))&&
	     (!((JetCA8Mass2>100)&&(JetCA8Mass2<150)&&(JetCA8MassDrop2<0.25))))||
 	    ((!((JetCA8Mass1>100)&&(JetCA8Mass1<150)&&(JetCA8MassDrop1<0.25)))&&
	     (((JetCA8Mass2>100)&&(JetCA8Mass2<150)&&(JetCA8MassDrop2<0.25)))))
	  {
              mass_1Htag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>100)&&(JetCA8Mass1<150)&&(JetCA8MassDrop1<0.25)) &&
	     ((JetCA8Mass2>100)&&(JetCA8Mass2<150)&&(JetCA8MassDrop2<0.25)))
	  {
              mass_2Htag->Fill(DijetMass, weight);
	  }
	  if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.4)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.4))))
	  {
              mass_0Wtag04->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.4)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.4))))||
 	    ((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.4)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.4)))))
	  {
              mass_1Wtag04->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.4)) &&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.4)))
	  {
              mass_2Wtag04->Fill(DijetMass, weight);
	  }
	  if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  {
              mass_0Mtag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	    ((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	     (((JetCA8Mass2>70)&&(JetCA8Mass2<100)))))
	  {
              mass_1Mtag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100)) &&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)))
	  {
              mass_2Mtag->Fill(DijetMass, weight);
	  }






	  if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  {
              mass_0mtag->Fill(DijetMass, weight);
	      mgg = DijetMass;
	      categories = 5;
              if(mgg>890) TCVARS->Fill();
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1>0.25)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2>0.25)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_0mdtag->Fill(DijetMass, weight);
	      mgg = DijetMass;
	      categories = 4;
              if(mgg>890) TCVARS->Fill();
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.25)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.25)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_1mdtag->Fill(DijetMass, weight);
	      mgg = DijetMass;
	      categories = 3;
              if(mgg>890) TCVARS->Fill();
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2>0.25)))
	  {
              mass_2mtag_0mdtag->Fill(DijetMass, weight);
	      mgg = DijetMass;
	      categories = 2;
              if(mgg>890) TCVARS->Fill();
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     (((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2<0.25))||
	      ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2>0.25))))
	  {
              mass_2mtag_1mdtag->Fill(DijetMass, weight);
	      mgg = DijetMass;
	      categories = 1;
              if(mgg>890) TCVARS->Fill();
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2<0.25)))
	  {
              mass_2mtag_2mdtag->Fill(DijetMass, weight);
	      mgg = DijetMass;
	      categories = 0;
              if(mgg>890) TCVARS->Fill();
	  }


	  if((!((JetAK7Mass1>70)&&(JetAK7Mass1<100)))&&
	     (!((JetAK7Mass2>70)&&(JetAK7Mass2<100))))
	  {
              mass_0mak7tag->Fill(DijetMassAK7, weight);
	  }
	  if(((((JetAK7Mass1>70)&&(JetAK7Mass1<100)&&(JetAK7MassDrop1>0.25)))&&
	      (!((JetAK7Mass2>70)&&(JetAK7Mass2<100))))||
 	     ((((JetAK7Mass2>70)&&(JetAK7Mass2<100)&&(JetAK7MassDrop2>0.25)))&&
	      (!((JetAK7Mass1>70)&&(JetAK7Mass1<100)))))
	  {
              mass_1mak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((((JetAK7Mass1>70)&&(JetAK7Mass1<100)&&(JetAK7MassDrop1<0.25)))&&
	      (!((JetAK7Mass2>70)&&(JetAK7Mass2<100))))||
 	     ((((JetAK7Mass2>70)&&(JetAK7Mass2<100)&&(JetAK7MassDrop2<0.25)))&&
	      (!((JetAK7Mass1>70)&&(JetAK7Mass1<100)))))
	  {
              mass_1mak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((JetAK7Mass1>70)&&(JetAK7Mass1<100))&&
	     ((JetAK7Mass2>70)&&(JetAK7Mass2<100))&&
	     ((JetAK7MassDrop1>0.25)&&(JetAK7MassDrop2>0.25)))
	  {
              mass_2mak7tag_0mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((JetAK7Mass1>70)&&(JetAK7Mass1<100))&&
	     ((JetAK7Mass2>70)&&(JetAK7Mass2<100))&&
	     (((JetAK7MassDrop1>0.25)&&(JetAK7MassDrop2<0.25))||
	      ((JetAK7MassDrop1<0.25)&&(JetAK7MassDrop2>0.25))))
	  {
              mass_2mak7tag_1mdtag->Fill(DijetMassAK7, weight);
	  }
	  if(((JetAK7Mass1>70)&&(JetAK7Mass1<100))&&
	     ((JetAK7Mass2>70)&&(JetAK7Mass2<100))&&
	     ((JetAK7MassDrop1<0.25)&&(JetAK7MassDrop2<0.25)))
	  {
              mass_2mak7tag_2mdtag->Fill(DijetMassAK7, weight);
	  }


	  if((!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))&&
	     (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))
	  {
              mass_0smtag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>40)&&(JetCA8Mass1<70)&&(JetCA8MassDrop1>0.25)))&&
	      (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))||
 	     ((((JetCA8Mass2>40)&&(JetCA8Mass2<70)&&(JetCA8MassDrop2>0.25)))&&
	      (!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))))
	  {
              mass_1smtag_0mdtag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>40)&&(JetCA8Mass1<70)&&(JetCA8MassDrop1<0.25)))&&
	      (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))||
 	     ((((JetCA8Mass2>40)&&(JetCA8Mass2<70)&&(JetCA8MassDrop2<0.25)))&&
	      (!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))))
	  {
              mass_1smtag_1mdtag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     ((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2>0.25)))
	  {
              mass_2smtag_0mdtag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     (((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2<0.25))||
	      ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2>0.25))))
	  {
              mass_2smtag_1mdtag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2<0.25)))
	  {
              mass_2smtag_2mdtag->Fill(DijetMass, weight);
	  }


	  //if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	  //   (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  //{
          //    mass_0mtag->Fill(DijetMass, weight);
	  //}
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1>0.5)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2>0.5)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_0nstag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.5)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.5)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_1nstag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8Nsub1>0.5)&&(JetCA8Nsub2>0.5)))
	  {
              mass_2mtag_0nstag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     (((JetCA8Nsub1>0.5)&&(JetCA8Nsub2<0.5))||
	      ((JetCA8Nsub1<0.5)&&(JetCA8Nsub2>0.5))))
	  {
              mass_2mtag_1nstag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8Nsub1<0.5)&&(JetCA8Nsub2<0.5)))
	  {
              mass_2mtag_2nstag->Fill(DijetMass, weight);
	  }



	  //if((!((JetAK7Mass1>70)&&(JetAK7Mass1<100)))&&
	  //   (!((JetAK7Mass2>70)&&(JetAK7Mass2<100))))
	  //{
          //    mass_0mak7tag->Fill(DijetMassAK7, weight);
	  //}
	  if(((((JetAK7Mass1>70)&&(JetAK7Mass1<100)&&(JetAK7Nsub1>0.5)))&&
	      (!((JetAK7Mass2>70)&&(JetAK7Mass2<100))))||
 	     ((((JetAK7Mass2>70)&&(JetAK7Mass2<100)&&(JetAK7Nsub2>0.5)))&&
	      (!((JetAK7Mass1>70)&&(JetAK7Mass1<100)))))
	  {
              mass_1mak7tag_0nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((((JetAK7Mass1>70)&&(JetAK7Mass1<100)&&(JetAK7Nsub1<0.5)))&&
	      (!((JetAK7Mass2>70)&&(JetAK7Mass2<100))))||
 	     ((((JetAK7Mass2>70)&&(JetAK7Mass2<100)&&(JetAK7Nsub2<0.5)))&&
	      (!((JetAK7Mass1>70)&&(JetAK7Mass1<100)))))
	  {
              mass_1mak7tag_1nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((JetAK7Mass1>70)&&(JetAK7Mass1<100))&&
	     ((JetAK7Mass2>70)&&(JetAK7Mass2<100))&&
	     ((JetAK7Nsub1>0.5)&&(JetAK7Nsub2>0.5)))
	  {
              mass_2mak7tag_0nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((JetAK7Mass1>70)&&(JetAK7Mass1<100))&&
	     ((JetAK7Mass2>70)&&(JetAK7Mass2<100))&&
	     (((JetAK7Nsub1>0.5)&&(JetAK7Nsub2<0.5))||
	      ((JetAK7Nsub1<0.5)&&(JetAK7Nsub2>0.5))))
	  {
              mass_2mak7tag_1nstag->Fill(DijetMassAK7, weight);
	  }
	  if(((JetAK7Mass1>70)&&(JetAK7Mass1<100))&&
	     ((JetAK7Mass2>70)&&(JetAK7Mass2<100))&&
	     ((JetAK7Nsub1<0.5)&&(JetAK7Nsub2<0.5)))
	  {
              mass_2mak7tag_2nstag->Fill(DijetMassAK7, weight);
	  }



	  //if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	  //   (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  //{
          //    mass_0mtag->Fill(DijetMass, weight);
	  //}
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1>0.55)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2>0.55)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_0nsltag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8Nsub1<0.55)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8Nsub2<0.55)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_1nsltag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8Nsub1>0.55)&&(JetCA8Nsub2>0.55)))
	  {
              mass_2mtag_0nsltag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     (((JetCA8Nsub1>0.55)&&(JetCA8Nsub2<0.55))||
	      ((JetCA8Nsub1<0.55)&&(JetCA8Nsub2>0.55))))
	  {
              mass_2mtag_1nsltag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8Nsub1<0.55)&&(JetCA8Nsub2<0.55)))
	  {
              mass_2mtag_2nsltag->Fill(DijetMass, weight);
	  }



	  //if((!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))&&
	  //   (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))
	  //{
          //    mass_0smtag->Fill(DijetMass, weight);
	  //}
	  if(((((JetCA8Mass1>40)&&(JetCA8Mass1<70)&&(JetCA8Nsub1>0.5)))&&
	      (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))||
 	     ((((JetCA8Mass2>40)&&(JetCA8Mass2<70)&&(JetCA8Nsub2>0.5)))&&
	      (!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))))
	  {
              mass_1smtag_0nstag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>40)&&(JetCA8Mass1<70)&&(JetCA8Nsub1<0.5)))&&
	      (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))||
 	     ((((JetCA8Mass2>40)&&(JetCA8Mass2<70)&&(JetCA8Nsub2<0.5)))&&
	      (!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))))
	  {
              mass_1smtag_1nstag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     ((JetCA8Nsub1>0.5)&&(JetCA8Nsub2>0.5)))
	  {
              mass_2smtag_0nstag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     (((JetCA8Nsub1>0.5)&&(JetCA8Nsub2<0.5))||
	      ((JetCA8Nsub1<0.5)&&(JetCA8Nsub2>0.5))))
	  {
              mass_2smtag_1nstag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     ((JetCA8Nsub1<0.5)&&(JetCA8Nsub2<0.5)))
	  {
              mass_2smtag_2nstag->Fill(DijetMass, weight);
	  }



	  if(((JetCA8MassDrop1>0.8)||(JetCA8MassDrop2>0.8))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100)))&&
             (!((JetCA8Mass1>70)&&(JetCA8Mass1<100))))
	  {
              mass_0mtag_1smdtag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1>0.8)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2>0.8)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_1smdtag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1>0.8)||(JetCA8MassDrop2>0.8)))
	  {
              mass_2mtag_1smdtag->Fill(DijetMass, weight);
	  }


	  if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  {
              mass8_0mtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1>0.25)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2>0.25)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass8_1mtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.25)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.25)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass8_1mtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2>0.25)))
	  {
              mass8_2mtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     (((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2<0.25))||
	      ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2>0.25))))
	  {
              mass8_2mtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2<0.25)))
	  {
              mass8_2mtag_2mdtag->Fill(DijetMassCA8, weight);
	  }


	  if((!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))&&
	     (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))
	  {
              mass8_0smtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((JetCA8Mass1>40)&&(JetCA8Mass1<70)&&(JetCA8MassDrop1>0.25)))&&
	      (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))||
 	     ((((JetCA8Mass2>40)&&(JetCA8Mass2<70)&&(JetCA8MassDrop2>0.25)))&&
	      (!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))))
	  {
              mass8_1smtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((((JetCA8Mass1>40)&&(JetCA8Mass1<70)&&(JetCA8MassDrop1<0.25)))&&
	      (!((JetCA8Mass2>40)&&(JetCA8Mass2<70))))||
 	     ((((JetCA8Mass2>40)&&(JetCA8Mass2<70)&&(JetCA8MassDrop2<0.25)))&&
	      (!((JetCA8Mass1>40)&&(JetCA8Mass1<70)))))
	  {
              mass8_1smtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     ((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2>0.25)))
	  {
              mass8_2smtag_0mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     (((JetCA8MassDrop1>0.25)&&(JetCA8MassDrop2<0.25))||
	      ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2>0.25))))
	  {
              mass8_2smtag_1mdtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetCA8Mass1>40)&&(JetCA8Mass1<70))&&
	     ((JetCA8Mass2>40)&&(JetCA8Mass2<70))&&
	     ((JetCA8MassDrop1<0.25)&&(JetCA8MassDrop2<0.25)))
	  {
              mass8_2smtag_2mdtag->Fill(DijetMassCA8, weight);
	  }


	  //if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	  //   (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  //{
          //    mass_0mtag->Fill(DijetMass, weight);
	  //}
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1>0.4)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2>0.4)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_0mdltag->Fill(DijetMass, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)&&(JetCA8MassDrop1<0.4)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)&&(JetCA8MassDrop2<0.4)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              mass_1mtag_1mdltag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1>0.4)&&(JetCA8MassDrop2>0.4)))
	  {
              mass_2mtag_0mdltag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     (((JetCA8MassDrop1>0.4)&&(JetCA8MassDrop2<0.4))||
	      ((JetCA8MassDrop1<0.4)&&(JetCA8MassDrop2>0.4))))
	  {
              mass_2mtag_1mdltag->Fill(DijetMass, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100))&&
	     ((JetCA8MassDrop1<0.4)&&(JetCA8MassDrop2<0.25)))
	  {
              mass_2mtag_1mdtag_1mdltag->Fill(DijetMass, weight);
	  }



	  if((!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	     (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))
	  {
              jet1_massdrop_0mtag->Fill(JetCA8MassDrop1, weight);
              jet2_massdrop_0mtag->Fill(JetCA8MassDrop2, weight);
	  }
	  if(((((JetCA8Mass1>70)&&(JetCA8Mass1<100)))&&
	      (!((JetCA8Mass2>70)&&(JetCA8Mass2<100))))||
 	     ((((JetCA8Mass2>70)&&(JetCA8Mass2<100)))&&
	      (!((JetCA8Mass1>70)&&(JetCA8Mass1<100)))))
	  {
              jet1_massdrop_1mtag->Fill(JetCA8MassDrop1, weight);
              jet2_massdrop_1mtag->Fill(JetCA8MassDrop2, weight);
	  }
	  if(((JetCA8Mass1>70)&&(JetCA8Mass1<100))&&
	     ((JetCA8Mass2>70)&&(JetCA8Mass2<100)))
	  {
              jet1_massdrop_2mtag->Fill(JetCA8MassDrop1, weight);
              jet2_massdrop_2mtag->Fill(JetCA8MassDrop2, weight);
	  }

	  if((!((JetCA8MassDrop1<0.25)))&&
	     (!((JetCA8MassDrop2<0.25))))
	  {
              jet1_mass_0mdtag->Fill(JetCA8Mass1, weight);
              jet2_mass_0mdtag->Fill(JetCA8Mass2, weight);
	  }
	  if(((((JetCA8MassDrop1<0.25)))&&
	      (!((JetCA8MassDrop2<0.25))))||
 	     ((((JetCA8MassDrop2<0.25)))&&
	      (!((JetCA8MassDrop1<0.25)))))
	  {
              jet1_mass_1mdtag->Fill(JetCA8Mass1, weight);
              jet2_mass_1mdtag->Fill(JetCA8Mass2, weight);
	  }
	  if(((JetCA8MassDrop1<0.25))&&
	     ((JetCA8MassDrop2<0.25)))
	  {
              jet1_mass_2mdtag->Fill(JetCA8Mass1, weight);
              jet2_mass_2mdtag->Fill(JetCA8Mass2, weight);
	  }

	}

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

  TCVARS->Write();

  stream.close();
  ofile.close();

  return 0;
}
