#ifndef H2TAUTAUFITTER_H
#define H2TAUTAUFITTER_H

#include <TROOT.h>
#include <TNamed.h>
#include <TFile.h>
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>


#include <RooAbsPdf.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>

#include <iostream>

using namespace std;


class H2TauTauFitter: TNamed {

public:
  
  H2TauTauFitter(TString outPutPath);
  ~H2TauTauFitter();
  
  void SetDataSet(RooDataSet* data){data_=data;}
  void SetSignalPdf(RooAbsPdf* pdf){signalPdf_=pdf;}
  void SetBackgroundPdf(RooAbsPdf* pdf){backgroundPdf_=pdf;}

  Bool_t fit();

  void SaveFitResults();
  
  
private:

  TString outPutPath_;

  RooDataSet* data_;
  RooAbsPdf* signalPdf_;
  RooAbsPdf* backgroundPdf_;
  RooAddPdf* totalPdf_;
  
  RooRealVar signalYield_;

  RooFitResult* fitResults_;
  Bool_t fitSucceeded_;

  fstream logfile_;
};

#endif
