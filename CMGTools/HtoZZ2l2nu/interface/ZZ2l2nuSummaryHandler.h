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
#include "TMath.h"
#include "Math/LorentzVector.h"

#include "Math/LorentzVector.h"
#include "TMath.h"
#include "TVector2.h"

#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

#define MAXPARTICLES 50
#define MAXMCPARTICLES 250
#define MAXMEASUREMENTS 20

struct ZZ2l2nuSummary_t
{
  enum HiggsPtWeights{ hKfactor, hKfactor_renUp, hKfactor_renDown, hKfactor_factUp, hKfactor_factDown };
  enum LeptonInfo { PtError, NeutralHadronIso, ChargedHadronIso, PhotonIso };
  enum JetInfo { TCHE, TCHP, SSVHE, SSVHP };

  Int_t run,lumi,event;
  Float_t curAvgInstLumi,curIntegLumi;
  Int_t cat, mccat;
  Bool_t hasTrigger;
  Int_t triggerType;

  Float_t rho,rho25,rho25Neut;

  //gen level 
  Int_t nvtx, ngenITpu,ngenOOTpu,ngenOOTpum1, ngenTruepu;
  Float_t puWeight, hptWeights[5];
  Float_t pthat,genWeight, qscale, x1,x2;
  Int_t id1,id2;

  //dilepton
  Float_t l1_px  ,l1_py  ,l1_pz  ,l1_en, l1_ptErr, l1_ecalIso, l1_hcalIso, l1_trkIso, l1_gIso, l1_chIso, l1_puchIso, l1_nhIso; 
  Int_t l1_id, l1_genid, l1_pid;
  Float_t l1_ensf, l1_ensferr;
  Float_t l1_d0, l1_dZ, l1_ip3d, l1_ip3dsig, l1_trkpt,l1_trketa,l1_trkphi,l1_trkchi2,l1_trkValidPixelHits,l1_trkValidTrackerHits,l1_trkLostInnerHits;
  
  Float_t l2_px  ,l2_py  ,l2_pz  ,l2_en, l2_ptErr, l2_ecalIso, l2_hcalIso, l2_trkIso, l2_gIso, l2_chIso, l2_puchIso, l2_nhIso; 
  Int_t l2_id, l2_genid, l2_pid;
  Float_t l2_ensf, l2_ensferr;
  Float_t l2_d0, l2_dZ, l2_ip3d, l2_ip3dsig, l2_trkpt,l2_trketa,l2_trkphi,l2_trkchi2,l2_trkValidPixelHits,l2_trkValidTrackerHits,l2_trkLostInnerHits;

  //other leptons
  Int_t ln;
  Float_t ln_px[MAXPARTICLES],ln_py[MAXPARTICLES],ln_pz[MAXPARTICLES],ln_en[MAXPARTICLES], ln_ptErr[MAXPARTICLES];
  Float_t ln_ensf[MAXPARTICLES], ln_ensferr[MAXPARTICLES];
  Float_t ln_ecalIso[MAXPARTICLES], ln_hcalIso[MAXPARTICLES], ln_trkIso[MAXPARTICLES], ln_gIso[MAXPARTICLES],  ln_chIso[MAXPARTICLES], ln_puchIso[MAXPARTICLES], ln_nhIso[MAXPARTICLES]; 
  Float_t ln_d0[MAXPARTICLES],ln_dZ[MAXPARTICLES], ln_ip3d[MAXPARTICLES], ln_ip3dsig[MAXPARTICLES], ln_trkpt[MAXPARTICLES],ln_trketa[MAXPARTICLES],ln_trkphi[MAXPARTICLES],ln_trkchi2[MAXPARTICLES],ln_trkValidPixelHits[MAXPARTICLES],ln_trkValidTrackerHits[MAXPARTICLES],ln_trkLostInnerHits[MAXPARTICLES];
  Int_t ln_id[MAXPARTICLES], ln_genid[MAXPARTICLES], ln_pid[MAXPARTICLES];

  //lepton id variables
  Int_t en,en_idbits[MAXPARTICLES];
  Float_t en_hoe[MAXPARTICLES],en_hoebc[MAXPARTICLES],en_dphiin[MAXPARTICLES],en_detain[MAXPARTICLES],en_sihih[MAXPARTICLES],en_sipip[MAXPARTICLES],en_r9[MAXPARTICLES];
  Float_t en_sce[MAXPARTICLES],en_sceta[MAXPARTICLES],en_scphi[MAXPARTICLES],en_e2x5max[MAXPARTICLES],en_e1x5[MAXPARTICLES],en_e5x5[MAXPARTICLES];
  Float_t en_h2te[MAXPARTICLES],en_h2tebc[MAXPARTICLES],en_ooemoop[MAXPARTICLES],en_fbrem[MAXPARTICLES],en_eopin[MAXPARTICLES];
  Float_t en_mvatrigv0[MAXPARTICLES], en_mvanontrigv0[MAXPARTICLES];
  Float_t en_dEtaCalo[MAXPARTICLES], en_dPhiCalo[MAXPARTICLES], en_kfchi2[MAXPARTICLES], en_kfhits[MAXPARTICLES], en_kfhitsall[MAXPARTICLES], en_sihip[MAXPARTICLES], en_nbrems[MAXPARTICLES], en_etawidth[MAXPARTICLES], en_phiwidth[MAXPARTICLES], en_e1x5e5x5[MAXPARTICLES], en_preShowerOverRaw[MAXPARTICLES], en_eopout[MAXPARTICLES];

  Int_t mn,mn_idbits[MAXPARTICLES];
  Float_t mn_nMatches[MAXPARTICLES],mn_nMatchedStations[MAXPARTICLES],mn_validMuonHits[MAXPARTICLES],mn_innerTrackChi2[MAXPARTICLES],mn_trkLayersWithMeasurement[MAXPARTICLES],mn_pixelLayersWithMeasurement[MAXPARTICLES];

  //jets
  Int_t jn;
  Float_t jn_rawsf[MAXPARTICLES];
  Float_t jn_px[MAXPARTICLES],    jn_py[MAXPARTICLES],      jn_pz[MAXPARTICLES],          jn_en[MAXPARTICLES];
  Float_t jn_neutHadFrac[MAXPARTICLES], jn_neutEmFrac[MAXPARTICLES], jn_chHadFrac[MAXPARTICLES];
  Float_t jn_btag1[MAXPARTICLES], jn_btag2[MAXPARTICLES],   jn_btag3[MAXPARTICLES], jn_btag4[MAXPARTICLES];
  Float_t jn_btag5[MAXPARTICLES], jn_btag6[MAXPARTICLES],   jn_btag7[MAXPARTICLES], jn_btag8[MAXPARTICLES], jn_btag9[MAXPARTICLES];
  Float_t jn_lxy[MAXPARTICLES], jn_lxyErr[MAXPARTICLES], jn_svmass[MAXPARTICLES], jn_svpt[MAXPARTICLES], jn_svdr[MAXPARTICLES];
  Int_t   jn_genid[MAXPARTICLES], jn_genflav[MAXPARTICLES];
  Float_t jn_genpt[MAXPARTICLES], jn_pumva[MAXPARTICLES], jn_puminmva[MAXPARTICLES];
  Bool_t  jn_isCfromGsplit[MAXPARTICLES], jn_isBfromGsplit[MAXPARTICLES];
  Float_t jn_beta[MAXPARTICLES],  jn_betaStar[MAXPARTICLES], jn_dRMean[MAXPARTICLES], jn_ptD[MAXPARTICLES], jn_ptRMS[MAXPARTICLES],jn_etaW[MAXPARTICLES],jn_phiW[MAXPARTICLES];
  Int_t jn_idbits[MAXPARTICLES];

  Float_t htvec_px, htvec_py, htvec_pz, htvec_en;

  Int_t ajn;
  Float_t ajn_rawsf[MAXPARTICLES];
  Float_t ajn_px[MAXPARTICLES],    ajn_py[MAXPARTICLES],      ajn_pz[MAXPARTICLES],    ajn_en[MAXPARTICLES]; 
  Float_t ajn_neutHadFrac[MAXPARTICLES], ajn_neutEmFrac[MAXPARTICLES], ajn_chHadFrac[MAXPARTICLES];
  Float_t ajn_btag1[MAXPARTICLES], ajn_btag2[MAXPARTICLES],   ajn_btag3[MAXPARTICLES], ajn_btag4[MAXPARTICLES];
  Float_t ajn_btag5[MAXPARTICLES], ajn_btag6[MAXPARTICLES],   ajn_btag7[MAXPARTICLES], ajn_btag8[MAXPARTICLES], ajn_btag9[MAXPARTICLES];
  Float_t ajn_lxy[MAXPARTICLES], ajn_lxyErr[MAXPARTICLES], ajn_svmass[MAXPARTICLES], ajn_svpt[MAXPARTICLES], ajn_svdr[MAXPARTICLES];
  Int_t   ajn_genid[MAXPARTICLES], ajn_genflav[MAXPARTICLES];
  Float_t ajn_genpt[MAXPARTICLES], ajn_pumva[MAXPARTICLES], ajn_puminmva[MAXPARTICLES];
  Bool_t  ajn_isCfromGsplit[MAXPARTICLES],ajn_isBfromGsplit[MAXPARTICLES];
  Float_t ajn_beta[MAXPARTICLES],  ajn_betaStar[MAXPARTICLES], ajn_dRMean[MAXPARTICLES], ajn_ptD[MAXPARTICLES], ajn_ptRMS[MAXPARTICLES], ajn_etaW[MAXPARTICLES],ajn_phiW[MAXPARTICLES];
  Int_t ajn_idbits[MAXPARTICLES];

  //primary vertex
  Float_t vtx_px  ,vtx_py  ,vtx_pz  ,vtx_en;
  
  //met types
  Int_t nmet;
  Float_t met_phi[MAXPARTICLES] ,met_pt[MAXPARTICLES];
  Float_t sumEt,     sumEtcentral,     primVertexSumEt,     otherVertexSumEt;
  Float_t chsumEt,   chsumEtcentral,   primVertexChSumEt,   otherVertexChSumEt;
  Float_t neutsumEt, neutsumEtcentral, primVertexNeutSumEt, otherVertexNeutSumEt;

  //gamma candidate (iso1=ecalIso, iso2=hcalIso, iso3=trkIso - have to modify these names in the future)
  Int_t gn,gn_prescale[16], gn_triggerWord;
  Float_t g_px[MAXPARTICLES], g_py[MAXPARTICLES]  , g_pz[MAXPARTICLES]  , g_en[MAXPARTICLES], g_iso1[MAXPARTICLES], g_iso2[MAXPARTICLES], g_iso3[MAXPARTICLES], g_sihih[MAXPARTICLES], g_r9[MAXPARTICLES], g_hoe[MAXPARTICLES], g_htoe[MAXPARTICLES], g_sipip[MAXPARTICLES];
  Float_t g_corren[MAXPARTICLES], g_correnerr[MAXPARTICLES];
  Int_t g_idbits[MAXPARTICLES];
  
  //gen level event
  Float_t h_px, h_py, h_pz, h_en;
  Int_t nmcparticles,mc_nup;
  Float_t mc_px[MAXMCPARTICLES],mc_py[MAXMCPARTICLES],mc_pz[MAXMCPARTICLES],mc_en[MAXMCPARTICLES], mc_lxy[MAXPARTICLES]; 
  Int_t mc_id[MAXPARTICLES], mc_status[MAXPARTICLES];

  //additional measurements you may want to add
  Int_t nmeasurements;
  Float_t measurements[MAXMEASUREMENTS];
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
  bool attachToTree(TTree *t, bool full=true);
  void getEntry(int ientry) { 
    resetStruct();
    if(t_) t_->GetEntry(ientry); 
  }

  bool hasSpoilerAlert(bool isData);

  int getEntries() { return (t_ ? t_->GetEntriesFast() : 0); }

  //getter
  TTree *getTree() { return t_; }

  void resetStruct();
    
 private:

  //the tree
  TTree *t_;
};

#endif
