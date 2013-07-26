//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     Thu May 31 21:49:25 2012 by mkntanalyzer.py
// Author:      Andreas Hinzmann
//-----------------------------------------------------------------------------
#include "higgs_tagged_dijet_analysis.h"

#ifdef PROJECT_NAME
#include "PhysicsTools/TheNtupleMaker/interface/pdg.h"
#else
#include "pdg.h"
#endif

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include <vector>

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
  
  stream.select("npatJetHelper_selectedPatJetsCA8CHSwithQjets", njethelperCA8);
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
  stream.select("npatJetHelper_selectedPatJetsCA8CHSpruned", njethelperCA8pruned);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.mass", jethelperCA8pruned_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.phi", jethelperCA8pruned_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.eta", jethelperCA8pruned_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.pt", jethelperCA8pruned_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.energy", jethelperCA8pruned_energy);
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
  stream.select("edmEventHelperExtra_info.numberOfPrimaryVertices", eventhelperextra_numberOfPrimaryVertices);
  stream.select("sdouble_vertexWeightSummer12MC53X2012ABCDData.value", vertexWeight);
  stream.select("PileupSummaryInfo_addPileupInfo.getBunchCrossing", pileupsummaryinfo_getBunchCrossing);
  stream.select("PileupSummaryInfo_addPileupInfo.getPU_NumInteractions", pileupsummaryinfo_getPU_NumInteractions);
  stream.select("PileupSummaryInfo_addPileupInfo.getTrueNumInteractions", pileupsummaryinfo_getTrueNumInteractions);
  stream.select("sdouble_kt6PFJets_rho.value", sdouble_kt6PFJets_rho_value);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8);                                                                                                        // 
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v10", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v10);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v11", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v11);                                                                                                        //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v12", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v12);                                                                                                        // <--
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v1", triggerresultshelper_HLT_HT500_v1); 
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v2", triggerresultshelper_HLT_HT500_v2); 
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT500_v3", triggerresultshelper_HLT_HT500_v3); 
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v1", triggerresultshelper_HLT_HT550_v1); // <--
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v2", triggerresultshelper_HLT_HT550_v2); // trigger monitor
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v3", triggerresultshelper_HLT_HT550_v3); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v4", triggerresultshelper_HLT_HT550_v4); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v5", triggerresultshelper_HLT_HT550_v5); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v6", triggerresultshelper_HLT_HT550_v6); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v7", triggerresultshelper_HLT_HT550_v7); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v8", triggerresultshelper_HLT_HT550_v8); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT550_v9", triggerresultshelper_HLT_HT550_v9); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v1", triggerresultshelper_HLT_HT650_v1); // <--
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v2", triggerresultshelper_HLT_HT650_v2); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v3", triggerresultshelper_HLT_HT650_v3); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v4", triggerresultshelper_HLT_HT650_v4); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v5", triggerresultshelper_HLT_HT650_v5); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v6", triggerresultshelper_HLT_HT650_v6); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v7", triggerresultshelper_HLT_HT650_v7); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v8", triggerresultshelper_HLT_HT650_v8); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v9", triggerresultshelper_HLT_HT650_v9); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v1", triggerresultshelper_HLT_HT750_v1); // <--
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v2", triggerresultshelper_HLT_HT750_v2); // 
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v3", triggerresultshelper_HLT_HT750_v3); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v4", triggerresultshelper_HLT_HT750_v4); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v5", triggerresultshelper_HLT_HT750_v5); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v6", triggerresultshelper_HLT_HT750_v6); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v7", triggerresultshelper_HLT_HT750_v7); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v8", triggerresultshelper_HLT_HT750_v8); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v9", triggerresultshelper_HLT_HT750_v9); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v1", triggerresultshelper_HLT_PFHT650_v1); // <--
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v2", triggerresultshelper_HLT_PFHT650_v2); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v3", triggerresultshelper_HLT_PFHT650_v3); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v4", triggerresultshelper_HLT_PFHT650_v4); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v5", triggerresultshelper_HLT_PFHT650_v5); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v6", triggerresultshelper_HLT_PFHT650_v6); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v7", triggerresultshelper_HLT_PFHT650_v7); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v8", triggerresultshelper_HLT_PFHT650_v8); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v9", triggerresultshelper_HLT_PFHT650_v9); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v10", triggerresultshelper_HLT_PFHT650_v10); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v11", triggerresultshelper_HLT_PFHT650_v11); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFNoPUHT650_v1", triggerresultshelper_HLT_PFNoPUHT650_v1); // <--
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFNoPUHT650_v2", triggerresultshelper_HLT_PFNoPUHT650_v2); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFNoPUHT650_v3", triggerresultshelper_HLT_PFNoPUHT650_v3); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFNoPUHT650_v4", triggerresultshelper_HLT_PFNoPUHT650_v4); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFNoPUHT650_v5", triggerresultshelper_HLT_PFNoPUHT650_v5); //
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFNoPUHT650_v6", triggerresultshelper_HLT_PFNoPUHT650_v6); //
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

  stream.select("recoGenParticleHelper_genParticles.eta", genparticlehelper_eta);
  stream.select("recoGenParticleHelper_genParticles.phi", genparticlehelper_phi);
  stream.select("recoGenParticleHelper_genParticles.pt", genparticlehelper_pt);
  stream.select("recoGenParticleHelper_genParticles.mass", genparticlehelper_mass);
  stream.select("npatJetHelper_patGenJetsCA8CHS", njethelperGenCA8);
  stream.select("patJetHelper_patGenJetsCA8CHS.mass", jethelperGenCA8_mass);
  stream.select("patJetHelper_patGenJetsCA8CHS.eta", jethelperGenCA8_eta);
  stream.select("patJetHelper_patGenJetsCA8CHS.phi", jethelperGenCA8_phi);
  stream.select("patJetHelper_patGenJetsCA8CHS.pt", jethelperGenCA8_pt);
  stream.select("patJetHelper_patGenJetsCA8CHS.et", jethelperGenCA8_et);
  stream.select("npatJetHelper_patGenJetsCA8CHSpruned", njethelperGenCA8pruned);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.mass", jethelperGenCA8pruned_mass);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.eta", jethelperGenCA8pruned_eta);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.phi", jethelperGenCA8pruned_phi);
  stream.select("patJetHelper_patGenJetsCA8CHSpruned.pt", jethelperGenCA8pruned_pt);


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

  TH1F* mass_0mtag=new TH1F("dijet_mass_0mtag","M_{jj}",8000,0,8000);                               // declaration
  mass_0mtag->Sumw2();                                                                              // 
  TH1F* mass_1mtag_1lptag=new TH1F("dijet_mass_1mtag_1lptag","M_{jj}",8000,0,8000);                 // of the event
  mass_1mtag_1lptag->Sumw2();                                                                       //
  TH1F* mass_1mtag_1hptag=new TH1F("dijet_mass_1mtag_1hptag","M_{jj}",8000,0,8000);                 // categories,
  mass_1mtag_1hptag->Sumw2();                                                                       //
  TH1F* mass_2mtag_2lptag=new TH1F("dijet_mass_2mtag_2lptag","M_{jj}",8000,0,8000);                 // see the part
  mass_2mtag_2lptag->Sumw2();                                                                       //
  TH1F* mass_2mtag_1hptag_1lptag=new TH1F("dijet_mass_2mtag_1hptag_1lptag","M_{jj}",8000,0,8000);   // "fill histograms"
  mass_2mtag_1hptag_1lptag->Sumw2();                                                                //
  TH1F* mass_2mtag_2hptag=new TH1F("dijet_mass_2mtag_2hptag","M_{jj}",8000,0,8000);                 // below
  mass_2mtag_2hptag->Sumw2();                                                                       //
  
  TH1F* mass=new TH1F("dijet_mass","M_{jj}",8000,0,8000);                                           // histogram of the masses of all the ranked events
  mass->Sumw2();                                                                                    //

  Double_t mgg;
  Double_t evWeight = 1.0;
  Int_t categories;
  Int_t categoriesNS;
  double weight=1.0;
  double pi=3.141592654;

  //TTree *TCVARS = new TTree("TCVARS", "VV selection");                       // create the TCVARS tree (ranked, mgg>890, deta<1.3)                     
  //TCVARS->Branch("mgg",&mgg,"mgg/D");                                         // as well as its branches.
  //TCVARS->Branch("evWeight",&evWeight,"evWeight/D");                          //
  //TCVARS->Branch("Weight",&weight,"Weight/D");                                // 
  //TCVARS->Branch("categories",&categories,"categories/I");                    //
  //TCVARS->Branch("categoriesNS",&categoriesNS,"categoriesNS/I");              //

  double DijetMass;
  double DijetMassNoCHS;
  double DijetMassCA8;
  double DijetMassCA8pruned;
  double DijetMassCA8dw;
  double DijetMassCA8up;
  double DijetMassCA8pruneddw;
  double DijetMassCA8prunedup;
  double deta;
  double cteta;
  double Ht;
  double njets;
  double njetsCA8;  
  double Jet1pt;
  double Jet2pt;
  double Jet1eta;
  double Jet2eta;
  double Jet1phi;
  double Jet2phi;
  double dphi;
  double met;
  double Jet1CA8Mass;
  double Jet2CA8Mass;
  double Jet1CA8Massdw;
  double Jet2CA8Massdw;
  double Jet1CA8Massup;
  double Jet2CA8Massup;
  double Jet1CA8MassDrop;
  double Jet2CA8MassDrop;
  double Jet1CA8Nsub;
  double Jet2CA8Nsub;
  double dphi1;
  double dphi2;
  double Jet1dr;
  double Jet2dr;
  int nbvert;
  double Jet1CA8C2beta17;
  double Jet2CA8C2beta17;
  double Jet1mef;
  double Jet1nhef;
  double Jet1neef;
  int Jet1nbcons;
  double Jet1chef;
  double Jet1ceef;
  double Jet1cmult;
  double Jet2mef;
  double Jet2nhef;
  double Jet2neef;
  int  Jet2nbcons;
  double Jet2chef;
  double Jet2ceef;
  double Jet2cmult;
  int fat;
  int ht550;
  int ht650;
  int ht750;
  int pfht650;
  int pfnopuht650;
  int trig;

  
  double dphiparttogenij;
  double detaparttogenij;
  double drparttogenij;
  double drparttogeni;
  int jparttogeni;
  double genjetmass1;
  double genjetphi1;
  double genjeteta1;
  double genjetpt1;
  double genjetmass2;
  double genjetphi2;
  double genjeteta2;
  double genjetpt2;
  double prunedgenjetmass1;
  double prunedgenjetphi1;
  double prunedgenjeteta1;
  double prunedgenjetpt1;
  double prunedgenjetmass2;
  double prunedgenjetphi2;
  double prunedgenjeteta2;
  double prunedgenjetpt2;
  double recojetmass1;
  double recojetphi1;
  double recojeteta1;
  double recojetpt1;
  double recojetmass2;
  double recojetphi2;
  double recojeteta2;
  double recojetpt2;
  double prunedrecojetmass1;
  double prunedrecojetphi1;
  double prunedrecojeteta1;
  double prunedrecojetpt1;
  double prunedrecojetmass2;
  double prunedrecojetphi2;
  double prunedrecojeteta2;
  double prunedrecojetpt2;
  double dphigentoprunedgeni1;
  double detagentoprunedgeni1;
  double drgentoprunedgeni1;
  double dphigentoprunedgeni2;
  double detagentoprunedgeni2;
  double drgentoprunedgeni2;
  int igentoprunedgen1;
  double drgentoprunedgen1;
  int igentoprunedgen2;
  double drgentoprunedgen2;
  double dmgentoprunedgen1;
  double dmgentoprunedgen2;
  double dpgentoprunedgen1;
  double dpgentoprunedgen2;
  double dphigentorecoi1;
  double detagentorecoi1;
  double drgentorecoi1;
  double dphigentorecoi2;
  double detagentorecoi2;
  double drgentorecoi2;
  int igentoreco1;
  double drgentoreco1;
  int igentoreco2;
  double drgentoreco2;
  double dmgentoreco1;
  double dmgentoreco2;
  double dpgentoreco1;
  double dpgentoreco2;
  double dphirecotoprunedrecoi1;
  double detarecotoprunedrecoi1;
  double drrecotoprunedrecoi1;
  double dphirecotoprunedrecoi2;
  double detarecotoprunedrecoi2;
  double drrecotoprunedrecoi2;
  int irecotoprunedreco1;
  double drrecotoprunedreco1;
  int irecotoprunedreco2;
  double drrecotoprunedreco2;
  double dmrecotoprunedreco1;
  double dmrecotoprunedreco2;
  double dprecotoprunedreco1;
  double dprecotoprunedreco2;
  double dphiprunedgentoprunedreco1;
  double detaprunedgentoprunedreco1;
  double dphiprunedgentoprunedreco2;
  double detaprunedgentoprunedreco2;
  double drprunedgentoprunedreco1;
  double drprunedgentoprunedreco2;
  double dmprunedgentoprunedreco1;
  double dmprunedgentoprunedreco2;
  double dpprunedgentoprunedreco1;
  double dpprunedgentoprunedreco2;
 
  



  TTree *dijetWtag = new TTree("dijetWtag", "dijetWtag");                      // creates the dijetWtag tree (ranked, mgg>890) and its branches
  //dijetWtag->Branch("DijetMass",&DijetMass,"DijetMass/D");
  //dijetWtag->Branch("DijetMassNoCHS",&DijetMassNoCHS,"DijetMassNoCHS/D");
  dijetWtag->Branch("DijetMassCA8",&DijetMassCA8,"DijetMassCA8/D");
  //dijetWtag->Branch("DijetMassAK7",&DijetMassAK7,"DijetMassAK7/D");
  dijetWtag->Branch("deta",&deta,"deta/D");                                    //

  dijetWtag->Branch("cteta",&cteta,"cteta/D");                                 // addition of cos teta* just for the lulz
  dijetWtag->Branch("Ht",&Ht,"Ht/D");                                          //
  dijetWtag->Branch("Jet1pt",&Jet1pt,"Jet1pt/D");                              // addition of Ht: sum of the pt 
  dijetWtag->Branch("Jet2pt",&Jet2pt,"Jet2pt/D");                              // 
  dijetWtag->Branch("Jet1eta",&Jet1eta,"Jet1eta/D");                           // addition of dphi = delta phi and met = MET / sum Et

  dijetWtag->Branch("Jet2eta",&Jet2eta,"Jet2eta/D");                           // 
  dijetWtag->Branch("Jet1phi",&Jet1phi,"Jet1phi/D");                           //
  dijetWtag->Branch("Jet2phi",&Jet2phi,"Jet2phi/D");                           // 
  dijetWtag->Branch("dphi",&dphi,"dphi/D");                                    //
  dijetWtag->Branch("met",&met,"met/D");                                       // 
  dijetWtag->Branch("Jet1CA8Mass",&Jet1CA8Mass,"Jet1CA8Mass/D");               // MassDrop: the mass of the biggest subjet divided by the mass of the jet
  dijetWtag->Branch("Jet2CA8Mass",&Jet2CA8Mass,"Jet2CA8Mass/D");               // it's replaced by Nsub, better parameter 
  dijetWtag->Branch("Jet1CA8MassDrop",&Jet1CA8MassDrop,"Jet1CA8MassDrop/D");   // 
  dijetWtag->Branch("Jet2CA8MassDrop",&Jet2CA8MassDrop,"Jet2CA8MassDrop/D");   // Nsub measures the proximity of the jet to a color monopole with regard
  dijetWtag->Branch("Jet1CA8Nsub",&Jet1CA8Nsub,"Jet1CA8Nsub/D");               // to a color dipole 
  dijetWtag->Branch("Jet2CA8Nsub",&Jet2CA8Nsub,"Jet2CA8Nsub/D");               // the smaller Nsub, the purer the jet, ie the closest to a dipole
  dijetWtag->Branch("Jet1dr",&Jet1dr,"Jet1dr/D");                              //
  dijetWtag->Branch("Jet2dr",&Jet2dr,"Jet2dr/D");                              // addition of dr: distance between the 2 subjets in each jet
  dijetWtag->Branch("nbvert",&nbvert,"nbvert/I");                              //
  dijetWtag->Branch("Jet1mef",&Jet1mef,"Jet1mef/D");                           // addition of nbvert: number of primary vertices                         
  dijetWtag->Branch("Jet1nhef",&Jet1nhef,"Jet1nhef/D");                        //                                                     
  dijetWtag->Branch("Jet1neef",&Jet1neef,"Jet1neef/D");                        // addition of mef: muon energy fraction     
  dijetWtag->Branch("Jet1nbcons",&Jet1nbcons,"Jet1nbcons/I");                  //      
  dijetWtag->Branch("Jet1chef",&Jet1chef,"Jet1chef/D");                        // addition of nhef: neutral hadron energy fraction       
  dijetWtag->Branch("Jet1ceef",&Jet1ceef,"Jet1ceef/D");                        // 
  dijetWtag->Branch("Jet1cmult",&Jet1cmult,"Jet1cmult/D");                     // addition of neef: neutral EM  energy fraction  
  dijetWtag->Branch("Jet2mef",&Jet2mef,"Jet2mef/D");                           //                                                 
  dijetWtag->Branch("Jet2nhef",&Jet2nhef,"Jet2nhef/D");                        // addition of nhef: neutral hadron energy fraction  
  dijetWtag->Branch("Jet2neef",&Jet2neef,"Jet2neef/D");                        //      
  dijetWtag->Branch("Jet2nbcons",&Jet2nbcons,"Jet2nbcons/I");                  // addition of nbcons: number of constituants     
  dijetWtag->Branch("Jet2chef",&Jet2chef,"Jet2chef/D");                        //      
  dijetWtag->Branch("Jet2ceef",&Jet2ceef,"Jet2ceef/D");                        // addition of chef: charged hadron energy fraction
  dijetWtag->Branch("Jet2cmult",&Jet2cmult,"Jet2cmult/D");                     // 
  dijetWtag->Branch("weight",&weight,"weight/D");                              // addition of ceef: charged EM energy fraction
  dijetWtag->Branch("categories",&categories,"categories/I");                  // addition of cmult: charged multiplicity


  TTree *jes = new TTree("jes", "jes"); 
  jes->Branch("DijetMassCA8",&DijetMassCA8,"DijetMassCA8/D");
  jes->Branch("DijetMassCA8pruned",&DijetMassCA8pruned,"DijetMassCA8pruned/D");
  jes->Branch("DijetMassCA8dw",&DijetMassCA8dw,"DijetMassCA8dw/D");
  jes->Branch("DijetMassCA8up",&DijetMassCA8up,"DijetMassCA8up/D");
  jes->Branch("DijetMassCA8pruneddw",&DijetMassCA8pruneddw,"DijetMassCA8pruneddw/D");
  jes->Branch("DijetMassCA8prunedup",&DijetMassCA8prunedup,"DijetMassCA8prunedup/D");
  jes->Branch("Jet1CA8Mass",&Jet1CA8Mass,"Jet1CA8Mass/D");               
  jes->Branch("Jet2CA8Mass",&Jet2CA8Mass,"Jet2CA8Mass/D");
  jes->Branch("Jet1CA8Massdw",&Jet1CA8Massdw,"Jet1CA8Massdw/D");               
  jes->Branch("Jet2CA8Massdw",&Jet2CA8Massdw,"Jet2CA8Massdw/D");
  jes->Branch("Jet1CA8Massup",&Jet1CA8Massup,"Jet1CA8Massup/D");               
  jes->Branch("Jet2CA8Massup",&Jet2CA8Massup,"Jet2CA8Massup/D");
  jes->Branch("Jet1CA8Nsub",&Jet1CA8Nsub,"Jet1CA8Nsub/D");              
  jes->Branch("Jet2CA8Nsub",&Jet2CA8Nsub,"Jet2CA8Nsub/D"); 
  jes->Branch("deta",&deta,"deta/D");                                   



  TTree *triggers = new TTree("triggers", "triggers"); 
  triggers->Branch("DijetMassCA8",&DijetMassCA8,"DijetMassCA8/D");
  triggers->Branch("trig",&trig,"trig/I"); 
  triggers->Branch("fat",&fat,"fat/I");                                       // 
  triggers->Branch("ht550",&ht550,"ht550/I");                                 // addition of the trigger booleans FAT, HT550, HT650 & PFHT650
  triggers->Branch("ht650",&ht650,"ht650/I");                                 //
  triggers->Branch("pfht650",&pfht650,"pfht650/I");                           // 
  triggers->Branch("categories",&categories,"categories/I");
                                                                                


  TTree *missingmass = new TTree("missingmass", "missingmass"); 
  missingmass->Branch("genjetmass1",&genjetmass1,"genjetmass1/D");
  missingmass->Branch("genjetmass2",&genjetmass2,"genjetmass2/D");
  missingmass->Branch("prunedgenjetmass1",&prunedgenjetmass1,"prunedgenjetmass1/D");
  missingmass->Branch("prunedgenjetmass2",&prunedgenjetmass2,"prunedgenjetmass2/D");
  missingmass->Branch("dmgentoprunedgen1",&dmgentoprunedgen1,"dmgentoprunedgen1/D");
  missingmass->Branch("dmgentoprunedgen2",&dmgentoprunedgen2,"dmgentoprunedgen2/D");
  missingmass->Branch("dpgentoprunedgen1",&dpgentoprunedgen1,"dpgentoprunedgen1/D");
  missingmass->Branch("dpgentoprunedgen2",&dpgentoprunedgen2,"dpgentoprunedgen2/D");
  missingmass->Branch("drgentoprunedgen1",&drgentoprunedgen1,"drgentoprunedgen1/D");
  missingmass->Branch("drgentoprunedgen2",&drgentoprunedgen2,"drgentoprunedgen2/D");
  missingmass->Branch("recojetmass1",&recojetmass1,"recojetmass1/D");
  missingmass->Branch("recojetmass2",&recojetmass2,"recojetmass2/D");
  missingmass->Branch("dmgentoreco1",&dmgentoreco1,"dmgentoreco1/D");
  missingmass->Branch("dmgentoreco2",&dmgentoreco2,"dmgentoreco2/D");
  missingmass->Branch("dpgentoreco1",&dpgentoreco1,"dpgentoreco1/D");
  missingmass->Branch("dpgentoreco2",&dpgentoreco2,"dpgentoreco2/D");
  missingmass->Branch("drgentoreco1",&drgentoreco1,"drgentoreco1/D");
  missingmass->Branch("drgentoreco2",&drgentoreco2,"drgentoreco2/D");
  missingmass->Branch("prunedrecojetmass1",&prunedrecojetmass1,"prunedrecojetmass1/D");
  missingmass->Branch("prunedrecojetmass2",&prunedrecojetmass2,"prunedrecojetmass2/D");
  missingmass->Branch("dmrecotoprunedreco1",&dmrecotoprunedreco1,"dmrecotoprunedreco1/D");
  missingmass->Branch("dmrecotoprunedreco2",&dmrecotoprunedreco2,"dmrecotoprunedreco2/D");
  missingmass->Branch("dprecotoprunedreco1",&dprecotoprunedreco1,"dprecotoprunedreco1/D");
  missingmass->Branch("dprecotoprunedreco2",&dprecotoprunedreco2,"dprecotoprunedreco2/D");
  missingmass->Branch("drrecotoprunedreco1",&drrecotoprunedreco1,"drrecotoprunedreco1/D");
  missingmass->Branch("drrecotoprunedreco2",&drrecotoprunedreco2,"drrecotoprunedreco2/D");
  missingmass->Branch("dmprunedgentoprunedreco1",&dmprunedgentoprunedreco1,"dmprunedgentoprunedreco1/D");
  missingmass->Branch("dmprunedgentoprunedreco2",&dmprunedgentoprunedreco2,"dmprunedgentoprunedreco2/D");
  missingmass->Branch("dpprunedgentoprunedreco1",&dpprunedgentoprunedreco1,"dpprunedgentoprunedreco1/D");
  missingmass->Branch("dpprunedgentoprunedreco2",&dpprunedgentoprunedreco2,"dpprunedgentoprunedreco2/D");
  missingmass->Branch("drprunedgentoprunedreco1",&drprunedgentoprunedreco1,"drprunedgentoprunedreco1/D");
  missingmass->Branch("drprunedgentoprunedreco2",&drprunedgentoprunedreco2,"drprunedgentoprunedreco2/D");
  missingmass->Branch("deta",&deta,"deta/D");
  missingmass->Branch("met",&met,"met/D");
  missingmass->Branch("categories",&categories,"categories/I"); 



  // Total uncertainty
  JetCorrectionUncertainty *total = new JetCorrectionUncertainty(*(new JetCorrectorParameters("/afs/cern.ch/user/s/sthebaud/private/CMGTools/CMSSW_5_2_5/src/Ntuples/TNMc1/Summer13_V4_DATA_UncertaintySources_AK7PFchs.txt", "Total")));
  




  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  bool hcallasereventfilter2012active=false;
  bool datafiltersactive=false;


  lepdecay0 = 0;
  lepdecay1 = 0;
  lepdecay2 = 0;

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


	  cout << entry << " out of " << nevents <<endl;


	  if(!((jethelperCA8_pt.size()>=2)))                     // if you have less than 2 jets (CA), get out
	      continue;

          njets=0;                                               // counts the jets with a pt above 100
	  for(unsigned n=0;n<jethelper_pt.size();n++)            // 
	    if(jethelper_pt[n]>100)                              // what for? It's never used afterward
	         njets++;                                        
          njetsCA8=0;
	  for(unsigned n=0;n<jethelperCA8_pt.size();n++)         // 
	     if(jethelperCA8_pt[n]>100)                          // ditto with CA
	         njetsCA8++;                                     // 
          

          TLorentzVector Jet1;
          TLorentzVector Jet2;
	  Jet1.SetPtEtaPhiE(jethelper_pt[0],jethelper_eta[0],jethelper_phi[0],jethelper_energy[0]);                           //
	  Jet2.SetPtEtaPhiE(jethelper_pt[1],jethelper_eta[1],jethelper_phi[1],jethelper_energy[1]);                           // useless because no CA?
          DijetMass = (Jet1+Jet2).M();                                                                                        //

	  Jet1.SetPtEtaPhiE(jethelperNoCHS_pt[0],jethelperNoCHS_eta[0],jethelperNoCHS_phi[0],jethelperNoCHS_energy[0]);       //
	  Jet2.SetPtEtaPhiE(jethelperNoCHS_pt[1],jethelperNoCHS_eta[1],jethelperNoCHS_phi[1],jethelperNoCHS_energy[1]);       // useless because noCHS?
          DijetMassNoCHS = (Jet1+Jet2).M();                                                                                   //

                                                                                                                      


	  // ------------------ Dijet Mass to be used -----------------

	  Jet1.SetPtEtaPhiE(jethelperCA8_pt[0],jethelperCA8_eta[0],jethelperCA8_phi[0],jethelperCA8_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelperCA8_pt[1],jethelperCA8_eta[1],jethelperCA8_phi[1],jethelperCA8_energy[1]);
          DijetMassCA8 = (Jet1+Jet2).M();

	  TLorentzVector Jet1pruned;
          TLorentzVector Jet2pruned;

	  Jet1pruned.SetPtEtaPhiE(jethelperCA8pruned_pt[0],jethelperCA8pruned_eta[0],jethelperCA8pruned_phi[0],jethelperCA8pruned_energy[0]);
	  Jet2pruned.SetPtEtaPhiE(jethelperCA8pruned_pt[1],jethelperCA8pruned_eta[1],jethelperCA8pruned_phi[1],jethelperCA8pruned_energy[1]);
          DijetMassCA8pruned = (Jet1pruned+Jet2pruned).M();
	  

	  total->setJetPt(jethelperCA8_pt[0]);                   // total 
	  total->setJetEta(jethelperCA8_eta[0]);                 // uncertainties
	  double uncert1 = total->getUncertainty(true);          // for jet 1 

	  total->setJetPt(jethelperCA8_pt[1]);                   // total 
	  total->setJetEta(jethelperCA8_eta[1]);                 // uncertainties
	  double uncert2 = total->getUncertainty(true);          // for jet 2 

	  
	  TLorentzVector Jet1dw;
          TLorentzVector Jet2dw;
	  TLorentzVector Jet1up;
          TLorentzVector Jet2up;

	  Jet1dw=(1-uncert1)*Jet1;
	  Jet2dw=(1-uncert2)*Jet2;
	  Jet1up=(1+uncert1)*Jet1;
	  Jet2up=(1+uncert2)*Jet2;

	  DijetMassCA8dw = (Jet1dw+Jet2dw).M();
	  DijetMassCA8up = (Jet1up+Jet2up).M();


	  total->setJetPt(jethelperCA8pruned_pt[0]);                   // total 
	  total->setJetEta(jethelperCA8pruned_eta[0]);                 // uncertainties
	  double uncert1pruned = total->getUncertainty(true);          // for jet 1 pruned

	  total->setJetPt(jethelperCA8pruned_pt[1]);                   // total 
	  total->setJetEta(jethelperCA8pruned_eta[1]);                 // uncertainties
	  double uncert2pruned = total->getUncertainty(true);          // for jet 2 pruned

	  
	  TLorentzVector Jet1pruneddw;
          TLorentzVector Jet2pruneddw;
	  TLorentzVector Jet1prunedup;
          TLorentzVector Jet2prunedup;

	  Jet1pruneddw=(1-uncert1pruned)*Jet1pruned;
	  Jet2pruneddw=(1-uncert2pruned)*Jet2pruned;
	  Jet1prunedup=(1+uncert1pruned)*Jet1pruned;
	  Jet2prunedup=(1+uncert2pruned)*Jet2pruned;

	  DijetMassCA8pruneddw = (Jet1pruneddw+Jet2pruneddw).M();
	  DijetMassCA8prunedup = (Jet1prunedup+Jet2prunedup).M();


	  Jet1CA8Massdw = Jet1pruneddw.M();
	  Jet2CA8Massdw = Jet2pruneddw.M();
	  Jet1CA8Massup = Jet1prunedup.M();
	  Jet2CA8Massup = Jet2prunedup.M();
	  


          deta = fabs(jethelperCA8_eta[0]-jethelperCA8_eta[1]);  //
          cteta = tanh(deta/2);                                  //
	  Ht=0;                                                  // 
	  for(unsigned n=0;n<jethelper_pt.size();n++)           
	    Ht = Ht +jethelper_pt[n];
          Jet1pt = jethelperCA8_pt[0];                           //
          Jet2pt = jethelperCA8_pt[1];                           // computes the jets  
          Jet1eta = jethelperCA8_eta[0];                         // dynamics variables
          Jet2eta = jethelperCA8_eta[1];                         //
          Jet1phi = jethelperCA8_phi[0];                         //
          Jet2phi = jethelperCA8_phi[1];                         //
	  dphi = Jet1phi - Jet2phi;                              //
	  if (dphi < -pi)                                        // 
	    dphi = dphi + (2*pi);                                //
	  if (dphi > pi)                                         //
	    dphi = dphi - (2*pi);                                //
	  if (dphi>0)                                            //                                                   
	    dphi = dphi - pi;                                    //                                                    
	  if (dphi<0)                                            //                                                   
	    dphi = -dphi-pi;                                     //                                                  
	  met = (met2_et/met2_sumEt);
	  dphi1 = jethelperCA8pruned_daughter_0_phi[0]-jethelperCA8pruned_daughter_1_phi[0];
	  if (dphi1 < -pi)
	    dphi1 = dphi1 + (2*pi);
	  if (dphi1 > pi)
	    dphi1 = dphi1 - (2*pi);
	  dphi2 = jethelperCA8pruned_daughter_0_phi[1]-jethelperCA8pruned_daughter_1_phi[1];
	  if (dphi2 < -pi)
	    dphi2 = dphi2 + (2*pi);
	  if (dphi2 > pi)
	    dphi2 = dphi2 - (2*pi);

	  Jet1dr = sqrt(((jethelperCA8pruned_daughter_0_eta[0]-jethelperCA8pruned_daughter_1_eta[0])*(jethelperCA8pruned_daughter_0_eta[0]-jethelperCA8pruned_daughter_1_eta[0])) + ((dphi1)*(dphi1)));
	  Jet2dr = sqrt(((jethelperCA8pruned_daughter_0_eta[1]-jethelperCA8pruned_daughter_1_eta[1])*(jethelperCA8pruned_daughter_0_eta[1]-jethelperCA8pruned_daughter_1_eta[1])) + ((dphi2)*(dphi2)));

          

	  Jet1CA8Mass = jethelperCA8pruned_mass[0];              // computes the jets mass and massdrop
          Jet2CA8Mass = jethelperCA8pruned_mass[1];              //
          Jet1CA8MassDrop = max(jethelperCA8pruned_daughter_0_mass[0],jethelperCA8pruned_daughter_1_mass[0])/Jet1CA8Mass/jethelperCA8pruned_uncor_pt[0]*jethelperCA8pruned_pt[0];                                           //
          Jet2CA8MassDrop = max(jethelperCA8pruned_daughter_0_mass[1],jethelperCA8pruned_daughter_1_mass[1])/Jet2CA8Mass/jethelperCA8pruned_uncor_pt[1]*jethelperCA8pruned_pt[1];                                           //

	  if ((jethelperCA8pruned_daughter_0_mass[0]<0.0001)||(jethelperCA8pruned_daughter_1_mass[0]<0.0001)) Jet1CA8MassDrop = 2;  // eliminates the bad
	  if ((jethelperCA8pruned_daughter_0_mass[1]<0.0001)||(jethelperCA8pruned_daughter_1_mass[1]<0.0001)) Jet2CA8MassDrop = 2;  // clustering cases

          Jet1CA8Nsub = jethelperCA8_tau2[0]/jethelperCA8_tau1[0];   // computes the purity
          Jet2CA8Nsub = jethelperCA8_tau2[1]/jethelperCA8_tau1[1];   // variable  Nsub

	  nbvert=eventhelperextra_numberOfPrimaryVertices;

	  Jet1mef = jethelperCA8_muonEnergyFraction[0];
	  Jet1nhef = jethelperCA8_neutralHadronEnergyFraction[0];
	  Jet1neef = jethelperCA8_neutralEmEnergyFraction[0];
	  Jet1nbcons = jethelperCA8_nConstituents[0];
	  Jet1chef = jethelperCA8_chargedHadronEnergyFraction[0];
	  Jet1cmult = jethelperCA8_chargedMultiplicity[0];
	  Jet1ceef = jethelperCA8_chargedEmEnergyFraction[0];
	  Jet2mef = jethelperCA8_muonEnergyFraction[1];
	  Jet2nhef = jethelperCA8_neutralHadronEnergyFraction[1];
	  Jet2neef = jethelperCA8_neutralEmEnergyFraction[1];
	  Jet2nbcons = jethelperCA8_nConstituents[1];
	  Jet2chef = jethelperCA8_chargedHadronEnergyFraction[1];
	  Jet2cmult = jethelperCA8_chargedMultiplicity[1];
	  Jet2ceef = jethelperCA8_chargedEmEnergyFraction[1];
	  
	  if ((triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v10==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v11==1) || (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v12==1))
	    fat=1;
	  else
	    fat=0;
	    
	  if ((triggerresultshelper_HLT_HT550_v1==1) || (triggerresultshelper_HLT_HT550_v2==1) || (triggerresultshelper_HLT_HT550_v3==1) || (triggerresultshelper_HLT_HT550_v4==1) || (triggerresultshelper_HLT_HT550_v5==1) || (triggerresultshelper_HLT_HT550_v6==1) || (triggerresultshelper_HLT_HT550_v7==1) || (triggerresultshelper_HLT_HT550_v8==1) || (triggerresultshelper_HLT_HT550_v9==1))
	    ht550=1;
	  else
	    ht550=0;

	  if ((triggerresultshelper_HLT_HT650_v1==1) || (triggerresultshelper_HLT_HT650_v2==1) || (triggerresultshelper_HLT_HT650_v3==1) || (triggerresultshelper_HLT_HT650_v4==1) || (triggerresultshelper_HLT_HT650_v5==1) || (triggerresultshelper_HLT_HT650_v6==1) || (triggerresultshelper_HLT_HT650_v7==1) || (triggerresultshelper_HLT_HT650_v8==1) || (triggerresultshelper_HLT_HT650_v9==1))
	    ht650=1;
	  else
	    ht650=0;

	  if ((triggerresultshelper_HLT_HT750_v1==1) || (triggerresultshelper_HLT_HT750_v2==1) || (triggerresultshelper_HLT_HT750_v3==1) || (triggerresultshelper_HLT_HT750_v4==1) || (triggerresultshelper_HLT_HT750_v5==1) || (triggerresultshelper_HLT_HT750_v6==1) || (triggerresultshelper_HLT_HT750_v7==1) || (triggerresultshelper_HLT_HT750_v8==1) || (triggerresultshelper_HLT_HT750_v9==1))
	    ht750=1;
	  else
	    ht750=0;

	  if ((triggerresultshelper_HLT_PFNoPUHT650_v1==1) || (triggerresultshelper_HLT_PFNoPUHT650_v2==1) || (triggerresultshelper_HLT_PFNoPUHT650_v3==1) || (triggerresultshelper_HLT_PFNoPUHT650_v4==1) || (triggerresultshelper_HLT_PFNoPUHT650_v5==1) || (triggerresultshelper_HLT_PFNoPUHT650_v6==1))
	    pfnopuht650=1;
	  else
	    pfnopuht650=0;
	  
	  if ((triggerresultshelper_HLT_PFHT650_v1==1) || (triggerresultshelper_HLT_PFHT650_v2==1) || (triggerresultshelper_HLT_PFHT650_v3==1) || (triggerresultshelper_HLT_PFHT650_v4==1) || (triggerresultshelper_HLT_PFHT650_v5==1) || (triggerresultshelper_HLT_PFHT650_v6==1) || (triggerresultshelper_HLT_PFHT650_v7==1) || (triggerresultshelper_HLT_PFHT650_v8==1) || (triggerresultshelper_HLT_PFHT650_v9==1) || (triggerresultshelper_HLT_PFHT650_v10==1) || (triggerresultshelper_HLT_PFHT650_v11==1))
	    pfht650=1;
	  else
	    pfht650=0;

	  

	  if ((fat==1) || (ht750==1) || (pfnopuht650==1) || (pfht650==1))
	    trig=1;
	  else
	    trig=0;


	  
	  
	  
	  


	  // Jet mass peak shift study stuff. Decomment it if you want to fill the associated variables.
	  
	 



	  /*

	  
	  if (njethelperGenCA8>0)
	    {


	      genjetmass1 = jethelperGenCA8_mass[0];
	      genjetmass2 = jethelperGenCA8_mass[1];
	      genjetpt1 = jethelperGenCA8_pt[0];
	      genjetpt2 = jethelperGenCA8_pt[1];
	      genjetphi1 = jethelperGenCA8_phi[0];
	      genjetphi2 = jethelperGenCA8_phi[1];
	      genjeteta1 = jethelperGenCA8_eta[0];
	      genjeteta2 = jethelperGenCA8_eta[1];
	
	      

	      // Association of gen jets with pruned gen jets

	      drgentoprunedgen1=1000;
	      drgentoprunedgen2=1000;

	      for(int i=0;i<njethelperGenCA8pruned;++i)
		{
		  dphigentoprunedgeni1 = genjetphi1 - jethelperGenCA8pruned_phi[i];
		  if (dphigentoprunedgeni1 < -pi)
		    dphigentoprunedgeni1 = dphigentoprunedgeni1 + (2*pi);
		  if (dphigentoprunedgeni1 > pi)
		    dphigentoprunedgeni1 = dphigentoprunedgeni1 - (2*pi);
		  dphigentoprunedgeni2 = genjetphi2 - jethelperGenCA8pruned_phi[i];
		  if (dphigentoprunedgeni2 < -pi)
		    dphigentoprunedgeni2 = dphigentoprunedgeni2 + (2*pi);
		  if (dphigentoprunedgeni2 > pi)
		    dphigentoprunedgeni2 = dphigentoprunedgeni2 - (2*pi);
		  detagentoprunedgeni1 = genjeteta1 - jethelperGenCA8pruned_eta[i];
		  detagentoprunedgeni2 = genjeteta2 - jethelperGenCA8pruned_eta[i];
		  drgentoprunedgeni1 = sqrt((dphigentoprunedgeni1*dphigentoprunedgeni1) + (detagentoprunedgeni1*detagentoprunedgeni1));
		  drgentoprunedgeni2 = sqrt((dphigentoprunedgeni2*dphigentoprunedgeni2) + (detagentoprunedgeni2*detagentoprunedgeni2));
		  if (drgentoprunedgeni1<drgentoprunedgen1)
		    {
		      igentoprunedgen1=i;
		      drgentoprunedgen1=drgentoprunedgeni1;
		    }
		  if (drgentoprunedgeni2<drgentoprunedgen2)
		    {
		      igentoprunedgen2=i;
		      drgentoprunedgen2=drgentoprunedgeni2;
		    }
		}

	      prunedgenjetmass1 = jethelperGenCA8pruned_mass[igentoprunedgen1];
	      prunedgenjetmass2 = jethelperGenCA8pruned_mass[igentoprunedgen2];
	      prunedgenjetpt1 = jethelperGenCA8pruned_pt[igentoprunedgen1];
	      prunedgenjetpt2 = jethelperGenCA8pruned_pt[igentoprunedgen2];
	      prunedgenjetphi1 = jethelperGenCA8pruned_phi[igentoprunedgen1];
	      prunedgenjetphi2 = jethelperGenCA8pruned_phi[igentoprunedgen2];
	      prunedgenjeteta1 = jethelperGenCA8pruned_eta[igentoprunedgen1];
	      prunedgenjeteta2 = jethelperGenCA8pruned_eta[igentoprunedgen2];
	      dmgentoprunedgen1 = genjetmass1 - prunedgenjetmass1;
	      dmgentoprunedgen2 = genjetmass2 - prunedgenjetmass2;
	      dpgentoprunedgen1 = genjetpt1 - prunedgenjetpt1;
	      dpgentoprunedgen2 = genjetpt2 - prunedgenjetpt2;

	  
    

	      // Association of gen jets with reconstructed jets

	      drgentoreco1=1000;
	      drgentoreco2=1000;

	  
	      for(int i=0;i<njethelperCA8;++i)
		{
		  dphigentorecoi1 = genjetphi1 - jethelperCA8_phi[i];
		  if (dphigentorecoi1 < -pi)
		    dphigentorecoi1 = dphigentorecoi1 + (2*pi);
		  if (dphigentorecoi1 > pi)
		    dphigentorecoi1 = dphigentorecoi1 - (2*pi);
		  dphigentorecoi2 = genjetphi2 - jethelperCA8_phi[i];
		  if (dphigentorecoi2 < -pi)
		    dphigentorecoi2 = dphigentorecoi2 + (2*pi);
		  if (dphigentorecoi2 > pi)
		    dphigentorecoi2 = dphigentorecoi2 - (2*pi);
		  detagentorecoi1 = genjeteta1 - jethelperCA8_eta[i];
		  detagentorecoi2 = genjeteta2 - jethelperCA8_eta[i];
		  drgentorecoi1 = sqrt((dphigentorecoi1*dphigentorecoi1) + (detagentorecoi1*detagentorecoi1));
		  drgentorecoi2 = sqrt((dphigentorecoi2*dphigentorecoi2) + (detagentorecoi2*detagentorecoi2));
		  
		  if (drgentorecoi1<drgentoreco1)
		    {
		      igentoreco1=i;
		      drgentoreco1=drgentorecoi1;
		    }
		  if (drgentorecoi2<drgentoreco2)
		    {
		      igentoreco2=i;
		      drgentoreco2=drgentorecoi2;
		    }
		}

	      recojetmass1 = jethelperCA8_mass[igentoreco1];
	      recojetmass2 = jethelperCA8_mass[igentoreco2];
	      recojetpt1 = jethelperCA8_pt[igentoreco1];
	      recojetpt2 = jethelperCA8_pt[igentoreco2];
	      recojetphi1 = jethelperCA8_phi[igentoreco1];
	      recojetphi2 = jethelperCA8_phi[igentoreco2];
	      recojeteta1 = jethelperCA8_eta[igentoreco1];
	      recojeteta2 = jethelperCA8_eta[igentoreco2];
	      dmgentoreco1 = recojetmass1 - genjetmass1;
	      dmgentoreco2 = recojetmass2 - genjetmass2;
	      dpgentoreco1 = recojetpt1 - genjetpt1;
	      dpgentoreco2 = recojetpt2 - genjetpt2;

	      



	      // Association of reco jets with pruned reco jets

	      drrecotoprunedreco1=1000;
	      drrecotoprunedreco2=1000;

	      for(int i=0;i<njethelperCA8pruned;++i)
		{
		  dphirecotoprunedrecoi1 = recojetphi1 - jethelperCA8pruned_phi[i];
		  if (dphirecotoprunedrecoi1 < -pi)
		    dphirecotoprunedrecoi1 = dphirecotoprunedrecoi1 + (2*pi);
		  if (dphirecotoprunedrecoi1 > pi)
		    dphirecotoprunedrecoi1 = dphirecotoprunedrecoi1 - (2*pi);
		  dphirecotoprunedrecoi2 = recojetphi2 - jethelperCA8pruned_phi[i];
		  if (dphirecotoprunedrecoi2 < -pi)
		    dphirecotoprunedrecoi2 = dphirecotoprunedrecoi2 + (2*pi);
		  if (dphirecotoprunedrecoi2 > pi)
		    dphirecotoprunedrecoi2 = dphirecotoprunedrecoi2 - (2*pi);
		  detarecotoprunedrecoi1 = recojeteta1 - jethelperCA8pruned_eta[i];
		  detarecotoprunedrecoi2 = recojeteta2 - jethelperCA8pruned_eta[i];
		  drrecotoprunedrecoi1 = sqrt((dphirecotoprunedrecoi1*dphirecotoprunedrecoi1) + (detarecotoprunedrecoi1*detarecotoprunedrecoi1));
		  drrecotoprunedrecoi2 = sqrt((dphirecotoprunedrecoi2*dphirecotoprunedrecoi2) + (detarecotoprunedrecoi2*detarecotoprunedrecoi2));
		  if (drrecotoprunedrecoi1<drrecotoprunedreco1)
		    {
		      irecotoprunedreco1=i;
		      drrecotoprunedreco1=drrecotoprunedrecoi1;
		    }
		  if (drrecotoprunedrecoi2<drrecotoprunedreco2)
		    {
		      irecotoprunedreco2=i;
		      drrecotoprunedreco2=drrecotoprunedrecoi2;
		    }
		}

	      prunedrecojetmass1 = jethelperCA8pruned_mass[irecotoprunedreco1];
	      prunedrecojetmass2 = jethelperCA8pruned_mass[irecotoprunedreco2];
	      prunedrecojetpt1 = jethelperCA8pruned_pt[irecotoprunedreco1];
	      prunedrecojetpt2 = jethelperCA8pruned_pt[irecotoprunedreco2];
	      prunedrecojetphi1 = jethelperCA8pruned_phi[irecotoprunedreco1];
	      prunedrecojetphi2 = jethelperCA8pruned_phi[irecotoprunedreco2];
	      prunedrecojeteta1 = jethelperCA8pruned_eta[irecotoprunedreco1];
	      prunedrecojeteta2 = jethelperCA8pruned_eta[irecotoprunedreco2];
	      dmrecotoprunedreco1 = recojetmass1 - prunedrecojetmass1;
	      dmrecotoprunedreco2 = recojetmass2 - prunedrecojetmass2;
	      dprecotoprunedreco1 = recojetpt1 - prunedrecojetpt1;
	      dprecotoprunedreco2 = recojetpt2 - prunedrecojetpt2;


	  


	      // Variables for pruned gen to pruned reconstructed

	  
	      dphiprunedgentoprunedreco1 = prunedgenjetphi1 - prunedrecojetphi1;
	      if (dphiprunedgentoprunedreco1 < -pi)
		dphiprunedgentoprunedreco1 = dphiprunedgentoprunedreco1 + (2*pi);
	      if (dphiprunedgentoprunedreco1 > pi)
		dphiprunedgentoprunedreco1 = dphiprunedgentoprunedreco1 - (2*pi);
	      dphiprunedgentoprunedreco2 = prunedgenjetphi2 - prunedrecojetphi2;
	      if (dphiprunedgentoprunedreco2 < -pi)
		dphiprunedgentoprunedreco2 = dphiprunedgentoprunedreco2 + (2*pi);
	      if (dphiprunedgentoprunedreco2 > pi)
		dphiprunedgentoprunedreco2 = dphiprunedgentoprunedreco2 - (2*pi);
	      detaprunedgentoprunedreco1 = prunedgenjeteta1 - prunedrecojeteta1;
	      detaprunedgentoprunedreco2 = prunedgenjeteta2 - prunedrecojeteta2;
	      drprunedgentoprunedreco1 = sqrt((dphiprunedgentoprunedreco1*dphiprunedgentoprunedreco1) + (detaprunedgentoprunedreco1*detaprunedgentoprunedreco1));
	      drprunedgentoprunedreco2 = sqrt((dphiprunedgentoprunedreco2*dphiprunedgentoprunedreco2) + (detaprunedgentoprunedreco2*detaprunedgentoprunedreco2));
	      dmprunedgentoprunedreco1 = prunedgenjetmass1 - prunedrecojetmass1;
	      dmprunedgentoprunedreco2 = prunedgenjetmass2 - prunedrecojetmass2;
	      dpprunedgentoprunedreco1 = prunedgenjetpt1 - prunedrecojetpt1;
	      dpprunedgentoprunedreco2 = prunedgenjetpt2 - prunedrecojetpt2;

	    }



	  */











	  //_____________________________________________________________________
  
	  Jet1CA8C2beta17 = jethelperCA8_C2beta17[0];                // honestly I don't care, they're
          Jet2CA8C2beta17 = jethelperCA8_C2beta17[1];                // never used afterward
 
           
          if (triggerresultshelper_hcallasereventfilter2012!=0)                       //
	    hcallasereventfilter2012active=true;                                      //
                                                                                      //
	  if((triggerresultshelper_primaryVertexFilterPath!=0)&&                      //
	     (triggerresultshelper_noscrapingFilterPath!=0)&&                         //  eliminates the
	     (triggerresultshelper_trackingFailureFilterPath!=0)&&                    //  detector noise
	     (triggerresultshelper_hcalLaserEventFilterPath!=0)&&                     //
	     (triggerresultshelper_HBHENoiseFilterPath!=0)&&                          //
	     (triggerresultshelper_CSCTightHaloFilterPath!=0)&&                       //
	     (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0))        //
	    datafiltersactive=true;                                                   //

          //if(DijetMass>1600)
          //    cout << "every " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << endl;

	  bool Jet1_Kinematic_Selection = (Jet1pt>30) && (fabs(Jet1eta)<2.5);                       // jets with pt<30 are not 
	  bool Jet2_Kinematic_Selection = (Jet2pt>30) && (fabs(Jet2eta)<2.5);                       //
                                                                                                    // measurable anyway
	  bool Jets_Kinematic_Selection = Jet1_Kinematic_Selection && Jet2_Kinematic_Selection;     //

	  bool Jet1_Tight_Id =   (jethelperCA8_muonEnergyFraction[0]<0.80)&&        //
	    (jethelperCA8_neutralHadronEnergyFraction[0]<0.90)&&                    //
	    (jethelperCA8_neutralEmEnergyFraction[0]<0.90)&&                        //
	    (jethelperCA8_nConstituents[0]>1)&&                                     //
	    ((fabs(jethelper_eta[0])>2.4)||                                         //
	     ((jethelperCA8_chargedHadronEnergyFraction[0]>0.01)&&                  //
	      (jethelperCA8_chargedMultiplicity[0]>0)&&                             //
	      (jethelperCA8_chargedEmEnergyFraction[0]<0.99)));                     //
                                                                                    // 
	  bool Jet2_Tight_Id  = (jethelperCA8_muonEnergyFraction[1]<0.80)&&         // background
	     (jethelperCA8_neutralHadronEnergyFraction[1]<0.90)&&                   // filtering
	     (jethelperCA8_neutralEmEnergyFraction[1]<0.90)&&                       //
	     (jethelperCA8_nConstituents[1]>1)&&                                    //
	     ((fabs(jethelperCA8_eta[1])>2.4)||                                     //
	      ((jethelperCA8_chargedHadronEnergyFraction[1]>0.01)&&                 //
	       (jethelperCA8_chargedMultiplicity[1]>0)&&                            //
	       (jethelperCA8_chargedEmEnergyFraction[1]<0.99)));                    //
	                                                                            //
	  bool Jets_Id_Selection =  Jet1_Tight_Id && Jet2_Tight_Id;                 //


	  bool Jets_Selection = Jets_Kinematic_Selection && Jets_Id_Selection;

	  bool noiseRemoval = (((sdouble_kt6PFJets_rho_value<40)&&                                             //
	       (triggerresultshelper_primaryVertexFilterPath!=0)&&                                             //
	       (triggerresultshelper_noscrapingFilterPath!=0)&&                                                //
	       (triggerresultshelper_trackingFailureFilterPath!=0)&&                                           //
	       (triggerresultshelper_hcalLaserEventFilterPath!=0)&&                                            // eliminates the
	       (triggerresultshelper_HBHENoiseFilterPath!=0)&&                                                 // detector noise
	       (triggerresultshelper_CSCTightHaloFilterPath!=0)&&                                              //
	       (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0))||                             //
	      (datafiltersactive==false))&&                                                                    //
	    ((triggerresultshelper_hcallasereventfilter2012!=0)||(hcallasereventfilter2012active==false));     //


	  bool centralitySelection = deta<1.3;                 // centrality condition

	  if(!(noiseRemoval && Jets_Selection)) continue;


	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

	  jes->Fill();

          if(geneventinfoproduct_weight>0)
	      weight=geneventinfoproduct_weight;
	  else
	      weight=1;

	  weight = weight*vertexWeight;

          // if((Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(DijetMassCA8>890))
          //    cout << "filtered " << eventhelper_run << ":" << eventhelper_luminosityBlock << ":" << eventhelper_event << ":" << DijetMass << ":" << Jet1CA8Mass << ":" << Jet2CA8Mass << ":" << Jet1CA8Nsub << ":" << Jet2CA8Nsub << endl;

          categories=-1;
          categoriesNS=-1;
          mgg = DijetMassCA8;


	  mass->Fill(DijetMassCA8, weight);                                                        // histogram of the masses of all the ranked events

	  if((!((Jet1CA8Mass>110)&&(Jet1CA8Mass<135)))&&                                           // no jet in the
	     (!((Jet2CA8Mass>110)&&(Jet2CA8Mass<135))))                                            // mass window
	  {
              mass_0mtag->Fill(DijetMassCA8, weight);
	      categories = 5;
	  }
	  if(((((Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet1CA8Nsub<0.75)&&(Jet1CA8Nsub>0.5)))&&    // 1 unpure jet in 
	      (!((Jet2CA8Mass>110)&&(Jet2CA8Mass<135))))||                                         // the mass window,
 	     ((((Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet2CA8Nsub<0.75)&&(Jet2CA8Nsub>0.5)))&&    // the other out of 
	      (!((Jet1CA8Mass>110)&&(Jet1CA8Mass<135)))))                                          // the mass window
	  {
              mass_1mtag_1lptag->Fill(DijetMassCA8, weight);
	      categories = 4;
	  }
	  if(((((Jet1CA8Mass>110)&&(Jet1CA8Mass<135)&&(Jet1CA8Nsub<0.5)))&&                        // 1 pure jet in
	      (!((Jet2CA8Mass>110)&&(Jet2CA8Mass<135))))||                                         // the mass window
 	     ((((Jet2CA8Mass>110)&&(Jet2CA8Mass<135)&&(Jet2CA8Nsub<0.5)))&&                        // the other out of
	      (!((Jet1CA8Mass>110)&&(Jet1CA8Mass<135)))))                                          // the mass window
	  {
              mass_1mtag_1hptag->Fill(DijetMassCA8, weight);
	      categories = 3;
	  }
	  if(((Jet1CA8Mass>110)&&(Jet1CA8Mass<135))&&                                              // the 2 jets are 
	     ((Jet2CA8Mass>110)&&(Jet2CA8Mass<135))&&                                              // unpure but in 
	     ((Jet1CA8Nsub<0.75)&&(Jet2CA8Nsub<0.75)&&(Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub>0.5)))       // the mass window
	  {
              mass_2mtag_2lptag->Fill(DijetMassCA8, weight);
	      categories = 2;
	  }
	  if(((Jet1CA8Mass>110)&&(Jet1CA8Mass<135))&&                                              // the 2 jets are in 
	     ((Jet2CA8Mass>110)&&(Jet2CA8Mass<135))&&                                              // the mass window
	     (((Jet1CA8Nsub<0.75)&&(Jet1CA8Nsub>0.5)&&(Jet2CA8Nsub<0.5))||                         // but one is pure
	      ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.75)&&(Jet2CA8Nsub>0.5))))                         // and the other unpure
	  {
              mass_2mtag_1hptag_1lptag->Fill(DijetMassCA8, weight);
	      categories = 1;
	  }
	  if(((Jet1CA8Mass>110)&&(Jet1CA8Mass<135))&&                                              // the 2 jets are
	     ((Jet2CA8Mass>110)&&(Jet2CA8Mass<135))&&                                              // pure and in
	     ((Jet1CA8Nsub<0.5)&&(Jet2CA8Nsub<0.5)))                                               // the mass window
	  {
              mass_2mtag_2hptag->Fill(DijetMassCA8, weight);
	      categories = 0;
	  }

          if(mgg>890) 
	    {                                                                    // dijetWtag and missing mass gather the events with Mjj>890
	      dijetWtag->Fill();                                                 // TCVARS gathers, among these events, those with deta<1.3
	      missingmass->Fill();
	    }
	  triggers->Fill();

	}
  stream.close();
  ofile.close();

  return 0;
}
