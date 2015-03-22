#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>

namespace WMass{

  static const int ZMassCentral_MeV = 91188;
  static const int WMassCentral_MeV = 80398;
  
  static const int WMassSkipNSteps = 5;
  static const int WMassNSteps = 5;
  
  static const int Wmass_values_array[2*WMassNSteps+1] = {80348,80358,80368,80378,80388,80398,80408,80418,80428,80438,80448};
  static const int Zmass_values_array[2*WMassNSteps+1] = {91138,91148,91158,91168,91178,91188,91198,91208,91218,91228,91238};
  
  static const int PtSFNSteps = 1;
  static const int PtSFtype[PtSFNSteps] = { -1 };

  static const int etaMuonNSteps = 1;
  static const float etaMaxMuons[etaMuonNSteps] = { 0.9 };
  static const int nSigOrQCD = 2;
  TString nSigOrQCD_str[nSigOrQCD] = {"Sig","QCD"};
  static const int NFitVar = 3;
  TString FitVar_str[NFitVar] = {"Pt","Mt","MET"};
  static const int PDF_sets = 229800;
  static const int PDF_members = 1;
  static const int NVarRecoilCorr = 18;
  
  static const int WpTcut = 15;
  
  // static const double sel_xmin[3]={30,  60, 30};
  static const double sel_xmin[3]={30,  60, 30};
  static const double sel_xmax[3]={55, 100, 55};
  
  static const double fit_xmin[3]={32,  65, 32};
  static const double fit_xmax[3]={45,  90, 45};

  static const int LHE_mass_central_index = 101;
  // 209 #w,ren,fac,pdf,mW,mZ 1.0 1.0 1.0 229800 80.398 91.1876               <------------ NNPDF2.3 NLO
  static const int LHE_NNPDF2p3_NLO_central_index = 209; // 100 members
  // 309 #w,ren,fac,pdf,mW,mZ 0.995244555969 1.0 1.0 11000 80.398 91.1876     <------------ CT10 NLO
  static const int LHE_CT10_NLO_central_index = 309;     // 53 members
  // 362 #w,ren,fac,pdf,mW,mZ 1.01129558613 1.0 1.0 232000 80.398 91.1876      <------------ NNPDF2.3 NNLO
  static const int LHE_NNPDF2p3_NNLO_central_index = 362; // 100 members
  
 static const int RecoilCorrVarDiagoParU1orU2fromDATAorMC_[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,  1,  1,  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2,  2 };
 static const int RecoilCorrVarDiagoParN_[]                  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  

  // bool isGood_WGenPos_1_Gen(double WGen_m,
                            // int MuGen_charge
                            // ){
    // return (WGen_m>0 && MuGen_charge>0);
  // }

  // bool isGood_WGenPos_2_ZGenMassCut(double WGen_m,
                                    // int MuGen_charge
                                    // ){
    // return isGood_WGenPos_1_Gen(WGen_m,MuGen_charge);
  // }
  
  // bool isGood_WGenPos_3_Mu1GenCut(double WGen_m,
                                    // int MuGen_charge,
                                    // double MuGen_eta,
                                    // double etaMax
                                    // ){
    // return (isGood_WGenPos_2_ZGenMassCut(WGen_m,MuGen_charge)
            // && TMath::Abs(MuGen_eta)<etaMax);
  // }


}
