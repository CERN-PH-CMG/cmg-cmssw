#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TF1.h"
#include "TStyle.h"
#include "TMath.h"
#include "TVirtualFitter.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TRandom1.h"
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
///#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"
#include "TLatex.h"

#include "TMatrixDSymEigen.h"
#include "TVectorD.h"
#include "TMatrixD.h"

#include <vector>
#include "TLorentzVector.h"

using namespace RooFit;

/////

TH1D histoU1O("hu1O","histo U1",100,-10,10);
TH1D histoU1Z("hu1Z","histo U1",100,-10,10);
TH1D histoU1("hu1","histo U1",100,-10,10);
TH2D histoU1vsZpt("hu1vsZpt","histo U1 vsZpt",100,0,20, 100,-10,10);
TH2D histoU1vsU2("hu1vsu2","histo U1 vs U2",100,-10,10, 100,-10,10);

TH2D histoU1vsZptfit("hu1vsZptfit","histo U1 vsZpt",500,0,50, 2000,-100,100);

TH1D histoU2("hu2","histo U2",100,-10,10);

/////

bool usePol3=false;
bool dodebug=false; 

bool doType2=true;
bool doSingleGauss=false;

bool doOnlyU=false;
bool doOnlyU1=false;
bool doOnlyU2=false;

bool doPrint=false;
bool doPrintAll=false; // when this is set to true, do the binned
bool doBinned=false;

bool doNeutralMet=false;
bool doChargedMet=true;
bool donoPU=false;

bool doIterativeMet=false;
bool doSecondStep=false;
bool doAbsolute=false;

bool doMetPhiCorr=false;
bool doPosW=false;
bool doNegW=false;

bool doRunA=false;
bool doRunB=false;

bool doVTXbinning=false;
bool doYbinning=true;

double etaMuonSel = 2.1;
//double etaMuonSel = 1.6;
//double etaMuonSel = 1.1;
int pType=-1;

////
TFile *fDataFile = 0;
TTree *fDataTree = 0;

TFile *fTTbarFile    = 0;
TTree *fTTbarTree    = 0;
////

bool  fData   = false;
float fMetMax = 0; float fZPtMin = 0; float fZPtMax = 0; int fNJetSelect = 0;      
/*float   fNPV    = 0;*/ int   fNPV    = 0;
/*float fU1 = 0; float fU2 = 0; */
/*float fTKU1 = 0; float fTKU2 = 0;*/
//float fPFU1 = 0;  float fPFU2   = 0; 

double fPFU1 = 0;  double fPFU2   = 0;
double fU1 = 0; double fU2 = 0;
double fTKU1 = 0; double fTKU2 = 0;

/*float fZPt = 0; float fZPhi = 0; float fPhi = 0; */ double fZPt = 0; double fZPhi = 0; double fZRap = 0; double fPhi = 0;
/*float fMet = 0;  float fMPhi = 0; */ 
double fMet = 0;  double fMPhi = 0;
double ftkMet = 0;  double ftkMPhi = 0;
//double fneuMet = 0;  double fneuPhi = 0; 

/*float fPt1 = 0; float fPhi1 = 0; float fPt2 = 0; float fEta2 = 0; float fPhi2 = 0; */
double fPt1 = 0; double fPhi1 = 0; double fPt2 = 0; double fEta2 = 0; double fPhi2 = 0; 

int fNJet = 0; float fWeight = 0;

int fId1 = 0; int fId2 = 0; float fGPhi1 = 0; float fGPhi2 = 0; float fGEta1  = 0; float fGEta2 = 0;
int fNBTag = 0; float fMJJ = 0; /*float fJPt1 = 0;*/ double fJPt1 = 0; float fDEta = 0; int fIS2011A = 0; int fId = -1; float fVPt = 0; float fVPhi = 0;

double fpdgId1, fpdgId2;

double fZmass;
double fMuPos_phi, fMuPos_eta, fMuPos_pt, fMuPos_charge, fMuPosReliso, fMuPos_dxy, fMuPos_dz;
double fMuNeg_phi, fMuNeg_eta, fMuNeg_pt, fMuNeg_charge,fMuNeg_dxy, fMuNeg_dz, fMuNegReliso;
int fMuPosTrg, fMuPosIsTightAndIso, fevtHasGoodVtx, fevtHasTrg, fMuNegIsTightAndIso, fMuNegTrg;
int fMuPosIsTight, fMuNegIsTight;

int fRun=-1;
int fLumi=-1;
int fEvent=-1;

std::vector<std::vector<std::vector<float> > > lXVals_all;  std::vector<std::vector<std::vector<float> > > lYVals_all; std::vector<std::vector<std::vector<float> > > lYTVals_all;
std::vector<std::vector<std::vector<float> > > lXEVals_all; std::vector<std::vector<std::vector<float> > > lYEVals_all; 

float ***vlXVals_all,***vlXEVals_all,***vlYVals_all,***vlYTVals_all,***vlYEVals_all; //two * are needed because it is a pofloater to a pofloater

///$$$$$$$$$$$$
///$$$$
///$$$$  UTILITY to apply type1
///$$$$
///$$$$$$$$$$$$  


vector<double> lZMSumEt; // vector<double> lZMWeights;                                                                                                          
vector<double> lZMSumEtbis; // vector<double> lZMWeights;                                                                                                          
vector<double> lZDSumEt; // vector<double> lZMWeights;                                                                                                          
std::vector<TF1*>   lZDU1Fit; std::vector<TF1*> lZDU1RMSSMFit;
std::vector<TF1*>   lZDU2Fit; std::vector<TF1*> lZDU2RMSSMFit;
std::vector<TF1*>   lZDU1RMS1Fit; std::vector<TF1*> lZDU1RMS2Fit;
std::vector<TF1*>   lZDU2RMS1Fit; std::vector<TF1*> lZDU2RMS2Fit;

std::vector<TF1*>   lZMU1Fit; std::vector<TF1*> lZMU1RMSSMFit;
std::vector<TF1*>   lZMU2Fit; std::vector<TF1*> lZMU2RMSSMFit;
std::vector<TF1*>   lZMU1RMS1Fit; std::vector<TF1*> lZMU1RMS2Fit;
std::vector<TF1*>   lZMU2RMS1Fit; std::vector<TF1*> lZMU2RMS2Fit;

vector<double> lZMSumEt_second; // vector<double> lZMWeights;                                                                                                          
std::vector<TF1*>   lZMU1Fit_second; std::vector<TF1*> lZMU1RMSSMFit_second;
std::vector<TF1*>   lZMU2Fit_second; std::vector<TF1*> lZMU2RMSSMFit_second;
std::vector<TF1*>   lZMU1RMS1Fit_second; std::vector<TF1*> lZMU1RMS2Fit_second;
std::vector<TF1*>   lZMU2RMS1Fit_second; std::vector<TF1*> lZMU2RMS2Fit_second;

//int vtxBin=-1;                                                                                                                                                
const int readRecoil(std::vector<double> &iSumEt,
                     std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
                     std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,//std::vector<TF1*> &iU1Sig3Fit,                                 
                     std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
                     std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,//std::vector<TF1*> &iU2Sig3Fit,                                               
                     std::string iFName = "recoilfit.root",std::string iPrefix="",int vtxBin=-1, int type=0) {

  //type=1 read U1; type=2 read U2;
  cout << "inside readRecoil" << endl;

  TFile *lFile  = new TFile(iFName.c_str());
  lFile->ls();

  //TGraph *lGraph = (TGraph *) lFile->FindObjectAny("sumet");                                                                                                  
  //  const int lNBins = 20;//lGraph->GetN();                   
  const int lNBins = 201;//lGraph->GetN();                   

  //  int init = -1; // this is for inclusive
  int init = 1; // this is for the binned

  for(int i0 = init; i0 <= lNBins; i0++) {
    if(i0!=vtxBin) continue;
    cout << "reading bin " << i0 << endl;

    std::string lStr = iPrefix;
    //iSumEt.push_back(lGraph->GetY()[i0]);                                                                                                                     

    std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10;

    if(type==0) {
      pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);              
      pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
      pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);         
      pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);              
      pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);           
      pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);        
    }
    if(type==1) {
      pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);              
      pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
      pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);         
    }
    if(type==2) {
      pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);              
      pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);           
      pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);        
    }
  }

  cout << "read U1 size: " << iU1Fit.size() << endl;

  lFile->Close();
  iSumEt.push_back(1000);
  return lNBins;
}

double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2) {
  double lMX = -iEPt*cos(iEPhi) - iU1*cos(iWPhi) + iU2*sin(iWPhi);
  double lMY = -iEPt*sin(iEPhi) - iU1*sin(iWPhi) - iU2*cos(iWPhi);

  if(iMet == 0) return sqrt(lMX*lMX + lMY*lMY);
  if(iMet == 1) {if(lMX > 0) {return atan(lMY/lMX);} return (fabs(lMY)/lMY)*3.14159265 + atan(lMY/lMX); }
  if(iMet == 2) return lMX;
  if(iMet == 3) return lMY;
  return lMY;

}

void applyType1CorrMET(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
                       double iLepPt,double iLepPhi,TRandom3 *iRand,
                       TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
                       TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
                       TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit
                       /*double &iU1,double &iU2,double iFluc,double iScale*/) {

  double lRescale  = sqrt((TMath::Pi())/2.);
  double pU1       = iU1RZDatFit->Eval(iGenPt)/iU1RZMCFit->Eval(iGenPt);
  double pU2       = 0; //Right guys are for cumulants => code deleted

  //  cout << "genPt " << iGenPt  << "Data " << iU1RZDatFit->Eval(iGenPt) << " MC "<< iU1RZMCFit->Eval(iGenPt)  << " scaleFactor " << pU1 << endl;

  double pFrac1    = max( iU1MSZDatFit->Eval(iGenPt)*iU1MSZDatFit->Eval(iGenPt)
                          -(pU1*pU1)*iU1MSZMCFit->Eval(iGenPt)*iU1MSZMCFit->Eval(iGenPt),0. );
  double pFrac2    = max( iU2MSZDatFit->Eval(iGenPt)*iU2MSZDatFit->Eval(iGenPt)
                          -iU2MSZMCFit->Eval(iGenPt)*iU2MSZMCFit ->Eval(iGenPt),0. );


  pFrac1 = sqrt(pFrac1)*lRescale;
  pFrac2 = sqrt(pFrac2)*lRescale;

  //NEED TO ADD Uncertainty propagation later 

  double pUX  = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY  = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU   = sqrt(pUX*pUX+pUY*pUY);

  double pCos = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  ///// V1 (original)

  pU1   = iRand->Gaus(pU*pCos*pU1,pFrac1);
  pU2   = iRand->Gaus(pU*pSin,pFrac2);

  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  //  iU1   = pU1;
  //  iU2   = pU2;
                                                                                                                                              
  return;

}

double diGausPVal(double iVal,double iFrac,double iSigma1,double iSigma2) { 
  return iFrac*TMath::Erf(iVal/iSigma1) + (1-iFrac)*TMath::Erf(iVal/iSigma2);
}

double diGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2) { 
  double lVal = TMath::ErfInverse(iPVal);
  double lMin = lVal * ((iSigma1 < iSigma2) ? iSigma1 : iSigma2);
  double lMax = lVal * ((iSigma1 < iSigma2) ? iSigma2 : iSigma1);
  //  cout << "-- Min - " << lMin <<  " -> " << lMax << " -- " << iSigma1 << " -- " << iSigma2 << endl;
  double lDiff = (lMax-lMin);
  //Iterative procedure to invert a double gaussian given a PVal
  int lId = 0; int lN1 = 4;  int lN2 = 10; 
  for(int i0 = 0; i0 < lN1; i0++) { 
    if(i0 != 0) lMin = lMin + (lId-1)*lDiff/lN2;
    if(i0 != 0) lDiff/=lN2;
    for(int i1 = 0; i1 < lN2; i1++) { 
      double pVal = lMin + lDiff/lN2*i1;
      pVal = diGausPVal(pVal,iFrac,iSigma1,iSigma2);
      if(pVal > iPVal) {lId = i1; break;}
      //      if(pVal < iPVal && lDiff < 0 ) {lId = i1; break;}
    }
  }
  //  cout << "-- Final Val "  <<  (lMin + (lId-0.5)*lDiff/lN2) << " -- " << lId << endl;
  return (lMin + (lId-0.5)*lDiff/lN2);
}

double oneGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2) { 

  double lVal = TMath::ErfInverse(iPVal);
  double pVal = lVal *iSigma1;
  //  double sigmaD = diGausPVal(pVal,iFrac,iSigma1,iSigma2);

  return pVal;
}

void applyType2CorrU(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
		     double iLepPt,double iLepPhi,/*TRandom3 *iRand,*/
		     TF1 *iU1Default,
		     TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
		     TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit, 
		     TF1 *iU1S1ZDatFit, TF1 *iU1S1ZMCFit, 
		     TF1 *iU1S2ZDatFit, TF1 *iU1S2ZMCFit, 
		     TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
		     TF1 *iU2S1ZDatFit, TF1 *iU2S1ZMCFit,        
		     TF1 *iU2S2ZDatFit, TF1 *iU2S2ZMCFit ){        
  
  double pDefU1    = iU1Default->Eval(iGenPt);
  double lRescale  = sqrt((TMath::Pi())/2.);     
  double pDU1       = iU1RZDatFit ->Eval(iGenPt);
  //double pDU2       = 0; sPM
  double pDFrac1    = iU1MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt)*pDFrac1;
  double pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt)*pDFrac1;
  if(doAbsolute) pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt);
  if(doAbsolute) pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt);

  double pDFrac2    = iU2MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*pDFrac2;
  double pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*pDFrac2;
  if(doAbsolute) pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt);
  if(doAbsolute) pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt);
  //double pDMean1    = pDFrac1;
  //double pDMean2    = pDFrac2;
 
  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMU2       = 0; 
  double pMFrac1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt)*pMFrac1;
  double pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt)*pMFrac1;
  if(doAbsolute) pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt);
  if(doAbsolute) pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt);

  double pMFrac2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt)*pMFrac2;
  double pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt)*pMFrac2;
  if(doAbsolute) pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt);
  if(doAbsolute) pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt);

  //double pMMean1    = pMFrac1;
  //double pMMean2    = pMFrac2;
  //Uncertainty propagation
  pDFrac1     = (pDFrac1-pDSigma1_2)/(pDSigma1_1-pDSigma1_2);
  pDFrac2     = (pDFrac2-pDSigma2_2)/(pDSigma2_1-pDSigma2_2);
  pMFrac1     = (pMFrac1-pMSigma1_2)/(pMSigma1_1-pMSigma1_2);
  pMFrac2     = (pMFrac2-pMSigma2_2)/(pMSigma2_1-pMSigma2_2);

  /////
  /////
  /////

  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  double offset = iU1RZMCFit->Eval(iGenPt);
  if(fData) offset = iU1RZDatFit->Eval(iGenPt);
 
  double normSigma = iU1MSZMCFit ->Eval(iGenPt)/iU2MSZMCFit ->Eval(iGenPt);
  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-offset; // pU1 of the event ; pDefU1 is the scale iU1RZDatFit ->Eval(iGenPt);
  double pU2Diff  = pU2;
  pU1Diff*=normSigma;

  if(iGenPt<20) {
    histoU1vsZpt.Fill(iGenPt,pU1Diff);
    histoU1vsU2.Fill(pU1Diff,pU2Diff);
    histoU1.Fill(pU1Diff);
    histoU1O.Fill(pU1);
    histoU1Z.Fill(pU1+iGenPt);
    histoU2.Fill(pU2Diff);
  }

  double recoil = sqrt(pU1Diff*pU1Diff+pU2Diff*pU2Diff); // this is scaled a bit

  if(dodebug) cout << "==> Pt " << iGenPt << " pU1: " << pU1 << " pDefU1 " << pDefU1 << " offset: " << offset << " pU1Diff: " << pU1Diff << " recoil " << recoil << endl;

  /////
  /////
  /////

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);
  double pTU1Diff        = pU1Diff;
  
  //  cout << "------------"<< endl;
  //  cout << "diGAUSS "<< endl;
  //  cout << "  pU1ValM " << pU1ValM  << " pU1ValD " << pU1ValD  <<endl;
  //  cout << "  pU2ValM " << pU2ValM  << " pU2ValD " << pU2ValD  <<endl;

  double pU1ValM = 0;
  double pU2ValM = 0 ;
  double pU1ValD = 0 ;
  double pU2ValD = 0;
  double pUValM = 0;
  double pUValD = 0 ;

  if(doSingleGauss) {

    //    pU1ValM         = diGausPVal(fabs(pU1Diff),1,iU1MSZMCFit ->Eval(iGenPt)*lRescale,0); // when is singleGauss pMFrac1=1 pMSigma1_1=fullRMS pMSigma1_2=0 
    //    pU2ValM         = diGausPVal(fabs(pU2Diff),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
    pUValM         = diGausPVal(fabs(recoil),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
    
    //    pU1ValD         = oneGausPInverse(pU1ValM  ,1,iU1MSZDatFit->Eval(iGenPt)*lRescale,0);
    //    pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0);
    pUValD         = oneGausPInverse(pUValM ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0);
    
  }

  //  cout << "oneGAUSS " << endl;
  //  cout << " iU1MSZMCFit ->Eval(iGenPt) " << iU1MSZMCFit ->Eval(iGenPt) << " U2MSZMCFit ->Eval(iGenPt) " << iU2MSZMCFit ->Eval(iGenPt) << endl;
  //  cout << "  pU1ValM " << pU1ValM_one  << " pU1ValD " << pU1ValD_one  << endl;
  //  cout << "  pU2ValM " << pU2ValM_one  << " pU2ValD " << pU2ValD_one  << endl;

  pU1ValD=pUValD*(pU1/pU)*(1/normSigma);
  pU2ValD=pUValD*(pU2/pU);

  if(dodebug) cout << " ==> pU1 " << pU1 << " pU1ValD " << pU1ValD << " pU2 " << pU2 << " pU2ValD " << pU2ValD << endl;

  pU1ValD*=p1Charge;
  pU2ValD*=p2Charge;

  if(dodebug)   cout << " ==> Zpt: " << iGenPt << " pDef1: " << pDefU1 << " pDU1: " << pDU1 << " pMU1: " << pMU1 << endl;

  pDefU1 *= (pDU1/pMU1);

  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  //  iU1   = pU1; 
  //  iU2   = pU2;

  return;

}

void applyType2CorrMET(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
                       double iLepPt,double iLepPhi,/*TRandom3 *iRand,*/
		       TF1 *iU1Default,
		       TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
		       TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit, 
		       TF1 *iU1S1ZDatFit, TF1 *iU1S1ZMCFit, 
		       TF1 *iU1S2ZDatFit, TF1 *iU1S2ZMCFit, 
		       TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
		       TF1 *iU2S1ZDatFit, TF1 *iU2S1ZMCFit,        
		       TF1 *iU2S2ZDatFit, TF1 *iU2S2ZMCFit ){        

  //  cout << "inside metType2 " << endl;
  //  bool doAbsolute=true;

  //  bool doSingleGauss=true;

  double pDefU1    = iU1Default->Eval(iGenPt);
  double lRescale  = sqrt((TMath::Pi())/2.);     
  double pDU1       = iU1RZDatFit ->Eval(iGenPt);
  //double pDU2       = 0; sPM
  double pDFrac1    = iU1MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt)*pDFrac1;
  double pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt)*pDFrac1;
  if(doAbsolute) pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt);
  if(doAbsolute) pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt);

  double pDFrac2    = iU2MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*pDFrac2;
  double pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*pDFrac2;
  if(doAbsolute) pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt);
  if(doAbsolute) pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt);
  //double pDMean1    = pDFrac1;
  //double pDMean2    = pDFrac2;
 
  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMU2       = 0; 
  double pMFrac1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt)*pMFrac1;
  double pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt)*pMFrac1;
  if(doAbsolute) pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt);
  if(doAbsolute) pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt);

  double pMFrac2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt)*pMFrac2;
  double pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt)*pMFrac2;
  if(doAbsolute) pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt);
  if(doAbsolute) pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt);

  //double pMMean1    = pMFrac1;
  //double pMMean2    = pMFrac2;
  //Uncertainty propagation
  pDFrac1     = (pDFrac1-pDSigma1_2)/(pDSigma1_1-pDSigma1_2);
  pDFrac2     = (pDFrac2-pDSigma2_2)/(pDSigma2_1-pDSigma2_2);
  pMFrac1     = (pMFrac1-pMSigma1_2)/(pMSigma1_1-pMSigma1_2);
  pMFrac2     = (pMFrac2-pMSigma2_2)/(pMSigma2_1-pMSigma2_2);

  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  /////
  
  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-pDefU1;
  double pU2Diff  = pU2;

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);
  double pTU1Diff        = pU1Diff;
  
  //  cout << "------------"<< endl;
  //  cout << "diGAUSS "<< endl;
  //  cout << "  pU1ValM " << pU1ValM  << " pU1ValD " << pU1ValD  <<endl;
  //  cout << "  pU2ValM " << pU2ValM  << " pU2ValD " << pU2ValD  <<endl;

  double pU1ValM = 0;
  double pU2ValM = 0 ;
  double pU1ValD = 0 ;
  double pU2ValD = 0;

  if(doSingleGauss) {

    pU1ValM         = diGausPVal(fabs(pU1Diff),1,iU1MSZMCFit ->Eval(iGenPt)*lRescale,0); // when is singleGauss pMFrac1=1 pMSigma1_1=fullRMS pMSigma1_2=0 
    pU2ValM         = diGausPVal(fabs(pU2Diff),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
    
    if(!doOnlyU1 && !doOnlyU2) {
      pU1ValD         = oneGausPInverse(pU1ValM  ,1,iU1MSZDatFit->Eval(iGenPt)*lRescale,0);
      pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0);
    }

    if(doOnlyU1 && !doOnlyU2) {
      pU1ValD         = oneGausPInverse(pU1ValM  ,1,iU1MSZDatFit->Eval(iGenPt)*lRescale,0);
      pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZMCFit->Eval(iGenPt)*lRescale,0); // since doingOnlyU1, use MC for U2
    }

    if(!doOnlyU1 && doOnlyU2) {
      pU1ValD         = oneGausPInverse(pU1ValM  ,1,iU1MSZMCFit->Eval(iGenPt)*lRescale,0); // since doingOnlyU2, use MC for U1 
      pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0); 
    }

  } else {

    pU1ValM         = diGausPVal(fabs(pU1Diff),pMFrac1,pMSigma1_1,pMSigma1_2); // when is singleGauss pMFrac1=1 pMSigma1_1=fullRMS pMSigma1_2=0 
    pU2ValM         = diGausPVal(fabs(pU2Diff),pMFrac2,pMSigma2_1,pMSigma2_2);

    if(!doOnlyU1 && !doOnlyU2) {
      pU1ValD         = diGausPInverse(pU1ValM  ,pDFrac1,pDSigma1_1,pDSigma1_2);
      pU2ValD         = diGausPInverse(pU2ValM  ,pDFrac2,pDSigma2_1,pDSigma2_2);
    }

    if(doOnlyU1 && !doOnlyU2) {
      pU1ValD         = diGausPInverse(pU1ValM  ,pDFrac1,pDSigma1_1,pDSigma1_2);
      pU2ValD         = diGausPInverse(pU2ValM  ,pMFrac2,pMSigma2_1,pMSigma2_2);
    }

    if(!doOnlyU1 && doOnlyU2) {
      pU1ValD         = diGausPInverse(pU1ValM  ,pMFrac1,pMSigma1_1,pMSigma1_2);
      pU2ValD         = diGausPInverse(pU2ValM  ,pDFrac2,pDSigma2_1,pDSigma2_2);
    }

  }

  //  cout << "oneGAUSS " << endl;
  //  cout << " iU1MSZMCFit ->Eval(iGenPt) " << iU1MSZMCFit ->Eval(iGenPt) << " U2MSZMCFit ->Eval(iGenPt) " << iU2MSZMCFit ->Eval(iGenPt) << endl;
  //  cout << "  pU1ValM " << pU1ValM_one  << " pU1ValD " << pU1ValD_one  << endl;
  //  cout << "  pU2ValM " << pU2ValM_one  << " pU2ValD " << pU2ValD_one  << endl;

  pU1ValD*=p1Charge;
  pU2ValD*=p2Charge;

  if(dodebug)   cout << " ==> Zpt: " << iGenPt << " pDef1: " << pDefU1 << " pDU1: " << pDU1 << " pMU1: " << pMU1 << endl;

  pDefU1 *= (pDU1/pMU1);

  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  //  iU1   = pU1; 
  //  iU2   = pU2;

  return;

}


///$$$$$$$$$$$$
///$$$$
///$$$$  UTILITY to simple calculation
///$$$$ 
///$$$$$$$$$$$$ 

float getPhiCorrMET(bool iType) { 

  float metx = fMet * cos( fMPhi );
  float mety = fMet * sin( fMPhi );
  
  float shiftx = 0.;
  float shifty = 0.;
  
  shiftx = (!fData) ? (-0.02245 - 0.0498*fNPV)
    : (-0.1676 + 0.2509*fNPV);
  shifty = (!fData) ? (0.1158 - 0.3201*fNPV)
    : (-0.2244 - 0.4009*fNPV);
  
  metx -= shiftx;
  mety -= shifty;

  if(!iType) return atan2( mety , metx ); // metPhi
  if(iType) return sqrt( metx*metx + mety*mety ); // met 

}

float getNeuMET(bool iType) {

  float metx = fMet * cos ( fMPhi ) - ftkMet * cos ( ftkMPhi );
  float mety = fMet * sin ( fMPhi ) - ftkMet * sin ( ftkMPhi );

  float met=sqrt( metx*metx + mety*mety );
  float metphi = atan2( mety , metx );

  if(!iType && met>0) return metphi;
  if(iType && met>0) return met;

  return 0.;
}

///$$$$$$$$$$$$
///$$$$
///$$$$  UTILITY to read the NTUPLE
///$$$$
///$$$$$$$$$$$$
                                                                                                                                                                               

void load(TTree *iTree, int type) { 

  fWeight = 1;
  iTree->ResetBranchAddresses();
  //iTree->SetBranchAddress("IS2011A",&fIS2011A);

  // MARIA this is not there only the leading jet infos
  //  iTree->SetBranchAddress("nbtag" ,&fNBTag); // this is not commented
  //iTree->SetBranchAddress("mjj"   ,&fMJJ);

  //iTree->SetBranchAddress("jdeta" ,&fDEta);
  //iTree->SetBranchAddress("id1_l" ,&fId1);
  //iTree->SetBranchAddress("id2_l" ,&fId2);
  //iTree->SetBranchAddress("phi_z",&fVPhi);
  //iTree->SetBranchAddress("pt_z" ,&fVPt);
  //iTree->SetBranchAddress("phi1_l",&fGPhi1);
  //iTree->SetBranchAddress("phi2_l",&fGPhi2);
  //iTree->SetBranchAddress("eta1_l",&fGEta1);
  //iTree->SetBranchAddress("eta2_l",&fGEta2);

  /*
    iTree->SetBranchAddress("eta_2" ,&fEta2);
    iTree->SetBranchAddress("phi_2" ,&fPhi2);
    iTree->SetBranchAddress("taujetpt" ,&fPt2)
    iTree->SetBranchAddress("pt_vis"  ,&fPt1);
    iTree->SetBranchAddress("phi_vis" ,&fPhi1);
  */

  /*
  // commented for now since is not good for later
  iTree->SetBranchAddress("MuNeg_eta" ,&fEta2);
  iTree->SetBranchAddress("MuNeg_phi" ,&fPhi2);
  iTree->SetBranchAddress("MuNeg_pt" ,&fPt2); 
  */

  /* 
  iTree->SetBranchAddress("MuPos_pt"  ,&fPt1); 
  iTree->SetBranchAddress("MuPos_phi" ,&fPhi1); // this maybe for the W
  */

  /*
  iTree->SetBranchAddress("Z_pt"  ,&fPt1); // this is for the Z; this is not working because of renaming
  iTree->SetBranchAddress("Z_phi" ,&fPhi1);
  */

  iTree->SetBranchAddress("pt_vis"  ,&fPt1);
  iTree->SetBranchAddress("phi_vis" ,&fPhi1);

  iTree->SetBranchAddress("run" ,&fRun);
  iTree->SetBranchAddress("lumi" ,&fLumi);
  iTree->SetBranchAddress("evt" ,&fEvent);

  //iTree->SetBranchAddress("Weight",&fWeight); // this is set to 1 fot now

  //  iTree->SetBranchAddress("Jet_leading_pt"  ,&fJPt1);
  //  iTree->SetBranchAddress("jetpt1"  ,&fJPt1);
  iTree->SetBranchAddress("nvtx"   ,&fNPV);
  //  iTree->SetBranchAddress("npv"   ,&fNPV);

  if((!doPosW) && (!doNegW)) {
    if(!fData) iTree->SetBranchAddress("ZGen_pt" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("ZGen_phi",&fZPhi);
    if(!fData) iTree->SetBranchAddress("ZGen_rap",&fZRap);
    //  if(!fData) iTree->SetBranchAddress("Z_pt" ,&fZPt);
    //  if(!fData) iTree->SetBranchAddress("Z_phi",&fZPhi);
    if(fData)  iTree->SetBranchAddress("Z_pt" ,&fZPt);
    if(fData)  iTree->SetBranchAddress("Z_phi",&fZPhi);
    if(fData)  iTree->SetBranchAddress("Z_rap",&fZRap);
  }

  if((doPosW) || (doNegW)) {
    if(!fData) iTree->SetBranchAddress("WGen_pt" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("WGen_phi",&fZPhi);
    if(!fData) iTree->SetBranchAddress("WGen_rap",&fZRap);

    //    cout << " pt W " << fZPt  << " phi W " << fZPhi << endl;
    //  if(!fData) iTree->SetBranchAddress("Z_pt" ,&fZPt);
    //  if(!fData) iTree->SetBranchAddress("Z_phi",&fZPhi);
    if(fData)  iTree->SetBranchAddress("W_pt" ,&fZPt);
    if(fData)  iTree->SetBranchAddress("W_phi",&fZPhi);
  }

  /*
    if(!fData) iTree->SetBranchAddress("pt_z" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("phi_z",&fZPhi);
    if(fData)  iTree->SetBranchAddress("pt_z" ,&fZPt);
    if(fData)  iTree->SetBranchAddress("phi_z",&fZPhi);
  */

  // MARIA this is not there only the leading jet infos
  iTree->SetBranchAddress("njets",&fNJet);

  if(type==0) {
    //iTree->SetBranchAddress("u1x_mva"    ,&fU1);
    //iTree->SetBranchAddress("u2x_mva"    ,&fU2);
    //iTree->SetBranchAddress("metx_mva"   ,&fMet);
    //iTree->SetBranchAddress("metphix_mva",&fMPhi);
    iTree->SetBranchAddress("mvamet"   ,&fMet);
    iTree->SetBranchAddress("mvametphi",&fMPhi);
  } else if(type==1) {
    iTree->SetBranchAddress("trku1" ,&fU1);
    iTree->SetBranchAddress("trku2" ,&fU2);
    iTree->SetBranchAddress("trkmet",&fMet); 
  } else if(type==2) {
    //iTree->SetBranchAddress("trku1" ,&fTKU1);
    //iTree->SetBranchAddress("trku2" ,&fTKU2);
    // MARIA: I'm using those now
    iTree->SetBranchAddress("u1"  ,&fPFU1);
    iTree->SetBranchAddress("u2"  ,&fPFU2);
    //    iTree->SetBranchAddress("met",&fMet);
    //    iTree->SetBranchAddress("metphi",&fMPhi);

    iTree->SetBranchAddress("tkmet",&ftkMet);
    iTree->SetBranchAddress("tkmet_phi",&ftkMPhi);

    if(donoPU){
      iTree->SetBranchAddress("nopumet",&fMet);
      iTree->SetBranchAddress("nopumet_phi",&fMPhi);
    } else {
      iTree->SetBranchAddress("pfmet",&fMet);
      iTree->SetBranchAddress("pfmet_phi",&fMPhi);
    }

  }


  /// these are needed for the selection

  /// Z selection 
  if((!doPosW) && (!doNegW)) {

    iTree->SetBranchAddress("Z_mass" ,&fZmass);

    iTree->SetBranchAddress("MuPos_eta" ,&fMuPos_eta);
    iTree->SetBranchAddress("MuPos_pt" ,&fMuPos_pt);
    iTree->SetBranchAddress("MuPos_charge" ,&fMuPos_charge);
    iTree->SetBranchAddress("MuPosRelIso" ,&fMuPosReliso);
    iTree->SetBranchAddress("MuPos_dxy" ,&fMuPos_dxy);
      
    iTree->SetBranchAddress("MuNeg_eta" ,&fMuNeg_eta);
    iTree->SetBranchAddress("MuNeg_pt" ,&fMuNeg_pt);
    iTree->SetBranchAddress("MuNeg_charge" ,&fMuNeg_charge);
    iTree->SetBranchAddress("MuNegRelIso" ,&fMuNegReliso);
    iTree->SetBranchAddress("MuNeg_dxy" ,&fMuNeg_dxy);
    
    iTree->SetBranchAddress("MuPosTrg" ,&fMuPosTrg);
    iTree->SetBranchAddress("MuNegTrg" ,&fMuNegTrg);
    iTree->SetBranchAddress("MuPosIsTightAndIso" ,&fMuPosIsTightAndIso);
    iTree->SetBranchAddress("MuNegIsTightAndIso" ,&fMuNegIsTightAndIso);

    iTree->SetBranchAddress("MuPosIsTight" ,&fMuPosIsTight);
    iTree->SetBranchAddress("MuNegIsTight" ,&fMuNegIsTight);

  }

  /// W selection 
  if((doPosW) || (doNegW)) {
    iTree->SetBranchAddress("Mu_eta" ,&fMuPos_eta);
    iTree->SetBranchAddress("Mu_pt" ,&fMuPos_pt);
    iTree->SetBranchAddress("Mu_phi" ,&fMuPos_phi);
    iTree->SetBranchAddress("Mu_charge" ,&fMuPos_charge);
    iTree->SetBranchAddress("MuRelIso" ,&fMuPosReliso);
    iTree->SetBranchAddress("Mu_dxy" ,&fMuPos_dxy);
    iTree->SetBranchAddress("Mu_dz" ,&fMuPos_dz);
    iTree->SetBranchAddress("MuIsTight" ,&fMuPosIsTight);

    // fake muon from the W
    iTree->SetBranchAddress("NuGen_eta" ,&fMuNeg_eta);
    iTree->SetBranchAddress("NuGen_pt"  ,&fMuNeg_pt);
    iTree->SetBranchAddress("NuGen_phi" ,&fMuNeg_phi);
    
    //    fPt1=fMuPos_pt;
    //    fPhi1=fMuPos_phi;

  }

  iTree->SetBranchAddress("evtHasGoodVtx" ,&fevtHasGoodVtx);
  iTree->SetBranchAddress("evtHasTrg" ,&fevtHasTrg);

  iTree->SetBranchAddress("parton1_pdgId" ,&fpdgId1);
  iTree->SetBranchAddress("parton2_pdgId" ,&fpdgId2);
  
}

void calculateU1U2(double &iPar, bool iType) {

  double lUX  = fMet*cos(fMPhi) + fPt1*cos(fPhi1);
  double lUY  = fMet*sin(fMPhi) + fPt1*sin(fPhi1);
  double lU   = sqrt(lUX*lUX+lUY*lUY);

  // rotate of -180 the X and Y component
  double lCos = - (lUX*cos(fZPhi) + lUY*sin(fZPhi))/lU;
  double lSin =   (lUX*sin(fZPhi) - lUY*cos(fZPhi))/lU;
  fU1 = lU*lCos;
  fU2 = lU*lSin;
  if(iType)  iPar = fU1;
  if(!iType) iPar = fU2;

  /*
  cout << "----------------" << endl;
  cout << "fU1: recomputed " << fU1 << " stored " << fPFU1 << endl;
  cout << "fU2: recomputed " << fU2 << " stored " << fPFU2 << endl;
  */

}

bool checkOdd() {

  if(fData) return true;

  if(!fData && fEvent % 2) {
    // check the odd events for the closure test
    return true;
  }

  return false;

}

bool runSelection() {

  //  cout << "inside Z selction " << etaMuonSel << endl;  
  //   double etaMuon=0.6;  
  
  double etaMuon=etaMuonSel;
  double muonPt=30*(90./80);
  
  //   cout << "etaMuon "<< etaMuon << endl;
  
  ////////                      
  
  bool LeadingPos=(TMath::Abs(fMuPos_eta)<etaMuon && fMuPos_pt>muonPt && fMuPosTrg
		   && TMath::Abs(fMuNeg_eta)<2.4 && fMuNeg_pt>10
		   && fMuPosIsTight
		   && fMuPosReliso<0.12
		   && fabs(fMuPos_dxy)<0.02
		   && fabs(fMuPos_dz)<0.1
		   && fMuNegIsTight
		   //              && fMuNegIsTightAndIso // relax iso on the second muon
		   //              && fMuNegReliso<0.12   // relax iso on the second muon
		   && fabs(fMuNeg_dxy)<0.02
                   && fabs(fMuNeg_dz)<0.1
		   );
  
  bool LeadingNeg=(TMath::Abs(fMuNeg_eta)<etaMuon && fMuNeg_pt>muonPt && fMuNegTrg
		   && TMath::Abs(fMuPos_eta)<2.4 && fMuPos_pt>10
		   //     && fMuNegIsTightAndIso
		   && fMuNegIsTight
		   && fMuNegReliso<0.12
		   && fabs(fMuNeg_dxy)<0.02
		   && fabs(fMuNeg_dz)<0.1
		   && fMuPosIsTight
		   //     && fMuPosIsTightAndIso // relax iso on the second muon
		   //              && fMuPosReliso<0.12   // relax iso on the second muon 
		   && fabs(fMuPos_dxy)<0.02
		   && fabs(fMuPos_dz)<0.1
		   );
  double metLikeX  = fMet*cos(fMPhi)+fMuPos_pt*cos(fMuPos_phi);
  double metLikeY  = fMet*sin(fMPhi)+fMuPos_pt*cos(fMuPos_phi);
  
  if(LeadingNeg) metLikeX  = fMet*cos(fMPhi)+fMuNeg_pt*cos(fMuNeg_phi);
  if(LeadingNeg) metLikeY  = fMet*sin(fMPhi)+fMuNeg_pt*cos(fMuNeg_phi);
  
  double metLike=sqrt(metLikeX*metLikeX+metLikeY*metLikeY);

  if( fevtHasGoodVtx
      && fevtHasTrg
      //      && fZmass>50
      //      && fZmass>70
      //      && fZmass<110
      && fZmass>81
      && fZmass<101
      && (fMuNeg_charge != fMuPos_charge)
      && (LeadingPos || LeadingNeg)
      && metLike>25
      )
    {
      //    cout << "GOOD selection" << endl;
      return true;
    }
  
  //  cout << "BAD selection" << endl;

   return false;

 }

 bool runWSelection(bool doPos) {

   bool doNeutrinoBias=false;

   //   cout << "inside W selction " << etaMuonSel << endl;             

   //   double etaMuon=1.4;
   double etaMuon=etaMuonSel;
   double muonPt=30; 
   //   double muonPt=27;

   ///   float recoil=sqrt(fU1*fU1 + fU2*fU2);

   if( !fevtHasGoodVtx ) return false;

   /*
   */

   if(doNeutrinoBias) {
     // need for the GenStudies
     if(doPos && (fMuPos_charge<0 || fMuNeg_charge<0)) return false;
     if((!doPos) && (fMuPos_charge>0 || fMuNeg_charge>0)) return false;
   } else {
     // need for the recoStudies
     if( !fevtHasTrg ) return false; 
     if(doPos && fMuPos_charge<0 ) return false;
     if((!doPos) && fMuPos_charge>0 ) return false;
   }

   /// add met and recoil  
                                                                                                                                                                   
   if( fMet <= 25 ) return false;

   // do not cut on the recoil otherwise we bias the fits  
   //  if( recoil >= 20 ) return false; 

   /// pick separately the positive and negative 

   bool LeadingPos=(fMuPos_pt>muonPt && TMath::Abs(fMuPos_eta)<etaMuon);
   bool LeadingNeg=false;
  
   if(doNeutrinoBias) {
     LeadingPos=(TMath::Abs(fMuPos_eta)<etaMuon && fMuPos_pt>muonPt 
		 && TMath::Abs(fMuNeg_eta)<2.4 && fMuNeg_pt>10);
     LeadingNeg=(TMath::Abs(fMuNeg_eta)<etaMuon && fMuNeg_pt>muonPt 
		 && TMath::Abs(fMuPos_eta)<2.4 && fMuPos_pt>10);
   }

   /// a good muon        
   if((LeadingPos || LeadingNeg)
      && fMuPosIsTight
      && fMuPosReliso<0.12
      && fabs(fMuPos_dxy)<0.02
   //      && fabs(fMuPos_dz)<0.1 this is missing in the in ntuples_2013_09_14/
   ) return true;

   return false;

 }         


bool passMatching() { 

  double lDEta0  = fGEta1-fEta2;
  double lDPhi0 = fabs(fGPhi1-fPhi2); if(lDPhi0 > 2.*TMath::Pi()-lDPhi0) lDPhi0 = 2.*TMath::Pi()-lDPhi0;
  double lDR0 = sqrt(lDEta0*lDEta0+lDPhi0*lDPhi0);
  double lDEta1  = fGEta2-fEta2;
  double lDPhi1 = fabs(fGPhi2-fPhi2); if(lDPhi1 > 2.*TMath::Pi()-lDPhi1) lDPhi1 = 2.*TMath::Pi()-lDPhi1;
  double lDR1 = sqrt(lDEta1*lDEta1+lDPhi1*lDPhi1);
  return (lDR0 < 0.3 || lDR1 < 0.3);

}

double getCorError2(double iVal,TF1 *iFit) { 

  double lE = sqrt(iFit->GetParError(0))  + iVal*sqrt(iFit->GetParError(2));
  if(fabs(iFit->GetParError(4)) > 0) lE += iVal*iVal*sqrt(iFit->GetParError(4));
  return lE*lE;

}

bool checkPDF(int typeBoson, bool doPlot) { 

  // THOSE are for the Z
  if(typeBoson==23) {
    if((fpdgId1+fpdgId2)==0) {
      if( pType==1 && abs(fpdgId1)==1 && abs(fpdgId2)==1 ) return true; // cout << " d dbar " << endl;
      if( pType==2 && abs(fpdgId1)==2 && abs(fpdgId2)==2 ) return true; // cout << " u ubar " << endl;
      if( pType==3 && abs(fpdgId1)==3 && abs(fpdgId2)==3 ) return true; // cout << " s sbar " << endl;
	if( pType==4 && abs(fpdgId1)==4 && abs(fpdgId2)==4 ) return true; // cout << " c cbar " << endl;
	if( pType==5 && abs(fpdgId1)==5 && abs(fpdgId2)==5 ) return true; // cout << " b bbar " << endl;
    } else if( pType==0 && (abs(fpdgId1)==0 || abs(fpdgId2)==0) ) {
      return true; //cout << " gluon + X  " << endl;
    } else {
      //      cout << " UNKNOWN "<< endl;
    }
  }
  
  if(typeBoson==24) {
    // THOSE are for the W+
    if( pType==1 && ((fpdgId1==2 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==2))) return true; //cout << " u dbar " << endl;
    if( pType==2 && ((fpdgId1==2 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==2))) return true; //cout << " u sbar " << endl;
    if( pType==3 && ((fpdgId1==2 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==2))) return true; //cout << " u bbar " << endl;
    if( pType==4 && ((fpdgId1==4 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==4))) return true; //cout << " c dbar " << endl;
    if( pType==5 && ((fpdgId1==4 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==4))) return true; //cout << " c sbar " << endl;
    if( pType==6 && ((fpdgId1==4 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==4))) return true; //cout << " c bbar " << endl;
    if( pType==0 && (fpdgId1==0 || fpdgId2==0) ) return true; //cout << "gluon + X "<< endl;
  }
  
  if(typeBoson==(-24)) {
    // THOSE are for the W-
    if( pType==1 && ((fpdgId1==1 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==1))) return true; //cout << " d ubar " << endl;
    if( pType==2 && ((fpdgId1==1 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==1))) return true; //cout << " d cbar " << endl;
    if( pType==3 && ((fpdgId1==3 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==3))) return true; //cout << " s ubar " << endl;
    if( pType==4 && ((fpdgId1==3 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==3))) return true; //cout << " s cbar " << endl;
    if( pType==5 && ((fpdgId1==5 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==5))) return true; //cout << " b ubar " << endl;
    if( pType==6 && ((fpdgId1==5 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==5))) return true; //cout << " b cbar " << endl;
    if( pType==0 && (fpdgId1==0 || fpdgId2==0)) return true; //cout << "gluon + X "<< endl;
  }
    
  return false;
  
}
  
bool calculateBosonRap(int step) {

  if( step == 1 && fabs(fZRap)<1 ) return true;
  if( step == 2 && fabs(fZRap)>1 && fabs(fZRap)<=2 ) return true;
  if( step == 3 && fabs(fZRap)>2) return true;

  if( step == 125 && fabs(fZRap)>1 && fabs(fZRap)<=1.25 ) return true;
  if( step == 150 && fabs(fZRap)>1.25 && fabs(fZRap)<=1.5 ) return true;
  if( step == 175 && fabs(fZRap)>1.50 && fabs(fZRap)<=1.75 ) return true;
  if( step == 200 && fabs(fZRap)>1.75 && fabs(fZRap)<=2.0 ) return true;
  if( step == 201 && fabs(fZRap)>2.0 ) return true;

  return false;
	  
}


bool passId(int iId) { 

  //  if(fId == 0) return true;
  //if(iId  < 0) {if(!(fIS2011A == 0))                                {return false;}}
  //if(iId  > 0) {if(!(fIS2011A == 1))                                {return false;}}

  double lId = fabs(iId);

  if(doBinned) {
    if(lId == 1) {if(fNPV > 5              ) {return false; } {return true;}}
    //  if(lId == 1) {if(fNPV < 3 || fNPV > 5              ) {return false; } {return true;}}
    if(lId == 2) {if(fNPV < 6 || fNPV  > 10) {return false; } {return true;}}
    if(lId == 3) {if(fNPV < 11             ) {return false; } {return true;}}
    
    if(lId == 0) {if(fNPV > 12             ) {return false; } {return true;}}

  } else {

    if(lId == 1) {if(fNPV >1              ) {return false; } {return true;}} // include the vtx==0
    if(lId == 2) {if(fNPV !=2              ) {return false; } {return true;}}
    if(lId == 3) {if(fNPV !=3              ) {return false; } {return true;}}
    if(lId == 4) {if(fNPV !=4              ) {return false; } {return true;}}
    if(lId == 5) {if(fNPV !=5              ) {return false; } {return true;}}
    if(lId == 6) {if(fNPV !=6              ) {return false; } {return true;}}
    if(lId == 7) {if(fNPV !=7              ) {return false; } {return true;}}
    if(lId == 8) {if(fNPV !=8              ) {return false; } {return true;}}
    if(lId == 9) {if(fNPV !=9              ) {return false; } {return true;}}
    if(lId == 10) {if(fNPV !=10              ) {return false; } {return true;}}
    if(lId == 11) {if(fNPV !=11              ) {return false; } {return true;}}
    if(lId == 12) {if(fNPV !=12              ) {return false; } {return true;}}
    if(lId == 13) {if(fNPV !=13              ) {return false; } {return true;}}
    if(lId == 14) {if(fNPV !=14              ) {return false; } {return true;}}
    if(lId == 15) {if(fNPV !=15              ) {return false; } {return true;}}
    if(lId == 16) {if(fNPV !=16              ) {return false; } {return true;}}
    if(lId == 17) {if(fNPV !=17              ) {return false; } {return true;}}
    if(lId == 18) {if(fNPV !=18              ) {return false; } {return true;}}
    if(lId == 19) {if(fNPV !=19              ) {return false; } {return true;}}
    ///    if(lId >= 20) {if(fNPV !=20              ) {return false; } {return true;}} // include the vtx>20                                              
    if(lId >= 20) {if(fNPV <20              ) {return false; } {return true;}} // include the vtx>20         

  }

  //  cout << "NVertex " << fNPV << endl;
  
  return true;
  
}

double getError2(double iVal,TF1 *iFit) { 
  double lE2 = iFit->GetParError(0) + iVal*iFit->GetParError(1) + iVal*iVal*iFit->GetParError(2);
  if(iFit->GetParError(3)!=0) lE2 += iVal*iVal*iVal*     iFit->GetParError(3);
  if(iFit->GetParError(4)!=0) lE2 += iVal*iVal*iVal*iVal*iFit->GetParError(4);
  if(iFit->GetParError(5)!=0) lE2 += iVal*iVal*iVal*iVal*iVal*iFit->GetParError(5);
  //  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) == 0) lE2 += iVal*iVal*               iFit->GetParError(5);
  //  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) != 0) lE2 += iVal*iVal*iVal*iVal*iVal*iFit->GetParError(5);
  if(iFit->GetParError(6)!=0) lE2 += iVal*iVal*iVal*iVal*iVal*iVal*iFit->GetParError(6);
  return lE2;
}

double getError(double iVal,TF1 *iWFit,TF1 *iZDatFit,TF1 *iZMCFit,bool iRescale=true) {
  double lEW2  = getError2(iVal,iWFit);
  if(!iRescale) return sqrt(lEW2);
  double lEZD2 = getError2(iVal,iZDatFit);
  double lEZM2 = getError2(iVal,iZMCFit);
  double lZDat = iZDatFit->Eval(iVal);
  double lZMC  = iZMCFit->Eval(iVal);
  double lWMC  = iWFit->Eval(iVal);
  double lR    = lZDat/lZMC;
  double lER   = lR*lR/lZDat/lZDat*lEZD2 + lR*lR/lZMC/lZMC*lEZM2;
  double lVal  = lR*lR*lEW2 + lWMC*lWMC*lER;
  return sqrt(lVal);//+(iZMCFit->Eval(iVal)-iWFit->Eval(iVal);
}

void computeFitErrors(TF1 *iFit,TFitResultPtr &iFPtr,TF1 *iTrueFit,bool iPol0=false) {

  //   bool lPol0 = iPol0;
   bool lPol2 = (iTrueFit->GetParError(2) != 0);
   bool lPol3 = (iTrueFit->GetParError(3) != 0);

   TMatrixDSym lCov = iFPtr->GetCovarianceMatrix();
   double lE0 = (lCov)(0,0);
   double lE1 = (lCov)(0,1) + (lCov)(1,0);
   double lE2 = (lCov)(1,1);
   double lE3 = 0; if(lPol2) lE3 = 2*(lCov)(1,2);
   double lE4 = 0; if(lPol2) lE4 =  (lCov)(2,2);   //This scheme preserves the diagaonals 
                                                                                                    
   if(lPol2 || lPol3) lE2 += 2*(lCov)(0,2);
   // MARIA: this is commented       
   /*if(lPol2) lE5 = 2*(lCov)(2,0); */

   double lE5 = 0;
   double lE6 = 0;
   if(lPol3) {
     // MARIA commented: this is needed also for the pol2      
     /*lE2 += 2*(lCov)(0,2); */
     lE3 += 2*(lCov)(0,3);
     lE4 += 2*(lCov)(1,3);
     lE5  = 2*(lCov)(2,3);
     lE6  = (lCov)(3,3);
   }

   // MARIA: this is commented since iPol0 is just true/false for iRMS 
   //   if(lPol0) {lE0 = iTrueFit->GetParError(0)*iTrueFit->GetParError(0); lE1 = 0; lE2 = 0; lE3=0;} 
                                                                                        
   iFit->SetParameter(0,iTrueFit->GetParameter(0)); iFit->SetParError(0,lE0);
   iFit->SetParameter(1,iTrueFit->GetParameter(1)); iFit->SetParError(1,lE1);
   iFit->SetParameter(2,iTrueFit->GetParameter(2)); iFit->SetParError(2,lE2);
   iFit->SetParameter(3,iTrueFit->GetParameter(3)); iFit->SetParError(3,lE3);
   iFit->SetParameter(4,iTrueFit->GetParameter(4)); iFit->SetParError(4,lE4);
   iFit->SetParError(5,lE5);
   iFit->SetParError(6,lE6);

   cout << "===> Errors Diagonalized and Stored: lE0=" << lE0 << " lE1="<< lE1 << " lE2="<< lE2 << " lE3="<< lE3 << " lE4="<< lE4 << " lE5="<< lE5 << " lE6="<< lE6 << endl;

   // 
   //   cout << "===> THIS TRY TO DIAGONALIZE THE MATRIX "<< endl; 
   //   const TVectorD diag = TMatrixDDiag_const(*lCov); 
   //   lCov->NormbyDiag(diag); 
   //   lCov->GetEigenValues()            
   //   lCov->Print();

   /*
   cout << "===> BELOW THE COVARIANCE MATRIX "<< endl;  
   lCov.Print();           

   TMatrixDSymEigen me(lCov); 
   TVectorD eigenval = me.GetEigenValues();   
   TMatrixD eigenvec = me.GetEigenVectors();  
   cout << "===> BELOW ARE THE EIGEN-VAL "<< endl;  
   eigenval.Print();    
   cout << "===> BELOW ARE THE EIGEN-VEC "<< endl; 
   eigenvec.Print();                 
   */


}


void drawErrorBands(TF1 *iFit, float iXMax) { 
  int lN = int(iXMax*5.)*2;
  TGraph *lG0 = new TGraph(lN-1);
  TGraph *lG1 = new TGraph(lN-1);
  for(int i0 = 0; i0 < lN/2; i0++) {
    double pVal = i0*0.2;
    lG0->SetPoint(i0,pVal,iFit->Eval(pVal)+sqrt(getError2(pVal,iFit)));      //Small Yellow Band
    lG1->SetPoint(i0,pVal,iFit->Eval(pVal)+sqrt(getCorError2(pVal,iFit)));   //Large Redfi Band
  }
  for(int i0 = 0; i0 < lN/2; i0++) {
    double pVal = lN/2*0.2-(i0+1)*0.2;
    lG0->SetPoint(lN/2+i0,pVal,iFit->Eval(pVal)-sqrt(getError2(pVal,iFit)));
    lG1->SetPoint(lN/2+i0,pVal,iFit->Eval(pVal)-sqrt(getCorError2(pVal,iFit)));
  }
  lG0->SetFillColor(kYellow);
  //  lG0->SetFillColor(kGreen);
  lG1->SetFillColor(kGreen);
  lG1->Draw("F");
  lG0->Draw("F");
}
double fCorrMax = 70;

TGraphErrors* makeGraph(int iNBins,int iId,std::vector<float> &iXVals,std::vector<float> &iY0Vals,std::vector<float> &iY1Vals,
			std::vector<float> &iXEVals,std::vector<float> &iYEVals) { 
  std::vector<float> iPar0;    std::vector<float> iPar1;
  std::vector<float> iEPar0;   std::vector<float> iEPar1;
  for(unsigned int i0 = 0; i0 < iXVals.size(); i0++) { 
    if(iXVals[i0] < iId*(int(fCorrMax/iNBins))) continue;
    //if(iId != iNBins-1 && 
    if(iXVals[i0] > (iId+1)*(int(fCorrMax/iNBins))) continue;
    iPar0.push_back(iY0Vals[i0]); 
    iPar1.push_back(iY1Vals[i0]); 
    iEPar0.push_back(1);
    iEPar1.push_back(iYEVals[i0]);
  }
  TGraphErrors *lF0 = new TGraphErrors(iPar1.size(),&iPar0[0],&iPar1[0],&iEPar0[0],&iEPar1[0]);
  return lF0;
}

TH2F* makeHist(int iNBins,int iId,std::vector<float> &iXVals,std::vector<float> &iY0Vals,std::vector<float> &iY1Vals) { 
  TH2F *lH0 = new TH2F("X","X",30,-5,0,30,-5,0);
  for(unsigned int i0 = 0; i0 < iXVals.size(); i0++) { 
    if(iXVals[i0] < iId*(int(fCorrMax/iNBins))) continue;
    //if(iId != iNBins-1 && 
    if(iXVals[i0] > (iId+1)*(int(fCorrMax/iNBins))) continue;
    lH0->Fill(iY0Vals[i0],iY1Vals[i0]);
  }
  return lH0;
}

double angle(double lMX,double lMY) {
  if(lMX > 0) {return atan(lMY/lMX);} 
  return (fabs(lMY)/lMY)*3.14159265 + atan(lMY/lMX);
}

/*
//void fitCorr(bool iRMS,TTree *iTree,TCanvas *iC,float &lPar0,float &lPar1,
void fitCorr(bool iRMS,TTree *iTree,TCanvas *iC,double &lPar0,double &lPar1,
	     TF1   *iMeanFit0=0,TF1   *iMeanFit1=0,
	     TF1   *iRMSFit0 =0,TF1   *iRMSFit1 =0,
             TF1   *iCorrFit=0) { 
  iC->cd();
  fCorrMax = 70;
  const int iNBins = 7;
  TGraphErrors  **lG0  = new TGraphErrors*[iNBins]; double *lXCorr = new double[iNBins]; double *lCorr = new double[iNBins];
  std::vector<float> lXVals; std::vector<float> lY0Vals; std::vector<float> lY1Vals;
  std::vector<float> lXEVals; std::vector<float> lYEVals; 

  //  for(int i1 = 0; i1 <  iTree->GetEntries(); i1++) {
  for(int i1 = 0; i1 <  10000; i1++) {
    iTree->GetEntry(i1);
    if(!passId(fId)) continue;
    calculateU1U2(lPar0,true);
    calculateU1U2(lPar1,false);
    if(fZPt > fZPtMax) continue;
    if(fMet > fMetMax) continue;
    
    /// MARIA comment this for now
    //    if(fNJetSelect<2 && fNJet!=fNJetSelect && fNJetSelect != -1) continue;
    //    if(fNJetSelect==2 && fNJet<2) continue;
   
     double pVal0 = lPar0;
    double pVal1 = lPar1;
    if(iMeanFit0 != 0 && iRMSFit0 != 0 )   pVal0 = (lPar0-iMeanFit0->Eval(fZPt))/iRMSFit0->Eval(fZPt);
    if(iMeanFit1 != 0 && iRMSFit1 != 0 )   pVal1 = (lPar1-iMeanFit1->Eval(fZPt))/iRMSFit1->Eval(fZPt);
    //if( (pVal0 > 0 || pVal1 > 0)) continue;
    if(fWeight == 0) continue;
    double pWeight = fWeight;
    if(fData) pWeight = 1;
    //if(pVal1 > pVal0) continue;
    //if(iRMS && pVal0 > 0. && ((fPFU1 == lPar0) || (fTKU1 == lPar0))) continue;
    //if(iRMS && pVal1 > 0. && ((fPFU1 == lPar1) || (fTKU1 == lPar1))) continue;
    //double pWeight = fWeight;
    lXVals.push_back(fZPt); lXEVals.push_back(2.);
    (!iRMS) ? lY0Vals.push_back(pVal0) : lY0Vals.push_back(pVal0);
    (!iRMS) ? lY1Vals.push_back(pVal1) : lY1Vals.push_back(pVal1);
    lYEVals.push_back(1./sqrt(pWeight));
  }

  for(int i0 = 0; i0 < iNBins; i0++) lG0[i0] = makeGraph(iNBins,i0,lXVals,lY0Vals,lY1Vals,lXEVals,lYEVals);
  //for(int i0 = 2; i0 < 7; i0++) project(iNBins,i0,lXVals,lY0Vals,lY1Vals);
  for(int i0 = 0; i0 < iNBins; i0++) { 
     //lG0[i0]->Fit("pol1");
    //lG0[i0]->Draw("ap"); 
    lXCorr[i0] = i0*(fCorrMax/iNBins) + (fCorrMax/iNBins/2.);
    lG0[i0]->Fit("pol1","SR","EXO");
    lCorr [i0] = lG0[i0]->GetFunction("pol1")->GetParameter(1);
  }
  TGraph *lFinalCorr = new TGraph(iNBins,lXCorr,lCorr);
  std::string lType = "pol1";
  TF1 *lFit = new TF1("test",lType.c_str());
  TFitResultPtr  lFitPtr = lFinalCorr->Fit(lFit,"SR","",0,fCorrMax);
  computeFitErrors(iCorrFit,lFitPtr,lFit,iRMS);
  lFinalCorr->SetMarkerStyle(kFullCircle);
  lFinalCorr->Draw("ap");
  drawErrorBands(iCorrFit,100);
  lFinalCorr->Draw("p");
  iC->SaveAs("Crap.png");
  cin.get();
}
*/

/*
void makePdf(TTree *iTree1,double iPtMin,double iPtMax,RooDataHist *iData,RooHistPdf *iPdf,RooRealVar &iVar,TF1 *iMeanFit,float &lPar) { 
  float lPt   = 0; iTree1->SetBranchAddress("pt_z"   ,&lPt); 
  float lU1   = 0; iTree1->SetBranchAddress("u1x_mva",&lU1); 
  float lU2   = 0; iTree1->SetBranchAddress("u2x_mva",&lU2); 
  float lNJet = 0; iTree1->SetBranchAddress("njet"   ,&lNJet); 
  TH1F *lH0   = new TH1F("pH","pH",100,-100,100);
  for(int i1 = 0; i1 <  iTree1->GetEntries(); i1++) {
    iTree1->GetEntry(i1);
    //if(!passId(fId)) continue;
    if(lPt < iPtMin || lPt > iPtMax ) continue;

    // MARIA comment this for now
    //    if(fNJetSelect<2  && lNJet != fNJetSelect && fNJetSelect != -1) continue;
    //    if(fNJetSelect==2 && lNJet<2) continue;
    
    double pVal = 0;
    if(lPar == fU1) pVal = lU1;
    if(lPar == fU2) pVal = lU2;
    if(iMeanFit != 0)   pVal = (lPar - iMeanFit->Eval(fZPt));
    lH0->Fill(pVal);
  }
  iData = new RooDataHist("Data","Data",RooArgList(iVar),lH0);
  iPdf  = new RooHistPdf ("HPdf","HPdf",iVar,*iData,0);
  delete lH0;
}
*/

void fitGraph(TTree *iTree,TTree *iTree1, TCanvas *iC,
//	      float &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1* iMeanFit=0,
	      double &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1* iMeanFit=0,
	      bool iPol1 = false, 
	      bool iRMS  = false, int iType = 0) { 

  //RooFit Build a double Gaussian
  TRandom lRand(0xDEADBEEF);
  RooRealVar    lRWeight("weight","weight",0,10);
  ////  RooRealVar lRPt  ("pt","Z_{p_{T}}",10,1000);                                                                                                      
  RooRealVar lRPt  ("pt","Z_{p_{T}}",fZPtMin,fZPtMax);

  // original range for pull
  //  double minSigma=-5.; double maxSigma=5.;
  double minSigma=-10.; double maxSigma=10.;
  // guess for the GeV fit 
  if(doAbsolute) minSigma=-50.; maxSigma=50.;
  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,minSigma,maxSigma);
  if(lPar!=fU1) lRXVar.SetTitle("(U_{2}(Z_{p_{T}})-x_{i})/#sigma_{U2} (Z_{p_{T}})");

  // this for the 2D Fit (is a cubic function to take into account the correlation with the pt)
  ////// those are the 2D histograms     
  // ===> Those are the variable fitted  
  // lA1Sig,lB1Sig,lC1Sig,lA2Sig,lB2Sig,lC2Sig,
  // those are set constant (0) lD1Sig, lD2Sig, lR1Mean  
  
  RooRealVar    lA1Sig("a1sig","a1sig",0.8,0,1);
  RooRealVar    lB1Sig("b1sig","b1sig",0. , -0.1, 0.1);
  RooRealVar    lC1Sig("c1sig","c1sig",0. , -0.1, 0.1);  //lC1Sig.setConstant(kTRUE);
  RooRealVar    lD1Sig("d1sig","d1sig",0. , -0.1, 0.1);  lD1Sig.setConstant(kTRUE);
  //Phil suggestion to recover the tails enlarge the range 
  //   RooRealVar    lA2Sig("a2sig","a2sig",1.4,1,3); 

   RooRealVar    lA2Sig("a2sig","a2sig",1.4,1,5);  // bound to 10 sigma 
   RooRealVar    lB2Sig("b2sig","b2sig",0.0 ,-0.1, 0.1);
   RooRealVar    lC2Sig("c2sig","c2sig",0.0 ,-0.1, 0.1);  //lC2Sig.setConstant(kTRUE);
   RooRealVar    lD2Sig("d2sig","d2sig",0.0 ,-0.1, 0.1);  lD2Sig.setConstant(kTRUE);
   RooRealVar    lR1Mean("mean","mean",0,-10,10); lR1Mean.setConstant(kTRUE); 

   // ===> This is the model for the 2D          
   RooFormulaVar l1Sigma("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
   RooFormulaVar l2Sigma("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
   RooFormulaVar lFrac  ("frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(l1Sigma,l2Sigma));
   RooGaussian   lGaus1("gaus1","gaus1",lRXVar,lR1Mean,l1Sigma);
   RooGaussian   lGaus2("gaus2","gaus2",lRXVar,lR1Mean,l2Sigma);
   RooAddPdf     lGAdd("Add","Add",lGaus1,lGaus2,lFrac);

   ////// those are the 1D histograms     
   // ===> Those are the variable fitted 
   // Rfrac, Rmean, Rsigma1, Rsigma2 
   //   RooRealVar    lR1Sigma("Rsigma1","RSigma1",8. ,0,40); 
   //   RooRealVar    lR2Sigma("Rsigma2","RSigma2",12.,1,40); 
   //   RooRealVar    lR1Frac ("Rfrac"  ,"Rfrac"  ,0.5,0,1); 
                                                                                                                                 
   RooRealVar    lR1Sigma("Rsigma1","RSigma1",1. ,0,40);
   RooRealVar    lR2Sigma("Rsigma2","RSigma2",12.,1,40);
   RooRealVar    lR1Frac ("Rfrac"  ,"Rfrac"  ,0.5,0.,1.0);
   RooRealVar    lR2Mean ("Rmean",  "Rmean",0,-10,10); lR2Mean.setConstant(kTRUE); 
                                                                                                     
   // ===> This is the model for the 1D  
   RooGaussian   lRGaus1("Rgaus1","Rgaus1",lRXVar,lR2Mean,lR1Sigma);
   RooGaussian   lRGaus2("Rgaus2","Rgaus2",lRXVar,lR2Mean,lR2Sigma);
   RooAddPdf     lRGAdd ("RAdd"  ,"RAdd",lRGaus1,lRGaus2,lR1Frac);


  if(iRMS){   
    for(int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
      // pVal = (lPar - iMeanFit->Eval(fZPt));
      double temp = vlYVals_all[lPar!=fU1][iRMS][iev];
      if(iMeanFit != 0) temp = vlYVals_all[lPar!=fU1][iRMS][iev] - iMeanFit->Eval(vlXVals_all[lPar!=fU1][iRMS][iev]);
      vlYVals_all[lPar!=fU1][iRMS][iev] = abs(temp);
      vlYTVals_all[lPar!=fU1][iRMS][iev] = temp; 
    }
  }

  // MARIA something to add to the plots
  TString leg = "#sigma_{mean} = ";   
  //  leg += lY0[i0];
  
  TLatex latexLabel;    
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  //    latexLabel.DrawLatex(0.2, 0.7, leg);
  
  TString leg1 = "NVtx ";  
  if(fId == 0) leg1 += " = 0-12";

  if(fId == 1) leg1 += " = 1";
  if(fId == 2) leg1 += " = 2";
  if(fId == 3) leg1 += " = 3";
  if(fId == 4) leg1 += " = 4";
  if(fId == 5) leg1 += " = 5";
  if(fId == 6) leg1 += " = 6";
  if(fId == 7) leg1 += " = 7";
  if(fId == 8) leg1 += " = 8";
  if(fId == 9) leg1 += " = 9";
  if(fId == 10) leg1 += " = 10";
  if(fId == 11) leg1 += " = 11";
  if(fId == 12) leg1 += " = 12";
  if(fId == 13) leg1 += " = 13";
  if(fId == 14) leg1 += " = 14";
  if(fId == 15) leg1 += " = 15";
  if(fId == 16) leg1 += " = 16";
  if(fId == 17) leg1 += " = 17";
  if(fId == 18) leg1 += " = 18";
  if(fId == 19) leg1 += " = 19";
  if(fId >= 20) leg1 += " >= 20";

  TString legDATA = "DATA";  
  if(!fData) legDATA = "MC";
  if(!fData && doPosW) legDATA += " Wpos";
  if(!fData && doNegW) legDATA += " Wneg";

  TString legU1U2 = "U1";  
  if(lPar!=fU1) legU1U2 = "U2";

  iC->cd();
  //Basic Fit
  //TGraphErrors *pGraphA = new TGraphErrors(lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]),(Float_t*)&(lXEVals[0]),(Float_t*)&(lYEVals[0]));
  // TGraph *pGraphA       = new TGraph      (lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]));

  TGraph *pGraphA       = new TGraph(lXVals_all.at(0).at(0).size(), 
                                     vlXVals_all[0][0], 
                                     vlYVals_all[lPar!=fU1][iRMS]
                                     );

  std::string lType     = "pol3"; if(iPol1) lType = "pol2"; //pol1 -->higgs 
  if(iType == 1)  lType = "pol3"; 

  lType     = "pol2";
  if(usePol3) lType     = "pol3";
  //http://root.cern.ch/root/html/TGraph.html
  //"EX0" When fitting a TGraphErrors or TGraphAsymErrors do not consider errors in the coordinate
  //"R"   Use the Range specified in the function range
  //"S"  The result of the fit is returned in the TFitResultPtr
  //"E"  Perform better Errors estimation using Minos technique
  
  //// THIS IS THE unbinned Fit
  TF1 *lFit = new TF1("test",lType.c_str()); //fZPtMax = 200;                                                                                                                               
  //  if(!iRMS && doSecondStep) lFit->FixParameter(0,0);
  if(!iRMS && !doSecondStep) lFit->FixParameter(0,0);

  //  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SR","EXO",fZPtMin,fZPtMax); //"EXO"
  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SRE","",fZPtMin,fZPtMax); 
  computeFitErrors(iFit,lFitPtr,lFit,iRMS);

  if(doPrint) {

    pGraphA->Draw("ap");
    drawErrorBands(iFit,fZPtMax);
    
    Double_t chi2   = lFit->GetChisquare(); // to retrieve the fit chi2 
                                                                                                                      
    int ndof = lFitPtr->Ndf();
    if(lPar==fU1 && !iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U1 mean " << endl;
    if(lPar==fU1 && iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U1 RMS " << endl;
    if(!lPar==fU1 && !iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U2 mean " << endl;
    if(!lPar==fU1 && iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U2 RMS " << endl;

    if(lPar==fU1) pGraphA->GetYaxis()->SetTitle("U1"); 
    if(lPar!=fU1) pGraphA->GetYaxis()->SetTitle("U2"); 
    pGraphA->GetXaxis()->SetTitle("Z p_{T}"); 


    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);

    TFile f1("fileGraph_Mean.root","UPDATE");

    if(iRMS==0 && lPar==fU1)  pGraphA->SetName("mean_U1");
    if(!iRMS==0 && lPar==fU1) pGraphA->SetName("RMS_U1");;
    if(!iRMS==0 && lPar!=fU1) pGraphA->SetName("RMS_U2");;
    pGraphA->Write();
    f1.Write();
    f1.Close();

    TString test="";
    
    if(iRMS==0) test += "DrawUnbinned_mean";
    if(!iRMS==0) test += "DrawUnbinned_RMS";
    
    if(lPar==fU1) test += "_U1";
    if(lPar!=fU1) test += "_U2";
    
    if(fData) test += "_data";
    if(!fData) test += "_MC";

    if(doPosW) test += "_Wpos";
    if(doNegW) test += "_Wneg";

    test += "_vtx";
    test += fId;
    test += ".png";
    
    iC->SaveAs(test.Data());
    
  }

  if(!doType2) return;
  if(!iRMS) return;

  //Build the double gaussian dataset
  std::vector<RooDataSet> lResidVals; 
  std::vector<RooDataHist> lResidVals2D; 
  std::stringstream pSS; pSS << "Crapsky" << 0;
  pSS << "2D"; RooDataHist lData2D(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar,lRPt));
  //  int lNBins = 10;
  //  int lNBins = 25;
  int lNBins = 50;
  for(int i0  = 0; i0 < lNBins; i0++) { 
    RooDataSet lData(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar)); 
    //    lRPt.setBins(500);
    //    lRPt.setBins(fZPtMax);
    //    lRPt.setBins(lNBins);
    lResidVals.push_back(lData); 
  }
  lRPt.setBins(fZPtMax*50);
  lResidVals2D.push_back(lData2D);

  // not sure those will be used (!)
  std::vector<float> lX3SVals; std::vector<float> lXE3SVals; std::vector<float> lY3SVals; std::vector<float> lYE3SVals; //Events with sigma > 3
  std::vector<float> lX4SVals; std::vector<float> lXE4SVals; std::vector<float> lY4SVals; std::vector<float> lYE4SVals; //Events with sigma > 3


  for(unsigned int i0 = 0; i0 < lXVals_all.at(0).at(0).size(); i0++) { 
    // normalization too much 
    // MARIA changed the lFit to iFit  
    double lYTest = iFit->Eval(vlXVals_all[lPar!=fU1][iRMS][i0])*sqrt(2*3.14159265)/2.;
    // MARIA: here the switch for pull or GeV
    if(!doAbsolute) lRXVar.setVal(vlYTVals_all[lPar!=fU1][iRMS][i0]/(lYTest)); // residual  for the Pull
    if(doAbsolute) lRXVar.setVal(vlYTVals_all[lPar!=fU1][iRMS][i0]/(sqrt(2*3.14159265)/2.));  // residual  for the fit in GeV 
    lRPt.setVal(vlXVals_all[lPar!=fU1][iRMS][i0]);     // Zpt
    lRWeight.setVal(1./vlYEVals_all[lPar!=fU1][iRMS][i0]/vlYEVals_all[lPar!=fU1][iRMS][i0]);
    int pId = int(vlXVals_all[lPar!=fU1][iRMS][i0]/(fZPtMax/lNBins)); if(pId > lNBins-1) pId = lNBins-1; 
    // MARIA: USED for the 1D fit (binned/uncorrelated in Zpt and binned in residuals )  
    lResidVals[pId].add(RooArgSet(lRXVar));//,lRWeight.getVal()); //Fill the Double Gaussian
    // MARIA: USED for the 2D fit (binned/correlated in Zpt and unbinned in Residauls )   
    lResidVals2D[0].add(RooArgSet(lRXVar,lRPt));//,lRWeight.getVal()); //Fill the Double Gaussian
  }
  lGAdd.mustBeExtended();
  /// swith off Minos to speed up 
  //  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos());//,Minos()); //Double Gaussian fit
  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos()); //Double Gaussian fit

  //// AFTER here set up for the 1D fit                                                                                                                    
  if(doPrintAll) {

    //Plot it all
    lRXVar.setBins(100); // Bins of the binned Pull
    
    double *lX   = new double[lNBins];
    double *lY0  = new double[lNBins];
    double *lY1  = new double[lNBins];
    double *lY2  = new double[lNBins];
    
    double *lEX  = new double[lNBins];
    double *lEY0 = new double[lNBins];
    double *lEY1 = new double[lNBins];
    double *lEY2 = new double[lNBins];

    double *lchi2  = new double[lNBins];
    double *myFrac = new double[lNBins];
    double *myMean = new double[lNBins];
    double *myFracE = new double[lNBins];
    double *myMeanE = new double[lNBins];

    for(int i0  = 0; i0 < lNBins; i0++) { 
      lRGAdd.fitTo(lResidVals[i0],Save(kTRUE),NumCPU(2));//,Minos());//,Minos()); //Double Gaussian fit
      lRPt.setRange(i0*10,i0*10+10);
      RooPlot *lFrame1 = lRXVar.frame(Title("XXX")) ;
      //lResidVals2D[0].plotOn(lFrame1);
      lResidVals [i0].plotOn(lFrame1,RooFit::MarkerColor(kRed));
      lRGAdd.plotOn(lFrame1);
      lRGAdd.plotOn(lFrame1,RooFit::Components(lRGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
      lRGAdd.plotOn(lFrame1,RooFit::Components(lRGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(kViolet));
      iC->cd(); lFrame1->Draw();
      
      Double_t chi21D = lFrame1->chiSquare();
      TString chi2str = "chi2 = ";
      chi2str += chi21D;
      latexLabel.DrawLatex(0.65, 0.8, chi2str);
      TString frac1str = "frac = ";
      frac1str += lR1Frac.getVal();
      latexLabel.DrawLatex(0.65, 0.7, frac1str);
      TString rms1str = "sigma1 = ";
      rms1str += lR1Sigma.getVal();
      latexLabel.DrawLatex(0.65, 0.6, rms1str);
      TString rms2str = "sigma2 = ";
      rms2str += lR2Sigma.getVal();
      latexLabel.DrawLatex(0.65, 0.5, rms2str);

      lchi2[i0]=chi21D;
      myFrac[i0] = lR1Frac.getVal();
      myMean[i0] = lR2Mean.getVal();
      myFracE[i0] = lR1Frac.getError();
      myMeanE[i0] = lR2Mean.getError();

      //cin.get();    
      lX[i0] = (fZPtMax/lNBins)*i0;
      lEX[i0] = (fZPtMax/lNBins)/10;
      // normalization too much
      //      lY0[i0] = (lR1Frac .getVal()*lR1Sigma.getVal() + (1.-lR1Frac.getVal())*lR2Sigma.getVal())/sqrt(2*3.14159265)*2.;
      lY0[i0] = (lR1Frac .getVal()*lR1Sigma.getVal() + (1.-lR1Frac.getVal())*lR2Sigma.getVal());
      lY1[i0] = lR1Sigma.getVal()/sqrt(2*3.14159265)*2.;
      lY2[i0] = lR2Sigma.getVal()/sqrt(2*3.14159265)*2.;
      lEY0[i0]  = lR1Frac .getError()*lR1Sigma.getVal()  * lR1Frac .getError()*lR1Sigma.getVal();
      lEY0[i0] += lR1Frac .getVal()  *lR1Sigma.getError()* lR1Frac .getVal()  *lR1Sigma.getError();
      lEY0[i0] += lR1Frac .getError()*lR2Sigma.getVal()  * lR1Frac .getError()*lR2Sigma.getVal();
      lEY0[i0] += (1-lR1Frac .getVal())*lR2Sigma.getError()* (1-lR1Frac .getVal())  *lR2Sigma.getError();
      lEY0[i0] = sqrt(lEY0[i0])/sqrt(2*3.14159265)*2.;
      lEY1[i0] = lR1Sigma.getError()/sqrt(2*3.14159265)*2.;
      lEY2[i0] = lR2Sigma.getError()/sqrt(2*3.14159265)*2.;
      
      //    if(doPrintAll) {
      
      latexLabel.DrawLatex(0.25, 0.8, leg1);

      TString leg2 = "Zpt = ";
      leg2 += i0;      

      latexLabel.DrawLatex(0.25, 0.7, leg2);
      latexLabel.DrawLatex(0.25, 0.25, legDATA);
      latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

      //      stringstream pSS1; 
      TString test="";
      
      if(iFitS1==0) test += "mean";
      if(iFitS1!=0) test += "rms";
      
      if(lPar==fU1) test += "_U1";
      if(lPar!=fU1) test += "_U2";
      
      if(fData) test += "_data";
      if(!fData) test += "_MC";

      if(doPosW) test += "_Wpos";
      if(doNegW) test += "_Wneg";
      
      test += "_Zpt";
      test += i0;

      test += "_jets";
      test += fNJetSelect;
      test += "_vtx";
      test += fId;
      test += ".png";
      
      iC->SaveAs(test.Data());
      
    }
    //  }


    TGraphErrors *lM0 = new TGraphErrors(lNBins,lX,myMean,lEX,lEY0); lM0->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG0 = new TGraphErrors(lNBins,lX,lY0,lEX,lEY0); lG0->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG1 = new TGraphErrors(lNBins,lX,lY1,lEX,lEY1); lG1->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG2 = new TGraphErrors(lNBins,lX,lY2,lEX,lEY2); lG2->SetMarkerStyle(kFullCircle);
    
    if(lPar==fU1) lM0->GetYaxis()->SetTitle("U1 mean");
    if(lPar!=fU1) lM0->GetYaxis()->SetTitle("U2 mean");
    if(lPar==fU1) lG0->GetYaxis()->SetTitle("U1 #sigma_{mean}");
    if(lPar!=fU1) lG0->GetYaxis()->SetTitle("U2 #sigma_{mean}");
    lG1->GetYaxis()->SetTitle("#sigma_{1}");
    lG2->GetYaxis()->SetTitle("#sigma_{2}");
    
    lM0->GetYaxis()->SetRangeUser(-1.5,1.5);
    lG0->GetYaxis()->SetRangeUser(0.,5.);
    lG1->GetYaxis()->SetRangeUser(0.,5.0);
    lG2->GetYaxis()->SetRangeUser(0.,5.0);
    
    lM0->GetXaxis()->SetTitle("Z p_{T}");
    lG0->GetXaxis()->SetTitle("Z p_{T}");
    lG1->GetXaxis()->SetTitle("Z p_{T}");
    lG2->GetXaxis()->SetTitle("Z p_{T}");
    
    //////
    //// cin.get();
    //////
    
    //  if(doPrintAll) {
    
    iC->cd();
    TF1 *lFitPull  = new TF1("lFitPull",   "pol2");
    TF1 *iFitPull  = new TF1("iFitPull",   "pol10"); // this need to be large to store the errors from the computeFitErrors
    TFitResultPtr  lFitPtr0 = lG0->Fit(lFitPull,"SRE","", fZPtMin, fZPtMax); //"EXO"                                                                       
    computeFitErrors(iFitPull,lFitPtr0,lFitPull,iRMS);
    lG0->Draw("ape");
    drawErrorBands(iFitPull,fZPtMax);
    lG0->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    TString pSS0="G0";
    
    if(lPar==fU1) pSS0 += "_U1";
    if(lPar!=fU1) pSS0 += "_U2";
    
    if(fData) pSS0 += "_data";
    if(!fData) pSS0 += "_MC";
    
    if(doPosW) pSS0 += "_Wpos";
    if(doNegW) pSS0 += "_Wneg";
    
    pSS0 += "_";
    pSS0 += fNJetSelect;
    pSS0 += "_";
    pSS0 += fId;
    pSS0 += ".png";
    
    iC->SaveAs(pSS0.Data());



    /*
    stringstream pSS0;
    if(lPar==fU1) { 
      if(fData) pSS0 << "G0_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    } else {
      if(fData) pSS0 << "G0_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    }

    iC->SaveAs(pSS0.str().c_str());
    */

    ///////                                                                                                                                                


    /*
    stringstream pSS0;
    if(lPar==fU1) { 
      if(fData) pSS0 << "G0_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    } else {
      if(fData) pSS0 << "G0_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    }

    iC->SaveAs(pSS0.str().c_str());
    */

    ///////                                                                                                                                                
    ///////                                                                                                                                                

    TF1 *lFitPullMean  = new TF1("lFitPullMean",   "pol2");
    TF1 *iFitPullMean  = new TF1("iFitPullMean",   "pol10"); // this need to be large to store the errors from the computeFitErrors
    TFitResultPtr  lFitPtrM0 = lM0->Fit(lFitPullMean,"SRE","", fZPtMin, fZPtMax); //"EXO"                                                                  
    computeFitErrors(iFitPullMean,lFitPtrM0,lFitPullMean,iRMS);
    lM0->Draw("ape");
    drawErrorBands(iFitPullMean,fZPtMax);
    lM0->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);

    TString pMM0="M0";

    if(lPar==fU1) pMM0 += "_U1";
    if(lPar!=fU1) pMM0 += "_U2";

    if(fData) pMM0 += "_data";
    if(!fData) pMM0 += "_MC";

    if(doPosW) pMM0 += "_Wpos";
    if(doNegW) pMM0 += "_Wneg";

    pMM0 += "_";
    pMM0 += fNJetSelect;
    pMM0 += "_";
    pMM0 += fId;
    pMM0 += ".png";

    iC->SaveAs(pMM0.Data());


    //////
    //// cin.get();
    //////
    
    iC->cd();
    //  TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SRE","", fZPtMin, fZPtMax); //"EXO"
    computeFitErrors(iFitS1,lFitPtr1,lFit,iRMS);

    //    lG1->GetYaxis()->SetRangeUser(0.,5.);

    lG1->Draw("ape");
    drawErrorBands(iFitS1,fZPtMax);
    lG1->Draw("pe");


    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    TString pSS1="pG1";
    
    if(lPar==fU1) pSS1 += "_U1";
    if(lPar!=fU1) pSS1 += "_U2";
    
    if(fData) pSS1 += "_data";
    if(!fData) pSS1 += "_MC";
    
    if(doPosW) pSS1 += "_Wpos";
    if(doNegW) pSS1 += "_Wneg";
    
    pSS1 += "_";
    pSS1 += fNJetSelect;
    pSS1 += "_";
    pSS1 += fId;
    pSS1 += ".png";
    
    iC->SaveAs(pSS1.Data());

    /*
     stringstream pG1;
    if(lPar==fU1) {  
    if(fData) pG1 << "pG1_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
    if(!fData) pG1 << "pG1_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    } else {
    if(fData) pG1 << "pG1_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
    if(!fData) pG1 << "pG1_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    }
    //  iC->SaveAs("G1.png");
    iC->SaveAs(pG1.str().c_str());
    */
    
    //////
    //// cin.get();
    //////
    
    iC->cd();
    //  TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SRE","", fZPtMin, fZPtMax); 
    computeFitErrors(iFitS2,lFitPtr2,lFit,iRMS);

    //    lG2->GetYaxis()->SetRangeUser(0.,5.);

    lG2->Draw("ape");
    drawErrorBands(iFitS2,fZPtMax);
    lG2->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    TString pSS2="pG2";
    
    if(lPar==fU1) pSS2 += "_U1";
    if(lPar!=fU1) pSS2 += "_U2";
    
    if(fData) pSS2 += "_data";
    if(!fData) pSS2 += "_MC";
    
    if(doPosW) pSS2 += "_Wpos";
    if(doNegW) pSS2 += "_Wneg";
    
    pSS2 += "_";
    pSS2 += fNJetSelect;
    pSS2 += "_";
    pSS2 += fId;
    pSS2 += ".png";
    
    iC->SaveAs(pSS2.Data());    

    /*
      stringstream pG2; 
      if(lPar==fU1) {
      if(fData) pG2 << "pG2_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pG2 << "pG2_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
      } else {
      if(fData) pG2 << "pG2_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pG2 << "pG2_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
      }
      
      iC->SaveAs(pG2.str().c_str());
    // cin.get();
    */

    iC->cd();
    iC->Divide(2,2);
    iC->cd(1);
    TGraph* gChi2 = new TGraph(lNBins, lX, lchi2);
    gChi2->SetName("chi2");
    if(lPar==fU1) gChi2->SetTitle("U1");
    if(lPar!=fU1) gChi2->SetTitle("U2");
    gChi2->GetXaxis()->SetTitle("Z p_{T}");
    gChi2->GetYaxis()->SetTitle("chi2");
    gChi2->Draw("ap");

    iC->cd(2);
    TGraph* gFrac = new TGraph(lNBins, lX, myFrac);
    if(lPar==fU1) gFrac->SetTitle("U1");
    if(lPar!=fU1) gFrac->SetTitle("U2");
    //    gFrac->SetName("gFrac");                                                                                                            
    gFrac->GetXaxis()->SetTitle("Z p_{T}");
    gFrac->GetYaxis()->SetTitle("Frac");
    gFrac->Draw("ap");

    iC->cd(3);
    TGraph* gMean = new TGraph(lNBins, lX, myMean);
    if(lPar==fU1) gMean->SetTitle("U1");
    if(lPar!=fU1) gMean->SetTitle("U2");
    //    gFrac->SetName("gFrac");                                                                                                            
    gMean->GetXaxis()->SetTitle("Z p_{T}");
    gMean->GetYaxis()->SetTitle("Mean");
    gMean->Draw("ap");

    if(lPar==fU1) iC->SaveAs("chi2Plot_U1.png");
    if(lPar!=fU1) iC->SaveAs("chi2Plot_U2.png");


  return;
  } // end print

  bool doPol1=false;

  // MARIA comment for now
  TMatrixDSym   lCov = pFR->covarianceMatrix();

  cout << "covariance matrix " << endl;
  pFR->covarianceMatrix().Print();
  
  //Now Store each sigma of the double gaussian in a fit function
  iFitS1->SetParameter(0,lA1Sig.getVal()); 
  iFitS1->SetParameter(1,lB1Sig.getVal()); 
  if(!doPol1) iFitS1->SetParameter(2,lC1Sig.getVal());
  
  double E0S1=lA1Sig.getError()*lA1Sig.getError();
  double E1S1=2*(lCov)(0,2);
  double E2S1=lB1Sig.getError()*lB1Sig.getError();
  double E3S1=0;
  double E4S1=0;

  if(!doPol1) {
    // the ones below only if we have three Param
    E2S1+=2*(lCov)(0,4);
    E3S1=2*(lCov)(2,4);
    E4S1=lC1Sig.getError()*lC1Sig.getError();
  }
  iFitS1->SetParError(0,E0S1);
  iFitS1->SetParError(1,E1S1);
  iFitS1->SetParError(2,E2S1);
  if(!doPol1) iFitS1->SetParError(3,E3S1);
  if(!doPol1) iFitS1->SetParError(4,E4S1);

  /////

  iFitS2->SetParameter(0,lA2Sig.getVal()); 
  iFitS2->SetParameter(1,lB2Sig.getVal()); 
  iFitS2->SetParameter(2,lC2Sig.getVal());

  double E0S2=lA2Sig.getError()*lA2Sig.getError();
  double E1S2=2*(lCov)(1,3);
  double E2S2=lB2Sig.getError()*lB2Sig.getError();
  double E3S2=0;
  double E4S2=0;
  if(!doPol1) {
    // the ones below only if we have three Param
    E2S2+=2*(lCov)(1,5);
    E3S2=2*(lCov)(3,5);
    E4S2=lC2Sig.getError()*lC2Sig.getError();
  }
  iFitS2->SetParError(0,E0S2);
  iFitS2->SetParError(1,E1S2);
  iFitS2->SetParError(2,E2S2);
  if(!doPol1) iFitS2->SetParError(3,E3S2);
  if(!doPol1) iFitS2->SetParError(4,E4S2);

  /*
  /// ORIGINAL FROM PHIL
  iFitS1->SetParameter(0,lA1Sig.getVal()); iFitS1->SetParError(0,lA1Sig.getError()*lA1Sig.getError());
  iFitS1->SetParError(1,2*(lCov)(0,2));
  iFitS1->SetParameter(1,lB1Sig.getVal()); iFitS1->SetParError(2,lB1Sig.getError()*lB1Sig.getError());
  iFitS1->SetParError(3,2*(lCov)(2,4));
  iFitS1->SetParameter(2,lC1Sig.getVal()); iFitS1->SetParError(4,lC1Sig.getError()*lC1Sig.getError());
  iFitS1->SetParError(5,2*(lCov)(0,4));
  iFitS1->SetParameter(3,lD1Sig.getVal()); iFitS1->SetParError(6,lD1Sig.getError()*lD1Sig.getError());
  
  iFitS2->SetParameter(0,lA2Sig.getVal()); iFitS2->SetParError(0,lA2Sig.getError()*lA2Sig.getError());
  iFitS2->SetParError(1,2*(lCov)(1,3));
  iFitS2->SetParameter(1,lB2Sig.getVal()); iFitS2->SetParError(2,lB2Sig.getError()*lB2Sig.getError());
  iFitS2->SetParError(3,2*(lCov)(3,5));
  iFitS2->SetParameter(2,lC2Sig.getVal()); iFitS2->SetParError(4,lC2Sig.getError()*lC2Sig.getError());
  iFitS2->SetParError(5,2*(lCov)(1,5));
  iFitS2->SetParameter(3,lD2Sig.getVal()); iFitS2->SetParError(6,lD2Sig.getError()*lD2Sig.getError());
  */



}

void fitRecoil1(TTree * iTree, /*float &iVPar,*/ double &iVPar,
	       TF1 *iMFit, TF1 *iMRMSFit, TF1 *iRMS1Fit, TF1 *iRMS2Fit,int iType) { 
  TCanvas *lXC1 = new TCanvas("C1","C1",800,600); lXC1->cd(); 

  fitGraph(iTree,fTTbarTree,lXC1,iVPar,iMFit   ,0       ,0       ,0    ,true ,false,iType);
  fitGraph(iTree,fTTbarTree,lXC1,iVPar,iMRMSFit,iRMS1Fit,iRMS2Fit,iMFit,false,true ,iType);

}

void writeRecoil(TF1 *iU1Fit, TF1 *iU1MRMSFit, TF1 *iU1RMS1Fit, TF1 *iU1RMS2Fit,
		 TF1 *iU2Fit, TF1 *iU2MRMSFit, TF1 *iU2RMS1Fit, TF1 *iU2RMS2Fit,
		 std::string iName="recoilfit.root") {
  TFile *lFile = new TFile(iName.c_str(),"UPDATE");
  assert(iU1Fit);     iU1Fit    ->Write();
  assert(iU2Fit);     iU2Fit    ->Write();
  assert(iU1MRMSFit); iU1MRMSFit->Write();
  assert(iU1RMS1Fit); iU1RMS1Fit->Write();
  assert(iU1RMS2Fit); iU1RMS2Fit->Write();
  assert(iU2MRMSFit); iU2MRMSFit->Write();
  assert(iU2RMS1Fit); iU2RMS1Fit->Write();
  assert(iU2RMS2Fit); iU2RMS2Fit->Write();
  lFile->Close();
  delete lFile;
}

/*
void writeCorr(TF1 *iU1U2PFCorr, TF1 *iU1U2TKCorr, TF1 *iPFTKU1Corr, TF1 *iPFTKU2Corr, TF1 *iPFTKUM1Corr, TF1 *iPFTKUM2Corr, 
	       std::string iName) {
  TFile *lFile = new TFile((iName).c_str(),"UPDATE");
  assert(iU1U2PFCorr);     iU1U2PFCorr->Write();
  //assert(iU1U2TKCorr);     iU1U2TKCorr->Write();
  //assert(iU1U2PFCorr);     iPFTKU1Corr->Write();
  //assert(iU1U2TKCorr);     iPFTKU2Corr->Write();
  //assert(iU1U2PFCorr);     iPFTKUM1Corr->Write();
  //assert(iU1U2TKCorr);     iPFTKUM2Corr->Write();
  lFile->Close();
  delete lFile;
}
*/
void loopOverTree(TTree *iTree, bool isBKG=false) {

  if(!isBKG) cout << "==> looping over tree" << endl;
  if(isBKG) cout << "==> looping over BKG tree" << endl;

  int nEntries = iTree->GetEntries();
  if(isBKG) nEntries = iTree->GetEntries()/30;

  for(int i1 = 0; i1 <  nEntries; i1++) {
  //  for(int i1 = 0; i1 <  1000000; i1++) {
    //  for(int i1 = 0; i1 <  1e6; i1++) {
    iTree->GetEntry(i1);

    ////////    cout << "RUN " << fRun << " LUMI " << fLumi << " EVENT " << fEvent << endl;

    /*
    // this what I used to do
    // for MC select only odd
    if(!fData && fEvent % 2) continue;
    */

    /*
    // for iteration on MC select only even
    if(!fData && !(fEvent % 2)) continue;
    */

    //    if(doRunA && fEvent>175832) continue;
    //    if(doRunB && fEvent<175832) continue;

    // THIS WAS FOR THE VTX selection
    if(doVTXbinning && fId!=(-1) && !passId(fId)) continue;

    // this is Z selection
    if((!doPosW) && (!doNegW) && (!runSelection()))  continue;

    // this is for the W selection
    if(doPosW && (!runWSelection(true))) continue;
    if(doNegW && (!runWSelection(false))) continue;

    if(fZPt<0) continue;
    if(fZPt > fZPtMax) continue;

    //////
    //////

    if(doYbinning && (!calculateBosonRap(fId))) continue; 
    //    if(!calculateBosonRap(1)) continue;  // look the bin with 1<y<2 
   
    if(pType!=-1) {
      ////    cout << "passed selection " << endl;
      if((!doPosW) && (!doNegW) && !checkPDF(23,false)) continue; 
      if((doPosW) && !checkPDF(24,false)) continue; 
      if((doNegW) && !checkPDF(-24,false)) continue; 
    }

    ////
    ////

    if(doMetPhiCorr) {
      fMet  = getPhiCorrMET(1);
      fMPhi = getPhiCorrMET(2);
    }

    if(doNeutralMet) {
      fMet  = getNeuMET(1);
      fMPhi = getNeuMET(2);
    }

    if(doChargedMet) {
      fMet = ftkMet;
      fMPhi = ftkMPhi;
    }

    if(doIterativeMet && doType2) {

      /*
      cout << " before applyType2CorrMET " ;
      cout << lZMU1Fit[0] << " "  
	   << lZDU1Fit[0]<< " " << lZMU1Fit[0] << " " 
	   << lZDU1RMSSMFit[0] << " " << lZMU1RMSSMFit[0] << " "
	   << lZDU1RMS1Fit[0] << " " << lZMU1RMS1Fit[0] << " "
	   << lZDU1RMS2Fit[0] << " " << lZMU1RMS2Fit[0] << " " 
	   << lZDU2RMSSMFit[0] << " " << lZMU2RMSSMFit[0] << " "                                                                                                                                      
	   << lZDU2RMS1Fit[0] << " " << lZMU2RMS1Fit[0] << " "
	   << lZDU2RMS2Fit[0] << " " << lZMU2RMS2Fit[0] << " "
	   << endl;
      */

      if(dodebug)      cout << "original MET " << fMet ; 

      // this is good for the inclusive bins since take the first only 
      applyType2CorrMET(fMet,fMPhi,
                        fZPt,fZPhi,
                        fPt1,fPhi1,
			lZMU1Fit[0],
                        lZDU1Fit[0], lZMU1Fit[0], // SCALE
                        lZDU1RMSSMFit[0], lZMU1RMSSMFit[0], // RMS
			lZDU1RMS1Fit[0], lZMU1RMS1Fit[0], 
			lZDU1RMS2Fit[0], lZMU1RMS2Fit[0], 
                        lZDU2RMSSMFit[0], lZMU2RMSSMFit[0], // RMS 
			lZDU2RMS1Fit[0], lZMU2RMS1Fit[0], 
			lZDU2RMS2Fit[0], lZMU2RMS2Fit[0] 
			);
      /*
      int rapBin=0;
      if(fabs(fZRap)<1) rapBin=0;
      if(fabs(fZRap)>=1 && fabs(fZRap)<=1.25) rapBin=1;
      if(fabs(fZRap)>=1.25 && fabs(fZRap)<=1.5) rapBin=2;
      if(fabs(fZRap)>=1.5 && fabs(fZRap)<=1.75) rapBin=3;
      if(fabs(fZRap)>=1.75 && fabs(fZRap)<=2.0) rapBin=4;
      if(fabs(fZRap)>2.0) rapBin=5;

      applyType2CorrMET(fMet,fMPhi,
                        fZPt,fZPhi,
                        fPt1,fPhi1,
			lZMU1Fit[rapBin],
                        lZDU1Fit[rapBin], lZMU1Fit[rapBin], // SCALE
                        lZDU1RMSSMFit[rapBin], lZMU1RMSSMFit[rapBin], // RMS
			lZDU1RMS1Fit[rapBin], lZMU1RMS1Fit[rapBin], 
			lZDU1RMS2Fit[rapBin], lZMU1RMS2Fit[rapBin], 
                        lZDU2RMSSMFit[rapBin], lZMU2RMSSMFit[rapBin], // RMS 
			lZDU2RMS1Fit[rapBin], lZMU2RMS1Fit[rapBin], 
			lZDU2RMS2Fit[rapBin], lZMU2RMS2Fit[rapBin] 
			);
      */

      if(dodebug)       cout << " MET after smearing " << fMet ; 

      //////
      //////
      //////

      /*
      applyType2CorrU(fMet,fMPhi,
		      fZPt,fZPhi,
		      fPt1,fPhi1,
		      lZMU1Fit[0],
		      lZDU1Fit[0], lZMU1Fit[0], // SCALE                                                                                                                                       
		      lZDU1RMSSMFit[0], lZMU1RMSSMFit[0], // RMS                                                                                                                               
		      lZDU1RMS1Fit[0], lZMU1RMS1Fit[0],
		      lZDU1RMS2Fit[0], lZMU1RMS2Fit[0],
		      lZDU2RMSSMFit[0], lZMU2RMSSMFit[0], // RMS                                                                                                                               
		      lZDU2RMS1Fit[0], lZMU2RMS1Fit[0],
		      lZDU2RMS2Fit[0], lZMU2RMS2Fit[0]
		      );

      */

      //////
      //////
      //////

      if(doSecondStep) {

	applyType2CorrMET(fMet,fMPhi,
			  fZPt,fZPhi,
			  fPt1,fPhi1,
			  lZMU1Fit_second[0],
			  lZDU1Fit[0], lZMU1Fit_second[0], // SCALE
			  lZDU1RMSSMFit[0], lZMU1RMSSMFit_second[0], // RMS
			  lZDU1RMS1Fit[0], lZMU1RMS1Fit_second[0], 
			  lZDU1RMS2Fit[0], lZMU1RMS2Fit_second[0], 
			  lZDU2RMSSMFit[0], lZMU2RMSSMFit_second[0], // RMS 
			  lZDU2RMS1Fit[0], lZMU2RMS1Fit_second[0], 
			  lZDU2RMS2Fit[0], lZMU2RMS2Fit_second[0] 
			  );
	if(dodebug) 	cout << " MET after second smearing " << fMet << endl; 

      }
    }

    if(doIterativeMet && (!doType2)) {

      TRandom3 * fRandom = new TRandom3(1234567);

      /*
	TLorentzVector neuMET, chMET, allMET, newchMET, newPFMET;
	allMET.SetPtEtaPhiM(fMet,0,fMPhi,0);
	chMET.SetPtEtaPhiM(ftkMet,0,ftkMPhi,0);
	neuMET = allMET-chMET;
	
	fMet = ftkMet;
	fMPhi = ftkMPhi;
      */
      
      //      cout << "before " << fMet  << " " << fMPhi << endl;
      applyType1CorrMET(fMet,fMPhi,
                        fZPt,fZPhi,
			fPt1,fPhi1,
                        fRandom,
                        lZDU1Fit[0],lZMU1Fit[0], // SCALE 
			lZDU1RMSSMFit[0],lZMU1RMSSMFit[0], // RMS 
                        lZDU2RMSSMFit[0],lZMU2RMSSMFit[0] // RMS 
                        //                  double &iU1,double &iU2,double iFluc,double iScale,
			);
      //      cout << "after " << fMet  << " " << fMPhi << endl;

      /*      
      if(!doChargedMet) {
	newchMET.SetPtEtaPhiM(fMet,0,fMPhi,0);
	newPFMET=newchMET+neuMET;
	fMet = newPFMET.Pt();
	fMPhi = newPFMET.Phi();
      }
      */
    }

    if(fMet > fMetMax) continue;

    //if(!passMatching()) continue;
    //if(fPt2 > 30) fNJet++;
    
    /*
    //MARIA comment for now
    ////    if(fNBTag > 0) continue;
    if(iPol1 && fZPt > 50. && lPar == fU1 && lPar  > (47.5-0.95*fZPt) && iType == 0) continue; //Remove Diboson events at high pT
    */

    /* MARIA comment for now
       if(fNJetSelect<2  && fNJet != fNJetSelect && fNJetSelect != -1) continue;
       if(fNJetSelect==2 && fNJet<2) continue;
    */

    double lPar;
    // }
    bool U1U2truefalse[2]={true,false};
    //    bool MeanRMStruefalse[2]={false,true};
    
    for(int iU1U2=0; iU1U2<2; iU1U2++){
        std::vector<std::vector<float> > dummy;
        lXVals_all.push_back(dummy); lXEVals_all.push_back(dummy); lYVals_all.push_back(dummy); lYTVals_all.push_back(dummy); lYEVals_all.push_back(dummy);
      
      for(int iMeanRMS=0; iMeanRMS<2; iMeanRMS++){
        std::vector<float> dummy1;
        lXVals_all.at(iU1U2).push_back(dummy1); lXEVals_all.at(iU1U2).push_back(dummy1); 
        lYVals_all.at(iU1U2).push_back(dummy1); lYTVals_all.at(iU1U2).push_back(dummy1); lYEVals_all.at(iU1U2).push_back(dummy1);
        
        bool isU1=U1U2truefalse[iU1U2];
	//	bool iRMS=MeanRMStruefalse[iMeanRMS];
        
        calculateU1U2(lPar,isU1);
	if(dodebug) cout << "lPar " << lPar << " isU1 " << isU1 << endl;

        double pVal = lPar;    
        // if(iRMS && iMeanFit != 0)   pVal = (lPar - iMeanFit->Eval(fZPt));
        if(fWeight == 0) continue;
        lXVals_all.at(iU1U2).at(iMeanRMS).push_back(fZPt);  

	lXEVals_all.at(iU1U2).at(iMeanRMS).push_back(2.);

        double pWeight = fWeight;
        if(fData) pWeight = 1;
        // (!iRMS) ? lYVals_all.at(iU1U2).at(iMeanRMS).push_back(pVal) : lYVals_all.at(iU1U2).at(iMeanRMS).push_back(fabs(pVal));
        lYVals_all.at(iU1U2).at(iMeanRMS).push_back(pVal);
        lYTVals_all.at(iU1U2).at(iMeanRMS).push_back(pVal); 
        lYEVals_all.at(iU1U2).at(iMeanRMS).push_back(1./sqrt(pWeight));

      }
    }
  }

  vlXVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlXEVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlYVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlYTVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlYEVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  
  for(int iU1U2=0; iU1U2<2; ++iU1U2){
    vlXVals_all[iU1U2]=new float*[2];
    vlXEVals_all[iU1U2]=new float*[2];
    vlYVals_all[iU1U2]=new float*[2];
    vlYTVals_all[iU1U2]=new float*[2];
    vlYEVals_all[iU1U2]=new float*[2];
      for(int iMeanRMS=0; iMeanRMS<2; ++iMeanRMS){
        vlXVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlXEVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlYVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlYTVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlYEVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        for(unsigned int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
          vlXVals_all[iU1U2][iMeanRMS][iev]  = lXVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlXVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlXEVals_all[iU1U2][iMeanRMS][iev] = lXEVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlXEVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYVals_all[iU1U2][iMeanRMS][iev]  = lYVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlYVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYTVals_all[iU1U2][iMeanRMS][iev] = lYTVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlYTVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYEVals_all[iU1U2][iMeanRMS][iev] = lYEVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlYEVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
        }
      }
  }
  
  // for(int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
    // cout << "iev= " << iev << endl;
    // for(int iU1U2=0; iU1U2<2; iU1U2++){
      // for(int iMeanRMS=0; iMeanRMS<2; iMeanRMS++){
        // cout << "iU1U2=" << iU1U2 << " iMeanRMS= " << iMeanRMS << endl;
        // cout << "lXVals_all= " << lXVals_all.at(iU1U2).at(iMeanRMS).at(iev)
             // << " lXEVals_all= " << lXEVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << " lYVals_all= " << lYVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << " lYTVals_all= " << lYTVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << " lYEVals_all= " << lYEVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << endl;
      // }
    // }
  // }

}

void fitRecoilMET(TTree *iTree,std::string iName,int type, int lfId) { 
  std::string lPrefix="PF"; if(type == 1) lPrefix = "TK";
  load(iTree,type);

  loopOverTree(iTree);
  //  fU2=fU2*(-1);

  std::stringstream PUstring;
  PUstring << lfId;

  //////////
  /// Those need to be at least pol4/pol6 to store the results 
  /// of the ComputerErrors for pol2/pol3 fits
  /// Made as pol10 to be safe 

  TF1 *lU1Fit     = new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1MRMSFit = new TF1((lPrefix+"u1MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU1RMS1Fit = new TF1((lPrefix+"u1RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1RMS2Fit = new TF1((lPrefix+"u1RMS2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2Fit     = new TF1((lPrefix+"u2Mean_"+PUstring.str()).c_str(),   "pol10");  
  TF1 *lU2MRMSFit = new TF1((lPrefix+"u2MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU2RMS1Fit = new TF1((lPrefix+"u2RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2RMS2Fit = new TF1((lPrefix+"u2RMS2_"+PUstring.str()).c_str(),   "pol10");

  //  cout << "doing U1 " << endl;
  fitRecoil1(iTree,fU1,lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,type);
  //  cout << "doing U2 " << endl;
  fitRecoil1(iTree,fU2,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,type);

  writeRecoil(lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,iName);

}

void fitRecoilMETMIX(TTree *iTree, TTree *iTreeBKG, std::string iName,int type, int lfId) { 
  std::string lPrefix="PF"; if(type == 1) lPrefix = "TK";
  load(iTree,type);
  loopOverTree(iTree);

  load(iTreeBKG,type);
  loopOverTree(iTreeBKG,true);

  //  fU2=fU2*(-1);

  std::stringstream PUstring;
  PUstring << lfId;

  //////////
  /// Those need to be at least pol4/pol6 to store the results 
  /// of the ComputerErrors for pol2/pol3 fits
  /// Made as pol10 to be safe 

  TF1 *lU1Fit     = new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1MRMSFit = new TF1((lPrefix+"u1MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU1RMS1Fit = new TF1((lPrefix+"u1RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1RMS2Fit = new TF1((lPrefix+"u1RMS2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2Fit     = new TF1((lPrefix+"u2Mean_"+PUstring.str()).c_str(),   "pol10");  
  TF1 *lU2MRMSFit = new TF1((lPrefix+"u2MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU2RMS1Fit = new TF1((lPrefix+"u2RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2RMS2Fit = new TF1((lPrefix+"u2RMS2_"+PUstring.str()).c_str(),   "pol10");

  //  cout << "doing U1 " << endl;
  fitRecoil1(iTree,fU1,lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,type);
  //  cout << "doing U2 " << endl;
  fitRecoil1(iTree,fU2,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,type);

  writeRecoil(lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,iName);

}


/*
int readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
	       std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
	       std::string iFName,std::string iPrefix) {
  TFile *lFile  = new TFile(iFName.c_str());
  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((iPrefix+"u1Mean_0").c_str()));
  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1MeanRMS_0").c_str()));
  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1RMS1_0").c_str()));
  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1RMS2_0").c_str()));
  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2Mean_0").c_str()));
  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2MeanRMS_0").c_str()));
  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS1_0").c_str()));
  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS2_0").c_str()));
  lFile->Close();
  return 1;
}
*/

/*
void makeCorr(TTree *iTree,std::string iName) { 
  TCanvas *lXC1 = new TCanvas("C1","C1",800,600); lXC1->cd(); 
  TF1 *lU1U2PFCorr = new TF1("u1u2pfCorr_0","pol6");
  TF1 *lU1U2TKCorr = new TF1("u1u2tkCorr_0","pol6");
  TF1 *lPFTKU1Corr = new TF1("pftku1Corr_0","pol6");
  TF1 *lPFTKU2Corr = new TF1("pftku2Corr_0","pol6");
  TF1 *lPFTKUM1Corr = new TF1("pftkum1Corr_0","pol6");
  TF1 *lPFTKUM2Corr = new TF1("pftkum2Corr_0","pol6");

  vector<TF1*>   lPFU1Fit; vector<TF1*> lPFU1RMSSMFit; vector<TF1*> lPFU1RMS1Fit; vector<TF1*> lPFU1RMS2Fit; 
  vector<TF1*>   lPFU2Fit; vector<TF1*> lPFU2RMSSMFit; vector<TF1*> lPFU2RMS1Fit; vector<TF1*> lPFU2RMS2Fit; 
  vector<TF1*>   lTKU1Fit; vector<TF1*> lTKU1RMSSMFit; vector<TF1*> lTKU1RMS1Fit; vector<TF1*> lTKU1RMS2Fit; 
  vector<TF1*>   lTKU2Fit; vector<TF1*> lTKU2RMSSMFit; vector<TF1*> lTKU2RMS1Fit; vector<TF1*> lTKU2RMS2Fit; 
  readRecoil(lPFU1Fit,lPFU1RMSSMFit,lPFU1RMS1Fit,lPFU1RMS2Fit,
	     lPFU2Fit,lPFU2RMSSMFit,lPFU2RMS1Fit,lPFU2RMS2Fit,iName,"PF");
  readRecoil(lTKU1Fit,lTKU1RMSSMFit,lTKU1RMS1Fit,lTKU1RMS2Fit,  
	     lTKU2Fit,lTKU2RMSSMFit,lTKU2RMS1Fit,lTKU2RMS2Fit,iName,"TK");
  load(iTree,2);
  fitCorr(false,iTree,lXC1,fPFU1,fPFU2,lPFU1Fit[0],lPFU2Fit[0],lPFU1RMSSMFit[0],lPFU2RMSSMFit[0],lU1U2PFCorr);
  //fitCorr(false,iTree,lXC1,fTKU1,fTKU2,lTKU1Fit[0],lTKU2Fit[0],lTKU1RMSSMFit[0],lTKU2RMSSMFit[0],lU1U2TKCorr);
  //fitCorr(false,iTree,lXC1,fPFU1,fTKU1,lPFU1Fit[0],lTKU1Fit[0],lPFU1RMSSMFit[0],lTKU1RMSSMFit[0],lPFTKU1Corr);
  //fitCorr(false,iTree,lXC1,fPFU2,fTKU2,lPFU2Fit[0],lTKU2Fit[0],lPFU2RMSSMFit[0],lTKU2RMSSMFit[0],lPFTKU2Corr);
  //fitCorr(false,iTree,lXC1,fPFU1,fTKU2,lPFU1Fit[0],lTKU2Fit[0],lPFU1RMSSMFit[0],lTKU2RMSSMFit[0],lPFTKUM1Corr);
  //fitCorr(false,iTree,lXC1,fPFU2,fTKU1,lPFU2Fit[0],lTKU1Fit[0],lPFU2RMSSMFit[0],lTKU1RMSSMFit[0],lPFTKUM2Corr);
  writeCorr(lU1U2PFCorr,lU1U2TKCorr,lPFTKU1Corr,lPFTKU2Corr,lPFTKUM1Corr,lPFTKUM2Corr,iName);
}
*/

void fitRecoil(TTree *iTree,std::string iName,int ifId) { 
  fitRecoilMET(iTree,iName,2,ifId);
  //  fitRecoilMET(iTree,iName,0);
  //  fitRecoilMET(iTree,iName,1); 
  //  makeCorr(iTree,iName);
}

void fitRecoilMIX(TTree *iTree,TTree *iTreeBKG,std::string iName,int ifId) { 
  fitRecoilMETMIX(iTree,iTreeBKG,iName,2,ifId);
  //  fitRecoilMET(iTree,iName,0);
  //  fitRecoilMET(iTree,iName,1); 
  //  makeCorr(iTree,iName);
}

void runRecoilFit(int MCtype, int iloop, int processType) { 
//void runRecoilFit(int MCtype, int iloop) { 
  //  Prep();

  gStyle->SetOptFit(111111);

  TString name="recoilfits/recoilfit";

  /// SETTING
  /// MARIA Zpt TEST RANGE
  fNJetSelect = -1; fMetMax = 500; fZPtMin = 4; fZPtMax = 50; 
  ///// process Type is used inside the checkPDF
  pType=processType;
  ///// fId is used inside the rapidity bin
  if(doYbinning) fId = iloop; 
  cout << " ==> looking at the production process " << pType << " rapidity Bin " << fId << endl;


  // THIS IS FOR FINE BINNING                                                                                                                                                
  if(MCtype==50) {

    //    for(int iloop=1; iloop < 21; iloop++) {                                                                                                                            

    cout << "PROCESSING DY +TOP MC " << endl;

    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");
    fTTbarFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/TTJets/ZTreeProducer_tree_RecoSkimmed.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fTTbarTree = (TTree*) fTTbarFile->FindObjectAny("ZTreeProducer");

    fData = true; 

    if(fData)  name+="_DATAlike";
    if(!fData) name+="_genZ";

    if(doChargedMet)  name+="_tkmet";
    if(!doChargedMet) name+="_pfmet";

    if(!doYbinning && fId == (-1.))  name+="_inclusiveNvtx";
    if(!doYbinning && fId != (-1.))  name+="_Nvtx";

    if(etaMuonSel==(1.6)) name+="_eta16";
    if(etaMuonSel==(1.1)) name+="_eta11";
    if(etaMuonSel==(2.1)) name+="_eta21";

    name+="_MZ81101";
    //    name+="_MZ70110";
    name+="_PDF";
    name += pType;
    if(doYbinning) name += "_rapBin";
    if(doYbinning) name += fId;
                                                                                                                                                            
    if(!usePol3) name += "_pol2";
    if(usePol3) name += "_pol3";

    if(doType2)  name += "_type2";
    if(!doType2) name += "_type1";

    if(!doSingleGauss) name += "_doubleGauss";
    if(doSingleGauss) name += "_oneGauss";

    name+="_x2Stat";
    if(doAbsolute) name+="_absolute";
    if(doIterativeMet) name+="_ITERATIVE";
    if(doSecondStep) name+="_second";

    name+="_MIXBKG.root";

    cout << name.Data() << endl;

    fitRecoilMIX(fDataTree,fTTbarTree,name.Data(), fId);


  }



  if(MCtype==1) {

    cout << "PROCESSING DY MC MADGRAPH -- inclusive Nvertex -- charged only " << endl;
    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fData = false; 

    if(fData)  name+="_DATA";
    if(!fData) name+="_genZ";

    if(doChargedMet)  name+="_tkmet";
    if(!doChargedMet) name+="_pfmet";

    if(!doYbinning && fId == (-1.))  name+="_inclusiveNvtx";
    if(!doYbinning && fId != (-1.))  name+="_Nvtx";

    if(etaMuonSel==(2.1)) name+="_eta21";
    if(etaMuonSel==(1.6)) name+="_eta16";
    if(etaMuonSel==(1.1)) name+="_eta11";

    name+="_MZ81101";
    //    name+="_MZ70110";
    name+="_PDF";
    name += pType;
    if(doYbinning) name += "_rapBin";
    if(doYbinning) name += fId;

    if(!usePol3) name += "_pol2";
    if(usePol3) name += "_pol3";

    if(doType2)  name += "_type2";
    if(!doType2) name += "_type1";

    if(!doSingleGauss) name += "_doubleGauss";
    if(doSingleGauss) name += "_oneGauss";

    name+="_x2Stat";
    if(doAbsolute) name+="_absolute";
    if(doIterativeMet) name+="_ITERATIVE";
    if(doSecondStep) name+="_second";

    if(doIterativeMet && !doOnlyU1 && doOnlyU2) name+="_applyOnlyU2";
    if(doIterativeMet && doOnlyU1 && !doOnlyU2) name+="_applyOnlyU1";

    if(fData)  name+=".root";
    if(!fData) name+="_madgraph.root";

    cout << name.Data() << endl;
    if(!doIterativeMet) fitRecoil(fDataTree,name.Data(),fId);

    if(doIterativeMet) {

      readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,/*lZMU23SigFit,*/"recoilfits/recoilfit_genZ_tkmet_eta21_MZ81101_PDF-1_rapBin1_pol2_type2_doubleGauss_x2Stat_madgraph.root" ,"PF",fId);
      readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,/*lZDU23SigFit,*/"recoilfits/recoilfit_DATA_tkmet_eta21_MZ81101_rapBin1_pol2_type2_doubleGauss_x2Stat.root" ,"PF",fId);

      cout << name.Data() << endl;
      fitRecoil(fDataTree,name.Data(),fId);

    }

    TFile f2("fileHistoZMC.root","RECREATE");
    histoU1vsZpt.Write();
    histoU1vsU2.Write();
    histoU1.Write();
    histoU1O.Write();
    histoU1Z.Write();
    histoU2.Write();
    f2.Write();
    f2.Close();
    
  }

  if(MCtype==2) {

    cout << "PROCESSING DY DATA -- inclusive Nvertex -- charged only " << endl;
    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DATA/ZTreeProducer_tree_RecoSkimmed.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fData = true; 

    if(fData)  name+="_DATA";
    if(!fData) name+="_genZ";

    if(doChargedMet)  name+="_tkmet";
    if(!doChargedMet) name+="_pfmet";

    if(!doYbinning && fId == (-1.))  name+="_inclusiveNvtx";
    if(!doYbinning && fId != (-1.))  name+="_Nvtx";

    if(etaMuonSel==2.1) name+="_eta21";
    if(etaMuonSel==1.6) name+="_eta16";
    if(etaMuonSel==1.1) name+="_eta11";

    name+="_MZ81101";
    //    name+="_MZ70110";

    if(doYbinning) name += "_rapBin";
    if(doYbinning) name += fId;

    if(!usePol3) name += "_pol2";
    if(usePol3) name += "_pol3";
    //////    name+="_pol2";

    if(doType2)  name+="_type2";
    if(!doType2) name+="_type1";

    if(!doSingleGauss) name += "_doubleGauss";
    if(doSingleGauss) name += "_oneGauss";

    name+="_x2Stat";
    if(doAbsolute) name+="_absolute";
    if(doIterativeMet) name+="_ITERATIVE";

    if(fData)  name+=".root";
    if(!fData) name+="_madgraph.root";

    cout << name.Data() << endl;

    fitRecoil(fDataTree,name.Data(), fId); 

  }


  if(MCtype==3) {

    //    etaMuonSel=1.1;
    //    pType=processType;

    doPosW=true;
    
    cout << "PROCESSING W po MC" << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree.root");
    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_03_04/WJetsLL/WTreeProducer_tree.root");    

    fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
    fData = false; 

    name+="_Wpos_tkmet";

    if(!doYbinning && fId == (-1.))  name+="_inclusiveNvtx";
    if(!doYbinning && fId != (-1.))  name+="_Nvtx";

    if(etaMuonSel==(1.1)) name+="_eta11";
    if(etaMuonSel==(1.6)) name+="_eta16";
    if(etaMuonSel==(2.1)) name+="_eta21";

    name+="_PDF";
    name+=pType;
    if(doYbinning) name += "_rapBin";
    if(doYbinning) name += fId;

    if(!usePol3) name += "_pol2";
    if(usePol3) name += "_pol3";

    if(doType2)  name+="_type2";
    if(!doType2) name+="_type1";

    if(!doSingleGauss) name += "_doubleGauss";
    if(doSingleGauss) name += "_oneGauss";

    name+="_x2Stat";
    if(doAbsolute) name+="_absolute";
    if(doIterativeMet) name+="_ITERATIVE";

    name+="_madgraph.root";

    cout << name.Data() << endl;
    fitRecoil(fDataTree, name.Data(), fId);
    

  }


  if(MCtype==4) {
    
    //    etaMuonSel=1.6;

    doNegW=true;
    
    cout << "PROCESSING W neg MC" << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
    // fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree.root");
    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_03_04/WJetsLL/WTreeProducer_tree.root");        

    fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
    fData = false; /*fId = -1; */
    if(doYbinning) fId = iloop;
    
    //    name+="BLABLAU2X1BLA";
    name+="_Wneg_tkmet";

    if(!doYbinning && fId == (-1.))  name+="_inclusiveNvtx";
    if(!doYbinning && fId != (-1.))  name+="_Nvtx";

    if(etaMuonSel==(1.1)) name+="_eta11";
    if(etaMuonSel==(1.6)) name+="_eta16";
    if(etaMuonSel==(2.1)) name+="_eta21";

    name+="_PDF";
    name+=pType;
    if(doYbinning) name += "_rapBin";
    if(doYbinning) name += fId;

    if(!usePol3) name += "_pol2";
    if(usePol3) name += "_pol3";

    if(doType2)  name+="_type2";
    if(!doType2) name+="_type1";

    if(!doSingleGauss) name += "_doubleGauss";
    if(doSingleGauss) name += "_oneGauss";

    name+="_x2Stat";
    if(doAbsolute) name+="_absolute";
    if(doIterativeMet) name+="_ITERATIVE";    

    name+="_madgraph.root";

    cout << name.Data() << endl;
    fitRecoil(fDataTree, name.Data(), fId);

  }


  //////
  //////
  //////
  //////
  ///////

  // THIS IS FOR FINE BINNING
  if(MCtype==100) {
    
    //    for(int iloop=1; iloop < 21; iloop++) {
      
      cout << "PROCESSING DY MC " << endl;
      
      //      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/DYJets/ZTreeProducer_tree_SignalRecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");

      fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
      fData = false; fId = iloop;
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_inc.root", fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_tkmet_type1_pol2_fixError_madgraph.root", fId);
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_pfmet_type1_pol2_fixError_madgraph.root", fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_tkmet_type1_pol2_fixError_madgraph.root", fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_tkmet_type1_pol3_fixError_madgraph.root", fId);
      if(!doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_pfmet_type2_pol2_fixError_eta16_madgraph.root", fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_tkmet_type1_pol2_fixError_eta16_madgraph.root", fId);
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_pfmet_type1_pol3_fixError_madgraph.root", fId);
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_pfmet_type1_pol2_fixError_madgraph.root", fId);
      //    }      

  }

  if(MCtype==200) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      cout << "PROCESSING DY DATA " << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      //      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
      fData = true; fId = iloop;
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_inc.root",fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_tkmet_type1_pol2_fixError.root", fId);
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_pfmet_type1_pol2_fixError.root", fId);

      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_tkmet_type1_pol2_fixError.root", fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_tkmet_type1_pol2_fixError_eta16.root", fId);
      if(!doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_pfmet_type2_pol2_fixError_eta16.root", fId);
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_pfmet_type1_pol3_fixError.root", fId);
      //    }      

  }

  if(MCtype==300) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      doPosW=true;

      cout << "PROCESSING W MC" << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      //      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
      fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
      fData = false; fId = iloop;
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_genWpos_inc.root",fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genWpos_tkmet_type1_pol2_fixError.root", fId);      
      if(!doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genWpos_pfmet_type2_pol2_fixError_eta16.root", fId);      
      //    }      

  }


  if(MCtype==400) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      doNegW=true;

      cout << "PROCESSING W MC" << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      //      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
      fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
      fData = false; fId = iloop;
      //      fitRecoil(fDataTree,"recoilfits/recoilfit_genWneg_inc.root",fId);
      //      if(doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genWpos_tkmet_type1_pol2_fixError.root", fId);      
      if(!doChargedMet )      fitRecoil(fDataTree,"recoilfits/recoilfit_genWneg_pfmet_type2_pol2_fixError_eta16.root", fId);      
      
      //    }      

  }


  // THIS IS FOR FINE BINNING                                                                                                                                   
  if(MCtype==500) {

    cout << "PROCESSING DY - applying iterative " << endl;

    fId = iloop;

    //    readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,"recoilfits/recoilfit_genZ_tkmet_pol2_type1_madgraph.root" ,"",fId);                                                                                                 

    //      readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,/*lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,/*lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,*/"recoilfits/recoilfit_genZ_tkmet_type1_pol3_fixError_madgraph.root" ,"PF",fId);
    //  readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,/*lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,/*lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU23SigFit,*/"recoilfits/recoilfit_DATA_tkmet_type1_pol3_fixError.root" ,"PF",fId);

    readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,/*lZMU23SigFit,*/"recoilfits/recoilfit_genZ_pfmet_type1_pol2_fixError_madgraph.root" ,"PF",fId,0);
    readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,/*lZDU23SigFit,*/"recoilfits/recoilfit_DATA_pfmet_type1_pol2_fixError.root" ,"PF",fId,0);
    
    //    readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,/*lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,/*lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,*/"recoilfits/recoilfit_genZ_tkmet_inclusiveNvtx_eta16_pol2_type1_madgraph.root" ,"PF",fId);
    //    readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,/*lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,/*lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU23SigFit,*/"recoilfits/recoilfit_genZ_tkmet_inclusiveNvtx_eta16_pol2_type1.root" ,"PF",fId);

    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");

    fData =false; fId = iloop;

    //    if(doIterativeMet && doChargedMet) fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_tkmet_type1_pol2_fixError_madgraph_ITERATIVE.root", fId);
    if(doIterativeMet) fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_pfmet_type1_pol2_fixError_madgraph_ITERATIVE.root", fId);

  }

  return;

}
