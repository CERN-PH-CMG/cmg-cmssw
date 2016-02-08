#ifndef _COMMON_STUFF_H
#define _COMMON_STUFF_H

#include <iostream>
#include <TChain.h>
#include <TClonesArray.h>
#include <TString.h>
#include <map>

#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TString.h>
#include <string>
#include <TSystem.h>
#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>


namespace common_stuff {
 
  // NOTE ABOUT USAGE: std::string title can contain also axis labels, separate with semi-colon. Example: "histname;xtitle;ytitle"
  
  void plot1D(std::string title, double xval, double weight, std::map<std::string, TH1D*> &allhistos, int numbinsx, double xmin, double xmax);
  void plot1D(std::string title, double xval, double weight, std::map<std::string, TH1D*> &allhistos, int numbinsx, double *xarray);
  
  void plot2D(std::string title, double xval, double yval, double weight, std::map<std::string, TH2D*> &allhistos, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax);
  void plot2D(std::string title, double xval, double yval, double weight, std::map<std::string, TH2D*> &allhistos, int numbinsx, double *xarray, int numbinsy, double *yarray);
  
  void plot3D(std::string title, double xval, double yval, double zval, double weight, std::map<std::string, TH3D*> &allhistos, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax, int numbinsz, double zmin, double zmax);
  void plot3D(std::string title, double xval, double yval, double zval, double weight, std::map<std::string, TH3D*> &allhistos, int numbinsx, double *xarray, int numbinsy, double *yarray, int numbinsz, double *zarray);
  
  void cloneHisto1D(std::string title_old, std::string title_new, std::map<std::string, TH1D*> &allhistos);
  void cloneHisto2D(std::string title_old, std::string title_new, std::map<std::string, TH2D*> &allhistos);

  void makeRatioHisto1D(std::string title1, std::string title2, std::string title_ratio, std::map<std::string, TH1D*> &allhistos);
  
  void writeOutHistos(TFile *fout, std::map<std::string, TH1D*> h_1d, std::map<std::string, TH2D*> h_2d, std::map<std::string, TH3D*> h_3d);
  
  std::pair<double,double> getPhiCorrMET( double met, double metphi, int nvtx, bool ismc );
  
  double getMTFirstOrder(double Mu_pt, double Mu_phi, double tkmet,double tkmet_phi, double coeff);

  double BWweight(double VGen_mass, double iVmass_GeV, double gen_mass_value_MeV, double gamma);

  float deltaPhi( float phi1 , float phi2 );

  void plotAndSaveHisto1D(TFile*f1, TString str1, TFile*f2, TString str2, int logx, int logy, int logz, int normalized);

  void plotAndSaveHisto1D_stack(TString LegendEvTypeTeX, TFile*fMCsig, TFile*fMCEWK, TFile*fMCTT, TFile*fMCQCD, TFile*fDATA, TString HistoName_st, int logx, int logy, int logz, int scaleMCtoDATA, TString title,double xmin, double xmax, int rebinfactor, int PullOrRatio);
  
  void ComputeAllVarPietro(TLorentzVector lepP,TLorentzVector lepN, double &costh_CS, double &phi_CS, double &costh_HX, double &phi_HX);

  void calculateU1U2( double fMet , double fMPhi,double fZPt, double fZPhi, double fPt1, double fPhi1, double & fU1,double & fU2 );
  
  TString getCompleteTitleReturnName(std::string title);

};

#endif // _COMMON_STUFF_H
