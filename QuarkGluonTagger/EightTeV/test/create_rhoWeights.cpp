#include <iostream>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"



bool ptHatReweight = true;


int main(int argc, char* argv[]) {

  if( argc < 3 ) {
    std::cout << "USAGE: ./create_rhoWeights [mcDataset] [selection]" << std::endl;
    std::cout << "where [selection] can be either \"ZJet\" or \"DiJet\"" << std::endl;
    exit(11);
  }


  std::string mcDataset(argv[1]);


  std::string selection(argv[2]);
  if( selection!="ZJet" && selection!="DiJet" ) {
    std::cout << "Only supporting \"ZJet\" and \"DiJet\" selections at the moment. Exiting." << std::endl;
    exit(13);
  }



  std::string dataDataset;
  if( selection=="ZJet" )
    dataDataset = "data2012ABCD_MuPD_24Aug_skim_new";
  else if( selection=="DiJet" )
    dataDataset = "data2012ABCD_MBPD_24Aug_new"; //fuck the JetPD



  //std::cout << "HAVE YOU DONE THE pT REWEIGHTING FIRST?" << std::endl;
  //std::string tmp_string;
  //std::cin >> tmp_string;

  std::string selectionType = (selection=="ZJet") ? "ZJet" : "DiJet";

  std::string mcFileName = "sunilFlat_" + selectionType + "_" + mcDataset;
  if( selection=="DiJet" && ptHatReweight ) mcFileName += "_ptHatWeight";
  mcFileName += ".root";
  std::string dataFileName = "sunilFlat_" + selectionType + "_" + dataDataset + ".root";

  TFile* mcFile = TFile::Open(mcFileName.c_str());
  TFile* dataFile = TFile::Open(dataFileName.c_str());

  std::cout << "-> Opened MC file " << mcFileName << std::endl;
  std::cout << "-> Opened DATA file " << dataFileName << std::endl;

  TTree* mcTree = (TTree*)mcFile->Get("tree_passedEvents");
  TTree* dataTree = (TTree*)dataFile->Get("tree_passedEvents");

  TH1D* h1_rho_data = new TH1D("rho_data", "", 90, 0., 45.);
  TH1D* h1_rho_mc = new TH1D("rho_mc", "", 90., 0., 45.);

  mcTree->Project( "rho_mc", "rhoPF", "wt_pteta");
  dataTree->Project( "rho_data", "rhoPF");

 
  std::string weightFileName = "rhoWeights_" + mcDataset + ".root";
  TFile* file_weights = TFile::Open(weightFileName.c_str(), "recreate");
  file_weights->cd();

  TH1D* h1_weights = new TH1D(*h1_rho_data);
  h1_weights->Divide(h1_rho_mc);
  h1_weights->SetName("rho_weights");

  h1_weights->Write();
  file_weights->Close(); 

  std::cout << "-> Created weights file " << weightFileName << std::endl;

  return 0;

}
