//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by TheNtupleMaker
// Created:     Thu May 31 21:49:25 2012 by mkntanalyzer.py
// Author:      Andreas Hinzmann
//-----------------------------------------------------------------------------
#include "chi_analysis.h"

#ifdef PROJECT_NAME
#include "PhysicsTools/TheNtupleMaker/interface/pdg.h"
#else
#include "pdg.h"
#endif

#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Get file list and histogram filename from command line

  commandLine cmdline;
  decodeCommandLine(argc, argv, cmdline);

  // Get names of ntuple files to be processed and open chain of ntuples

  vector<string> filenames = getFilenames(cmdline.filelist);
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
  stream.select("patJetHelper_selectedPatJetsCHS.nConstituents", jethelper_nConstituents);
  stream.select("patJetHelper_selectedPatJetsCHS.neutralEmEnergyFraction", jethelper_neutralEmEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHS.neutralHadronEnergyFraction", jethelper_neutralHadronEnergyFraction);
  stream.select("patJetHelper_selectedPatJetsCHSpruned.numberOfDaughters", jethelper_numberOfDaughters);
  stream.select("patJetHelper_selectedPatJetsCHS.phi", jethelper_phi);
  stream.select("patJetHelper_selectedPatJetsCHS.pt", jethelper_pt);
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
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9", triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v1", triggerresultshelper_HLT_HT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v2", triggerresultshelper_HLT_HT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v3", triggerresultshelper_HLT_HT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v4", triggerresultshelper_HLT_HT650_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v5", triggerresultshelper_HLT_HT650_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v6", triggerresultshelper_HLT_HT650_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v7", triggerresultshelper_HLT_HT650_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v8", triggerresultshelper_HLT_HT650_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT650_v9", triggerresultshelper_HLT_HT650_v9);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v1", triggerresultshelper_HLT_HT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v2", triggerresultshelper_HLT_HT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v3", triggerresultshelper_HLT_HT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v4", triggerresultshelper_HLT_HT750_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v5", triggerresultshelper_HLT_HT750_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v6", triggerresultshelper_HLT_HT750_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v7", triggerresultshelper_HLT_HT750_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v8", triggerresultshelper_HLT_HT750_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_HT750_v9", triggerresultshelper_HLT_HT750_v9);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v1", triggerresultshelper_HLT_PFHT650_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v2", triggerresultshelper_HLT_PFHT650_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v3", triggerresultshelper_HLT_PFHT650_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v4", triggerresultshelper_HLT_PFHT650_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v5", triggerresultshelper_HLT_PFHT650_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v6", triggerresultshelper_HLT_PFHT650_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v7", triggerresultshelper_HLT_PFHT650_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v8", triggerresultshelper_HLT_PFHT650_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT650_v9", triggerresultshelper_HLT_PFHT650_v9);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v1", triggerresultshelper_HLT_PFHT700_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v2", triggerresultshelper_HLT_PFHT700_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v3", triggerresultshelper_HLT_PFHT700_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v4", triggerresultshelper_HLT_PFHT700_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v5", triggerresultshelper_HLT_PFHT700_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v6", triggerresultshelper_HLT_PFHT700_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v7", triggerresultshelper_HLT_PFHT700_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v8", triggerresultshelper_HLT_PFHT700_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT700_v9", triggerresultshelper_HLT_PFHT700_v9);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v1", triggerresultshelper_HLT_PFHT750_v1);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v2", triggerresultshelper_HLT_PFHT750_v2);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v3", triggerresultshelper_HLT_PFHT750_v3);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v4", triggerresultshelper_HLT_PFHT750_v4);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v5", triggerresultshelper_HLT_PFHT750_v5);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v6", triggerresultshelper_HLT_PFHT750_v6);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v7", triggerresultshelper_HLT_PFHT750_v7);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v8", triggerresultshelper_HLT_PFHT750_v8);
  stream.select("edmTriggerResultsHelper_TriggerResults_HLT.HLT_PFHT750_v9", triggerresultshelper_HLT_PFHT750_v9);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.hcalLaserEventFilterPath", triggerresultshelper_hcalLaserEventFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.noscrapingFilterPath", triggerresultshelper_noscrapingFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.primaryVertexFilterPath", triggerresultshelper_primaryVertexFilterPath);
  stream.select("edmTriggerResultsHelper_TriggerResults_PAT.trackingFailureFilterPath", triggerresultshelper_trackingFailureFilterPath);

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

  std::vector<double> massBins = {400,600,800,1000,1200,1500,1900,2400,3000,4000,5000,7000};

  TCanvas c1("c1","c1",200,200);
  TH1F* h1=new TH1F("dijet_mass","M_{jj}",65,1500,8000);
  h1->Sumw2();
  
  std::vector<TH1F*> hists;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "chi";
      hists.push_back(new TH1F(name.str().c_str(),name.str().c_str(),15,1,16));
      hists[j]->Sumw2();
  }
  
  std::vector<TH1F*> histspt1;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "pt1";
      histspt1.push_back(new TH1F(name.str().c_str(),name.str().c_str(),30,0,3000));
      histspt1[j]->Sumw2();
  }
  
  std::vector<TH1F*> histspt2;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "pt2";
      histspt2.push_back(new TH1F(name.str().c_str(),name.str().c_str(),30,0,3000));
      histspt2[j]->Sumw2();
  }
  
  std::vector<TH1F*> histsy1;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "y1";
      histsy1.push_back(new TH1F(name.str().c_str(),name.str().c_str(),25,-2.5,2.5));
      histsy1[j]->Sumw2();
  }
  
  std::vector<TH1F*> histsy2;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "y2";
      histsy2.push_back(new TH1F(name.str().c_str(),name.str().c_str(),25,-2.5,2.5));
      histsy2[j]->Sumw2();
  }
  
  std::vector<TH1F*> histsyboost;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "yboost";
      histsyboost.push_back(new TH1F(name.str().c_str(),name.str().c_str(),12,0,1.2));
      histsyboost[j]->Sumw2();
  }
  
  std::vector<TH1F*> histsmetsumet;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "metsumet";
      histsmetsumet.push_back(new TH1F(name.str().c_str(),name.str().c_str(),20,0,1.00001));
      histsmetsumet[j]->Sumw2();
  }
  
  std::vector<TH1F*> histsdphi;
  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      std::stringstream name;
      name << "dijet_" << massBins[j] << "_" << massBins[j+1] << "_" << "dphi";
      histsdphi.push_back(new TH1F(name.str().c_str(),name.str().c_str(),20,0,3.15));
      histsdphi[j]->Sumw2();
  }
  
  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  std::cout << "event..." << std::endl;

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
	 
	  if(entry%100000==0) std::cout << entry << "..." << std::endl;
	  
	  // ---------------------
	  // -- event selection --
	  // ---------------------
           
	  if(!((jethelper2_pt.size()>=2)&&
	     (jethelper2_pt[0]>30)&&
	     (jethelper2_pt[1]>30)&&
	     (fabs(jethelper2_rapidity[0])<2.5)&&
	     (fabs(jethelper2_rapidity[1])<2.5)&&
             (fabs(jethelper2_rapidity[0]+jethelper2_rapidity[1])/2.<1.11)&&
             (exp(fabs(jethelper2_rapidity[0]-jethelper2_rapidity[1]))<16)&&
	     
	     //(jethelper2_neutralHadronEnergyFraction[0]<0.99)&&
	     (jethelper2_neutralHadronEnergyFraction[0]<0.90)&&
	     //(jethelper2_neutralEmEnergyFraction[0]<0.99)&&
	     (jethelper2_neutralEmEnergyFraction[0]<0.90)&&
	     (jethelper2_nConstituents[0]>1)&&
	     ((fabs(jethelper2_eta[0])>2.4)||
	      ((jethelper2_chargedHadronEnergyFraction[0]>0)&&
	       (jethelper2_chargedMultiplicity[0]>0)&&
	       (jethelper2_chargedEmEnergyFraction[0]<0.99)))&&
	     
	     //(jethelper2_neutralHadronEnergyFraction[1]<0.99)&&
	     (jethelper2_neutralHadronEnergyFraction[1]<0.90)&&
	     //(jethelper2_neutralEmEnergyFraction[1]<0.99)&&
	     (jethelper2_neutralEmEnergyFraction[1]<0.90)&&
	     (jethelper2_nConstituents[1]>1)&&
	     ((fabs(jethelper2_eta[1])>2.4)||
	      ((jethelper2_chargedHadronEnergyFraction[1]>0)&&
	       (jethelper2_chargedMultiplicity[1]>0)&&
	       (jethelper2_chargedEmEnergyFraction[1]<0.99)))&&
	     
	     (triggerresultshelper_primaryVertexFilterPath)&&
	     (triggerresultshelper_noscrapingFilterPath)&&
	     (triggerresultshelper_hcalLaserEventFilterPath)&&

	     ((triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v1)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v2)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v3)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v4)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v5)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v6)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v7)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v8)||
	      (triggerresultshelper_HLT_FatDiPFJetMass750_DR1p1_Deta1p5_v9)||
	      (triggerresultshelper_HLT_PFHT650_v1)||
	      (triggerresultshelper_HLT_PFHT650_v2)||
	      (triggerresultshelper_HLT_PFHT650_v3)||
	      (triggerresultshelper_HLT_PFHT650_v4)||
	      (triggerresultshelper_HLT_PFHT650_v5)||
	      (triggerresultshelper_HLT_PFHT650_v6)||
	      (triggerresultshelper_HLT_PFHT650_v7)||
	      (triggerresultshelper_HLT_PFHT650_v8)||
	      (triggerresultshelper_HLT_PFHT650_v9)||
	      (triggerresultshelper_HLT_PFHT700_v1)||
	      (triggerresultshelper_HLT_PFHT700_v2)||
	      (triggerresultshelper_HLT_PFHT700_v3)||
	      (triggerresultshelper_HLT_PFHT700_v4)||
	      (triggerresultshelper_HLT_PFHT700_v5)||
	      (triggerresultshelper_HLT_PFHT700_v6)||
	      (triggerresultshelper_HLT_PFHT700_v7)||
	      (triggerresultshelper_HLT_PFHT700_v8)||
	      (triggerresultshelper_HLT_PFHT700_v9)||
	      (triggerresultshelper_HLT_PFHT750_v1)||
	      (triggerresultshelper_HLT_PFHT750_v2)||
	      (triggerresultshelper_HLT_PFHT750_v3)||
	      (triggerresultshelper_HLT_PFHT750_v4)||
	      (triggerresultshelper_HLT_PFHT750_v5)||
	      (triggerresultshelper_HLT_PFHT750_v6)||
	      (triggerresultshelper_HLT_PFHT750_v7)||
	      (triggerresultshelper_HLT_PFHT750_v8)||
	      (triggerresultshelper_HLT_PFHT750_v9)||
	      (triggerresultshelper_HLT_HT650_v1)||
	      (triggerresultshelper_HLT_HT650_v2)||
	      (triggerresultshelper_HLT_HT650_v3)||
	      (triggerresultshelper_HLT_HT650_v4)||
	      (triggerresultshelper_HLT_HT650_v5)||
	      (triggerresultshelper_HLT_HT650_v6)||
	      (triggerresultshelper_HLT_HT650_v7)||
	      (triggerresultshelper_HLT_HT650_v8)||
	      (triggerresultshelper_HLT_HT650_v9)||
	      (triggerresultshelper_HLT_HT750_v1)||
	      (triggerresultshelper_HLT_HT750_v2)||
	      (triggerresultshelper_HLT_HT750_v3)||
	      (triggerresultshelper_HLT_HT750_v4)||
	      (triggerresultshelper_HLT_HT750_v5)||
	      (triggerresultshelper_HLT_HT750_v6)||
	      (triggerresultshelper_HLT_HT750_v7)||
	      (triggerresultshelper_HLT_HT750_v8)||
	      (triggerresultshelper_HLT_HT750_v9))
	    )) continue;
	   
	  // ---------------------
	  // -- fill histograms --
	  // ---------------------	  

          double weight=1;
          if(geneventinfoproduct_weight>0)
	      weight=geneventinfoproduct_weight;

          h1->Fill(eventhelperextra_dijet_invmass, weight);

          for ( size_t j = 0; j < (massBins.size()-1); ++j )
          {
              if((eventhelperextra_dijet_invmass>=massBins[j])&&
	         (eventhelperextra_dijet_invmass<massBins[j+1]))
              {
                  hists[j]->Fill(exp(fabs(jethelper2_rapidity[0]-jethelper2_rapidity[1])), weight);
                  histspt1[j]->Fill(jethelper2_pt[0], weight);
                  histspt2[j]->Fill(jethelper2_pt[1], weight);
                  histsy1[j]->Fill(jethelper2_rapidity[0], weight);
                  histsy2[j]->Fill(jethelper2_rapidity[1], weight);
                  histsyboost[j]->Fill(fabs(jethelper2_rapidity[0]+jethelper2_rapidity[1])/2., weight);
                  histsmetsumet[j]->Fill(met2_et/met2_sumEt, weight);
                  histsdphi[j]->Fill(reco::deltaPhi(jethelper2_phi[0],jethelper2_phi[1]));
              }
          }
  
	}

  std::cout << "done" << std::endl;

  h1->Write();
  h1->Draw("histe");
  gPad->SetLogy(true);
  c1.SaveAs((cmdline.outputfilename.substr(0,cmdline.outputfilename.size()-5)+"_mass.pdf").c_str());

  for ( size_t j = 0; j < (massBins.size()-1); ++j )
  {
      hists[j]->Write();
      histspt1[j]->Write();
      histspt2[j]->Write();
      histsy1[j]->Write();
      histsy2[j]->Write();
      histsyboost[j]->Write();
      histsmetsumet[j]->Write();
      histsdphi[j]->Write();
      hists[j]->Draw("histe");
      std::stringstream name;
      name << cmdline.outputfilename.substr(0,cmdline.outputfilename.size()-5) << "_" << massBins[j] << "_" << massBins[j+1] << "_" << "chi.pdf";
      gPad->SetLogy(false);
      c1.SaveAs(name.str().c_str());
  }

  stream.close();
  ofile.close();

  return 0;
}
