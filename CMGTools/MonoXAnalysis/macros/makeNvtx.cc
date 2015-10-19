#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include <iostream>

void makeNvtx(bool ismc=false, const char* option="recreate") {

  TString treeDir = "/afs/cern.ch/work/e/emanuele/monox/heppy/CMSSW_7_4_12/src/CMGTools/MonoXAnalysis/cfg/TREES_SPRING15_25ns_2LEPSKIM_06Oct2015/";
  TString dataDir = treeDir+"DoubleEG_Run2015D_v3_runs_246908_257599";
  TString mcDir = treeDir+"DYJetsToLL_M50";


  if(!ismc) {
    TFile *dataHFile = TFile::Open("nvtx_profile.root",option);
    TFile *fileData = TFile::Open(dataDir+"/treeProducerDarkMatterMonoJet/tree.root");
    fileData->cd();
    TTree *t = (TTree*)fileData->Get("tree");
    std::cout << "tree entries for data " << t->GetEntries() << std::endl;
    TH1F *nvtxProfile = new TH1F("nvtx_data","",40,0,40);
    t->Project("nvtx_data","nVert");
  
    dataHFile->cd();
    nvtxProfile->Write();
    dataHFile->Close();
  } else {
    TFile *mcHFile = TFile::Open("nvtx_profile.root",option);
    TFile *fileMC = TFile::Open(mcDir+"/treeProducerDarkMatterMonoJet/tree.root");
    fileMC->cd();
    TTree *t = (TTree*)fileMC->Get("tree");
    std::cout << "tree entries for MC " << t->GetEntries() << std::endl;
    TH1F *nvtxProfile = new TH1F("nvtx_signal","",40,0,40);  
    t->Project("nvtx_signal","nVert");

    mcHFile->cd();  
    nvtxProfile->Write();
    mcHFile->Close();
  }

}

void doAll() {
  makeNvtx(true,"recreate");
  makeNvtx(false,"update");
}
