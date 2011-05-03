#ifndef zz2l2nusummaryhandler_h
#define zz2l2nusummaryhandler_h

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <string.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>

#include "TTree.h"

#endif

struct ZZ2l2nuSummary_t
{
  Int_t run,lumi,event;
  Int_t cat;
  Int_t njets,nbtags;
  Float_t weight;
  Float_t pfmet_pt,pfmet_phi,pfmet_sig;   //met
  Float_t cormet_pt,cormet_phi;
  Float_t redmet_pt,redmet_phi;
  Float_t redmet_dilL,redmet_dilT,redmet_deltaL,redmet_deltaT,redmet_rjetL,redmet_rjetT,redmet_metL,redmet_metT,redmet_rL,redmet_rT;
  Float_t projmet_pt;
  Float_t tkmet_pt,tkmet_phi;
  Float_t genmet_pt,genmet_phi;
  Float_t l1px,l1py,l1pz,l1e; //leptons
  Float_t l2px,l2py,l2pz,l2e;
  Int_t l1id,l2id;
};

class ZZ2l2nuSummaryHandler{
 public:

  //c/dtor
  ZZ2l2nuSummaryHandler();
  ~ZZ2l2nuSummaryHandler();
  
  //current event
  ZZ2l2nuSummary_t evSummary_;
  ZZ2l2nuSummary_t &getEvent() { return evSummary_; }
  
  //write mode
  bool initTree(TTree *t);
  void fillTree();

  //read mode
  bool attachToTree(TTree *t);
  void getEntry(int ientry) { if(t_) t_->GetEntry(ientry); }
  int getEntries() { return (t_ ? t_->GetEntriesFast() : 0); }

 private:

  //the tree
  TTree *t_;
};

#endif
