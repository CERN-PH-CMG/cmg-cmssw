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


class common_stuff {
 
 public:
  common_stuff();
  ~common_stuff();
  
  // NOTE ABOUT USAGE: std::string title can contain also axis labels, separate with semi-colon. Example: "histname;xtitle;ytitle"
  
  static void plot1D(std::string title, double xval, double weight, std::map<std::string, TH1D*> &allhistos, int numbinsx, double xmin, double xmax);
  static void plot1D(std::string title, double xval, double weight, std::map<std::string, TH1D*> &allhistos, int numbinsx, double *xarray);
  
  static void plot2D(std::string title, double xval, double yval, double weight, std::map<std::string, TH2D*> &allhistos, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax);
  static void plot2D(std::string title, double xval, double yval, double weight, std::map<std::string, TH2D*> &allhistos, int numbinsx, double *xarray, int numbinsy, double *yarray);
  
  static void plot3D(std::string title, double xval, double yval, double zval, double weight, std::map<std::string, TH3D*> &allhistos, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax, int numbinsz, double zmin, double zmax);
  static void plot3D(std::string title, double xval, double yval, double zval, double weight, std::map<std::string, TH3D*> &allhistos, int numbinsx, double *xarray, int numbinsy, double *yarray, int numbinsz, double *zarray);
  
  static void cloneHisto1D(std::string title_old, std::string title_new, std::map<std::string, TH1D*> &allhistos);
  static void cloneHisto2D(std::string title_old, std::string title_new, std::map<std::string, TH2D*> &allhistos);

  static void makeRatioHisto1D(std::string title1, std::string title2, std::string title_ratio, std::map<std::string, TH1D*> &allhistos);
  
  static void writeOutHistos(TFile *fout, std::map<std::string, TH1D*> h_1d, std::map<std::string, TH2D*> h_2d, std::map<std::string, TH3D*> h_3d);
  
  static std::pair<double,double> getPhiCorrMET( double met, double metphi, int nvtx, bool ismc );
  
  static double getMTFirstOrder(double Mu_pt, double Mu_phi, double tkmet,double tkmet_phi, double coeff);

  static double BWweight(double VGen_mass, double iVmass_GeV, double gen_mass_value_MeV, double gamma);

  static float deltaPhi( float phi1 , float phi2 );

  static void plotAndSaveHisto1D(TFile*f1, TString str1, TFile*f2, TString str2, int logx, int logy, int logz, int normalized);

  static void plotAndSaveHisto1D_stack(TString LegendEvTypeTeX, TFile*fMCsig, TFile*fMCEWK, TFile*fMCTT, TFile*fMCQCD, TFile*fDATA, TString HistoName_st, int logx, int logy, int logz, int scaleMCtoDATA, TString title,double xmin, double xmax, int rebinfactor, int PullOrRatio);
  // static const double fit_xmin[];
  // static const double fit_xmax[];
  
  static void ComputeAllVarPietro(TLorentzVector lepP,TLorentzVector lepN, double &costh_CS, double &phi_CS, double &costh_HX, double &phi_HX);

  static void calculateU1U2( double fMet , double fMPhi,double fZPt, double fZPhi, double fPt1, double fPhi1, double & fU1,double & fU2 );
  
 private:
  
  static TString getCompleteTitleReturnName(std::string title);

};
