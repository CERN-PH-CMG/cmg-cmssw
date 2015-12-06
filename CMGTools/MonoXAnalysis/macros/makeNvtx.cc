#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TChain.h"
#include "TString.h"
#include <iostream>

void makeNvtx(bool ismc=false, const char* option="recreate") {

  TString treeDir = "/data1/emanuele/monox/TREES_25ns_1LEPSKIM_23NOV2015/";
  std::vector<TString> dataDirs;
  dataDirs.push_back("DoubleMuon_Run2015C_05Oct_runs_254227_255031");
  dataDirs.push_back("DoubleMuon_Run2015D_05Oct_runs_256630_258158");
  dataDirs.push_back("DoubleMuon_Run2015D_v4_runs_258159_260627");
  TString mcDir = treeDir+"DYJetsToLL_M50";


  if(!ismc) {
    TFile *dataHFile = TFile::Open("nvtx_profile.root",option);
    TChain *t = new TChain("tree");
    for(int i=0; i<(int)dataDirs.size();++i)
      t->Add(treeDir+dataDirs[i]+"/treeProducerDarkMatterMonoJet/tree.root");
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
