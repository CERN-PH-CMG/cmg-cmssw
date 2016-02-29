#ifndef CMGTools_Utilities_RecoilCorrector_H
#define CMGTools_Utilities_RecoilCorrector_H


#include <vector>
#include <sstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom3.h"
#include "PdfDiagonalizer.h"
#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "RooAddPdf.h"
#include "RooAbsReal.h"
#include "RooKeysPdf.h"

//
// ** apply phil's recoil corrections **
// 
// usage: 
//    double met=rawMetValue;
//    double metphi=rawMetPhiValue;
//    RecoilCorrector corrector;
//    corrector->Correct(met,metphi,GenZPt,GenZPhi,leptonPt,leptonPhi);
//    printf("corrected met: %10.2f%10.2f\n",met,metphi);
//
// where leptonPt, leptonPhi are dilepton kinematics for z->ll and single lepton kinematics for w->lnu
//

using namespace std;

class RecoilCorrector
{

public:
  RecoilCorrector(bool doKeys, string iNameZDat, string iNameZ_key, TString model_name = "fitresult_Add");
  void reset(int RecoilCorrParMaxU1, int RecoilCorrParMaxU2, int rapBinCorr);
  void CorrectMET3gaus(double &pfmet, double &pfmetphi,double bosonPt,double bosonPhi,double sumLepPt,double sumLepPhi,double &iU1,double &iU2,int RecoilCorrVarDiagoParU1orU2fromDATAorMC,int RecoilCorrVarDiagoParN,int RecoilCorrVarDiagoParSigmas,int njet=0, int recoilCorrSigmas=1);
  void CorrectU1U2(double &pfu1, double &pfu2, double &trku1, double &trku2, 
    double bosonPt, double bosonPhi, double iLepPt, double iLepPhi,double iFluc,double iScale=0,int njet=0);
  void addDataFile(std::string iNameDat, std::string iNameKeyDat, TString model_name = "fitresult_Add");
  void addMCFile  (std::string iNameMC, std::string iNameKeyMC, TString model_name = "fitresult_Add");

protected:
  enum Recoil { 
    PFU1,
    PFU2,
    PFMSU1,
    PFMSU2,
    PFS1U1,
    PFS2U1,
    PFS1U2,
    PFS2U2,
    TKU1,
    TKU2,
    TKMSU1,
    TKMSU2,
    TKS1U1,
    TKS2U1,
    TKS1U2,
    TKS2U2
  };

  void readRecoil(
    std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
    std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1RMS3Fit,
    std::vector<TF1*> &iU1FracFit,std::vector<TF1*> &iU1Mean1Fit, std::vector<TF1*> &iU1Mean2Fit,
    std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
    std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2RMS3Fit,
    std::vector<TF1*> &iU2FracFit,std::vector<TF1*> &iU2Mean1Fit, std::vector<TF1*> &iU2Mean2Fit,
    std::string iFName,std::string iFKeyName, std::string iPrefix, int mytype, TString model_name = "fitresult_Add"
  );

  void applyCorrMET3gaus(double &iMet,double &iMETPhi,double bosonPt,double bosonPhi,
    double i,double iLepPhi,
    TF1 *iU1Default,
    TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
    TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
    TF1 *iU1S1ZDatFit, TF1 *iU1S1ZMCFit,
    TF1 *iU1S2ZDatFit, TF1 *iU1S2ZMCFit,
    TF1 *iU1S3ZDatFit, TF1 *iU1S3ZMCFit,
    TF1 *iU1fracZDatFit, TF1 *iU1fracZMCFit,
    TF1 *iU1mean1ZDatFit, TF1 *iU1mean1ZMCFit,
    TF1 *iU1mean2ZDatFit, TF1 *iU1mean2ZMCFit,
    //
    TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
    TF1 *iU2S1ZDatFit, TF1 *iU2S1ZMCFit,
    TF1 *iU2S2ZDatFit, TF1 *iU2S2ZMCFit,
    TF1 *iU2S3ZDatFit, TF1 *iU2S3ZMCFit,
    TF1 *iU2fracZDatFit, TF1 *iU2fracZMCFit,
    TF1 *iU2mean1ZDatFit, TF1 *iU2mean1ZMCFit,
    TF1 *iU2mean2ZDatFit, TF1 *iU2mean2ZMCFit,
    double iFlucU2,double iFlucU1,double iScale, int mytype
  );
  void applyCorrMET3gausPDF(double &iMet,double &iMETPhi,double bosonPt,double bosonPhi,
    double sumLepPt,double sumLepPhi,
    TF1 *iU1Default,
    TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
    TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
    TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
    int rapbin, int nSigmas, double &pU1, double &pU2
  );
  double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2);
  void runDiago(RooWorkspace *w,RooFitResult *result, TString fit, RooAbsReal *&pdfUiCdf);
  void makeKeysVec(RooWorkspace *w, TFile *file, TString fit, std::vector<RooAbsReal*> &pdfUiCdfm, bool isU1);
  double keysInvGraph(double iPVal, double Zpt, std::vector<RooAbsReal*> pdfKeyMCcdf, std::vector<RooAbsReal*> pdfKeyDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA, bool isU1, double max);
  double triGausInvGraphPDF(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA, double max);
  
  static const int rapbins = 2;
  enum filetype { targetMC , ZDATA , ZMC };
  static const int Ntypes = 3;
  
  RooWorkspace *wU1[Ntypes][rapbins],*wU2[Ntypes][rapbins];
  RooWorkspace *wU1diago[Ntypes][rapbins],*wU2diago[Ntypes][rapbins];
  RooWorkspace *wU1key[Ntypes][rapbins],*wU2key[Ntypes][rapbins];
  
  RooAddPdf *pdfU1[Ntypes][rapbins],*pdfU2[Ntypes][rapbins];
  RooAbsReal *pdfU1Cdf[Ntypes][rapbins],*pdfU2Cdf[Ntypes][rapbins];
  
  vector<RooAbsReal*> pdfKeyU1Cdf[Ntypes][rapbins], pdfKeyU2Cdf[Ntypes][rapbins];
  
  RooFitResult* frU1[Ntypes][rapbins];
  RooFitResult* frU2[Ntypes][rapbins];
  
  vector<TF1*> fF1U1Fit; vector<TF1*> fF1U1RMSSMFit; vector<TF1*> fF1U1RMS1Fit; vector<TF1*> fF1U1RMS2Fit, fF1U1RMS3Fit, fF1U1FracFit, fF1U1Mean1Fit, fF1U1Mean2Fit; 
  vector<TF1*> fF1U2Fit; vector<TF1*> fF1U2RMSSMFit; vector<TF1*> fF1U2RMS1Fit; vector<TF1*> fF1U2RMS2Fit, fF1U2RMS3Fit, fF1U2FracFit, fF1U2Mean1Fit, fF1U2Mean2Fit; 
  vector<TF1*> fF2U1Fit; vector<TF1*> fF2U1RMSSMFit; vector<TF1*> fF2U1RMS1Fit; vector<TF1*> fF2U1RMS2Fit, fF2U1RMS3Fit, fF2U1FracFit, fF2U1Mean1Fit, fF2U1Mean2Fit; 
  vector<TF1*> fF2U2Fit; vector<TF1*> fF2U2RMSSMFit; vector<TF1*> fF2U2RMS1Fit; vector<TF1*> fF2U2RMS2Fit, fF2U2RMS3Fit, fF2U2FracFit, fF2U2Mean1Fit, fF2U2Mean2Fit; 

  vector<TF1*> fD1U1Fit; vector<TF1*> fD1U1RMSSMFit; vector<TF1*> fD1U1RMS1Fit; vector<TF1*> fD1U1RMS2Fit, fD1U1RMS3Fit, fD1U1FracFit, fD1U1Mean1Fit, fD1U1Mean2Fit; 
  vector<TF1*> fD1U2Fit; vector<TF1*> fD1U2RMSSMFit; vector<TF1*> fD1U2RMS1Fit; vector<TF1*> fD1U2RMS2Fit, fD1U2RMS3Fit, fD1U2FracFit, fD1U2Mean1Fit, fD1U2Mean2Fit; 
  vector<TF1*> fD2U1Fit; vector<TF1*> fD2U1RMSSMFit; vector<TF1*> fD2U1RMS1Fit; vector<TF1*> fD2U1RMS2Fit, fD2U1RMS3Fit, fD2U1FracFit, fD2U1Mean1Fit, fD2U1Mean2Fit; 
  vector<TF1*> fD2U2Fit; vector<TF1*> fD2U2RMSSMFit; vector<TF1*> fD2U2RMS1Fit; vector<TF1*> fD2U2RMS2Fit, fD2U2RMS3Fit, fD2U2FracFit, fD2U2Mean1Fit, fD2U2Mean2Fit; 

  vector<TF1*> fM1U1Fit; vector<TF1*> fM1U1RMSSMFit; vector<TF1*> fM1U1RMS1Fit; vector<TF1*> fM1U1RMS2Fit, fM1U1RMS3Fit, fM1U1FracFit, fM1U1Mean1Fit, fM1U1Mean2Fit; 
  vector<TF1*> fM1U2Fit; vector<TF1*> fM1U2RMSSMFit; vector<TF1*> fM1U2RMS1Fit; vector<TF1*> fM1U2RMS2Fit, fM1U2RMS3Fit, fM1U2FracFit, fM1U2Mean1Fit, fM1U2Mean2Fit; 
  vector<TF1*> fM2U1Fit; vector<TF1*> fM2U1RMSSMFit; vector<TF1*> fM2U1RMS1Fit; vector<TF1*> fM2U1RMS2Fit, fM2U1RMS3Fit, fM2U1FracFit, fM2U1Mean1Fit, fM2U1Mean2Fit; 
  vector<TF1*> fM2U2Fit; vector<TF1*> fM2U2RMSSMFit; vector<TF1*> fM2U2RMS1Fit; vector<TF1*> fM2U2RMS2Fit, fM2U2RMS3Fit, fM2U2FracFit, fM2U2Mean1Fit, fM2U2Mean2Fit; 

  vector<TF1*> fF1U1U2Corr;     vector<TF1*> fF2U1U2Corr;
  vector<TF1*> fF1F2U1Corr;     vector<TF1*> fF1F2U2Corr;
  vector<TF1*> fF1F2U1U2Corr;   vector<TF1*> fF1F2U2U1Corr;

  vector<TF1*> fM1U1U2Corr;     vector<TF1*> fM2U1U2Corr;
  vector<TF1*> fM1M2U1Corr;     vector<TF1*> fM1M2U2Corr;
  vector<TF1*> fM1M2U1U2Corr;   vector<TF1*> fM1M2U2U1Corr;

  bool doKeys;

};


#endif
