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

#define MAXPARTICLES 25
struct ZZ2l2nuSummary_t
{
  Int_t run,lumi,event;
  Int_t cat;
  Int_t nvtx, ngenpu;
  Int_t njets;
  Float_t weight;
  Float_t rho;
  Int_t nparticles,nmcparticles;
  Float_t px[MAXPARTICLES],py[MAXPARTICLES],pz[MAXPARTICLES],en[MAXPARTICLES]; 
  Float_t mcpx[MAXPARTICLES],mcpy[MAXPARTICLES],mcpz[MAXPARTICLES],mcen[MAXPARTICLES]; 
  Int_t id[MAXPARTICLES],genid[MAXPARTICLES],mcid[MAXPARTICLES];
  Float_t info1[MAXPARTICLES],info2[MAXPARTICLES],info3[MAXPARTICLES],info4[MAXPARTICLES],info5[MAXPARTICLES];
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
