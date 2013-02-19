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
  stream.select("patJetHelper_selectedPatJets.chargedEmEnergyFraction", jethelper2_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.chargedHadronEnergyFraction", jethelper2_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.chargedMultiplicity", jethelper2_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJets.energy", jethelper2_energy);
  stream.select("patJetHelper_selectedPatJets.eta", jethelper2_eta);
  stream.select("patJetHelper_selectedPatJets.rapidity", jethelper2_rapidity);
  stream.select("patJetHelper_selectedPatJets.mass", jethelper2_mass);
  stream.select("patJetHelper_selectedPatJets.muonEnergyFraction", jethelper2_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.nConstituents", jethelper2_nConstituents);
  stream.select("patJetHelper_selectedPatJets.neutralEmEnergyFraction", jethelper2_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.neutralHadronEnergyFraction", jethelper2_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJets.phi", jethelper2_phi);
  stream.select("patJetHelper_selectedPatJets.pt", jethelper2_pt);
  stream.select("patJetHelper_selectedPatJetsCHS.chargedEmEnergyFraction", jethelper_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.chargedHadronEnergyFraction", jethelper_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.chargedMultiplicity", jethelper_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCHS.combinedSecondaryVertexBJetTags", jethelper_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.combinedSecondaryVertexMVABJetTags", jethelper_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.energy", jethelper_energy);
  stream.select("patJetHelper_selectedPatJetsCHS.eta", jethelper_eta);
  stream.select("patJetHelper_selectedPatJetsCHS.rapidity", jethelper_rapidity);
  stream.select("patJetHelper_selectedPatJetsCHS.jetBProbabilityBJetTags", jethelper_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.jetProbabilityBJetTags", jethelper_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCHS.mass", jethelper_mass);
  stream.select("patJetHelper_selectedPatJetsCHS.muonEnergyFraction", jethelper_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.nConstituents", jethelper_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCHS.neutralEmEnergyFraction", jethelper_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.neutralHadronEnergyFraction", jethelper_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.phi", jethelper_phi);
  stream.select("patJetHelper_selectedPatJetsCHS.pt", jethelper_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.numberOfDaughters", jethelper_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.combinedSecondaryVertexBJetTags", jethelper3_combinedSecondaryVertexBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.combinedSecondaryVertexMVABJetTags", jethelper3_combinedSecondaryVertexMVABJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.energy", jethelper3_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.et", jethelper3_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.eta", jethelper3_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.genJet_energy", jethelper3_genJet_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.genJet_eta", jethelper3_genJet_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.genJet_phi", jethelper3_genJet_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.genJet_pt", jethelper3_genJet_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.genParton_pdgId", jethelper3_genParton_pdgId);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.jetArea", jethelper3_jetArea);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.jetBProbabilityBJetTags", jethelper3_jetBProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.jetProbabilityBJetTags", jethelper3_jetProbabilityBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.mass", jethelper3_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.nConstituents", jethelper3_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.partonFlavour", jethelper3_partonFlavour);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.phi", jethelper3_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.pt", jethelper3_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.simpleSecondaryVertexHighEffBJetTags", jethelper3_simpleSecondaryVertexHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.simpleSecondaryVertexHighPurBJetTags", jethelper3_simpleSecondaryVertexHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.trackCountingHighEffBJetTags", jethelper3_trackCountingHighEffBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.trackCountingHighPurBJetTags", jethelper3_trackCountingHighPurBJetTags);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.uncor_energy", jethelper3_uncor_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.uncor_et", jethelper3_uncor_et);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.uncor_pt", jethelper3_uncor_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_chargedEmEnergyFraction", jethelper_daughter_0_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_chargedHadronEnergyFraction", jethelper_daughter_0_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_chargedMultiplicity", jethelper_daughter_0_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_energy", jethelper_daughter_0_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_eta", jethelper_daughter_0_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_mass", jethelper_daughter_0_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_muonEnergyFraction", jethelper_daughter_0_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_nConstituents", jethelper_daughter_0_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_neutralEmEnergyFraction", jethelper_daughter_0_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_neutralHadronEnergyFraction", jethelper_daughter_0_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_phi", jethelper_daughter_0_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_photonEnergyFraction", jethelper_daughter_0_photonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_0_pt", jethelper_daughter_0_pt);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_chargedEmEnergyFraction", jethelper_daughter_1_chargedEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_chargedHadronEnergyFraction", jethelper_daughter_1_chargedHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_chargedMultiplicity", jethelper_daughter_1_chargedMultiplicity);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_energy", jethelper_daughter_1_energy);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_eta", jethelper_daughter_1_eta);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_mass", jethelper_daughter_1_mass);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_muonEnergyFraction", jethelper_daughter_1_muonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_nConstituents", jethelper_daughter_1_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_neutralEmEnergyFraction", jethelper_daughter_1_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_neutralHadronEnergyFraction", jethelper_daughter_1_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_phi", jethelper_daughter_1_phi);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_photonEnergyFraction", jethelper_daughter_1_photonEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCA8CHSpruned.daughter_1_pt", jethelper_daughter_1_pt);
  stream.select("patMET_patMETsRaw.et", met2_et);
  stream.select("patMET_patMETsRaw.sumEt", met2_sumEt);
  stream.select("npatJetHelper_selectedPatJetsCHS", njethelper);
  stream.select("npatJetHelper_selectedPatJets", njethelper2);
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

  stream.select("patJetHelper_selectedPatJetsCA8CHS.tau1", jethelper_tau1);
  stream.select("patJetHelper_selectedPatJetsCA8CHS.tau2", jethelper_tau2);

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
  
  TH1F* mass_2Wtag=new TH1F("dijet_mass_2Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Wtag->Sumw2();
  TH1F* massNoCHS_2Wtag=new TH1F("dijet_massNoCHS_2Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  massNoCHS_2Wtag->Sumw2();
  TH1F* massCA8_2Wtag=new TH1F("dijet_massCA8_2Wtag","M_{jj}",NBINS-1,BOUNDARIES);
  massCA8_2Wtag->Sumw2();
  TH1F* mass_2Nsubtag=new TH1F("dijet_mass_2Nsubtag","M_{jj}",NBINS-1,BOUNDARIES);
  mass_2Nsubtag->Sumw2();

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  double weight=1;

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
	  Jet1.SetPtEtaPhiE(jethelper2_pt[0],jethelper2_eta[0],jethelper2_phi[0],jethelper2_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelper2_pt[1],jethelper2_eta[1],jethelper2_phi[1],jethelper2_energy[1]);
          double DijetMassNoCHS = (Jet1+Jet2).M();
	  Jet1.SetPtEtaPhiE(jethelper3_pt[0],jethelper3_eta[0],jethelper3_phi[0],jethelper3_energy[0]);
	  Jet2.SetPtEtaPhiE(jethelper3_pt[1],jethelper3_eta[1],jethelper3_phi[1],jethelper3_energy[1]);
          double DijetMassCA8 = (Jet1+Jet2).M();
          double JetMass1 = jethelper3_mass[0];
          double JetMass2 = jethelper3_mass[1];
          double JetMassDrop1 = max(jethelper_daughter_0_mass[0],jethelper_daughter_1_mass[0])/JetMass1;
          double JetMassDrop2 = max(jethelper_daughter_0_mass[1],jethelper_daughter_1_mass[1])/JetMass2;
          double JetNsub1 = jethelper_tau2[0]/jethelper_tau1[0];
          double JetNsub2 = jethelper_tau2[1]/jethelper_tau1[1];
           
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
	     (sdouble_kt6PFJets_rho_value<40)&&
	     (triggerresultshelper_primaryVertexFilterPath!=0)&&
	     (triggerresultshelper_noscrapingFilterPath!=0)&&
	     (triggerresultshelper_trackingFailureFilterPath!=0)&&
	     (triggerresultshelper_hcalLaserEventFilterPath!=0)&&
	     (triggerresultshelper_HBHENoiseFilterPath!=0)&&
	     (triggerresultshelper_CSCTightHaloFilterPath!=0)&&
	     (triggerresultshelper_EcalDeadCellTriggerPrimitiveFilterPath!=0)&&
	     (triggerresultshelper_hcallasereventfilter2012!=0)
	    )) continue;
	   
	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

          if(geneventinfoproduct_weight>0)
	      weight=geneventinfoproduct_weight;
	  else
	      weight=1;

	  if(((JetMass1>70)&&(JetMass1<100)&&(JetMassDrop1<0.25)) &&
	     ((JetMass2>70)&&(JetMass2<100)&&(JetMassDrop2<0.25)))
	  {
              mass_2Wtag->Fill(DijetMass, weight);
              massNoCHS_2Wtag->Fill(DijetMassNoCHS, weight);
              massCA8_2Wtag->Fill(DijetMassCA8, weight);
	  }
	  if(((JetMass1>70)&&(JetMass1<100)&&(JetNsub1<0.4)) &&
	     ((JetMass2>70)&&(JetMass2<100)&&(JetNsub2<0.4)))
	  {
              mass_2Nsubtag->Fill(DijetMass, weight);
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
  mass_2Nsubtag->SetLineColor(4);
  mass_2Nsubtag->Draw("hesame");
  c1.SaveAs((cmdline.outputfilename.substr(0,cmdline.outputfilename.size()-5)+"_Wtag_mass.pdf").c_str());
  c1.SaveAs((cmdline.outputfilename.substr(0,cmdline.outputfilename.size()-5)+"_Wtag_mass.root").c_str());

  stream.close();
  ofile.close();

  return 0;
}
