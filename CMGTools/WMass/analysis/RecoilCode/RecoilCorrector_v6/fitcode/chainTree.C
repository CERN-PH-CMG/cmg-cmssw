#include "TChain.h"
#include "TTree.h"
#include "TFile.h"

void chainTree(bool doMad, bool doRaw) {

  bool doIter=true;
  
  TChain chain("ZTreeProducer");

  if(doMad && !doRaw && doIter) {
    chain.Add("TREE/foutIter_loopOverTree_mad1_iter1_0_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad1_iter1_250000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad1_iter1_500000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad1_iter1_750000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad1_iter1_1000000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad1_iter1_1250000_writeTree_1.root");
  }

  if(!doMad && !doRaw && doIter) {
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_0_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_250000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_500000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_750000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_1000000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_1250000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_1500000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_1750000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_2000000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_2250000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_2500000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_2750000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_3000000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_3250000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_3500000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_3750000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_4000000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_4250000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_4500000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_4750000_writeTree_1.root");
    chain.Add("TREE/foutIter_loopOverTree_mad0_iter1_5000000_writeTree_1.root");

  }

  TString fileName = Form("TREE/output_mad%d_iter%d.root",doMad,doIter);

  TFile *file = TFile::Open(fileName.Data(),"RECREATE");
  chain.CloneTree(-1,"fast");
  file->Write();
  delete file;


}
