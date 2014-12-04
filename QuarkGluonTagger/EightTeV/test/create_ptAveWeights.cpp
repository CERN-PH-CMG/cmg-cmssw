#include <iostream>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"




int main(int argc, char* argv[]) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./create_ptAveWeights [dataset]" << std::endl;
    std::cout << "where [dataset] can be either \"JetPD\" or \"MB\"" << std::endl;
    exit(11);
  }


  std::string dataset(argv[1]);
  if( dataset!="MB" && dataset!="JetPD" ) {
    std::cout << "Only supporting \"JetPD\" and \"MB\" at the moment. Exiting." << std::endl;
    exit(13);
  }

  std::string mcDataset;
  std::string dataDataset;
  if( dataset=="MB" ) {
    mcDataset = "flatQCD_P6_Dijets_24Aug"; 
    dataDataset = "data2012ABCD_MBPD_24Aug"; 
  } else if( dataset=="JetPD" ) {
    mcDataset = "flatQCD_P6_Dijets_24Aug"; 
    dataDataset = "data2012ABCD_JetPD_24Aug"; 
  }


  //std::cout << "HAVE YOU DONE THE RHO REWEIGHTING FIRST?" << std::endl;
  //std::string tmp_string;
  //std::cin >> tmp_string;

  std::string mcFileName = "sunilFlat_DiJet_" + mcDataset + ".root";
  std::string dataFileName = "sunilFlat_DiJet_" + dataDataset + ".root";

  TFile* mcFile = TFile::Open(mcFileName.c_str());
  TFile* dataFile = TFile::Open(dataFileName.c_str());

  std::cout << "-> Opened MC file " << mcFileName << std::endl;
  std::cout << "-> Opened DATA file " << dataFileName << std::endl;

  TTree* mcTree = (TTree*)mcFile->Get("tree_passedEvents");
  TTree* dataTree = (TTree*)dataFile->Get("tree_passedEvents");

  TH1D* h1_pt_data = new TH1D("pt_data", "", 500, 20., 520.);
  TH1D* h1_pt_mc = new TH1D("pt_mc", "", 500, 20., 520.);

  mcTree->Project( "pt_mc", "0.5*(ptJet[0]+ptJet[1])" );
  dataTree->Project( "pt_data", "0.5*(ptJet[0]+ptJet[1])" );

 
  std::string weightFileName = "ptWeights_" + mcDataset + ".root";
  TFile* file_weights = TFile::Open(weightFileName.c_str(), "recreate");
  file_weights->cd();

  TH1D* h1_weights = new TH1D(*h1_pt_data);
  h1_weights->Divide(h1_pt_mc);
  h1_weights->SetName("ptAve_weights");

  h1_weights->Write();
  file_weights->Close(); 

  std::cout << "-> Created weights file " << weightFileName << std::endl;

  return 0;

}
