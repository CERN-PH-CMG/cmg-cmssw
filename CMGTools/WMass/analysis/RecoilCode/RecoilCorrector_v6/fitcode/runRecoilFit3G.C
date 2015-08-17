/*
//How to run on the Z all Y binning
//(for DATA replace the first argument with 2
//(for Wpos replace the first argument with 3
//(for Wneg replace the first argument with 4

nohup root.exe -b -x -q runRecoilFit3G.C+\(1,1,-1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,125,-1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,150,-1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,175,-1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,200,-1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,201,-1\) &

//for PDF see settings

### gluon
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,1,0\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(3,1,0\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(4,1,0\) &

### quarks
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,1,1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(3,1,1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(4,1,1\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,1,2\) &
sleep 20

nohup root.exe -b -x -q runRecoilFit3G.C+\(1,1,3\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(1,1,4\) &
sleep 20

nohup root.exe -b -x -q runRecoilFit3G.C+\(3,1,2\) &
sleep 20
nohup root.exe -b -x -q runRecoilFit3G.C+\(4,1,2\) &
sleep 20
*/




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
#include "TH3F.h"

#include "TH1I.h"
#include "TProfile2D.h"

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
#include "RooPolynomial.h"
#include "RooGaussian.h"
#include "RooGaussModel.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooProdPdf.h"
#include "RooVoigtian.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooKeysPdf.h"
#include "Roo2DKeysPdf.h"
#include "RooDoubleCBFast.h"
#include "RooAddition.h"
#include "RooCustomizer.h"
#include "RooWorkspace.h"
#include "Math/MinimizerOptions.h"
#include "Math/Minimizer.h"
//#include "../../../AnalysisCode/rochcor_44X_v3.C"

///#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"
#include "TLatex.h"

#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TVectorD.h"
#include "TMatrixD.h"

#include <vector>
#include "TLorentzVector.h"

#include "TStopwatch.h"

using namespace RooFit;

/////

TH1D histoU1O("hu1O","histo U1",100,-10,10);
TH1D histoU1Z("hu1scale","histo U1",100,-5.,5.);
TH1D histoZPt("hZpt","histo Zpt",500,0.,50.);
TH1D histoUnbinnedY1ZPt("hUnbinnedY1Zpt","histo Unbinned Y1 Zpt",500,0.,50.);
TH1D histoUnbinnedY2ZPt("hUnbinnedY2Zpt","histo Unbinned Y2 Zpt",500,0.,50.);

TH1D histoSumEtY1("sumEtY1","all",500,0,500);
TH1D histoSumEtY2("sumEtY2","all",500,0,500);

TH1D histoU1("hu1","histo U1",100,-10,10);
TH1D histoU1diff("hu1diff","histo U1 - Zpt",100,-10,10);
TH2D histoU1vsZpt("hu1vsZpt","histo U1 vs Zpt",100,0,20,100,-10,10);
TH2D histoU1diffvsZpt("hu1diffvsZpt","histo (U1+Zpt) vs Zpt",100,0,20,100,-10,10);
TH2D histoU1scalevsZpt("hu1scalevsZpt","histo (U1/Zpt) vs Zpt",100,0,20,100,-10,10);
TH2D histoU1scalevsZptscale("hu1scalevsZptscale","histo (U1/Zpt) vs (Zptreco/Zpt)",100,-2,8,100,-10,10);
TH2D histoU1vsU2("hu1vsu2","histo U1 vs U2",100,-10,10,100,-10,10);
TH2D histoU1vsU2corrU("hu1vsu2corrU","histo U1 vs U2 after corrU",100,-10,10,100,-10,10);
TH2D histoU1vsU2corrUnorm("hu1vsu2corrUnorm","histo U1 vs U2 after corrU",100,-10,10,100,-10,10);
TH2D histoRecoil("hrecoil"," recoil vs recoilCorr",100,-20,20,100,-20,20);
TH2D histoU1vsU2_520("hu1vsu2520","",100,-10,10,100,-10,10);

TH1D histoU1original("hu1original", "histo U1 original", 100, -25, 25);
TH1D histoU2original("hu2original", "histo U2 original", 100, -25, 25);
TH1D histoU1smeared("hu1smeared", "histo U1 smeared", 100, -25, 25);
TH1D histoU2smeared("hu2smeared", "histo U2 smeared", 100, -25, 25);

TH1D histoU2origFrac("hu2origFrac", "histo U2 orig frac", 100, 0, 1);
TH1D histoU2wishFrac("hu2wishFrac", "histo U2 wish frac", 100, 0, 1);
TH1D histoU2smearedFrac("hu2smearedFrac", "histo U2 smeared frac", 100, 0, 1);

TH1D histoU2origFrac05("hu2origFrac05", "histo U2 orig frac", 100, 0, 1);
TH1D histoU2wishFrac05("hu2wishFrac05", "histo U2 wish frac", 100, 0, 1);
TH1D histoU2origFrac510("hu2origFrac510", "histo U2 orig frac", 100, 0, 1);
TH1D histoU2wishFrac510("hu2wishFrac510", "histo U2 wish frac", 100, 0, 1);
TH1D histoU2origFrac1015("hu2origFrac1015", "histo U2 orig frac", 100, 0, 1);
TH1D histoU2wishFrac1015("hu2wishFrac1015", "histo U2 wish frac", 100, 0, 1);
TH1D histoU2origFrac1520("hu2origFrac1520", "histo U2 orig frac", 100, 0, 1);
TH1D histoU2wishFrac1520("hu2wishFrac1520", "histo U2 wish frac", 100, 0, 1);

TH2D histoU1Rawcorr("hU1Rawcorr","histo U1 raw vs U1 after corrU",100,-10,10,100,-10,10);
TH2D histoU2Rawcorr("hU2Rawcorr","histo U2 raw vs U2 after corrU",100,-10,10,100,-10,10);

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

TH2D histoCorrU1("hCorrU1","",12,0,12, 12,0,12);
TH2D histoCorrU2("hCorrU2","",12,0,12, 12,0,12);

TH2D histoU1vsMuPt("histoU1vsMuPt","",100,0,100, 100,-50,50);
TH2D histoU2vsMuPt("histoU2vsMuPt","",100,0,100, 100,-50,50);

TH1D histoDeltaU1("hdeltaU1","histo U1corr-U1original",100,-5.,5.);
TH1D histoDeltaU2("hdeltaU2","histo U2corr-U2original",100,-5.,5.);

TH1D histoDeltaU1pull("hdeltaU1pull","histo U1corr-U1original",1001,-0.5,0.5);
TH1D histoDeltaU2pull("hdeltaU2pull","histo U2corr-U2original",1001,-0.5,0.5);

TH2D histoDeltaU1pullvsU("hdeltaU1pullvsU","histo U1original vs U1corr-U1original",1001,-1.0,1.0, 401,-20.,20.);
TH2D histoDeltaU2pullvsU("hdeltaU2pullvsU","histo U2original vs U2corr-U2original",1001,-1.0,1.0, 401,-20.,20.);

TH2D histoDeltaU1pullvsUreduced("hdeltaU1pullvsUreduced","histo U1original vs U1corr-U1original",1001,-0.1,0.1, 401,-20.,20.);
TH2D histoDeltaU2pullvsUreduced("hdeltaU2pullvsUreduced","histo U2original vs U2corr-U2original",1001,-0.1,0.1, 401,-20.,20.);

TH1D histoDeltaU1pull56("hdeltaU1pull56","histo U1corr-U1original",501,-0.25,0.25);
TH1D histoDeltaU2pull56("hdeltaU2pull56","histo U2corr-U2original",501,-0.25,0.25);

TH1D histoUpVal("histoUpVal","histoUpVal", 100,-1.5, 1.5);
TH1D histoUraw2("histoUraw2","histoUraw2", 200, 0., 50.);
TH1D histoUraw1("histoUraw1","histoUraw1", 200, 0., 50.);

TH1D histoFrac1D("histoFrac1D","histoFrac1D", 500, 0., 1.);
TH1D histoFrac1M("histoFrac1M","histoFrac1M", 500, 0., 1.);
TH1D histoFrac2D("histoFrac2D","histoFrac2D", 500, 0., 1.);
TH1D histoFrac2M("histoFrac2M","histoFrac2M", 500, 0., 1.);

TH2D histoFrac1D2D("histoFrac1D2D","",100,0.,1.,100,0.,1.);

//## needed for the boson PDF check check of the application of the recoilCorr
TH1D Vpt0("Vpt0","gluon",100,0,50);
TH1D Vpt1("Vpt1","quark1",100,0,50);
TH1D Vpt2("Vpt2","quark2",100,0,50);
TH1D Vpt3("Vpt3","quark3",100,0,50);
TH1D Vpt4("Vpt4","quark4",100,0,50);
TH1D Vpt5("Vpt5","quark5",100,0,50);
TH1D Vpt10("Vpt10","quark10",100,0,50);

TH1D sumEt("sumEt","all",500,0,500);
TH1D sumEt0("sumEt0","gluon",100,0,500);
TH1D sumEt1("sumEt1","quark1",100,0,500);
TH1D sumEt2("sumEt2","quark2",100,0,500);
TH1D sumEt3("sumEt3","quark3",100,0,500);
TH1D sumEt4("sumEt4","quark4",100,0,500);
TH1D sumEt5("sumEt5","quark5",100,0,500);
TH1D sumEt10("sumEt10","quark10",100,0,500);

TH1D Vy0("Vy0","gluon",200,-3.,3.);
TH1D Vy1("Vy1","quark1",200,-3.,3.);
TH1D Vy2("Vy2","quark2",200,-3.,3.);
TH1D Vy3("Vy3","quark3",200,-3.,3.);
TH1D Vy4("Vy4","quark4",200,-3.,3.);
TH1D Vy5("Vy5","quark5",200,-3.,3.);
TH1D Vy10("Vy10","quark10",200,-3.,3.);

TH1D xPDF0("xPDF0","xPDF0",200,-5.,0.);
TH1D xPDF1("xPDF1","xPDF1",200,-5.,0.);
TH1D xPDFm2("xPDFm2","xPDFm2",200,-5.,0.);
TH1D xPDFm1("xPDFm1","xPDFm1",200,-5.,0.);
TH1D xPDF2("xPDF2","xPDF2",200,-5.,0.);
TH1D xPDF3("xPDF3","xPDF3",200,-5.,0.);
TH1D xPDF4("xPDF4","xPDF4",200,-5.,0.);
TH1D xPDF5("xPDF5","xPDF5",200,-5.,0.);

//## PhiStar plot
TH1D histoPhiStar("hPhiStar","histo PhiStar",1000,-1.,1.);
TH1D histoThetaStar("hThetaStar","histo ThetaStar",1000,-5.,5.);
TH2D histoPhiStarvsZpt("histoPhiStarvsZpt","histo PhiStar vs Zpt",100,0,20,100,0.,0.5);
TH2D histoPhiStarvsgenZpt("histoPhiStarvsgenZpt","histo PhiStar vs genZpt",100,0,20,100,0.,0.5);

/////
///// 
bool doType2 = true; //false for the fit to the scale and the rms only

/////
///// BELOW FLAGS for verbose
bool dodebug = false; 
bool doPrint = true; // save the unbinned Mean Scale and RMS ; the results of rootFit unbinned fit; the extra plots
bool doPrintAll = true; // when this is set to true, do the binned

bool doKeys = false;

/// BELOW FLAGS to set the fits
bool usePol3 = true;
bool usePol4 = false;
bool useSubRanges = false;
bool useErfPol2ScaleU1 = false;

bool doDegenerateSigma = false;
bool do3G=true;
bool do1CB2G=false;

/// BELOW FLOAGS for the inversion
bool invGraph = true;
bool doSingleGauss = false;
bool doTriGauss = true;
bool doApplyCorr = false; // smearing along the recoil vector
bool doOnlyU1 = false;
bool doOnlyU2 = false;
bool doIterativeMet = false; // <-- together
bool writeTree = false; // <-- together
bool doClosure = false;
bool doAbsolute = false;

bool doLepProjAbsolute = false;

/// BELOW FLAGS for dataset and met def
bool do8TeV = false;
bool doABC = false;
bool doMad = true;
bool doBKG = false;
bool doGigiRescaling = false;
bool do3Sigma = false;

bool doHalfStat=false;
bool doRunA=false;
bool doRunB=false;

int VTXbin=-1;

/////

bool doRecoParam=false;

bool doWeight=false;

bool doPhiStar=false;
bool doOnlyU = false; // un-needed

bool doRoch_corr = false;
bool doKalman_corr = false;

bool doBinnedVTX = false;

bool doNeutralMet=false;
bool doChargedMet=true;
bool doGenTkMet=false;
bool doGenPfMet=false;
bool donoPU=false;

bool doMetPhiCorr=false;
bool doPosW=false;
bool doNegW=false;

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

int startTreeEntries = 0;

////

bool  fData   = false;
float fMetMax = 0; float fZPtMin = 0; float fZPtMax = 0; int fNJetSelect = 0;      
/*float   fNPV    = 0;*/ int   fNPV    = 0;
/*float fU1 = 0; float fU2 = 0; */
/*float fTKU1 = 0; float fTKU2 = 0;*/
//float fPFU1 = 0;  float fPFU2   = 0; 

double fPFU1 = 0;  double fPFU2   = 0;
double fU1 = -99999; double fU2 = -99999;
double fU1lep = -99999; double fU2lep = -99999;
double fTKU1 = 0; double fTKU2 = 0;

/*float fZPt = 0; float fZPhi = 0; float fPhi = 0; */
double fZGenPt=0; double fZPt = 0; double fZPhi = 0; double fZRap = 0;// double fPhi = 0;
double fZrecoPt = 0; double fZrecoPhi = 0; double fZrecoRap = 0;

/*float fMet = 0;  float fMPhi = 0; */ 
double fMet = 0;  double fMPhi = 0;  double fMSumET=0;
double ftkMet = 0;  double ftkMPhi = 0; double ftkMSumET=0;
//double fneuMet = 0;  double fneuPhi = 0; 

/*float fPt1 = 0; float fPhi1 = 0; float fPt2 = 0; float fEta2 = 0; float fPhi2 = 0; */
double fPt1 = 0; double fPhi1 = 0; double fPt2 = 0; double fEta2 = 0; double fPhi2 = 0; 

int fNJet = 0; float fWeight = 0;

int fId1 = 0; int fId2 = 0; float fGPhi1 = 0; float fGPhi2 = 0; float fGEta1  = 0; float fGEta2 = 0;
int fNBTag = 0; float fMJJ = 0; /*float fJPt1 = 0;*/ double fJPt1 = 0; float fDEta = 0; int fIS2011A = 0; int fId = -1; float fVPt = 0; float fVPhi = 0;

double fpdgId1, fpdgId2;
double fx1, fx2;

double fZmass;
double fMuPos_phi, fMuPos_eta, fMuPos_pt, fMuPos_mass, fMuPos_charge, fMuPosReliso, fMuPos_dxy, fMuPos_dz;
double fMuNeg_phi, fMuNeg_eta, fMuNeg_pt, fMuNeg_mass, fMuNeg_charge, fMuNegReliso, fMuNeg_dxy, fMuNeg_dz;
int fMuPosTrg, fMuPosIsTightAndIso, fevtHasGoodVtx, fevtHasTrg, fMuNegIsTightAndIso, fMuNegTrg;
int fMuPosIsTight, fMuNegIsTight;
double fMuNegGen_pt,fMuPosGen_pt;


//rochcor_44X_v3 *muoncor44X ;

int fRun=-1;
int fLumi=-1;
int fEvent=-1;

std::vector<std::vector<std::vector<double> > > lXVals_all;  std::vector<std::vector<std::vector<double> > > lYVals_all; std::vector<std::vector<std::vector<double> > > lYTVals_all;
std::vector<std::vector<std::vector<double> > > lXEVals_all; std::vector<std::vector<std::vector<double> > > lYEVals_all; 

double ***vlXVals_all,***vlXEVals_all,***vlYVals_all,***vlYTVals_all,***vlYEVals_all; //two * are needed because it is a pofloater to a pofloater

std::vector<std::vector<std::vector<double> > > lXVals_3S, lYVals_3S;
double ***vlXVals_3S, ***vlYVals_3S;

///$$$$$$$$$$$$
///$$$$
///$$$$  UTILITY to apply type1
///$$$$
///$$$$$$$$$$$$  

//RooAbsPdf* lpdfMCU1; RooAbsPdf* lpdfMCU2;
//RooAbsPdf* lpdfDATAU1; RooAbsPdf* lpdfDATAU2;
RooAbsReal* lpdfMCU1; RooAbsReal* lpdfMCU2;
RooAbsReal* lpdfDATAU1; RooAbsReal* lpdfDATAU2;
RooWorkspace *lwMCU1; RooWorkspace *lwMCU2;
RooWorkspace *lwDATAU1; RooWorkspace *lwDATAU2;

int iParamU1=-1.;
int iParamU2=-1.;

vector<double> lZMSumEt; // vector<double> lZMWeights;                                                                                                          
vector<double> lZMSumEtbis; // vector<double> lZMWeights;                                                                                                          
vector<double> lZDSumEt; // vector<double> lZMWeights;                                                                                                          
std::vector<TF1*>   lZDU1Fit; std::vector<TF1*> lZDU1RMSSMFit;
std::vector<TF1*>   lZDU2Fit; std::vector<TF1*> lZDU2RMSSMFit;
std::vector<TF1*>   lZDU1RMS1Fit; std::vector<TF1*> lZDU1RMS2Fit; std::vector<TF1*> lZDU1RMS3Fit;
std::vector<TF1*>   lZDU2RMS1Fit; std::vector<TF1*> lZDU2RMS2Fit; std::vector<TF1*> lZDU2RMS3Fit;
std::vector<TF1*> lZDU1FracFit; std::vector<TF1*> lZDU1Mean1Fit; std::vector<TF1*> lZDU1Mean2Fit;
std::vector<TF1*> lZDU2FracFit; std::vector<TF1*> lZDU2Mean1Fit; std::vector<TF1*> lZDU2Mean2Fit;

std::vector<TF1*>   lZMU1Fit; std::vector<TF1*> lZMU1RMSSMFit;
std::vector<TF1*>   lZMU2Fit; std::vector<TF1*> lZMU2RMSSMFit;
std::vector<TF1*>   lZMU1RMS1Fit; std::vector<TF1*> lZMU1RMS2Fit; std::vector<TF1*> lZMU1RMS3Fit;
std::vector<TF1*>   lZMU2RMS1Fit; std::vector<TF1*> lZMU2RMS2Fit; std::vector<TF1*> lZMU2RMS3Fit;
std::vector<TF1*> lZMU1FracFit; std::vector<TF1*> lZMU1Mean1Fit; std::vector<TF1*> lZMU1Mean2Fit;
std::vector<TF1*> lZMU2FracFit; std::vector<TF1*> lZMU2Mean1Fit; std::vector<TF1*> lZMU2Mean2Fit;


//$$$$$$$$$$$$$$$$$$$$$$$$
//$$   below code from https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/master/src/PdfDiagonalizer.cc
//$$$$$$$$$$$$$$$$$$$$$$$$

class PdfDiagonalizer {
public:
  PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result);

  RooAbsPdf *diagonalize(RooAbsPdf &pdf) ;
  const RooArgList & originalParams() { return parameters_; }
  const RooArgList & diagonalParams() { return eigenVars_; }
private:
  std::string name_;
  RooArgList  parameters_;
  RooArgList  eigenVars_;
  RooArgList  replacements_;
};

//std::string name_;
//RooArgList  * parameters_; // these are the original paramters
//RooArgList  * eigenVars_; // these are the diagonal parameters

//RooArgList  replacements_;

//void PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result) {
PdfDiagonalizer::PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result) :
  name_(name),
  parameters_(result.floatParsFinal())
{

  //  name_.append(name);/// MARIA hard coded
  //  parameters_= new RooArgList(result.floatParsFinal());
  //  int n = parameters_->getSize();

  int n = parameters_.getSize();
  //  int n = result.floatParsFinal().getSize();
  //  cout << " parameters_.getSize() " << n  << " " << result.floatParsFinal() << endl;

  TMatrixDSym cov(result.covarianceMatrix());
  TMatrixDSymEigen eigen(cov);

  const TMatrixD& vectors = eigen.GetEigenVectors();
  const TVectorD& values  = eigen.GetEigenValues();

//  cout << " --> print the eigenvalues " << endl;
//  values.Print();

  char buff[10240];

  RooArgList  eigenVars;
  //
  // create unit gaussians per eigen-vector
  for (int i = 0; i < n; ++i) {
    //    snprintf(buff,sizeof(buff),"%s_eig%s[-5,5]", name, parameters_->at(i)->GetName());
    snprintf(buff,sizeof(buff),"%s_eig%d[-5,5]", name, i);
    // w->factory(buff);
    eigenVars_.add(*(w->factory(buff)));
  }

  //  cout << "got here " << endl;
  //  eigenVars_= new RooArgList(eigenVars);

  // put them in a list, with a one at the end to set the mean
  //  RooArgList eigvVarsPlusOne(*eigenVars_);
  RooArgList eigvVarsPlusOne(eigenVars_);
  if (w->var("_one_") == 0) w->factory("_one_[1]");
  eigvVarsPlusOne.add(*w->var("_one_"));

  // then go create the linear combinations
  // each is equal to the transpose matrx times the square root of the eigenvalue (so that we get unit gaussians)
  for (int i = 0; i < n; ++i) {
    RooArgList coeffs;
    for (int j = 0; j < n; ++j) {
      snprintf(buff,sizeof(buff),"%s_eigCoeff_%d_%d[%g]", name, i, j, vectors(i,j)*sqrt(values(j)));
      //      snprintf(buff,sizeof(buff),"%s_eigCoeff_%d_%d[%g]", name, parameters_->at(i)->GetName(), j, vectors(i,j)*sqrt(values(j)));
      coeffs.add(*w->factory(buff));
    }
    snprintf(buff,sizeof(buff),"%s_eigBase_%d[%g]", name, i, (dynamic_cast<RooAbsReal*>(parameters_.at(i)))->getVal());
    //    snprintf(buff,sizeof(buff),"%s_eigBase_%d[%g]", name, i, (dynamic_cast<RooAbsReal*>(parameters_->at(i)))->getVal());
    //    snprintf(buff,sizeof(buff),"%s_eigBase_%s[%g]", name, parameters_->at(i)->GetName(), (dynamic_cast<RooAbsReal*>(parameters_->at(i)))->getVal());
    coeffs.add(*w->factory(buff));
    snprintf(buff,sizeof(buff),"%s_eigLin_%d", name, i);
    //    snprintf(buff,sizeof(buff),"%s_eigLin_%s", name, parameters_->at(i)->GetName());
    RooAddition *add = new RooAddition(buff,buff,coeffs,eigvVarsPlusOne);
    w->import(*add,Silence());
    replacements_.add(*add);
  }

  //  cout << "---------------------- NEW WORKSPACE --------------------- "<< endl;
  //  w->Print();

}

RooAbsPdf* PdfDiagonalizer::diagonalize(RooAbsPdf &pdf)
{
  if (!pdf.dependsOn(parameters_)) return 0;

  /*
  if (!pdf.dependsOn(*parameters_)) return 0;

  cout << " original parameters " << endl;
  parameters_->Print();

  cout << " diagonalized parameters " << endl;
  eigenVars_->Print();
  replacements_.Print();
  */

  // now do the customization
  RooCustomizer custom(pdf, name_.c_str());
  for (int i = 0, n = parameters_.getSize(); i < n; ++i) {
    if (pdf.dependsOn(*parameters_.at(i))) {
      custom.replaceArg(*parameters_.at(i), *replacements_.at(i));
    }
  }

  RooAbsPdf *ret = dynamic_cast<RooAbsPdf *>(custom.build());
  ret->SetName((std::string(pdf.GetName()) + "_" + name_).c_str());
  return ret;


}

bool isMC=false;

//$$$$$$$$$$$$$$$$$$$$$$$$
//$$   end code from https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/master/src/PdfDiagonalizer.cc
//$$$$$$$$$$$$$$$$$$$$$$$$

//int vtxBin=-1;                                                                                                                                                
const int readRecoil(std::vector<double> &iSumEt,
                     std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
                     std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1RMS3Fit,
		     std::vector<TF1*> &iU1FracFit,std::vector<TF1*> &iU1Mean1Fit, std::vector<TF1*> &iU1Mean2Fit,//std::vector<TF1*> &iU1Sig3Fit,
		     //
                     std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
                     std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2RMS3Fit,
		     std::vector<TF1*> &iU2FracFit,std::vector<TF1*> &iU2Mean1Fit, std::vector<TF1*> &iU2Mean2Fit,//std::vector<TF1*> &iU2Sig3Fit,
		     //
		     /*
		     RooAbsPdf * & iPdfU1, RooWorkspace * & iwU1,
		     RooAbsPdf * & iPdfU2, RooWorkspace * & iwU2,
		     */
		     RooAbsReal * & iPdfU1, RooWorkspace * & iwU1,
		     RooAbsReal * & iPdfU2, RooWorkspace * & iwU2,
                     std::string iFName = "recoilfit.root",std::string iPrefix="",int vtxBin=-1, int mytype=0) {


  //type=1 read U1; type=2 read U2;
  cout << "inside readRecoil" << endl;

  cout << " readRecoil " << iFName.c_str() << endl;

  TFile *lFile  = new TFile(iFName.c_str());
  //  lFile->ls();

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

    std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10,pSS11,pSS12,pSS13,pSS14,pSS15,pSS16;

    if(mytype==0) {
      pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);              
      pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
      pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      pSS5  << lStr << "u1RMS3_"    << i0;  iU1RMS3Fit.push_back( (TF1*) lFile->FindObjectAny((pSS5.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);         
      pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);              
      pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);           
      pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      pSS10  << lStr << "u2RMS3_"    << i0;  iU2RMS3Fit.push_back( (TF1*) lFile->FindObjectAny((pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
      //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
      pSS11  << lStr << "u1Frac_"    << i0;  iU1FracFit.push_back( (TF1*) lFile->FindObjectAny((pSS11.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      pSS12  << lStr << "u2Frac_"    << i0;  iU2FracFit.push_back( (TF1*) lFile->FindObjectAny((pSS12.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
      pSS13  << lStr << "u1Mean1_"    << i0;  iU1Mean1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS13.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
      pSS14  << lStr << "u1Mean2_"    << i0;  iU1Mean2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS14.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
      pSS15  << lStr << "u2Mean1_"    << i0;  iU2Mean1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS15.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
      pSS16  << lStr << "u2Mean2_"    << i0;  iU2Mean2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS16.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    }
    if(mytype==1) {
      pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);              
      pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
      pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
      //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);         
    }
    if(mytype==2) {
      pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);              
      pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);           
      pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
      //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);        
    }

    cout << " ------- reading workspace 1 -------------------- " << endl;

    RooAddPdf* pdfU1 = (RooAddPdf*) lFile->Get(Form("AddU1Y%d",i0));
    iwU1 = new RooWorkspace("wU1","wU1");
    iwU1->import(*pdfU1,Silence());
    RooFitResult* frU1 = (RooFitResult*) lFile->Get(Form("fitresult_AddU1Y%d_Crapsky0_U1_2D",fId));
    
    PdfDiagonalizer * diagoU1 = new PdfDiagonalizer("eigU1", iwU1, *frU1);
    RooAbsPdf* diagPdfU1 = diagoU1->diagonalize(*pdfU1);
    iwU1->import(*diagPdfU1, RooFit::RecycleConflictNodes(),RooFit::Silence());
    //  //  iwU1->import(*diagPdfU1,Silence());
    //  iwU1->Print("tV");

    if(iParamU1!=(-1) && isMC) {
      cout << "set Param iParamU1" << endl;
      RooRealVar* myVarU1=iwU1->var(Form("eigU1_eig%d",iParamU1));
      myVarU1->setVal(1);
    }

    //    RooRealVar* myX1 = (RooRealVar*) diagPdfU1->getVariables()->find("XVar");
    //    iPdfU1 = diagPdfU1->createCdf(*myX1,RooFit::ScanAllCdf());
    RooRealVar* myX1=iwU1->var("XVar");
    //    iPdfU1 = pdfU1->createCdf(*myX1,RooFit::ScanAllCdf());
    iPdfU1 = pdfU1->createCdf(*myX1);
    iwU1->import(*iPdfU1, RooFit::RecycleConflictNodes(),RooFit::Silence());
    //    delete pdfU1;
    //    delete frU1;
    //    delete diagPdfU1;
    //    delete myX1;

    cout << " ------- reading workspace 2 -------------------- " << endl;

    RooAddPdf* pdfU2 = (RooAddPdf*) lFile->Get(Form("AddU2Y%d",i0));
    iwU2 = new RooWorkspace("wU2","wU2");
    iwU2->import(*pdfU2,Silence());
    RooFitResult* frU2 = (RooFitResult*) lFile->Get(Form("fitresult_AddU2Y%d_Crapsky0_U2_2D",fId));
    
    PdfDiagonalizer * diagoU2 = new PdfDiagonalizer("eigU2", iwU2, *frU2);
    RooAbsPdf* diagPdfU2 = diagoU2->diagonalize(*pdfU2);
    iwU2->import(*diagPdfU2, RooFit::RecycleConflictNodes(),RooFit::Silence());
    //  //  iwU2->import(*diagPdfU2,Silence());
  
    if(iParamU2!=(-1) && isMC) {
      cout << "set Param iParamU2" << endl;
      RooRealVar* myVarU2=iwU2->var(Form("eigU2_eig%d",iParamU2));
      myVarU2->setVal(1);
    }

    //    RooRealVar* myX2 = (RooRealVar*) diagPdfU2->getVariables()->find("XVar");
    //    iPdfU2 = diagPdfU2->createCdf(*myX2,RooFit::ScanAllCdf());
    RooRealVar* myX2=iwU2->var("XVar");
    //    iPdfU2 = pdfU2->createCdf(*myX2,RooFit::ScanAllCdf());
    iPdfU2 = pdfU2->createCdf(*myX2);
    iwU2->import(*iPdfU2, RooFit::RecycleConflictNodes(),RooFit::Silence());
    //    delete pdfU2;
    //    delete frU2;
    //    delete diagPdfU2;
    //  delete myX2;
    
  }

  //  cout << " start fileName " << iFName.c_str() << endl;
  //  cout << "_____________ PRINT U1 workspace  ________________" << endl;
  //  iwU1->Print("tV");
  //  cout << "_____________ PRINT U1 workspace DATA ________________" << endl;
  //  iwU2->Print("tV");
  //  cout << " end fileName " << iFName.c_str() << endl;

  lFile->Close();
  iSumEt.push_back(1000);
  return lNBins;

}


double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2) {
  double lMX = -iEPt*cos(iEPhi) - iU1*cos(iWPhi) + iU2*sin(iWPhi);
  double lMY = -iEPt*sin(iEPhi) - iU1*sin(iWPhi) - iU2*cos(iWPhi);

  if(iMet == 0) return sqrt(lMX*lMX + lMY*lMY);
  if(iMet == 1) {if(lMX > 0) {return atan(lMY/lMX);} return (fabs(lMY)/lMY)*TMath::Pi() + atan(lMY/lMX); }
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
  
  double lVal=iFrac*TMath::Erf(iVal/iSigma1) + (1-iFrac)*TMath::Erf(iVal/iSigma2);
  /*
  cout << "=====================================" << endl;
  cout << "fabs(recoil_RAW)=" << iVal 
       << " iFrac=" << iFrac
       << " iSigma1=" << iSigma1
       << " iSigma2=" << iSigma2
       << " lVal=" << lVal 
       << endl;
  cout << "    TMath::Erf(iVal/iSigma1) " << TMath::Erf(iVal/iSigma1) << "   TMath::Erf(iVal/iSigma2) " << TMath::Erf(iVal/iSigma2) << endl;
  */
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
  //  cout << "-- Min - " << lMin <<  " -> " << lMax << " -- Sigma1=" << iSigma1 << " Sigma2=" << iSigma2 << endl;

  double lDiff = (lMax-lMin);
  //Iterative procedure to invert a double gaussian given a PVal
  //  int lId = 0; int lN1 = 4;  int lN2 = 10; 
  //  int lId = 0; int lN1 = 10;  int lN2 = 10; 
  int lId = 0; int lN1 = 10;  int lN2 = 100; 
  for(int i0 = 0; i0 < lN1; i0++) { 
    if(i0 != 0) lMin = lMin + (lId-1)*lDiff/lN2;
    if(i0 != 0) lDiff/=lN2;
    for(int i1 = 0; i1 < lN2; i1++) { 
      double pVal = lMin + lDiff/lN2*i1;
      pVal = diGausPVal(pVal,iFrac,iSigma1,iSigma2);
      if(TMath::ErfInverse(pVal)==0) { lId = -99; break;};

      if(pVal==(lMin + lDiff/lN2*i1)) pVal=(lMin + lDiff/lN2*i1);
      if(pVal > iPVal) {lId = i1; break;}
      //      if(pVal < iPVal && lDiff < 0 ) {lId = i1; break;}
    }
  }

  if(lId==-99) { return iPVal; cout << "nothing here " << endl; } 

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

double oneGausInvGraph(double iPVal,double iFracMC,double iSigma1MC,double iSigma2MC,double iFracDATA,double iSigma1DATA,double iSigma2DATA) {

  TF1 *mc_gaus = new TF1("mc_gaus","gaus",-50,50);
  mc_gaus->SetParameter(0,100); // mean at zero
  mc_gaus->SetParameter(1,0); // mean at zero
  mc_gaus->SetParameter(2,iSigma1MC); // set rms
  TF1 *data_gaus = new TF1("data_gaus","gaus",-50,50);
  data_gaus->SetParameter(0,100); // mean at zero
  data_gaus->SetParameter(1,0); // mean at zero
  data_gaus->SetParameter(2,iSigma1DATA); // set rms 
  mc_gaus->SetNpx(1000);
  data_gaus->SetNpx(1000);

  /*
  mc_gaus->SetLineColor(2);
  data_gaus->SetLineColor(4);
  mc_gaus->Draw("L");
  data_gaus->Draw("L same");
  */

  TGraph *gr_mc = new TGraph(mc_gaus, "I");
  TGraph *gr_data = new TGraph(data_gaus, "I");

  TGraph *gr_data_inverse = new TGraph(gr_data->GetN(),gr_data->GetY(), gr_data->GetX());

  /*
  gr_mc->SetLineColor(kMagenta);
  gr_mc->SetMarkerColor(kMagenta);
  gr_mc->Draw("A P L same");

  gr_data->SetLineColor(kGreen+1);
  gr_data->SetMarkerColor(kGreen+1);
  gr_data->Draw("P L same ");

  gr_data_inverse->SetLineColor(kCyan);
  gr_data_inverse->SetMarkerColor(kCyan);
  gr_data_inverse->Draw("P L same ");

  mc_gaus->Draw("L same");
  data_gaus->Draw("L same");
  */

  double pVal=gr_data_inverse->Eval(gr_mc->Eval(iPVal));

  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " FROM INVERSE: " << gr_data_inverse->Eval(gr_mc->Eval(iPVal)) << endl;

  delete mc_gaus;
  delete data_gaus;
  delete gr_mc;
  delete gr_data;

  delete gr_data_inverse;

  return pVal;

}


double diGausInvGraph(double iPVal,double iFracMC,double iSigma1MC,double iSigma2MC,double iFracDATA,double iSigma1DATA,double iSigma2DATA) {

  TF1 *totalMC = new TF1("totalMC","gaus(0)+gaus(3)",0,100);
  totalMC->SetParameter(0,iFracMC); 
  totalMC->SetParameter(1,0); // mean at zero
  totalMC->SetParameter(2,iSigma1MC); 
  totalMC->SetParameter(3,(1-iFracMC)); 
  totalMC->SetParameter(4,0); // mean at zero
  totalMC->SetParameter(5,iSigma2MC);
  totalMC->SetNpx(2000);

  TF1 *totalDATA = new TF1("totalDATA","gaus(0)+gaus(3)",0,100);
  totalDATA->SetParameter(0,iFracDATA); 
  totalDATA->SetParameter(1,0); 
  totalDATA->SetParameter(2,iSigma1DATA); 
  totalDATA->SetParameter(3,(1-iFracDATA));
  totalDATA->SetParameter(4,0); 
  totalDATA->SetParameter(5,iSigma2DATA);
  totalDATA->SetNpx(2000);

  TGraph *gr_mc = new TGraph(totalMC, "I");
  TGraph *gr_data = new TGraph(totalDATA, "I");
  TGraph *gr_data_inverse = new TGraph(gr_data->GetN(),gr_data->GetY(), gr_data->GetX());

  /*
  gr_mc->SetLineColor(kMagenta);
  gr_mc->SetMarkerColor(kMagenta);
  gr_mc->Draw("A P L same");

  gr_data->SetLineColor(kGreen+1);
  gr_data->SetMarkerColor(kGreen+1);
  gr_data->Draw("P L same ");

  gr_data_inverse->SetLineColor(kCyan);
  gr_data_inverse->SetMarkerColor(kCyan);
  gr_data_inverse->Draw("P L same ");
  */

  double pVal=gr_data_inverse->Eval(gr_mc->Eval(iPVal));
  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " FROM INVERSE: " << gr_data_inverse->Eval(gr_mc->Eval(iPVal)) << endl;

  delete totalMC;
  delete totalDATA;

  delete gr_mc;
  delete gr_data;
  delete gr_data_inverse;

  return pVal;

}


double getFrac2gauss(double RMS, double sigma1, double sigma2) {

  return (RMS-sigma2)/(sigma1-sigma2);
  //  return (RMS*RMS-sigma2*sigma2)/(sigma1*sigma1-sigma2*sigma2);

}


double getFrac3gauss(double RMS, double f1, double sigma1, double sigma2, double sigma3) {

  sigma1=sigma1/RMS;
  sigma2=sigma2/RMS;
  sigma3=sigma3/RMS;
  
  double f2=(-1.)*( -f1*sigma1 + f1*sigma3 - sigma3 + 1. ) / ( (f1-1.)*(sigma2-sigma3) );

  return f2*(1-f1);

}


double triGausInvGraph(double iPVal, /**/ double meanRMSMC, double iMean1MC, double iMean2MC, double iFrac1MC,double iSigma1MC,double iSigma2MC,double iSigma3MC,/**/ double meanRMSDATA, double iMean1DATA, double iMean2DATA,double iFrac1DATA,double iSigma1DATA,double iSigma2DATA,double iSigma3DATA) {


  // this for three gaussian
  double iFrac2DATA = getFrac3gauss(meanRMSDATA, iFrac1DATA, iSigma1DATA, iSigma2DATA, iSigma3DATA);
  double iFrac2MC = getFrac3gauss(meanRMSMC, iFrac1MC, iSigma1MC, iSigma2MC, iSigma3MC);

  /*
  // those parameters are passed in GeV
  cout << "meanRMSMC " << meanRMSMC << " iFrac1MC=" << iFrac1MC << " iSigma1MC=" << iSigma1MC << " iSigma2MC=" << iSigma2MC << " iSigma3MC=" << iSigma3MC << " iMean1MC=" << iMean1MC << "iMean2MC=" << iMean2MC << endl;
  cout << "meanRMSDATA " << meanRMSDATA << " iFrac1DATA=" << iFrac1DATA << " iSigma1DATA=" << iSigma1DATA << " iSigma2DATA=" << iSigma2DATA << " iSigma3DATA=" << iSigma3DATA << " iMean1DATA=" << iMean1DATA << "iMean2DATA=" << iMean2DATA << endl;
  cout << "iFrac2DATA=" << iFrac2DATA << " iFrac2MC= " << iFrac2MC << endl;
  */

  TF1 *totalMC = new TF1("totalMC","gaus(0)+gaus(3)+gaus(6)",0,100);
  totalMC->SetParameter(0,iFrac1MC); 
  totalMC->SetParameter(1,iMean1MC); // mean
  totalMC->SetParameter(2,iSigma1MC); 
  totalMC->SetParameter(3,iFrac2MC); 
  totalMC->SetParameter(4,iMean2MC); // mean
  totalMC->SetParameter(5,iSigma2MC);
  totalMC->SetParameter(6,(1-iFrac1MC-iFrac2MC)); 
  totalMC->SetParameter(7,iMean2MC); // mean
  totalMC->SetParameter(8,iSigma3MC);
  totalMC->SetNpx(2000);

  TF1 *totalDATA = new TF1("totalDATA","gaus(0)+gaus(3)+gaus(6)",0,100);
  totalDATA->SetParameter(0,iFrac1DATA); 
  totalDATA->SetParameter(1,iMean1DATA); // mean
  totalDATA->SetParameter(2,iSigma1DATA); 
  totalDATA->SetParameter(3,iFrac2DATA);
  totalDATA->SetParameter(4,iMean2DATA); // mean
  totalDATA->SetParameter(5,iSigma2DATA);
  totalDATA->SetParameter(6,(1-iFrac1DATA-iFrac2DATA));
  totalDATA->SetParameter(7,iMean2DATA); // mean
  totalDATA->SetParameter(8,iSigma3DATA);
  totalDATA->SetNpx(2000);

  /*
  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();

  totalMC->SetLineColor(kMagenta); 
  totalMC->Draw("hist");
  totalDATA->SetLineColor(kBlue); 
  totalDATA->Draw("hist same");
  c->SaveAs("gauss.png");
  cin.get(); 
  */
  //  cout << "constructed gauss " << endl;

  TGraph *gr_mc = new TGraph(totalMC, "I");
  TGraph *gr_data = new TGraph(totalDATA, "I");
  TGraph *gr_data_inverse = new TGraph(gr_data->GetN(),gr_data->GetY(), gr_data->GetX());

  /*
  gr_mc->SetLineColor(kMagenta);
  gr_mc->SetMarkerColor(kMagenta);
  gr_mc->Draw("A P L same");

  gr_data->SetLineColor(kGreen+1);
  gr_data->SetMarkerColor(kGreen+1);
  gr_data->Draw("P L same ");

  gr_data_inverse->SetLineColor(kCyan);
  gr_data_inverse->SetMarkerColor(kCyan);
  gr_data_inverse->Draw("P L same ");
  */

  double pVal=gr_data_inverse->Eval(gr_mc->Eval(iPVal));
  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " FROM INVERSE: " << gr_data_inverse->Eval(gr_mc->Eval(iPVal)) << endl;

  delete totalMC;
  delete totalDATA;

  delete gr_mc;
  delete gr_data;
  delete gr_data_inverse;

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

  histoFrac1D.Fill(pDFrac1);
  histoFrac2D.Fill(pDFrac2);
  histoFrac1M.Fill(pMFrac1);
  histoFrac2M.Fill(pMFrac2);
  histoFrac1D2D.Fill(pDFrac1,pDFrac2);

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
  double pU1Diff  = pU1-offset; // pU1 of the event ; offset
  double pU2Diff  = pU2;

  histoU1original.Fill(pU1);
  histoU2original.Fill(pU2);

  if(!scaleU2) pU2Diff*=normSigmaM; // make U2 similar to U1
  if(scaleU2) pU1Diff*=normSigmaM; // make U1 similar to U2

  if(iGenPt<20) {
    histoU1vsZpt.Fill(iGenPt,pU1Diff);
    histoU1vsU2.Fill(pU1Diff,pU2Diff);
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
  double pUValMtest = 0;
  double pUValM = 0;
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
    if(pUValD==fabs(pUValM)) pUValD=fabs(recoil); // in those cases do nothing 

    if(scaleU2) pUValMtest     = diGausPInverse(pUValM  ,pMFrac2,pMSigma2_1,pMSigma2_2);
    if(pUValM==fabs(recoil)) pUValMtest=fabs(recoil); // in those cases do nothing
    //    histoDeltaU1.Fill(pUValMtest-fabs(recoil));

  }

  if(scaleU2) {
    pU1ValD=pUValD*(pU1Diff/recoil)*(1/normSigmaD);
    pU2ValD=pUValD*(pU2Diff/recoil);
  } else {
    pU1ValD=pUValD*(pU1Diff/recoil);
    pU2ValD=pUValD*(pU2Diff/recoil)*(1/normSigmaD);
  }


  if(iGenPt<20 && pUValD!=fabs(recoil)) {
    histoUpVal.Fill(pUValM);
    histoUraw1.Fill(recoil);
    histoUraw2.Fill(pUValD);
    histoU1Rawcorr.Fill(pU1Diff,pU1ValD);
    histoU2Rawcorr.Fill(pU2Diff,pU2ValD);
    histoU1vsU2corrUnorm.Fill(pU1ValD,pU2ValD);
  }

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

  histoU1smeared.Fill(pU1);
  histoU2smeared.Fill(pU2);



  if(dodebug) cout << " Met=" << iMet << " Metphi=" << iMPhi << endl;
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
		       TF1 *iU2mean2ZDatFit, TF1 *iU2mean2ZMCFit
		       ){

  //  cout << "inside metType2 " << endl;
  //  bool doAbsolute=true;

  //  bool doSingleGauss=true;

  double lRescale  = sqrt((TMath::Pi())/2.);
  //  double lRescale  = 1;     // for squares

  double pDefU1    = iU1Default->Eval(iGenPt);
  double pDU1       = iU1RZDatFit ->Eval(iGenPt);
  //double pDU2       = 0; sPM
  double pDRMSU1    = iU1MSZDatFit->Eval(iGenPt)*lRescale;
  //  double pDRMSU1    = sqrt(iU1MSZDatFit->Eval(iGenPt));     // for squares 
  double pDSigmaU1_1 = iU1S1ZDatFit->Eval(iGenPt)*pDRMSU1;
  double pDSigmaU1_2 = iU1S2ZDatFit->Eval(iGenPt)*pDRMSU1;
  double pDSigmaU1_3 = 0;
  if(iU1S3ZDatFit) pDSigmaU1_3 = iU1S3ZDatFit->Eval(iGenPt)*pDRMSU1;

  double pDRMSU2    = iU2MSZDatFit->Eval(iGenPt)*lRescale;
  //  double pDRMSU2    = iU2MSZDatFit->Eval(iGenPt);     // for squares
  double pDSigmaU2_1 = iU2S1ZDatFit->Eval(iGenPt)*pDRMSU2;
  double pDSigmaU2_2 = iU2S2ZDatFit->Eval(iGenPt)*pDRMSU2;
  double pDSigmaU2_3 = 0;
  if(iU2S3ZDatFit) pDSigmaU2_3 = iU2S3ZDatFit->Eval(iGenPt)*pDRMSU2;
 
  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  //  double pMU2       = 0; 
  double pMRMSU1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  //  double pMRMSU1    = iU1MSZMCFit ->Eval(iGenPt);      // for squares
  double pMSigmaU1_1 = iU1S1ZMCFit ->Eval(iGenPt)*pMRMSU1;
  double pMSigmaU1_2 = iU1S2ZMCFit ->Eval(iGenPt)*pMRMSU1;
  double pMSigmaU1_3 = 0;
  if(iU1S3ZMCFit) pMSigmaU1_3 = iU1S3ZMCFit ->Eval(iGenPt)*pMRMSU1;

  double pMRMSU2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;
  //  double pMRMSU2    = iU2MSZMCFit ->Eval(iGenPt);     // for squares
  double pMSigmaU2_1 = iU2S1ZMCFit ->Eval(iGenPt)*pMRMSU2;
  double pMSigmaU2_2 = iU2S2ZMCFit ->Eval(iGenPt)*pMRMSU2;
  double pMSigmaU2_3 = 0;
  if(iU2S3ZMCFit) pMSigmaU2_3 = iU2S3ZMCFit ->Eval(iGenPt)*pMRMSU2;

  double meanM1U1=iU1mean1ZMCFit->Eval(iGenPt)*pMRMSU1;
  double meanM2U1=iU1mean2ZMCFit->Eval(iGenPt)*pMRMSU1;
  
  double meanM1U2=iU2mean1ZMCFit->Eval(iGenPt)*pMRMSU2;
  double meanM2U2=iU2mean2ZMCFit->Eval(iGenPt)*pMRMSU2;
  
  double meanD1U1=iU1mean1ZDatFit->Eval(iGenPt)*pDRMSU1;
  double meanD2U1=iU1mean2ZDatFit->Eval(iGenPt)*pDRMSU1;
  
  double meanD1U2=iU2mean1ZDatFit->Eval(iGenPt)*pDRMSU2;
  double meanD2U2=iU2mean2ZDatFit->Eval(iGenPt)*pDRMSU2;

  ///
  /// ENDING of the PARAMETERS
  ///
  ///

  //double pMMean1    = pMFrac1;
  //double pMMean2    = pMFrac2;

  // this for two gaussian
  double pDFracU1 = getFrac2gauss(pDRMSU1, pDSigmaU1_1, pDSigmaU1_2);
  double pDFracU2 = getFrac2gauss(pDRMSU2, pDSigmaU2_1, pDSigmaU2_2);
  double pMFracU1 = getFrac2gauss(pMRMSU1, pMSigmaU1_1, pMSigmaU1_2);
  double pMFracU2 = getFrac2gauss(pMRMSU2, pMSigmaU2_1, pMSigmaU2_2);

  histoU2origFrac.Fill(pMFracU2);
  histoU2wishFrac.Fill(pDFracU2);

  if(iGenPt>0 && iGenPt<=5) histoU2origFrac05.Fill(pMFracU2);
  if(iGenPt>0 && iGenPt<=5) histoU2wishFrac05.Fill(pDFracU2);
  if(iGenPt>5 && iGenPt<=10) histoU2origFrac510.Fill(pMFracU2);
  if(iGenPt>5 && iGenPt<=10) histoU2wishFrac510.Fill(pDFracU2);
  if(iGenPt>10 && iGenPt<=15) histoU2origFrac1015.Fill(pMFracU2);
  if(iGenPt>10 && iGenPt<=15) histoU2wishFrac1015.Fill(pDFracU2);
  if(iGenPt>15 && iGenPt<=20) histoU2origFrac1520.Fill(pMFracU2);
  if(iGenPt>15 && iGenPt<=20) histoU2wishFrac1520.Fill(pDFracU2);

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

  histoU1original.Fill(pU1);
  histoU2original.Fill(pU2);

  if(dodebug) cout << " initial pU1 = " << pU1 << " pU2 = " << pU2 << endl;

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);
  /////  double pTU1Diff        = pU1Diff;
  
  //  cout << "------------"<< endl;
  //  cout << "diGAUSS "<< endl;
  //  cout << "  pU1ValM " << pU1ValM  << " pU1ValD " << pU1ValD  <<endl;
  //  cout << "  pU2ValM " << pU2ValM  << " pU2ValD " << pU2ValD  <<endl;
  double pU1ValMtest = 0;
  double pU2ValMtest = 0;

  double pU1ValM = 0;
  double pU2ValM = 0 ;
  double pU1ValD = 0 ;
  double pU2ValD = 0;

  //  pU2ValM         = diGausPVal(fabs(pU2Diff),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
  //  pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0); 

  //  cout << "pMFrac1=" << pMFrac1 << " pMSigma1_1=" << pMSigma1_1 << " pMSigma1_2=" << pMSigma1_2 << "MEAN2=" << iU1MSZMCFit ->Eval(iGenPt)*lRescale<< endl;
  //  cout  << "--------------------------" << endl;

  //  cout << "pU1Diff " << pU1Diff << "pU2Diff " << pU2Diff << endl;

  //  cout << "doing graph inversion " << endl;

  ///$$$$$
  ///$$$$$ Graph inversion
  ///$$$$$

  if(invGraph) {

    if(doSingleGauss) {

      if(doOnlyU1 && !doOnlyU2) {
	pU1ValD         = oneGausInvGraph(fabs(pU1Diff), 1., pMRMSU1, 0., 1., pDRMSU1 ,0.);
        pU2ValD = fabs(pU2Diff);
      }
      if(!doOnlyU1 && doOnlyU2) {
        pU1ValD = fabs(pU1Diff);
	pU2ValD         = oneGausInvGraph(fabs(pU2Diff), 1., pMRMSU2, 0., 1., pDRMSU2 ,0.);
      }

    } else if(doTriGauss) {
      
      if(doOnlyU1 && !doOnlyU2) {
	pU1ValD         = triGausInvGraph(fabs(pU1Diff), /**/ pMRMSU1, meanM1U1, meanM2U1, iU1fracZMCFit->Eval(iGenPt), pMSigmaU1_1, pMSigmaU1_2, pMSigmaU1_3, /**/ pDRMSU1, meanD1U1, meanD2U1, iU1fracZDatFit->Eval(iGenPt), pDSigmaU1_1, pDSigmaU1_2, pDSigmaU1_3);
	pU2ValD = fabs(pU2Diff);
      }
      if(!doOnlyU1 && doOnlyU2) {
	//	cout << "Zpt " << iGenPt << endl;
	pU1ValD = fabs(pU1Diff);
	pU2ValD         = triGausInvGraph(fabs(pU2Diff), /**/ pMRMSU2, meanM1U2, meanM2U2, iU2fracZMCFit->Eval(iGenPt), pMSigmaU2_1, pMSigmaU2_2, pMSigmaU2_3, /**/ pDRMSU2, meanD1U2, meanD2U2, iU2fracZDatFit->Eval(iGenPt), pDSigmaU2_1, pDSigmaU2_2, pDSigmaU2_3);
      }

    } else if(!doSingleGauss && !doTriGauss) {

      if(doOnlyU1 && !doOnlyU2) {
	pU1ValD         = diGausInvGraph(fabs(pU1Diff), pMFracU1, pMSigmaU1_1, pMSigmaU1_2, pDFracU1, pDSigmaU1_1, pDSigmaU1_2);
        pU2ValD = fabs(pU2Diff);
      }
      if(!doOnlyU1 && doOnlyU2) {
        pU1ValD = fabs(pU1Diff);
	pU2ValD         = diGausInvGraph(fabs(pU2Diff), pMFracU2, pMSigmaU2_1, pMSigmaU2_2, pDFracU2, pDSigmaU2_1, pDSigmaU2_2);
      }
    }

  }

  ///$$$$$
  ///$$$$$ Numerical inversion
  ///$$$$$

  if(!invGraph) {
    //    cout  << "U1 original = " << fabs(pU1Diff) << " smeared " << pU1ValD << endl;
    //    cout  << "  U2 original = " << fabs(pU2Diff) << " smeared " << pU2ValD << endl;
    
    if(doSingleGauss) {
      
      pU1ValM         = diGausPVal(fabs(pU1Diff),1,pMRMSU1,0); // when is singleGauss pMFrac1=1 pMSigma1_1=fullRMS pMSigma1_2=0 
      pU2ValM         = diGausPVal(fabs(pU2Diff),1,pMRMSU2,0);
      
      if(!doOnlyU1 && !doOnlyU2) {
	pU1ValD         = oneGausPInverse(pU1ValM  ,1,pDRMSU1,0);
	pU2ValD         = oneGausPInverse(pU2ValM  ,1,pDRMSU2,0);
      }
      
      if(doOnlyU1 && !doOnlyU2) {
	pU1ValD         = oneGausPInverse(pU1ValM  ,1,pDRMSU1,0);
	pU2ValD         = oneGausPInverse(pU2ValM  ,1,pMRMSU2,0); // since doingOnlyU1, use MC for U2
      }
      
      if(!doOnlyU1 && doOnlyU2) {
	pU1ValD         = oneGausPInverse(pU1ValM  ,1,pMRMSU1,0); // since doingOnlyU2, use MC for U1 
	pU2ValD         = oneGausPInverse(pU2ValM  ,1,pDRMSU2,0); 
      }
      
      //condition #1
      if(pU1ValM==fabs(pU1Diff)) pU1ValD=fabs(pU1Diff); // in those cases do nothing  since Erf is zero
      if(pU2ValM==fabs(pU2Diff)) pU2ValD=fabs(pU2Diff); // in those cases do nothing  since Erf is zero
      
      
    } else {
      
      pU1ValM         = diGausPVal(fabs(pU1Diff),pMFracU1,pMSigmaU1_1,pMSigmaU1_2); // when is singleGauss pMFrac1=1 pMSigma1_1=fullRMS pMSigma1_2=0 
      pU2ValM         = diGausPVal(fabs(pU2Diff),pMFracU2,pMSigmaU2_1,pMSigmaU2_2);
      
      if(dodebug)  cout << "            after diGausPVal " << endl;
      if(dodebug)  cout << "                fabs(pU1Diff)=" << fabs(pU1Diff) << "; pU1ValM=" << pU1ValM << endl;
      if(dodebug)  cout << "                fabs(pU2Diff)=" << fabs(pU2Diff) << "; pU2ValM=" << pU2ValM << endl;
      
      if(!doOnlyU1 && !doOnlyU2) {
	pU1ValD         = diGausPInverse(pU1ValM  ,pDFracU1, pDSigmaU1_1, pDSigmaU1_2);
	pU2ValD         = diGausPInverse(pU2ValM  ,pDFracU2, pDSigmaU2_1, pDSigmaU2_2);
      }
      
      if(doOnlyU1 && !doOnlyU2) {
	pU1ValD         = diGausPInverse(pU1ValM  ,pDFracU1, pDSigmaU1_1, pDSigmaU1_2);
	pU2ValD         = diGausPInverse(pU2ValM  ,pMFracU2, pMSigmaU2_1, pMSigmaU2_2);
      }
      
      if(!doOnlyU1 && doOnlyU2) {
	pU1ValD         = diGausPInverse(pU1ValM  ,pMFracU1, pMSigmaU1_1,pMSigmaU1_2);
	pU2ValD         = diGausPInverse(pU2ValM  ,pDFracU2, pDSigmaU2_1,pDSigmaU2_2);
      }
      
      //condition #1
      if(pU1ValM==fabs(pU1Diff)) pU1ValD=fabs(pU1Diff); // in those cases do nothing
      if(pU2ValM==fabs(pU2Diff)) pU2ValD=fabs(pU2Diff); // in those cases do nothing
      
      //condition #2
      if(pU1ValD==pU1ValM) pU1ValD=fabs(pU1Diff); // in those cases do nothing 
      if(pU2ValD==pU2ValM) pU2ValD=fabs(pU2Diff); // in those cases do nothing 
      
      // Some TEST
      pU1ValMtest     = diGausPInverse(pU1ValM, pMFracU1, pMSigmaU1_1, pMSigmaU1_2);
      pU2ValMtest     = diGausPInverse(pU2ValM, pMFracU2, pMSigmaU2_1, pMSigmaU2_2);
      //condition #1
      if(pU1ValM==fabs(pU1Diff)) pU1ValMtest=fabs(pU1Diff); // in those cases do nothing                                                                                       
      if(pU2ValM==fabs(pU2Diff)) pU2ValMtest=fabs(pU2Diff); // in those cases do nothing                                                                                       
      //condition #2
      if(pU1ValMtest==pU1ValM) pU1ValMtest=fabs(pU1Diff); // in those cases do nothing                                                                                       
      if(pU2ValMtest==pU2ValM) pU2ValMtest=fabs(pU2Diff); // in those cases do nothing                                                                                       
      // fill TEST histo
      //      histoDeltaU1.Fill(pU1ValMtest-fabs(pU1Diff));
      //      histoDeltaU2.Fill(pU2ValMtest-fabs(pU2Diff));
      
    }

  }

  //  histoDeltaU1.Fill(-pU1ValD+fabs(pU1Diff));
  //  histoDeltaU2.Fill(-pU2ValD+fabs(pU2Diff));

  pU1ValD*=p1Charge;
  pU2ValD*=p2Charge;

  //  cout << "pU1ValD " << pU1ValD << "pU2ValD " << pU2ValD << endl;

  if(dodebug)   cout << " ==> Zpt: " << iGenPt << " pDef1: " << pDefU1 << " pDU1: " << pDU1 << " pMU1: " << pMU1 << endl;

  pDefU1 *= (pDU1/pMU1);

  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);

  histoU1smeared.Fill(pU1);
  histoU2smeared.Fill(pU2);

  if(dodebug)  cout << " after pU1 = " << pU1 << " pU2 = " << pU2 << endl;

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
    // to get the recoPt also for MC
    //    if(!fData)  iTree->SetBranchAddress("Z_pt" ,&fZPt);
    //    if(!fData)  iTree->SetBranchAddress("Z_phi",&fZPhi);
    //    if(!fData)  iTree->SetBranchAddress("Z_rap",&fZRap);

    if(!fData)  iTree->SetBranchAddress("Z_pt" ,&fZrecoPt);
    if(!fData)  iTree->SetBranchAddress("Z_phi" ,&fZrecoPhi);
    if(!fData)  iTree->SetBranchAddress("Z_rap" ,&fZrecoRap);

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
    iTree->SetBranchAddress("tkmet_sumEt",&ftkMSumET);
    if(doGenTkMet){
      iTree->SetBranchAddress("gentkmet",&ftkMet);
      iTree->SetBranchAddress("gentkmet_phi",&ftkMPhi);
      iTree->SetBranchAddress("gentkmet_sumEt",&ftkMSumET);
      iTree->SetBranchAddress("MuPosGen_pt" ,&fMuPosGen_pt);
      iTree->SetBranchAddress("MuNegGen_pt" ,&fMuNegGen_pt);
    }

    if(doGenPfMet){
      iTree->SetBranchAddress("genpfmet",&ftkMet);
      iTree->SetBranchAddress("genpfmet_phi",&ftkMPhi);
      iTree->SetBranchAddress("genpfmet_sumEt",&ftkMSumET);
      iTree->SetBranchAddress("MuPosGen_pt" ,&fMuPosGen_pt);
      iTree->SetBranchAddress("MuNegGen_pt" ,&fMuNegGen_pt);
    }
    if(doABC){
      iTree->SetBranchAddress("tkmetABC",&ftkMet);
      iTree->SetBranchAddress("tkmetABC_phi",&ftkMPhi);
      iTree->SetBranchAddress("tkmetABC_sumEt",&ftkMSumET);
    }

    if(donoPU){
      iTree->SetBranchAddress("nopumet",&fMet);
      iTree->SetBranchAddress("nopumet_phi",&fMPhi);
    } else {
      iTree->SetBranchAddress("pfmet",&fMet);
      iTree->SetBranchAddress("pfmet_phi",&fMPhi);
      iTree->SetBranchAddress("pfmet_sumEt",&fMSumET);

    }

  }


  /// these are needed for the selection

  /// Z selection 
  if((!doPosW) && (!doNegW)) {

    iTree->SetBranchAddress("Z_mass" ,&fZmass);

    iTree->SetBranchAddress("MuPos_eta" ,&fMuPos_eta);
    iTree->SetBranchAddress("MuPos_pt" ,&fMuPos_pt);
    iTree->SetBranchAddress("MuPos_phi" ,&fMuPos_phi);
    iTree->SetBranchAddress("MuPos_mass" ,&fMuPos_mass);
    iTree->SetBranchAddress("MuPos_charge" ,&fMuPos_charge);
    iTree->SetBranchAddress("MuPosRelIso" ,&fMuPosReliso);
    iTree->SetBranchAddress("MuPos_dxy" ,&fMuPos_dxy);
      
    iTree->SetBranchAddress("MuNeg_eta" ,&fMuNeg_eta);
    iTree->SetBranchAddress("MuNeg_pt" ,&fMuNeg_pt);
    iTree->SetBranchAddress("MuNeg_phi" ,&fMuNeg_phi);
    iTree->SetBranchAddress("MuNeg_mass" ,&fMuNeg_mass);
    iTree->SetBranchAddress("MuNeg_charge" ,&fMuNeg_charge);
    iTree->SetBranchAddress("MuNegRelIso" ,&fMuNegReliso);
    iTree->SetBranchAddress("MuNeg_dxy" ,&fMuNeg_dxy);
    
    iTree->SetBranchAddress("MuPosTrg" ,&fMuPosTrg);
    iTree->SetBranchAddress("MuNegTrg" ,&fMuNegTrg);
    //    iTree->SetBranchAddress("MuPosIsTightAndIso" ,&fMuPosIsTightAndIso);
    //    iTree->SetBranchAddress("MuNegIsTightAndIso" ,&fMuNegIsTightAndIso);

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

  iTree->SetBranchAddress("parton1_x" ,&fx1);
  iTree->SetBranchAddress("parton2_x" ,&fx2);
  
}

void calculateU1U2(double &iPar, bool iType) {

  //  cout << " inside calculateU1U2: fMet = " << fMet << " fMPhi = " << fMPhi << " fPt1 " << fPt1 << endl;

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

void calculateU1U2lep(double &iPar, bool iType) {
  //  cout << " inside calculateU1U2: fMet = " << fMet << " fMPhi = " << fMPhi << " fPt1 " << fPt1 << endl;

  // reset U1 U2
  fU1lep = -9999;
  fU2lep = -9999;

  double lUX  = fMet*cos(fMPhi) + fPt1*cos(fPhi1);
  double lUY  = fMet*sin(fMPhi) + fPt1*sin(fPhi1);
  //  double lUX  = fMet*cos(fMuPos_phi) + fPt1*cos(fMuPos_pt);
  //  double lUY  = fMet*sin(fMuPos_phi) + fPt1*sin(fMuPos_pt);
  double lU   = sqrt(lUX*lUX+lUY*lUY);

  // rotate of -180 the X and Y component
  double lCos = - (lUX*cos(fMuPos_phi) + lUY*sin(fMuPos_phi))/lU;
  double lSin =   (lUX*sin(fMuPos_phi) - lUY*cos(fMuPos_phi))/lU;
  fU1lep = lU*lCos;
  fU2lep = lU*lSin;
  if(iType)  iPar = fU1lep;
  if(!iType) iPar = fU2lep;

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

TH3F *scale_A1;
TH3F *scale_A2;
TH3F *scale_e;
TH3F *scale_L;
TH3F *scale_B0;
TH3F *scale_B1;
TH3F *scale_B2;
TH3F *scale_C1;
TH3F *scale_C2;

TH3F *shifted_A1;
TH3F *shifted_A2;
TH3F *shifted_e;
TH3F *shifted_L;
TH3F *shifted_B0;
TH3F *shifted_B1;
TH3F *shifted_B2;
TH3F *shifted_C1;
TH3F *shifted_C2;
TH2F *magnetic;

void setUpMuonCorrection(bool isData_) {

  // https://github.com/CERN-PH-CMG/cmg-cmssw/blob/c910b07530278a29f2b30bc78ba08143c8d59b22/CMGTools/WMass/analysis/AnalysisCode/KalmanCalibrator.cc#L7

  TRandom * random_ = new TRandom3(10101982);
  TFile *filekalman_;

  if (isData_) {
    //    std::string path("../../../utils/kalmanCalibration_data_19042015.root")
    std::string path("../../../utils/kalmanCalibration_data_31072015.root");
    filekalman_ = new TFile(path.c_str());
    magnetic = (TH2F*)filekalman_->Get("magnetic");
    magnetic->SetDirectory(0);
  }
  else {
    //    std::string path("../../../utils/kalmanCalibration_mc_19042015.root");
    std::string path("../../../utils/kalmanCalibration_mc_31072015.root");
    filekalman_ = new TFile(path.c_str());
  }
  filekalman_->ls();

  scale_A1 =(TH3F*)filekalman_->Get("A1");
  scale_A2 =(TH3F*)filekalman_->Get("A2");
  scale_e = (TH3F*)filekalman_->Get("e") ;
  scale_L = (TH3F*)filekalman_->Get("L");
  scale_B0 = (TH3F*)filekalman_->Get("B0") ;
  scale_B1 = (TH3F*)filekalman_->Get("B1") ;
  scale_B2 = (TH3F*)filekalman_->Get("B2") ;
  scale_C1 = (TH3F*)filekalman_->Get("C1") ;
  scale_C2 = (TH3F*)filekalman_->Get("C2") ;

  scale_A1->SetDirectory(0);
  scale_A2->SetDirectory(0);
  scale_e->SetDirectory(0);
  scale_L->SetDirectory(0);
  scale_B0->SetDirectory(0);
  scale_B1->SetDirectory(0);
  scale_B2->SetDirectory(0);
  scale_C1->SetDirectory(0);
  scale_C2->SetDirectory(0);


  shifted_A1 =(TH3F*)scale_A1->Clone();
  shifted_A1->SetName("shifted_A1");
  shifted_A1->SetDirectory(0);

  shifted_A2 =(TH3F*)scale_A2->Clone();
  shifted_A2->SetName("shifted_A2");
  shifted_A2->SetDirectory(0);

  shifted_e = (TH3F*)scale_e->Clone();
  shifted_e->SetName("shifted_e");
  shifted_e->SetDirectory(0);

  shifted_L = (TH3F*)scale_L->Clone();
  shifted_L->SetName("shifted_L");
  shifted_L->SetDirectory(0);

  shifted_B0 = (TH3F*)scale_B0->Clone();
  shifted_B0->SetName("shifted_B0");
  shifted_B0->SetDirectory(0);

  shifted_B1 = (TH3F*)scale_B1->Clone();
  shifted_B1->SetName("shifted_B1");
  shifted_B1->SetDirectory(0);

  shifted_B2 = (TH3F*)scale_B2->Clone();
  shifted_B2->SetName("shifted_B2");
  shifted_B2->SetDirectory(0);

  shifted_C1 = (TH3F*)scale_C1->Clone();
  shifted_C1->SetName("shifted_C1");
  shifted_C1->SetDirectory(0);

  shifted_C2 = (TH3F*)scale_C2->Clone();
  shifted_C2->SetName("shifted_C2");
  shifted_C2->SetDirectory(0);

  filekalman_->Close();


  cout << "reached here " << endl;

  /*
  // those are not needed for the central corrections

  TH3F *sigma_A_target = (TH3F*)filekalman_->Get("sigma_A_target");
  TH3F *sigma_B_target = (TH3F*)filekalman_->Get("sigma_B_target");
  TH3F *sigma_C_target = (TH3F*)filekalman_->Get("sigma_C_target");

  TH3F *sigma_A_ref = (TH3F*)filekalman_->Get("sigma_A_ref");
  TH3F *sigma_B_ref = (TH3F*)filekalman_->Get("sigma_B_ref");
  TH3F *sigma_C_ref = (TH3F*)filekalman_->Get("sigma_C_ref");

  TH3F *ebe_A = (TH3F*)filekalman_->Get("ebe_A");
  TH3F *ebe_B = (TH3F*)filekalman_->Get("ebe_B");
  TH3F *ebe_C = (TH3F*)filekalman_->Get("ebe_C");


  TH3F *closure_ = (TH3F*)file_->Get("closure");

  TMatrixDSym *cholesky_ = (TMatrixDSym*)file_->Get("cholesky");
  TH1I *covHistoMap_ = (TH1I*)file_->Get("covHistoMap");
  TH1I *covBinMap_ = (TH1I*)file_->Get("covBinMap");

  TVectorD* eigenvalues_ = (TVectorD*)file_->Get("eigenvalues");
  TMatrixD* eigenvectors_ = (TMatrixD*)file_->Get("eigenvectors");

  varyClosure_=0;
  */

}


void getPtMuonCorrrection(TLorentzVector &muon,int charge, bool isData_) {

  //////
  // https://github.com/CERN-PH-CMG/cmg-cmssw/blob/c910b07530278a29f2b30bc78ba08143c8d59b22/CMGTools/WMass/analysis/AnalysisCode/KalmanCalibrator.cc#L140

  double pt=muon.Pt(); double eta=muon.Eta(); double phi=muon.Phi(); double mass=muon.M();

  double magneticMapFactor=1.0;
  if (isData_)magneticMapFactor = magnetic->GetBinContent(magnetic->GetBin(
									   magnetic->GetXaxis()->FindBin(phi),
									   magnetic->GetYaxis()->FindBin(eta)
									   )
							  );

  double curvature = magneticMapFactor/pt;
  double sinTheta  = sin(2*atan(exp(-eta)));
  double e = shifted_e->GetBinContent(scale_e->GetBin(1,scale_e->GetYaxis()->FindBin(eta),1));

  double A1 = shifted_A1->GetBinContent(13);
  double A2 = shifted_A2->GetBinContent(13);
  double B0 = shifted_B0->GetBinContent(scale_B0->GetBin(1,scale_B0->GetYaxis()->FindBin(eta),1));
  double B1 = shifted_B1->GetBinContent(scale_B1->GetBin(1,scale_B1->GetYaxis()->FindBin(eta),1));
  double B2 = shifted_B2->GetBinContent(scale_B2->GetBin(1,scale_B2->GetYaxis()->FindBin(eta),1));
  double C1 = shifted_C1->GetBinContent(scale_C1->GetBin(1,scale_C1->GetYaxis()->FindBin(eta),1));
  double C2 = shifted_C2->GetBinContent(scale_C2->GetBin(1,scale_C2->GetYaxis()->FindBin(eta),1));

  double B = B0+B1*sin(phi)+B2*sin(2*phi)+C1*cos(phi)+C2*cos(2*phi);
  double mag=A1+A2*eta*eta;

  curvature = mag*curvature -e*sinTheta*curvature*curvature+charge*B;

  //  pt = (1.0/curvature)*(1.0+varyClosure_*closure(pt,eta));
  pt = (1.0/curvature);

  muon.SetPtEtaPhiM(pt,eta,phi,mass);

  return;

}

void applyGigiRescaling() {

      TLorentzVector Z;
      
      //Z.SetPxPyPzE(fZrecoPt*cos(fZPhi), fZrecoPt*sin(fZPhi), fZ_mt*TMath::SinH(fZRap), TMath::Sqrt(fZrecoPt*cos(fZPhi)*fZrecoPt*cos(fZPhi)+fZrecoPt*sin(fZPhi)*fZrecoPt*sin(fZPhi)+fZ_mt*TMath::SinH(fZRap)*fZ_mt*TMath::SinH(fZRap)+fZmass*fZmass));
      
      TLorentzVector PosMuon, NegMuon;
      PosMuon.SetPtEtaPhiM(fMuPos_pt, fMuPos_eta, fMuPos_phi, fMuPos_mass);
      NegMuon.SetPtEtaPhiM(fMuNeg_pt, fMuNeg_eta, fMuNeg_phi, fMuNeg_mass);
      
      
      double x=90.9376/(TMath::Sqrt(2*(1-cos(PosMuon.Angle( NegMuon.Vect() )))));
      double y=TMath::Sqrt(PosMuon.P()/NegMuon.P());

      PosMuon.SetRho(x*y);
      NegMuon.SetRho(x/y);
      
      Z=PosMuon+NegMuon;
      
      fMuPos_pt = PosMuon.Pt();
      fMuNeg_pt = NegMuon.Pt();
      //      cout << "fZmass " << fZmass << " Z.M() " << Z.M() << endl; 

      fZmass = Z.M();

      // those only on DATA since for the MC are used the Gen
      if(fData) fZPt = Z.Pt();
      if(fData) fZPhi = Z.Phi();
      if(fData) fZRap = Z.Rapidity();

}


double calculateSinThetaStar(double eta1, double eta2) {
  return TMath::Sqrt(1-TMath::Power(TMath::TanH((eta1-eta2)/2),2));
}

double calculateCosThetaStar(double eta1, double eta2) {
  return TMath::TanH((eta1-eta2)/2);
}

double calculatePhiStar(double eta1, double eta2, double phi1, double phi2){

  float dphi = fabs( phi1 - phi2 );
  if( dphi > TMath::Pi() ) dphi = TMath::TwoPi() - dphi;
  double Phi_acop=TMath::Pi()-dphi;

  double SinThetaStar=calculateSinThetaStar(eta1,eta2);

  double PhiStar=(TMath::Tan(Phi_acop/2))*SinThetaStar;

  return PhiStar;

}

bool runSelection(bool doMet) {
  
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
		   //		   && fabs(fMuNeg_dz)<0.1
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

  bool ZmassWindow=false;
  if(fZmass>81 && fZmass<101) ZmassWindow=true;
  //  if(doBKG && fZmass>70 && fZmass<110) ZmassWindow=true;
  //    ?? MARIA BKG ??
  //  if(doBKG && fZmass>50 && fZmass<130) ZmassWindow=true; // this is to derive the BKG

  if( fevtHasGoodVtx
      && fevtHasTrg
      //      && fZmass>50
      //      && fZmass>70 // this is the default cut
      //      && fZmass<110 // this is the default cut  
      //      && fZmass>81
      //      && fZmass<101
      && ZmassWindow
      && (fMuNeg_charge != fMuPos_charge)
      && (LeadingPos || LeadingNeg)
      )
    {

      // switched off MET
      // MARIA 
      //      if( doMet && metLike<25 ) return false;

      //    cout << "GOOD selection" << endl;
      /////////
      /// FILLING HISTOGRAMS
      
      /*
      histoZPt.Fill(fZPt);

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
   
      //      double SinThetaStar=calculateSinThetaStar(fMuPos_eta, fMuNeg_eta);
      double CosThetaStar=calculateCosThetaStar(fMuPos_eta, fMuNeg_eta);

      double PhiStar=calculatePhiStar(fMuPos_eta, fMuNeg_eta, fMuPos_phi, fMuNeg_phi);
      histoPhiStarvsgenZpt.Fill(fPt1,PhiStar);
      histoPhiStarvsZpt.Fill(fZPt,PhiStar);
      histoPhiStar.Fill(PhiStar);
      histoThetaStar.Fill(CosThetaStar);

      if(dodebug) cout << "passed run selection "<< endl;     
      */

      return true;

    }
  
  //  cout << "BAD selection" << endl;

   return false;

 }

bool runWSelection(bool doPos, bool doMet) {

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
     
   // switched off MET                                                                                                                                                              
   //   if( doMet && fMet <= 25 ) return false;

   // this is needed to initialize the recoil eventually
   double pippo=0;
   calculateU1U2(pippo, true );
   //   float recoil=sqrt(fU1*fU1 + fU2*fU2);
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
     // W selection     

     histoU1vsZpt.Fill(fZPt,fU1);
     histoU1vsU2.Fill(fU1,fU2);
     
     histoU1diffvsZpt.Fill(fZPt,fU1+fPt1);
     histoU1vsMuPt.Fill(fMuPos_pt,fU2);

     histoU1diff.Fill(fU1+fPt1);     
     histoU1.Fill(fU1);
     histoU2.Fill(fU2);

     if(dodebug) cout << "passed runW selection "<< endl;     

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


void fillXPDF() {

      if(fpdgId1==1) xPDF1.Fill(log(fx1)); // valence u + sea u 
      if(fpdgId2==1) xPDF1.Fill(log(fx2));

      if(fpdgId1==2) xPDF2.Fill(log(fx1)); // valence d  + sea d 
      if(fpdgId2==2) xPDF2.Fill(log(fx2));

      if(fpdgId1==-1) xPDFm1.Fill(log(fx1)); // sea ubar
      if(fpdgId2==-1) xPDFm1.Fill(log(fx2));

      if(fpdgId1==-2) xPDFm2.Fill(log(fx1)); // sea ubar
      if(fpdgId2==-2) xPDFm2.Fill(log(fx2));

      if(abs(fpdgId1)==3) xPDF3.Fill(log(fx1)); // PDF quark and antiquark from sea are the same
      if(abs(fpdgId2)==3) xPDF3.Fill(log(fx2));
      if(abs(fpdgId1)==4) xPDF4.Fill(log(fx1)); 
      if(abs(fpdgId2)==4) xPDF4.Fill(log(fx2)); 

}

bool checkPDF(int typeBoson, bool doPlot) { 

  // https://lhapdf.hepforge.org/design.html
  // Flavours are identified by standard PDG ID codes. A PDF can contain arbitrarily many flavours. A special case is ID = 0, which for backward compatibility and convenience is treated as equivalent to ID = 21 (this allows a for-loop from -6 to +6 to cover all quarks and the gluon without need of special treatment to replace the 0 index with 21).

  // cout << "====================" << endl;
  // cout << "== THOSE are for the Z 
  // cout << "====================" << endl;

  if(typeBoson==23) {

    fillXPDF();

    if((fpdgId1+fpdgId2)==0) {
      if( pType==1 && abs(fpdgId1)==1 && abs(fpdgId2)==1 ) { if(doPlot) {Vpt1.Fill(fZPt); Vy1.Fill(fZRap); sumEt1.Fill(ftkMSumET);} return true; } // cout << " d dbar " << endl;
      if( pType==2 && abs(fpdgId1)==2 && abs(fpdgId2)==2 ) { if(doPlot) {Vpt2.Fill(fZPt); Vy2.Fill(fZRap); sumEt2.Fill(ftkMSumET);} return true; } // cout << " u ubar " << endl;
      if( pType==3 && abs(fpdgId1)==3 && abs(fpdgId2)==3 ) { if(doPlot) {Vpt3.Fill(fZPt); Vy3.Fill(fZRap); sumEt3.Fill(ftkMSumET);} return true; } // cout << " s sbar " << endl;
      if( pType==4 && abs(fpdgId1)==4 && abs(fpdgId2)==4 ) { if(doPlot) {Vpt4.Fill(fZPt); Vy4.Fill(fZRap); sumEt4.Fill(ftkMSumET);} return true; } // cout << " c cbar " << endl;
      if( pType==5 && abs(fpdgId1)==5 && abs(fpdgId2)==5 ) { if(doPlot) {Vpt5.Fill(fZPt); Vy5.Fill(fZRap); sumEt5.Fill(ftkMSumET);} return true; } // cout << " b bbar " << endl;
    } else if( pType==0 && (abs(fpdgId1)==0 || abs(fpdgId2)==0) ) {
      if(doPlot) {
	if(abs(fpdgId1)==0) xPDF0.Fill(log(fx1)); if(abs(fpdgId2)==0) xPDF0.Fill(log(fx2));
	Vpt0.Fill(fZPt); Vy0.Fill(fZRap); sumEt0.Fill(ftkMSumET);};
      return true; //   cout << " gluon + X  " << endl;
    } else {
      //      cout << "UNKNOWN: " << "fpdgId1 " << fpdgId1 << " fpdgId2 " << fpdgId2 << endl;
      if(doPlot) {Vpt10.Fill(fZPt); Vy10.Fill(fZRap);  sumEt10.Fill(ftkMSumET);} return false; //cout << " UNKNOWN "<< endl;
    }
  }

  // cout << "====================" << endl;
  // cout << "== THOSE are for the W +" << endl;  
  // cout << "====================" << endl;

  if(typeBoson==24) {

    fillXPDF();

    // cout << "== typeBoson " << typeBoson << " doPosW " << doPosW << endl;
    // cout << "== pType " << " fpdgId1 " << fpdgId1 <<  " fpdgId2 " << fpdgId2 << endl;
    
    if( pType==1 && ((fpdgId1==2 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==2))) {
      if(doPlot) {Vpt1.Fill(fZPt); Vy1.Fill(fZRap); sumEt1.Fill(ftkMSumET);} return true; //cout << " u dbar " << endl;
    } else if( pType==2 && (
			    ((fpdgId1==2 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==2)) ||  //cout << " u sbar " << endl;   
			    ((fpdgId1==2 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==2)) ||  //cout << " u bbar " << endl; 
			    ((fpdgId1==4 && fpdgId2==-1) || (fpdgId1==-1 && fpdgId2==4)) ||  //cout << " c dbar " << endl; 
			    ((fpdgId1==4 && fpdgId2==-3) || (fpdgId1==-3 && fpdgId2==4)) ||  //cout << " c sbar " << endl; 
			    ((fpdgId1==4 && fpdgId2==-5) || (fpdgId1==-5 && fpdgId2==4))     //cout << " c bbar " << endl;  
			    )
	       ) { if(doPlot) {Vpt2.Fill(fZPt); Vy2.Fill(fZRap); sumEt2.Fill(ftkMSumET);} return true; 
    } else if( pType==0 && (fpdgId1==0 || fpdgId2==0) ) {
      if(doPlot) {
	if(abs(fpdgId1)==0) xPDF0.Fill(log(fx1)); if(abs(fpdgId2)==0) xPDF0.Fill(log(fx2));
	Vpt0.Fill(fZPt); Vy0.Fill(fZRap); sumEt0.Fill(ftkMSumET);
      }
      return true; //cout << "gluon + X "<< endl;
    } else {  
      Vpt10.Fill(fZPt); Vy10.Fill(fZRap); sumEt10.Fill(ftkMSumET);return false; //   cout << " UNKNOWN "<< endl;
    }
  }

  // cout << "====================" << endl;
  // cout << "== THOSE are for the W -" << endl;  
  // cout << "====================" << endl;

  if(typeBoson==(-24)) {

    fillXPDF();

    if( pType==1 && ((fpdgId1==1 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==1))) {
      if(doPlot) { Vpt1.Fill(fZPt); Vy1.Fill(fZRap); sumEt1.Fill(ftkMSumET); } return true; //cout << " d ubar " << endl;
    }
    if( pType==2 && (
		     ((fpdgId1==1 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==1)) || //cout << " d cbar " << endl;
		     ((fpdgId1==3 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==3)) || //cout << " s ubar " << endl; 
		     ((fpdgId1==3 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==3)) || //cout << " s cbar " << endl;
		     ((fpdgId1==5 && fpdgId2==-2) || (fpdgId1==-2 && fpdgId2==5)) || //cout << " b ubar " << endl;  
		     ((fpdgId1==5 && fpdgId2==-4) || (fpdgId1==-4 && fpdgId2==5)) //cout << " b cbar " << endl;   
		     )
	) { 
      if(doPlot) { Vpt2.Fill(fZPt); Vy2.Fill(fZRap); sumEt2.Fill(ftkMSumET);} return true;
    } else if( pType==0 && (fpdgId1==0 || fpdgId2==0))  {
      if(doPlot) {
	if(abs(fpdgId1)==0) xPDF0.Fill(log(fx1)); if(abs(fpdgId2)==0) xPDF0.Fill(log(fx2));
	Vpt0.Fill(fZPt); Vy0.Fill(fZRap); sumEt0.Fill(ftkMSumET);
      }
      return true; //cout << "gluon + X "<< endl;
    } else {
      if(doPlot) { Vpt10.Fill(fZPt); Vy10.Fill(fZRap); sumEt10.Fill(ftkMSumET);}
      return false; //   cout << " UNKNOWN "<< endl;
    }
  }
    
  return false;
  
}
  
bool calculateBosonRap(int step) {

  if( step == 1 && fabs(fZRap)<1 ) return true;
  if( step == 2 && fabs(fZRap)>=1 ) return true;
  //  if( step == 3 && fabs(fZRap)>2) return true;

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

  if(doBinnedVTX) {

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
  
  /*
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

  cout << "UpperError(0) " << iFPtr->UpperError(0) << " LowerError(0) " << iFPtr->LowerError(0) << endl;
  cout << "UpperError(1) " << iFPtr->UpperError(1) << " LowerError(0) " << iFPtr->LowerError(1) << endl;
  cout << "UpperError(2) " << iFPtr->UpperError(2) << " LowerError(0) " << iFPtr->LowerError(2) << endl;
  */

  TMatrixDSym lCov = iFPtr->GetCovarianceMatrix();
  lCov.Print();  

  double err00=(lCov)(0,0); double err01=(lCov)(0,1); double err10=(lCov)(1,0); double err11=(lCov)(1,1);

  double lE0 = err00;
  double lE1 = err01 + err10;
  double lE2 = err11;

  double lE3 = 0; 
  double lE4 = 0; 

  // pol2 or greater
  if(lPol2) {

    double err02=(lCov)(0,2); double err12=(lCov)(1,2); double err22=(lCov)(2,2);

    lE2 += 2*err02;
    lE3 = 2*err12;
    lE4 = err22; 

  }


  double lE5 = 0;
  double lE6 = 0;
  double lE7 = 0;

  // pol3 or greater
  if(lPol3) {

    double err03=(lCov)(0,3); double err13=(lCov)(1,3); double err23=(lCov)(2,3); double err33=(lCov)(3,3);

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
  if(iTrueFit->GetParameter(5)) iFit->SetParameter(5,iTrueFit->GetParameter(5)); 
  if(iTrueFit->GetParameter(6)) iFit->SetParameter(6,iTrueFit->GetParameter(6)); 
  if(iTrueFit->GetParameter(7)) iFit->SetParameter(7,iTrueFit->GetParameter(7)); 
  iFit->SetParError(5,lE5);
  iFit->SetParError(6,lE6);
  iFit->SetParError(7,lE7);

  
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



void computeFitErrorsERF(TF1 *iFit,TFitResultPtr &iFPtr,TF1 *iTrueFit,bool iPol0=false) {

  //   bool lPol0 = iPol0;                                                                                                                                                           
  //  bool lPol2 = (iTrueFit->GetParError(2) != 0);
  //  bool lPol3 = (iTrueFit->GetParError(3) != 0);
 
  TMatrixDSym lCov = iFPtr->GetCovarianceMatrix();

  double err00=(lCov)(0,0); double err01=(lCov)(0,1); double err10=(lCov)(1,0); double err11=(lCov)(1,1);
  double err02=(lCov)(0,2); double err12=(lCov)(1,2); double err22=(lCov)(2,2);
  double err03=(lCov)(0,3); double err13=(lCov)(1,3); double err23=(lCov)(2,3); double err33=(lCov)(3,3);

  iFit->SetParameter(0,iTrueFit->GetParameter(0));
  iFit->SetParameter(1,iTrueFit->GetParameter(1));
  iFit->SetParameter(2,iTrueFit->GetParameter(2));
  iFit->SetParameter(3,iTrueFit->GetParameter(3));

  /*
  if(!fData && !doIterativeMet){
    iFit->SetParameter(4,iTrueFit->GetParameter(4));
    iFit->SetParameter(5,iTrueFit->GetParameter(5));
  }
  */

  // MARIA: unused err10
  iFit->SetParError(0,err00);
  iFit->SetParError(1,err11);
  iFit->SetParError(2,err22);
  iFit->SetParError(3,err33);
  iFit->SetParError(5,err01);
  iFit->SetParError(6,err02);
  iFit->SetParError(7,err03);
  iFit->SetParError(9,err12);
  iFit->SetParError(10,err13);
  iFit->SetParError(12,err23);

  /*
  if(!fData && !doIterativeMet){
    double err44=(lCov)(4,4); double err34=(lCov)(3,4); double err04=(lCov)(0,4); double err14=(lCov)(1,4); double err24=(lCov)(2,4);
    iFit->SetParError(4,err44);
    iFit->SetParError(8,err04);
    iFit->SetParError(11,err14);
    iFit->SetParError(13,err24);
    iFit->SetParError(14,err34);
  }
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
  return (fabs(lMY)/lMY)*TMath::Pi() + atan(lMY/lMX);
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

double range_min;
double range_max;
float binSize=1.; // this is 1 GeV for Zpt
//double range_min = fZPtMin+binSize;
//double range_max = fZPtMax-binSize;
//double range_min = fZPtMin;
//double range_max = fZPtMax;
double minRangeSigma; 
double maxRangeSigma;

RooDataHist * bkg_hist;
RooHistPdf * bkg_pdf;
RooAddPdf * model;
RooAddPdf * model2D;
//RooRealVar sigbkgfrac("sigbkgfrac","sigbkgfrac",0.98,0.95,1.);
RooFormulaVar * sigbkgFrac;
RooFormulaVar * sigbkgFrac2D;

RooHistPdf * bkg_pdf2D;
RooDataHist * bkg_hist2D;

double rangeMinXVar = -5.;
double rangeMaxXVar = 5.;

//RooRealVar lRPt;
//RooRealVar lRXVar;
RooRealVar lRPt("pt","Z_{p_{T}}",5.,0.,20.);
RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,rangeMinXVar,rangeMaxXVar);
RooAddPdf * lRGAdd;
RooAddPdf * lGAdd;  
RooGaussian * lGaus1;
RooGaussian * lGaus2;
RooGaussian * lGaus3;
RooDoubleCBFast * lCB;

RooGaussian * lRGaus1;
RooGaussian * lRGaus2;
RooGaussian * lRGaus3;
RooDoubleCBFast * lRCB;


RooRealVar  lR1Mean("Rmean",  "Rmean",0,-10.,10.);
RooRealVar  lR1MeanLarge("RmeanLarge",  "RmeanLarge",0,-10.,10.);
RooRealVar  lR1MeanVeryLarge("RmeanVeryLarge",  "RmeanVeryLarge",0,-10.,10.);
RooRealVar  lR1Sigma("Rsigma1","RSigma1",0,0.,10.);
RooRealVar  lR2Sigma("Rsigma2","RSigma2",0,0.,10.);
RooRealVar  lR3Sigma("Rsigma3","RSigma3",0,0.,10.);

//RooRealVar l1Sigma("sigma1","sigma1",0,0.,10.);
//RooRealVar l2Sigma("sigma2","sigma2",0,0.,10.);
//RooRealVar l3Sigma("sigma3","sigma3",0,0.,10.);

RooRealVar lRAlpha1("RAlpha1","Alpha1",0,0.,10.);
RooRealVar lRN1("RN1","RN1",0,0.,10.);
RooRealVar lRAlpha2("RAlpha2","RAlpha2",0,0.,10.);
RooRealVar lRN2("RN2","RN2",0,0.,10.);

RooFormulaVar * l1Sigma;
RooFormulaVar * l2Sigma;
RooFormulaVar * l3Sigma;

//RooRealVar lR1Frac;
RooFormulaVar * lR1Frac;
RooFormulaVar * lRFrac ;
RooFormulaVar * l1Frac;
RooFormulaVar * lFrac;

RooRealVar lAbkgFrac("AbkgFrac","AbkgFrac",0.,-10.,10.);
RooRealVar lBbkgFrac("BbkgFrac","BbkgFrac",0.,-10.,10.);
RooRealVar lCbkgFrac("CbkgFrac","CbkgFrac",0.,-10.,10.);
RooRealVar lDbkgFrac("DbkgFrac","DbkgFrac",0.,-10.,10.);

RooRealVar lRAFrac("RAFrac","RAFrac",0.,-5.,5.);
RooRealVar lRBFrac("RBFrac","RBFrac",0.,-5.,5.);

RooRealVar lAFrac("AFrac","AFrac",0.,0.,5.);
RooRealVar lBFrac("BFrac","BFrac",0.,-5.,5.);
RooRealVar lCFrac("CFrac","CFrac",0.,-5.,5.);
RooRealVar lDFrac("DFrac","DFrac",0.,-5.,5.);

RooRealVar lA1Frac("A1Frac","A1Frac",0.,0.,5.);
RooRealVar lB1Frac("B1Frac","B1Frac",0.,-5.,5.);
RooRealVar lC1Frac("C1Frac","C1Frac",0.,-5.,5.);
RooRealVar lD1Frac("D1Frac","D1Frac",0.,-5.,5.);

RooRealVar lA1Sig("a1sig","a1sig",0.,  0.,10.);
RooRealVar lB1Sig("b1sig","b1sig",0.,-10.,10.);
RooRealVar lC1Sig("c1sig","c1sig",0.,-10.,10.);
RooRealVar lD1Sig("d1sig","d1sig",0.,-10.,10.);

RooRealVar lA2Sig("a2sig","a2sig",0.,  0.,10.);
RooRealVar lB2Sig("b2sig","b2sig",0.,-10.,10.);
RooRealVar lC2Sig("c2sig","c2sig",0.,-10.,10.);
RooRealVar lD2Sig("d2sig","d2sig",0.,-10.,10.);

RooRealVar lA3Sig("a3sig","a3sig",0.,  0.,10.);
RooRealVar lB3Sig("b3sig","b3sig",0.,-10.,10.);
RooRealVar lC3Sig("c3sig","c3sig",0.,-10.,10.);
RooRealVar lD3Sig("d3sig","d3sig",0.,-10.,10.);

double startSigma1, minSigma1, maxSigma1;
double startSigma2, minSigma2, maxSigma2;
double startSigma3, minSigma3, maxSigma3; 

// CB variables
double startAlpha1, minAlpha1, maxAlpha1;
double startN1,minN1,maxN1;
double startAlpha2, minAlpha2, maxAlpha2;
double startN2,minN2,maxN2;

double minFrac, maxFrac;
double minFrac2, maxFrac2;

double minMean=0.; double maxMean=1.;
double minMean2=-1.; double maxMean2=0.;

RooRealVar   lAMean2("Amean2","Amean2",0,-5.,5.);
RooRealVar   lBMean2("Bmean2","Bmean2",0,-5.,5.);
RooRealVar   lCMean2("Cmean2","Cmean2",0,-5.,5.);
//RooRealVar   lMean("mean","mean",0,-10.,10.);
RooRealVar   lAMean1("Amean1","Amean1",0,-5.,5.);
RooRealVar   lBMean1("Bmean1","Bmean1",0,-5.,5.);
RooRealVar   lCMean1("Cmean1","Cmean1",0,-5.,5.);
//RooRealVar   lMean1("mean1","mean1",0,-10.,10.);
RooRealVar   lAMean3("Amean3","Amean3",0,-5.,5.);
RooRealVar   lBMean3("Bmean3","Bmean3",0,-5.,5.);
RooRealVar   lCMean3("Cmean3","Cmean3",0,-5.,5.);
RooFormulaVar * lMean2;
RooFormulaVar * lMean1;
RooFormulaVar * lMean;



/*
void validatePDF() {

  RooRealVar lSigma("sigma","sigma",1.5, 0., 2.5); lSigma.setConstant(kTRUE);
  RooRealVar lMean("mean","mean",0,-1.,1.); lMean.setConstant(kTRUE);

  lGaus1 = new RooGaussian("gaus1","gaus1",lRXVar,lMean,lSigma);
  RooDataSet *data1D = lGaus1->generate(lRXVar,1000) ;

  RooPlot* xframe  = lRXVar.frame(Title("Test 1D")) ;
  data1D->plotOn(xframe) ;
  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();
  xframe->Draw() ;

}
*/

/*
void reConstructPDF() {

  cout << "========================" << endl;
  cout << "====== building 1D model ===" << endl;
  cout << "========================" << endl;

  do3G=false;

  double  startSigma1=0.5; 
  double  minSigma1=0.2; 
  double  maxSigma1=1.2;

  double  startSigma2=1.2; 
  double  minSigma2=0.7; 
  double  maxSigma2=3.;

  double  startSigma3 = 3.; //2.
  double  minSigma3 = 2.; 
  double  maxSigma3 = 8.; //12

  //  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,-15.,15.);

  //// 
  //// 
  // ===> MEAN
  //  lR1Mean=RooRealVar("Rmean",  "Rmean",0,minMean,maxMean); // lR1Mean->setConstant(kTRUE); 
  lR1Mean = new RooRealVar("Rmean",  "Rmean",0,-1.,1.); // lR1Mean->setConstant(kTRUE); 
  // ===> SIGMA

  lR1Sigma = new RooRealVar("Rsigma1","RSigma1",startSigma1, minSigma1, maxSigma1 );
  lR2Sigma = new RooRealVar("Rsigma2","RSigma2",startSigma2, minSigma2, maxSigma2 );
  lR3Sigma = new RooRealVar("Rsigma3","RSigma3",startSigma3, minSigma3, maxSigma3 );  if(!do3G) lR3Sigma->setConstant(kTRUE);

  lRGaus1 = new RooGaussian("Rgaus1","Rgaus1",lRXVar,*lR1Mean,*lR1Sigma);
  lRGaus2 = new RooGaussian("Rgaus2","Rgaus2",lRXVar,*lR1Mean,*lR2Sigma);
  lRGaus3 = new RooGaussian("Rgaus3","Rgaus3",lRXVar,*lR1Mean,*lR3Sigma);

  RooDataSet *data1D = lRGaus1->generate(lRXVar,1000) ;

  RooPlot* xframe  = lRXVar.frame(Title("Test 1D")) ;
  data1D->plotOn(xframe) ;
  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();
  xframe->Draw() ;

}
*/

void constructPDFbkg(double lPar, int index) {

  cout << "========================" << endl;
  if(lPar==fU1)  cout << "====== building signal + BKG model === U1 " << endl;
  if(lPar!=fU1)  cout << "====== building sigmal + BKG model === U2 " << endl;
  cout << "========================" << endl;

  //// 
  //// 

  //  TFile *file_ = TFile::Open("file2Dfit_TOPDATA_Y1_3G.root");
  TFile *file_ = TFile::Open("file2Dfit_TOPDATA_all_allM_3G.root");

  TString histo = "hh_";
  if(lPar==fU1) histo +="U1";
  if(lPar!=fU1) histo +="U2";
  histo += "_1_";
  histo += index;
  histo += "__XVar";

  TH1F * h_ = (TH1F*) file_->Get(histo.Data());

  cout << h_ << endl;

  //  RooDataHist bkg_hist("bkghist","bkghist",lRXVar,Import(*h_));
  bkg_hist= new RooDataHist("bkghist","bkghist",lRXVar,h_);
  bkg_pdf = new RooHistPdf("bkg_pdf","bkg_pdf",RooArgSet(lRXVar),*bkg_hist);

  lAbkgFrac.setRange(0.95,1.);
  lAbkgFrac.setVal(0.98);

  sigbkgFrac= new RooFormulaVar("bkgfrac","@0",RooArgSet(lAbkgFrac));

  cout << "sigbkgFrac " << sigbkgFrac->getVal() << endl;

  //  sigbkgfrac.setRange(0.95,1.);
  //  sigbkgfrac.setVal(0.98);

  // Sum the composite signal and background 
  //  model = new RooAddPdf("model","g1+g2+g3+bkg",RooArgList(*lRGAdd,bkg_pdf),sigbkgfrac);
  if(lPar==fU1 && fId==1) model = new RooAddPdf("modelU1Y1","modelU1Y1",*lRGAdd,*bkg_pdf,*sigbkgFrac);
  if(lPar!=fU1 && fId==1) model = new RooAddPdf("modelU2Y1","modelU2Y1",*lRGAdd,*bkg_pdf,*sigbkgFrac);
  if(lPar==fU1 && fId==2) model = new RooAddPdf("modelU1Y2","modelU1Y2",*lRGAdd,*bkg_pdf,*sigbkgFrac);
  if(lPar!=fU1 && fId==2) model = new RooAddPdf("modelU2Y2","modelU2Y2",*lRGAdd,*bkg_pdf,*sigbkgFrac);
  /*
  RooPlot* xframe  = lRXVar.frame(Title("Test 1D")) ;
  RooDataSet *data1D = model->generate(lRXVar,1000) ;
  data1D->plotOn(xframe) ;
  model->plotOn(xframe,LineColor(kBlack)) ;
  // Draws PDF, normalized to plotted data
  //  lRGAdd->plotOn(xframe,LineColor(kRed)); 
  //bkg_pdf.plotOn(xframe,LineColor(kGreen)) ;
  // Plot fractions of normalized PDF
  // model->plotOn(xframe,Components("bkg_pdf"),LineColor(kRed),LineStyle(kDashed)) ;
  // model->plotOn(xframe,Components("RAdd"),LineColor(kGreen),LineStyle(kDotted)) ;
  // Plot GPD-like stacked components
  model->plotOn(xframe,LineColor(kBlack),LineStyle(kDotted),FillColor(kGreen),DrawOption("F")) ;
  //model->plotOn(xframe,Components("bkg_pdf,RAdd"),LineColor(kGreen),LineStyle(kDotted),FillColor(kGreen),DrawOption("F")) ;
  model->plotOn(xframe,Components("bkg_pdf")     ,LineColor(kBlack)  ,LineStyle(kDashed),FillColor(kRed)  ,DrawOption("F")) ;
  data1D->plotOn(xframe) ;
  xframe->Draw(); 

  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();
  xframe->Draw() ;
  c->SaveAs("sumPlot.png");
  */

  return;

}


void constructPDFbkg2D(double lPar) {

  cout << "========================" << endl;
  if(lPar==fU1)  cout << "====== building 2D signal + BKG model === U1 " << endl;
  if(lPar!=fU1)  cout << "====== building 2D sigmal + BKG model === U2 " << endl;
  cout << "========================" << endl;
  //// 
  //// 

  //  TFile *file_ = TFile::Open("file2Dfit_TOPDATA_Y1_3G.root");
  TFile *file_ = TFile::Open("file2Dfit_TOPDATA_all_allM_3G.root");

  TString histo = "hh_";
  if(lPar==fU1) histo +="U1";
  if(lPar!=fU1) histo +="U2";
  histo += "_1__pt";
  //  histo += index;
  histo += "_XVar";

  TH2F * h_ = (TH2F*) file_->Get(histo.Data());

  cout << h_ << endl;

  lAbkgFrac.setRange(0.95,1.);
  lAbkgFrac.setVal(0.98);

  sigbkgFrac2D= new RooFormulaVar("bkgfrac","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lAbkgFrac,lBbkgFrac,lCbkgFrac,lRPt,lDbkgFrac));

  //  RooDataHist bkg_hist("bkghist","bkghist",lRXVar,Import(*h_));
  bkg_hist2D= new RooDataHist("bkghist2D","bkghist2D",RooArgSet(lRPt,lRXVar),h_);
  bkg_pdf2D = new RooHistPdf("bkg_pdf2D","bkg_pdf2D",RooArgSet(lRPt,lRXVar),*bkg_hist2D);

  //  sigbkgfrac.setRange(0.95,1.);
  //  sigbkgfrac.setVal(0.98);

  // Sum the composite signal and background 
  //  model = new RooAddPdf("model","g1+g2+g3+bkg",RooArgList(*lRGAdd,bkg_pdf),sigbkgfrac);

  if(lPar==fU1 && fId==1) model2D = new RooAddPdf("model2DU1Y1","model2DU1Y1",*lGAdd,*bkg_pdf,*sigbkgFrac2D);
  if(lPar!=fU1 && fId==1) model2D = new RooAddPdf("model2DU2Y1","model2DU2Y1",*lGAdd,*bkg_pdf,*sigbkgFrac2D);
  if(lPar==fU1 && fId==2) model2D = new RooAddPdf("model2DU1Y2","model2DU1Y2",*lGAdd,*bkg_pdf,*sigbkgFrac2D);
  if(lPar!=fU1 && fId==2) model2D = new RooAddPdf("model2DU2Y2","model2DU2Y2",*lGAdd,*bkg_pdf,*sigbkgFrac2D);


  return;

}


void constructPDF(double lPar) {

  //  range_min = 10;
  //  range_max = 15;
  binSize=1.; // this is 1 GeV for Zpt
  //double range_min = fZPtMin+binSize;
  //double range_max = fZPtMax-binSize;
  //  range_min = fZPtMin;
  //  range_max = fZPtMax;
  minRangeSigma=rangeMinXVar;
  maxRangeSigma=rangeMaxXVar;

  //  double range_min = fZPtMin;
  //double range_max = fZPtMax;

  // this is the range of th X axis
  //  double minRangeSigma=-10.; double maxRangeSigma=10.;

  if(doAbsolute) { minRangeSigma=-50.; maxRangeSigma=50.;}

  // this is the mean
  if(doAbsolute) { minMean=-1.; maxMean=1.; }

  // this is the p0 value of the sigma
  // should not be negative and the
  startSigma1=0.6; 
  minSigma1=0.2; 
  maxSigma1=1.2;

  startSigma2=1.5; 
  minSigma2=1.2 ; 
  maxSigma2=8.;

  if(doAbsolute) { startSigma1=3*0.5;  startSigma2=3*1.2; }
  if(doAbsolute) { minSigma1=0.;  maxSigma1=10.;}
  if(doAbsolute) { minSigma2=3.;  maxSigma2=40.;}

  if(do3G) {

    startSigma1=0.4;
    minSigma1=0.2;
    maxSigma1=1.2;

    startSigma2=1.2;
    minSigma2=0.5;
    maxSigma2=2.;

    startSigma3 = 2.1; //2.
    minSigma3 = 1.2;
    maxSigma3 = 5.; //12

    minFrac = 0.;
    maxFrac = 1.;
    minFrac2 = 0.4;
    maxFrac2 = 1.;
    //    if(lPar!=fU1) maxFrac = 0.6;

  }

  if(do1CB2G) {

    // Those are CHRIstos initial values
    startSigma1=0.4; 
    minSigma1=0.2; 
    maxSigma1=1.2;

    startSigma2=1.2; 
    minSigma2=0.6; 
    maxSigma2=2.;

    startSigma3 = 2.1; //2.
    minSigma3 = 1.5; 
    maxSigma3 = 6.; //12

    minFrac = 0.;
    maxFrac = 1.;
    if(lPar!=fU1) maxFrac = 0.6;

    startAlpha1=2.; //----|CHRISTOS:
    minAlpha1=2.; //-------|INITIAL
    maxAlpha1=2.; //------|VALUES
    //---------------------|FOR THE
    startN1=4.6; //--------|RooRealVar
    minN1=4.4; //----------|PARAMETERS
    maxN1=5.2; //----------|OF
    //---------------------|THE
    startAlpha2=2.; //----|DOUBLE
    minAlpha2=2.; //------|CRYSTAL
    maxAlpha2=2.; //------|BALL
    //---------------------|IN THE
    startN2=6.7; //--------|WANTED
    minN2=6.5; //----------|FIT
    maxN2=7.2; //----------|1CB+2GAUSSIANS

  }

  cout << "========================" << endl;
  if(lPar==fU1)  cout << "====== building 1D model === U1 " << endl;
  if(lPar!=fU1)  cout << "====== building 1D model === U2 " << endl;
  cout << "========================" << endl;
  //// 
  //// 

  lR1Mean.setRange(minMean,maxMean); lR1Mean.setVal(0);
  lR1MeanLarge.setRange(minMean2,maxMean2); lR1MeanLarge.setVal(0.);
  lR1MeanVeryLarge.setRange(minMean2,maxMean2); lR1MeanVeryLarge.setVal(0.);

  if(lPar!=fU1) lR1Mean.setConstant(kTRUE);
  if(lPar!=fU1) lR1MeanLarge.setConstant(kTRUE);
  if(lPar!=fU1) lR1MeanVeryLarge.setConstant(kTRUE);

  lRAFrac.setRange(minFrac,maxFrac); lRAFrac.setVal(0.35);
  lRBFrac.setRange(minFrac2,maxFrac2); lRBFrac.setVal(0.65);

  lRAlpha1.setRange(minAlpha1, maxAlpha1); lRAlpha1.setVal(startAlpha1);
  lRAlpha2.setRange(minAlpha2, maxAlpha2); lRAlpha2.setVal(startAlpha2);

  lRN1.setRange(minN1, maxN1); lRN1.setVal(startN1);
  lRN2.setRange(minN2, maxN2); lRN2.setVal(startN2);

  lR1Sigma.setRange(minSigma1, maxSigma1); lR1Sigma.setVal(startSigma1);
  lR2Sigma.setRange(minSigma2, maxSigma2); lR2Sigma.setVal(startSigma2);
  lR3Sigma.setRange(minSigma3, maxSigma3); lR3Sigma.setVal(startSigma3);

  // ===> Functions and FRACTION

  if(do1CB2G) {
    lRCB= new RooDoubleCBFast("RCB1","RCB1",lRXVar,lR1Mean,lR1Sigma,lRAlpha1,lRN1,lRAlpha2,lRN2); //CHRISTOS : lRGaus1 -> lRCB (NOTE! DON'T CHANGE THE "RGaus1" OTHERWISE NO CHI2 WILL BE GIVEN
    lRGaus2= new RooGaussian("Rgaus2","Rgaus2",lRXVar,lR1MeanLarge,lR2Sigma);
    lRGaus3= new RooGaussian("Rgaus3","Rgaus3",lRXVar,lR1MeanVeryLarge,lR3Sigma);

    lR1Frac= new RooFormulaVar("R1frac","@0",RooArgSet(lRAFrac));
    lRFrac= new RooFormulaVar("Rfrac","@0",RooArgSet(lRBFrac));
    lRGAdd= new RooAddPdf("RAdd","RAdd",RooArgList(*lRCB,*lRGaus2,*lRGaus3),RooArgList(*lR1Frac,*lRFrac),kTRUE); //CHRISTOS: lRGaus1 -> lRCB
  } else if(do3G) {
    ///<+++++ 3G 

    lRGaus1= new RooGaussian("Rgaus1","Rgaus1",lRXVar,lR1Mean,lR1Sigma);
    lRGaus2= new RooGaussian("Rgaus2","Rgaus2",lRXVar,lR1MeanLarge,lR2Sigma);
    lRGaus3= new RooGaussian("Rgaus3","Rgaus3",lRXVar,lR1MeanLarge,lR3Sigma);
    //  lRGaus3= new RooGaussian("Rgaus3","Rgaus3",lRXVar,lR1MeanVeryLarge,lR3Sigma);

    // defaults
    lR1Frac= new RooFormulaVar("R1frac","@0",RooArgSet(lRAFrac));
    lRFrac= new RooFormulaVar("Rfrac"  ,"(-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3)))",RooArgSet(*lR1Frac,lR1Sigma,lR2Sigma,lR3Sigma)); 
    
    /*
    lR1Frac= new RooFormulaVar("R1frac","@0",RooArgSet(lRAFrac));
    if(lPar==fU1) lRFrac= new RooFormulaVar("Rfrac","@0",RooArgSet(lRBFrac));
    if(lPar!=fU1) lRFrac= new RooFormulaVar("Rfrac"  ,"(-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3)))",RooArgSet(*lR1Frac,lR1Sigma,lR2Sigma,lR3Sigma));
    */
    //    RooFormulaVar lR1Frac_("R1frac","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lAFrac,lBFrac,lCFrac,lRPt,lDFrac));
    //    RooFormulaVar lRFrac_("Rfrac"  ,"-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3))",RooArgSet(lR1Frac,lR1Sigma,lR2Sigma,lR3Sigma)); 
    lRGAdd= new RooAddPdf("RAdd","RAdd",RooArgList(*lRGaus1,*lRGaus2,*lRGaus3),RooArgList(*lR1Frac,*lRFrac),kTRUE);
  } else {
    ///<+++++ 2G 
    //    lR1Frac=RooRealVar("R1frac"  ,"R1frac"  , 0.5, 1., 0.9);  
    lR1Frac= new RooFormulaVar("R1frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(lR1Sigma,lR2Sigma)); //this only for the pull ; for abolute keep independent
    //  RooFormulaVar lR1Frac  ("R1frac"  ,"(@1*@1-1)/(@1*@1-@0*@0)",RooArgSet(lR1Sigma,lR2Sigma)); //this only for the pull ; for abolute keep independent ; for squares
    lRGAdd= new RooAddPdf("RAdd","RAdd",*lRGaus1,*lRGaus2,*lR1Frac);  
  }

  //  lRGAdd->mustBeExtended();  

  double fR2=lRFrac->getVal()*(1-lR1Frac->getVal());

  cout << "== 1D" << endl;
  cout << "  lR1Frac=" << lR1Frac->getVal() << " lRFrac=" << lRFrac->getVal() << endl;
  cout << " f1=" << lR1Frac->getVal() << " f2=" << fR2 << endl;
  if((lR1Frac->getVal()+fR2)>1 || (lR1Frac->getVal()+fR2)<0) cout << " wrong sum of the fraction"<< endl;
  cout << "  sigmaR1=" << lR1Sigma.getVal() << " sigmaR2=" << lR2Sigma.getVal() << " sigmaR3=" << lR3Sigma.getVal() << endl;

  cout << "========================" << endl;
  cout << "====== 1D model done ===" << endl;
  cout << "========================" << endl;

  /*
  RooPlot* xframe  = lRXVar.frame(Title("Test 1D")) ;
  RooDataSet *data1D = lRGAdd->generate(lRXVar,1000) ;
  data1D->plotOn(xframe) ;
  
  lRGAdd->plotOn(xframe) ;
  lRGaus1->plotOn(xframe,LineColor(kRed)) ;
  lRGaus2->plotOn(xframe,LineColor(kMagenta)) ;
  lRGaus3->plotOn(xframe,LineColor(kGreen)) ;
  xframe->Draw(); 

  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();
  xframe->Draw() ;
  */

  return;

}


void constructPDF2d(TF1 * FitSigma1, TF1 * FitSigma2, TF1 * FitSigma3,  TF1 * FitFrac, TF1 * FitFrac2, TF1 *FitMean1, TF1 *FitMean2, /*TF1 *FitMean3,*/ double lPar) {

  //  double nSigma=10;

  cout << "========================" << endl;
  if(lPar==fU1)  cout << "====== building 2D model === U1 " << endl;
  if(lPar!=fU1)  cout << "====== building 2D model === U2 " << endl;
  cout << "========================" << endl;

  if(lPar!=fU1) {
    lAMean1.setVal(0);  lAMean1.setConstant(kTRUE);
    lBMean1.setVal(0);  lBMean1.setConstant(kTRUE);
    lCMean1.setVal(0);  lCMean1.setConstant(kTRUE);
    lAMean2.setVal(0);  lAMean2.setConstant(kTRUE);
    lBMean2.setVal(0);  lBMean2.setConstant(kTRUE);
    lCMean2.setVal(0);  lCMean2.setConstant(kTRUE);
    //    lAMean3.setVal(0);  lAMean3.setConstant(kTRUE);
    //    lBMean3.setVal(0);  lBMean3.setConstant(kTRUE);
    //    lCMean3.setVal(0);  lCMean3.setConstant(kTRUE);
  } else {
    lAMean1.setVal(FitMean1->GetParameter(0));
    lBMean1.setVal(FitMean1->GetParameter(1));
    lCMean1.setVal(FitMean1->GetParameter(2));
    lAMean2.setVal(FitMean2->GetParameter(0));
    lBMean2.setVal(FitMean2->GetParameter(1));
    lCMean2.setVal(FitMean2->GetParameter(2));
    //    lAMean3.setVal(FitMean3->GetParameter(0));
    //    lBMean3.setVal(FitMean3->GetParameter(1));
    //    lCMean3.setVal(FitMean3->GetParameter(2));
  }

  lMean = new RooFormulaVar("mean"  ,"@0+@1*@3+@2*@3*@3",RooArgSet(lAMean1,lBMean1,lCMean1,lRPt));
  lMean1 = new RooFormulaVar("mean1"  ,"@0+@1*@3+@2*@3*@3",RooArgSet(lAMean2,lBMean2,lCMean2,lRPt));
  //  lMean2 = new RooFormulaVar("mean2"  ,"@0+@1*@3+@2*@3*@3",RooArgSet(lAMean3,lBMean3,lCMean3,lRPt));

  //  if(!fData && !doMad && (!doPosW && doNegW)) nSigma =30;// "Wneg";
  //  if(!fData && !doMad && (doPosW && !doNegW)) nSigma =30;// "Wpos";
  //  if(!fData && !doMad) nSigma =30;

  /*
  lAFrac.setRange(FitFrac->GetParameter(0)-nSigma*FitFrac->GetParError(0),FitFrac->GetParameter(0)+nSigma*FitFrac->GetParError(0));  lAFrac.setVal(FitFrac->GetParameter(0));
  lBFrac.setRange(FitFrac->GetParameter(1)-nSigma*FitFrac->GetParError(1),FitFrac->GetParameter(1)+nSigma*FitFrac->GetParError(1));  lBFrac.setVal(FitFrac->GetParameter(1));//  lBFrac.setConstant(kTRUE);
  lCFrac.setRange(FitFrac->GetParameter(2)-nSigma*FitFrac->GetParError(2),FitFrac->GetParameter(2)+nSigma*FitFrac->GetParError(2));  lCFrac.setVal(FitFrac->GetParameter(2));//  lCFrac.setConstant(kTRUE);
  */

  lAFrac.setVal(FitFrac->GetParameter(0));
  lBFrac.setVal(FitFrac->GetParameter(1));
  lCFrac.setVal(FitFrac->GetParameter(2));
  //  lDFrac.setVal(FitFrac->GetParameter(3));
  lDFrac.setVal(0);  lDFrac.setRange(0.,0.); lDFrac.setConstant(kTRUE);

  lA1Frac.setVal(FitFrac2->GetParameter(0));
  lB1Frac.setVal(FitFrac2->GetParameter(1));
  lC1Frac.setVal(FitFrac2->GetParameter(2));
  //  lDFrac.setVal(FitFrac->GetParameter(3));
  lD1Frac.setVal(0);  lD1Frac.setRange(0.,0.); lD1Frac.setConstant(kTRUE);

  ////////
  //////// 
  //////// 
  // ===> SIGMA  

  lA1Sig.setVal(FitSigma1->GetParameter(0)); //lA1Sig.setRange(FitSigma1->GetParameter(0)-nSigma*FitSigma1->GetParError(0),FitSigma1->GetParameter(0)+nSigma*FitSigma1->GetParError(0));
  lB1Sig.setVal(FitSigma1->GetParameter(1)); //lB1Sig.setRange(FitSigma1->GetParameter(1)-nSigma*FitSigma1->GetParError(1),FitSigma1->GetParameter(1)+nSigma*FitSigma1->GetParError(1));
  lC1Sig.setVal(FitSigma1->GetParameter(2)); //lC1Sig.setRange(FitSigma1->GetParameter(2)-nSigma*FitSigma1->GetParError(2),FitSigma1->GetParameter(2)+nSigma*FitSigma1->GetParError(2));
  lD1Sig.setVal(0.);          lD1Sig.setRange(0. , 0.); lD1Sig.setConstant(kTRUE); 

  lA2Sig.setVal(FitSigma2->GetParameter(0)); //lA2Sig.setRange(FitSigma2->GetParameter(0)-nSigma*FitSigma2->GetParError(0),FitSigma2->GetParameter(0)+nSigma*FitSigma2->GetParError(0));
  lB2Sig.setVal(FitSigma2->GetParameter(1)); //lB2Sig.setRange(FitSigma2->GetParameter(1)-nSigma*FitSigma2->GetParError(1),FitSigma2->GetParameter(1)+nSigma*FitSigma2->GetParError(1));
  lC2Sig.setVal(FitSigma2->GetParameter(2)); //lC2Sig.setRange(FitSigma2->GetParameter(2)-nSigma*FitSigma2->GetParError(2),FitSigma2->GetParameter(2)+nSigma*FitSigma2->GetParError(2));
  lD2Sig.setVal(0.);          lD2Sig.setRange(0. , 0.); lD2Sig.setConstant(kTRUE); 

  lA3Sig.setVal(FitSigma3->GetParameter(0)); //lA3Sig.setRange(FitSigma3->GetParameter(0)-nSigma*FitSigma3->GetParError(0),FitSigma3->GetParameter(0)+nSigma*FitSigma3->GetParError(0));
  lB3Sig.setVal(FitSigma3->GetParameter(1)); //lB3Sig.setRange(FitSigma3->GetParameter(1)-nSigma*FitSigma3->GetParError(1),FitSigma3->GetParameter(1)+nSigma*FitSigma3->GetParError(1));
  lC3Sig.setVal(FitSigma3->GetParameter(2)); //lC3Sig.setRange(FitSigma3->GetParameter(2)-nSigma*FitSigma3->GetParError(2),FitSigma3->GetParameter(2)+nSigma*FitSigma3->GetParError(2));
  lD3Sig.setVal(0.);          lD3Sig.setRange(0. , 0.); lD3Sig.setConstant(kTRUE);

  /*
  // ===> small gaussian : pol0
  lA1Sig.setVal(startSigma1); lA1Sig.setRange(minSigma1 , maxSigma1);
  //  lB1Sig.setVal(0.01);          lB1Sig.setRange(0. , 0.1);
  //  lC1Sig.setVal(0.);          lC1Sig.setRange(-0.01 , 0.01);
  //  if(!doMad && !fData && lPar==fU1) lC1Sig->setConstant(kTRUE);  // POWHEG guessed from Z 
  lD1Sig.setVal(0.);          lD1Sig.setRange(0. , 0.); lD1Sig.setConstant(kTRUE);
  //  if(lPar!=fU1) {lB1Sig.setVal(0.); lB1Sig.setConstant(kTRUE);}
  //  if(lPar==fU1) {lB1Sig.setVal(0.05); lB1Sig.setRange(0. , 0.1);}
  lB1Sig.setVal(0.); lB1Sig.setConstant(kTRUE);
  lC1Sig.setVal(0.); lC1Sig.setConstant(kTRUE);

  // ===> medium gaussian : pol0
  lA2Sig.setVal(startSigma2); lA2Sig.setRange(minSigma2 , maxSigma2); 
  //  lB2Sig.setVal(0.01);        lB2Sig.setRange(0. , 0.1);
  //  lC2Sig.setVal(0.001);          lC2Sig.setRange(-0.01 , 0.01); 
  //  if(!doMad && !fData && lPar==fU1) lC1Sig->setConstant(kTRUE);  // POWHEG guessed from Z 
  lD2Sig.setVal(0.);          lD2Sig.setRange(0. , 0.); lD2Sig.setConstant(kTRUE); 
  //  if(lPar!=fU1) {lB2Sig.setVal(0.); lB2Sig.setConstant(kTRUE);}
  //  if(lPar==fU1) {lB2Sig.setVal(0.05); lB2Sig.setRange(0. , 0.1);}
  lB2Sig.setVal(0.); lB2Sig.setConstant(kTRUE);
  lC2Sig.setVal(0.); lC2Sig.setConstant(kTRUE);

  // ===> large gaussian : pol2
  lA3Sig.setVal(startSigma3); lA3Sig.setRange(minSigma3 , maxSigma3);                
  //  lB3Sig.setVal(0.05);        lB3Sig.setRange(0. , 0.1);                             
  //  lC3Sig.setVal(0.005);       lC3Sig.setRange(0. , 0.01);                            
  //  if(!doMad && !fData && lPar==fU1) lC1Sig->setConstant(kTRUE);  // POWHEG guessed from Z 
  lD3Sig.setVal(0.);          lD3Sig.setRange(0. , 0.); lD3Sig.setConstant(kTRUE);   
  //  if(lPar!=fU1) {lB3Sig.setVal(0.); lB3Sig.setConstant(kTRUE);}
  //  if(lPar==fU1) {lB3Sig.setVal(0.3);}
  lB3Sig.setVal(0.); lB3Sig.setConstant(kTRUE);
  lC3Sig.setVal(0.); lC3Sig.setConstant(kTRUE);
  */

  ////////
  //////// 
  //////// 
  l1Sigma = new RooFormulaVar("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
  l2Sigma = new RooFormulaVar("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
  l3Sigma = new RooFormulaVar("sigma3","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA3Sig,lB3Sig,lC3Sig,lRPt,lD3Sig));
 
  //  l1Sigma.setRange(minSigma1, maxSigma1); l1Sigma.setVal(startSigma1);
  //  l2Sigma.setRange(minSigma2, maxSigma2); l2Sigma.setVal(startSigma2);
  //  l3Sigma.setRange(minSigma3, maxSigma3); l3Sigma.setVal(startSigma3);

  lGaus1 = new RooGaussian("gaus1","gaus1",lRXVar,*lMean,*l1Sigma);
  lGaus2 = new RooGaussian("gaus2","gaus2",lRXVar,*lMean1,*l2Sigma);
  lGaus3 = new RooGaussian("gaus3","gaus3",lRXVar,*lMean1,*l3Sigma);
  //  lGaus3 = new RooGaussian("gaus3","gaus3",lRXVar,*lMean2,*l3Sigma);

  if(do3G) {
    ///<+++++ 3G 

    // default
    l1Frac= new RooFormulaVar("frac1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lAFrac,lBFrac,lCFrac,lRPt,lDFrac));
    lFrac= new RooFormulaVar("frac"  ,"(-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3)))",RooArgSet(*l1Frac,*l1Sigma,*l2Sigma,*l3Sigma));

    /*   
    l1Frac= new RooFormulaVar("frac1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lAFrac,lBFrac,lCFrac,lRPt,lDFrac));
    if(lPar==fU1) lFrac= new RooFormulaVar("frac","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Frac,lB1Frac,lC1Frac,lRPt,lD1Frac));
    if(lPar!=fU1) lFrac= new RooFormulaVar("frac"  ,"(-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3)))",RooArgSet(*l1Frac,*l1Sigma,*l2Sigma,*l3Sigma)); // free parameter for the doAbsolute=true
    */

    //  RooAddPdf lGAdd("Add","Add",RooArgList(lGaus1,lGaus2,lGaus3),RooArgList(l1Frac,lFrac),kTRUE);

    if(lPar==fU1 && fId==1) lGAdd = new RooAddPdf("AddU1Y1","AddU1Y1",RooArgList(*lGaus1,*lGaus2,*lGaus3),RooArgList(*l1Frac,*lFrac),kTRUE);
    if(lPar!=fU1 && fId==1) lGAdd = new RooAddPdf("AddU2Y1","AddU2Y1",RooArgList(*lGaus1,*lGaus2,*lGaus3),RooArgList(*l1Frac,*lFrac),kTRUE);
    if(lPar==fU1 && fId==2) lGAdd = new RooAddPdf("AddU1Y2","AddU1Y2",RooArgList(*lGaus1,*lGaus2,*lGaus3),RooArgList(*l1Frac,*lFrac),kTRUE);
    if(lPar!=fU1 && fId==2) lGAdd = new RooAddPdf("AddU2Y2","AddU2Y2",RooArgList(*lGaus1,*lGaus2,*lGaus3),RooArgList(*l1Frac,*lFrac),kTRUE);
    //RooAbsReal
  } else {
    ///<+++++ 2G 
    lFrac= new RooFormulaVar("frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(*l1Sigma,*l2Sigma));
    //  RooFormulaVar lFrac  ("frac"  ,"(@1*@1-1.)/(@1*@1-@0*@0)",RooArgSet(l1Sigma,l2Sigma));   // for squares   
    lGAdd= new RooAddPdf("Add","Add",RooArgList(*lGaus1,*lGaus2),RooArgList(*lFrac),kTRUE); ///<+++++ 2G
  }

  //  lGAdd->mustBeExtended();  

  double f2=lFrac->getVal()*(1-l1Frac->getVal());

  cout << "== 2D" << endl;
  cout << "  l1Frac=" << l1Frac->getVal() << " lFrac=" << lFrac->getVal() << endl;
  cout << " f1=" << l1Frac->getVal() << " f2=" << f2 << endl;
  if((l1Frac->getVal()+f2)>1 || (l1Frac->getVal()+f2)<0) cout << " wrong sum of the fraction"<< endl;
  cout << "  sigma1=" << l1Sigma->getVal() << " sigma2=" << l2Sigma->getVal() << " sigma3=" << l3Sigma->getVal() << endl;


  RooArgSet* comps = lGAdd->getComponents() ;
  comps->Print();
  //  RooArgSet::Add_components = (Add,gaus1,sigma1,frac1,gaus2,sigma2,Add_recursive_fraction_gaus2,frac,sigma3,gaus3,Add_recursive_fraction_gaus3)
  RooAbsArg* frac1_ = comps->find("frac1") ;
  RooArgSet* frac1Var1 = frac1_->getVariables() ;
  frac1Var1->Print() ;
  RooAbsArg* frac_ = comps->find("frac") ;
  RooArgSet* fracVar = frac_->getVariables() ;
  fracVar->Print() ;

  /*
  // Generate a data sample of 1000 events in x from model
  RooDataSet *data = lGAdd->generate(lRXVar,1000) ;

  RooPlot* xframe  = lRXVar.frame(Title("Test 2D")) ;
  data->plotOn(xframe) ;
  lGAdd->plotOn(xframe) ;
  lGaus1->plotOn(xframe,LineColor(kRed)) ;
  lGaus2->plotOn(xframe,LineColor(kMagenta)) ;
  lGaus3->plotOn(xframe,LineColor(kGreen)) ;

  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();
  xframe->Draw() ;
  */

  return;

}


void diagoResults(bool doU1=false) {

  TFile *file_ = TFile::Open("recoilfits/recoilfit_JAN24_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root");

  TString namePDF="";
  if(doU1) namePDF="AddU1";
  if(!doU1) namePDF="AddU2";
  TString nameRooFitResults="";
  if(doU1) nameRooFitResults="fitresult_AddU1_Crapsky0_U1_2D";
  if(!doU1) nameRooFitResults="fitresult_AddU2_Crapsky0_U2_2D";

  RooAddPdf* pdfOriginal = (RooAddPdf*) file_->Get(namePDF.Data());
  RooFitResult* frOriginal = (RooFitResult*) file_->Get(nameRooFitResults.Data());

  //  RooWorkspace *w;
  RooWorkspace *w = new RooWorkspace("w","w");
  w->import(*pdfOriginal,Silence());

  PdfDiagonalizer * diago = new PdfDiagonalizer("eig", w, *frOriginal);
  RooAbsPdf* newPdf = diago->diagonalize(*pdfOriginal);

  //  w->Print("tV");
  /*
  cout << " original parameters " << endl;
  parameters_->Print();

  cout << " diagonalized parameters " << endl;
  eigenVars_->Print();
  replacements_.Print();
  */

  //original parameters
  //RooArgList:: = (AFrac,BFrac,CFrac,a1sig,a2sig,a3sig,b1sig,b2sig,b3sig,c1sig,c2sig,c3sig)
  //diagonalized parameters
  //RooArgList:: = (eig_eigAFrac,eig_eigBFrac,eig_eigCFrac,eig_eiga1sig,eig_eiga2sig,eig_eiga3sig,eig_eigb1sig,eig_eigb2sig,eig_eigb3sig,eig_eigc1sig,eig_eigc2sig,eig_eigc3sig)

  RooRealVar* mypt=w->var("pt");
  //  mypt->setVal(10);
  mypt->setVal(20);

  cout << " -------- plotting pdf ------ " << endl;

  // make plot in the recoil space (integrate on PT)
  RooPlot *lFrame2D = lRXVar.frame(Title("frame2D")) ;
  /*
  for ( int i=0; i<parameters_->getSize(); i++ ) {
    cout << " i " << i << endl;
    RooRealVar* myVar=w->var(Form("eig_eig%d",i));
    //  cout << "====> eig_eigLin_AFrac " << myVarLin->getVal() << endl;
    myVar->setVal(10);
    newPdf->plotOn(lFrame2D,RooFit::LineColor(kMagenta+i));
    myVar->setVal(-10);
    newPdf->plotOn(lFrame2D,RooFit::LineColor(kViolet+i));
  }
  */
  pdfOriginal->plotOn(lFrame2D,RooFit::LineColor(kGreen),RooFit::LineStyle(kDashed));
  newPdf->plotOn(lFrame2D,RooFit::LineColor(kBlue),RooFit::LineStyle(kDashed));

  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400);
  c->cd();
  lFrame2D->Draw();
 
  c->SaveAs("newOldPDF.png");

  delete pdfOriginal;
  delete frOriginal;
  delete newPdf;
  delete w;

  return;

}


//double triGausInvGraph(double iPVal, double Zpt, RooAddPdf *pdfMC, RooAddPdf *pdfDATA, RooWorkspace *wMC, RooWorkspace *wDATA) {
//double triGausInvGraph(double iPVal, double Zpt, RooAbsPdf *pdfMC, RooAbsPdf *pdfDATA, RooWorkspace *wMC, RooWorkspace *wDATA) {
double triGausInvGraph(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA) {
  
  double pVal=(-999.);
  if(iPVal>=5) { pVal=iPVal; return pVal; }
  if(iPVal<=-5) { pVal=iPVal; return pVal; }

  //  cout << " inside triGausInvGraph " << endl;

  RooRealVar* myptm= wMC->var("pt");
  RooRealVar* myptd= wDATA->var("pt");
  RooRealVar* myXm = wMC->var("XVar");
  RooRealVar* myXd = wDATA->var("XVar");
 
  myptm->setVal(Zpt);
  myptd->setVal(Zpt);

  RooRealVar* myptmCDF = (RooRealVar*) pdfMCcdf->getVariables()->find("pt");
  RooRealVar* myptdCDF = (RooRealVar*) pdfDATAcdf->getVariables()->find("pt");

  myptmCDF->setVal(Zpt);
  myptdCDF->setVal(Zpt);

  RooRealVar* myXmCDF = (RooRealVar*) pdfMCcdf->getVariables()->find("XVar");
  RooRealVar* myXdCDF = (RooRealVar*) pdfDATAcdf->getVariables()->find("XVar");

  //  myXmCDF->setVal(Zpt);
  //  myXdCDF->setVal(Zpt);


  // cout << "PRINTING THE DATA workspace" << endl;
  //  wDATA->Print("tV");

  //  RooAbsReal* pdfMCcdf = pdfMC->createCdf(*myXm);
  //  RooAbsReal* pdfDATAcdf = pdfDATA->createCdf(*myXd);

  /*
  cout << " --------- MC ----------" << endl;
  wMC->Print("tV"); ;
  cout << " --------- DATA --------" << endl;
  wDATA->Print("tV"); ;
  */

  /*
  //  TF1* funcMCpdf = (TF1*) pdfMC->asTF( RooArgList(lRXVar));
  TF1* funcMCcdf = (TF1*) pdfMCcdf->asTF( RooArgList(*myXm));
  //  TF1* funcDATApdf = (TF1*) pdfDATA->asTF( RooArgList(lRXVar));
  TF1* funcDATAcdf = (TF1*) pdfDATAcdf->asTF( RooArgList(*myXd));

  TGraph *gr_mc = new TGraph(funcMCcdf);
  TGraph *gr_data = new TGraph(funcDATAcdf);
  TGraph *gr_data_inverse = new TGraph(gr_data->GetN(),gr_data->GetY(), gr_data->GetX());
 
  double pVal=gr_data_inverse->Eval(gr_mc->Eval(iPVal));
  cout << "ORIGINAL MC: " << iPVal ;
  cout << " FROM INVERSE graph: " << gr_data_inverse->Eval(gr_mc->Eval(iPVal)) << endl;
  */

  myXmCDF->setVal(iPVal);
  //  double pVal=pdfDATAcdf->findRoot(*myXd,myXd->getMin(),myXd->getMax(),pdfMCcdf->evaluate());
  pVal=pdfDATAcdf->findRoot(*myXdCDF,myXdCDF->getMin(), myXdCDF->getMax(),pdfMCcdf->getVal());

  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " from findRoot " << pVal << endl;

  if(iPVal<-5) {
    cout << "lowRange "<<  endl;
    cout << "ORIGINAL MC: " << iPVal ;
    cout << " from findRoot " << pVal << endl;
  }

  if(pVal==0) {
    cout << "null "<<  endl;
    cout << " ORIGINAL MC: " << iPVal ;
    cout << "  from findRoot " << pVal << endl;
  }
  if(pVal==1) {
    cout << "unity "<<  endl;
    cout << " ORIGINAL MC: " << iPVal ;
    cout << "  from findRoot " << pVal << endl;
  }

  if(iPVal<-5 || pVal==0 || pVal==1)  {
    cout << " ======================= " << endl;
  }

  // add protection for outlier since I tabulated up to 5
  if(pVal>=rangeMaxXVar) pVal=iPVal;
  if(pVal<=rangeMinXVar) pVal=iPVal;

  /*
  if((iPVal-pVal)>0.5) {

    cout << " ------------------------------------------------------- " << endl;
    cout << " =====> DIFF: " << " ZPt=" << Zpt << endl;
    cout << " initial pU1 = " << iPVal << " after = " << pVal << endl;
  }
  */

  /*
  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  //  c->SetLogy(1);
  c->cd();
    
  hMC1->SetLineColor(kMagenta);
  hMC1->Draw("hist");
  hMC->SetLineColor(kRed);
  hMC->Draw("hist same");

  hDATA1->SetLineColor(kGreen);
  hDATA1->Draw("hist same");
  hDATA->SetLineColor(kGreen+1);
  hDATA->Draw("hist same");

  gr_mc->SetLineColor(kRed);
  gr_mc->SetMarkerStyle(22);
  gr_mc->SetMarkerColor(kRed);
  gr_mc->Draw("A P L");

  gr_data->SetLineColor(kGreen+1);
  gr_data->SetMarkerStyle(23);
  gr_data->SetMarkerColor(kGreen+1);
  gr_data->Draw("P L same ");
  */

  //  gr_data_inverse->SetLineColor(kCyan);
  //  gr_data_inverse->SetMarkerColor(kCyan);
  //  gr_data_inverse->Draw("P L same "
  //  c->SaveAs("Graph.png");

  //  delete wMC;
  //  delete wDATA;
  //  delete myptm;
  //  delete myptd;
  //  delete myptm;
  //  delete myptd;
  //  delete myXm;
  //  delete myXd;
  //  delete pdfMCcdf;
  //  delete pdfDATAcdf;
  //  delete funcMCcdf;
  //  delete funcDATAcdf;
  //  delete gr_mc;
  //  delete gr_data;
  //  delete gr_data_inverse;

  myptd->setVal(0 );
  myptm->setVal(0 );
  myXm->setVal(0 );
  myXd->setVal(0 );

  return pVal;

}

void applyTriGausInv(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
		     double iLepPt,double iLepPhi,/*TRandom3 *iRand,*/
		     TF1 *iU1Default,
		     TF1 *iU1RZMCFit,  TF1 *iU1RZDatFit, //scale
		     TF1 *iU1MSZMCFit, TF1 *iU1MSZDatFit, // RMS U1
		     TF1 *iU2MSZMCFit, TF1 *iU2MSZDatFit, // RMS U2
		     RooAbsReal* ipdfMCU1, RooWorkspace *iwMCU1,
		     RooAbsReal* ipdfDATAU1, RooWorkspace *iwDATAU1,
		     RooAbsReal* ipdfMCU2, RooWorkspace *iwMCU2,
		     RooAbsReal* ipdfDATAU2, RooWorkspace *iwDATAU2
		     /*
		     RooAbsPdf* ipdfMCU1, RooWorkspace *iwMCU1,
		     RooAbsPdf* ipdfDATAU1, RooWorkspace *iwDATAU1,
		     RooAbsPdf* ipdfMCU2, RooWorkspace *iwMCU2,
		     RooAbsPdf* ipdfDATAU2, RooWorkspace *iwDATAU2
		     */
		     ) {

  double lRescale  = sqrt((TMath::Pi())/2.);
  //  double lRescale  = 1;     // for squares

  double pDefU1    = iU1Default->Eval(iGenPt);

  double pDU1       = iU1RZDatFit ->Eval(iGenPt); // U1 average scale
  double pDRMSU1    = iU1MSZDatFit->Eval(iGenPt)*lRescale; // U1 average RMS
  double pDRMSU2    = iU2MSZDatFit->Eval(iGenPt)*lRescale; // U2 average RMS

  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMRMSU1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMRMSU2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;

  ///
  /// ENDING of the PARAMETERS
  ///

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

  double pU1Initial=pU1Diff;
  double pU2Initial=pU2Diff;

  double p1Charge = pU1Diff/fabs(pU1Diff);
  double p2Charge = pU2Diff/fabs(pU2Diff);

  double pU1ValD = 0 ;
  double pU2ValD = 0;
  double pUValM = 0;
  double pUValD = 0 ;

  // go to the pull space (we are on MC)
  // ????? what to do for the scale ??????

  if(doOnlyU1 && !doOnlyU2) {

    pU1Diff = pU1Diff/pMRMSU1;
    pU1ValD = triGausInvGraph(pU1Diff,iGenPt,ipdfMCU1,ipdfDATAU1,iwMCU1,iwDATAU1);
    pU1ValD = pU1ValD*pDRMSU1;

    pU2ValD = fabs(pU2Diff);

    pDefU1 *= (pDU1/pMU1);
  }

  if(!doOnlyU1 && doOnlyU2) {

    pU1ValD = pU1Diff;

    //    cout << "-------- RMSmc=" << pMRMSU2 << " RMSdata=" << pDRMSU2  << " ------------------------------ " << endl;
    //    cout << "pU2Diff(GeV)=" << pU2Diff;
    pU2Diff = pU2Diff/pMRMSU2; 
    //    cout << "   pU2Diff (pull) " << pU2Diff << endl;
    pU2ValD = triGausInvGraph(pU2Diff,iGenPt,ipdfMCU2,ipdfDATAU2,iwMCU2,iwDATAU2);
    //    cout << "   pU2ValD(pull) " << pU2ValD; 
    pU2ValD = pU2ValD*pDRMSU2; 

  }

  if(!doOnlyU1 && !doOnlyU2) {

    pU1Diff = pU1Diff/pMRMSU1;
    pU2Diff = pU2Diff/pMRMSU2;
    pU1ValD = triGausInvGraph(pU1Diff,iGenPt,ipdfMCU1,ipdfDATAU1,iwMCU1,iwDATAU1);
    pU2ValD = triGausInvGraph(pU2Diff,iGenPt,ipdfMCU2,ipdfDATAU2,iwMCU2,iwDATAU2);
    histoDeltaU1pull.Fill(pU1ValD-pU1Diff);
    histoDeltaU2pull.Fill(pU2ValD-pU2Diff);
    histoDeltaU1pullvsUreduced.Fill(pU1ValD-pU1Diff,pU1Diff);
    histoDeltaU2pullvsUreduced.Fill(pU2ValD-pU2Diff,pU2Diff);

    if(iGenPt>=5 && iGenPt<6) histoDeltaU1pull56.Fill(pU1ValD-pU1Diff);
    if(iGenPt>=5 && iGenPt<6) histoDeltaU2pull56.Fill(pU2ValD-pU2Diff);


    pU1ValD = pU1ValD*pDRMSU1;
    pU2ValD = pU2ValD*pDRMSU2;

    pDefU1 *= (pDU1/pMU1);
  }

  //  pU1ValD*=p1Charge; /// removed the ABS value on the argument of triGausInvGraph
  //  pU2ValD*=p2Charge;

  histoDeltaU1.Fill(pU1ValD-pU1Initial);
  histoDeltaU2.Fill(pU2ValD-pU2Initial);

  histoDeltaU1pullvsU.Fill(pU1ValD-pU1Initial,pU1Initial);
  histoDeltaU2pullvsU.Fill(pU2ValD-pU2Initial,pU2Initial);

  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);

  return;

}



//$$$$$$$$$$$$$$$$$$$$$$$$
//$$   end code from https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/master/src/PdfDiagonalizer.cc
//$$$$$$$$$$$$$$$$$$$$$$$$

void fitGraph(TTree *iTree,TTree *iTree1, TCanvas *iC,
//	      float &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1* iMeanFit=0,
	      double &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1 *iFitS3=0, 
	      TF1 *iFrac=0, TF1 *iFrac2=0, TF1* iMean1Fit=0, TF1* iMean2Fit=0, TF1* iMeanFit=0,
	      bool iPol1 = false, 
	      bool iRMS  = false, int iType = 0, std::string rootFileNameFrame="") { 

  /////
  /////
  /// temporary fix
  if(lPar!=fU1) fZPtMax=50;
  if(lPar==fU1) fZPtMax=20;


  //RooFit Build a double Gaussian
  TRandom lRand(0xDEADBEEF);
  //  RooRealVar    lRWeight("weight","weight",0,10);
  RooRealVar    lRWeight("weight","weight",0.5); // fixed weight for now
  
  range_min=fZPtMin;
  range_max=fZPtMax;

  minRangeSigma = rangeMinXVar;
  maxRangeSigma = rangeMaxXVar;

  if(doAbsolute) minRangeSigma = -50.;
  if(doAbsolute) maxRangeSigma = 50.;


  lRPt.setVal(0);     lRPt.setRange(range_min,range_max);
  lRXVar.setVal(0);   lRXVar.setRange(minRangeSigma,maxRangeSigma);

  //  RooRealVar lRPt("pt","Z_{p_{T}}",0, range_min, range_max);
  //  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,-10.,10.);

  if(iRMS) { 
    
    //  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,minRangeSigma,maxRangeSigma);
    //    if(lPar!=fU1) lRXVar.SetTitle("(U_{#perp}(Z_{p_{T}})-x_{i})/#sigma_{U#perp} (Z_{p_{T}})");
    //    if(lPar==fU1) lRXVar.SetTitle("(U_{#parallel}(Z_{p_{T}})-x_{i})/#sigma_{U#parallel} (Z_{p_{T}})");
    //    if(lPar!=fU1) lRXVar.SetTitle("(U^{#perp}_{i}-#mu^{i}_{#perp}(Z_{p_{T}}))/#sigma^{i}_{U#perp}(Z_{p_{T}})");
    //    if(lPar==fU1) lRXVar.SetTitle("(U^{#parallel}_{i}-#mu^{i}_{#parallel}(Z_{p_{T}}))/#sigma^{i}_{U#parallel}(Z_{p_{T}})");
    if(lPar!=fU1) lRXVar.SetTitle("(U^{#perp}_{i}-#mu^{i}(Z_{p_{T}}))/#sigma^{i}(Z_{p_{T}})");
    if(lPar==fU1) lRXVar.SetTitle("(U^{#parallel}_{i}-#mu^{i}(Z_{p_{T}}))/#sigma^{i}(Z_{p_{T}})");
    //  lRXVar.SetTitle("(U_{2}(Z_{p_{T}})-x_{i})/#sigma_{U2} (Z_{p_{T}})");

    constructPDF(lPar);
    //    constructPDF2d(lPar);

    /*

    RooArgSet* comps = lGAdd->getComponents() ;
    comps->Print();

    RooArgSet* comps1d = lRGAdd->getComponents() ;
    comps1d->Print();
    */

    /*
    // Generate a data sample of 1000 events in x from model
    RooDataSet *data = lRGAdd->generate(lRXVar,1000) ;
    cout << "generated dataset" << endl;
    
    RooPlot* xframe  = lRXVar.frame(Title("Test 1D")) ;
    data->plotOn(xframe) ;
    lRGAdd->plotOn(xframe) ;
    lRGaus1->plotOn(xframe,LineColor(kRed)) ;
    lRGaus2->plotOn(xframe,LineColor(kMagenta)) ;
    lRGaus3->plotOn(xframe,LineColor(kGreen)) ;
    
    //    TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
    //    c->cd();
    //    xframe->Draw() ;
    //    cin.get(); 
    */
  }  

  /*
    if(doPhiStar) {
    binSize=1./100.; 
    range_min=0.+binSize;
    range_max=0.4-binSize;
  }

  RooRealVar lRPt  ("pt","Z_{p_{T}}",range_min,range_max);
  if(doPhiStar) lRPt.SetTitle("#phi^{*}");

  // this is the range of th X axis
  //  double minRangeSigma=-10.; double maxRangeSigma=10.;
  double minRangeSigma=-15.; double maxRangeSigma=15.;
  if(doAbsolute) { minRangeSigma=-50.; maxRangeSigma=50.;}

  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,minRangeSigma,maxRangeSigma);
  if(lPar!=fU1) lRXVar.SetTitle("(U_{2}(Z_{p_{T}})-x_{i})/#sigma_{U2} (Z_{p_{T}})");
  if(doAbsolute && lPar==fU1) lRXVar.SetTitle("U_{1}(Z_{p_{T}})-x_{i}");
  if(doAbsolute && lPar!=fU1) lRXVar.SetTitle("U_{2}(Z_{p_{T}})-x_{i}");

  // this is the mean
  double minMean=-1.; double maxMean=1.;
  if(doAbsolute) { minMean=-1.; maxMean=1.; }

  // this is the p0 value of the sigma
  // should not be negative and the
  double startSigma1=0.6; 
  double minSigma1=0.2; double maxSigma1=1.2;
  if(fId==201) maxSigma1=1.5;
  double startSigma2=1.5; 
  double minSigma2=1.2 ; double maxSigma2=8.;
  if(doAbsolute) { startSigma1=3*0.5;  startSigma2=3*1.2; }
  if(doAbsolute) { minSigma1=0.;  maxSigma1=10.;}
  if(doAbsolute) { minSigma2=3.;  maxSigma2=40.;}

  double startSigma3 = 0.; 
  double minSigma3 = 0.; 
  double maxSigma3 = 0.; 

  if(do3G) {

    startSigma1=0.6; 
    minSigma1=0.2; 
    maxSigma1=1.2;

    startSigma2=1.; 
    minSigma2=0.7; 
    maxSigma2=3.;

    startSigma3 = startSigma2*1.6; //2.
    minSigma3 = 1.4; 
    maxSigma3 = 8; //12

  }

  if(doDegenerateSigma) startSigma2=0.0;

  double SF=1;  
  if(doAbsolute) SF=20;

  double B1min = 0.1*SF;
  double B2min = 0.5*SF;
  double B3min = 0.5*SF;

  if(do3G) {
    B1min = 0.;
    B2min = 0.;
    B3min = 0.;
  }

  double B1init= 0.0;
  double C1init= 0.0; 
  double B2init= 0.0;
  double C2init= 0.0; 
  double B3init= 0.0;
  double C3init= 0.0; 

/// setting for POWHEG should be ok also for madgraph
//  if(!fData && lPar==fU1) B1init = 0.02; // powheg U1
//  if(!fData && lPar==fU1) C1init = -0.0003; // powheg U1
//  if(!fData && lPar==fU1) B2init = 0.005; // powheg U1
//  if(!fData && lPar==fU1) C2init = 0.0001; // powheg U1

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

  ////// those are the 2D histograms     
  //// 
  //// 
  // ===> SIGMA
  RooRealVar lA1Sig("a1sig","a1sig", startSigma1, minSigma1 , maxSigma1 );
  RooRealVar lB1Sig("b1sig","b1sig", B1init , B1min , 0.1*SF);   // lB1Sig.setConstant(kTRUE); 
  RooRealVar lC1Sig("c1sig","c1sig", C1init , -0.01*SF, 0.01*SF);   // lC1Sig.setConstant(kTRUE);
  //  if(!doMad && !fData && lPar==fU1) lC1Sig.setConstant(kTRUE);  // POWHEG guessed from Z 
  RooRealVar lD1Sig("d1sig","d1sig",0. , -0.1*SF, 0.1*SF);  lD1Sig.setConstant(kTRUE);

  RooRealVar lA2Sig("a2sig","a2sig", startSigma2, minSigma2 , maxSigma2 );               if(doDegenerateSigma) lA2Sig.setConstant(kTRUE); 
  RooRealVar lB2Sig("b2sig","b2sig", B2init , B2min , 0.5*SF);                          if(doDegenerateSigma) lB2Sig.setConstant(kTRUE);
  RooRealVar lC2Sig("c2sig","c2sig", C2init ,-0.05*SF, 0.05*SF);                        if(doDegenerateSigma) lC2Sig.setConstant(kTRUE);
  //  if(!doMad && !fData && lPar==fU1) lC2Sig.setConstant(kTRUE);  // POWHEG guessed from Z 
  RooRealVar lD2Sig("d2sig","d2sig",0.0 ,-0.1*SF, 0.1*SF);  lD2Sig.setConstant(kTRUE);  if(doDegenerateSigma) lD2Sig.setConstant(kTRUE);

  RooRealVar lA3Sig("a3sig","a3sig", startSigma3, minSigma3 , maxSigma3 );   if(!do3G) lA3Sig.setConstant(kTRUE);
  RooRealVar lB3Sig("b3sig","b3sig", B3init , B3min , 0.5*SF);              if(!do3G) lB3Sig.setConstant(kTRUE);
  RooRealVar lC3Sig("c3sig","c3sig", C3init ,-0.05*SF, 0.05*SF);            if(!do3G) lC3Sig.setConstant(kTRUE);
  //  if(!doMad && !fData && lPar==fU1) lC3Sig.setConstant(kTRUE);  
  RooRealVar lD3Sig("d3sig","d3sig",0.0 ,-0.1*SF, 0.1*SF);  lD3Sig.setConstant(kTRUE); if(!do3G) lD3Sig.setConstant(kTRUE);

  //  RooPolynomial l1Sigma("sigma1","sigma1",lRPt,RooArgList(lA1Sig,lB1Sig,lC1Sig,lD1Sig),0) ;
  //  RooPolynomial l2Sigma("sigma2","sigma2",lRPt,RooArgList(lA2Sig,lB2Sig,lC2Sig,lD2Sig),0) ;
  //  RooPolynomial l3Sigma("sigma3","sigma3",lRPt,RooArgList(lA3Sig,lB3Sig,lC3Sig,lD3Sig),0) ;
  RooFormulaVar l1Sigma("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
  RooFormulaVar l2Sigma("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
  RooFormulaVar l3Sigma("sigma3","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA3Sig,lB3Sig,lC3Sig,lRPt,lD3Sig));

  //  RooGaussian lGaus1("gaus1","gaus1",lRXVar,lMean,l1Sigma);
  //  RooGaussian lGaus2("gaus2","gaus2",lRXVar,lMean,l2Sigma);
  //  RooGaussian lGaus3("gaus3","gaus3",lRXVar,lMean,l3Sigma);
  RooGaussModel lGaus1("gaus1","gaus1",lRXVar,lMean,l1Sigma);
  RooGaussModel lGaus2("gaus2","gaus2",lRXVar,lMean,l2Sigma);
  RooGaussModel lGaus3("gaus3","gaus3",lRXVar,lMean,l3Sigma);

  // ===> FRACTION
  RooRealVar lAFrac("AFrac","AFrac",0.4,0.1,0.6); //lAFrac.setConstant(kTRUE);
  RooRealVar lBFrac("BFrac","BFrac",0,-0.1*SF,0.1*SF);  lBFrac.setConstant(kTRUE);
  RooRealVar lCFrac("CFrac","CFrac",0,-0.1*SF,0.1*SF);  lCFrac.setConstant(kTRUE); 
  RooRealVar lDFrac("DFrac","DFrac",0,-0.1*SF,0.1*SF);  lDFrac.setConstant(kTRUE); 

  lB1Sig.setConstant(kTRUE);
  lB2Sig.setConstant(kTRUE);
  lB3Sig.setConstant(kTRUE);
  lC1Sig.setConstant(kTRUE);
  lC2Sig.setConstant(kTRUE);
  lC3Sig.setConstant(kTRUE);

  ///<+++++ 2G 
  //  RooFormulaVar lFrac  ("frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(l1Sigma,l2Sigma));
  //  RooFormulaVar lFrac  ("frac"  ,"(@1*@1-1.)/(@1*@1-@0*@0)",RooArgSet(l1Sigma,l2Sigma));   // for squares   
  //  RooAddPdf     lGAdd("Add","Add",lGaus1,lGaus2,lFrac); ///<+++++ 2G
    
  ///<+++++ 3G 
  RooFormulaVar l1Frac ("frac1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lAFrac,lBFrac,lCFrac,lRPt,lDFrac));
  //  RooPolynomial l1Frac ("frac1","frac1",lRPt,RooArgList(lAFrac,lBFrac,lCFrac,lDFrac),0) ;
  RooFormulaVar lFrac("frac"  ,"-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3))",RooArgSet(l1Frac,l1Sigma,l2Sigma,l3Sigma)); // free parameter for the doAbsolute=true
  RooAddPdf lGAdd("Add","Add",RooArgList(lGaus1,lGaus2,lGaus3),RooArgList(l1Frac,lFrac),kTRUE);
  //  RooHistPdf Zbkg("Zbkg","Zbkg",x,tmp,2);// this for the top background

  ////// those are the 1D histograms     
  //// 
  //// 
  // ===> MEAN
  RooRealVar    lR1Mean ("Rmean",  "Rmean",0,minMean,maxMean); // lR1Mean.setConstant(kTRUE); 
  // ===> SIGMA
  RooRealVar    lR1Sigma("Rsigma1","RSigma1",startSigma1, minSigma1, maxSigma1 );
  RooRealVar    lR2Sigma("Rsigma2","RSigma2",startSigma2, minSigma2, maxSigma2 );
  RooRealVar    lR3Sigma("Rsigma3","RSigma3",startSigma3, minSigma3, maxSigma3 );  if(!do3G) lR3Sigma.setConstant(kTRUE);
  RooGaussian   lRGaus1("Rgaus1","Rgaus1",lRXVar,lR1Mean,lR1Sigma);
  RooGaussian   lRGaus2("Rgaus2","Rgaus2",lRXVar,lR1Mean,lR2Sigma);
  RooGaussian   lRGaus3("Rgaus3","Rgaus3",lRXVar,lR1Mean,lR3Sigma);
  // ===> FRACTION
  double init=0;
  double max=0;
  if(do3G) init=0.6;
  if(do3G) max=1;

  ///<+++++ 3G 
  RooRealVar lR1Frac ("R1frac"  ,"R1frac"  , init ,0, max); 
  RooFormulaVar    lRFrac ("Rfrac"  ,"-(-@0*@1+@0*@3-@3+1)/((@0-1)*(@2-@3))",RooArgSet(lR1Frac,lR1Sigma,lR2Sigma,lR3Sigma)); 
  RooAddPdf     lRGAdd("RAdd","RAdd",RooArgList(lRGaus1,lRGaus2,lRGaus3),RooArgList(lR1Frac,lRFrac),kTRUE);
  
  ///<+++++ 2G  
  //  RooFormulaVar lR1Frac  ("R1frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(lR1Sigma,lR2Sigma)); //this only for the pull ; for abolute keep independent
  //  RooRealVar lRFrac ("Rfrac"  ,"Rfrac"  , init ,0, max);  
  //  RooFormulaVar lR1Frac  ("Rfrac"  ,"(@1*@1-1)/(@1*@1-@0*@0)",RooArgSet(lR1Sigma,lR2Sigma)); //this only for the pull ; for abolute keep independent ; for squares
  //  RooAddPdf     lRGAdd("RAdd","RAdd",lRGaus1,lRGaus2,lR1Frac);  ///<+++++ 2G

  cout << "====================" << endl;
  cout << "== Printing the initial values of the constraint " << endl;
  cout << "== l1Frac=" << l1Frac.getVal() << " lFrac=" << lFrac.getVal() << endl;
  cout << "====================" << endl;

*/

  // cout << "====================" << endl;
  // cout << "== Setting the Minimizer " << endl;
  // cout << "====================" << endl;

  double tolerance = ROOT::Math::MinimizerOptions::DefaultTolerance();
  string algo = ROOT::Math::MinimizerOptions::DefaultMinimizerAlgo();
  string type = ROOT::Math::MinimizerOptions::DefaultMinimizerType();
  int strategy= ROOT::Math::MinimizerOptions::DefaultStrategy();
  
  int precision= ROOT::Math::MinimizerOptions::DefaultPrecision();
  int MaxFunctionCalls= ROOT::Math::MinimizerOptions::DefaultMaxFunctionCalls();
  int MaxIterations= ROOT::Math::MinimizerOptions::DefaultMaxIterations();

  cout << "DEFAULTS: algo " << algo.c_str() << " type " << type.c_str() << " tolerance " << tolerance << " strategy " << strategy << " precision " << precision << " MaxIterations " << MaxIterations << " MaxFunctionCalls " << MaxFunctionCalls << endl;
    
  string minimizerType = "Minuit2";
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer(minimizerType.c_str());
  string type2 = ROOT::Math::MinimizerOptions::DefaultMinimizerType();

  ROOT::Math::MinimizerOptions::SetDefaultStrategy(2);
  ROOT::Math::MinimizerOptions::SetDefaultTolerance(0.001);

  cout << "CHANGED TO: algo " << algo.c_str() << " type " << type2.c_str() << " tolerance " << ROOT::Math::MinimizerOptions::DefaultTolerance() << " strategy " << ROOT::Math::MinimizerOptions::DefaultStrategy() << endl;

  ////
  ////
  // cout << "====================" << endl;
  // cout << "== Setting the legend that are needed for later " << endl;
  // cout << "====================" << endl;
  
  // MARIA something to add to the plots
  TString leg = "#sigma_{mean} = ";   
  //  leg += lY0[i0];
  
  TLatex latexLabel;    
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  //    latexLabel.DrawLatex(0.2, 0.7, leg);
  
  TString leg1 = "";  
  if(fId == 1) leg1 += " |Y(V)| < 1";
  if(fId == 2) leg1 += " |Y(V)| >= 1";
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
  if(doMad && (!fData)) {
    legDATA = "Z madgraph";
    if(doPosW) legDATA = " Wpos madgraph";
    if(doNegW) legDATA = " Wneg madgraph";
  }
  if(!doMad && (!fData)) {
    legDATA = "Z powheg";
    if(doPosW) legDATA = " Wpos powheg";
    if(doNegW) legDATA = " Wneg powheg";
  }


  TString legU1U2 = "U#parallel";
  if(lPar!=fU1) legU1U2 = "U#perp";

  TString labeling="";
  
  if(lPar==fU1) labeling += "_U1";
  if(lPar!=fU1) labeling += "_U2";
  
  if(fData) labeling += "_data";
  if(!fData) labeling += "_MC";
  
  if(!doMad && (!fData)) labeling += "_powheg";
  if(doMad && (!fData)) labeling += "_madgraph";
  
  if(doPosW) labeling += "_Wpos";
  if(doNegW) labeling += "_Wneg";
  
  if(doBKG) labeling += "_bkg";

  if(doAbsolute) labeling += "_absolute";
  if(!doAbsolute) labeling += "_pull";

  if(doRunA) labeling += "_runA";
  if(doRunB) labeling += "_runB";
  
  if(doIterativeMet) labeling += "_iterative";
  if(doClosure) labeling += "_closure";

  labeling += "_";
  labeling += fNJetSelect;
  labeling += "_";
  labeling += fId;
  labeling += "_vtx";
  labeling += VTXbin;
  labeling += "_PDF";
  labeling += pType;

  if(do3G) labeling += "_3G";

  ////
  // cout << "====================" << endl;
  // cout << "== Setting the fit for the mean scale and RMS " << endl;
  // cout << "====================" << endl;

  iC->cd();

  if(iRMS){   
    for(unsigned int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
      // pVal = (lPar - iMeanFit->Eval(fZPt));
      double temp = vlYVals_all[lPar!=fU1][iRMS][iev];
      if(iMeanFit != 0) temp = vlYVals_all[lPar!=fU1][iRMS][iev] - iMeanFit->Eval(vlXVals_all[lPar!=fU1][iRMS][iev]);
      vlYVals_all[lPar!=fU1][iRMS][iev] = abs(temp); // for the fit of the main RMS
      //      vlYVals_all[lPar!=fU1][iRMS][iev] = abs(temp*temp); // for the fit of the main RMS;     // for squares
      vlYTVals_all[lPar!=fU1][iRMS][iev] = temp; // used for the fit to the pull
    }
  }

  //TGraphErrors *pGraphA = new TGraphErrors(lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]),(Float_t*)&(lXEVals[0]),(Float_t*)&(lYEVals[0]));
  // TGraph *pGraphA       = new TGraph      (lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]));

  if(lPar==fU1 && !iRMS) {

    for(unsigned int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
      if(fId==1) histoUnbinnedY1ZPt.Fill(vlXVals_all[lPar!=fU1][iRMS][iev]);
      if(fId==2) histoUnbinnedY2ZPt.Fill(vlXVals_all[lPar!=fU1][iRMS][iev]);
    }
  }

  TGraph *pGraphA       = new TGraph(lXVals_all.at(0).at(0).size(), 
				     vlXVals_all[0][0], 
				     vlYVals_all[lPar!=fU1][iRMS]
				     );
  
  
  std::string lType     = "pol3"; if(iPol1) lType = "pol2"; //pol1 -->higgs 
  if(iType == 1)  lType = "pol3"; 

  lType     = "pol2";
  if(usePol3) lType     = "pol3";
  if(usePol4) lType     = "pol4";
  if(usePol3 && useSubRanges && lPar==fU1 && !iRMS) lType = "pol3(0)+pol3(4)"; 

  if(useErfPol2ScaleU1 && lPar==fU1 && !iRMS) {
    //    lType     = "(TMath::Erf(x*[0])-TMath::Erf(x*[1]))*pol2(2)"; // MC 2 Erf
    //    lType     = "TMath::Erf(x*[0])*pol2(1)";
    if(fData || doIterativeMet || !fData)  lType     = "TMath::Erf(x*[0])*pol2(1)"; // data 1 Erf
  }

  //http://root.cern.ch/root/html/TGraph.html
  //"EX0" When fitting a TGraphErrors or TGraphAsymErrors do not consider errors in the coordinate
  //"R"   Use the Range specified in the function range
  //"S"  The result of the fit is returned in the TFitResultPtr
  //"E"  Perform better Errors estimation using Minos technique
  
  //// THIS IS THE unbinned Fit
  TF1 *lFit = new TF1("test",lType.c_str()); //fZPtMax = 200;                                                                                                                               

  cout << lType.c_str() << endl;

  // cout << "====================" << endl;
  // cout << "== Setting the initial values for U1 scale " << endl;
  // cout << "====================" << endl;

  // pol3 -- set to a constant the first param
  if(!iRMS && lPar==fU1 && !useErfPol2ScaleU1 && !useSubRanges) lFit->FixParameter(0,0);
  //  if(!useErfPol2ScaleU1 && !useSubRanges) lFit->FixParameter(0,0);

  if(!iRMS && lPar==fU1 && useErfPol2ScaleU1) {

    /*
    // erf MC 2Erf
    if(!fData && !doIterativeMet){

      lFit->SetParameter(0,4.65859e-02);
      lFit->SetParameter(1,-2.26153e-02);
      //lFit->FixParameter(1,1);
      lFit->SetParameter(2,-3.33465e+00);
      //lFit->SetParLimits(2, 0, 3.);
      lFit->SetParameter(3, -1.48770e-01);
      //lFit->SetParLimits(3,-1,1.);
      lFit->SetParameter(4, -1.04105e-03);
    }
    */

    // erf DATA
    if( fData || doIterativeMet || !fData) {
      lFit->SetParameter(0,-0.07);
      lFit->SetParLimits(0,-2, 2);

      lFit->SetParameter(1, 3.0);
      lFit->SetParLimits(1,0,10);

      lFit->SetParameter(2, 0.15);
      lFit->SetParLimits(2,-1, 1);

      lFit->SetParameter(3, 0.005);
      lFit->SetParLimits(3,-1,1);
      //f1->SetParameter(4,  2.15617e-02);
      //f1->SetParLimits(4, -1, 1);
      //f1->SetParameter(4,    -1.91054e-04); 
      //f1->SetParLimits(4, -1, 1); 
      //f1->SetParameter(5,   -6.87765e-05); 
    }
  }

  if(useSubRanges && !iRMS && lPar==fU1) {

    Double_t par[8];
    TF1* first = new TF1("m1","pol3",0,6);
    TF1* second = new TF1("m2","pol3",6,fZPtMax);
    
    //    first->SetParameter(0,0);
    //    first->SetParLimits(0,-0.1, 0.1);
    //    first->FixParameter(0,0);
    pGraphA->Fit(first, "R");
    pGraphA->Fit(second, "R+");
    
    first->GetParameters(&par[0]);
    second->GetParameters(&par[4]);
    lFit->SetParameters(par);
    
  }

  //  if(!iRMS && lPar==fU1) lFit->SetParLimits(0, 0.0, 1.0); // first parameter between 0 and 1.0
  //  if(!iRMS && lPar==fU1) lFit->SetParLimits(0, -0.5, 1.0); // first parameter between 0 and 1.0

  //  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SR","EXO",fZPtMin,fZPtMax); //"EXO"
  //  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SRE","",fZPtMin,fZPtMax); 
  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SRE","",range_min,range_max);

  //  cout<<"lFit1   "<< lFit->GetExpFormula()<<endl;

  // only once
  if(useErfPol2ScaleU1 && !iRMS && lPar==fU1) {
    computeFitErrorsERF(iFit,lFitPtr,lFit,iRMS);
  } else {
    computeFitErrors(iFit,lFitPtr,lFit,iRMS);
  }

  /*
  // twice only for the U1scale                                                                                                                                                
  if(!iRMS && lPar==fU1) {
    TF1 *lFit2 = new TF1("test2",lType.c_str());
    lFit2->SetParameter(0,lFit->GetParameter(0));
    lFit2->SetParameter(1,lFit->GetParameter(1));
    
    if(usePol3 || usePol4 || useErfPol2ScaleU1) {
      lFit2->SetParameter(2,lFit->GetParameter(2));
      lFit2->SetParameter(3,lFit->GetParameter(3)); 

      if(usePol4 ) {
	lFit2->SetParameter(4,lFit->GetParameter(4));
      }
    }

//    if(useErfPol2ScaleU1 && !fData && !doIterativeMet ) {
//      lFit2->SetParameter(4,lFit->GetParameter(4));
//      lFit2->SetParameter(5,lFit->GetParameter(5));
//    } 
   
    TFitResultPtr  lFitPtr2 = pGraphA->Fit(lFit2,"SRE","",range_min,range_max);

    if(useErfPol2ScaleU1) computeFitErrorsERF(iFit,lFitPtr2,lFit2,iRMS);
    if(!useErfPol2ScaleU1) computeFitErrors(iFit,lFitPtr2,lFit2,iRMS); 

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

  TString fileName2D="fileGraph_Unbinned_meanRMS_";
  if(!fData && (!doPosW && doNegW)) fileName2D += "Wneg";
  if(!fData && (doPosW && !doNegW)) fileName2D += "Wpos";
  if(!fData && (!doPosW && !doNegW)) fileName2D += "Z";
  if(fData) fileName2D += "DATA";
  if(!fData) fileName2D += "MC"; 
  if(doMad && !fData) fileName2D += "_MADGRAPH";
  if(!doMad && !fData) fileName2D += "_POWHEG";
  //  if(doClosure) fileName2D += "_closure";
  //  if(doClosure) fileName2D += "_closureVSDATA";
  if(doClosure) fileName2D += "_closureVSMAD";
  //  fileName2D += "_upToZpt50";
  if(doHalfStat) fileName2D += "_halfSize";
  if(doRunA) fileName2D += "_runA";
  if(doRunB) fileName2D += "_runB";
  if(do8TeV)  fileName2D += "_8TeV";
  if(doRecoParam) fileName2D += "_doRecoParam";
  if(doLepProjAbsolute) fileName2D += "_doLepProjAbsolute";
  fileName2D += "_AUG16";
  fileName2D += ".root";
  
  if(doPrint) {

    pGraphA->Draw("ap");
    drawErrorBands(iFit,fZPtMax);
    
    Double_t chi2   = lFitPtr->Chi2(); // to retrieve the fit chi2 
                                                                                                                      
    int ndof = lFitPtr->Ndf();
    if(lPar==fU1 && !iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U1 mean " << endl;
    if(lPar==fU1 && iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U1 RMS " << endl;
    if(!lPar==fU1 && !iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U2 mean " << endl;
    if(!lPar==fU1 && iRMS) cout << "MARIA CHI2 "<< chi2 << " for "<< ndof << "DOF <== U2 RMS " << endl;

    if(lPar==fU1) pGraphA->GetYaxis()->SetTitle("U#parallel"); 
    if(lPar!=fU1) pGraphA->GetYaxis()->SetTitle("U#perp"); 
    pGraphA->GetXaxis()->SetTitle("Z p_{T}"); 

    latexLabel.DrawLatex(0.20, 0.8, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.7, leg3);
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

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
    if(lPar==fU1 && !iRMS && fId==1)     histoUnbinnedY1ZPt.Write();
    if(lPar==fU1 && !iRMS && fId==2)     histoUnbinnedY2ZPt.Write();

    if(lPar==fU1 && !iRMS && fId==1)     histoSumEtY1.Write();
    if(lPar==fU1 && !iRMS && fId==2)     histoSumEtY2.Write();

    f1.Write();
    f1.Close();

  }

  ////
  //  cout << "====================" << endl;
  //  cout << "== Setting the PULLS " << endl;
  //  cout << "====================" << endl;


  if(!doType2) return;
  if(!iRMS) return;

  if(do3Sigma) {

    // for 3Sigma study fill the vector for the pull
    for(unsigned int iev=0; iev<lXVals_all.at(lPar!=fU1).at(iRMS).size(); iev++){
      double temp = vlYVals_all[lPar!=fU1][iRMS][iev];
      if(iMeanFit != 0) temp = vlYVals_all[lPar!=fU1][iRMS][iev] - iMeanFit->Eval(vlXVals_all[lPar!=fU1][iRMS][iev]);


      double MeanSigma = lFit->GetParameter(0) + vlXVals_all[lPar!=fU1][iRMS][iev] * lFit->GetParameter(1) + vlXVals_all[lPar!=fU1][iRMS][iev] * vlXVals_all[lPar!=fU1][iRMS][iev] * lFit->GetParameter(2); 
      //      cout << " MeanSigma " << MeanSigma << " p0=" << lFit->GetParameter(0) << " p1=" << lFit->GetParameter(0) << " p2= " << lFit->GetParameter(2) << endl;
      //      cout << "fZpt " << vlXVals_all[lPar!=fU1][iRMS][iev] << endl;
      if(abs(temp)>3*MeanSigma) lXVals_3S[lPar!=fU1][iRMS].push_back(vlXVals_all[lPar!=fU1][iRMS][iev]);
      if(abs(temp)>3*MeanSigma) lYVals_3S[lPar!=fU1][iRMS].push_back(abs(temp)); // this check the residuals 

    }

    //    cout << "size X " << lXVals_3S.at(lPar!=fU1).at(iRMS).size() << " size Y " << lYVals_3S.at(lPar!=fU1).at(iRMS).size() << endl;

    vlXVals_3S[lPar!=fU1][iRMS]=new double[lXVals_3S.at(lPar!=fU1).at(iRMS).size()];
    vlYVals_3S[lPar!=fU1][iRMS]=new double[lYVals_3S.at(lPar!=fU1).at(iRMS).size()];

    for(unsigned int iev=0; iev<lXVals_3S.at(lPar!=fU1).at(iRMS).size(); iev++){

      vlXVals_3S[lPar!=fU1][iRMS][iev]=lXVals_3S[lPar!=fU1][iRMS][iev];
      vlYVals_3S[lPar!=fU1][iRMS][iev]=lYVals_3S[lPar!=fU1][iRMS][iev]; // this check the residuals

    }


    if(doPrint) {

      TGraph *pGraph3Sigma       = new TGraph(lXVals_3S[lPar!=fU1][iRMS].size(), 
					      vlXVals_3S[lPar!=fU1][iRMS], 
					      vlYVals_3S[lPar!=fU1][iRMS]
					      );
      //// THIS IS THE unbinned Fit
      TF1 *lFit3S = new TF1("test",lType.c_str()); //fZPtMax = 200;                                                                                                                               
      TFitResultPtr  lFitPtr3S = pGraph3Sigma->Fit(lFit3S,"SRE","",range_min,range_max);
      
      //      double Mean3Sigma = lFit3S->GetParameter(0) + fZPt * lFit3S->GetParameter(1) + fZPt*fZPt * lFit3S->GetParameter(2); 
      //      cout << " Mean3Sigma " << Mean3Sigma << " p0=" << lFit3S->GetParameter(0) << " p1=" << lFit3S->GetParameter(0) << " p2= " << lFit3S->GetParameter(2) << endl;
      
      if(lPar==fU1) pGraph3Sigma->GetYaxis()->SetTitle("U#parallel > 3 SigmaMeanU1"); 
      if(lPar!=fU1) pGraph3Sigma->GetYaxis()->SetTitle("U#perp > 3 SigmaMeanU2"); 
      pGraph3Sigma->GetXaxis()->SetTitle("Z p_{T}"); 

      pGraph3Sigma->SetMarkerColor(kCyan);

      TFile f1(fileName2D,"UPDATE");
      
      if(lPar==fU1) pGraph3Sigma->SetName("graph_3S_U1");
      if(lPar!=fU1) pGraph3Sigma->SetName("graph_3S_U2");
      pGraph3Sigma->SetTitle("");
      pGraph3Sigma->Draw("ap");
      //      drawErrorBands(iFit,fZPtMax);
      
      pGraph3Sigma->Write(); 
      f1.Write();
      f1.Close();
   	
    }    
  }
  
  
  cout << "====================" << endl;
  cout << "== Building Datasets" << endl; 
  cout << "====================" << endl;

  //Build the double gaussian dataset
  std::vector<RooDataSet> lResidVals; 
  //  std::vector<RooDataHist> lResidVals; 

  std::vector<RooKeysPdf> lResidValsKeys;

  //http://roofit.sourceforge.net/docs/tutorial/intro/roofit_tutorial_intro.pdf
  // RooDataSet - unbinned (weighted & unweighted) but very slow
  // RooDataHist - binned
  //  std::vector<RooDataSet> lResidVals2D; 

  std::vector<Roo2DKeysPdf> lResidVals2DKeys;
  std::stringstream pkey; pkey << "Crapsky" << 0;
  if(lPar==fU1) pkey << "_U1_";
  if(lPar!=fU1) pkey << "_U2_";
  pkey << "2D";

  std::vector<RooDataHist> lResidVals2D;
  std::stringstream pSS; pSS << "Crapsky" << 0;
  if(lPar==fU1) pSS << "_U1_";
  if(lPar!=fU1) pSS << "_U2_";
  pSS << "2D"; RooDataHist lData2D(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar,lRPt));
  //  Roo2DKeysPdf pdf_keys2D(pkey.str().c_str(), pkey.str().c_str(), lRXVar, lRPt, lData2D);

  //  pSS << "2D"; RooDataSet *lData2D = new RooDataSet(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar,lRPt));
  //  int lNBins = 10;
  //  int lNBins = 25;
  ///////  int lNBins = 50;
  int lNBins = (range_max-range_min)/binSize;
  //  int lNBins = fZPtMax;
  for(int i0  = 0; i0 < fZPtMax; i0++) { 
    std::stringstream pSS1;
    pSS1 << "Crapsky";
    if(lPar==fU1) pSS1 << "_U1";
    if(lPar!=fU1) pSS1 << "_U2";
    if(fId==1) pSS1 << "_1_";
    if(fId==2) pSS1 << "_2_";
    pSS1 << i0;

    RooDataSet lData(pSS1.str().c_str(),pSS1.str().c_str(),RooArgSet(lRXVar)); 

    //    RooDataSet lData_1(pSS1.str().c_str(),pSS1.str().c_str(),RooArgSet(lRXVar)); 
    //    RooDataSet * lData_2 = (RooDataSet*) lData_1.reduce("XVar>-4.");
    //    RooDataSet * lData   = (RooDataSet*) lData_2->reduce("XVar<4."); 
    if(doWeight) {
      RooRealVar* w = (RooRealVar*) lData.addColumn(lRWeight);
      RooDataSet wData(lData.GetName(),lData.GetTitle(),&lData,*lData.get(),(char*)0,w->GetName());
      lResidVals.push_back(wData);
    } else {
      //      lResidVals.push_back(*lData); 
      lResidVals.push_back(lData); 
    }

  }

  cout << "lNBins " << lNBins << endl;
  cout << "Size binned Datasets: " << lResidVals.size() << endl;

  if(doWeight) {
    //    RooRealVar* w2D = (RooRealVar*) lData2D->addColumn(lRWeight); 
    //    RooDataSet wData2D(lData2D->GetName(),lData2D->GetTitle(),lData2D,*lData2D->get(),(char*)0,w2D->GetName());
    //    lResidVals2D.push_back(wData2D);
    lResidVals2D.push_back(lData2D);
  } else {
    //    lResidVals2D.push_back(*lData2D);
    lResidVals2D.push_back(lData2D);
    //    lResidVals2DKeys.push_back(pdf_keys2D);
  }

  // not sure those will be used (!)
  std::vector<float> lX3SVals; std::vector<float> lXE3SVals; std::vector<float> lY3SVals; std::vector<float> lYE3SVals; //Events with sigma > 3
  std::vector<float> lX4SVals; std::vector<float> lXE4SVals; std::vector<float> lY4SVals; std::vector<float> lYE4SVals; //Events with sigma > 3

  double lRescale  = sqrt((TMath::Pi())/2.);
  //    lRPt.setBins(500);
  //    lRPt.setBins(fZPtMax);
  //    lRPt.setBins(lNBins);
  lRPt.setBins(10000);
  //  lRPt.setBins(lNBins*50);

  for(unsigned int i0 = 0; i0 < lXVals_all.at(0).at(0).size(); i0++) { 

    // Zpt range
    //    if(vlXVals_all[lPar!=fU1][iRMS][i0]>range_min && vlXVals_all[lPar!=fU1][iRMS][i0]<range_max) {
    if(vlXVals_all[lPar!=fU1][iRMS][i0]<range_min) continue;
    if(vlXVals_all[lPar!=fU1][iRMS][i0]>=range_max) continue;
    
    // normalization too much 
    // MARIA changed the lFit to iFit  
    double lYTest = iFit->Eval(vlXVals_all[lPar!=fU1][iRMS][i0])*lRescale;
    //    double lYTest = sqrt(iFit->Eval(vlXVals_all[lPar!=fU1][iRMS][i0]));      // for squares

    // clean the under/overflow
    if(!doAbsolute && (vlYTVals_all[lPar!=fU1][iRMS][i0]/(lYTest)<rangeMinXVar || vlYTVals_all[lPar!=fU1][iRMS][i0]/(lYTest)>rangeMaxXVar)) continue;
    if(doAbsolute && (vlYTVals_all[lPar!=fU1][iRMS][i0]<-50. || vlYTVals_all[lPar!=fU1][iRMS][i0]>50.)) continue;

    // MARIA: here the switch for pull or GeV
    if(!doAbsolute) lRXVar.setVal(vlYTVals_all[lPar!=fU1][iRMS][i0]/(lYTest)); // residual  for the Pull
    if(doAbsolute) lRXVar.setVal(vlYTVals_all[lPar!=fU1][iRMS][i0]);  // residual  for the fit in GeV
    lRPt.setVal(vlXVals_all[lPar!=fU1][iRMS][i0]);     // Zpt

    //    lRWeight.setVal(1./vlYEVals_all[lPar!=fU1][iRMS][i0]/vlYEVals_all[lPar!=fU1][iRMS][i0]);
    /////    int pId = int(vlXVals_all[lPar!=fU1][iRMS][i0]/((range_max-range_min)/lNBins)); if(pId > lNBins-1) pId = lNBins-1;
    //    int pId = int(vlXVals_all[lPar!=fU1][iRMS][i0]/((range_max-range_min)/lNBins)); // if(pId > lNBins-1) pId = lNBins-1;
    int pId = int(vlXVals_all[lPar!=fU1][iRMS][i0]);

    lRWeight.setVal(0.5);

    // MARIA: USED for the 1D fit (binned/uncorrelated in Zpt and binned in residuals )  
    //    RooDataSet lData_1(RooArgSet(lRXVar)); 
    //    lResidVals[pId].add(RooArgSet(lRXVar)/*,lRWeight.getVal()*/); //Fill the Double Gaussian
    lResidVals[pId].add(RooArgSet(lRXVar),lRWeight.getVal()); //Fill the 1D Double Gaussian

    //    lResidVals[pId].reduce("XVar>-4.");
    //    lResidVals[pId].reduce("XVar<4."); 
    //    RooDataSet * lData_2 = (RooDataSet*) lData_1.reduce("XVar>-4.");
    //    RooDataSet * lData   = (RooDataSet*) lData_2->reduce("XVar<4."); 

    //    lResidVals[pId].add(*lData)/*,lRWeight.getVal()*/); //Fill the Double Gaussian
    // MARIA: USED for the 2D fit (binned/correlated in Zpt and unbinned in Residauls )   
    lResidVals2D[0].add(RooArgSet(lRXVar,lRPt)/*,lRWeight.getVal()*/); //Fill the Double Gaussian

  }

  cout << "Size binned Datasets: " << lResidVals.size() << " range_min=" << range_min << " range_max=" << range_max << endl;

  /// swith off Minos to speed up 
  //  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos());//,Minos()); //Double Gaussian fit
  //  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos()); //Double Gaussian fit
  // other options inside
  // http://root.cern.ch/root/html/RooAbsPdf.html#RooAbsPdf:fitTo
  ////  options Extended(kTRUE) doens't work
  //  lGAdd->mustBeExtended();
  //  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Constrained(),Warnings(kTRUE),Save(kTRUE),ConditionalObservables(lRPt),Range(range_min, range_max),NumCPU(2),/*Minimizer("Minuit2"),*/Strategy(2),Minos());//,Minos()); //Double Gaussian fit
  //  RooFitResult *pFR = lGAdd->fitTo(lResidVals2D[0],Warnings(kTRUE),Save(kTRUE),ConditionalObservables(lRPt),Constrained(),NumCPU(4)/*Minimizer("Minuit2"),*/,Strategy(2),Minos());//,Minos()); //Double Gaussian fit
  //  RooFitResult *pFR = lGAdd->fitTo(lResidVals2D[0],Constrained(),PrintEvalErrors(-1),Save(kTRUE),ConditionalObservables(lRPt),NumCPU(4),/*Minimizer("Minuit2"),*/Strategy(2),Minos()); //Double Gaussian fit
  //  Verbose(kFALSE), PrintLevel(-1), Warnings(kFALSE), PrintEvalErrors(-1) 


  RooFitResult *pFR = 0;

  cout << "====================" << endl;
  cout << "== FILL histograms from dataset and fitted PDF===" << endl; 
  cout << "====================" << endl;


  //  TString nameHisto="hh_U1";
  //  if(lPar!=fU1) nameHisto="hh_U2";
  //  nameHisto  += "_";
  //  nameHisto += fId;
  
  //  // HISTOs from dataset
  //  TH1* hh  = lResidVals2D[0].createHistogram(nameHisto,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  //  if(hh) cout << " ===>>>>> CREATED 2D HISTOGRAM FILE from dataset" << hh << endl;

  /*
  // HISTOs from fitted pdf
  TH1* hh_ = lGAdd->createHistogram(nameHistoFit,lRPt,Binning(100),YVar(lRXVar,Binning(100)));
  TH1* hh1_ = lGaus1->createHistogram(nameHistoFitG1,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  TH1* hh2_ = lGaus2->createHistogram(nameHistoFitG2,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  TH1* hh3_ = lGaus3->createHistogram(nameHistoFitG3,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
  if(hh1_ && hh2_ && hh3_) cout << " ===>>>>> CREATED 2D HISTOGRAM FILE from fitted PDF" << hh1_ << " " << hh2_ << " " << hh3_<< endl;
  */

  TString fileName2DFIT="file2Dfit_";
  fileName2DFIT += "AUG16_";
  if(!fData && (!doPosW && doNegW) && !doBKG) fileName2DFIT += "Wneg";
  if(!fData && (doPosW && !doNegW) && !doBKG) fileName2DFIT += "Wpos";
  if(!fData && (!doPosW && !doNegW) && !doBKG) fileName2DFIT += "Z";
  if(doBKG) fileName2DFIT += "TOP";
  if(fData) fileName2DFIT += "DATA";
  if(!fData) fileName2DFIT += "MC"; 
  if(doMad && !fData) fileName2DFIT += "_MADGRAPH";
  if(!doMad && !fData) fileName2DFIT += "_POWHEG";
  if(doClosure) fileName2DFIT += "_CLOSURE";
  if(doAbsolute) fileName2DFIT += "_ABSOLUTE";
  if(doIterativeMet) fileName2DFIT += "_ITERATIVE";
  if(useErfPol2ScaleU1) fileName2DFIT += "_ERF";
  fileName2DFIT += "_Y";
  fileName2DFIT += fId;
  fileName2DFIT += "_PDF";
  fileName2DFIT += pType;
  if(do3G)fileName2DFIT += "_3G";
  if(doOnlyU1) fileName2DFIT += "_onlyU1";
  if(doOnlyU2) fileName2DFIT += "_onlyU2";
  if(iParamU1!=-1) fileName2DFIT += "_iParamU1";
  if(iParamU1!=-1) fileName2DFIT += iParamU1;
  if(iParamU2!=-1) fileName2DFIT += "_iParamU2";
  if(iParamU2!=-1) fileName2DFIT += iParamU2;
  if(doHalfStat) fileName2DFIT += "_halfSize";
  if(doRunA) fileName2DFIT += "_runA";
  if(doRunB) fileName2DFIT += "_runB";
  if(doRecoParam) fileName2DFIT += "_recoParam";
  if(doLepProjAbsolute) fileName2D += "_doLepProjAbsolute";

  fileName2DFIT += "_startEntries";
  fileName2DFIT += startTreeEntries;
  fileName2DFIT += ".root";
  
  if(doPrint) {

    TString nameHisto="hh_U1";
    if(lPar!=fU1) nameHisto="hh_U2";
    nameHisto  += "_";
    nameHisto += fId;

    TString nameHisto_="";

    TString nameHistoFit="";
    TString nameHistoFitG1="";
    TString nameHistoFitG2="";
    TString nameHistoFitG3="";


    TFile f4(fileName2DFIT.Data(),"UPDATE");

    for(unsigned int id=1; id<lResidVals.size(); id++) {

      // HISTOs from dataset
      // with Initial values
      nameHisto_  = nameHisto.Data();
      nameHisto_ += "_";
      nameHisto_ += id;
      nameHisto_ += "_1D";

      TH1* hh_1d_  = lResidVals[id].createHistogram(nameHisto_,lRXVar,Binning(1000));
      if(hh_1d_) cout << " ===>>>>> CREATED 1D HISTOGRAM FILE from dataset" << id << " " << hh_1d_ << endl;
      //      if(hh_1d_) hh_1d_->Write(); // the createHistogram already store

      /*
      //      HISTOs from fitted pdf
      // with Initial values
      nameHistoFitG1=nameHisto_.Data();
      nameHistoFitG1 += "_FitG1";
      nameHistoFitG2=nameHisto_.Data();
      nameHistoFitG2 += "_FitG2";
      nameHistoFitG3=nameHisto_.Data();
      nameHistoFitG3 += "_FitG3";
      TH1* hh_ = lGAdd->createHistogram(nameHistoFit,lRXVar,Binning(100));
      TH1* hh1_1d_ = lRGaus1->createHistogram(nameHistoFitG1,lRXVar,Binning(100));
      TH1* hh2_1d_ = lRGaus2->createHistogram(nameHistoFitG2,lRXVar,Binning(100));
      TH1* hh3_1d_ = lRGaus3->createHistogram(nameHistoFitG3,lRXVar,Binning(100));
      if(hh1_1d_ && hh2_1d_ && hh3_1d_) cout << " ===>>>>> CREATED 1D HISTOGRAM FILE from fitted PDF" << hh1_1d_ << " " << hh2_1d_ << " " << hh3_1d_ <<endl;
      */

    }

    // HISTOs from dataset (this create both 2D and other projections)
    //    TH1* hh  = lResidVals2D[0].createHistogram(nameHisto,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
    //    if(hh) cout << " ===>>>>> CREATED 2D HISTOGRAM FILE from dataset" << hh << endl;

    //    if(hh) hh->Write();
    //    if(hh1_) hh1_->Write();
    //    if(hh2_) hh2_->Write();
    //    if(hh3_) hh3_->Write();

    f4.Write();
    f4.Close();

  }

  if(writeTree) return;
  //  if(doBKG) return;
  if(doClosure || doAbsolute) return;
  //  if(doAbsolute) return;

  //// AFTER here set up for the 1D fit                                                                                                                    
  //  if(doPrintAll && !doIterativeMet) {
  if(doPrintAll) {

    //////
    //////
    //////
    //////
    //////

    //Plot it all
    lRXVar.setBins(100); // Bins of the histograms with the Pull
    
    double *lX   = new double[lNBins];
    double *lY0  = new double[lNBins];
    double *lY1  = new double[lNBins];
    double *lY2  = new double[lNBins];
    double *lY3  = new double[lNBins];
    
    double *lEX  = new double[lNBins];
    double *lEY0 = new double[lNBins];
    double *lEY1 = new double[lNBins];
    double *lEY2 = new double[lNBins];
    double *lEY3 = new double[lNBins];

    double *lchi2  = new double[lNBins];
    double *myFrac = new double[lNBins];
    double *myFrac2 = new double[lNBins];
    double *myFracSecond = new double[lNBins];
    double *myFracSecondE = new double[lNBins];
    double *myMean = new double[lNBins];
    double *myMeanLarge = new double[lNBins];
    double *myMeanVeryLarge = new double[lNBins];
    double *myFracE = new double[lNBins];
    double *myFrac2E = new double[lNBins];
    double *myMeanE = new double[lNBins];
    double *myMeanLargeE = new double[lNBins];
    double *myMeanVeryLargeE = new double[lNBins];

    double *myFrac2D = new double[lNBins];
    double *myFrac22D = new double[lNBins];
    double *myFracSecond2D = new double[lNBins];
    double *myFrac2DE = new double[lNBins];
    double *myFrac22DE = new double[lNBins];
    double *myFracSecond2DE = new double[lNBins];

    double *myMean2D = new double[lNBins];
    double *myMeanLarge2D = new double[lNBins];
    double *myMeanVeryLarge2D = new double[lNBins];
    double *myMean2DE = new double[lNBins];
    double *myMeanLarge2DE = new double[lNBins];
    double *myMeanVeryLarge2DE = new double[lNBins];
    double *lY02D = new double[lNBins];
    double *lY12D = new double[lNBins];
    double *lY22D = new double[lNBins];
    double *lY32D = new double[lNBins];
    double *lEY02D = new double[lNBins];
    double *lEY12D = new double[lNBins];
    double *lEY22D = new double[lNBins];
    double *lEY32D = new double[lNBins];

    double *color = new double[lNBins];

    for(unsigned int i0  = range_min; i0 < range_max; i0++) { 

    /*
    for(int i0  = 0; i0 < fZPtMax ; i0++) { 
      
      if(i0<range_min || i0>=range_max) {

	lchi2  = 0;
	lX   = 0; lEX  = 0;
	myMean = 0; myMeanE = 0;
	lY0  = 0; lEY0 = 0;
	lY1  = 0; lEY1 = 0;
	lY2  = 0; lEY2 = 0;
	lY3  = 0; lEY3 = 0;
       
        myFrac = 0; myFracE = 0;
	myFracSecond = 0; myFracSecondE = 0;            
        myFrac2D = 0; myFrac2DE =0;    
        myFracSecond2D = 0; myFracSecond2DE = 0;

	myMean2D = 0; myMean2DE = 0;

        lY02D = 0; lEY02D = 0;
	lY12D = 0; lEY12D = 0;
	lY22D = 0; lEY22D = 0; 
	lY32D = 0; lEY32D = 0;

	continue;
      }
    */

      cout << "------------"<< endl;
      cout << "------------"<< endl;
      cout << "--- binned Fit: doing now " << i0 << "---" << lResidVals[i0].GetName() << endl;
      cout << "------------"<< endl;
      cout << "------------"<< endl;
      
      TString legVPT = Form("Zpt = %d",i0);
	
      RooFitResult *fr= 0;
      if(doBKG && fData) {

	constructPDFbkg(lPar,i0);

	fr = model->fitTo(lResidVals[i0],Warnings(kTRUE),Save(kTRUE),NumCPU(4),Minimizer("Minuit2","migrad"),Strategy(2)/*,Minos()*/);//,Minos()); //Double Gaussian fit for the binned fit

	RooPlot* xframe  = lRXVar.frame(Title("Test 1D BKG")) ;

	lResidVals[i0].plotOn(xframe) ;
	RooArgSet* comps = model->getComponents() ;
	comps->Print();

	model->plotOn(xframe,LineColor(kBlack)) ;
	model->plotOn(xframe,LineColor(kBlack),LineStyle(kDotted),FillColor(kGreen),DrawOption("F")) ;
	//model->plotOn(xframe,Components("bkg_pdf,RAdd"),LineColor(kGreen),LineStyle(kDotted),FillColor(kGreen),DrawOption("F")) ;
  	model->plotOn(xframe,Components("bkg_pdf")     ,LineColor(kBlack)  ,LineStyle(kDashed),FillColor(kRed)  ,DrawOption("F")) ;
	lResidVals[i0].plotOn(xframe) ;


	TString nameCanvasOn="PLOT/CanvasBKG_";
	if(lPar==fU1) nameCanvasOn += "U1_";
	if(lPar!=fU1) nameCanvasOn += "U2_";
	nameCanvasOn  += i0;
	nameCanvasOn  += ".png";

	TCanvas* c = new TCanvas("validateBKGPDF","validateBKGPDF",800,600) ;
	c->cd();
	c->SetLogy(1);

	xframe->GetYaxis()->SetRangeUser(0.01,5000);
	xframe->Draw() ;
	xframe->SetTitle("") ;

	latexLabel.DrawLatex(0.20, 0.85, leg1);
	latexLabel.DrawLatex(0.20, 0.8, legVPT);
	latexLabel.DrawLatex(0.20, 0.75, legDATA);
	latexLabel.DrawLatex(0.20, 0.7, legU1U2);

	TString statusstr = Form("status = %d",fr->status());
	latexLabel.DrawLatex(0.7, 0.85, statusstr);
	TString bkgFrac = Form("fracSig = %.3f",sigbkgFrac->getVal());
	latexLabel.DrawLatex(0.7, 0.8, bkgFrac);
	//      cout << "printed chi2" << endl;

	c->SaveAs(nameCanvasOn.Data());

      }   else  {

	fr= lRGAdd->fitTo(lResidVals[i0],Warnings(kTRUE),Save(kTRUE),NumCPU(4),Minimizer("Minuit2","migrad"),Strategy(2)/*,Minos()*/);//,Minos()); //Double Gaussian fit for the binned fit

      }
      
      /////      lRPt.setRange(i0*10,i0*10+10); /// not sure is used 
      cout << "------------"<< endl;
      cout << "------------"<< endl;
      cout << "--- binned Fit: done ---"<< endl;
      cout << "------------"<< endl;
      cout << "------------"<< endl;

      RooPlot *lFrame1 = lRXVar.frame() ;
      if(lPar==fU1) lFrame1->SetName(Form("1DFrame_%d_U1_Y%d",i0,fId));
      if(lPar!=fU1) lFrame1->SetName(Form("1DFrame_%d_U2_Y%d",i0,fId));
      lFrame1->SetTitle("");
      //lResidVals2D[0].plotOn(lFrame1);

      // plot the results of the binned fit

      cout << "====================" << endl;
      cout << "== plot the results of the binned fit " << i0 << endl;
      cout << "====================" << endl;

      lResidVals[i0].plotOn(lFrame1,MarkerColor(kRed));

      if(doKeys) {

	std::stringstream pKey1;
	pKey1 << "Keys";
	if(lPar==fU1) pKey1 << "_U1";
	if(lPar!=fU1) pKey1 << "_U2";
	if(fId==1) pKey1 << "_1_";
	if(fId==2) pKey1 << "_2_";
	pKey1 << i0;
	//	cout << "filling keys " << pKey1.str().c_str() << endl;
	RooKeysPdf pdf_keys(pKey1.str().c_str(), pKey1.str().c_str(), lRXVar, lResidVals[i0] , RooKeysPdf::MirrorBoth,2);
	///      lResidValsKeys.push_back(pdf_keys);

	pdf_keys.plotOn(lFrame1,LineColor(kGray)) ;

      }

      //see slide 18 in http://roofit.sourceforge.net/docs/tutorial/plot/roofit_tutorial_plot.pdf
      // drawing options in http://root.cern.ch/root/html/tutorials/roofit/rf610_visualerror.C.html
      // results of drawing options https://www.slac.stanford.edu/grp/eg/minos/ROOTSYS/cvs/roofit/doc/v524/
      //Integrating out per-event errors 
      //      lRGAdd.plotOn(lFrame1,RooFit::LineColor(kBlue));
      //Projecting per-event errors with data

      if(fr) {
	//	lRGAdd->plotOn(lFrame1,ProjWData(lRXVar,lResidVals[i0]),RooFit::LineColor(kBlue));
	lRGAdd->plotOn(lFrame1,FillColor(7),VisualizeError(*fr,1),RooFit::Components(*lRGAdd)); // 1 sigma band  
	// draw 1 sigma vand
	if(do3G) lRGAdd->plotOn(lFrame1,FillColor(kOrange-3),VisualizeError(*fr,1),RooFit::Components(*lRGaus1)); // 1 sigma band
	if(do1CB2G) lRGAdd->plotOn(lFrame1,FillColor(kOrange-3),VisualizeError(*fr,1),RooFit::Components(*lRCB)); // 1 sigma band
	//      lRGAdd.plotOn(lFrame1,LineStyle(kDashed),LineColor(kRed),LineWidth(2),VisualizeError(*fr,1,kFALSE),DrawOption("L"),RooFit::Components(lRGaus1));
	lRGAdd->plotOn(lFrame1,FillColor(kMagenta-9),VisualizeError(*fr,1),RooFit::Components(*lRGaus2)); // 1 sigma band
	//      lRGAdd.plotOn(lFrame1,LineStyle(kDashed),LineColor(kViolet),LineWidth(2),VisualizeError(*fr,1),DrawOption("L"),RooFit::Components(lRGaus2));
	if(do3G || do1CB2G) lRGAdd->plotOn(lFrame1,FillColor(kGreen+2),VisualizeError(*fr,1),RooFit::Components(*lRGaus3)); // 1 sigma band
	//      lRGAdd.plotOn(lFrame1,LineStyle(kDashed),LineColor(kViolet),LineWidth(2),VisualizeError(*fr,1),DrawOption("L"),RooFit::Components(lRGaus2));
	// draw central value
	lRGAdd->plotOn(lFrame1,RooFit::LineColor(kBlue));   
	lRGAdd->plotOn(lFrame1,RooFit::Components(*lRGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed)); // central value
	lRGAdd->plotOn(lFrame1,RooFit::Components(*lRGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(kViolet)); // central value
	if(do3G) lRGAdd->plotOn(lFrame1,RooFit::Components(*lRGaus3),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen)); // central value
	lRGAdd->paramOn(lFrame1, Format("NELU", AutoPrecision(2)), Layout(0.16, 0.5,0.95) );

	TFile f15(rootFileNameFrame.c_str(),"UPDATE");
	
	if(lFrame1) lFrame1->Write();
	f15.Write();
	f15.Close();

      }


      if(i0==5 || i0==10 || i0==15 || i0==20 || i0==25) {
	TString nameHistoOn="hh_plotOn_U1";
	if(lPar!=fU1) nameHistoOn="hh_plotOn_U2";
	nameHistoOn  += "_";
	nameHistoOn += fId;
	nameHistoOn  += "_Zpt";
	nameHistoOn += i0;
	
	TString nameHistoOnFit=nameHistoOn.Data();
	nameHistoOnFit += "_Fit";
	TString nameHistoOnFitG1=nameHistoOn.Data();
	nameHistoOnFitG1 += "_FitG1";
	TString nameHistoOnFitG2=nameHistoOn.Data();
	nameHistoOnFitG2 += "_FitG2";
	TString nameHistoOnFitG3=nameHistoOn.Data();
	nameHistoOnFitG3 += "_FitG3";
	
	TH1* hh1D  = lResidVals[i0].createHistogram(nameHistoOn,lRXVar,Binning(1000));
	TH1* hh1D_F = lRGAdd->createHistogram(nameHistoOnFit,lRXVar,Binning(1000));
	TH1* hh1D_F1 = lRGaus1->createHistogram(nameHistoOnFitG1,lRXVar,Binning(1000));
	TH1* hh1D_F2 = lRGaus2->createHistogram(nameHistoOnFitG2,lRXVar,Binning(1000));
	TH1* hh1D_F3 = lRGaus3->createHistogram(nameHistoOnFitG3,lRXVar,Binning(1000));

	TFile f5(fileName2DFIT.Data(),"UPDATE");
	
      	if(hh1D) hh1D->Write();
	if(hh1D_F) hh1D_F->Write();
	if(hh1D_F1) hh1D_F1->Write();
	if(hh1D_F2) hh1D_F2->Write();
	if(hh1D_F3) hh1D_F3->Write();

	f5.Write();
	f5.Close();


      }


      cout << "====================" << endl;
      cout << "== plot the results of the binned fit " << endl;
      cout << "====================" << endl;

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
      
      //      cout << "++++> CHI2=" << lFrame1->chiSquare() << " reduced CHI2=" << lFrame1->chiSquare(fr->floatParsFinal().getSize()) << endl;

      //      cout << " namePDF=" << lRGAdd->GetName() << " nameDataset= " << lResidVals[i0].GetName() << endl;
      //      (RooHist::h_Crapsky4,RooCurve::RAdd_Norm[XVar],RooCurve::RAdd_Norm[XVar]_errorband_Comp[Rgaus1],RooCurve::RAdd_Norm[XVar]_errorband_Comp[Rgaus2],RooCurve::RAdd_Norm[XVar]_Comp[Rgaus1],RooCurve::RAdd_Norm[XVar]_Comp[Rgaus2])
      lFrame1->Print(); 

      if(fr) {

	TString nameRooHist=Form("h_%s",lResidVals[i0].GetName());
	TString nameRooCurve="RAdd_Norm[XVar]";
	
	//	TString chi2str = "0";
	//	lchi2[i0]=0;

	TString chi2str = Form("chi2/ndof = %.2f",lFrame1->chiSquare("RAdd_Norm[XVar]",nameRooHist.Data()));
	if(!fr->status()) {
	  lchi2[i0-int(range_min)]=lFrame1->chiSquare("RAdd_Norm[XVar]",nameRooHist.Data());
	} else {
	  lchi2[i0-int(range_min)]=0;
	}

	//      TString chi2str = Form("chi2/ndof = %.2f",lFrame1->chiSquare(fr->floatParsFinal().getSize()));
	latexLabel.DrawLatex(0.65, 0.8, chi2str);
	TString statusstr = Form("status = %d",fr->status());
	latexLabel.DrawLatex(0.65, 0.75, statusstr);

	/*
	// OPTIONAL PRINTING
	TString frac1str = Form("frac1 = %.2f",lR1Frac->getVal());
	latexLabel.DrawLatex(0.65, 0.7, frac1str);
	if(do3G) {
	  TString frac2str = Form("frac2 = %.2f",lRFrac->getVal()*(1-lR1Frac->getVal()));
	  latexLabel.DrawLatex(0.65, 0.65, frac2str);
	  TString frac3str = Form("frac3 = %.2f",(1-lR1Frac->getVal())*(1-lRFrac->getVal()));
	  latexLabel.DrawLatex(0.65, 0.6, frac3str);
	} 
	TString rms1str = Form("sigma1 = %.2f",lR1Sigma.getVal());
	latexLabel.DrawLatex(0.65, 0.55, rms1str);
	TString rms2str = Form("sigma2 = %.2f",lR2Sigma.getVal());
	latexLabel.DrawLatex(0.65, 0.5, rms2str);
	TString rms3str = Form("sigma3 = %.2f",lR3Sigma.getVal());
	latexLabel.DrawLatex(0.65, 0.45, rms3str);
	TString meanstr = Form("mean = %.2f",lR1Mean.getVal());
	latexLabel.DrawLatex(0.65, 0.4, meanstr);
	*/

      }

      ///////

      latexLabel.DrawLatex(0.20, 0.45, leg1);
      //      if(doVTXbinning) latexLabel.DrawLatex(0.25, 0.45, leg3);
      latexLabel.DrawLatex(0.20, 0.4, legVPT);
      latexLabel.DrawLatex(0.20, 0.25, legDATA);
      latexLabel.DrawLatex(0.20, 0.3, legU1U2);

      //      if(!doMad && (!fData)) latexLabel.DrawLatex(0.15, 0.2, "powheg");
      //      if(doMad && (!fData)) latexLabel.DrawLatex(0.15, 0.2, "madgraph");

      TLine *lineZero = new TLine(0, 0,  0, 50000);
      lineZero->SetLineColor(11);
      lineZero->Draw("same");

      TLine *lineZeroX = new TLine(-5., 0,  5., 0);
      lineZeroX->SetLineColor(11);
      lineZeroX->Draw("same");

      TString test="";
      
      if(iFitS1==0) test += "PLOT/mean";
      if(iFitS1!=0 && !doAbsolute) test += "PLOT/pull";
      if(iFitS1!=0 && doAbsolute) test += "PLOT/rms";
      if(iFitS1!=0 && doBKG)  test += "bkg";

      test += "_Zpt";
      test += i0;

      test += labeling.Data();

      TString testPDF=Form("%s.pdf",test.Data());
      test += ".png";
      
      iC->SaveAs(test.Data());
      if(i0==5 || i0==10 || i0==15) iC->Print(testPDF.Data());

      // http://root.cern.ch/root/html/src/ROOT__Minuit2__Minuit2Minimizer.h.html#140
      // https://root.cern.ch/phpBB3/viewtopic.php?f=15&t=16764
      // status=0 ok others bad

      //      lX[i0] = range_min+((range_max-range_min)/lNBins)*i0;
      lX[i0-int(range_min)] = i0;
      //      lEX[i0] = range_min+((range_max-range_min)/lNBins)/1000;
      lEX[i0-int(range_min)] = 0;

      // normalization too much
      //      lY0[i0] = (lR1Frac .getVal()*lR1Sigma.getVal() + (1.-lR1Frac.getVal())*lR2Sigma.getVal())/sqrt(2*3.14159265)*2.;

      // 1D - central values

      if(!fr->status()) {
	lY0[i0-int(range_min)] = (lR1Frac->getVal()*lR1Sigma.getVal() + (1.-lR1Frac->getVal())*lR2Sigma.getVal()); 
	if(do3G) lY0[i0-int(range_min)] = (lR1Frac->getVal()*lR1Sigma.getVal() + lRFrac->getVal()*(1.-lR1Frac->getVal())*lR2Sigma.getVal() + (1.-lR1Frac->getVal())*(1.-lRFrac->getVal())*lR3Sigma.getVal()); 
	lEY0[i0-int(range_min)]  = 0;
	// sigma 1,2,3
	lY1[i0-int(range_min)] = lR1Sigma.getVal();///sqrt(2*3.14159265)*2.;
	lY2[i0-int(range_min)] = lR2Sigma.getVal();///sqrt(2*3.14159265)*2.;
	if(do3G) lY3[i0-int(range_min)] = lR3Sigma.getVal();///sqrt(2*3.14159265)*2.;
	lEY1[i0-int(range_min)] = lR1Sigma.getError();///sqrt(2*3.14159265)*2.;
	lEY2[i0-int(range_min)] = lR2Sigma.getError();///sqrt(2*3.14159265)*2.;
	if(do3G) lEY3[i0-int(range_min)] = lR3Sigma.getError();///sqrt(2*3.14159265)*2.;
	// mean
	myMean[i0-int(range_min)] = lR1Mean.getVal();
	myMeanE[i0-int(range_min)] = lR1Mean.getError();
	myMeanLarge[i0-int(range_min)] = lR1MeanLarge.getVal();
	myMeanLargeE[i0-int(range_min)] = lR1MeanLarge.getError();
	myMeanVeryLarge[i0-int(range_min)] = lR1MeanVeryLarge.getVal();
	myMeanVeryLargeE[i0-int(range_min)] = lR1MeanVeryLarge.getError();
	// fraction
	myFrac[i0-int(range_min)] = lR1Frac->getVal();
        myFracE[i0-int(range_min)] = lR1Frac->getPropagatedError(*fr) ;
	myFrac2[i0-int(range_min)] = lRFrac->getVal(); // second fraction
        myFrac2E[i0-int(range_min)] = lRFrac->getPropagatedError(*fr) ;

	//	if(do3G) myFracSecond[i0-int(range_min)] = lRFrac->getVal()*(1-lR1Frac->getVal());
	//	if(do3G && pFR) myFracSecond2D[i0-int(range_min)] = lFrac->getVal()*(1-l1Frac->getVal());
      } else {
	// if the fit fail set the value to zero and with large error
	lY0[i0-int(range_min)] = 0;
	lY1[i0-int(range_min)] = 0;
	lY2[i0-int(range_min)] = 0;
	lY3[i0-int(range_min)] = 0;
	myMean[i0-int(range_min)] = 0;
	myMeanLarge[i0-int(range_min)] = 0;
	myMeanVeryLarge[i0-int(range_min)] = 0;
	myFrac[i0-int(range_min)] = 0;
	myFrac2[i0-int(range_min)] = 0;
	lEY0[i0-int(range_min)] = 999.;
	lEY1[i0-int(range_min)] = 999.;
	lEY2[i0-int(range_min)] = 999.;
	lEY3[i0-int(range_min)] = 999.;
	myMeanE[i0-int(range_min)] = 999.;
	myMeanLargeE[i0-int(range_min)] = 999.;
	myMeanVeryLargeE[i0-int(range_min)] = 999.;
	myFracE[i0-int(range_min)] = 999.;
	myFrac2E[i0-int(range_min)] = 999.;
      }


      // errors

//	lEY0[i0]  = lR1Frac .getError()*lR1Sigma.getVal()  * lR1Frac .getError()*lR1Sigma.getVal();
//	lEY0[i0] += lR1Frac .getVal()  *lR1Sigma.getError()* lR1Frac .getVal()  *lR1Sigma.getError();
//	lEY0[i0] += lR1Frac .getError()*lR2Sigma.getVal()  * lR1Frac .getError()*lR2Sigma.getVal();
//	lEY0[i0] += (1-lR1Frac .getVal())*lR2Sigma.getError()* (1-lR1Frac .getVal())  *lR2Sigma.getError();
//	lEY0[i0] = sqrt(lEY0[i0])/sqrt(2*3.14159265)*2.;
//	myFracE[i0] = lR1Frac.getError();
      
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 
      cout << "i0=" << i0 << "  MEAN-2d=" << " pt-dependent:  " << lAMean1.getVal()+lBMean1.getVal()*i0+lCMean1.getVal()*i0*i0 << endl;
      cout << "i0=" << i0 << "  MEAN-1d=" << lR1Mean.getVal() << endl;
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 
      cout << "---------------"<< endl; 

      /*      
      if(!do3G) {
	// recompute the Fraction to get the same definition of the 2D fit
	myFrac1D[i0] = ((lY2[i0]-1.)/(lY2[i0]-lY1[i0]));
	//      myFrac1D[i0] = ((lY2[i0]*lY2[i0]-1.)/(lY2[i0]*lY2[i0]-lY1[i0]*lY1[i0])); // squares
	
	
	double tmpFrac2D=((lY22D[i0]-1.)/(lY22D[i0]-lY12D[i0]));
	lY02D[i0] = (tmpFrac2D*lY12D[i0] + (1.-tmpFrac2D)*lY22D[i0]);
	//      double tmpFrac2D=((lY22D[i0]*lY22D[i0]-1.)/(lY22D[i0]*lY22D[i0]-lY12D[i0]*lY12D[i0])); // for squares
	//      lY02D[i0] = (tmpFrac2D*lY12D[i0]*lY12D[i0] + (1.-tmpFrac2D)*lY22D[i0]*lY22D[i0]);
	myFrac2D[i0] = tmpFrac2D;
      }
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

    cout << "=====================" << endl;
    cout << "==== doing Graph ====" << endl; 
    cout << "=====================" << endl;


    TGraphErrors *lM0 = new TGraphErrors(lNBins,lX,myMean,lEX,myMeanE); lM0->SetMarkerStyle(kFullCircle); // this is the mean fitted of the two gaussian
    TGraphErrors *lM0large = new TGraphErrors(lNBins,lX,myMeanLarge,lEX,myMeanLargeE); lM0large->SetMarkerStyle(kFullCircle); // this is the mean fitted of the two gaussian
    TGraphErrors *lM0Verylarge = new TGraphErrors(lNBins,lX,myMeanVeryLarge,lEX,myMeanVeryLargeE); lM0Verylarge->SetMarkerStyle(kFullCircle); // this is the mean fitted of the two gaussian
    TGraphErrors *lG0 = new TGraphErrors(lNBins,lX,lY0,lEX,lEY0); lG0->SetMarkerStyle(kFullCircle); // this is the RMS obtained with the two fitted gaussian and RMS 
    TGraphErrors *lG1 = new TGraphErrors(lNBins,lX,lY1,lEX,lEY1); lG1->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG2 = new TGraphErrors(lNBins,lX,lY2,lEX,lEY2); lG2->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG3 = new TGraphErrors(lNBins,lX,lY3,lEX,lEY3); lG3->SetMarkerStyle(kFullCircle);
    TGraphErrors* gFrac = new TGraphErrors(lNBins, lX, myFrac, lEX, myFracE);  gFrac->SetMarkerStyle(kFullCircle);
    TGraphErrors* gFrac2 = new TGraphErrors(lNBins, lX, myFrac2, lEX, myFrac2E);  gFrac->SetMarkerStyle(kFullCircle);

    TGraphErrors *lG0_2d = new TGraphErrors(lNBins,lX,lY02D,lEX,lEY02D); lG0_2d->SetMarkerStyle(kOpenSquare); // this is the convoluted RMS

    /*
    if(lPar==fU1) lM0->GetYaxis()->SetTitle("U1 mean");
    if(lPar!=fU1) lM0->GetYaxis()->SetTitle("U2 mean");
    if(lPar==fU1) lG0->GetYaxis()->SetTitle("U1 #sigma_{mean}");
    if(lPar!=fU1) lG0->GetYaxis()->SetTitle("U2 #sigma_{mean}");
    */

    if(lPar==fU1) lM0->GetYaxis()->SetTitle("mean [ (U#parallel-U#perp_{fit})/#sigma_{fit}(U#parallel) ]");
    if(lPar==fU1) lM0large->GetYaxis()->SetTitle("mean [ (U#parallel-U#perp_{fit})/#sigma_{fit}(U#parallel) ]");
    if(lPar!=fU1) lM0->GetYaxis()->SetTitle("mean [ (U#perp-U#perp_{fit})/#sigma_{fit}(Uperp) ]");
    if(lPar==fU1) lG0->GetYaxis()->SetTitle("RMS [ (U#parallel-U#parallel_{fit})/#sigma_{fit}(U#parallel) ]");
    if(lPar!=fU1) lG0->GetYaxis()->SetTitle("RMS [ (U#perp-U#perp_{fit})/#sigma_{fit}(U#perp) ]");

    lG1->GetYaxis()->SetTitle("#sigma_{1}");
    lG2->GetYaxis()->SetTitle("#sigma_{2}");
    lG3->GetYaxis()->SetTitle("#sigma_{3}");
    gFrac->GetYaxis()->SetTitle("fraction_{sigma_{1}}");
    gFrac2->GetYaxis()->SetTitle("fraction_{sigma_{2}}");
    
    double SFabs=1;
    if(doAbsolute) SFabs=10;
    //    lM0->GetYaxis()->SetRangeUser(-1.,1.);
    lM0->GetYaxis()->SetRangeUser(-0.5,0.5);
    lM0large->GetYaxis()->SetRangeUser(-0.5,0.5);
    lG0->GetYaxis()->SetRangeUser(0.,2.*SFabs);
    lG1->GetYaxis()->SetRangeUser(0.,1.5*SFabs);
    lG2->GetYaxis()->SetRangeUser(0.,5.*SFabs);
    lG3->GetYaxis()->SetRangeUser(0.,5.*SFabs);
    gFrac->GetYaxis()->SetRangeUser(0.,1.5);
    gFrac2->GetYaxis()->SetRangeUser(0.,1.5);
    
    lM0->GetXaxis()->SetTitle("Z p_{T}");
    lM0large->GetXaxis()->SetTitle("Z p_{T}");
    lG0->GetXaxis()->SetTitle("Z p_{T}");
    lG1->GetXaxis()->SetTitle("Z p_{T}");
    lG2->GetXaxis()->SetTitle("Z p_{T}");
    lG3->GetXaxis()->SetTitle("Z p_{T}");
    gFrac->GetXaxis()->SetTitle("Z p_{T}");
    gFrac2->GetXaxis()->SetTitle("Z p_{T}");

    if(doPosW || doNegW) {
      lM0->GetXaxis()->SetTitle("W p_{T}");
      lM0large->GetXaxis()->SetTitle("W p_{T}");
      lG0->GetXaxis()->SetTitle("W p_{T}");
      lG1->GetXaxis()->SetTitle("W p_{T}");
      lG2->GetXaxis()->SetTitle("W p_{T}");
      lG3->GetXaxis()->SetTitle("W p_{T}");
      gFrac->GetXaxis()->SetTitle("W p_{T}");
      gFrac2->GetXaxis()->SetTitle("W p_{T}");
    }

    lM0->SetTitle("");
    lM0large->SetTitle("");
    lG0->SetTitle("");
    lG1->SetTitle("");
    lG2->SetTitle("");
    lG3->SetTitle("");
    gFrac->SetTitle("");
    gFrac2->SetTitle("");

    lG1->GetXaxis()->SetTitleSize(0.05);
    lG2->GetXaxis()->SetTitleSize(0.05);
    lG3->GetXaxis()->SetTitleSize(0.05);
    gFrac->GetXaxis()->SetTitleSize(0.05);
    lG1->GetYaxis()->SetTitleSize(0.05);
    lG2->GetYaxis()->SetTitleSize(0.05);
    lG3->GetYaxis()->SetTitleSize(0.05);
    gFrac->GetYaxis()->SetTitleSize(0.05);
    gFrac2->GetYaxis()->SetTitleSize(0.05);

    //////
    //// cin.get();
    //////

    //    for(int ix=0; ix<=lNBins; ix++) {
    //      cout << ix << " lX=" << lX[ix] << " lchi2=" << lchi2[ix]<< endl;
    //    } 


    iC->cd();
    iC->Clear();
    //    iC->Divide(2,2);
    //    iC->cd(1);
    TGraph* gChi2 = new TGraph(lNBins, lX, lchi2);
    if(lPar==fU1) gChi2->SetName("chi2_U1");
    if(lPar!=fU1) gChi2->SetName("chi2_U2");
    if(lPar==fU1) gChi2->SetTitle("U#perp");
    if(lPar!=fU1) gChi2->SetTitle("U#parallel");
    gChi2->SetMarkerColor(kBlue);
    gChi2->SetMarkerStyle(kOpenSquare);
    gChi2->GetXaxis()->SetTitle("Z p_{T}");
    gChi2->GetYaxis()->SetTitle("chi2");
    gChi2->Draw("AP");
    //    gPad->Modified();

    ////
    ////
    TString pSS6="PLOTNOTE/chi2Plot";
    pSS6 += labeling.Data();
    TString pSS6pdf=Form("%s.pdf",pSS6.Data());
    pSS6 += ".png";
    gPad->SaveAs(pSS6.Data());
    gPad->Print(pSS6pdf.Data());

    iC->cd();
    TF1 *lFitPull  = new TF1("lFitPull",   "pol2");
    TF1 *iFitPull  = new TF1("iFitPull",   "pol10"); // this need to be large to store the errors from the computeFitErrors
    TFitResultPtr  lFitPtr0 = lG0->Fit(lFitPull,"SRE","", range_min , range_max); //"EXO"                                                                       
    cout << " lFitPull " << lFitPull << "lFitPtr0 " << lFitPtr0 << endl;

    computeFitErrors(iFitPull,lFitPtr0,lFitPull,iRMS);
    lG0->Draw("ape");
    drawErrorBands(iFitPull,fZPtMax);
    lG0->Draw("pe");
    // superimpose the 2D fit
    lG0_2d->Draw("pe");

    latexLabel.DrawLatex(0.20, 0.7, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.7, leg3);
    //    latexLabel.DrawLatex(0.15, 0.8, "Y(V)<1");
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

    TString pSS0="PLOTNOTE/G0";
    pSS0 += labeling.Data();
    TString pSS0pdf=Form("%s.pdf",pSS0.Data());
    pSS0 += ".png";
    iC->SaveAs(pSS0.Data());
    iC->Print(pSS0pdf.Data());

    //////
    //// cin.get();
    //////
    TF1 *lFitS1  = new TF1("lFitS1",   "pol2");
    TFitResultPtr  lFitPtr1 = lG1->Fit(lFitS1,"SRE","", range_min , range_max); //"EXO"

    TF1 *lFitS2  = new TF1("lFitS2",   "pol2");
    TFitResultPtr  lFitPtr2 = lG2->Fit(lFitS2,"SRE","", range_min , range_max); 

    TF1 *lFitS3  = new TF1("lFitS3",   "pol2");
    TFitResultPtr  lFitPtr3 = lG3->Fit(lFitS3,"SRE","", range_min , range_max); 

    TF1 *iFitFrac  = new TF1("iFitFrac",   "pol10");
    TF1 *lFitFrac  = new TF1("lFitFrac",   "pol2");
    TFitResultPtr  lFitPtrFrac = gFrac->Fit(lFitFrac,"SRE","", range_min , range_max); //"EXO"

    TF1 *iFitFrac2  = new TF1("iFitFrac2",   "pol10");
    TF1 *lFitFrac2  = new TF1("lFitFrac2",   "pol2");
    TFitResultPtr  lFitPtrFrac2 = gFrac2->Fit(lFitFrac2,"SRE","", range_min , range_max); //"EXO"

    TF1 *lFitPullMean1  = new TF1("lFitPullMean1",   "pol2");
    TF1 *iMean1Fit  = new TF1("iMean1Fit",   "pol10"); // this need to be large to store the errors from the computeFitErrors
    TFitResultPtr  lFitPtrM0 = lM0->Fit(lFitPullMean1,"SRE","", range_min , range_max); //"EXO"

    TF1 *lFitPullMean2  = new TF1("lFitPullMean2",   "pol2");
    TF1 *iMean2Fit  = new TF1("iMean2Fit",   "pol10"); // this need to be large to store the errors from the computeFitErrors
    TFitResultPtr  lFitPtrM0large = lM0large->Fit(lFitPullMean2,"SRE","", range_min , range_max); //"EXO"

    //    TF1 *lFitPullMean3  = new TF1("lFitPullMean3",   "pol2");
    //    TF1 *iMean3Fit  = new TF1("iMean3Fit",   "pol10"); // this need to be large to store the errors from the computeFitErrors
    //    TFitResultPtr  lFitPtrM0Verylarge = lM0Verylarge->Fit(lFitPullMean3,"SRE","", range_min , range_max); //"EXO"


    if(do1CB2G) return; //CHRISTOS

    cout << "============" << endl;
    cout << "============" << endl;
    cout << "=== Unbinned Fit: doing now ===" << endl;
    cout << "============" << endl;
    cout << "============" << endl;
    
    //    constructPDF2d(lFitS1,lFitS2,lFitS3,lFitFrac,lFitPullMean1,lFitPullMean2,lFitPullMean3,lPar);
    constructPDF2d(lFitS1,lFitS2,lFitS3,lFitFrac,lFitFrac2,lFitPullMean1,lFitPullMean2,lPar);

    if(fData && doBKG) {

      constructPDFbkg2D(lPar);

      pFR = model2D->fitTo(lResidVals2D[0],Constrained(),PrintEvalErrors(-1),Save(kTRUE),ConditionalObservables(lRPt),NumCPU(4),Minimizer("Minuit2","migrad"),Strategy(2)/*,Minos()*/); 

    } else {
      
      pFR = lGAdd->fitTo(lResidVals2D[0],Constrained(),PrintEvalErrors(-1),Save(kTRUE),ConditionalObservables(lRPt),NumCPU(4),Minimizer("Minuit2","migrad"),Strategy(2)/*,Minos()*/); 
      
      RooPlot *lFrame2D = lRXVar.frame() ;
      lFrame2D->SetName("2DFrame");
      lFrame2D->SetTitle("");

      lGAdd->plotOn(lFrame2D,FillColor(kGreen),VisualizeError(*pFR,1),RooFit::Components(*lGAdd)); // 1 sigma band  
      lGAdd->plotOn(lFrame2D,ProjWData(lRXVar,lResidVals2D[0]),RooFit::LineColor(kGreen+2));

      //      RooPlot *lFrame2Dpt = lRPt.frame() ;
      //      lFrame2Dpt->SetName("2DFramePt");
      //      lFrame2Dpt->SetTitle("2DFramePt");

      //      lGAdd->plotOn(lFrame2Dpt,FillColor(kGreen),VisualizeError(*pFR,1),RooFit::Components(*lGAdd)); // 1 sigma band  
      //      lGAdd->plotOn(lFrame2Dpt,ProjWData(lRXVar,lResidVals2D[0]),RooFit::LineColor(kGreen+2));

      // Plot the whole dataset and gaussians
      //      lGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals2D[0]),RooFit::LineColor(kGreen+2));
      //      lGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals2D[0]),RooFit::Components(lGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(36));
      //      lGAdd.plotOn(lFrame1,ProjWData(lRXVar,lResidVals2D[0]),RooFit::Components(lGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(30));
      // Display fit parameters.
      //      lGAdd.paramOn(lFrame1, Format("NELU", AutoPrecision(2)), Layout(0.1, 0.4,0.9) );

    }

    TFile f15(rootFileNameFrame.c_str(),"UPDATE");

    if(lGAdd) lGAdd->Write();
    if(pFR) pFR->Write();
    
    f15.Write();
    f15.Close();
    
  /*
    // HISTOs from fitted pdf
    TH1* hh_ = lGAdd->createHistogram(nameHistoFit,lRPt,Binning(100),YVar(lRXVar,Binning(100)));
    TH1* hh1_ = lGaus1->createHistogram(nameHistoFitG1,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
    TH1* hh2_ = lGaus2->createHistogram(nameHistoFitG2,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
    TH1* hh3_ = lGaus3->createHistogram(nameHistoFitG3,lRPt,Binning(1000),YVar(lRXVar,Binning(100)));
    if(hh1_ && hh2_ && hh3_) cout << " ===>>>>> CREATED 2D HISTOGRAM FILE from fitted PDF" << hh1_ << " " << hh2_ << " " << hh3_<< endl;
  */

    cout << "====================" << endl;
    cout << "== FILL correlation coefficient histograms " << endl;
    cout << "====================" << endl;
    
    if(pFR) {
      TMatrixDSym lCov_ = pFR->correlationMatrix();
      
      lCov_.Print();
      
      for(int i=0 ; i<lCov_.GetNrows(); i++ ) {
	for(int j=0 ; j<lCov_.GetNcols(); j++ ) {
	  if(iRMS && lPar==fU1) histoCorrU1.SetBinContent(i+1,j+1,(lCov_)(i,j));
	  if(iRMS && lPar!=fU1) histoCorrU2.SetBinContent(i+1,j+1,(lCov_)(i,j));
	}
      }
      
    }

    TFile f6(fileName2DFIT.Data(),"UPDATE");
    if(iRMS && lPar==fU1) histoCorrU1.Write();
    if(iRMS && lPar!=fU1) histoCorrU2.Write();
    if(gChi2) gChi2->Write();
    f6.Write();
    f6.Close();
    
    cout << "====================" << endl;
    cout << "== FILL initial value: here 2D " << endl;
    cout << "====================" << endl;
   
    for(int i0  = range_min; i0 < range_max; i0++) { 
      myMean2D[i0-int(range_min)] = lAMean1.getVal()+lBMean1.getVal()*(i0*binSize)+lCMean1.getVal()*(i0*binSize)*(i0*binSize);;
      myMeanLarge2D[i0-int(range_min)] = lAMean2.getVal()+lBMean2.getVal()*(i0*binSize)+lCMean2.getVal()*(i0*binSize)*(i0*binSize);
      myMeanVeryLarge2D[i0-int(range_min)] = lAMean3.getVal()+lBMean3.getVal()*(i0*binSize)+lCMean3.getVal()*(i0*binSize)*(i0*binSize);
      myMean2DE[i0-int(range_min)] = 0; //lMean->getPropagatedError(*pFR);
      myMeanLarge2DE[i0-int(range_min)] = 0; //lMean1->getPropagatedError(*pFR);
      myMeanVeryLarge2DE[i0-int(range_min)] = 0; //lMean1->getPropagatedError(*pFR);

      //      cout << "++++++++" << endl;
      //      cout << "++++++++" << endl;
      //      cout << "++++++++ Mean = " << lMean->getVal() << " Mean1 = " << lMean1->getVal() << " i0=" << i0 << endl;

      //      myMean2D[i0-int(range_min)]=lA1Mean.getVal()+lA2Mean.getVal()*(i0*binSize)+lA3Mean.getVal()*(i0*binSize)*(i0*binSize);

      lY12D[i0-int(range_min)] = lA1Sig.getVal() + lB1Sig.getVal()*(i0*binSize) + lC1Sig.getVal()*(i0*binSize)*(i0*binSize);///sqrt(2*3.14159265)*2.;
      lY22D[i0-int(range_min)] = lA2Sig.getVal() + lB2Sig.getVal()*(i0*binSize) + lC2Sig.getVal()*(i0*binSize)*(i0*binSize);///sqrt(2*3.14159265)*2.;
      if(do3G) lY32D[i0-int(range_min)] = lA3Sig.getVal() + lB3Sig.getVal()*(i0*binSize) + lC3Sig.getVal()*(i0*binSize)*(i0*binSize);
      //      myFrac2D[i0-int(range_min)] = l1Frac->getval();
      myFrac2D[i0-int(range_min)] = lAFrac.getVal() + lBFrac.getVal()*(i0*binSize) + lCFrac.getVal()*(i0*binSize)*(i0*binSize);
      if(do3G) myFrac22D[i0-int(range_min)] = lA1Frac.getVal() + lB1Frac.getVal()*(i0*binSize) + lC1Frac.getVal()*(i0*binSize)*(i0*binSize);

      lEY02D[i0-int(range_min)] = 0; // to be implemented
      lEY12D[i0-int(range_min)] = 0; // to be implemented
      lEY22D[i0-int(range_min)] = 0; // to be implemented
      if(do3G) lEY32D[i0-int(range_min)] = 0; // to be implemented
      myFrac2DE[i0-int(range_min)] = 0;  // to be implemented
      if(do3G) myFrac22DE[i0-int(range_min)] = 0;  // to be implemented

    }

    TGraphErrors *lG1_2d = new TGraphErrors(lNBins,lX,lY12D,lEX,lEY12D); lG1_2d->SetMarkerStyle(kOpenSquare);
    TGraphErrors *lG2_2d = new TGraphErrors(lNBins,lX,lY22D,lEX,lEY22D); lG2_2d->SetMarkerStyle(kOpenSquare);
    TGraphErrors *lG3_2d = new TGraphErrors(lNBins,lX,lY32D,lEX,lEY32D); lG3_2d->SetMarkerStyle(kOpenSquare);
    TGraphErrors* gFrac2D = new TGraphErrors(lNBins,lX,myFrac2D,lEX,myFrac2DE); gFrac2D->SetMarkerStyle(kOpenSquare);
    TGraphErrors* gFrac22D = new TGraphErrors(lNBins,lX,myFrac22D,lEX,myFrac22DE); gFrac22D->SetMarkerStyle(kOpenSquare);
    //    TGraphErrors* gFracSecond2D = new TGraphErrors(lNBins, lX, myFracSecond2D, lEX, myFracSecond2DE);
    TGraphErrors *lM0_2d = new TGraphErrors(lNBins,lX,myMean2D,lEX,myMean2DE); lM0_2d->SetMarkerStyle(kOpenSquare); // this is the mean fitted of the two gaussian
    TGraphErrors *lM0large_2d = new TGraphErrors(lNBins,lX,myMeanLarge2D,lEX,myMeanLarge2DE); lM0large_2d->SetMarkerStyle(kOpenSquare); // this is the mean fitted of the two gaussian
    TGraphErrors *lM0Verylarge_2d = new TGraphErrors(lNBins,lX,myMeanVeryLarge2D,lEX,myMeanVeryLarge2DE); lM0Verylarge_2d->SetMarkerStyle(kOpenSquare); // this is the mean fitted of the two gaussian

    cout << "============" << endl;
    cout << "============" << endl;
    cout << "=== Unbinned Fit: done ===" << endl;
    cout << "============" << endl;
    cout << "============" << endl;

    //////
    //// cin.get();
    //////

    iC->cd();
    //  TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    //    TF1 *lFitS1  = new TF1("lFitS1",   "pol2");
    //    TFitResultPtr  lFitPtr1 = lG1->Fit(lFitS1,"SRE","", range_min , range_max); //"EXO"
    computeFitErrors(iFitS1,lFitPtr1,lFitS1,iRMS);

    lG1->GetYaxis()->SetRangeUser(0.,5.);

    lG1->Draw("ape");
    drawErrorBands(iFitS1,fZPtMax);
    lG1->Draw("pe");
    lG1_2d->Draw("pe");

    if(pFR) latexLabel.DrawLatex(0.20, 0.95, Form("status = %d", pFR->status()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.9, Form("A1Sig = %f +/- %f", lA1Sig.getVal(),lA1Sig.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.85, Form("B1Sig = %f +/- %f", lB1Sig.getVal(),lB1Sig.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.8, Form("C1Sig = %f +/- %f", lC1Sig.getVal(),lC1Sig.getError()));
    //    if(pFR) latexLabel.DrawLatex(0.25, 0.75, Form("lAFrac = %f", lAFrac.getVal()));
    //    if(pFR) latexLabel.DrawLatex(0.25, 0.7, Form("lBFrac = %f", lBFrac.getVal()));
    //    if(pFR) latexLabel.DrawLatex(0.25, 0.65, Form("lCFrac = %f", lCFrac.getVal()));


    latexLabel.DrawLatex(0.20, 0.7, leg1); // Y bins
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.7, leg3);
    //    latexLabel.DrawLatex(0.25, 0.8, "Y(V)<1");
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

    TLine *lineMin = new TLine(range_min, 0,   range_min, 5);
    TLine *lineMax = new TLine(range_max, 0.,   range_max, 5.);
    lineMin->SetLineColor(11);
    lineMax->SetLineColor(11);

    TLine *lineSS1_min = new TLine(range_min,minSigma1,range_max,minSigma1);
    TLine *lineSS1_cen = new TLine(range_min,startSigma1,range_max,startSigma1);
    TLine *lineSS1_max = new TLine(range_min,maxSigma1,range_max,maxSigma1);
    lineSS1_min->SetLineColor(11);
    lineSS1_cen->SetLineColor(13);
    lineSS1_max->SetLineColor(11);
    lineSS1_min->Draw("same");
    lineSS1_cen->Draw("same");
    lineSS1_max->Draw("same");
    lineMin->Draw("same");
    lineMax->Draw("same");

    TString pSS1="PLOTNOTE/pG1";
    pSS1 += labeling.Data();
    TString pSS1pdf=Form("%s.pdf",pSS1.Data());
    pSS1 += ".png";
    iC->SaveAs(pSS1.Data());
    iC->Print(pSS1pdf.Data());

    //////
    //// cin.get();
    //////

    iC->cd();
    //  TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    //    TF1 *lFitS2  = new TF1("lFitS2",   "pol2");
    //    TFitResultPtr  lFitPtr2 = lG2->Fit(lFitS2,"SRE","", range_min , range_max); 
    computeFitErrors(iFitS2,lFitPtr2,lFitS2,iRMS);

    lG2->GetYaxis()->SetRangeUser(0.,5.);

    lG2->Draw("ape");
    drawErrorBands(iFitS2,fZPtMax);
    lG2->Draw("pe");
    lG2_2d->Draw("pe"); 

    if(pFR) latexLabel.DrawLatex(0.20, 0.95, Form("status = %d", pFR->status()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.9, Form("A2Sig = %f +/- %f", lA2Sig.getVal(),lA2Sig.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.85, Form("B2Sig = %f +/- %f", lB2Sig.getVal(),lB2Sig.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.8, Form("C2Sig = %f +/- %f", lC2Sig.getVal(),lC2Sig.getError()));


    latexLabel.DrawLatex(0.20, 0.7, leg1); // Y bin
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.7, leg3);
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);


    TLine *lineSS2_min = new TLine(range_min, minSigma2,   range_max, minSigma2);
    TLine *lineSS2_cen = new TLine(range_min, startSigma2, range_max, startSigma2);
    TLine *lineSS2_max = new TLine(range_min, maxSigma2,   range_max, maxSigma2);
    lineSS2_min->SetLineColor(11);
    lineSS2_cen->SetLineColor(13);
    lineSS2_max->SetLineColor(11);
    lineSS2_min->Draw("same");
    lineSS2_cen->Draw("same");
    lineSS2_max->Draw("same");
    lineMin->Draw("same");
    lineMax->Draw("same");

    TString pSS2="PLOTNOTE/pG2";
    pSS2 += labeling.Data();
    TString pSS2pdf=Form("%s.pdf",pSS2.Data());
    pSS2 += ".png";
    iC->SaveAs(pSS2.Data());
    iC->Print(pSS2pdf.Data());

    //////
    //// cin.get();
    //////


    iC->cd();
    //  TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    //    TF1 *lFitS3  = new TF1("lFitS3",   "pol2");
    //    TFitResultPtr  lFitPtr3 = lG3->Fit(lFitS3,"SRE","", range_min , range_max); 
    //    cout << "iFitS3=" << iFitS3 << " lFitPtr3=" << lFitPtr3 << "lFitS3=" << lFitS3 << endl;
    computeFitErrors(iFitS3,lFitPtr3,lFitS3,iRMS);

    lG3->GetYaxis()->SetRangeUser(0.,5.);

    lG3->Draw("ape");
    drawErrorBands(iFitS3,fZPtMax);
    lG3->Draw("pe");
    lG3_2d->Draw("pe"); 

    if(pFR) latexLabel.DrawLatex(0.20, 0.95, Form("status = %d", pFR->status()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.9, Form("A3Sig = %f +/- %f", lA3Sig.getVal(),lA3Sig.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.85, Form("B3Sig = %f +/- %f", lB3Sig.getVal(),lB3Sig.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.8, Form("C3Sig = %f +/- %f", lC3Sig.getVal(),lC3Sig.getError()));


    latexLabel.DrawLatex(0.20, 0.7, leg1); // Y bin
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.7, leg3);
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

    //    if(!doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.75, "powheg");
    //    if(doMad && (!fData)) latexLabel.DrawLatex(0.25, 0.75, "madgraph");


    TLine *lineSS3_min = new TLine(range_min, minSigma3,   range_max, minSigma3);
    TLine *lineSS3_cen = new TLine(range_min, startSigma3, range_max, startSigma3);
    TLine *lineSS3_max = new TLine(range_min, maxSigma3,   range_max, maxSigma3);
    lineSS3_min->SetLineColor(11);
    lineSS3_cen->SetLineColor(13);
    lineSS3_max->SetLineColor(11);
    lineSS3_min->Draw("same");
    lineSS3_cen->Draw("same");
    lineSS3_max->Draw("same");
    lineMin->Draw("same");
    lineMax->Draw("same");

    TString pSS3="PLOTNOTE/pG3";
    pSS3 += labeling.Data();
    TString pSS3pdf=Form("%s.pdf",pSS3.Data());
    pSS3 += ".png";
    iC->SaveAs(pSS3.Data());
    iC->Print(pSS3pdf.Data());

    //////
    //// cin.get();
    //////

    TFile f9(fileName2DFIT.Data(),"UPDATE");
    if(lPar==fU1) lG1_2d->SetName("lG1_U1_2d");
    if(lPar==fU1) lG2_2d->SetName("lG2_U1_2d");
    if(lPar==fU1) lG3_2d->SetName("lG3_U1_2d");
    if(lPar!=fU1) lG1_2d->SetName("lG1_U2_2d");
    if(lPar!=fU1) lG2_2d->SetName("lG2_U2_2d");
    if(lPar!=fU1) lG3_2d->SetName("lG3_U2_2d");

    if(lPar==fU1) lM0_2d->SetName("lM0_U1_2d");
    if(lPar!=fU1) lM0_2d->SetName("lM0_U2_2d");
    if(lPar==fU1) lM0large_2d->SetName("lM0large_U1_2d");
    if(lPar!=fU1) lM0large_2d->SetName("lM0large_U2_2d");

    lM0_2d->Write();
    lM0large_2d->Write();
    lG1_2d->Write();
    lG2_2d->Write();
    lG3_2d->Write();
    //    gFrac2D->Write();
    f9.Write();
    f9.Close();

    //////
    //// cin.get();
    //////

    //    iC->cd();
    //    iC->Clear();
    //    gPad.DrawFrame(0,0,50,1);
    //    TGraph* gFrac = new TGraph(lNBins, lX, myFrac);
    //    TGraph* gFrac2D = new TGraph(lNBins, lX, myFrac2D);
    //    TGraph* gFracSecond = new TGraph(lNBins, lX, myFracSecond);
    //    TGraph* gFracSecond2D = new TGraph(lNBins, lX, myFracSecond2D);

    iC->cd();
    iC->Clear();
    computeFitErrors(iFitFrac,lFitPtrFrac,lFitFrac,iRMS);
    gFrac->Draw("ape");
    drawErrorBands(iFitFrac,fZPtMax);
    gFrac->Draw("pe");
    gFrac2D->Draw("pe");

    if(pFR) latexLabel.DrawLatex(0.20, 0.95, Form("status = %d", pFR->status()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.9, Form("AFrac = %f +/- %f", lAFrac.getVal(),lAFrac.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.85, Form("BFrac = %f +/- %f", lBFrac.getVal(),lBFrac.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.8, Form("CFrac = %f +/- %f", lCFrac.getVal(),lCFrac.getError()));

    //    gFrac->SetTitle("");
    //    gFrac->GetYaxis()->SetRangeUser(0.,1.);
    //    gFrac->SetMarkerStyle(kFullCircle);
    //    gFrac->GetYaxis()->SetTitle("#sigma_{1}");
    //    gFrac->GetXaxis()->SetTitle("Z p_{T}");

    //    gFrac->Draw("ape");
    //    drawErrorBands(iFitFrac,fZPtMax);
    //    gFrac->Draw("pe");

    /*
    gFrac->SetMarkerStyle(kOpenTriangleUp);
    gFrac->SetMarkerColor(kBlue);
    gFracSecond->SetMarkerStyle(kFullTriangleUp);
    gFracSecond->SetMarkerColor(kBlue);
    //    gFrac1D->SetMarkerColor(kRed);
    //    gFrac1D->SetMarkerStyle(kOpenCircle);
    gFrac2D->SetMarkerStyle(kOpenSquare);
    gFracSecond2D->SetMarkerStyle(kOpenSquare);
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(gFrac,"p");
    //    mg->Add(gFrac1D,"p");
    mg->Add(gFracSecond,"p");
    mg->Add(gFrac2D,"p");
    mg->Add(gFracSecond2D,"p");
    mg->Draw("A");
    //    if(lPar==fU1) mg->SetTitle("U1");
    //    if(lPar!=fU1) mg->SetTitle("U2");
    mg->SetMaximum(1);
    mg->SetMinimum(0);
    mg->GetXaxis()->SetTitle("Z p_{T}");
    mg->GetYaxis()->SetTitle("Frac #sigma_{1}");
    //    mg->GetYaxis()->SetRangeUser(0.,1.);
    gPad->Modified();
    */

    latexLabel.DrawLatex(0.20, 0.7, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.35, leg3);
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

    TLine *lineSS4_min = new TLine(range_min,minFrac,range_max,minFrac);
    TLine *lineSS4_max = new TLine(range_min,maxFrac,range_max,maxFrac);
    lineSS4_min->SetLineColor(11);
    lineSS4_max->SetLineColor(11);
    lineSS4_min->Draw("same");
    lineSS4_max->Draw("same");
    lineMin->Draw("same");
    lineMax->Draw("same");

    TString pSS4="PLOTNOTE/pGFrac";
    pSS4 += labeling.Data();
    TString pSS4pdf=Form("%s.pdf",pSS4.Data());
    pSS4 += ".png";
    gPad->SaveAs(pSS4.Data());
    gPad->Print(pSS4pdf.Data());

    ////
    ////
    ////
    ////

    iC->cd();
    iC->Clear();
    computeFitErrors(iFitFrac2,lFitPtrFrac2,lFitFrac2,iRMS);
    gFrac2->Draw("ape");
    drawErrorBands(iFitFrac2,fZPtMax);
    gFrac2->Draw("pe");
    gFrac22D->Draw("pe");

    if(pFR) latexLabel.DrawLatex(0.20, 0.95, Form("status = %d", pFR->status()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.9, Form("AFrac = %f +/- %f", lA1Frac.getVal(),lA1Frac.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.85, Form("BFrac = %f +/- %f", lB1Frac.getVal(),lB1Frac.getError()));
    if(pFR) latexLabel.DrawLatex(0.20, 0.8, Form("CFrac = %f +/- %f", lC1Frac.getVal(),lC1Frac.getError()));

    latexLabel.DrawLatex(0.20, 0.7, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.35, leg3);
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

    TLine *lineSS5_min = new TLine(range_min,minFrac,range_max,minFrac);
    TLine *lineSS5_max = new TLine(range_min,maxFrac,range_max,maxFrac);
    lineSS5_min->SetLineColor(11);
    lineSS5_max->SetLineColor(11);
    lineSS5_min->Draw("same");
    lineSS5_max->Draw("same");
    lineMin->Draw("same");
    lineMax->Draw("same");

    TString pSS5="PLOTNOTE/pGFrac2";
    pSS5 += labeling.Data();
    TString pSS5pdf=Form("%s.pdf",pSS5.Data());
    pSS5 += ".png";
    gPad->SaveAs(pSS5.Data());
    gPad->Print(pSS5pdf.Data());

    //////
    //// cin.get();
    //////
    iC->cd();
    iC->Clear();

    computeFitErrors(iMean1Fit,lFitPtrM0,lFitPullMean1,iRMS);
    lM0->Draw("ape");
    drawErrorBands(iMean1Fit,fZPtMax);
    lM0->Draw("pe");
    lM0_2d->Draw("pe");

    computeFitErrors(iMean2Fit,lFitPtrM0large,lFitPullMean2,iRMS);
    lM0large->Draw("pe");
    drawErrorBands(iMean2Fit,fZPtMax);
    lM0large->SetMarkerColor(2);
    lM0large_2d->SetMarkerColor(2);
    lM0large->Draw("pe");
    lM0large_2d->Draw("pe");

    //    computeFitErrors(iMean3Fit,lFitPtrM0Verylarge,lFitPullMean3,iRMS);
    //    lM0Verylarge->Draw("pe");
    //    drawErrorBands(iMean3Fit,fZPtMax);
    //    lM0Verylarge->SetMarkerColor(4);
    //    lM0Verylarge_2d->SetMarkerColor(4);
    //    lM0Verylarge->Draw("pe");
    //    lM0Verylarge_2d->Draw("pe");

    latexLabel.DrawLatex(0.20, 0.7, leg1);
    if(doVTXbinning) latexLabel.DrawLatex(0.20, 0.6, leg3);
    //    latexLabel.DrawLatex(0.15, 0.8, "Y(V)<1");
    latexLabel.DrawLatex(0.20, 0.75, legDATA);
    latexLabel.DrawLatex(0.20, 0.3, legU1U2);

    TString pMM0="PLOTNOTE/M0";
    pMM0 += labeling.Data();
    TString pMM0pdf=Form("%s.pdf",pMM0.Data());
    pMM0 += ".png";
    iC->SaveAs(pMM0.Data());
    iC->Print(pMM0pdf.Data());

    /*
    iC->cd();
    iC->Clear();

    lG3->SetMarkerColor(4);
    lG2->SetMarkerColor(2);
    lG1->SetMarkerColor(kMagenta);
    lG3->Draw("A P E"); 
    lG2->Draw("P E same");
    lG1->Draw("P E same");

    iC->SaveAs("TestGraph.png");
    */



  } //  END //  if(doPrintAll && !doIterativeMet)

  bool doPol1=false;

  TMatrixDSym   lCov = pFR->covarianceMatrix();

  cout << "covariance matrix " << endl;
  pFR->covarianceMatrix().Print();
  
  //Now Store each sigma of the double gaussian in a fit function from the Unbinned FIT
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
  if(!doPol1) iFitS2->SetParameter(2,lC2Sig.getVal());

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

  /////

  iFitS3->SetParameter(0,lA3Sig.getVal()); 
  iFitS3->SetParameter(1,lB3Sig.getVal()); 
  if(!doPol1) iFitS3->SetParameter(2,lC3Sig.getVal());
  double E0S3 = 0;
  double E1S3 = 0;
  double E2S3 = 0;
  double E3S3 = 0;
  double E4S3 = 0;

  //  cout << "Saving lFrac into RootFile " << l1Frac.getVal() << endl;

  iFrac->SetParameter(0,lAFrac.getVal());
  iFrac->SetParameter(1,lBFrac.getVal());
  iFrac->SetParameter(2,lCFrac.getVal());
  iFrac->SetParameter(3,lDFrac.getVal());

  iFrac2->SetParameter(0,lA1Frac.getVal());
  iFrac2->SetParameter(1,lB1Frac.getVal());
  iFrac2->SetParameter(2,lC1Frac.getVal());
  iFrac2->SetParameter(3,lD1Frac.getVal());

  iMean1Fit->SetParameter(0,lAMean1.getVal());
  iMean1Fit->SetParameter(1,lBMean1.getVal());
  iMean1Fit->SetParameter(2,lCMean1.getVal());

  iMean2Fit->SetParameter(0,lAMean2.getVal());
  iMean2Fit->SetParameter(1,lBMean2.getVal());
  iMean2Fit->SetParameter(2,lCMean2.getVal());

  cout << "fraction to save="<< iFrac->Eval(fZPt) << endl;

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
		TF1 *iMFit, TF1 *iMRMSFit, TF1 *iRMS1Fit, TF1 *iRMS2Fit,TF1 *iRMS3Fit,TF1 *iFracFit,TF1 *iFrac2Fit,TF1 *iMean1Fit,TF1 *iMean2Fit,int iType,std::string iName="") {
  TCanvas *lXC1 = new TCanvas("C1","C1",800,600); lXC1->cd(); 

  // doing SCALE
  fitGraph(iTree,fTTbarTree,lXC1,iVPar,iMFit   ,0       ,0       ,0       ,0       ,0        ,0        ,0        ,0    ,true ,false,iType,iName);
  // doing RMS
  fitGraph(iTree,fTTbarTree,lXC1,iVPar,iMRMSFit,iRMS1Fit,iRMS2Fit,iRMS3Fit,iFracFit,iFrac2Fit,iMean1Fit,iMean2Fit,iMFit,false,true ,iType,iName);

}

void writeRecoil(TF1 *iU1Fit, TF1 *iU1MRMSFit, TF1 *iU1RMS1Fit, TF1 *iU1RMS2Fit, TF1 *iU1RMS3Fit, TF1 *iU1FracFit, TF1 *iU1Frac2Fit, TF1 *U1Mean1Fit, TF1 *U1Mean2Fit,
		 TF1 *iU2Fit, TF1 *iU2MRMSFit, TF1 *iU2RMS1Fit, TF1 *iU2RMS2Fit, TF1 *iU2RMS3Fit, TF1 *iU2FracFit, TF1 *iU2Frac2Fit, TF1 *U2Mean1Fit, TF1 *U2Mean2Fit,
		 std::string iName="recoilfit.root") {
  TFile *lFile = new TFile(iName.c_str(),"UPDATE");
  assert(iU1Fit);     iU1Fit    ->Write();
  assert(iU2Fit);     iU2Fit    ->Write();
  assert(iU1MRMSFit); iU1MRMSFit->Write();
  assert(iU1RMS1Fit); iU1RMS1Fit->Write();
  assert(iU1RMS2Fit); iU1RMS2Fit->Write();
  assert(iU1RMS3Fit); iU1RMS3Fit->Write();
  assert(iU2MRMSFit); iU2MRMSFit->Write();
  assert(iU2RMS1Fit); iU2RMS1Fit->Write();
  assert(iU2RMS2Fit); iU2RMS2Fit->Write();
  assert(iU2RMS3Fit); iU2RMS3Fit->Write();
  assert(iU1FracFit); iU1FracFit->Write();
  assert(iU2FracFit); iU2FracFit->Write();
  assert(iU1Frac2Fit); iU1Frac2Fit->Write();
  assert(iU2Frac2Fit); iU2Frac2Fit->Write();
  assert(U1Mean1Fit); U1Mean1Fit->Write();
  assert(U1Mean2Fit); U1Mean2Fit->Write();
  assert(U2Mean1Fit); U2Mean1Fit->Write();
  assert(U2Mean2Fit); U2Mean2Fit->Write();

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

  if(!doBKG ) cout << "==> looping over tree" << endl;
  if(doBKG ) cout << "==> looping over BKG tree" << endl;

  int nEntries = iTree->GetEntries();
  //  if(isBKG) nEntries = iTree->GetEntries()/30;

  if(writeTree) {

    //    if(writeTree) nEntries = 1000000; // this for the SKIM
    //    if(writeTree) nEntries = 500000; // this takes little
    //    nEntries = 250000; // this takes little
    if((iTree->GetEntries()-startTreeEntries)<nEntries) nEntries = iTree->GetEntries()-startTreeEntries;
    
  }

  TFile *fout_loopOverTree = 0;
  TTree *iterEvTree = 0;

  double fMetOriginal=fMet;
  double fMPhiOriginal=fMPhi;

  if(writeTree) {

    //    fout_loopOverTree = new TFile(Form("TREE/foutIter_loopOverTree_POWasDATA_mad%d_iter%d_onlyU2%d_onlyU1%d_iParamU1%d_iParamU2%d_%d_writeTree_%d.root",doMad,doIterativeMet,doOnlyU2,doOnlyU1,iParamU1,iParamU2,startTreeEntries,writeTree),"RECREATE");
    //    fout_loopOverTree = new TFile(Form("TREE/foutIter_loopOverTree_Y%d_POWasMAD_mad%d_iter%d_onlyU2%d_onlyU1%d_iParamU1%d_iParamU2%d_%d_writeTree_%d.root",fId,doMad,doIterativeMet,doOnlyU2,doOnlyU1,iParamU1,iParamU2,startTreeEntries,writeTree),"RECREATE");
    //    fout_loopOverTree = new TFile(Form("TREE/foutIter_loopOverTree_Y%d_MADasMAD_mad%d_iter%d_onlyU2%d_onlyU1%d_iParamU1%d_iParamU2%d_%d_writeTree_%d.root",fId,doMad,doIterativeMet,doOnlyU2,doOnlyU1,iParamU1,iParamU2,startTreeEntries,writeTree),"RECREATE");
    //    fout_loopOverTree = new TFile("TREE/test.root","RECREATE");

    //    fout_loopOverTree = new TFile(Form("TREE/foutIter_loopOverTree_mad%d_iter%d_%d_writeTree_%d.root",doMad,doIterativeMet,startTreeEntries,writeTree),"RECREATE");
    fout_loopOverTree = new TFile(Form("TREE/foutIter_skimmedTree_Y%d_mad%d_doKalmanCorr%d_iter%d_%d_writeTree_%d_aug16.root",fId,doMad,doKalman_corr,doIterativeMet,startTreeEntries,writeTree),"RECREATE");

    iterEvTree = new TTree("ZTreeProducer","ZTreeProducer");
    
    TBranch* br_run = iterEvTree->Branch("run", &fRun, "fRun/I");
    TBranch* br_lumi = iterEvTree->Branch("lumi", &fLumi, "fLumi/I");
    TBranch* br_evt = iterEvTree->Branch("evt", &fEvent, "fEvent/I");
    TBranch* br_nvtx = iterEvTree->Branch("nvtx", &fNPV, "fNPV/I");

    TBranch* br_fZPt = iterEvTree->Branch("Z_pt", &fZrecoPt, "fZPt/D");
    TBranch* br_fZPhi = iterEvTree->Branch("Z_phi", &fZrecoPhi, "fZPhi/D");
    TBranch* br_fZRap = iterEvTree->Branch("Z_rap", &fZrecoRap, "fZRap/D");
    
    TBranch* br_fPt1 = iterEvTree->Branch("Pt1", &fPt1, "fPt1/D");
    TBranch* br_fPhi1 = iterEvTree->Branch("Phi1", &fPhi1, "fPhi1/D");
    //  TBranch* br_fMet = iterEvTree->Branch("Met", &fMet, "fMet/D");
    //  TBranch* br_fMPhi = iterEvTree->Branch("MPhi", &fMPhi, "fMPhi/D");
    TBranch* br_pt_vis = iterEvTree->Branch("pt_vis", &fPt1, "pt_vis/D");
    TBranch* br_phi_vis = iterEvTree->Branch("phi_vis", &fPhi1, "phi_vis/D");

    TBranch* br_fZGenPt = iterEvTree->Branch("ZGen_pt" ,&fZPt, "fZPt/D");
    TBranch* br_fZGenPhi = iterEvTree->Branch("ZGen_phi" ,&fZPhi, "fZPhi/D");
    TBranch* br_fZGenRap = iterEvTree->Branch("ZGen_rap" ,&fZRap, "fZRap/D");
    
    TBranch* br_tkmet = iterEvTree->Branch("tkmet",&fMet,"tkmet/D");   /// this will be replaced after the smearing
    TBranch* br_tkmetphi = iterEvTree->Branch("tkmet_phi",&fMPhi,"tkmet_phi/D");  /// this will be replaced after the smearing
    TBranch* br_tkmetsumEt = iterEvTree->Branch("tkmet_sumEt",&ftkMSumET,"tkmet_sumEt/D");

    TBranch* br_tkmet_ori = iterEvTree->Branch("tkmet_ori",&fMetOriginal,"tkmet_ori/D");   /// this will be replaced after the smearing
    TBranch* br_tkmetphi_ori = iterEvTree->Branch("tkmet_phi_ori",&fMPhiOriginal,"tkmet_phi_ori/D");  /// this will be replaced after the smearing
    
    //    TBranch* br_tkmet = iterEvTree->Branch("tkmet",&ftkMet,"tkmet/D");   /// this will be replaced after the smearing
    //    TBranch* br_tkmetphi = iterEvTree->Branch("tkmet_phi",&ftkMPhi,"tkmet_phi/D");  /// this will be replaced after the smearing
    //    TBranch* br_tkmetsumEt = iterEvTree->Branch("tkmet_sumEt",&ftkMSumET,"tkmet_sumEt/D");
    
    TBranch* br_Z_mass = iterEvTree->Branch("Z_mass" ,&fZmass, "Z_mass/D");
    TBranch* br_MuPos_eta = iterEvTree->Branch("MuPos_eta" ,&fMuPos_eta, "MuPos_eta/D");
    TBranch* br_MuPos_pt = iterEvTree->Branch("MuPos_pt" ,&fMuPos_pt,"MuPos_pt/D");
    TBranch* br_MuPos_phi = iterEvTree->Branch("MuPos_phi" ,&fMuPos_phi,"MuPos_phi/D");
    TBranch* br_MuPos_mass = iterEvTree->Branch("MuPos_mass" ,&fMuPos_mass,"MuPos_mass/D" );
    TBranch* br_MuPos_charge = iterEvTree->Branch("MuPos_charge" ,&fMuPos_charge,"MuPos_charge/D" );
    TBranch* br_MuPosRelIso = iterEvTree->Branch("MuPosRelIso" ,&fMuPosReliso,"MuPosRelIso/D");
    TBranch* br_MuPos_dxy = iterEvTree->Branch("MuPos_dxy" ,&fMuPos_dxy, "MuPos_dxy/D");
    
    TBranch* br_MuNeg_eta = iterEvTree->Branch("MuNeg_eta" ,&fMuNeg_eta,"MuNeg_eta/D");
    TBranch* br_MuNeg_pt = iterEvTree->Branch("MuNeg_pt" ,&fMuNeg_pt,"MuNeg_pt/D");
    TBranch* br_MuNeg_phi = iterEvTree->Branch("MuNeg_phi" ,&fMuNeg_phi,"MuNeg_phi/D");
    TBranch* br_MuNeg_mass = iterEvTree->Branch("MuNeg_mass" ,&fMuNeg_mass, "MuNeg_mass/D");
    TBranch* br_MuNeg_charge = iterEvTree->Branch("MuNeg_charge" ,&fMuNeg_charge,"MuNeg_charge/D");
    TBranch* br_MuNegRelIso = iterEvTree->Branch("MuNegRelIso" ,&fMuNegReliso, "MuNegRelIso/D");
    TBranch* br_MuNeg_dxy = iterEvTree->Branch("MuNeg_dxy" ,&fMuNeg_dxy, "MuNeg_dxy/D");
    
    TBranch* br_MuPosTrg = iterEvTree->Branch("MuPosTrg" ,&fMuPosTrg, "MuPosTrg/I");
    TBranch* br_MuNegTrg = iterEvTree->Branch("MuNegTrg" ,&fMuNegTrg, "MuNegTrg/I");
    
    TBranch* br_MuPosIsTight = iterEvTree->Branch("MuPosIsTight" ,&fMuPosIsTight, "MuPosIsTight/I");
    TBranch* br_MuNegIsTight = iterEvTree->Branch("MuNegIsTight" ,&fMuNegIsTight, "MuNegIsTight/I");
    
    TBranch* br_evtHasGoodVtx = iterEvTree->Branch("evtHasGoodVtx" ,&fevtHasGoodVtx, "evtHasGoodVtx/I");
    TBranch* br_evtHasTrg = iterEvTree->Branch("evtHasTrg" ,&fevtHasTrg, "evtHasTrg/I");
    
  }

  for(int i1 = startTreeEntries; i1 < startTreeEntries + nEntries; i1++) {

  //  for(int i1 = 0; i1 <  1000000; i1++) {
  //  for(int i1 = 0; i1 <  10000; i1++) {
  //  for(int i1 = 0; i1 <  10000000; i1++) {
  //  for(int i1 = 0; i1 <  500000; i1++) {
  //  for(int i1 = 0; i1 <  100; i1++) {
    //  for(int i1 = 0; i1 <  1e6; i1++) {
    iTree->GetEntry(i1);

    //    if(i1%10000==0) cout <<"Analyzed entry "<< i1 <<"/"<< nEntries << endl;
    if(i1%100000==0) cout <<"Analyzed entry "<< i1 <<"/"<< nEntries << endl;
    //    if(i1%1==0) cout <<"Analyzed entry "<< i1 <<"/"<< nEntries << endl;
    if(doIterativeMet && i1%10000==0) cout <<"Analyzed entry "<< i1 << " out of nEntries=" << nEntries << " started entry "<< startTreeEntries << " (totalEntries= "<< iTree->GetEntries() << ")" << endl;

    if(doHalfStat && i1%2 && fData) continue;
    if(doRunA && fRun>175832) continue;
    if(doRunB && fRun<175832) continue;


    //    cout << "------------------------------------" << endl;
    //    cout << "Zpt as read " << fZPt << " reco" << fZrecoPt << endl;

    //    cout << " -------------------------------- " << endl;
    if(dodebug)       cout << " -------------------------------- " << endl;
    if(dodebug)       cout << "RUN " << fRun << " LUMI " << fLumi << " EVENT " << fEvent << endl;

    //    if(doGigiRescaling) applyGigiRescaling();


    double ftkMSumET_original=ftkMSumET;
    // undo the muon before the Kalman corrections
    if(doGenTkMet || doGenPfMet) {
      ftkMSumET = ftkMSumET - fMuPosGen_pt - fMuNegGen_pt; // want to check the sumEt of the recoil
    } else {
      ftkMSumET = ftkMSumET - fMuPos_pt - fMuNeg_pt; // want to check the sumEt of the recoil
    }

    if(doKalman_corr) {

      //      cout << "========================================== " << endl;
      //      cout << "before correction Mass= " << fZmass << " Pt= " << fZPt << endl;
      //      cout << "before correction pos " << fMuPos_pt << " neg " << fMuNeg_pt << endl;
      TLorentzVector muonPos, muonNeg, Zorg, muonPosCorr, muonNegCorr,Zcorr;

      //      cout << "fMuNeg_pt " << fMuNeg_pt << " fMuNeg_eta " << fMuNeg_eta << " fMuNeg_phi " << fMuNeg_phi << endl;
      //      cout << "fMuPos_pt " << fMuPos_pt << " fMuPos_eta " << fMuPos_eta << " fMuPos_phi " << fMuPos_phi << endl;

      muonNeg.SetPtEtaPhiM( fMuNeg_pt, fMuNeg_eta, fMuNeg_phi, 0);
      muonPos.SetPtEtaPhiM( fMuPos_pt, fMuPos_eta, fMuPos_phi, 0);
      Zorg=muonPos+muonNeg;

      getPtMuonCorrrection(muonNeg , fMuNeg_charge, fData);
      getPtMuonCorrrection(muonPos , fMuPos_charge, fData);

      Zcorr = muonNeg + muonPos;

      /////

      //----------------------------
      //those things below are used for the selection of events
      // rescale the relIso
      fMuPosReliso = fMuPosReliso*(fMuPos_pt/muonPos.Pt());
      fMuNegReliso = fMuNegReliso*(fMuNeg_pt/muonNeg.Pt());
      // adjust the pt
      fMuPos_pt = muonPos.Pt();
      fMuNeg_pt = muonNeg.Pt();
      // adjust the Zmass
      fZmass = Zcorr.M();

      fZrecoPt = Zcorr.Pt();
      fZrecoPhi = Zcorr.Phi();
      fZrecoRap = Zcorr.Rapidity();

      // cout << "fZmass " << fZmass << endl;

      //----------------------------
      // those only on DATA since for the MC are used the Gen
      if(fData) fZPt = Zcorr.Pt();
      if(fData) fZPhi = Zcorr.Phi();
      if(fData) fZRap = Zcorr.Rapidity();

    }

    /*
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
    */

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

    // Need to cut away the genStuff at -99
    if(fZPt<0) continue;

    //    cout << "RUN " << fRun << " LUMI " << fLumi << " EVENT " << fEvent << endl;

    ///$$$$ ---> in case is needed to get the Reco and not Gen
    if(doRecoParam) {
      if(!fData) fZPt = fZrecoPt;
      if(!fData) fZPhi = fZrecoPhi;
      if(!fData) fZRap = fZrecoRap;
    }

    // Fit only the good range
    if(fZPt > fZPtMax) continue;

    //    cout << "RUN " << fRun << " LUMI " << fLumi << " EVENT " << fEvent << endl;
    //    cout << " fZpt=" << fZPt << endl;

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

    fMetOriginal=fMet;
    fMPhiOriginal=fMPhi;

    // THIS WAS FOR THE VTX selection
    if(doVTXbinning && !passId(VTXbin)) continue;
    //    if(doVTXbinning && !passId(fId)) continue;
    //    if(doVTXbinning && !passId(fId)) continue;

    //    ?? MARIA BKG ??
    if((!calculateBosonRap(fId))) continue; 
    //    if(!calculateBosonRap(1)) continue;  // look the bin with 0<y<1 

    //////
    //////

    //// Inside the Z and W selection there are MET releated selection
    //// not apply at the first round when doing Iterative

    bool doMetCut=true;
    if (doIterativeMet) doMetCut=false;

    // this is Z selection
    if((!doPosW) && (!doNegW) && (!runSelection(doMetCut)))  continue;
    // this is for the W selection
    if(doPosW && (!runWSelection(true,doMetCut))) continue;
    if(doNegW && (!runWSelection(false,doMetCut))) continue;

    if(pType!=-1) {
      ////    cout << "passed selection " << endl;
      if((!doPosW) && (!doNegW) && !checkPDF(23,true)) continue; 
      if((doPosW) && !checkPDF(24,true)) continue; 
      if((doNegW) && !checkPDF(-24,true)) continue; 
    }

    /////
    /////

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

      //      if(dodebug)      cout << "fEvent" << fEvent << " original MET " << fMet << " phi " << fMPhi << endl;
      //      cout << "fEvent" << fEvent << " original MET " << fMet << " phi " << fMPhi << " fZPt "<< fZPt << endl;

      if(!doApplyCorr) {

	bool doOldMethod=false;
	if(doOldMethod) {
	  // this is good for the inclusive bins and for the binned one too since take the first only 
	  applyType2CorrMET(fMet,fMPhi,
			    fZPt,fZPhi,
			    fPt1,fPhi1,
			    lZMU1Fit[0],
			    lZDU1Fit[0], lZMU1Fit[0], // SCALE
			    lZDU1RMSSMFit[0], lZMU1RMSSMFit[0], // RMS
			    lZDU1RMS1Fit[0], lZMU1RMS1Fit[0],
			    lZDU1RMS2Fit[0], lZMU1RMS2Fit[0],
			    lZDU1RMS3Fit[0], lZMU1RMS3Fit[0],
			    lZDU1FracFit[0], lZMU1FracFit[0],
			    lZDU1Mean1Fit[0], lZMU1Mean1Fit[0],
			    lZDU1Mean2Fit[0], lZMU1Mean2Fit[0],
			    //
			    lZDU2RMSSMFit[0], lZMU2RMSSMFit[0], // RMS 
			    lZDU2RMS1Fit[0], lZMU2RMS1Fit[0],
			    lZDU2RMS2Fit[0], lZMU2RMS2Fit[0],
			    lZDU2RMS3Fit[0], lZMU2RMS3Fit[0],
			    lZDU2FracFit[0], lZMU2FracFit[0],
			    lZDU2Mean1Fit[0], lZMU2Mean1Fit[0],
			    lZDU2Mean2Fit[0], lZMU2Mean2Fit[0]
			    );
	} else {

	  applyTriGausInv(fMet,fMPhi,
			  fZPt,fZPhi,
			  fPt1,fPhi1,
			  lZMU1Fit[0],
			  lZMU1Fit[0], lZDU1Fit[0], // SCALE
			  lZMU1RMSSMFit[0], lZDU1RMSSMFit[0], // RMS U1
			  lZMU2RMSSMFit[0], lZDU2RMSSMFit[0], // RMS U2
			  lpdfMCU1, lwMCU1, lpdfDATAU1, lwDATAU1,
			  lpdfMCU2, lwMCU2, lpdfDATAU2, lwDATAU2
			  );

	} // end if old methods
      } // end if !doApplyCorr
       
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

    }

    //    cout << "fEvent" << fEvent << " MET after " << fMet << " phi " << fMPhi << endl;;

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


    /*
    // for testsing
    cout << "before multiply fEvent" << fEvent << " MET after smearing " << fMet << " phi " << fMPhi << endl;

    fMet = fMet * 0.5;

    cout << "after multiply fEvent" << fEvent << " MET after smearing " << fMet << " phi " << fMPhi << endl;
    */

    if(dodebug)       cout << "fEvent" << fEvent << " MET after smearing " << fMet << " phi " << fMPhi << endl; 
    //    if(dodebug)       cout << " ------------------ " << endl;


    if (doIterativeMet) {

      doMetCut=true;
      
      // this is Z selection
      if((!doPosW) && (!doNegW) && (!runSelection(doMetCut)))  continue;
      // this is for the W selection
      if(doPosW && (!runWSelection(true,doMetCut))) continue;
      if(doNegW && (!runWSelection(false,doMetCut))) continue;

    }


    if(dodebug)       cout << " MET after smearing " << fMet << " phi " << fMPhi << endl; 
    //    if(dodebug)       cout << " ------------------ " << endl;

    if(fMet > fMetMax) continue;

    if(writeTree) iterEvTree->Fill();

    //    cout << " ftkMSumET " << ftkMSumET << endl;
    histoSumEtY1.Fill(ftkMSumET);
    histoSumEtY2.Fill(ftkMSumET);

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
        std::vector<std::vector<double> > dummy;
        lXVals_all.push_back(dummy); lXEVals_all.push_back(dummy); lYVals_all.push_back(dummy); lYTVals_all.push_back(dummy); lYEVals_all.push_back(dummy);
        lXVals_3S.push_back(dummy); lYVals_3S.push_back(dummy);      

      for(int iMeanRMS=0; iMeanRMS<2; iMeanRMS++){
	std::vector<double> dummy1;
	lXVals_all.at(iU1U2).push_back(dummy1); lXEVals_all.at(iU1U2).push_back(dummy1); 
	lYVals_all.at(iU1U2).push_back(dummy1); lYTVals_all.at(iU1U2).push_back(dummy1); lYEVals_all.at(iU1U2).push_back(dummy1);
	lXVals_3S.at(iU1U2).push_back(dummy1);
	lYVals_3S.at(iU1U2).push_back(dummy1);        

        bool isU1=U1U2truefalse[iU1U2];
	//	bool iRMS=MeanRMStruefalse[iMeanRMS];
        
	if(doLepProjAbsolute) {
	  calculateU1U2lep(lPar,isU1);
	} else {
	  calculateU1U2(lPar,isU1);
	}

        double pVal = lPar;    
        // if(iRMS && iMeanFit != 0)   pVal = (lPar - iMeanFit->Eval(fZPt));
        if(fWeight == 0) continue;

	double PhiStar=calculatePhiStar(fMuPos_eta, fMuNeg_eta, fMuPos_phi, fMuNeg_phi);
	if(doPhiStar) lXVals_all.at(iU1U2).at(iMeanRMS).push_back(PhiStar);
	else lXVals_all.at(iU1U2).at(iMeanRMS).push_back(fZPt);

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

  vlXVals_all=new double**[2]; //creates a new array of pofloaters to float objects
  vlXEVals_all=new double**[2]; //creates a new array of pofloaters to float objects
  vlYVals_all=new double**[2]; //creates a new array of pofloaters to float objects
  vlYTVals_all=new double**[2]; //creates a new array of pofloaters to float objects
  vlYEVals_all=new double**[2]; //creates a new array of pofloaters to float objects

  vlXVals_3S=new double**[2]; //creates a new array of pofloaters to float objects  
  vlYVals_3S=new double**[2]; //creates a new array of pofloaters to float objects

  for(int iU1U2=0; iU1U2<2; ++iU1U2){
    vlXVals_all[iU1U2]=new double*[2];
    vlXEVals_all[iU1U2]=new double*[2];
    vlYVals_all[iU1U2]=new double*[2];
    vlYTVals_all[iU1U2]=new double*[2];
    vlYEVals_all[iU1U2]=new double*[2];

    vlXVals_3S[iU1U2]=new double*[2];
    vlYVals_3S[iU1U2]=new double*[2];

      for(int iMeanRMS=0; iMeanRMS<2; ++iMeanRMS){
        vlXVals_all[iU1U2][iMeanRMS]=new double[lXVals_all.at(0).at(0).size()];
        vlXEVals_all[iU1U2][iMeanRMS]=new double[lXVals_all.at(0).at(0).size()];
        vlYVals_all[iU1U2][iMeanRMS]=new double[lXVals_all.at(0).at(0).size()];
        vlYTVals_all[iU1U2][iMeanRMS]=new double[lXVals_all.at(0).at(0).size()];
        vlYEVals_all[iU1U2][iMeanRMS]=new double[lXVals_all.at(0).at(0).size()];
        for(unsigned int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
          vlXVals_all[iU1U2][iMeanRMS][iev]  = lXVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlXVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlXEVals_all[iU1U2][iMeanRMS][iev] = lXEVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlXEVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYVals_all[iU1U2][iMeanRMS][iev]  = lYVals_all.at(iU1U2).at(iU1U2).at(iev);
	  //          if(!doIterativeMet) cout << "vlYVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lYVals_all.at(iU1U2).at(iU1U2).at(iev) << " Zpt=" << lXVals_all.at(iU1U2).at(iU1U2).at(iev) << endl;
	  //          if(doIterativeMet) cout << "(iter)vlYVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lYVals_all.at(iU1U2).at(iU1U2).at(iev) << " Zpt=" << lXVals_all.at(iU1U2).at(iU1U2).at(iev) <<endl;
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

  if( writeTree ) iterEvTree->Write();
  if( writeTree ) fout_loopOverTree->Close();

}

void fitRecoilMET(TTree *iTree,std::string iName,int type, int lfId) { 
  std::string lPrefix="PF"; if(type == 1) lPrefix = "TK";
  load(iTree,type);

  loopOverTree(iTree);
  //  fU2=fU2*(-1);

  if(writeTree) return;

  std::stringstream PUstring;
  PUstring << lfId;

  //////////
  /// Those need to be at least pol4/pol6 to store the results 
  /// of the ComputerErrors for pol2/pol3 fits
  /// Made as pol10 to be safe 

  TF1 *lU1Fit=0;
  if(!useErfPol2ScaleU1 && !useSubRanges) lU1Fit= new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "pol10");
  if(!useErfPol2ScaleU1 && useSubRanges) lU1Fit= new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "pol3(0)+pol3(4)");
  //  if(useErfPol2ScaleU1 && !fData && !doIterativeMet) lU1Fit= new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "(TMath::Erf(x*[0])-TMath::Erf(x*[1]))*pol12(2)");
  if(useErfPol2ScaleU1 && (fData || doIterativeMet || !fData))  lU1Fit= new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "TMath::Erf(x*[0])*pol12(1)"); 
  //  TF1 *lU1Fit     = new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1MRMSFit = new TF1((lPrefix+"u1MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU1RMS1Fit = new TF1((lPrefix+"u1RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1RMS2Fit = new TF1((lPrefix+"u1RMS2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1RMS3Fit = new TF1((lPrefix+"u1RMS3_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1FracFit = new TF1((lPrefix+"u1Frac_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1Frac2Fit = new TF1((lPrefix+"u1Frac2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1Mean1Fit= new TF1((lPrefix+"u1Mean1_"+PUstring.str()).c_str(),   "pol10");  
  TF1 *lU1Mean2Fit= new TF1((lPrefix+"u1Mean2_"+PUstring.str()).c_str(),   "pol10");
  //
  TF1 *lU2Fit     = new TF1((lPrefix+"u2Mean_"+PUstring.str()).c_str(),   "pol10");  
  TF1 *lU2MRMSFit = new TF1((lPrefix+"u2MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU2RMS1Fit = new TF1((lPrefix+"u2RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2RMS2Fit = new TF1((lPrefix+"u2RMS2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2RMS3Fit = new TF1((lPrefix+"u2RMS3_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2FracFit = new TF1((lPrefix+"u2Frac_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2Frac2Fit = new TF1((lPrefix+"u2Frac2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2Mean1Fit= new TF1((lPrefix+"u2Mean1_"+PUstring.str()).c_str(),   "pol10");  
  TF1 *lU2Mean2Fit= new TF1((lPrefix+"u2Mean2_"+PUstring.str()).c_str(),   "pol10");

  //  cout << "doing U1 " << endl;
  fitRecoil1(iTree,fU1,lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU1RMS3Fit,lU1FracFit,lU1Frac2Fit,lU1Mean1Fit,lU1Mean2Fit,type,iName);
  //  cout << "doing U2 " << endl;
  fitRecoil1(iTree,fU2,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,lU2RMS3Fit,lU2FracFit,lU2Frac2Fit,lU2Mean1Fit,lU2Mean2Fit,type,iName);

  writeRecoil(lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU1RMS3Fit,lU1FracFit,lU1Frac2Fit,lU1Mean1Fit,lU1Mean2Fit/**/,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,lU2RMS3Fit,lU2FracFit,lU2Frac2Fit,lU2Mean1Fit,lU2Mean2Fit/**/,iName);

}

/*

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
  TF1 *lU1RMS3Fit = new TF1((lPrefix+"u1RMS3_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU1FracFit = new TF1((lPrefix+"u1Frac_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2Fit     = new TF1((lPrefix+"u2Mean_"+PUstring.str()).c_str(),   "pol10");  
  TF1 *lU2MRMSFit = new TF1((lPrefix+"u2MeanRMS_"+PUstring.str()).c_str(),"pol10");
  TF1 *lU2RMS1Fit = new TF1((lPrefix+"u2RMS1_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2RMS2Fit = new TF1((lPrefix+"u2RMS2_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2RMS3Fit = new TF1((lPrefix+"u2RMS3_"+PUstring.str()).c_str(),   "pol10");
  TF1 *lU2FracFit = new TF1((lPrefix+"u2Frac_"+PUstring.str()).c_str(),   "pol10");

  //  cout << "doing U1 " << endl;
  fitRecoil1(iTree,fU1,lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU1RMS3Fit,lU1FracFit,type,iName);
  //  cout << "doing U2 " << endl;
  fitRecoil1(iTree,fU2,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,lU2RMS3Fit,lU2FracFit,type,iName);

  writeRecoil(lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU1RMS3Fit,lU1FracFit,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,lU2RMS3Fit,lU2FracFit,iName);

}
*/


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

/*
void fitRecoilMIX(TTree *iTree,TTree *iTreeBKG,std::string iName,int ifId) { 
  fitRecoilMETMIX(iTree,iTreeBKG,iName,2,ifId);
  //  fitRecoilMET(iTree,iName,0);
  //  fitRecoilMET(iTree,iName,1); 
  //  makeCorr(iTree,iName);
}
*/

//void main(int MCtype, int iloop, int processType) { 
//void runRecoilFit(int MCtype, int iloop, int processType, int myVTXbin) { 
void runRecoilFit3G(int MCtype, int iloop, int processType, bool doMadCFG=true, int startEntries=0, int setParamU1=-1, int setParamU2=-1) {
//void runRecoilFit(int MCtype, int iloop) { 
  //  Prep();
  //  VTXbin=myVTXbin;

  iParamU1=setParamU1;
  iParamU2=setParamU2;

  doMad = doMadCFG;
  /*
  gSystem->Load("../../../AnalysisCode/rochcor_44X_v3.C++");

  cout << "Loaded Library rochcor_44X "<< endl;
  muoncor44X = new rochcor_44X_v3();
  */

  bool thisISDATA=true;
  if(MCtype!=2)  thisISDATA=false;
  if(doKalman_corr) setUpMuonCorrection(thisISDATA);
 
  startTreeEntries = startEntries;

  gStyle->SetOptFit(111111);

  //  TString name="recoilfits/recoilfit_DEC19mcvsMC";
  //  TString name="recoilfits/recoilfit_JAN8test";
  //  TString name="recoilfits/recoilfit_JAN22_MADtoDATA";
  //  TString name="recoilfits/recoilfit_JAN22_MADtoMAD";
  //  TString name="recoilfits/recoilfit_JAN22_POWtoMAD";
  //  TString name="recoilfits/recoilfit_JAN28";
  TString name="recoilfits/recoilfit_AUG16";
  if(do8TeV) name +="_8TeV";
  if(doABC) name +="_ABC";

  /// SETTING
  /// MARIA Zpt TEST RANGE
  //  fNJetSelect = -1; fMetMax = 500; fZPtMin = 4; fZPtMax = 50; 
  //  fNJetSelect = -1; fMetMax = 500; fZPtMin = 0; fZPtMax = 21; 
  //  fNJetSelect = -1; fMetMax = 500; fZPtMin = 1; fZPtMax = 20;
  //  fNJetSelect = -1; fMetMax = 500; fZPtMin = 1; fZPtMax = 30;
  fNJetSelect = -1; fMetMax = 500; fZPtMin = 1; fZPtMax = 50;
  ///// process Type is used inside the checkPDF
  pType=processType;
  ///// fId is used inside the rapidity bin
  fId = iloop; 
  cout << " ==> looking at the production process " << pType << " rapidity Bin " << fId << endl;

  if(MCtype==1) {

    if(doMad) cout << "PROCESSING DY MC MADGRAPH -- inclusive Nvertex -- charged only " << endl;
    if(!doMad) cout << "PROCESSING DY MC POWHEG -- inclusive Nvertex -- charged only " << endl;

    // this is the 8 TeV (contains the other MET variables) 
    if(doMad && do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_gentkmet/ZTreeProducer_tree.root");

    // 44X
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");
    // 53X

    //    if(doMad && !do8TeV )  fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2015_05_24_53X_sumEtFIX/DYLL/ZTreeProducer_tree_SignalRecoSkimmed.root");
    //    if(!doMad && !do8TeV ) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2015_05_24_53X_sumEtFIX/DYMM/ZTreeProducer_tree_SignalRecoSkimmed.root");

    if(!do8TeV && !doClosure)  {
      /*HERE THE RAW muon*/
      //      fDataFile = TFile::Open(Form("TREE/output_skimmedTree_mad%d_Y%d_iter0.root",doMad,fId));
      //      cout << " reading TREE/output_skimmedTree_mad" << doMad << "_Y" << fId << "_iter0.root" << endl;
      /**/
      fDataFile = TFile::Open(Form("TREE/skimmedTree_Y%d_mad%d_doKalmanCorr1_iter0_writeTree_1_aug16.root",fId,doMad));
      cout << " reading TREE/skimmedTree_Y" << fId << "_mad" << doMad << "_doKalmanCorr1_iter0_writeTree_1.root" << endl;
    }

    /*
    if(doClosure && !do8TeV)   {
      fDataFile = TFile::Open(Form("TREE/output_mad%d_POWasMAD_onlyU20_onlyU10_iter1_feb8tris.root",doMad)); // closure MAD as DATA
      cout << " reading TREE/output_mad" << doMad << "_POWasMAD_onlyU20_onlyU10_iter1_feb8tris.root"<< endl;
    }
    */

    if(doClosure && !do8TeV)   {
      /*
      //      fDataFile = TFile::Open(Form("TREE/output_mad%d_POWasMAD_onlyU20_onlyU10_iParamU1%d_iParamU2%d_iter1_march26.root",doMad,iParamU1,iParamU2)); // closure vs DATA
      fDataFile = TFile::Open(Form("TREE/output_Y%d_mad%d_POWasMAD_onlyU20_onlyU10_iParamU1%d_iParamU2%d_iter1_apr13.root",fId,doMad,iParamU1,iParamU2)); // closure vs DATA
      cout << " reading TREE/output_mad" << doMad << "_POWasMAD_onlyU20_onlyU10_iParamU1" << iParamU1 << "_iParamU2" << iParamU2 << "_iter1_apr13.root"<< endl;
      //      fDataFile = TFile::Open(Form("TREE/output_mad%d_POWasMAD_onlyU20_onlyU10_iter1_feb16.root",doMad)); // closure vs DATA
      //      cout << " reading TREE/output_mad" << doMad << "_POWasMAD_onlyU20_onlyU10_iter1_feb16.root"<< endl;
      */

      if(!doMad) fDataFile = TFile::Open(Form("TREE/output_Y%d_mad%d_POWasMAD_onlyU20_onlyU10_iParamU1%d_iParamU2%d_iter1_may2.root",fId,doMad,iParamU1,iParamU2)); // closure POW vs MAD
      if(!doMad) cout << " reading TREE/output_mad" << doMad << "_POWasMAD_onlyU20_onlyU10_iParamU1" << iParamU1 << "_iParamU2" << iParamU2 << "_iter1_may2.root"<< endl;

      if(doMad) fDataFile = TFile::Open(Form("TREE/output_Y%d_mad%d_MADasMAD_onlyU20_onlyU10_iParamU1%d_iParamU2%d_iter1_may2.root",fId,doMad,iParamU1,iParamU2)); // closure MAD vs MAD
      if(doMad) cout << " reading TREE/output_mad" << doMad << "_MADasMAD_onlyU20_onlyU10_iParamU1" << iParamU1 << "_iParamU2" << iParamU2 << "_iter1_may2.root"<< endl;

    }

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");

    fData = false; 

    if(!fData) name+="_genZ";

    if(invGraph && doIterativeMet) name+="_invGraph";

    if(doIterativeMet) {

      TStopwatch t; 
      t.Start(); 

      isMC=true;
      ////// DATA closure
      if(!doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU1RMS3Fit,lZMU1FracFit, lZMU1Mean1Fit, lZMU1Mean2Fit,
			    lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU2RMS3Fit,lZMU2FracFit,lZMU2Mean1Fit, lZMU2Mean2Fit,
			    lpdfMCU1, lwMCU1, lpdfMCU2, lwMCU2,
			    ////			    "recoilfits/recoilfit_JAN25_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root" ,"PF",fId);
			    //			    "../../recoilfit_MARCH25_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root" ,"PF",fId);
			    //			    "/afs/cern.ch/user/d/dalfonso/public/recoilfit_APR13_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root" ,"PF",fId);
			    "../../recoilfit_AUG16_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root" ,"PF",fId);

      if(doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU1RMS3Fit,lZMU1FracFit,lZMU1Mean1Fit, lZMU1Mean2Fit,
			   lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU2RMS3Fit,lZMU2FracFit,lZMU2Mean1Fit, lZMU2Mean2Fit,
			   lpdfMCU1, lwMCU1, lpdfMCU2, lwMCU2,
			   //			   "recoilfits/recoilfit_JAN25_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
			   //			   "../../recoilfit_MARCH25_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
			   //			   "/afs/cern.ch/user/d/dalfonso/public/recoilfit_APR13_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
			   "../../recoilfit_AUG16_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);

      isMC=false;
  
      /*
      ////// DATA closure
      readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,lZDU1Mean1Fit, lZDU1Mean2Fit,
		 lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,lZDU2Mean1Fit, lZDU2Mean2Fit,
		 lpdfDATAU1, lwDATAU1, lpdfDATAU2, lwDATAU2,
		 //		 "recoilfits/recoilfit_JAN25_DATA_bkg_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X.root" ,"PF",fId);
		 //		 "recoilfits/recoilfit_JAN25_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X.root" ,"PF",fId);
		 "../../recoilfit_MARCH25_DATA_tkmet_eta21_MZ81101_pol4_type2_doubleGauss_triGauss_halfStat_UNBINNED_3G_53X.root" ,"PF",fId); 
      */

      // MADGRAPH as DATA closure
      readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,lZDU1Mean1Fit, lZDU1Mean2Fit,
		 lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,lZDU2Mean1Fit, lZDU2Mean2Fit,
		 lpdfDATAU1, lwDATAU1, lpdfDATAU2, lwDATAU2,
		 //		 "recoilfits/recoilfit_JAN25_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
		 //		 "../../recoilfit_MARCH25_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
		 //		 "/afs/cern.ch/user/d/dalfonso/public/recoilfit_APR13_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
		 "../../recoilfit_AUG16_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
      /*
      // POWHEG as DATA closure
      if(!doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,lZDU1Mean1Fit, lZDU1Mean2Fit,
			    lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,lZDU2Mean1Fit, lZDU2Mean2Fit,
			    lpdfDATAU1, lwDATAU1, lpdfDATAU2, lwDATAU2,
			    //			    "recoilfits/recoilfit_JAN25_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_powheg.root" ,"PF",fId);
			    "../../recoilfit_MARCH25_genZ_tkmet_eta21_MZ81101_PDF-1_pol4_type2_doubleGauss_triGauss_x2Stat_UNBINNED_3G_53X_madgraph.root" ,"PF",fId);
      */

      /*
      cout << "_____________ PRINT pointers MC  ________________" << endl;
      cout << " lpdfMCU1 " << lpdfMCU1 << " lwMCU1 " << lwMCU1 << " lpdfMCU2 " <<  lpdfMCU2 << " lwMCU2 "  << lwMCU2  << endl;
      cout << "_____________ PRINT pointers DATA ________________" << endl;
      cout << " lpdfDATAU1 " << lpdfDATAU1 << " lwDATAU1 " << lwDATAU1 << " lpdfDATAU2 " <<  lpdfDATAU2 << " lwDATAU2 "  << lwDATAU2  << endl;
      */


      t.Stop(); 
      t.Print();


      ///////// below fake test 
      //      if(!doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU1RMS3Fit,lZMU1FracFit, /*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU2RMS3Fit,lZMU2FracFit,/*lZMU23SigFit,*/"recoilfits/recoilfit_JAN9_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_UNBINNED_3G_53X.root" ,"PF",fId);
      //      if(doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU1RMS3Fit,lZMU1FracFit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU2RMS3Fit,lZMU2FracFit,/*lZMU23SigFit,*/"recoilfits/recoilfit_JAN9_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_UNBINNED_3G_53X.root" ,"PF",fId);

      ////// MC closure
      //      if(!doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,/*lZDU23SigFit,*/"recoilfits/recoilfit_JAN9_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_UNBINNED_3G_53X.root" ,"PF",fId);
      //      if(doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,/*lZDU23SigFit,*/"recoilfits/recoilfit_JAN9_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_UNBINNED_3G_53X.root" ,"PF",fId);

      //////
      //////
      //////
      //////

      //            if(!doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU1RMS3Fit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU2RMS3Fit,/*lZMU23SigFit,*/"recoilfits/recoilfit_NOV19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X_powheg.root" ,"PF",fId);
      //      if(doMad) readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU1RMS3Fit,/*lZMU13SigFit,*/lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU2RMS3Fit,/*lZMU23SigFit,*/"recoilfits/recoilfit_NOV19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X_madgraph.root" ,"PF",fId);

      ////// MC closure
      //      if(!doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,/*lZDU23SigFit,*/"recoilfits/recoilfit_DEC19_genZ_invGraph_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_3G_53X_powheg.root" ,"PF",fId);
      //      if(doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU1RMS3Fit,lZDU1FracFit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU2RMS3Fit,lZDU2FracFit,/*lZDU23SigFit,*/"recoilfits/recoilfit_DEC19_genZ_invGraph_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_3G_53X_madgraph.root" ,"PF",fId);

      ////// MC closure
      //      if(!doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,/*lZDU23SigFit,*/"recoilfits/recoilfit_NOV19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X_powheg.root" ,"PF",fId);
      //      if(doMad) readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,/*lZDU13SigFit,*/lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,/*lZDU23SigFit,*/"recoilfits/recoilfit_NOV19_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_53X_madgraph.root" ,"PF",fId);
	
    }
    
  }
  
  if(MCtype==2) {
    
    cout << "PROCESSING DY DATA -- inclusive Nvertex -- charged only " << endl;
    if(doBKG) cout << "w/ BKG fits  " << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/DATA/ZTreeProducer_tree_RecoSkimmed.root"); // this is 44X

    if(do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_8TeV/DATA_Run2012ABCD/ZTreeProducer_tree.root");
    //    if(!do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DATA/ZTreeProducer_tree.root");
    if(!do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2015_05_24_53X_sumEtFIX/DATA/ZTreeProducer_tree.root");
    //    if(doMad && !do8TeV )  fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsLL/ZTreeProducer_tree_SignalRecoSkimmed.root");

    //    if(!doMad && !do8TeV ) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsMM/ZTreeProducer_tree_SignalRecoSkimmed.root");

    // this is the 8TeV, is the runD, it contains the other METs, obtained from the Z skims
    //    if(doABC || do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_gentkmet/DATA_Run2012A/ZTreeProducer_tree.root");
    //    if(doABC || do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_gentkmet/DATA_Run2012B/ZTreeProducer_tree.root");
    //    if(doMad || do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_gentkmet/DATA_Run2012C/ZTreeProducer_tree.root");
    //    if(doMad || do8TeV) fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_gentkmet/DATA_Run2012D/ZTreeProducer_tree.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    fData = true; 

    name+="_DATA";
    //    name+="_MADlikeDATA";

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

  if(MCtype==5) {

    doBKG=true;

    cout << "PROCESSING TTBAR MC --- MADGRAPH" << endl;
    //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/TTJets/ZTreeProducer_tree.root");
    fDataFile = TFile::Open("root://eoscms//eos/cms//store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/TTJetsNoMcut/TTJets/ZTreeProducer_tree.root");

    fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
    //    fData = false; /*fId = -1; */ 
    fData = true; // need to treat this as DATA like

    name+="_TTBAR";

  }


  //////
  //// COMPLETE NAMING
  //////     

  if(doPhiStar) name+="_phiStar";
  if(doIterativeMet) name+="_ITERATIVE";

  if(doBKG) name+="_bkg";
  if(doClosure) name+="_CLOSURE";

  if(doChargedMet)  name+="_tkmet";
  if(!doChargedMet) name+="_pfmet";
  if(doGenTkMet||doGenPfMet) name +="_Gen";

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

  if(!usePol3 && !usePol4) name += "_pol2";
  if(usePol3) name += "_pol3";
  if(usePol4) name += "_pol4";
  if(useErfPol2ScaleU1) name += "_U1scaleErf";

  if(doType2)  name += "_type2";
  if(!doType2) name += "_type1";

  if(!doSingleGauss) name += "_doubleGauss";
  if(doSingleGauss) name += "_oneGauss";
  if(doTriGauss && !doSingleGauss) name += "_triGauss";
  

  if(doHalfStat) { 
    name+="_halfStat";
  } else { 
    name+="_x2Stat";
  }

  if(doRunA) name += "_runA";
  if(doRunB) name += "_runB";

  if(doLepProjAbsolute)  name +="_doLepProjAbsolute";
  if(doRecoParam) name += "_recoParam";
  if(doAbsolute) name+="_absolute";
  if(doApplyCorr) name+="_Correlation";
  
  if(doIterativeMet && !doOnlyU1 && doOnlyU2) name+="_applyOnlyU2";
  if(doIterativeMet && doOnlyU1 && !doOnlyU2) name+="_applyOnlyU1";
    
  if(doGigiRescaling) name+="_GigiRescale";
  if(doRoch_corr) name+="_ROCHcorr";
  if(doPrintAll) name+="_UNBINNED";

  if(doVTXbinning) name+="_VTX";
  if(doVTXbinning) name+=VTXbin;

  if(do3G) name+="_3G";

  name+="_53X";
  
  if(fData)  name+=".root";
  if(!fData && doMad) name+="_madgraph.root";
  if(!fData && !doMad) name+="_powheg.root";
  
  cout << name.Data() << endl;

  TStopwatch t; 
  t.Start(); 
  fitRecoil(fDataTree, name.Data(), fId);
  t.Stop(); 
  t.Print();

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
    if(!fData) fileName+="_PDF";
    if(!fData) fileName += pType;
    if(doYbinning) fileName += "_rapBin";
    if(doYbinning) fileName += fId;
    if(do8TeV) fileName += "_8TeV";
    if(!do8TeV) fileName += "_7TeV";
    if(doRecoParam) fileName += "_recoParam";
    if(doIterativeMet && doApplyCorr) fileName += "_ITERATIVE_doApplyCorr";
    if(doIterativeMet && invGraph) fileName += "_ITERATIVE_invGraph";

    //    if(doIterativeMet && doSingleGauss) fileName += "_ITERATIVE_oneGauss";
    //    if(doIterativeMet && !doSingleGauss) fileName += "_ITERATIVE_twoGauss";

    fileName += ".root";
    
    TFile f3(fileName.Data(),"RECREATE");
    histoZPt.Write();
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
    histoU1Rawcorr.Write();
    histoU2Rawcorr.Write();
    histoUpVal.Write();
    histoUraw1.Write();
    histoUraw2.Write();
    histoFrac1D.Write();
    histoFrac2D.Write();
    histoFrac1M.Write();
    histoFrac2M.Write();
    histoFrac1D2D.Write();
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
    /////
    histoDeltaU1.Write();
    histoDeltaU2.Write();
    histoDeltaU1pull.Write();
    histoDeltaU2pull.Write();
    histoDeltaU1pull56.Write();
    histoDeltaU2pull56.Write();
    histoDeltaU1pullvsU.Write();
    histoDeltaU2pullvsU.Write();

    histoDeltaU1pullvsUreduced.Write();
    histoDeltaU2pullvsUreduced.Write();

    /////
    histoU1original.Write();
    histoU2original.Write();
    histoU1smeared.Write();
    histoU2smeared.Write();
    histoU2origFrac.Write();
    histoU2wishFrac.Write();
    histoU2origFrac05.Write();
    histoU2wishFrac05.Write();
    histoU2origFrac510.Write();
    histoU2wishFrac510.Write();
    histoU2origFrac1015.Write();
    histoU2wishFrac1015.Write();
    histoU2origFrac1520.Write();
    histoU2wishFrac1520.Write();
    //###
    histoPhiStar.Write();
    histoThetaStar.Write();
    histoPhiStarvsZpt.Write();
    histoPhiStarvsgenZpt.Write();
    //###
    Vpt0.Write();
    Vpt1.Write();
    Vpt2.Write();
    Vpt3.Write();
    Vpt4.Write();
    Vpt5.Write();
    Vpt10.Write();

    Vy0.Write();
    Vy1.Write();
    Vy2.Write();
    Vy3.Write();
    Vy4.Write();
    Vy5.Write();
    Vy10.Write();

    sumEt0.Write();
    sumEt1.Write();
    sumEt2.Write();
    sumEt3.Write();
    sumEt4.Write();
    sumEt5.Write();
    sumEt10.Write();

    xPDF0.Write();
    xPDF1.Write();
    xPDF2.Write();
    xPDFm1.Write();
    xPDFm2.Write();
    xPDF3.Write();
    xPDF4.Write();
    xPDF5.Write();

    f3.Write();
    f3.Close();

  }

  //  return 0;
  return ;

}
