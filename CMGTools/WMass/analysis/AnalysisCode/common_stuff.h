#include <iostream>
#include <TChain.h>
#include <TClonesArray.h>
#include <TString.h>
#include <map>

#include <TH1D.h>
#include <TH2D.h>
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
  
  static void writeOutHistos(TFile *fout, std::map<std::string, TH1D*> h_1d, std::map<std::string, TH2D*> h_2d);

  
 private:
  
  static TString getCompleteTitleReturnName(std::string title);

};
  
