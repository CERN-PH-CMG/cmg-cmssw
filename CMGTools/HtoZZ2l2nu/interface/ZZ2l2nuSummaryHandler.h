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
  enum HiggsPtWeights{ hKfactor, hKfactor_renUp, hKfactor_renDown, hKfactor_factUp, hKfactor_factDown };
  enum LeptonInfo { PtError, NeutralHadronIso, ChargedHadronIso, PhotonIso };
  enum JetInfo { TCHE, TCHP, SSVHE, SSVHP };

  Int_t run,lumi,event;
  Int_t cat, mccat;
  Bool_t hasTrigger;

  //gen level 
  Int_t nvtx, ngenITpu,ngenOOTpu,ngenOOTpum1;
  Float_t puWeight, hptWeights[5];
  Float_t rho;
  Float_t pthat,genWeight, qscale, x1,x2;
  Int_t id1,id2;
  Int_t pass;

  //dilepton
  Float_t l1_px  ,l1_py  ,l1_pz  ,l1_en, l1_ptErr, l1_iso1, l1_iso2, l1_iso3, l1_iso4; Int_t l1_id, l1_genid, l1_pid;
  Float_t l2_px  ,l2_py  ,l2_pz  ,l2_en, l2_ptErr, l2_iso1, l2_iso2, l2_iso3, l2_iso4; Int_t l2_id, l2_genid, l2_pid;

  //other leptons
  Int_t ln;
  Float_t ln_px[MAXPARTICLES],ln_py[MAXPARTICLES],ln_pz[MAXPARTICLES],ln_en[MAXPARTICLES], ln_ptErr[MAXPARTICLES], ln_iso1[MAXPARTICLES], ln_iso2[MAXPARTICLES], ln_iso3[MAXPARTICLES], ln_iso4[MAXPARTICLES]; Int_t ln_id[MAXPARTICLES], ln_genid[MAXPARTICLES], ln_pid[MAXPARTICLES];

  //jets
  Int_t jn;
  Float_t jn_px[MAXPARTICLES],    jn_py[MAXPARTICLES],      jn_pz[MAXPARTICLES],          jn_en[MAXPARTICLES];
  Float_t jn_btag1[MAXPARTICLES], jn_btag2[MAXPARTICLES],   jn_neutHadFrac[MAXPARTICLES], jn_neutEmFrac[MAXPARTICLES], jn_chHadFrac[MAXPARTICLES], jn_pid[MAXPARTICLES];  
  Int_t   jn_genid[MAXPARTICLES], jn_genflav[MAXPARTICLES];

  Int_t ajn;
  Float_t ajn_px[MAXPARTICLES],    ajn_py[MAXPARTICLES],    ajn_pz[MAXPARTICLES],          ajn_en[MAXPARTICLES]; 
  Float_t ajn_btag1[MAXPARTICLES], ajn_btag2[MAXPARTICLES], ajn_neutHadFrac[MAXPARTICLES], ajn_neutEmFrac[MAXPARTICLES], ajn_chHadFrac[MAXPARTICLES], ajn_pid[MAXPARTICLES];  
  Int_t   ajn_genid[MAXPARTICLES], ajn_genflav[MAXPARTICLES];

  //primary vertex
  Float_t vtx_px  ,vtx_py  ,vtx_pz  ,vtx_en;
  
  //met types
  Int_t nmet;
  Float_t met_phi[MAXPARTICLES] ,met_pt[MAXPARTICLES];
  Float_t sumEt,     sumEtcentral,     primVertexSumEt,     otherVertexSumEt;
  Float_t chsumEt,   chsumEtcentral,   primVertexChSumEt,   otherVertexChSumEt;
  Float_t neutsumEt, neutsumEtcentral, primVertexNeutSumEt, otherVertexNeutSumEt;

  //gamma candidate
  Int_t gn;
  Float_t g_px[MAXPARTICLES], g_py[MAXPARTICLES]  , g_pz[MAXPARTICLES]  , g_en[MAXPARTICLES], g_iso1[MAXPARTICLES], g_iso2[MAXPARTICLES], g_iso3[MAXPARTICLES], g_sihih[MAXPARTICLES], g_r9[MAXPARTICLES], g_hoe[MAXPARTICLES];
  Bool_t g_conv[MAXPARTICLES];
  Float_t g_conv_px[MAXPARTICLES],g_conv_py[MAXPARTICLES],g_conv_pz[MAXPARTICLES],g_conv_en[MAXPARTICLES];
  
  //gen level event
  Float_t h_px, h_py, h_pz, h_en;
  Int_t nmcparticles;
  Float_t mc_px[MAXPARTICLES],mc_py[MAXPARTICLES],mc_pz[MAXPARTICLES],mc_en[MAXPARTICLES]; Int_t mc_id[MAXPARTICLES];
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
  bool initTree(TTree *t,bool needsToRecreate=true);
  void fillTree();

  //read mode
  bool attachToTree(TTree *t);
  void getEntry(int ientry) { 
    resetStruct();
    if(t_) t_->GetEntry(ientry); 
  }

  int getEntries() { return (t_ ? t_->GetEntriesFast() : 0); }

  //getter
  TTree *getTree() { return t_; }

  void resetStruct();

 private:

  //the tree
  TTree *t_;
};

#endif
