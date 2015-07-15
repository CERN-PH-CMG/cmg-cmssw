#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include <iostream>

void makeNvtx(bool ismc=false) {

  TString treeDir = "/data1/emanuele/monox/TREES_SPRING15_50ns_2LEPSKIM_10JUL2015/";
  TString dataDir = treeDir+"DoubleEG2015B";
  TString mcDir = treeDir+"DYJetsToLL_M50_50ns";


  if(!ismc) {
    TFile *dataHFile = TFile::Open("nvtxData.root","recreate");
    TFile *fileData = TFile::Open(dataDir+"/treeProducerDarkMatterMonoJet/tree.root");
    fileData->cd();
    TTree *t = (TTree*)fileData->Get("tree");
    std::cout << "tree entries for data " << t->GetEntries() << std::endl;
    TH1F *nvtxProfile = new TH1F("nvtx","",40,0,40);
    t->Project("nvtx","nVert");
  
    dataHFile->cd();
    nvtxProfile->Write();
    dataHFile->Close();
  } else {
    TFile *mcHFile = TFile::Open("nvtxMC.root","recreate");
    TFile *fileMC = TFile::Open(mcDir+"/treeProducerDarkMatterMonoJet/tree.root");
    fileMC->cd();
    TTree *t = (TTree*)fileMC->Get("tree");
    std::cout << "tree entries for MC " << t->GetEntries() << std::endl;
    TH1F *nvtxProfile = new TH1F("nvtx","",40,0,40);  
    t->Project("nvtx","nVert");

    mcHFile->cd();  
    nvtxProfile->Write();
    mcHFile->Close();
  }

}

void doAll() {
  makeNvtx(true);
  makeNvtx(false);
}
