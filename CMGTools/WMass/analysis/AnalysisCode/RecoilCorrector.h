#ifndef CMGTools_Utilities_RecoilCorrector_H
#define CMGTools_Utilities_RecoilCorrector_H

#include <vector>
#include <string>

#include "TFile.h"
#include "TString.h"
#include "TF1.h"
#include "TMath.h"

#include "RooDataSet.h"
#include "RooWorkspace.h"
#include "RooAddPdf.h"
#include "RooAbsReal.h"
#include "RooKeysPdf.h"

#include "PdfDiagonalizer.h"

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

class RecoilCorrector
{

public:
  RecoilCorrector(bool loadKeys, std::string iNameZDat, std::string iNameZ_key, TString model_name = "fitresult_Add");
  void reset(int RecoilCorrParMaxU1, int RecoilCorrParMaxU2, int rapBinCorr);
  double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2);
  void CorrectMET(double &pfmet, double &pfmetphi,double bosonPt,double bosonPhi,double sumLepPt,double sumLepPhi,double &iU1,double &iU2,int RecoilCorrVarDiagoParU1orU2fromDATAorMC,int RecoilCorrVarDiagoParN,int RecoilCorrVarDiagoParSigmas, int rapbin, int recoilCorrScale=1, bool correctWithKeys=false);
  void addDataFile(std::string iNameDat, std::string iNameKeyDat, TString model_name = "fitresult_Add");
  void addMCFile  (std::string iNameMC, std::string iNameKeyMC, TString model_name = "fitresult_Add");

protected:
  void readRecoil(
    std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
    std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
    std::string iFName,std::string iFKeyName, std::string iPrefix, int mytype, TString model_name = "fitresult_Add"
  );

  void applyMETCorrection(double &iMet,double &iMETPhi,double bosonPt,double bosonPhi,
    double sumLepPt,double sumLepPhi,
    TF1 *iU1Default,
    TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
    TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
    TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
    int rapbin, int corrScale, bool correctWithKeys,
    double &pU1, double &pU2
  );

  void runDiago(RooWorkspace *w,RooFitResult *result, TString fit, RooAbsReal *&pdfUiCdf);
  void makeKeysVec(RooWorkspace *w, TFile *file, TString fit, std::vector<RooAbsReal*> &pdfUiCdfm, bool isU1);
  double keysInvGraph(double iPVal, double Zpt, std::vector<RooAbsReal*> pdfKeyMCcdf, std::vector<RooAbsReal*> pdfKeyDATAcdf, bool isU1, double max);
  double triGausInvGraphPDF(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA, double max);
  
  static const int rapbins = 2;
  enum filetype { targetMC , ZDATA , ZMC };
  static const int Ntypes = 3;
  
  RooWorkspace *wU1[Ntypes][rapbins],*wU2[Ntypes][rapbins];
  RooWorkspace *wU1diago[Ntypes][rapbins],*wU2diago[Ntypes][rapbins];
  RooWorkspace *wU1key[Ntypes][rapbins],*wU2key[Ntypes][rapbins];
  
  RooAddPdf *pdfU1[Ntypes][rapbins],*pdfU2[Ntypes][rapbins];
  RooAbsReal *pdfU1Cdf[Ntypes][rapbins],*pdfU2Cdf[Ntypes][rapbins];
  
  std::vector<RooAbsReal*> pdfKeyU1Cdf[Ntypes][rapbins], pdfKeyU2Cdf[Ntypes][rapbins];
  
  RooFitResult* frU1[Ntypes][rapbins];
  RooFitResult* frU2[Ntypes][rapbins];
  
  std::vector<TF1*> fF1U1Fit; std::vector<TF1*> fF1U1RMSSMFit;
  std::vector<TF1*> fF1U2Fit; std::vector<TF1*> fF1U2RMSSMFit;

  std::vector<TF1*> fD1U1Fit; std::vector<TF1*> fD1U1RMSSMFit;
  std::vector<TF1*> fD1U2Fit; std::vector<TF1*> fD1U2RMSSMFit;

  std::vector<TF1*> fM1U1Fit; std::vector<TF1*> fM1U1RMSSMFit;
  std::vector<TF1*> fM1U2Fit; std::vector<TF1*> fM1U2RMSSMFit;

  bool loadKeys;

};


#endif
