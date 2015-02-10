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
  static const int WMassNSteps = 0;
  
  static const int Wmass_values_array[2*WMassNSteps+1] = {80398};
  static const int Zmass_values_array[2*WMassNSteps+1] = {91188};
  
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
  static const int NtoysMomCorr = 1;
  
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


void plotAndSaveHisto1D(TFile*f1, TString str1, TFile*f2, TString str2, int logx, int logy, int logz, int normalized){

  TH1D*h1=(TH1D*)f1->Get(str1.Data());
  h1->SetLineColor(2);
  TH1D*h2=(TH1D*)f2->Get(str2.Data());
  
  TCanvas *c1 = new TCanvas("c"+str1);
  c1->SetLogx(logx);
  c1->SetLogy(logy);
  c1->SetLogz(logz);
  
  if(normalized){
    h1->DrawNormalized();
    h2->DrawNormalized("same");
  }else{
    h1->Draw();
    h2->Draw("same");    
  }
  
  c1->SaveAs(str1+".png");
  
}

  
}
