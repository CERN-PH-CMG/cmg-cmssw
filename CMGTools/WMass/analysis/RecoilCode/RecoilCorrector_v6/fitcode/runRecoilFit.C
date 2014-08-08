
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <sstream>

#include "TLine.h"
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
#include "TMultiGraph.h"
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
#include "Math/MinimizerOptions.h"
//#include "../../../AnalysisCode/rochcor_44X_v3.h"
#include "../../../AnalysisCode/rochcor_44X_v3.C"

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
TH1D histoU1Z("hu1scale","histo U1",100,-5.,5.);
TH1D histoU1("hu1","histo U1",100,-10,10);
TH1D histoU1diff("hu1diff","histo U1 - Zpt",100,-10,10);
TH2D histoU1vsZpt("hu1vsZpt","histo U1 vs Zpt",100,0,20,100,-10,10);
TH2D histoU1diffvsZpt("hu1diffvsZpt","histo (U1-Zpt) vs Zpt",100,0,20,100,-10,10);
TH2D histoU1scalevsZpt("hu1scalevsZpt","histo (U1/Zpt) vs Zpt",100,0,20,100,-10,10);
TH2D histoU1scalevsZptscale("hu1scalevsZptscale","histo (U1/Zpt) vs (Zptreco/Zpt)",100,-2,8,100,-10,10);
TH2D histoU1vsU2("hu1vsu2","histo U1 vs U2",100,-10,10,100,-10,10);
TH2D histoU1vsU2corrU("hu1vsu2corrU","histo U1 vs U2 after corrU",100,-10,10,100,-10,10);
TH2D histoU1vsU2corrUnorm("hu1vsu2corrUnorm","histo U1 vs U2 after corrU",100,-10,10,100,-10,10);
TH2D histoRecoil("hrecoil"," recoil vs recoilCorr",100,-20,20,100,-20,20);
TH2D histoU1vsU2_520("hu1vsu2520","",100,-10,10,100,-10,10);

TH1D histoDelta("hdelta","histo ",100,-95,5);

TH2D histoU1vsU2_02("hu1vsu2_02","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_24("hu1vsu2_24","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_46("hu1vsu2_46","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_68("hu1vsu2_68","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_810("hu1vsu2_810","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_1012("hu1vsu2_1012","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_1214("hu1vsu2_1214","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_1416("hu1vsu2_1416","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_1618("hu1vsu2_1618","",100,-10,10, 100,-10,10);
TH2D histoU1vsU2_1820("hu1vsu2_1820","",100,-10,10, 100,-10,10);

TH2D histoU1vsZptfit("hu1vsZptfit","histo U1 vsZpt",500,0,50, 2000,-100,100);

TH1D histoU2("hu2","histo U2",100,-10,10);

TH2D histoCorrU1("hCorrU1","",10,0,10, 10,0,10);
TH2D histoCorrU2("hCorrU2","",10,0,10, 10,0,10);

TH2D histoU1vsMuPt("histoU1vsMuPt","",100,0,100, 100,-50,50);
TH2D histoU2vsMuPt("histoU2vsMuPt","",100,0,100, 100,-50,50);

TH1D histoPhiStar("hPhiStar","histo PhiStar",1000,-10,10);
TH1D histoThetaStar("hThetaStar","histo ThetaStar",1000,-10,10);
TH2D histoPhiStarvsZpt("histoPhiStarvsZpt","histo PhiStar vs Zpt",100,0,20,100,-10,10);

/////

bool usePol3 = true;
bool dodebug = false; 

bool doType2 = true; //false for the fit to the scale and the rms only
bool doSingleGauss = false;

bool doPrint = false;
bool doPrintAll = true; // when this is set to true, do the binned

bool doBinned = false; // this is for the MET
bool doIterativeMet = false;

bool doApplyCorr = false; // apply correlation
bool doAbsolute = false;

bool doOnlyU1 = false;
bool doOnlyU2 = false;

int VTXbin=-1;

/////

bool do8TeV = false;
bool doMad = false;

bool doSecondStep = false; // un-needed
bool doOnlyU = false; // un-needed

bool doRoch_corr = false;

bool doNeutralMet=false;
bool doChargedMet=true;
bool donoPU=false;

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
double fU1 = -99999; double fU2 = -99999;
double fTKU1 = 0; double fTKU2 = 0;

/*float fZPt = 0; float fZPhi = 0; float fPhi = 0; */
double fZGenPt=0; double fZPt = 0; double fZPhi = 0; double fZRap = 0;// double fPhi = 0;
double fZrecoPt = 0;

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

rochcor_44X_v3 *muoncor44X ;

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

  /*
  if((iFrac*TMath::Erf(iVal/iSigma1) + (1-iFrac)*TMath::Erf(iVal/iSigma2))==1) {
  cout << "fabs(recoil_RAW)=" << iVal 
       << " iFrac=" << iFrac
       << " iSigma1=" << iSigma1
       << " iSigma2=" << iSigma2
       << endl;
  cout << "TMath::Erf(iVal/iSigma1) " << TMath::Erf(iVal/iSigma1) << endl;
  }
  */

  double lVal=iFrac*TMath::Erf(iVal/iSigma1) + (1-iFrac)*TMath::Erf(iVal/iSigma2);

  if(TMath::ErfInverse(lVal)==0) return iVal;

  return lVal;

}

double diGausPInverse(double iPVal,double iFrac,double iSigma1,double iSigma2) { 
  double lVal = TMath::ErfInverse(iPVal);

  /*
  if(TMath::ErfInverse(lVal)==0) {
    //    cout << "found ErfInverse 0 "<< endl;
    // there is a protection before
  }
  */

  double lMin = lVal * ((iSigma1 < iSigma2) ? iSigma1 : iSigma2);
  double lMax = lVal * ((iSigma1 < iSigma2) ? iSigma2 : iSigma1);
  //  cout << "-- Min - " << lMin <<  " -> " << lMax << " -- " << iSigma1 << " -- " << iSigma2 << endl;

  double lDiff = (lMax-lMin);
  //Iterative procedure to invert a double gaussian given a PVal
  ///  int lId = 0; int lN1 = 4;  int lN2 = 10; 
  //  int lId = 0; int lN1 = 10;  int lN2 = 10; 
  int lId = 0; int lN1 = 10;  int lN2 = 100; 
  for(int i0 = 0; i0 < lN1; i0++) { 
    if(i0 != 0) lMin = lMin + (lId-1)*lDiff/lN2;
    if(i0 != 0) lDiff/=lN2;
    for(int i1 = 0; i1 < lN2; i1++) { 
      double pVal = lMin + lDiff/lN2*i1;
      pVal = diGausPVal(pVal,iFrac,iSigma1,iSigma2);
      if(pVal==(lMin + lDiff/lN2*i1)) pVal=(lMin + lDiff/lN2*i1);
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

  //  if(iPVal==1) cout << "pVal " << pVal << endl;

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
  
  if(dodebug) cout << "===========================" << endl;
  if(dodebug) cout << "BEFORE CORRU Met=" << iMet << endl;

  double pDefU1    = iU1Default->Eval(iGenPt);
  double lRescale  = sqrt((TMath::Pi())/2.);     
  double pDU1       = iU1RZDatFit ->Eval(iGenPt);
  //double pDU2       = 0; sPM
  double pDFrac1    = iU1MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt)*pDFrac1;
  double pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt)*pDFrac1;
  //  if(doAbsolute) pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt)*lRescale;

  double pDFrac2    = iU2MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*pDFrac2;
  double pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*pDFrac2;
  //  if(doAbsolute) pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*lRescale;
  //double pDMean1    = pDFrac1;
  //double pDMean2    = pDFrac2;
 
  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  //  double pMU2       = 0; 
  double pMFrac1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt)*pMFrac1;
  double pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt)*pMFrac1;
  //  if(doAbsolute) pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt)*lRescale;

  double pMFrac2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt)*pMFrac2;
  double pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt)*pMFrac2;
  //  if(doAbsolute) pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt)*lRescale;

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

  bool scaleU2=true; 

  double normSigmaM = iU2MSZMCFit ->Eval(iGenPt)/iU1MSZMCFit ->Eval(iGenPt);
  if(!scaleU2) normSigmaM = iU1MSZMCFit ->Eval(iGenPt)/iU2MSZMCFit ->Eval(iGenPt);
  double normSigmaD = iU2MSZDatFit ->Eval(iGenPt)/iU1MSZDatFit ->Eval(iGenPt);
  if(!scaleU2) normSigmaD = iU1MSZDatFit ->Eval(iGenPt)/iU2MSZDatFit ->Eval(iGenPt);

  if(fData) normSigmaM = 1.;
  if(fData) normSigmaD = 1.;

  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-offset; // pU1 of the event ; pDefU1 is the scale iU1RZDatFit ->Eval(iGenPt);
  double pU2Diff  = pU2;

  pU1Diff*=normSigmaM; /// comments for now

  if(iGenPt<20) {
    histoU1vsZpt.Fill(iGenPt,pU1Diff);
    histoU1vsU2.Fill(pU1Diff,pU2Diff);
    histoU1vsU2_520.Fill(pU1Diff,pU2Diff);
    histoU1.Fill(pU1Diff);
    histoU1O.Fill(pU1);
    histoU1Z.Fill(pU1/iGenPt);
    histoU2.Fill(pU2Diff);
  }

  if(iGenPt<20 && iGenPt>5) histoU1vsU2_520.Fill(pU1Diff,pU2Diff);

  /*
  if(iGenPt>0 && iGenPt<2) histoU1vsU2_02.Fill(pU1Diff,pU2Diff);
  if(iGenPt>2 && iGenPt<4) histoU1vsU2_24.Fill(pU1Diff,pU2Diff);
  if(iGenPt>4 && iGenPt<6) histoU1vsU2_46.Fill(pU1Diff,pU2Diff);
  if(iGenPt>6 && iGenPt<8) histoU1vsU2_68.Fill(pU1Diff,pU2Diff);
  if(iGenPt>8 && iGenPt<10) histoU1vsU2_810.Fill(pU1Diff,pU2Diff);
  if(iGenPt>10 && iGenPt<12) histoU1vsU2_1012.Fill(pU1Diff,pU2Diff);
  if(iGenPt>12 && iGenPt<14) histoU1vsU2_1214.Fill(pU1Diff,pU2Diff);
  if(iGenPt>14 && iGenPt<16) histoU1vsU2_1416.Fill(pU1Diff,pU2Diff);
  if(iGenPt>16 && iGenPt<18) histoU1vsU2_1618.Fill(pU1Diff,pU2Diff);
  if(iGenPt>18 && iGenPt<20) histoU1vsU2_1820.Fill(pU1Diff,pU2Diff);
  */

  double recoil = sqrt(pU1Diff*pU1Diff+pU2Diff*pU2Diff); // this is scaled a bit

  if(dodebug) cout << "==> before smearing: Pt " << iGenPt << "; recoil(centered) " << recoil << "; pDefU1 " << pDefU1 << " offset: " << offset << " pU1Diff: " << pU1Diff << endl;

  /////
  /////
  /////

  //  double p1Charge        = pU1Diff/fabs(pU1Diff);
  //  double p2Charge        = pU2Diff/fabs(pU2Diff);
  /////  double pTU1Diff        = pU1Diff;
  
  //  cout << "------------"<< endl;
  //  cout << "diGAUSS "<< endl;
  //  cout << "  pU1ValM " << pU1ValM  << " pU1ValD " << pU1ValD  <<endl;
  //  cout << "  pU2ValM " << pU2ValM  << " pU2ValD " << pU2ValD  <<endl;

  //  double pU1ValM = 0;
  //  double pU2ValM = 0 ;
  double pU1ValD = 0 ;
  double pU2ValD = 0;
  double pUValM = 0;
  double pUValMtest = 0;
  double pUValD = 0 ;

  if(doSingleGauss) {

    if(scaleU2) pUValM         = diGausPVal(fabs(recoil),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
    if(scaleU2) pUValD         = oneGausPInverse(pUValM ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0);

    if(!scaleU2) pUValM         = diGausPVal(fabs(recoil),1,iU1MSZMCFit ->Eval(iGenPt)*lRescale,0);
    if(!scaleU2) pUValD         = oneGausPInverse(pUValM ,1,iU1MSZDatFit->Eval(iGenPt)*lRescale,0);

    //    if(pUValM==fabs(recoil)) pUValD=fabs(recoil); // in those cases do nothing  since Erf is zero

  } else {

    if(scaleU2) pUValM         = diGausPVal(fabs(recoil),pMFrac2,pMSigma2_1,pMSigma2_2);
    if(scaleU2) pUValD         = diGausPInverse(pUValM  ,pDFrac2,pDSigma2_1,pDSigma2_2);

    if(!scaleU2) pUValM         = diGausPVal(fabs(recoil),pMFrac1,pMSigma1_1,pMSigma1_2);
    if(!scaleU2) pUValD         = diGausPInverse(pUValM  ,pDFrac1,pDSigma1_1,pDSigma1_2);
    
    if(pUValM==fabs(recoil)) pUValD=fabs(recoil); // in those cases do nothing

    //    cout << "=======>>>>>>> " << endl;

    if(scaleU2) pUValMtest     = diGausPInverse(pUValM  ,pMFrac2,pMSigma2_1,pMSigma2_2);
    if(pUValM==fabs(recoil)) pUValMtest=fabs(recoil); // in those cases do nothing
    histoDelta.Fill(pUValMtest-fabs(recoil));

    //    if(iGenPt<20 && pUValMtest-fabs(recoil) < -10) cout << "=======>>>>>>> strange value output: " << pUValMtest << "; input " << fabs(recoil) << " frac2 " << pDFrac2 << endl;


  }


  if(iGenPt<20) histoRecoil.Fill(recoil,pUValD);
  if(iGenPt<20) histoU1vsU2corrU.Fill(pUValD*(pU1Diff/recoil),pUValD*(pU2Diff/recoil));

  pU1ValD=pUValD*(pU1Diff/recoil)*(1/normSigmaD);
  pU2ValD=pUValD*(pU2Diff/recoil);

  if(iGenPt<20) histoU1vsU2corrUnorm.Fill(pU1ValD,pU2ValD);

  if(iGenPt>0 && iGenPt<2) histoU1vsU2_02.Fill(pU1ValD,pU2ValD);
  if(iGenPt>2 && iGenPt<4) histoU1vsU2_24.Fill(pU1ValD,pU2ValD);
  if(iGenPt>4 && iGenPt<6) histoU1vsU2_46.Fill(pU1ValD,pU2ValD);
  if(iGenPt>6 && iGenPt<8) histoU1vsU2_68.Fill(pU1ValD,pU2ValD);
  if(iGenPt>8 && iGenPt<10) histoU1vsU2_810.Fill(pU1ValD,pU2ValD);
  if(iGenPt>10 && iGenPt<12) histoU1vsU2_1012.Fill(pU1ValD,pU2ValD);
  if(iGenPt>12 && iGenPt<14) histoU1vsU2_1214.Fill(pU1ValD,pU2ValD);
  if(iGenPt>14 && iGenPt<16) histoU1vsU2_1416.Fill(pU1ValD,pU2ValD);
  if(iGenPt>16 && iGenPt<18) histoU1vsU2_1618.Fill(pU1ValD,pU2ValD);
  if(iGenPt>18 && iGenPt<20) histoU1vsU2_1820.Fill(pU1ValD,pU2ValD);

  if(dodebug) cout << " ==> after smearing: Pt " << iGenPt << "; recoil(centered) " << pUValD << " ; pU1 " << pU1 << " pDefU1+pU1ValD " << pDefU1+pU1ValD << "; pU2 " << pU2 << " pU2ValD " << pU2ValD << endl;

  // those are not needed since I multiply above
  //  pU1ValD*=p1Charge;
  //  pU2ValD*=p2Charge;

  pDefU1 *= (pDU1/pMU1);

  //  double iMet_noCorr  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  //  double iMPhi_noCorr = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  //  if(dodebug) cout << "AFTER CORRU Met_noCorr=" << iMet_noCorr ; 

  pU1   = pDefU1       +       pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  //  iU1   = pU1; 
  //  iU2   = pU2;

  if(dodebug) cout << " Met=" << iMet << endl;
  if(dodebug) cout << "===========================" << endl;

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
  //  if(doAbsolute) pDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt)*lRescale;

  double pDFrac2    = iU2MSZDatFit->Eval(iGenPt)*lRescale;
  double pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*pDFrac2;
  double pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*pDFrac2;
  //  if(doAbsolute) pDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*lRescale;
  //double pDMean1    = pDFrac1;
  //double pDMean2    = pDFrac2;
 
  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  //  double pMU2       = 0; 
  double pMFrac1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt)*pMFrac1;
  double pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt)*pMFrac1;
  //  if(doAbsolute) pMSigma1_1 = iU1S1ZMCFit ->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pMSigma1_2 = iU1S2ZMCFit ->Eval(iGenPt)*lRescale;

  double pMFrac2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt)*pMFrac2;
  double pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt)*pMFrac2;
  //  if(doAbsolute) pMSigma2_1 = iU2S1ZMCFit ->Eval(iGenPt)*lRescale;
  //  if(doAbsolute) pMSigma2_2 = iU2S2ZMCFit ->Eval(iGenPt)*lRescale;

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
  /////  double pTU1Diff        = pU1Diff;
  
  //  cout << "------------"<< endl;
  //  cout << "diGAUSS "<< endl;
  //  cout << "  pU1ValM " << pU1ValM  << " pU1ValD " << pU1ValD  <<endl;
  //  cout << "  pU2ValM " << pU2ValM  << " pU2ValD " << pU2ValD  <<endl;

  double pU1ValM = 0;
  double pU2ValM = 0 ;
  double pU1ValD = 0 ;
  double pU2ValD = 0;

  //  pU2ValM         = diGausPVal(fabs(pU2Diff),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
  //  pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0); 
  
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

    //    if(pU1ValD==0) pU1ValD=fabs(pU1Diff);
    //    if(pU2ValD==0) pU2ValD=fabs(pU2Diff);
    if(pU1ValM==fabs(pU1Diff)) pU1ValD=fabs(pU1Diff); // in those cases do nothing  since Erf is zero
    if(pU2ValM==fabs(pU2Diff)) pU2ValD=fabs(pU2Diff); // in those cases do nothing  since Erf is zero

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

    if(pU1ValM==fabs(pU1Diff)) pU1ValD=fabs(pU1Diff); // in those cases do nothing
    if(pU2ValM==fabs(pU2Diff)) pU1ValD=fabs(pU1Diff); // in those cases do nothing

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

double round_fn(double pre_round, int round_dig)
{
  pre_round *= pow(10.,round_dig);
  int the_floor = (int) TMath::Floor(pre_round);
  double remainder = pre_round - the_floor;
  if(remainder >= 0.5) the_floor++;
  pre_round = the_floor / pow(10.,round_dig);
  return pre_round;
}

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

  return 0;

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


  iTree->SetBranchAddress("run" ,&fRun);
  iTree->SetBranchAddress("lumi" ,&fLumi);
  iTree->SetBranchAddress("evt" ,&fEvent);
  iTree->SetBranchAddress("nvtx"   ,&fNPV);
  //iTree->SetBranchAddress("Weight",&fWeight); // this is set to 1 fot now

  iTree->SetBranchAddress("pt_vis"  ,&fPt1);
  iTree->SetBranchAddress("phi_vis" ,&fPhi1);

  if((!doPosW) && (!doNegW)) {

    if(!fData) iTree->SetBranchAddress("ZGen_pt" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("ZGen_phi",&fZPhi);
    if(!fData) iTree->SetBranchAddress("ZGen_rap",&fZRap);

    //    iTree->SetBranchAddress("ZGen_pt" ,&fZGenPt);   
    if(!fData)  iTree->SetBranchAddress("Z_pt" ,&fZrecoPt);


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
    //    iTree->SetBranchAddress("trku1" ,&fU1);
    //    iTree->SetBranchAddress("trku2" ,&fU2);
    iTree->SetBranchAddress("trkmet",&fMet); 
  } else if(type==2) {
    //iTree->SetBranchAddress("trku1" ,&fTKU1);
    //iTree->SetBranchAddress("trku2" ,&fTKU2);
    // MARIA: I'm using those now
    //    iTree->SetBranchAddress("u1"  ,&fPFU1);
    //    iTree->SetBranchAddress("u2"  ,&fPFU2);
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
    iTree->SetBranchAddress("MuPos_phi" ,&fMuPos_phi);
    iTree->SetBranchAddress("MuPos_charge" ,&fMuPos_charge);
    iTree->SetBranchAddress("MuPosRelIso" ,&fMuPosReliso);
    iTree->SetBranchAddress("MuPos_dxy" ,&fMuPos_dxy);
      
    iTree->SetBranchAddress("MuNeg_eta" ,&fMuNeg_eta);
    iTree->SetBranchAddress("MuNeg_pt" ,&fMuNeg_pt);
    iTree->SetBranchAddress("MuNeg_phi" ,&fMuNeg_phi);
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

  // reset U1 U2
  fU1 = -9999;
  fU2 = -9999;

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

double getPtMuonCorrrection(double pt, double eta, double phi, double charge) {

  if(pt>80) return pt;

  TFile *kalmanfile_ = TFile::Open("../../../utils/kalmanCalibration_data.root");
  TFile *bfile_ = TFile::Open("../../../utils/mapCalibration.root");

  TH1D * h_ = (TH1D*) bfile_->Get("mapCorrection");
  TH1D * hA = (TH1D*) kalmanfile_->Get("A_0");
  TH1D * hM = (TH1D*) kalmanfile_->Get("M_0");
  TH1D * hB = (TH1D*) kalmanfile_->Get("B_0");

  int mag_bin = h_->GetBin(h_->GetXaxis()->FindBin(phi),h_->GetYaxis()->FindBin(eta));
  double newCurv = h_->GetBinContent(mag_bin)/pt;

  int kalman_bin = hA->GetBinContent(hA->GetXaxis()->FindBin(newCurv),hA->GetYaxis()->FindBin(eta),hA->GetZaxis()->FindBin(phi));

  double A = hA->GetBinContent(kalman_bin);
  double M = hM->GetBinContent(kalman_bin);
  double B = hB->GetBinContent(kalman_bin);

  newCurv = (A-1)*newCurv + newCurv/(1+M*newCurv) + charge*B; 

  return 1.0/newCurv;

}


double calculateSinThetaStar(double eta1, double eta2) {
  return TMath::Sqrt(1-TMath::Power(TMath::TanH((eta1-eta2)/2),2));
}

double calculateCosThetaStar(double eta1, double eta2) {
  return TMath::TanH((eta1-eta2)/2);
}

double calculatePhiStar(double SinThetaStar, double fMuPos_eta, double fMuNeg_eta, double fMuPos_phi, double fMuNeg_phi){
  double PhiStar;
  double Phi_acop;

  if((fMuPos_phi-fMuNeg_phi)<TMath::Pi()) Phi_acop=TMath::Pi()-fMuPos_phi-fMuNeg_phi;
  else Phi_acop=TMath::Pi()-fMuNeg_phi-fMuPos_phi;

  return PhiStar=(TMath::Tan(Phi_acop/2))*SinThetaStar;
}

bool runSelection() {
  
  //  cout << "inside Z selction " << etaMuonSel << endl;  
  //   double etaMuon=0.6;  
  
  double etaMuon=etaMuonSel;
  //  double muonPt=30*(90./80);
  double muonPt=30.;
  //   cout << "etaMuon "<< etaMuon << endl;
  
  ////////                      
  
  bool LeadingPos=(TMath::Abs(fMuPos_eta)<etaMuon && fMuPos_pt>muonPt && fMuPosTrg
		   && TMath::Abs(fMuNeg_eta)<2.4 && fMuNeg_pt>10
		   && fMuPosIsTight
		   && fMuPosReliso<0.12
		   && fabs(fMuPos_dxy)<0.02
		   //		   && fabs(fMuPos_dz)<0.1
		   && fMuNegIsTight
		   //              && fMuNegIsTightAndIso // relax iso on the second muon
		   //              && fMuNegReliso<0.12   // relax iso on the second muon
		   && fabs(fMuNeg_dxy)<0.02
		   //                   && fabs(fMuNeg_dz)<0.1
		   );
  
  bool LeadingNeg=(TMath::Abs(fMuNeg_eta)<etaMuon && fMuNeg_pt>muonPt && fMuNegTrg
		   && TMath::Abs(fMuPos_eta)<2.4 && fMuPos_pt>10
		   //     && fMuNegIsTightAndIso
		   && fMuNegIsTight
		   && fMuNegReliso<0.12
		   && fabs(fMuNeg_dxy)<0.02
		   //		   && fabs(fMuNeg_dz)<0.1
		   && fMuPosIsTight
		   //     && fMuPosIsTightAndIso // relax iso on the second muon
		   //              && fMuPosReliso<0.12   // relax iso on the second muon 
		   && fabs(fMuPos_dxy)<0.02
		   //		   && fabs(fMuPos_dz)<0.1
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
      /////////
      /// FILLING HISTOGRAMS
      
      histoU1vsZpt.Fill(fZPt,fU1);
      histoU1diffvsZpt.Fill(fZPt,fU1+fZPt);
      histoU1scalevsZpt.Fill(fZPt,fU1/fZPt);
      histoU1scalevsZptscale.Fill(fZrecoPt/fZPt,fU1/fZPt);
      histoU1Z.Fill(fU1/fZPt);
      histoU1vsU2.Fill(fU1,fU2);
      
      histoU1vsMuPt.Fill(fMuNeg_pt,fU1);
      histoU2vsMuPt.Fill(fMuPos_pt,fU2);
   
      histoU1.Fill(fU1);
      histoU1diff.Fill(fU1+fZPt);
      histoU2.Fill(fU2);
   

      double SinThetaStar=calculateSinThetaStar(fMuPos_eta, fMuNeg_eta);
      double CosThetaStar=calculateCosThetaStar(fMuPos_eta, fMuNeg_eta);

      double PhiStar=calculatePhiStar(SinThetaStar, fMuPos_eta, fMuNeg_eta, fMuPos_phi, fMuNeg_phi);
      histoPhiStarvsZpt.Fill(fZPt,PhiStar);
      histoPhiStar.Fill(PhiStar);
      histoThetaStar.Fill(CosThetaStar);

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


   // this is needed to initialize the recoil eventually
   double pippo=0;
   calculateU1U2(pippo, true );
   float recoil=sqrt(fU1*fU1 + fU2*fU2);
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
      ) {
     
     /////////
     /// FILLING HISTOGRAMS
     
     histoU1vsZpt.Fill(fZPt,fU1);
     histoU1vsU2.Fill(fU1,fU2);
     
     histoU1diffvsZpt.Fill(fZPt,fU1+fPt1);
     histoU1vsMuPt.Fill(fMuPos_pt,fU2);

     histoU1diff.Fill(fU1+fPt1);     
     histoU1.Fill(fU1);
     histoU2.Fill(fU2);
     
     return true;
     
   }
   
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

    //    cout << "typeBoson " << typeBoson << " doPosW " << doPosW << endl;
    //    cout << "pType " << " fpdgId1 " << fpdgId1 <<  " fpdgId2 " << fpdgId2 << endl;

    // THOSE are for the W+
    if( pType==1 && ((fpdgId1==2 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==2))) return true; //cout << " u dbar " << endl;
    if( pType==2 && (
	((fpdgId1==2 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==2)) ||
	((fpdgId1==2 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==2)) ||
	((fpdgId1==4 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==4)) ||
	((fpdgId1==4 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==4)) ||
	((fpdgId1==4 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==4))
	)) return true; //cout << " u sbar " << endl;
    /*
    if( pType==3 && ((fpdgId1==2 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==2))) return true; //cout << " u bbar " << endl;
    if( pType==4 && ((fpdgId1==4 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==4))) return true; //cout << " c dbar " << endl;
    if( pType==5 && ((fpdgId1==4 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==4))) return true; //cout << " c sbar " << endl;
    if( pType==6 && ((fpdgId1==4 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==4))) return true; //cout << " c bbar " << endl;
    */
    if( pType==0 && (fpdgId1==0 || fpdgId2==0) ) return true; //cout << "gluon + X "<< endl;

  }
  
  if(typeBoson==(-24)) {
    // THOSE are for the W-
    if( pType==1 && ((fpdgId1==1 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==1))) return true; //cout << " d ubar " << endl;
    if( pType==2 && (
	((fpdgId1==1 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==1)) ||
	((fpdgId1==3 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==3)) || 
	((fpdgId1==3 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==3)) ||
        ((fpdgId1==5 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==5)) ||
        ((fpdgId1==5 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==5))
	)) return true; //cout << " b cbar " << endl;
    if( pType==0 && (fpdgId1==0 || fpdgId2==0))  return true; // cout << "gluon + X "<< endl; return true;
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
    if(lId == 2) {if(fNPV < 6 || fNPV  > 10) {return false; } {return true;}}
     if(lId == 3) {if(fNPV < 11             ) {return false; } {return true;}}
     //    if(lId == 0) {if(fNPV > 12             ) {return false; } {return true;}}
     //    if(lId == 1) {if(fNPV < 3 || fNPV > 5              ) {return false; } {return true;}}

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
  
  // those are for MINOS
  double err00=iFPtr->UpperError(0)*iFPtr->UpperError(0);
  double err10=iFPtr->UpperError(1)*iFPtr->UpperError(0);
  double err01=iFPtr->UpperError(0)*iFPtr->UpperError(1);
  double err11=iFPtr->UpperError(1)*iFPtr->UpperError(1);
  
  double err02=0;double err12=0;double err22=0;
  double err03=0;double err13=0;double err23=0;double err33=0;
  
  if(lPol2) {
    err02=iFPtr->UpperError(0)*iFPtr->UpperError(2);
    err12=iFPtr->UpperError(1)*iFPtr->UpperError(2);
    err22=iFPtr->UpperError(2)*iFPtr->UpperError(2);
  }
  
  if(lPol3) {
    err03=iFPtr->UpperError(0)*iFPtr->UpperError(3);
    err13=iFPtr->UpperError(1)*iFPtr->UpperError(3);
    err23=iFPtr->UpperError(2)*iFPtr->UpperError(3);
    err33=iFPtr->UpperError(3)*iFPtr->UpperError(3);
  }
  
  /*
    TMatrixDSym lCov = iFPtr->GetCovarianceMatrix();
    
    double err00=(lCov)(0,0); double err01=(lCov)(0,1); double err10=(lCov)(1,0); double err11=(lCov)(1,1);
    double err02=(lCov)(0,2); double err12=(lCov)(1,2); double err22=(lCov)(2,2);
    double err03=(lCov)(0,3); double err13=(lCov)(1,3); double err23=(lCov)(2,3); double err33=(lCov)(3,3);
  */
  

  double lE0 = err00;
  double lE1 = err01 + err10;
  double lE2 = err11;
  double lE3 = 0; if(lPol2) lE3 = 2*err12;
  double lE4 = 0; if(lPol2) lE4 = err22;   //This scheme preserves the diagaonals 
  
  if(lPol2 || lPol3) lE2 += 2*err02;
  
  double lE5 = 0;
  double lE6 = 0;
  if(lPol3) {
    lE3 += 2*err03;
    lE4 += 2*err13;
    lE5  = 2*err23;
    lE6  = err33;
  }
  
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

  // this is the range of th X axis
  //  double minRangeSigma=-10.; double maxRangeSigma=10.;
  double minRangeSigma=-15.; double maxRangeSigma=15.;
  if(doAbsolute) { minRangeSigma=-50.; maxRangeSigma=50.;}

  // this is the mean
  double minMean=-1.; double maxMean=1.;
  if(doAbsolute) { minMean=-1.; maxMean=1.; }

  // this is the p0 value of the sigma
  // should not be negative and the
  double startSigma1=0.6; 
  double minSigma1=0.2; double maxSigma1=1.2;
  if(fId==201) maxSigma1=1.5;
  double startSigma2=1.5; 
  double minSigma2=1.2; double maxSigma2=5.;
  if(doAbsolute) { startSigma1=3*0.5;  startSigma2=3*1.2; }
  if(doAbsolute) { minSigma1=0.;  maxSigma1=10.;}
  if(doAbsolute) { minSigma2=3.;  maxSigma2=40.;}

  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,minRangeSigma,maxRangeSigma);
  if(lPar!=fU1) lRXVar.SetTitle("(U_{2}(Z_{p_{T}})-x_{i})/#sigma_{U2} (Z_{p_{T}})");
  if(doAbsolute && lPar==fU1) lRXVar.SetTitle("U_{1}(Z_{p_{T}})-x_{i}");
  if(doAbsolute && lPar!=fU1) lRXVar.SetTitle("U_{2}(Z_{p_{T}})-x_{i}");

  // this for the 2D Fit (is a cubic function to take into account the pt dependence)
  ////// those are the 2D histograms     
  // ===> Those are the variable fitted  
  // 2Dparam: lFrac, lMean, l1Sigma, l2Sigma  
  // fit those: lA1Sig,lB1Sig,lC1Sig,lA2Sig,lB2Sig,lC2Sig,
  // those are set constant (0) lD1Sig, lD2Sig, lR1Mean  

  double SF=1;  
  if(doAbsolute) SF=20;

  // powheg ===> U2 sigma2 need to be pol1 while the other pol2 
  // madgraph + DATI ===> pol2 

  double B1init= 0.0;
  double C1init= 0.0; 
  double B2init= 0.0;
  double C2init= 0.0; 

  /// setting for POWHEG should be ok also for madgraph
  if(!fData && lPar==fU1) B1init = 0.02; // powheg U1
  if(!fData && lPar==fU1) C1init = -0.0003; // powheg U1
  if(!fData && lPar==fU1) B2init = 0.005; // powheg U1
  if(!fData && lPar==fU1) C2init = 0.0001; // powheg U1

  RooRealVar    lA1Sig("a1sig","a1sig", startSigma1, minSigma1, maxSigma1 );
  RooRealVar    lB1Sig("b1sig","b1sig", B1init , -0.1*SF, 0.1*SF);  
  RooRealVar    lC1Sig("c1sig","c1sig", C1init , -0.01*SF, 0.01*SF);  // lC1Sig.setConstant(kTRUE);
  RooRealVar    lD1Sig("d1sig","d1sig",0. , -0.1*SF, 0.1*SF);  lD1Sig.setConstant(kTRUE);

  RooRealVar    lA2Sig("a2sig","a2sig", startSigma2, minSigma2, maxSigma2 );
  RooRealVar    lB2Sig("b2sig","b2sig", B2init ,-0.1*SF, 0.1*SF);
  RooRealVar    lC2Sig("c2sig","c2sig", C2init ,-0.01*SF, 0.01*SF);  
  if(!doMad && !fData && lPar!=fU1) lC2Sig.setConstant(kTRUE);  // POWHEG guessed from Z
  RooRealVar    lD2Sig("d2sig","d2sig",0.0 ,-0.1*SF, 0.1*SF);  lD2Sig.setConstant(kTRUE);

  /*
    /// setting for MADGRAPH
    if(doMad && !fData && lPar==fU1) B2init = 0.05; // powheg U1
    if(doMad && !fData && lPar!=fU1) B2init = 0.02; // powheg U2
    if(doMad && !fData && (lPar==fU1 || lPar!=fU1)) C2init = -0.0005; // powheg U1,U2
    
    /// setting for POWHEG
    if(!doMad && !fData && lPar==fU1) B2init = -0.006; // powheg U1
    if(!doMad && !fData && lPar!=fU1) C2init = 0.0; // powheg U2 // this will be a constant later
    if(!doMad && !fData && lPar==fU1) C2init = 0.0006; // powheg U1
    
    /// setting for DATA
    if(fData && lPar==fU1) B2init = 0.05; // data U1 
    if(fData && lPar==fU1) C2init = 0.0009; // data U1 
    //  if(doMad && !fData ) B2init = 0.035; // madgraph
    
    RooRealVar    lA2Sig("a2sig","a2sig",startSigma2, minSigma2, maxSigma2 );
    RooRealVar    lB2Sig("b2sig","b2sig",B2init ,-0.1*SF, 0.1*SF);
    RooRealVar    lC2Sig("c2sig","c2sig",C2init ,-0.01*SF, 0.01*SF);  
    if(!doMad && !fData && lPar!=fU1) lC2Sig.setConstant(kTRUE);  // POWHEG guessed from Z
    RooRealVar    lD2Sig("d2sig","d2sig",0.0 ,-0.1*SF, 0.1*SF);  lD2Sig.setConstant(kTRUE);
    
  */

  //////
  RooRealVar    lA1Mean("A1mean","A1mean",0,minMean,maxMean); lA1Mean.setConstant(kTRUE);
  RooRealVar    lA2Mean("A2mean","A2mean",0,-0.1*SF,0.1*SF);  lA2Mean.setConstant(kTRUE);
  RooRealVar    lA3Mean("A3mean","A3mean",0,-0.1*SF,0.1*SF);  lA3Mean.setConstant(kTRUE); 
  RooRealVar    lMean("mean","mean",0,minMean,maxMean);  lMean.setConstant(kTRUE);
  // RooFormulaVar lMean ("mean"  ,"@0+@1*@3+@2*@3*@3",RooArgSet(lA1Mean,lA2Mean,lA3Mean,lRPt));

  RooRealVar    lA1Mean1("A1mean1","A1mean1",0,minMean,maxMean); lA1Mean1.setConstant(kTRUE);
  RooRealVar    lA2Mean1("A2mean1","A2mean1",0,-0.1*SF,0.1*SF);  lA2Mean1.setConstant(kTRUE);
  RooRealVar    lA3Mean1("A3mean1","A3mean1",0,-0.1*SF,0.1*SF);  lA3Mean1.setConstant(kTRUE); 
  RooRealVar    lMean1("mean1","mean1",0,minMean,maxMean);   lMean1.setConstant(kTRUE);
  //  RooFormulaVar lMean1 ("mean"  ,"@0+@1*@3+@2*@3*@3",RooArgSet(lA1Mean1,lA2Mean1,lA3Mean1,lRPt));
  
  // ===> This is the model for the 2D          
  RooFormulaVar l1Sigma("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
  RooFormulaVar l2Sigma("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
  RooFormulaVar lFrac  ("frac"  ,"(@1-1)/(@1-@0)",RooArgSet(l1Sigma,l2Sigma));
  RooGaussian   lGaus1("gaus1","gaus1",lRXVar,lMean,l1Sigma);
  RooGaussian   lGaus2("gaus2","gaus2",lRXVar,lMean,l2Sigma);
  RooAddPdf     lGAdd("Add","Add",lGaus1,lGaus2,lFrac);

  ////// those are the 1D histograms     
  // ===> Those are the variable fitted 
  // 1Dparam: lR1Frac, lR1Mean, lR1sigma, lR2sigma  
  RooRealVar    lR1Sigma("Rsigma1","RSigma1",startSigma1, minSigma1, maxSigma1 );
  RooRealVar    lR2Sigma("Rsigma2","RSigma2",startSigma2, minSigma2, maxSigma2 );
  RooFormulaVar lR1Frac  ("Rfrac"  ,"(@1-1.)/(@1-@0)",RooArgSet(lR1Sigma,lR2Sigma));
  //  RooRealVar    lR1Frac ("Rfrac"  ,"Rfrac"  , 0.6 ,0, 1.); // free parameter
  RooRealVar    lR1Mean ("Rmean",  "Rmean",0,minMean,maxMean); // lR1Mean.setConstant(kTRUE); 
  
  // ===> This is the model for the 1D  
  RooGaussian   lRGaus1("Rgaus1","Rgaus1",lRXVar,lR1Mean,lR1Sigma);
  RooGaussian   lRGaus2("Rgaus2","Rgaus2",lRXVar,lR1Mean,lR2Sigma);
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
  
  TString leg1 = "";  
  if(fId == 1) leg1 += " |Y(V)| < 1";
  if(fId == 125) leg1 += " 1 < |Y(V)| < 1.25";
  if(fId == 150) leg1 += " 1.25 < |Y(V)| < 1.50";
  if(fId == 175) leg1 += " 1.50 < |Y(V)| < 1.75";
  if(fId == 200) leg1 += " 1.75 < |Y(V)| < 2.";
  if(fId == 201) leg1 += " |Y(V)| > 2.";

  TString leg3 = "NVtx ";  
  if(VTXbin==1) leg3 += " = 0-5";
  if(VTXbin==2) leg3 += " = 5-10";
  if(VTXbin==3) leg3 += " = >10";

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


  double tolerance = ROOT::Math::MinimizerOptions::DefaultTolerance();
  string algo = ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo();
  string type = ROOT::Math::MinimizerOptions::DefaultMinimizerType();
  int strategy= ROOT::Math::MinimizerOptions::DefaultStrategy();
  
  cout << "DEFAULTS: algo " << algo.c_str() << " type " << type.c_str() << " tolerance " << tolerance << " strategy " << strategy << endl;
    
  string minimizerType = "Minuit2";
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer(minimizerType.c_str());
  string type2 = ROOT::Math::MinimizerOptions::DefaultMinimizerType();

  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);

  cout << "CHANGED TO: algo " << algo.c_str() << " type " << type2.c_str() << " tolerance " << ROOT::Math::MinimizerOptions::DefaultTolerance() << " strategy " << ROOT::Math::MinimizerOptions::DefaultStrategy() << endl;

  ////
  ////

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
  if(!iRMS && lPar==fU1) lFit->FixParameter(0,0);
  //  if(!iRMS && lPar==fU1) lFit->SetParLimits(0, 0.0, 1.0); // first parameter between 0 and 1.0
  //  if(!iRMS && lPar==fU1) lFit->SetParLimits(0, -0.5, 1.0); // first parameter between 0 and 1.0

  //  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SR","EXO",fZPtMin,fZPtMax); //"EXO"
  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SRE","",fZPtMin,fZPtMax); 

  //  only once
  computeFitErrors(iFit,lFitPtr,lFit,iRMS);


  /*
  // twice only for the U1scale
  TF1 *lFit2 = new TF1("test",lType.c_str()); //fZPtMax = 200;                                                                                                                               
  if(!iRMS && lPar==fU1) {
    lFit2->SetParameter(1,lFit->GetParameter(1));
    if(usePol3) lFit2->SetParameter(2,lFit->GetParameter(2));
    if(usePol3) lFit2->SetParameter(3,lFit->GetParameter(3));
    TFitResultPtr  lFitPtr2 = pGraphA->Fit(lFit2,"SRE","",fZPtMin,fZPtMax); 
    computeFitErrors(iFit,lFitPtr2,lFit2,iRMS);
  } else {
    computeFitErrors(iFit,lFitPtr,lFit,iRMS);
  }
  */

  cout << "============"<< endl;
  cout << "============"<< endl;
  if(iRMS)cout << "=== RMS ==="<< endl;
  if(!iRMS)cout << "=== scale ==="<< endl;
  if(lPar==fU1) cout << "=== U1 ==="<< endl;
  if(lPar!=fU1) cout << "=== U2 ==="<< endl;
  cout << "============"<< endl;
  cout << "============"<< endl;

  if(doPrint) {

    pGraphA->Draw("ap");
    drawErrorBands(iFit,fZPtMax);
    
    Double_t chi2   = lFitPtr->Chi2(); // to retrieve the fit chi2 
                                                                                                                      
    int ndof = lFitPtr->Ndf();
    if(lPar==fU1 && !iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U1 mean " << endl;
    if(lPar==fU1 && iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U1 RMS " << endl;
    if(!lPar==fU1 && !iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U2 mean " << endl;
    if(!lPar==fU1 && iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U2 RMS " << endl;

    if(lPar==fU1) pGraphA->GetYaxis()->SetTitle("U1"); 
    if(lPar!=fU1) pGraphA->GetYaxis()->SetTitle("U2"); 
    pGraphA->GetXaxis()->SetTitle("Z p_{T}"); 

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.7, leg3);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);

    TString fileName2D="fileGraph_Unbinned_meanRMS_";
    if(!fData && (!doPosW && doNegW)) fileName2D += "Wneg";
    if(!fData && (doPosW && !doNegW)) fileName2D += "Wpos";
    if(!fData && (!doPosW && !doNegW)) fileName2D += "Z";
    if(fData) fileName2D += "DATA";
    if(!fData) fileName2D += "MC"; 
    if(doMad && !fData) fileName2D += "_MADGRAPH";
    if(!doMad && !fData) fileName2D += "_POWHEG";
    fileName2D += ".root";

    TFile f1(fileName2D,"UPDATE");

    TString graphName="";
    if(!iRMS) graphName += "mean";
    if(iRMS) graphName += "RMS";
    if(lPar==fU1) graphName += "_U1";
    if(lPar!=fU1) graphName += "_U2";
    graphName += "_y";
    graphName += fId;

    pGraphA->SetName(graphName.Data());
    pGraphA->Write();
    f1.Write();
    f1.Close();

    /*
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
    test += VTXbin;
    test += ".png";
    

    iC->SaveAs(test.Data());
    */

  }

  if(!doType2) return;
  if(!iRMS) return;
  if(doIterativeMet) return;

  //Build the double gaussian dataset
  std::vector<RooDataSet> lResidVals; 
  std::vector<RooDataHist> lResidVals2D; 
  std::stringstream pSS; pSS << "Crapsky" << 0;
  pSS << "2D"; RooDataHist lData2D(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar,lRPt));
  //  int lNBins = 10;
  //  int lNBins = 25;
  ///////  int lNBins = 50;
  int lNBins = fZPtMax;
  //  int lNBins = fZPtMax-fZPtMin; // since the unbnned fit is done up to fZPtMax we can also do the binedFit up to fZPtMax and such that we have one bin for each GeV
  for(int i0  = 0; i0 < lNBins; i0++) { 
    RooDataSet lData(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar)); 
    //    lRPt.setBins(500);
    //    lRPt.setBins(fZPtMax);
    //    lRPt.setBins(lNBins);
    lResidVals.push_back(lData); 
  }
  //  lRPt.setBins(lNBins*50);
  lRPt.setBins(10000);
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

  cout << "============" << endl;
  cout << "============" << endl;
  cout << "=== Unbinned Fit: doing now ===" << endl;
  cout << "============" << endl;
  cout << "============" << endl;

  /// swith off Minos to speed up 
  //  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos());//,Minos()); //Double Gaussian fit
  //  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos()); //Double Gaussian fit
  // other options inside
  // http://root.cern.ch/root/html/RooAbsPdf.html#RooAbsPdf:fitTo
  ////  options Extended(kTRUE) doens't work
  lGAdd.mustBeExtended();
  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Constrained(),Warnings(kTRUE),Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),/*Minimizer("Minuit2","migrad"),*/Strategy(2),Minos());//,Minos()); //Double Gaussian fit


  // cout << "====================" << endl;
  // cout << "== FILL histograms from dataset and fitted PDF
  // cout << "====================" << endl;

  TString nameHisto="hh_U1";
  if(lPar!=fU1) nameHisto="hh_U2";
  nameHisto  += "_";
  nameHisto += fId;
  
  TString nameHistoFitG1=nameHisto.Data();
  nameHistoFitG1 += "_FitG1";
  TString nameHistoFitG2=nameHisto.Data();
  nameHistoFitG2 += "_FitG2";

  // HISTOs from dataset
  TH1* hh  = lResidVals2D[0].createHistogram(nameHisto,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  if(hh) cout << " ===>>>>> CREATED HISTOGRAM FILE" << hh << endl;

  // HISTOs from fitted pdf
  //  TH1* hh_ = lGAdd.createHistogram(nameHistoFit,lRPt,Binning(100),YVar(lRXVar,Binning(100)));
  TH1* hh1_ = lGaus1.createHistogram(nameHistoFitG1,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  TH1* hh2_ = lGaus2.createHistogram(nameHistoFitG2,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  if(hh1_ && hh2_) cout << " ===>>>>> CREATED HISTOGRAM FILE from fited PDF" << hh1_ << " " << hh2_ << endl;

  if(doPrint) {

    TString fileName2DFIT="file2Dfit_";
    if(!fData && (!doPosW && doNegW)) fileName2DFIT += "Wneg";
    if(!fData && (doPosW && !doNegW)) fileName2DFIT += "Wpos";
    if(!fData && (!doPosW && !doNegW)) fileName2DFIT += "Z";
    if(fData) fileName2DFIT += "DATA";
    if(!fData) fileName2DFIT += "MC"; 
    if(doMad && !fData) fileName2DFIT += "_MADGRAPH";
    if(!doMad && !fData) fileName2DFIT += "_POWHEG";
    fileName2DFIT += ".root";
    
    TFile f4(fileName2DFIT.Data(),"UPDATE");
    
    if(hh) hh->Write();
    if(hh1_) hh1_->Write();
    if(hh2_) hh2_->Write();
    f4.Write();
    f4.Close();

  }

  // cout << "====================" << endl;
  // cout << "== FILL correlation coefficient histograms
  // cout << "====================" << endl;

  TMatrixDSym lCov_ = pFR->correlationMatrix();

  lCov_.Print();

  for(int i=0 ; i<lCov_.GetNrows(); i++ ) {
    for(int j=0 ; j<lCov_.GetNcols(); j++ ) {
      if(iRMS && lPar==fU1) histoCorrU1.SetBinContent(i+1,j+1,(lCov_)(i,j));
      if(iRMS && lPar!=fU1) histoCorrU2.SetBinContent(i+1,j+1,(lCov_)(i,j));
    }
  }

  cout << "============" << endl;
  cout << "============" << endl;
  cout << "=== Unbinned Fit: done ===" << endl;
  cout << "============" << endl;
  cout << "============" << endl;

  //// AFTER here set up for the 1D fit                                                                                                                    
  if(doPrintAll) {

    //Plot it all
    lRXVar.setBins(100); // Bins of the histograms with the Pull
    
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

    double *myFrac1D = new double[lNBins];
    double *myFrac2D = new double[lNBins];
    double *myMean2D = new double[lNBins];
    double *lY02D = new double[lNBins];
    double *lY12D = new double[lNBins];
    double *lY22D = new double[lNBins];
    double *lEY02D = new double[lNBins];
    double *lEY12D = new double[lNBins];
    double *lEY22D = new double[lNBins];

    for(int i0  = 0; i0 < lNBins; i0++) { 

      cout << "------------"<< endl;
      cout << "------------"<< endl;
      cout << "--- binned Fit: doing now ---"<< endl;
      cout << "------------"<< endl;
      cout << "------------"<< endl;
      
      lRGAdd.fitTo(lResidVals[i0],Warnings(kTRUE),Save(kTRUE),NumCPU(2),/*Minimizer("Minuit2","migrad"),*/Strategy(2),Minos());//,Minos()); //Double Gaussian fit for the binned fit
      /////      lRPt.setRange(i0*10,i0*10+10); /// not sure is used 
      cout << "------------"<< endl;
      cout << "------------"<< endl;
      cout << "--- binned Fit: done ---"<< endl;
      cout << "------------"<< endl;
      cout << "------------"<< endl;

      RooPlot *lFrame1 = lRXVar.frame() ;
      lFrame1->SetName("");
      lFrame1->SetTitle("");
      //lResidVals2D[0].plotOn(lFrame1);

      // plot the results of the binned fit

      // cout << "====================" << endl;
      // cout << "== plot the results of the binned fit " << endl;
      // cout << "====================" << endl;
      lResidVals[i0].plotOn(lFrame1,RooFit::MarkerColor(kRed));
      //see slide 18 in http://roofit.sourceforge.net/docs/tutorial/plot/roofit_tutorial_plot.pdf
      //Integrating out per-event errors 
      //      lRGAdd.plotOn(lFrame1,RooFit::LineColor(kBlue));
      //Projecting per-event errors with data
      lRGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals[i0]),RooFit::LineColor(kBlue));
      lRGAdd.plotOn(lFrame1,RooFit::Components(lRGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
      lRGAdd.plotOn(lFrame1,RooFit::Components(lRGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(kViolet));


      // cout << "====================" << endl;
      // cout << "== plot the results of the unbinned fit " << endl;
      // cout << "====================" << endl;
      // Plot the whole dataset and gaussians
      //      lGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals2D[0]),RooFit::LineColor(kGreen+2));
      //      lGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals2D[0]),RooFit::Components(lGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(36));
      //      lGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals2D[0]),RooFit::Components(lGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(30));
      // Display fit parameters.
      //      lGAdd.paramOn(lFrame1, Format("NELU", AutoPrecision(2)), Layout(0.1, 0.4,0.9) );

      // Plot the mixed slice (not working)
      //      TString cut = "lRXVar > ";
      //      cut += i0;      
      //      cut += " && lRXVar <= ";      
      //      cut += i0+1;      
      //      cout << "cut is " << cut.Data() << endl;
      //      lGAdd.plotOn(lFrame1,Cut(cut.Data()),ProjWData(lRXVar,lResidVals2D[0]),RooFit::Components(lGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(36));
      //      lGAdd.plotOn(lFrame1,Cut(cut.Data()),ProjWData(lRXVar,lResidVals2D[0]),RooFit::Components(lGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(30));


      iC->cd(); lFrame1->Draw();
      
      TString chi2str = Form("chi2 = %.2f",lFrame1->chiSquare());
      latexLabel.DrawLatex(0.65, 0.8, chi2str);
      TString frac1str = Form("frac1 = %.2f",lR1Frac.getVal());
      latexLabel.DrawLatex(0.65, 0.75, frac1str);
      TString rms1str = Form("sigma1 = %.2f",lR1Sigma.getVal());
      latexLabel.DrawLatex(0.65, 0.7, rms1str);
      TString rms2str = Form("sigma2 = %.2f",lR2Sigma.getVal());
      latexLabel.DrawLatex(0.65, 0.65, rms2str);
      TString meanstr = Form("mean = %.2f",lR1Mean.getVal());
      latexLabel.DrawLatex(0.65, 0.6, meanstr);

      ///////

      TString leg2 = "Zpt = ";
      leg2 += i0;      

      latexLabel.DrawLatex(0.25, 0.8, leg1);
      if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.7, leg3);
      latexLabel.DrawLatex(0.25, 0.6, leg2);
      latexLabel.DrawLatex(0.25, 0.25, legDATA);
      latexLabel.DrawLatex(0.25, 0.3, legU1U2);   

      if(!doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.2, "powheg");   
      if(doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.2, "madgraph");   

      TString test="";
      
      if(iFitS1==0) test += "PLOT/mean";
      if(iFitS1!=0 && !doAbsolute) test += "PLOT/pull";
      if(iFitS1!=0 && doAbsolute) test += "PLOT/rms";
      
      if(lPar==fU1) test += "_U1";
      if(lPar!=fU1) test += "_U2";
      
      if(fData) test += "_data";
      if(!fData) test += "_MC";

      if(!doMad && (!fData)) test += "_powheg";
      if(doMad && (!fData)) test += "_madgraph";

      if(doPosW) test += "_Wpos";
      if(doNegW) test += "_Wneg";
      
      test += "_Zpt";
      test += i0;

      test += "_jets";
      test += fNJetSelect;
      test += "_";
      test += fId;
      test += "_vtx";
      test += VTXbin;
      
      TString testPDF=test.Data();

      test += ".png";
      testPDF += ".pdf";
      
      iC->SaveAs(test.Data());

      if(i0==5 || i0==10 || i0==15) iC->Print(testPDF.Data());

      //	lchi2[i0]=chi21D;
      // comments errors

      lX[i0] = (fZPtMax/lNBins)*i0;
      lEX[i0] = (fZPtMax/lNBins)/10;
      // normalization too much
      //      lY0[i0] = (lR1Frac .getVal()*lR1Sigma.getVal() + (1.-lR1Frac.getVal())*lR2Sigma.getVal())/sqrt(2*3.14159265)*2.;

      // central values
      myMean[i0] = lR1Mean.getVal();
      lY0[i0] = (lR1Frac .getVal()*lR1Sigma.getVal() + (1.-lR1Frac.getVal())*lR2Sigma.getVal());
      lY1[i0] = lR1Sigma.getVal();///sqrt(2*3.14159265)*2.;
      lEY2[i0] = lR2Sigma.getError();///sqrt(2*3.14159265)*2.;
      myFrac[i0] = lR1Frac.getVal();

      // errors
      myMeanE[i0] = lR1Mean.getError();
      /*
	lEY0[i0]  = lR1Frac .getError()*lR1Sigma.getVal()  * lR1Frac .getError()*lR1Sigma.getVal();
	lEY0[i0] += lR1Frac .getVal()  *lR1Sigma.getError()* lR1Frac .getVal()  *lR1Sigma.getError();
	lEY0[i0] += lR1Frac .getError()*lR2Sigma.getVal()  * lR1Frac .getError()*lR2Sigma.getVal();
	lEY0[i0] += (1-lR1Frac .getVal())*lR2Sigma.getError()* (1-lR1Frac .getVal())  *lR2Sigma.getError();
	lEY0[i0] = sqrt(lEY0[i0])/sqrt(2*3.14159265)*2.;
	myFracE[i0] = lR1Frac.getError();
      */
      lEY0[i0]  = 0;
      myFracE[i0] = 0;
      lEY1[i0] = lR1Sigma.getError();///sqrt(2*3.14159265)*2.;
      lY2[i0] = lR2Sigma.getVal();///sqrt(2*3.14159265)*2.;

      lEY02D[i0] = 0; // to be implemented
      lEY12D[i0] = 0; // to be implemented
      lEY22D[i0] = 0; // to be implemented   
      
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 
      cout << "i0 " << i0 << "MEAN-2d" << lMean.getVal() << " pt-dependent:  " << lA1Mean.getVal()+lA2Mean.getVal()*i0+lA3Mean.getVal()*i0*i0 << endl;
      cout << "i0 " << i0 << "MEAN-1d" << lR1Mean.getVal() << endl;
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 

      // recompute the Fraction to get the same definition of the 2D fit
      myFrac1D[i0] = ((lY2[i0]-1.)/(lY2[i0]-lY1[i0]));

      myMean2D[i0]=lA1Mean.getVal()+lA2Mean.getVal()*i0+lA3Mean.getVal()*i0*i0;
      lY12D[i0] = (lA1Sig.getVal() + lB1Sig.getVal()*i0 + lC1Sig.getVal()*i0*i0);///sqrt(2*3.14159265)*2.;
      lY22D[i0] = (lA2Sig.getVal() + lB2Sig.getVal()*i0 + lC2Sig.getVal()*i0*i0);///sqrt(2*3.14159265)*2.;

      double tmpFrac2D=((lY22D[i0]-1.)/(lY22D[i0]-lY12D[i0]));
      lY02D[i0] = (tmpFrac2D*lY12D[i0] + (1.-tmpFrac2D)*lY22D[i0]);
      myFrac2D[i0] = tmpFrac2D;
      //      RooFormulaVar l1Sigma("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
      //      RooFormulaVar l2Sigma("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
      //      RooFormulaVar lFrac  ("frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(l1Sigma,l2Sigma));

      /*	
      // 2Dparam: lFrac, lMean, l1Sigma, l2Sigma 
      // 1Dparam: lR1Frac, lR1Mean, lR1sigma, lR2sigma 
      lX[i0] = (fZPtMax/lNBins)*i0;
      lEX[i0] = (fZPtMax/lNBins)/10;
      
      lY0[i0] = (lFrac .getVal()*l1Sigma.getVal() + (1.-l1Frac.getVal())*l2Sigma.getVal());
      lY1[i0] = l1Sigma.getVal()/sqrt(2*3.14159265)*2.;
      lY2[i0] = l2Sigma.getVal()/sqrt(2*3.14159265)*2.;
      myMeanE[i0] = lMean.getError();
      */
      
    }
    
    //

    /*
    RooPlot *lFrame2D = lRXVar.frame(Title("frame2D"),Range(0,50)) ;
    lRGAdd.plotOn(lFrame2D);
    lRGAdd.plotOn(lFrame2D,RooFit::Components(lGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
    lRGAdd.plotOn(lFrame2D,RooFit::Components(lGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta));
    iC->cd(); lFrame2D->Draw();
    //
    */

    TGraphErrors *lM0 = new TGraphErrors(lNBins,lX,myMean,lEX,myMeanE); lM0->SetMarkerStyle(kFullCircle); // this is the mean fitted of the two gaussian
    TGraphErrors *lG0 = new TGraphErrors(lNBins,lX,lY0,lEX,lEY0); lG0->SetMarkerStyle(kFullCircle); // this is the RMS obtained with the two fitted gaussian and RMS 
    TGraphErrors *lG1 = new TGraphErrors(lNBins,lX,lY1,lEX,lEY1); lG1->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG2 = new TGraphErrors(lNBins,lX,lY2,lEX,lEY2); lG2->SetMarkerStyle(kFullCircle);
    
    TGraphErrors *lM0_2d = new TGraphErrors(lNBins,lX,myMean2D,lEX,lEY0); lM0_2d->SetMarkerStyle(kOpenSquare); // this is the mean fitted of the two gaussian
    TGraphErrors *lG0_2d = new TGraphErrors(lNBins,lX,lY02D,lEX,lEY02D); lG0_2d->SetMarkerStyle(kOpenSquare); // this is the convoluted RMS
    TGraphErrors *lG1_2d = new TGraphErrors(lNBins,lX,lY12D,lEX,lEY12D); lG1_2d->SetMarkerStyle(kOpenSquare);
    TGraphErrors *lG2_2d = new TGraphErrors(lNBins,lX,lY22D,lEX,lEY22D); lG2_2d->SetMarkerStyle(kOpenSquare);

    /*
    if(lPar==fU1) lM0->GetYaxis()->SetTitle("U1 mean");
    if(lPar!=fU1) lM0->GetYaxis()->SetTitle("U2 mean");
    if(lPar==fU1) lG0->GetYaxis()->SetTitle("U1 #sigma_{mean}");
    if(lPar!=fU1) lG0->GetYaxis()->SetTitle("U2 #sigma_{mean}");
    */

    if(lPar==fU1) lM0->GetYaxis()->SetTitle("mean [ (U1-U1_{fit})/#sigma_{fit}(U1) ]");
    if(lPar!=fU1) lM0->GetYaxis()->SetTitle("mean [ (U2-U2_{fit})/#sigma_{fit}(U2) ]");
    if(lPar==fU1) lG0->GetYaxis()->SetTitle("RMS [ (U1-U1_{fit})/#sigma_{fit}(U1) ]");
    if(lPar!=fU1) lG0->GetYaxis()->SetTitle("RMS [ (U2-U2_{fit})/#sigma_{fit}(U2) ]");

    lG1->GetYaxis()->SetTitle("#sigma_{1}");
    lG2->GetYaxis()->SetTitle("#sigma_{2}");
    
    double SF=1;
    if(doAbsolute) SF=10;
    lM0->GetYaxis()->SetRangeUser(-1.,1.);
    lG0->GetYaxis()->SetRangeUser(0.,2.*SF);
    lG1->GetYaxis()->SetRangeUser(0.,1.5*SF);
    lG2->GetYaxis()->SetRangeUser(0.,5.*SF);
    
    lM0->GetXaxis()->SetTitle("Z p_{T}");
    lG0->GetXaxis()->SetTitle("Z p_{T}");
    lG1->GetXaxis()->SetTitle("Z p_{T}");
    lG2->GetXaxis()->SetTitle("Z p_{T}");

    lM0->SetTitle("");
    lG0->SetTitle("");
    lG1->SetTitle("");
    lG2->SetTitle("");
    
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
    // superimpose the 2D fit
    lG0_2d->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.75, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.7, leg3);
    //    latexLabel.DrawLatex(0.25, 0.8, "Y(V)<1");
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    if(!doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.8, "powheg");
    if(doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.8, "madgraph");

    TString pSS0="PLOTNOTE/G0";
    
    if(lPar==fU1) pSS0 += "_U1";
    if(lPar!=fU1) pSS0 += "_U2";
    
    if(fData) pSS0 += "_data";
    if(!fData) pSS0 += "_MC";
    
    if(!doMad && (!fData)) pSS0 += "_powheg";
    if(doMad && (!fData)) pSS0 += "_madgraph";

    if(doPosW) pSS0 += "_Wpos";
    if(doNegW) pSS0 += "_Wneg";

    if(doAbsolute) pSS0 += "_absolute";
    if(!doAbsolute) pSS0 += "_pull";

    pSS0 += "_";
    pSS0 += fNJetSelect;
    pSS0 += "_";
    pSS0 += fId;
    pSS0 += "_vtx";
    pSS0 += VTXbin;

    TString pSS0pdf="";
    pSS0pdf=pSS0.Data();

    pSS0 += ".png";
    pSS0pdf += ".pdf";

    iC->SaveAs(pSS0.Data());
    iC->Print(pSS0pdf.Data());


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
    lM0_2d->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.75, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.6, leg3);
    //    latexLabel.DrawLatex(0.25, 0.8, "Y(V)<1");
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);

    if(!doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.80, "powheg");
    if(doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.80, "madgraph");

    TString pMM0="PLOTNOTE/M0";

    if(lPar==fU1) pMM0 += "_U1";
    if(lPar!=fU1) pMM0 += "_U2";

    if(fData) pMM0 += "_data";
    if(!fData) pMM0 += "_MC";

    if(!doMad && (!fData)) pMM0 += "_powheg";
    if(doMad && (!fData)) pMM0 += "_madgraph";

    if(doPosW) pMM0 += "_Wpos";
    if(doNegW) pMM0 += "_Wneg";

    if(doAbsolute) pMM0 += "_absolute";
    if(!doAbsolute) pMM0 += "_pull";

    pMM0 += "_";
    pMM0 += fNJetSelect;
    pMM0 += "_";
    pMM0 += fId;
    pMM0 += "_vtx";
    pMM0 += VTXbin;

    TString pMM0pdf="";
    pMM0pdf=pMM0.Data();

    pMM0 += ".png";
    pMM0pdf += ".pdf";

    iC->SaveAs(pMM0.Data());
    iC->Print(pMM0pdf.Data());

    //////
    //// cin.get();
    //////
    
    iC->cd();
    //  TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    TF1 *lFitS1  = new TF1("lFitS1",   "pol2");
    TFitResultPtr  lFitPtr1 = lG1->Fit(lFitS1,"SRE","", fZPtMin, fZPtMax); //"EXO"
    computeFitErrors(iFitS1,lFitPtr1,lFitS1,iRMS);

    //    lG1->GetYaxis()->SetRangeUser(0.,5.);

    lG1->Draw("ape");
    drawErrorBands(iFitS1,fZPtMax);
    lG1->Draw("pe");
    lG1_2d->Draw("pe");


    latexLabel.DrawLatex(0.25, 0.75, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.7, leg3);
    //    latexLabel.DrawLatex(0.25, 0.8, "Y(V)<1");
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    if(!doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.8, "powheg");
    if(doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.8, "madgraph");

    TLine *lineSS1_min = new TLine(fZPtMin,minSigma1,fZPtMax,minSigma1);
    TLine *lineSS1_cen = new TLine(fZPtMin,startSigma1,fZPtMax,startSigma1);
    TLine *lineSS1_max = new TLine(fZPtMin,maxSigma1,fZPtMax,maxSigma1);
    lineSS1_min->SetLineColor(11);
    lineSS1_cen->SetLineColor(13);
    lineSS1_max->SetLineColor(11);
    lineSS1_min->Draw("same");
    lineSS1_cen->Draw("same");
    lineSS1_max->Draw("same");

    TString pSS1="PLOTNOTE/pG1";
    
    if(lPar==fU1) pSS1 += "_U1";
    if(lPar!=fU1) pSS1 += "_U2";
    
    if(fData) pSS1 += "_data";
    if(!fData) pSS1 += "_MC";
    
    if(!doMad && (!fData)) pSS1 += "_powheg";
    if(doMad && (!fData)) pSS1 += "_madgraph";

    if(doPosW) pSS1 += "_Wpos";
    if(doNegW) pSS1 += "_Wneg";
    
    if(doAbsolute) pSS1 += "_absolute";
    if(!doAbsolute) pSS1 += "_pull";

    pSS1 += "_";
    pSS1 += fNJetSelect;
    pSS1 += "_";
    pSS1 += fId;
    pSS1 += "_vtx";
    pSS1 += VTXbin;


    TString pSS1pdf="";
    pSS1pdf=pSS1.Data();

    pSS1 += ".png";
    pSS1pdf += ".pdf";

    iC->SaveAs(pSS1.Data());
    iC->Print(pSS1pdf.Data());


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
    TF1 *lFitS2  = new TF1("lFitS2",   "pol2");
    TFitResultPtr  lFitPtr2 = lG2->Fit(lFitS2,"SRE","", fZPtMin, fZPtMax); 
    computeFitErrors(iFitS2,lFitPtr2,lFitS2,iRMS);

    //    lG2->GetYaxis()->SetRangeUser(0.,5.);

    lG2->Draw("ape");
    drawErrorBands(iFitS2,fZPtMax);
    lG2->Draw("pe");
    lG2_2d->Draw("pe"); 

    latexLabel.DrawLatex(0.25, 0.75, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.7, leg3);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    if(!doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.8, "powheg");
    if(doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.8, "madgraph");

    TLine *lineSS2_min = new TLine(fZPtMin, minSigma2,   fZPtMax, minSigma2);
    TLine *lineSS2_cen = new TLine(fZPtMin, startSigma2, fZPtMax, startSigma2);
    TLine *lineSS2_max = new TLine(fZPtMin, maxSigma2,   fZPtMax, maxSigma2);
    lineSS2_min->SetLineColor(11);
    lineSS2_cen->SetLineColor(13);
    lineSS2_max->SetLineColor(11);
    lineSS2_min->Draw("same");
    lineSS2_cen->Draw("same");
    lineSS2_max->Draw("same");

    TString pSS2="PLOTNOTE/pG2";
    
    if(lPar==fU1) pSS2 += "_U1";
    if(lPar!=fU1) pSS2 += "_U2";
    
    if(fData) pSS2 += "_data";
    if(!fData) pSS2 += "_MC";
    
    if(!doMad && (!fData)) pSS2 += "_powheg";
    if(doMad && (!fData)) pSS2 += "_madgraph";

    if(doPosW) pSS2 += "_Wpos";
    if(doNegW) pSS2 += "_Wneg";
    
    if(doAbsolute) pSS2 += "_absolute";
    if(!doAbsolute) pSS2 += "_pull";

    pSS2 += "_";
    pSS2 += fNJetSelect;
    pSS2 += "_";
    pSS2 += fId;
    pSS2 += "_vtx";
    pSS2 += VTXbin;
    
    TString pSS2pdf="";
    pSS2pdf=pSS2.Data();

    pSS2 += ".png";
    pSS2pdf += ".pdf";

    iC->SaveAs(pSS2.Data());
    iC->Print(pSS2pdf.Data());

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
    iC->Clear();
    //    gPad.DrawFrame(0,0,50,1);
    TGraph* gFrac = new TGraph(lNBins, lX, myFrac);
    TGraph* gFrac1D = new TGraph(lNBins, lX, myFrac1D);
    TGraph* gFrac2D = new TGraph(lNBins, lX, myFrac2D);
    gFrac1D->SetMarkerColor(kBlue);
    gFrac1D->SetMarkerStyle(kFullCircle);
    gFrac2D->SetMarkerStyle(kOpenSquare);
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gFrac,"p");
    mg->Add(gFrac1D,"p");
    mg->Add(gFrac2D,"p");
    mg->Draw("A");
    //    if(lPar==fU1) mg->SetTitle("U1");
    //    if(lPar!=fU1) mg->SetTitle("U2");
    mg->SetMaximum(1);
    mg->SetMinimum(0);
    mg->GetXaxis()->SetTitle("Z p_{T}");
    mg->GetYaxis()->SetTitle("Frac #sigma_{1}");
    //    mg->GetYaxis()->SetRangeUser(0.,1.);
    gPad->Modified();

    latexLabel.DrawLatex(0.65, 0.4, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.65, 0.35, leg3);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    if(!doMad && (!fData)) latexLabel.DrawLatex(0.65, 0.45, "powheg");
    if(doMad && (!fData)) latexLabel.DrawLatex(0.65, 0.45, "madgraph");

    TString pSS4="PLOTNOTE/pFrac";
    
    if(lPar==fU1) pSS4 += "_U1";
    if(lPar!=fU1) pSS4 += "_U2";
    
    if(fData) pSS4 += "_data";
    if(!fData) pSS4 += "_MC";

    if(!doMad && (!fData)) pSS4 += "_powheg";
    if(doMad && (!fData)) pSS4 += "_madgraph";

    if(doPosW) pSS4 += "_Wpos";
    if(doNegW) pSS4 += "_Wneg";
    
    if(doAbsolute) pSS4 += "_absolute";
    if(!doAbsolute) pSS4 += "_pull";

    pSS4 += "_";
    pSS4 += fNJetSelect;
    pSS4 += "_";
    pSS4 += fId;
    pSS4 += "_vtx";
    pSS4 += VTXbin;
    
    TString pSS4pdf="";
    pSS4pdf=pSS4.Data();

    pSS4 += ".png";
    pSS4pdf += ".pdf";

    gPad->SaveAs(pSS4.Data());
    gPad->Print(pSS4pdf.Data());


    iC->cd();
    iC->Clear();
    iC->Divide(2,2);
    iC->cd(1);
    TGraph* gChi2 = new TGraph(lNBins, lX, lchi2);
    gChi2->SetName("chi2");
    if(lPar==fU1) gChi2->SetTitle("U1");
    if(lPar!=fU1) gChi2->SetTitle("U2");
    gChi2->GetXaxis()->SetTitle("Z p_{T}");
    gChi2->GetYaxis()->SetTitle("chi2");
    gChi2->Draw("ap");


    iC->cd(3);
    TGraph* gMean = new TGraph(lNBins, lX, myMean);
    if(lPar==fU1) gMean->SetTitle("U1");
    if(lPar!=fU1) gMean->SetTitle("U2");
    //    gFrac->SetName("gFrac");                                                                                                            
    gMean->GetXaxis()->SetTitle("Z p_{T}");
    gMean->GetYaxis()->SetTitle("Mean");
    gMean->Draw("ap");

    if(lPar==fU1 && fData) iC->SaveAs("chi2Plot_U1_data.png");
    if(lPar!=fU1 && fData) iC->SaveAs("chi2Plot_U2_data.png");
    if(lPar==fU1 && !fData) iC->SaveAs("chi2Plot_U1_MC.png");
    if(lPar!=fU1 && !fData) iC->SaveAs("chi2Plot_U2_MC.png");


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

    if(i1%100000==0) cout <<"Analyzed entry "<< i1 <<"/"<< nEntries <<endl;

    ////////    cout << "RUN " << fRun << " LUMI " << fLumi << " EVENT " << fEvent << endl;

    if(doRoch_corr) {
      
      //      cout << "========================================== " << endl;
      //      cout << "before correction Mass= " << fZmass << " Pt= " << fZPt << endl;
      //      cout << "before correction pos " << fMuPos_pt << " neg " << fMuNeg_pt << endl;
      TLorentzVector muonPos, muonNeg, Zorg, muonPosCorr, muonNegCorr,Zcorr;
      
      //      cout << "fMuNeg_pt " << fMuNeg_pt << " fMuNeg_eta " << fMuNeg_eta << " fMuNeg_phi " << fMuNeg_phi << endl;
      //      cout << "fMuPos_pt " << fMuPos_pt << " fMuPos_eta " << fMuPos_eta << " fMuPos_phi " << fMuPos_phi << endl;

      muonNeg.SetPtEtaPhiM( fMuNeg_pt, fMuNeg_eta, fMuNeg_phi, 0);
      muonPos.SetPtEtaPhiM( fMuPos_pt, fMuPos_eta, fMuPos_phi, 0);
      Zorg=muonPos+muonNeg;

      //      cout << "check " << Zorg.M() << " pt "<< Zorg.Pt() << endl;	
      //      cout << "Before: fZPhi " << fZPhi << endl;

      //      cout << "Before corrections pt: " << muonPos.Pt() << " eta:" << muonPos.Eta();
      if(!fData) muoncor44X->momcor_mc( muonPos, fMuPos_charge, 0 );
      if(fData) muoncor44X->momcor_data( muonPos, fMuPos_charge, 0, fEvent<175832 ? 0 : 1 );
      muonPosCorr.SetPtEtaPhiM( muonPos.Pt(), fMuPos_eta, fMuPos_phi, 0);
      
      //      cout << "After corrections " << muonPos.Pt() << " eta:" << muonPos.Eta() << endl;
      if(!fData) muoncor44X->momcor_mc( muonNeg, fMuNeg_charge, 0 );
      if(fData) muoncor44X->momcor_data( muonNeg, fMuNeg_charge, 0, fEvent<175832 ? 0 : 1 );
      muonNegCorr.SetPtEtaPhiM( muonNeg.Pt(), fMuNeg_eta, fMuNeg_phi, 0);
      
      Zcorr = muonNegCorr + muonPosCorr;
      //      Zcorr = muonNeg + muonPos;
      
      /////
      
      fMuPos_pt = muonPosCorr.Pt();
      fMuNeg_pt = muonNegCorr.Pt();
      fZmass = Zcorr.M();

      //      cout << "fZmass " << fZmass << endl; 
      // those only on DATA since for the MC are used the Gen 
      if(fData) fZPt = Zcorr.Pt();
      if(fData) fZPhi = Zcorr.Phi();
      if(fData) fZRap = Zcorr.Rapidity(); 

    }

    /////

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

    //    if(fZGenPt<0) continue;
    if(fZPt<0) continue;
    if(fZPt > fZPtMax) continue;

    // THIS WAS FOR THE VTX selection
    if(doVTXbinning && !passId(VTXbin)) continue;
    //    if(doVTXbinning && !passId(fId)) continue;
    //    if(doVTXbinning && !passId(fId)) continue;

    // this is Z selection
    if((!doPosW) && (!doNegW) && (!runSelection()))  continue;

    // this is for the W selection
    if(doPosW && (!runWSelection(true))) continue;
    if(doNegW && (!runWSelection(false))) continue;

    //////
    //////

    if((!calculateBosonRap(fId))) continue; 
    //    if(!calculateBosonRap(1)) continue;  // look the bin with 0<y<1 

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

      //      if(dodebug)      cout << "original MET " << fMet ; 

      if(!doApplyCorr) {
	// this is good for the inclusive bins and for the binned one too since take the first only 
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
      }
      
      //      if(dodebug)       cout << " MET after smearing " << fMet ; 
      
      //////
      //////
      //////

      if(doApplyCorr) {
	// to test the correlations
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
      }

      //////
      //////
      //////

      /*
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
	//	if(dodebug) 	cout << " MET after second smearing " << fMet << endl; 

      }
      */

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
	//	if(dodebug) cout << "lPar " << lPar << " isU1 " << isU1 << endl;

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

//void main(int MCtype, int iloop, int processType) { 
//void runRecoilFit(int MCtype, int iloop, int processType, int myVTXbin) { 
void runRecoilFit(int MCtype, int iloop, int processType) { 
//void runRecoilFit(int MCtype, int iloop) { 
  //  Prep();
  //  VTXbin=myVTXbin;

  //  gSystem->Load("rochcor_44X_v3_C.so");
  gSystem->Load("../../../AnalysisCode/rochcor_44X_v3.C++");

  cout << "Loaded Library rochcor_44X "<< endl;
  muoncor44X = new rochcor_44X_v3();

  gStyle->SetOptFit(111111);

  //  TString name="recoilfits/recoilfit_AUG6";
  TString name="recoilfits/recoilfit_AUG6";
  if(do8TeV) name +="_8TeV";

  /// SETTING
  /// MARIA Zpt TEST RANGE
  //  fNJetSelect = -1; fMetMax = 500; fZPtMin = 4; fZPtMax = 50; 
  fNJetSelect = -1; fMetMax = 500; fZPtMin = 0; fZPtMax = 45; 
  ///// process Type is used inside the checkPDF
  pType=processType;
  ///// fId is used inside the rapidity bin
  fId = iloop; 
  cout << " ==> looking at the production process " << pType << " rapidity Bin " << fId << endl;

  // THIS IS FOR FINE BINNING                                                                                                                                                
  if(MCtype==50) {

    //    for(int iloop=1; iloop < 21; iloop++) {                                                                                                                            

    cout << "PROCESSING DY + TOP MC " << endl;

    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");
    fTTbarFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/TTJets/ZTreeProducer_tree_RecoSkimmed.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fTTbarTree = (TTree*) fTTbarFile->FindObjectAny("ZTreeProducer");

    fData = true; 

    if(fData)  name+="_DATAlike";
    if(!fData) name+="_genZ";

    name+="_MIXBKG";

  }


  if(MCtype==1) {

    if(doMad) cout << "PROCESSING DY MC MADGRAPH -- inclusive Nvertex -- charged only " << endl;
    if(!doMad) cout << "PROCESSING DY MC POWHEG -- inclusive Nvertex -- charged only " << endl;
    // 44X
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");
    // 53X
    if(doMad)  fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");  
    if(!doMad) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsMM/ZTreeProducer_tree_SignalRecoSkimmed.root");  

    // this is the 8 TeV, just a placeholder for now
    if(doMad && do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsLL_8TeV/ZTreeProducer_tree.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fData = false; 

    if(!fData) name+="_genZ";

    if(doIterativeMet) {
      
      if(!doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,/*lZMU23SigFit,*/"recoilfits/recoilfit_AUG6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X_powheg.root" ,"PF",fId);
      if(doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,/*lZMU23SigFit,*/"recoilfits/recoilfit_AUG6_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X_madgraph.root" ,"PF",fId);
      readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,/*lZDU23SigFit,*/"recoilfits/recoilfit_AUG6_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_53X.root" ,"PF",fId);
      
    }
    
    
  }
  
  if(MCtype==2) {
    
    cout << "PROCESSING DY DATA -- inclusive Nvertex -- charged only " << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DATA/ZTreeProducer_tree_RecoSkimmed.root"); // this is 44X
    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DATA/ZTreeProducer_tree.root"); 
    
    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fData = true; 

    name+="_DATA";

  }


  if(MCtype==3) {

    doPosW=true;
    
    if(doMad) cout << "PROCESSING W pos MC --- MADGRAPH" << endl;
    if(!doMad) cout << "PROCESSING W pos MC --- POWHEG" << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_03_04/WJetsLL/WTreeProducer_tree.root");    
    if(doMad) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/WJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root");
    if(!doMad) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/WPlusPOWHEG/WTreeProducer_tree.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
    fData = false; /*fId = -1; */ 

    name+="_Wpos";

  }


  if(MCtype==4) {
    
    doNegW=true;
    
    if(doMad) cout << "PROCESSING W neg MC --- MADGRAPH" << endl;
    if(!doMad) cout << "PROCESSING W neg MC --- POWHEG" << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_03_04/WJetsLL/WTreeProducer_tree.root");        
    if(doMad) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/WJetsLL/WTreeProducer_tree_SignalRecoSkimmed.root");
    if(!doMad) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/WMinusPOWHEG/WTreeProducer_tree.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
    fData = false; /*fId = -1; */
    
    name+="_Wneg";


  }

  //////
  //// COMPLETE NAMING
  //////     

  if(doIterativeMet) name+="_ITERATIVE";

  if(doChargedMet)  name+="_tkmet";
  if(!doChargedMet) name+="_pfmet";
  
  //    if(!doYbinning && fId == (-1.))  name+="_inclusiveNvtx";
  //    if(!doYbinning && fId != (-1.))  name+="_Nvtx";
  
  if(etaMuonSel==(2.1)) name+="_eta21";
  if(etaMuonSel==(1.6)) name+="_eta16";
  if(etaMuonSel==(1.1)) name+="_eta11";
  
  if((MCtype==2 || MCtype==1 || MCtype==50)) name+="_MZ81101";
  //  if(!fData && (MCtype==2 || MCtype==1 || MCtype==50)) name+="_MZ70110";
  if(!fData) name+="_PDF";
  if(!fData) name += pType;
  if(!doYbinning) name += "_rapBin";
  if(!doYbinning) name += fId;

  if(!usePol3) name += "_pol2";
  if(usePol3) name += "_pol3";
  
  if(doType2)  name += "_type2";
  if(!doType2) name += "_type1";

  if(!doSingleGauss) name += "_doubleGauss";
  if(doSingleGauss) name += "_oneGauss";
  
  name+="_x2Stat";
  if(doAbsolute) name+="_absolute";
  //    if(doSecondStep) name+="_second";
  if(doApplyCorr) name+="_Correlation";
  
  if(doIterativeMet && !doOnlyU1 && doOnlyU2) name+="_applyOnlyU2";
  if(doIterativeMet && doOnlyU1 && !doOnlyU2) name+="_applyOnlyU1";
    
  if(doRoch_corr) name+="_ROCHcorr";
  if(doPrintAll) name+="_UNBINNED";
  
  if(doVTXbinning) name+="_VTX";
  if(doVTXbinning) name+=VTXbin;
  
  name+="_53X";
  
  if(fData)  name+=".root";
  if(!fData && doMad) name+="_madgraph.root";
  if(!fData && !doMad) name+="_powheg.root";
  
  cout << name.Data() << endl;

  if(MCtype==50) {
    fitRecoilMIX(fDataTree,fTTbarTree,name.Data(), fId);
  } else {
    fitRecoil(fDataTree, name.Data(), fId);
  }

  //////
  //// EXTRA PLOTS
  //////

  if(doPrint) {

    TString fileName="fileHisto_";
    if(!fData && MCtype==4) fileName += "Wneg";
    if(!fData && MCtype==3) fileName += "Wpos";
    if(!fData && MCtype==2) fileName += "Z";
    if(fData) fileName += "DATA";
    if(!fData) fileName += "MC"; 
    if(doMad && !fData) fileName += "_MADGRAPH";
    if(!doMad && !fData) fileName += "_POWHEG";
    if(!usePol3) fileName += "_pol2";
    if(usePol3) fileName += "_pol3";
    if(doYbinning) fileName += "_rapBin";
    if(doYbinning) fileName += fId;
    
    fileName += ".root";
    
    TFile f3(fileName.Data(),"RECREATE");
    histoU1vsZpt.Write();
    histoRecoil.Write();
    histoU1vsU2.Write();
    histoU1vsU2corrU.Write();
    histoU1vsU2corrUnorm.Write();
    histoU1vsU2_520.Write();
    histoU1vsU2_02.Write();
    histoU1vsU2_24.Write();
    histoU1vsU2_46.Write();
    histoU1vsU2_68.Write();
    histoU1vsU2_810.Write();
    histoU1vsU2_1012.Write();
    histoU1vsU2_1214.Write();
    histoU1vsU2_1416.Write();
    histoU1vsU2_1618.Write();
    histoU1vsU2_1820.Write();
    histoU1.Write();
    histoU1O.Write();
    histoU1Z.Write();
    histoU2.Write();
    histoU1vsMuPt.Write();
    histoU1vsMuPt.Write();
    histoU1diff.Write();
    histoU1diffvsZpt.Write();
    histoU1scalevsZpt.Write();
    histoU1scalevsZptscale.Write();
    histoPhiStar.Write();
    histoThetaStar.Write();
    histoPhiStarvsZpt.Write();
    
    f3.Write();
    f3.Close();
    
  }

  //  return 0;
  return ;

}
