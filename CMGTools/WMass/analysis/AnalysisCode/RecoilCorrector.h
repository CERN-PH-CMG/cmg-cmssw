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
  RecoilCorrector(string iNameZDat, int iSeed=0xDEADBEEF,TString model_name = "fitresult_Add", TString fNonClosure_name = "");
  RecoilCorrector(string iNameZDat1, string iPrefix, int iSeed=0xDEADBEEF);
  ~RecoilCorrector();
  void CorrectAll(double &met, double &metphi, double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double &iU1,double &iU2,double iFluc,double iScale=0,int njet=0);
  void Correct(double &pfmet, double &pfmetphi, double &trkmet, double &trkmetphi, 
  double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double iFluc    ,double iScale=0,int njet=0);
  void CorrectType1(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,double iFlucU2,double iFlucU1,double iScale=0,int njet=0);
  void CorrectType2(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,double iFlucU2,double iFlucU1,double iScale=0,int njet=0,bool doSingleGauss=false);
  void reset(int RecoilCorrParMaxU1, int RecoilCorrParMaxU2);
  void CorrectMET3gaus(double &pfmet, double &pfmetphi,double iGenPt,double iGenPhi,double iLepPt,double iLepPhi,double &iU1,double &iU2,int RecoilCorrVarDiagoParU1orU2fromDATAorMC,int RecoilCorrVarDiagoParN,int RecoilCorrVarDiagoParSigmas,int njet=0,bool doSingleGauss=false, int mytype=0);
  void CorrectU1U2(double &pfu1, double &pfu2, double &trku1, double &trku2, 
  double iGenPt, double iGenPhi, double iLepPt, double iLepPhi,double iFluc,double iScale=0,int njet=0);
  void addDataFile(std::string iNameDat,/* , int RecoilCorrVarDiagoParU1orU2=1, int RecoilCorrU1VarDiagoParN=0, int RecoilCorrVarDiagoParSigmas=0 */
  TString model_name = "fitresult_Add");
  void addMCFile  (std::string iNameMC,TString model_name = "fitresult_Add");
  double NonClosure_weight(double iMet,double iMPhi,double iGenPt,double iGenPhi,double iGenRap, double iLepPt,double iLepPhi);
  double NonClosure_scale(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,double iGenRap, double iLepPt,double iLepPhi);

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

  // void readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
  // std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
  // std::string iFName,std::string iPrefix); 
  void readRecoil(/* std::vector<double> &iSumEt, */
                     std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
                     std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1RMS3Fit,
		     std::vector<TF1*> &iU1FracFit,std::vector<TF1*> &iU1Mean1Fit, std::vector<TF1*> &iU1Mean2Fit,//std::vector<TF1*> &iU1Sig3Fit,
                     std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
                     std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2RMS3Fit,
		     std::vector<TF1*> &iU2FracFit,std::vector<TF1*> &iU2Mean1Fit, std::vector<TF1*> &iU2Mean2Fit,//std::vector<TF1*> &iU2Sig3Fit,
                     std::string iFName,std::string iPrefix,int vtxBin, int mytype=0,/* , int RecoilCorrVarDiagoParU1orU2=1, int RecoilCorrU1VarDiagoParN=0, int RecoilCorrVarDiagoParSigmas=0 */TString model_name = "fitresult_Add");
                     
  void readCorr(std::string iName,//int iType=2,
  std::vector<TF1*> &iF1U1U2Corr  ,std::vector<TF1*> &iF2U1U2Corr,std::vector<TF1*> &iF1F2U1Corr,std::vector<TF1*> &iF1F2U2Corr,
  std::vector<TF1*> &iF1F2U1U2Corr,std::vector<TF1*> &iF1F2U2U1Corr,int iType=2);

  void metDistribution(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
  double iLepPt,double iLepPhi,TRandom3 *iRand,
  TF1 *iU1RZFit, 
  TF1 *iU1MSZFit, 
  TF1 *iU1S1ZFit,
  TF1 *iU1S2ZFit,
  TF1 *iU2MSZFit,		   
  TF1 *iU2S1ZFit, 
  TF1 *iU2S2ZFit, 
  TF1 *iU1U2Corr, 
  double &iU1,double &iU2,double iFluc=0,double iScale=0);

  void metDistribution(double &iPFMet,double &iPFMPhi,double &iTKMet,double &iTKMPhi,
  double iGenPt,double iGenPhi,
  double iLepPt,double iLepPhi,TRandom3 *iRand,
  TF1 *iU1RZPFFit,  TF1 *iU1RZTKFit, 
  TF1 *iU1MSZPFFit, TF1 *iU1MSZTKFit, 
  TF1 *iU1S1ZPFFit, TF1 *iU1S1ZTKFit,
  TF1 *iU1S2ZPFFit, TF1 *iU1S2ZTKFit,
  TF1 *iU2MSZPFFit, TF1 *iU2MSZTKFit,		   
  TF1 *iU2S1ZPFFit, TF1 *iU2S1ZTKFit, 
  TF1 *iU2S2ZPFFit, TF1 *iU2S2ZTKFit, 
  TF1 *iPFU1U2Corr, TF1 *iTKU1U2Corr,
  TF1 *iPFTKU1Corr, TF1 *iPFTKU2Corr,
  TF1 *iPFTKU1MCorr,TF1 *iPFTKU2MCorr,
  double &iU1,double &iU2,double iFluc=0,double iScale=0);

  void metDistributionType1(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
  double iLepPt,double iLepPhi,TRandom3 *iRand,
  TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
  TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit, 
  TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
  double &iU1,double &iU2,double iFlucU2=0,double iFlucU1=0,double iScale=0);

  void metDistributionType2(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
  double iLepPt,double iLepPhi,
  TF1 *iU1Default,
  TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
  TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit, 
  TF1 *iU1S1ZDatFit, TF1 *iU1S1ZMCFit, 
  TF1 *iU1S2ZDatFit, TF1 *iU1S2ZMCFit, 
  TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
  TF1 *iU2S1ZDatFit, TF1 *iU2S1ZMCFit,	   
  TF1 *iU2S2ZDatFit, TF1 *iU2S2ZMCFit,	   
  //			    TF1 *iU1U2ZDatCorr,TF1 *iU1U2ZMCCorr, // MARIA comment for now since not used
  double &iU1, double &iU2,double iFlucU2=0,double iFlucU1=0,double iScale=0,
  bool doSingleGauss=false);

  void metDistributionType2CorrU(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
  double iLepPt,double iLepPhi,
  TF1 *iU1Default,
  TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
  TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
  TF1 *iU1S1ZDatFit, TF1 *iU1S1ZMCFit,
  TF1 *iU1S2ZDatFit, TF1 *iU1S2ZMCFit,
  TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
  TF1 *iU2S1ZDatFit, TF1 *iU2S1ZMCFit,
  TF1 *iU2S2ZDatFit, TF1 *iU2S2ZMCFit,
  //                                         TF1 *iU1U2ZDatCorr,TF1 *iU1U2ZMCCorr, // MARIA comment for now                              
  double &iU1,double &iU2,double iFlucU2, double iFlucU1, double iScale,
  bool doSingleGauss);

  void applyCorrMET3gaus(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
  double iLepPt,double iLepPhi,
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
  void applyCorrMET3gausPDF(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
		     double iLepPt,double iLepPhi,/*TRandom3 *iRand,*/
		     TF1 *iU1Default,
		     TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
		     TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
		     TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
		     //                     RooAddPdf* pdfMCU1, RooAddPdf* pdfDATAU1, 
		     //                     RooAddPdf* pdfMCU2, RooAddPdf* pdfDATAU2 
         int RecoilCorrVarDiagoParU1orU2fromDATAorMC,int RecoilCorrVarDiagoParN,int RecoilCorrVarDiagoParSigmas, 
         int mytype, int fJet, double &pU1, double &pU2
		     );
  double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2);
  double diGausInvGraph(double iPVal,double iFracMC,double iSigma1MC,double iSigma2MC,double iFracDATA,double iSigma1DATA,double iSigma2DATA);
  double oneGausInvGraph(double iPVal,double iFracMC,double iSigma1MC,double iSigma2MC,double iFracDATA,double iSigma1DATA,double iSigma2DATA);
  double getFrac2gauss(double RMS, double sigma1, double sigma2);
  double getFrac3gauss(double RMS, double f1, double sigma1, double sigma2, double sigma3);
  void runDiago(RooWorkspace *w,RooFitResult *result, TString fit, RooAbsReal *&pdfUiCdf);
  double diGausPVal(double iVal, double iFrac,double iSimga1,double iSigma2);
  double triGausInvGraph(double iPVal, /**/ double meanRMSMC, double iMean1MC, double iMean2MC, double iFrac1MC,double iSigma1MC,double iSigma2MC,double iSigma3MC,/**/ double meanRMSDATA, double iMean1DATA, double iMean2DATA,double iFrac1DATA,double iSigma1DATA,double iSigma2DATA,double iSigma3DATA);
  // double triGausInvGraphPDF(double iPVal, double Zpt, RooAddPdf *pdfMC, RooAddPdf *pdfDATA, RooWorkspace *wMC, RooWorkspace *wDATA);
  double triGausInvGraphPDF(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA);
  double diGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2);
  double oneGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2);
  double getError(double iVal,TF1 *iZDatFit,Recoil iType);
  double getError2(double iVal,TF1 *iFit);
  double getCorError2(double iVal,TF1 *iFit);
  double mag(double iV0,double iV1,double iV2,double iV3);
  double correlatedSeed(double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3);
  double deCorrelate   (double iVal, double iCorr1,double iCorr2,double iCorr3,double iSeed0,double iSeed1,double iSeed2,double iSeed3);
  TF1*   getFunc(bool iMC, Recoil iType);
  double CorrVal(double iPt,double iVal,Recoil iType);
  //void   Correct(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double iFluc,int njet);
  
  static const int lNBins = 2;//lGraph->GetN();                   
  RooWorkspace *wU1[3][lNBins],*wU2[3][lNBins],*wU1diago[3][lNBins],*wU2diago[3][lNBins];
  RooAddPdf *pdfU1[3][lNBins],*pdfU2[3][lNBins];
  RooAbsReal *pdfU1Cdf[3][lNBins],*pdfU2Cdf[3][lNBins];
  RooFitResult* frU1[3][lNBins];
  RooFitResult* frU2[3][lNBins];
  TFile *fNonClosure; 
  TH2D *hNonClosure[2][2]; 
  TRandom3 *fRandom; 
  vector<TF1*> fF1U1Fit; vector<TF1*> fF1U1RMSSMFit; vector<TF1*> fF1U1RMS1Fit; vector<TF1*> fF1U1RMS2Fit, fF1U1RMS3Fit, fF1U1FracFit, fF1U1Mean1Fit, fF1U1Mean2Fit; 
  vector<TF1*> fF1U2Fit; vector<TF1*> fF1U2RMSSMFit; vector<TF1*> fF1U2RMS1Fit; vector<TF1*> fF1U2RMS2Fit, fF1U2RMS3Fit, fF1U2FracFit, fF1U2Mean1Fit, fF1U2Mean2Fit;; 
  vector<TF1*> fF2U1Fit; vector<TF1*> fF2U1RMSSMFit; vector<TF1*> fF2U1RMS1Fit; vector<TF1*> fF2U1RMS2Fit, fF2U1RMS3Fit, fF2U1FracFit, fF2U1Mean1Fit, fF2U1Mean2Fit; 
  vector<TF1*> fF2U2Fit; vector<TF1*> fF2U2RMSSMFit; vector<TF1*> fF2U2RMS1Fit; vector<TF1*> fF2U2RMS2Fit, fF2U2RMS3Fit, fF2U2FracFit, fF2U2Mean1Fit, fF2U2Mean2Fit; 

  vector<TF1*> fD1U1Fit; vector<TF1*> fD1U1RMSSMFit; vector<TF1*> fD1U1RMS1Fit; vector<TF1*> fD1U1RMS2Fit,fD1U1RMS3Fit, fD1U1FracFit, fD1U1Mean1Fit, fD1U1Mean2Fit; 
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
  int fId; int fJet;

};



#endif
