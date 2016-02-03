#include "TString.h"

namespace WMass{

  static const double ZMassCentral_MeV = 91188;
  static const double WMassCentral_MeV = 80398;
  static const double ZWmassRatio = ZMassCentral_MeV/WMassCentral_MeV;

  static const double Zmass_value = _ZMASS_VALUE;
  
  static const int PDF_sets = _PDF_SETS;
  static const int PDF_members = _PDF_MEMBERS;
  
  // static const int ZPtSFNSteps = _PTSF_NSTEPS;
  // static const int ZPtSFtype[PtSFNSteps] = { _PTSF_TYPE };
  
  static const double etaMaxMuons = _ETA_MAX_MUONS;
  static const double etaMaxMuons2 = _ETA_MAX_SUBMUONS;
  
  static const int WlikeCharge = _WLIKE_CHARGE;
  
  static const int nSigOrQCD = 2;
  TString nSigOrQCD_str[nSigOrQCD] = {"Sig","QCD"};
  
  static const int WpTcut = 15;
  
  static const int NFitVar = 3;
  TString FitVar_str[NFitVar] = {"Pt","Mt","MET"};
  
  static const double sel_xmin[NFitVar]={30,  60, 30};
  static const double sel_xmax[NFitVar]={55, 100, 55};
  
  static const double fit_xmin[NFitVar]={32,  65, 32};
  static const double fit_xmax[NFitVar]={45,  90, 45};

  static const int MuonKalmanVariation = _MUON_KALMAN_VARIATION; // 1 to 45
  static const bool MuonScaleVariation = _MUON_SCALE_VARIATION;
  static const int MuonVariationSigmas = _MUON_VARIATION_SIGMAS;

  static const int RecoilStatVariation = _RECOIL_STAT_VARIATION; // 1 to 144
  static const int RecoilVariationSigmas = _RECOIL_VARIATION_SIGMAS;
  
  //                                        ***RAPBIN 1***  ***RAPBIN 2***
  //                                        *DATA*  **MC**  *DATA*  **MC**
  //                                        U1  U2  U1  U2  U1  U2  U1  U2  
  static const int RecoilNVarsPerBlock[] = {21, 15, 21, 15, 21, 15, 21, 15};
  
  void RecoilStatVariationToInternalVars(int RecoilStatVariation, int& RapBinCorr, int& DATAorMC, int& U1orU2, int& RecoilEigenNum)
  {
    int block;
    for (block=0; RecoilStatVariation > RecoilNVarsPerBlock[block]; ++block) {
      RecoilStatVariation -= RecoilNVarsPerBlock[block];
    }
    RapBinCorr = (block & (1<<2)) ? 2 : 1;
    DATAorMC   = (block & (1<<1)) ? 2 : 1;
    U1orU2     = (block & (1<<0)) ? 2 : 1;
    RecoilEigenNum = RecoilStatVariation;
  }
  
  static const int LHE_mass_central_index = _LHE_MASS_CENTRAL_INDEX;
  // 209 #w,ren,fac,pdf,mW,mZ 1.0 1.0 1.0 229800 80.398 91.1876               <------------ NNPDF2.3 NLO
  static const int LHE_NNPDF2p3_NLO_central_index = 209; // 100 members
  // 309 #w,ren,fac,pdf,mW,mZ 0.995244555969 1.0 1.0 11000 80.398 91.1876     <------------ CT10 NLO
  static const int LHE_CT10_NLO_central_index = 309;     // 53 members
  // 362 #w,ren,fac,pdf,mW,mZ 1.01129558613 1.0 1.0 232000 80.398 91.1876     <------------ NNPDF2.3 NNLO
  static const int LHE_NNPDF2p3_NNLO_central_index = 362; // 100 members
}
