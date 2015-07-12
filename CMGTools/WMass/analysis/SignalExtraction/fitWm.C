//================================================================================================
//
// Perform fit to extract W->munu signal
//
//  * outputs plots and fit results summary
//
//________________________________________________________________________________________________

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                        // access to gROOT, entry point to ROOT system
#include <TSystem.h>                      // interface to OS
#include <TStyle.h>                       // class to handle ROOT plotting styles
#include <TFile.h>                        // file handle class
#include <TTree.h>                        // class to access ntuples
#include <TBenchmark.h>                   // class to track macro running statistics
#include <TH1D.h>                         // histogram class
#include <vector>                         // STL vector class
#include <iostream>                       // standard I/O
#include <iomanip>                        // functions to format standard I/O
#include <fstream>                        // functions for file I/O
#include <string>                         // C++ string class
#include <sstream>                        // class for parsing strings
#include "Math/LorentzVector.h"           // 4-vector class
#include <TLorentzVector.h>           // 4-vector class

#include "Utils/MyTools.hh"	          // various helper functions
#include "Utils/CPlot.hh"	          // helper class for plots
#include "Utils/MitStyleRemix.hh"      // style settings for drawing
#include "Utils/WModels.hh"            // definitions of PDFs for fitting
// #include "Utils/RhhCruijffPdf.hh"            // definitions of PDFs for fitting
#include "Utils/RecoilCorrector.hh"    // class to handle recoil corrections for MET

#include "../includes/common.h"

// RooFit headers
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooFormulaVar.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"
#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LorentzVector;


//=== FUNCTION DECLARATIONS ======================================================================================

// make data-fit difference plots
TH1D* makeDiffHist(TH1D* hData, TH1D* hFit, const TString name);

// print correlations of fitted parameters
void printCorrelations(ostream& os, RooFitResult *res);

// print chi2 test and KS test results
void printChi2AndKSResults(ostream& os, 
const Double_t chi2prob, const Double_t chi2ndf, 
const Double_t ksprob, const Double_t ksprobpe);

// make webpage
void makeHTML(const TString outDir);


//=== MAIN MACRO ================================================================================================= 

void fitWm(const TString  outputDir="test",   // output directory
const Double_t lumi=4.75,        // integrated luminosity (/fb)
const Int_t    Ecm=7         // center-of-mass energy
) {
  // gBenchmark->Start("fitWm");
  
  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //==============================================================================================================   
  
  // // MET histogram binning and range
  // const Int_t    NBINS   = 50;
  // const Double_t METMAX  = 100;
  
  // file format for output plots
  const TString format1("png"); 
  

  //--------------------------------------------------------------------------------------------------------------
  // Main W analysis code 
  //==============================================================================================================  
  
  int pTmTmet = 0; // 0 = pT , 1 = mT , 2 = MET
  int generated_PDF_set = 10042;
  TString eta_str = Form("%.1f",WMass::etaMaxMuons[0]); eta_str.ReplaceAll(".","p");
  
  // TString WNeg_fit_sig_histoname = "hWNeg_METNonScaled_SelRange_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WNeg_fit_qcd_histoname = "hWNeg_METNonScaled_SelRangeQCD_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WPos_fit_sig_histoname = "hWPos_METNonScaled_SelRange_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WPos_fit_qcd_histoname = "hWPos_METNonScaled_SelRangeQCD_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WNeg_fit_sig_histoname = "hWNeg_MtNonScaled_SelRange_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WNeg_fit_qcd_histoname = "hWNeg_MtNonScaled_SelRangeQCD_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WPos_fit_sig_histoname = "hWPos_MtNonScaled_SelRange_8_JetCut_pdf10042-0_eta1p1_80419";
  // TString WPos_fit_qcd_histoname = "hWPos_MtNonScaled_SelRangeQCD_8_JetCut_pdf10042-0_eta1p1_80419";
  
  TString WNeg_fit_sig_histoname = Form("hWNeg_%sNonScaled_SelRange_8_JetCut_pdf%d-0_eta%s_80419",WMass::FitVar_str[pTmTmet].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data());
  TString WNeg_fit_qcd_histoname = Form("hWNeg_%sNonScaled_SelRangeQCD_8_JetCut_pdf%d-0_eta%s_80419",WMass::FitVar_str[pTmTmet].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data());
  TString WPos_fit_sig_histoname = Form("hWPos_%sNonScaled_SelRange_8_JetCut_pdf%d-0_eta%s_80419",WMass::FitVar_str[pTmTmet].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data());
  TString WPos_fit_qcd_histoname = Form("hWPos_%sNonScaled_SelRangeQCD_8_JetCut_pdf%d-0_eta%s_80419",WMass::FitVar_str[pTmTmet].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,eta_str.Data());
  
  cout << "loading files and histos" << endl;
  TFile*fData      = new TFile(Form("../JobOutputs/%s/output_DATA/WanalysisOnDATA.root",outputDir.Data()));
  fData->Print();
  TFile*fWmunu     = new TFile(Form("../JobOutputs/%s/output_WJetsSig/WanalysisOnDATA.root",outputDir.Data()));
  fWmunu->Print();
  TFile*fEWKTT       = new TFile(Form("../JobOutputs/%s/output_EWKTT/WanalysisOnDATA.root",outputDir.Data()));
  fEWKTT->Print();
  // TH1D *hDataMet   = (TH1D*)fData->Get(WNeg_fit_sig_histoname); 
  TH1D *hDataMetm  = (TH1D*)fData->Get(WNeg_fit_sig_histoname); 
  TH1D *hDataMetp  = (TH1D*)fData->Get(WPos_fit_sig_histoname); 
  hDataMetm->Print();
  // TH1D *hWmunuMet  = (TH1D*)fWmunu->Get(WNeg_fit_sig_histoname); 
  TH1D *hWmunuMetm = (TH1D*)fWmunu->Get(WNeg_fit_sig_histoname); 
  TH1D *hWmunuMetp = (TH1D*)fWmunu->Get(WPos_fit_sig_histoname);
  hWmunuMetm->Print();
  // TH1D *hEWKMet    = (TH1D*)fEWKTT->Get(WNeg_fit_sig_histoname); 
  TH1D *hEWKMetm   = (TH1D*)fEWKTT->Get(WNeg_fit_sig_histoname); 
  TH1D *hEWKMetp   = (TH1D*)fEWKTT->Get(WPos_fit_sig_histoname); 

  // TH1D *hAntiDataMet   = (TH1D*)fData->Get(WNeg_fit_qcd_histoname); 
  TH1D *hAntiDataMetm  = (TH1D*)fData->Get(WNeg_fit_qcd_histoname); 
  TH1D *hAntiDataMetp  = (TH1D*)fData->Get(WPos_fit_qcd_histoname); 
  hAntiDataMetm->Print();
  // TH1D *hAntiWmunuMet  = (TH1D*)fWmunu->Get(WNeg_fit_qcd_histoname); 
  TH1D *hAntiWmunuMetm = (TH1D*)fWmunu->Get(WNeg_fit_qcd_histoname); 
  TH1D *hAntiWmunuMetp = (TH1D*)fWmunu->Get(WPos_fit_qcd_histoname); 
  // TH1D *hAntiEWKMet    = (TH1D*)fEWKTT->Get(WNeg_fit_qcd_histoname); 
  TH1D *hAntiEWKMetm   = (TH1D*)fEWKTT->Get(WNeg_fit_qcd_histoname); 
  TH1D *hAntiEWKMetp   = (TH1D*)fEWKTT->Get(WPos_fit_qcd_histoname); 
  hAntiEWKMetm->Print();
  
  cout << "ciao" << endl;
  TString outputDir2 = Form("../JobOutputs/%s/fitSigResults/",outputDir.Data());
  cout << outputDir2.Data() << endl;

   // Create output directory
  gSystem->mkdir(outputDir2);
  CPlot::sOutDir = outputDir2;  

  //
  // Declare fit parameters for signal and background yields
  // Note: W signal and EWK+top PDFs are constrained to the ratio described in MC
  //
  // RooRealVar nSig("nSig","nSig",0.7*(hDataMet->Integral()),0,hDataMet->Integral());
  // RooRealVar nQCD("nQCD","nQCD",0.3*(hDataMet->Integral()),0,hDataMet->Integral());
  // RooRealVar cewk("cewk","cewk",0.1,0,5) ;
  // cewk.setVal(hEWKMet->Integral()/hWmunuMet->Integral());
  // cewk.setConstant(kTRUE);
  // RooFormulaVar nEWK("nEWK","nEWK","cewk*nSig",RooArgList(nSig,cewk));
  // RooRealVar nAntiSig("nAntiSig","nAntiSig",0.05*(hAntiDataMet->Integral()),0,hAntiDataMet->Integral());
  // RooRealVar nAntiQCD("nAntiQCD","nAntiQCD",0.9*(hDataMet->Integral()),0,hDataMet->Integral());
  // RooRealVar dewk("dewk","dewk",0.1,0,5) ;
  // dewk.setVal(hAntiEWKMet->Integral()/hAntiWmunuMet->Integral());
  // dewk.setConstant(kTRUE);
  // RooFormulaVar nAntiEWK("nAntiEWK","nAntiEWK","dewk*nAntiSig",RooArgList(nAntiSig,dewk));
  
  RooRealVar nSigp("nSigp","nSigp",0.7*(hDataMetp->Integral()),0,hDataMetp->Integral());
  RooRealVar nQCDp("nQCDp","nQCDp",0.3*(hDataMetp->Integral()),0,hDataMetp->Integral());
  RooRealVar cewkp("cewkp","cewkp",0.1,0,5) ;
  cewkp.setVal(hEWKMetp->Integral()/hWmunuMetp->Integral());
  cewkp.setConstant(kTRUE);
  RooFormulaVar nEWKp("nEWKp","nEWKp","cewkp*nSigp",RooArgList(nSigp,cewkp));
  RooRealVar nAntiSigp("nAntiSigp","nAntiSigp",0.05*(hAntiDataMetp->Integral()),0,hAntiDataMetp->Integral());
  RooRealVar nAntiQCDp("nAntiQCDp","nAntiQCDp",0.9*(hAntiDataMetp->Integral()),0,hAntiDataMetp->Integral());
  RooRealVar dewkp("dewkp","dewkp",0.1,0,5) ;
  dewkp.setVal(hAntiEWKMetp->Integral()/hAntiWmunuMetp->Integral());
  dewkp.setConstant(kTRUE);
  RooFormulaVar nAntiEWKp("nAntiEWKp","nAntiEWKp","dewkp*nAntiSigp",RooArgList(nAntiSigp,dewkp));
  
  RooRealVar nSigm("nSigm","nSigm",0.7*(hDataMetm->Integral()),0,hDataMetm->Integral());
  RooRealVar nQCDm("nQCDm","nQCDm",0.3*(hDataMetm->Integral()),0,hDataMetm->Integral());
  RooRealVar cewkm("cewkm","cewkm",0.1,0,5) ;
  cewkm.setVal(hEWKMetm->Integral()/hWmunuMetm->Integral());
  cewkm.setConstant(kTRUE);
  RooFormulaVar nEWKm("nEWKm","nEWKm","cewkm*nSigm",RooArgList(nSigm,cewkm));  
  RooRealVar nAntiSigm("nAntiSigm","nAntiSigm",0.05*(hAntiDataMetm->Integral()),0,hAntiDataMetm->Integral());
  RooRealVar nAntiQCDm("nAntiQCDm","nAntiQCDm",0.9*(hAntiDataMetm->Integral()),0,hAntiDataMetm->Integral());
  RooRealVar dewkm("dewkm","dewkm",0.1,0,5) ;
  dewkm.setVal(hAntiEWKMetm->Integral()/hAntiWmunuMetm->Integral());
  dewkm.setConstant(kTRUE);
  RooFormulaVar nAntiEWKm("nAntiEWKm","nAntiEWKm","dewkm*nAntiSigm",RooArgList(nAntiSigm,dewkm));


  //
  // Construct PDFs for fitting
  //
  double METMAX = hDataMetm->GetXaxis()->GetXmax();
  int NBINS = hDataMetm->GetNbinsX();
  RooRealVar pTmTmet_xvar("pTmTmet_xvar","pTmTmet_xvar",hDataMetm->GetXaxis()->GetXmin(),METMAX);
  pTmTmet_xvar.setBins(NBINS);
  
  // Signal PDFs
  // RooDataHist wmunuMet ("wmunuMET", "wmunuMET", RooArgSet(pTmTmet_xvar),hWmunuMet);  RooHistPdf pdfWm ("wm", "wm", pTmTmet_xvar,wmunuMet, 1);
  RooDataHist wmunuMetp("wmunuMETp","wmunuMETp",RooArgSet(pTmTmet_xvar),hWmunuMetp); RooHistPdf pdfWmp("wmp","wmp",pTmTmet_xvar,wmunuMetp,1);
  RooDataHist wmunuMetm("wmunuMETm","wmunuMETm",RooArgSet(pTmTmet_xvar),hWmunuMetm); RooHistPdf pdfWmm("wmm","wmm",pTmTmet_xvar,wmunuMetm,1); 
  
  // EWK+top PDFs
  // RooDataHist ewkMet ("ewkMET", "ewkMET", RooArgSet(pTmTmet_xvar),hEWKMet);  RooHistPdf pdfEWK ("ewk", "ewk", pTmTmet_xvar,ewkMet, 1);
  RooDataHist ewkMetp("ewkMETp","ewkMETp",RooArgSet(pTmTmet_xvar),hEWKMetp); RooHistPdf pdfEWKp("ewkp","ewkp",pTmTmet_xvar,ewkMetp,1); 
  RooDataHist ewkMetm("ewkMETm","ewkMETm",RooArgSet(pTmTmet_xvar),hEWKMetm); RooHistPdf pdfEWKm("ewkm","ewkm",pTmTmet_xvar,ewkMetm,1); 
    
  // QCD Pdfs
  // CLucaModel0 qcd("qcd",pTmTmet_xvar);
  // CLucaModel0 qcdp("qcdp",pTmTmet_xvar);
  // CLucaModel0 qcdm("qcdm",pTmTmet_xvar);
  
  // TsallisPdf qcdp("qcdp",pTmTmet_xvar); // pT 
  // TsallisPdf qcdm("qcdm",pTmTmet_xvar); // pT 
  ExpPdf qcdp("qcdp",pTmTmet_xvar); // pT 
  ExpPdf qcdm("qcdm",pTmTmet_xvar); // pT 
  // RhhCruijffPdf qcdp("qcdp",pTmTmet_xvar); // mT 
  // RhhCruijffPdf qcdm("qcdm",pTmTmet_xvar); // mT 
  // CPepeModel0 qcdp("qcdp",pTmTmet_xvar); // MET
  // CPepeModel0 qcdm("qcdm",pTmTmet_xvar); // MET
  
  // Signal + Background PDFs
  // RooAddPdf pdfMet ("pdfMet", "pdfMet", RooArgList(pdfWm,pdfEWK,*(qcd.model)),   RooArgList(nSig,nEWK,nQCD));  
  RooAddPdf pdfMetp("pdfMetp","pdfMetp",RooArgList(pdfWmp,pdfEWKp,*(qcdp.model)),RooArgList(nSigp,nEWKp,nQCDp));
  RooAddPdf pdfMetm("pdfMetm","pdfMetm",RooArgList(pdfWmm,pdfEWKm,*(qcdm.model)),RooArgList(nSigm,nEWKm,nQCDm));
  
  
  // Anti-Signal PDFs
  // RooDataHist awmunuMet ("awmunuMET", "awmunuMET", RooArgSet(pTmTmet_xvar),hAntiWmunuMet);  RooHistPdf apdfWm ("awm", "awm", pTmTmet_xvar,awmunuMet, 1);
  RooDataHist awmunuMetp("awmunuMETp","awmunuMETp",RooArgSet(pTmTmet_xvar),hAntiWmunuMetp); RooHistPdf apdfWmp("awmp","awmp",pTmTmet_xvar,awmunuMetp,1);
  RooDataHist awmunuMetm("awmunuMETm","awmunuMETm",RooArgSet(pTmTmet_xvar),hAntiWmunuMetm); RooHistPdf apdfWmm("awmm","awmm",pTmTmet_xvar,awmunuMetm,1); 
  
  // Anti-EWK+top PDFs
  // RooDataHist aewkMet ("aewkMET", "aewkMET", RooArgSet(pTmTmet_xvar),hAntiEWKMet);  RooHistPdf apdfEWK ("aewk", "aewk", pTmTmet_xvar,aewkMet, 1);
  RooDataHist aewkMetp("aewkMETp","aewkMETp",RooArgSet(pTmTmet_xvar),hAntiEWKMetp); RooHistPdf apdfEWKp("aewkp","aewkp",pTmTmet_xvar,aewkMetp,1); 
  RooDataHist aewkMetm("aewkMETm","aewkMETm",RooArgSet(pTmTmet_xvar),hAntiEWKMetm); RooHistPdf apdfEWKm("aewkm","aewkm",pTmTmet_xvar,aewkMetm,1); 
  
  // Anti-selection PDFs
  // RooAddPdf apdfMet ("apdfMet", "apdfMet", RooArgList(apdfWm,apdfEWK,*(qcd.model)),   RooArgList(nAntiSig,nAntiEWK,nAntiQCD));  
  RooAddPdf apdfMetp("apdfMetp","apdfMetp",RooArgList(apdfWmp,apdfEWKp,*(qcdp.model)),RooArgList(nAntiSigp,nAntiEWKp,nAntiQCDp));
  RooAddPdf apdfMetm("apdfMetm","apdfMetm",RooArgList(apdfWmm,apdfEWKm,*(qcdm.model)),RooArgList(nAntiSigm,nAntiEWKm,nAntiQCDm));
  
  // PDF for simultaneous fit
  RooCategory rooCat("rooCat","rooCat");
  rooCat.defineType("Select");
  rooCat.defineType("Anti");
  
  // RooSimultaneous pdfTotal("pdfTotal","pdfTotal",rooCat);
  // pdfTotal.addPdf(pdfMet, "Select");
  // pdfTotal.addPdf(apdfMet,"Anti");
  
  RooSimultaneous pdfTotalp("pdfTotalp","pdfTotalp",rooCat);
  pdfTotalp.addPdf(pdfMetp, "Select");
  pdfTotalp.addPdf(apdfMetp,"Anti");
  
  RooSimultaneous pdfTotalm("pdfTotalm","pdfTotalm",rooCat);
  pdfTotalm.addPdf(pdfMetm, "Select");
  pdfTotalm.addPdf(apdfMetm,"Anti");
  
  //
  // Perform fits
  //

  // RooDataHist dataMet("dataMet", "dataMet", RooArgSet(pTmTmet_xvar), hDataMet);
  // RooDataHist antiMet("antiMet", "antiMet", RooArgSet(pTmTmet_xvar), hAntiDataMet);
  // RooDataHist dataTotal("dataTotal","dataTotal", RooArgList(pTmTmet_xvar), Index(rooCat),
  // Import("Select", dataMet),
  // Import("Anti",   antiMet));
  // RooFitResult *fitRes = pdfTotal.fitTo(dataTotal,Extended(),Minos(kTRUE),Save(kTRUE));
  
  RooDataHist dataMetp("dataMetp", "dataMetp", RooArgSet(pTmTmet_xvar), hDataMetp);
  RooDataHist antiMetp("antiMetp", "antiMetp", RooArgSet(pTmTmet_xvar), hAntiDataMetp);
  RooDataHist dataTotalp("dataTotalp","dataTotalp", RooArgList(pTmTmet_xvar), Index(rooCat),
  Import("Select", dataMetp),
  Import("Anti",   antiMetp));
  RooFitResult *fitResp = pdfTotalp.fitTo(dataTotalp,Extended(),Minos(kTRUE),Save(kTRUE));
  
  RooDataHist dataMetm("dataMetm", "dataMetm", RooArgSet(pTmTmet_xvar), hDataMetm);
  RooDataHist antiMetm("antiMetm", "antiMetm", RooArgSet(pTmTmet_xvar), hAntiDataMetm);
  RooDataHist dataTotalm("dataTotalm","dataTotalm", RooArgList(pTmTmet_xvar), Index(rooCat),
  Import("Select", dataMetm),
  Import("Anti",   antiMetm));
  RooFitResult *fitResm = pdfTotalm.fitTo(dataTotalm,Extended(),Minos(kTRUE),Save(kTRUE));
  
  //
  // Use histogram version of fitted PDFs to make ratio plots
  // (Will also use PDF histograms later for Chi^2 and KS tests)
  //
  // TH1D *hPdfMet = (TH1D*)(pdfMet.createHistogram("hPdfMet", pTmTmet_xvar));
  // hPdfMet->Scale((nSig.getVal()+nEWK.getVal()+nQCD.getVal())/hPdfMet->Integral());
  // TH1D *hMetDiff = makeDiffHist(hDataMet,hPdfMet,"hMetDiff");
  // hMetDiff->SetMarkerStyle(kFullCircle);
  // hMetDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfMetp = (TH1D*)(pdfMetp.createHistogram("hPdfMetp", pTmTmet_xvar));
  hPdfMetp->Scale((nSigp.getVal()+nEWKp.getVal()+nQCDp.getVal())/hPdfMetp->Integral());
  TH1D *hMetpDiff = makeDiffHist(hDataMetp,hPdfMetp,"hMetpDiff");
  hMetpDiff->SetMarkerStyle(kFullCircle);
  hMetpDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfMetm = (TH1D*)(pdfMetm.createHistogram("hPdfMetm", pTmTmet_xvar));
  hPdfMetm->Scale((nSigm.getVal()+nEWKm.getVal()+nQCDm.getVal())/hPdfMetm->Integral());
  TH1D *hMetmDiff = makeDiffHist(hDataMetm,hPdfMetm,"hMetmDiff");
  hMetmDiff->SetMarkerStyle(kFullCircle); 
  hMetmDiff->SetMarkerSize(0.9);
  
  // TH1D *hPdfAntiMet = (TH1D*)(apdfMet.createHistogram("hPdfAntiMet", pTmTmet_xvar));
  // hPdfMet->Scale((nAntiSig.getVal()+nAntiEWK.getVal()+nAntiQCD.getVal())/hPdfAntiMet->Integral());
  // TH1D *hAntiMetDiff = makeDiffHist(hAntiDataMet,hPdfAntiMet,"hAntiMetDiff");
  // hAntiMetDiff->SetMarkerStyle(kFullCircle);
  // hAntiMetDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfAntiMetp = (TH1D*)(apdfMetp.createHistogram("hPdfAntiMetp", pTmTmet_xvar));
  hPdfAntiMetp->Scale((nAntiSigp.getVal()+nAntiEWKp.getVal()+nAntiQCDp.getVal())/hPdfAntiMetp->Integral());
  TH1D *hAntiMetpDiff = makeDiffHist(hAntiDataMetp,hPdfAntiMetp,"hAntiMetpDiff");
  hAntiMetpDiff->SetMarkerStyle(kFullCircle);
  hAntiMetpDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfAntiMetm = (TH1D*)(apdfMetm.createHistogram("hPdfAntiMetm", pTmTmet_xvar));
  hPdfAntiMetm->Scale((nAntiSigm.getVal()+nAntiEWKm.getVal()+nAntiQCDm.getVal())/hPdfAntiMetm->Integral());
  TH1D *hAntiMetmDiff = makeDiffHist(hAntiDataMetm,hPdfAntiMetm,"hAntiMetmDiff");
  hAntiMetmDiff->SetMarkerStyle(kFullCircle); 
  hAntiMetmDiff->SetMarkerSize(0.9);
  
  
  //--------------------------------------------------------------------------------------------------------------
  // Make plots 
  //==============================================================================================================  
  
  TCanvas *c = MakeCanvas("c","c",800,800);
  c->Divide(1,2,0,0);
  c->cd(1)->SetPad(0,0.3,1.0,1.0);
  c->cd(1)->SetTopMargin(0.1);
  c->cd(1)->SetBottomMargin(0.01);
  c->cd(1)->SetLeftMargin(0.18);  
  c->cd(1)->SetRightMargin(0.07);  
  c->cd(1)->SetTickx(1);
  c->cd(1)->SetTicky(1);  
  c->cd(2)->SetPad(0,0,1.0,0.3);
  c->cd(2)->SetTopMargin(0.05);
  c->cd(2)->SetBottomMargin(0.45);
  c->cd(2)->SetLeftMargin(0.18);
  c->cd(2)->SetRightMargin(0.07);
  c->cd(2)->SetTickx(1);
  c->cd(2)->SetTicky(1);
  gStyle->SetTitleOffset(1.400,"Y");
  
  char ylabel[100];  // string buffer for y-axis label
  
  // label for lumi
  char lumitext[100];
  if(lumi<0.1) sprintf(lumitext,"%.1f pb^{-1}  at  #sqrt{s} = %i TeV",lumi*1000.,Ecm);
  else         sprintf(lumitext,"%.2f fb^{-1}  at  #sqrt{s} = %i TeV",lumi,Ecm);
  
  // plot colors
  Int_t linecolorW   = kOrange-3;
  Int_t fillcolorW   = kOrange-2;
  Int_t linecolorEWK = kOrange+10;
  Int_t fillcolorEWK = kOrange+7;
  Int_t linecolorQCD = kViolet+2;
  Int_t fillcolorQCD = kViolet-5;
  Int_t ratioColor   = kGray+2;
  
  //
  // Dummy histograms for TLegend
  // (I can't figure out how to properly pass RooFit objects...)
  //
  TH1D *hDummyData = new TH1D("hDummyData","",0,0,10);
  hDummyData->SetMarkerStyle(kFullCircle);
  hDummyData->SetMarkerSize(0.9);
  
  TH1D *hDummyW = new TH1D("hDummyW","",0,0,10);
  hDummyW->SetLineColor(linecolorW);
  hDummyW->SetFillColor(fillcolorW);
  hDummyW->SetFillStyle(1001);
  
  TH1D *hDummyEWK = new TH1D("hDummyEWK","",0,0,10);
  hDummyEWK->SetLineColor(linecolorEWK);
  hDummyEWK->SetFillColor(fillcolorEWK);
  hDummyEWK->SetFillStyle(1001);
  
  TH1D *hDummyQCD = new TH1D("hDummyQCD","",0,0,10);
  hDummyQCD->SetLineColor(linecolorQCD);
  hDummyQCD->SetFillColor(fillcolorQCD);
  hDummyQCD->SetFillStyle(1001);
  
  //
  // W MET plot
  //
  // RooPlot *wmframe = pTmTmet_xvar.frame(Bins(NBINS));    
  // dataMet.plotOn(wmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  // pdfMet.plotOn(wmframe,FillColor(fillcolorW),DrawOption("F"));
  // pdfMet.plotOn(wmframe,LineColor(linecolorW));
  // pdfMet.plotOn(wmframe,Components(RooArgSet(pdfEWK,*(qcd.model))),FillColor(fillcolorEWK),DrawOption("F"));
  // pdfMet.plotOn(wmframe,Components(RooArgSet(pdfEWK,*(qcd.model))),LineColor(linecolorEWK));
  // pdfMet.plotOn(wmframe,Components(RooArgSet(*(qcd.model))),FillColor(fillcolorQCD),DrawOption("F"));
  // pdfMet.plotOn(wmframe,Components(RooArgSet(*(qcd.model))),LineColor(linecolorQCD));
  // pdfMet.plotOn(wmframe,Components(RooArgSet(pdfWm)),LineColor(linecolorW),LineStyle(2));
  // dataMet.plotOn(wmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  
  // sprintf(ylabel,"Events / %.1f GeV",hDataMet->GetBinWidth(1));
  // CPlot plotMet("fitmet",wmframe,"","",ylabel);
  // plotMet.SetLegend(0.6,0.57,0.93,0.77);
  // plotMet.GetLegend()->AddEntry(hDummyData,"data","PL");
  // plotMet.GetLegend()->AddEntry(hDummyW,Form("W#rightarrow#mu#nu (%.1f\%)",nSig.getVal()/hDataMet->Integral()*100),"F");
  // plotMet.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nEWK.getVal()/hDataMet->Integral()*100),"F");
  // plotMet.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nQCD.getVal()/hDataMet->Integral()*100),"F");
  // plotMet.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  // plotMet.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  // plotMet.SetYRange(0.1,1.1*(hDataMet->GetMaximum()));
  // plotMet.Draw(c,kFALSE,format1,1);

  // CPlot plotMetDiff("fitmet","",Form("%s [GeV]",WMass::FitVar_str[pTmTmet].Data()),"#chi");
  // plotMetDiff.AddHist1D(hMetDiff,"EX0",ratioColor);
  // plotMetDiff.SetYRange(-8,8);
  // plotMetDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  // plotMetDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  // plotMetDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  // plotMetDiff.Draw(c,kTRUE,format1,2);
  
  // plotMet.SetName("fitmetlog");
  // plotMet.SetLogy();
  // plotMet.SetYRange(1e-3*(hDataMet->GetMaximum()),10*(hDataMet->GetMaximum()));
  // plotMet.Draw(c,kTRUE,format1,1);
  
  // RooPlot *awmframe = pTmTmet_xvar.frame(Bins(NBINS));    
  // antiMet.plotOn(awmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  // apdfMet.plotOn(awmframe,FillColor(fillcolorW),DrawOption("F"));
  // apdfMet.plotOn(awmframe,LineColor(linecolorW));
  // apdfMet.plotOn(awmframe,Components(RooArgSet(apdfEWK,*(qcd.model))),FillColor(fillcolorEWK),DrawOption("F"));
  // apdfMet.plotOn(awmframe,Components(RooArgSet(apdfEWK,*(qcd.model))),LineColor(linecolorEWK));
  // apdfMet.plotOn(awmframe,Components(RooArgSet(*(qcd.model))),FillColor(fillcolorQCD),DrawOption("F"));
  // apdfMet.plotOn(awmframe,Components(RooArgSet(*(qcd.model))),LineColor(linecolorQCD));
  // apdfMet.plotOn(awmframe,Components(RooArgSet(apdfWm)),LineColor(linecolorW),LineStyle(2));
  // antiMet.plotOn(awmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  // sprintf(ylabel,"Events / %.1f GeV",hAntiDataMet->GetBinWidth(1));
  // CPlot plotAntiMet("fitantimet",awmframe,"","",ylabel);
  // plotAntiMet.SetLegend(0.6,0.57,0.93,0.77);
  // plotAntiMet.GetLegend()->AddEntry(hDummyData,"data","PL");
  // plotAntiMet.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nAntiSig.getVal()/hAntiDataMet->Integral()*100),"F");
  // plotAntiMet.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nAntiEWK.getVal()/hAntiDataMet->Integral()*100),"F");
  // plotAntiMet.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nAntiQCD.getVal()/hAntiDataMet->Integral()*100),"F");
  // plotAntiMet.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  // plotAntiMet.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  // plotAntiMet.SetYRange(0.1,1.1*(hAntiDataMet->GetMaximum()));
  // plotAntiMet.Draw(c,kFALSE,format1,1);

  // CPlot plotAntiMetDiff("fitantimet","",Form("%s [GeV]",WMass::FitVar_str[pTmTmet].Data()),"#chi");
  // plotAntiMetDiff.AddHist1D(hMetDiff,"EX0",ratioColor);
  // plotAntiMetDiff.SetYRange(-8,8);
  // plotAntiMetDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  // plotAntiMetDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  // plotAntiMetDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  // plotAntiMetDiff.Draw(c,kTRUE,format1,2);
  
  // plotAntiMet.SetName("fitantimetlog");
  // plotAntiMet.SetLogy();
  // plotAntiMet.SetYRange(1e-3*(hAntiDataMet->GetMaximum()),10*(hAntiDataMet->GetMaximum()));
  // plotAntiMet.Draw(c,kTRUE,format1,1);
  
  //
  // W+ MET plot
  //
  RooPlot *wmpframe = pTmTmet_xvar.frame(Bins(NBINS));    
  dataMetp.plotOn(wmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMetp.plotOn(wmpframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMetp.plotOn(wmpframe,LineColor(linecolorW));
  pdfMetp.plotOn(wmpframe,Components(RooArgSet(pdfEWKp,*(qcdp.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMetp.plotOn(wmpframe,Components(RooArgSet(pdfEWKp,*(qcdp.model))),LineColor(linecolorEWK));
  pdfMetp.plotOn(wmpframe,Components(RooArgSet(*(qcdp.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMetp.plotOn(wmpframe,Components(RooArgSet(*(qcdp.model))),LineColor(linecolorQCD));
  pdfMetp.plotOn(wmpframe,Components(RooArgSet(pdfWmp)),LineColor(linecolorW),LineStyle(2));
  dataMetp.plotOn(wmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMetp->GetBinWidth(1));
  CPlot plotMetp("fitmetp",wmpframe,"","",ylabel);
  plotMetp.SetLegend(0.6,0.57,0.93,0.77);
  plotMetp.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMetp.GetLegend()->AddEntry(hDummyW,Form("W^{+}#rightarrow#mu^{+}#nu (%.1f\%)",nSigp.getVal()/hDataMetp->Integral()*100),"F");
  plotMetp.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nEWKp.getVal()/hDataMetp->Integral()*100),"F");
  plotMetp.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nQCDp.getVal()/hDataMetp->Integral()*100),"F");
  plotMetp.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMetp.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMetp.SetYRange(0.1,1.1*(hDataMetp->GetMaximum()));
  plotMetp.Draw(c,kFALSE,format1,1);

  CPlot plotMetpDiff("fitmetp","",Form("%s [GeV]",WMass::FitVar_str[pTmTmet].Data()),"#chi");
  plotMetpDiff.AddHist1D(hMetpDiff,"EX0",ratioColor);
  plotMetpDiff.SetYRange(-8,8);
  plotMetpDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotMetpDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotMetpDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotMetpDiff.Draw(c,kTRUE,format1,2);
  
  plotMetp.SetName("fitmetplog");
  plotMetp.SetLogy();
  plotMetp.SetYRange(1e-3*(hDataMetp->GetMaximum()),10*(hDataMetp->GetMaximum()));
  plotMetp.Draw(c,kTRUE,format1,1);
  // plotMetp.Draw(c,kTRUE,l,1);

  RooPlot *awmpframe = pTmTmet_xvar.frame(Bins(NBINS));    
  antiMetp.plotOn(awmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  apdfMetp.plotOn(awmpframe,FillColor(fillcolorW),DrawOption("F"));
  apdfMetp.plotOn(awmpframe,LineColor(linecolorW));
  apdfMetp.plotOn(awmpframe,Components(RooArgSet(apdfEWKp,*(qcdp.model))),FillColor(fillcolorEWK),DrawOption("F"));
  apdfMetp.plotOn(awmpframe,Components(RooArgSet(apdfEWKp,*(qcdp.model))),LineColor(linecolorEWK));
  apdfMetp.plotOn(awmpframe,Components(RooArgSet(*(qcdp.model))),FillColor(fillcolorQCD),DrawOption("F"));
  apdfMetp.plotOn(awmpframe,Components(RooArgSet(*(qcdp.model))),LineColor(linecolorQCD));
  apdfMetp.plotOn(awmpframe,Components(RooArgSet(apdfWmp)),LineColor(linecolorW),LineStyle(2));
  antiMetp.plotOn(awmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hAntiDataMetp->GetBinWidth(1));
  CPlot plotAntiMetp("fitantimetp",awmpframe,"","",ylabel);
  plotAntiMetp.SetLegend(0.6,0.57,0.93,0.77);
  plotAntiMetp.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotAntiMetp.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nAntiSigp.getVal()/hAntiDataMetp->Integral()*100),"F");
  plotAntiMetp.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nAntiEWKp.getVal()/hAntiDataMetp->Integral()*100),"F");
  plotAntiMetp.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nAntiQCDp.getVal()/hAntiDataMetp->Integral()*100),"F");
  plotAntiMetp.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotAntiMetp.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotAntiMetp.SetYRange(0.1,1.1*(hAntiDataMetp->GetMaximum()));
  plotAntiMetp.Draw(c,kFALSE,format1,1);

  CPlot plotAntiMetpDiff("fitantimetp","",Form("%s [GeV]",WMass::FitVar_str[pTmTmet].Data()),"#chi");
  plotAntiMetpDiff.AddHist1D(hAntiMetpDiff,"EX0",ratioColor);
  plotAntiMetpDiff.SetYRange(-8,8);
  plotAntiMetpDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotAntiMetpDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotAntiMetpDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotAntiMetpDiff.Draw(c,kTRUE,format1,2);
  
  plotAntiMetp.SetName("fitantimetplog");
  plotAntiMetp.SetLogy();
  plotAntiMetp.SetYRange(1e-3*(hAntiDataMetp->GetMaximum()),10*(hAntiDataMetp->GetMaximum()));
  plotAntiMetp.Draw(c,kTRUE,format1,1);
  
  //
  // W- MET plot
  //
  RooPlot *wmmframe = pTmTmet_xvar.frame(Bins(NBINS)); 
  dataMetm.plotOn(wmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMetm.plotOn(wmmframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMetm.plotOn(wmmframe,LineColor(linecolorW));
  pdfMetm.plotOn(wmmframe,Components(RooArgSet(pdfEWKm,*(qcdm.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMetm.plotOn(wmmframe,Components(RooArgSet(pdfEWKm,*(qcdm.model))),LineColor(linecolorEWK));
  pdfMetm.plotOn(wmmframe,Components(RooArgSet(*(qcdm.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMetm.plotOn(wmmframe,Components(RooArgSet(*(qcdm.model))),LineColor(linecolorQCD));
  pdfMetm.plotOn(wmmframe,Components(RooArgSet(pdfWmm)),LineColor(linecolorW),LineStyle(2));
  dataMetm.plotOn(wmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMetm->GetBinWidth(1));
  CPlot plotMetm("fitmetm",wmmframe,"","",ylabel);
  plotMetm.SetLegend(0.6,0.57,0.93,0.77);
  plotMetm.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMetm.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nSigm.getVal()/hDataMetm->Integral()*100),"F");
  plotMetm.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nEWKm.getVal()/hDataMetm->Integral()*100),"F");
  plotMetm.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nQCDm.getVal()/hDataMetm->Integral()*100),"F");
  plotMetm.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMetm.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMetm.SetYRange(0.1,1.1*(hDataMetm->GetMaximum()));
  plotMetm.Draw(c,kFALSE,format1,1);

  CPlot plotMetmDiff("fitmetm","",Form("%s [GeV]",WMass::FitVar_str[pTmTmet].Data()),"#chi");
  plotMetmDiff.AddHist1D(hMetmDiff,"EX0",ratioColor);
  plotMetmDiff.SetYRange(-8,8);
  plotMetmDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotMetmDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotMetmDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotMetmDiff.Draw(c,kTRUE,format1,2);
  
  plotMetm.SetName("fitmetmlog");
  plotMetm.SetLogy();
  plotMetm.SetYRange(1e-3*(hDataMetm->GetMaximum()),10*(hDataMetm->GetMaximum()));
  plotMetm.Draw(c,kTRUE,format1,1);

  RooPlot *awmmframe = pTmTmet_xvar.frame(Bins(NBINS)); 
  antiMetm.plotOn(awmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  apdfMetm.plotOn(awmmframe,FillColor(fillcolorW),DrawOption("F"));
  apdfMetm.plotOn(awmmframe,LineColor(linecolorW));
  apdfMetm.plotOn(awmmframe,Components(RooArgSet(apdfEWKm,*(qcdm.model))),FillColor(fillcolorEWK),DrawOption("F"));
  apdfMetm.plotOn(awmmframe,Components(RooArgSet(apdfEWKm,*(qcdm.model))),LineColor(linecolorEWK));
  apdfMetm.plotOn(awmmframe,Components(RooArgSet(*(qcdm.model))),FillColor(fillcolorQCD),DrawOption("F"));
  apdfMetm.plotOn(awmmframe,Components(RooArgSet(*(qcdm.model))),LineColor(linecolorQCD));
  apdfMetm.plotOn(awmmframe,Components(RooArgSet(apdfWmm)),LineColor(linecolorW),LineStyle(2));
  antiMetm.plotOn(awmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMetm->GetBinWidth(1));
  CPlot plotAntiMetm("fitantimetm",awmmframe,"","",ylabel);
  plotAntiMetm.SetLegend(0.6,0.57,0.93,0.77);
  plotAntiMetm.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotAntiMetm.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nAntiSigm.getVal()/hAntiDataMetm->Integral()*100),"F");
  plotAntiMetm.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nAntiEWKm.getVal()/hAntiDataMetm->Integral()*100),"F");
  plotAntiMetm.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nAntiQCDm.getVal()/hAntiDataMetm->Integral()*100),"F");
  plotAntiMetm.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotAntiMetm.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotAntiMetm.SetYRange(0.1,1.1*(hAntiDataMetm->GetMaximum()));
  plotAntiMetm.Draw(c,kFALSE,format1,1);

  CPlot plotAntiMetmDiff("fitantimetm","",Form("%s [GeV]",WMass::FitVar_str[pTmTmet].Data()),"#chi");
  plotAntiMetmDiff.AddHist1D(hAntiMetmDiff,"EX0",ratioColor);
  plotAntiMetmDiff.SetYRange(-8,8);
  plotAntiMetmDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotAntiMetmDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotAntiMetmDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotAntiMetmDiff.Draw(c,kTRUE,format1,2);
  
  plotAntiMetm.SetName("fitantimetmlog");
  plotAntiMetm.SetLogy();
  plotAntiMetm.SetYRange(1e-3*(hAntiDataMetm->GetMaximum()),10*(hAntiDataMetm->GetMaximum()));
  plotAntiMetm.Draw(c,kTRUE,format1,1);

  
  //--------------------------------------------------------------------------------------------------------------
  // Output
  //==============================================================================================================
  
  cout << "*" << endl;
  cout << "* SUMMARY" << endl;
  cout << "*--------------------------------------------------" << endl;  
  
  //
  // Write fit results
  //
  ofstream txtfile;
  char txtfname[100];    
  
  ios_base::fmtflags flags;
  
  Double_t chi2prob, chi2ndf;
  Double_t ksprob, ksprobpe;
  
  // chi2prob = hDataMet->Chi2Test(hPdfMet,"PUW");
  // chi2ndf  = hDataMet->Chi2Test(hPdfMet,"CHI2/NDFUW");
  // ksprob   = hDataMet->KolmogorovTest(hPdfMet);
  // ksprobpe = hDataMet->KolmogorovTest(hPdfMet,"DX");
  // sprintf(txtfname,"%s/fitresWm.txt",CPlot::sOutDir.Data());
  // txtfile.open(txtfname);
  // assert(txtfile.is_open());
  
  // flags = txtfile.flags();
  // txtfile << setprecision(10);
  // txtfile << " *** Yields *** " << endl;
  // txtfile << "Selected: " << hDataMet->Integral() << endl;
  // txtfile << "  Signal: " << nSig.getVal() << " +/- " << nSig.getPropagatedError(*fitRes) << endl;
  // txtfile << "     QCD: " << nQCD.getVal() << " +/- " << nQCD.getPropagatedError(*fitRes) << endl;
  // txtfile << "   Other: " << nEWK.getVal() << " +/- " << nEWK.getPropagatedError(*fitRes) << endl;
  // txtfile << endl;
  // txtfile.flags(flags);
  
  // fitRes->printStream(txtfile,RooPrintable::kValue,RooPrintable::kVerbose);
  // txtfile << endl;
  // printCorrelations(txtfile, fitRes);
  // txtfile << endl;
  // printChi2AndKSResults(txtfile, chi2prob, chi2ndf, ksprob, ksprobpe);
  // txtfile.close();
  
  chi2prob = hDataMetp->Chi2Test(hPdfMetp,"PUW");
  chi2ndf  = hDataMetp->Chi2Test(hPdfMetp,"CHI2/NDFUW");
  ksprob   = hDataMetp->KolmogorovTest(hPdfMetp);
  ksprobpe = hDataMetp->KolmogorovTest(hPdfMetp,"DX");  
  sprintf(txtfname,"%s/fitresWmp.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  flags = txtfile.flags();
  txtfile << setprecision(10);
  txtfile << " *** Yields *** " << endl;
  txtfile << "Selected: " << hDataMetp->Integral() << endl;
  txtfile << "  Signal: " << nSigp.getVal() << " +/- " << nSigp.getPropagatedError(*fitResp) << endl;
  txtfile << "     QCD: " << nQCDp.getVal() << " +/- " << nQCDp.getPropagatedError(*fitResp) << endl;
  txtfile << "   Other: " << nEWKp.getVal() << " +/- " << nEWKp.getPropagatedError(*fitResp) << endl;
  txtfile << endl; 
  txtfile.flags(flags);
  
  fitResp->printStream(txtfile,RooPrintable::kValue,RooPrintable::kVerbose);
  txtfile << endl;
  printCorrelations(txtfile, fitResp);
  txtfile << endl;
  printChi2AndKSResults(txtfile, chi2prob, chi2ndf, ksprob, ksprobpe);
  txtfile.close();

  chi2prob = hDataMetm->Chi2Test(hPdfMetm,"PUW");
  chi2ndf  = hDataMetm->Chi2Test(hPdfMetm,"CHI2/NDFUW");
  ksprob   = hDataMetm->KolmogorovTest(hPdfMetm);
  ksprobpe = hDataMetm->KolmogorovTest(hPdfMetm,"DX");  
  sprintf(txtfname,"%s/fitresWmm.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  flags = txtfile.flags();
  txtfile << setprecision(10);
  txtfile << " *** Yields *** " << endl;
  txtfile << "Selected: " << hDataMetm->Integral() << endl;
  txtfile << "  Signal: " << nSigm.getVal() << " +/- " << nSigm.getPropagatedError(*fitResm) << endl;
  txtfile << "     QCD: " << nQCDm.getVal() << " +/- " << nQCDm.getPropagatedError(*fitResm) << endl;
  txtfile << "   Other: " << nEWKm.getVal() << " +/- " << nEWKm.getPropagatedError(*fitResm) << endl;
  txtfile << endl;
  txtfile.flags(flags);
  
  fitResm->printStream(txtfile,RooPrintable::kValue,RooPrintable::kVerbose);
  txtfile << endl;
  printCorrelations(txtfile, fitResm);
  txtfile << endl;
  printChi2AndKSResults(txtfile, chi2prob, chi2ndf, ksprob, ksprobpe);
  txtfile.close();

  makeHTML(outputDir2);
  
  cout << endl;
  cout << "  <> Output saved in " << outputDir2 << "/" << endl;    
  cout << endl;     
  
  // TRYING TO SAVE QCD INTO FILE
  cout << "SAVING FIT HISTOS INTO FILE" << endl;    
  TFile*fout=new TFile(Form("%s/pfmet_SigBkg_FitResults.root",outputDir2.Data()),"RECREATE");
  fout->cd();
  // TH1D*hprova = (TH1D*) qcd.model->createHistogram("hprova","x",200);
  TH1D*hWneg_pfmet_QCDfromFit    = (TH1D*)qcdm.model->createHistogram("hWneg_pfmet_QCDfromFit",pTmTmet_xvar);
  hWneg_pfmet_QCDfromFit->SetName("hWneg_pfmet_QCDfromFit");
  hWneg_pfmet_QCDfromFit->Scale(nQCDm.getVal()/hWneg_pfmet_QCDfromFit->Integral());
  TH1D*hWneg_pfmet_SigFromFit    = (TH1D*)wmunuMetm.createHistogram("hWneg_pfmet_SigFromFit",pTmTmet_xvar);
  hWneg_pfmet_SigFromFit->SetName("hWneg_pfmet_SigFromFit");
  hWneg_pfmet_SigFromFit->Scale(nSigm.getVal()/hWneg_pfmet_SigFromFit->Integral());
  TH1D*hWneg_pfmet_EWKTTfromFit  = (TH1D*)ewkMetm.createHistogram("hWneg_pfmet_EWKTTfromFit",pTmTmet_xvar);
  hWneg_pfmet_EWKTTfromFit->SetName("hWneg_pfmet_EWKTTfromFit");
  hWneg_pfmet_EWKTTfromFit->Scale(nEWKm.getVal()/hWneg_pfmet_EWKTTfromFit->Integral());
  TH1D hWneg_pfmet_TotBKGfromFit    = (*hWneg_pfmet_EWKTTfromFit) + (*hWneg_pfmet_QCDfromFit);
  hWneg_pfmet_TotBKGfromFit.SetName("hWneg_pfmet_TotBKGfromFit");
  TH1D hWneg_pfmet_SigPlusBKGfromFit = (*hWneg_pfmet_SigFromFit) + (*hWneg_pfmet_EWKTTfromFit) + (*hWneg_pfmet_QCDfromFit);
  hWneg_pfmet_SigPlusBKGfromFit.SetName("hWneg_pfmet_SigPlusBKGfromFit");
  hWneg_pfmet_SigPlusBKGfromFit.Write();
  hWneg_pfmet_TotBKGfromFit.Write();
  hWneg_pfmet_QCDfromFit->Write();
  hWneg_pfmet_SigFromFit->Write();
  hWneg_pfmet_EWKTTfromFit->Write();
  hDataMetm->SetName("hWneg_pfmet_DATA");
  hDataMetm->Write();
  TH1D hWneg_pfmet_DATA_bkgSub = (*hDataMetm) - (hWneg_pfmet_TotBKGfromFit);
  hWneg_pfmet_DATA_bkgSub.SetName("hWneg_pfmet_DATA_bkgSub");
  hWneg_pfmet_DATA_bkgSub.Write();

  TH1D*hWneg_numbers=new TH1D("hWneg_numbers","hWneg_numbers",4,0.5,4.5);
  hWneg_numbers->Fill(1,hDataMetm->Integral());
  hWneg_numbers->Fill(2,nSigm.getVal());
  hWneg_numbers->Fill(3,nEWKm.getVal());
  hWneg_numbers->Fill(4,nQCDm.getVal());
  hWneg_numbers->Write();
  TH1D*hWneg_numbers_norm=new TH1D("hWneg_numbers_norm","hWneg_numbers_norm",4,0.5,4.5);
  hWneg_numbers_norm->Fill(1,hDataMetm->Integral()/hDataMetm->Integral());
  hWneg_numbers_norm->Fill(2,nSigm.getVal()/hDataMetm->Integral());
  hWneg_numbers_norm->Fill(3,nEWKm.getVal()/hDataMetm->Integral());
  hWneg_numbers_norm->Fill(4,nQCDm.getVal()/hDataMetm->Integral());
  hWneg_numbers_norm->Write();
  // rescale background fractions 
  TH1D*hWneg_numbers_norm_pfmet_gt_25=new TH1D("hWneg_numbers_norm_pfmet_gt_25","hWneg_numbers_norm_pfmet_gt_25",4,0.5,4.5);
  hWneg_numbers_norm_pfmet_gt_25->Fill(1,hDataMetm->Integral(hDataMetm->GetXaxis()->FindBin(25),-1)/hDataMetm->Integral(hDataMetm->GetXaxis()->FindBin(25),-1));
  hWneg_numbers_norm_pfmet_gt_25->Fill(2,hWneg_pfmet_SigFromFit->Integral(hWneg_pfmet_SigFromFit->GetXaxis()->FindBin(25),-1)/hDataMetm->Integral(hDataMetm->GetXaxis()->FindBin(25),-1));
  hWneg_numbers_norm_pfmet_gt_25->Fill(3,hWneg_pfmet_EWKTTfromFit->Integral(hWneg_pfmet_EWKTTfromFit->GetXaxis()->FindBin(25),-1)/hDataMetm->Integral(hDataMetm->GetXaxis()->FindBin(25),-1));
  hWneg_numbers_norm_pfmet_gt_25->Fill(4,hWneg_pfmet_QCDfromFit->Integral(hWneg_pfmet_QCDfromFit->GetXaxis()->FindBin(25),-1)/hDataMetm->Integral(hDataMetm->GetXaxis()->FindBin(25),-1));
  hWneg_numbers_norm_pfmet_gt_25->Write();
  // fout->Close();
  cout << "WNeg QCD PDF SAVED" << endl;    

  TH1D*hWpos_pfmet_QCDfromFit    = (TH1D*)qcdp.model->createHistogram("hWpos_pfmet_QCDfromFit",pTmTmet_xvar);
  hWpos_pfmet_QCDfromFit->SetName("hWpos_pfmet_QCDfromFit");
  hWpos_pfmet_QCDfromFit->Scale(nQCDp.getVal()/hWpos_pfmet_QCDfromFit->Integral());
  TH1D*hWpos_pfmet_SigFromFit    = (TH1D*)wmunuMetp.createHistogram("hWpos_pfmet_SigFromFit",pTmTmet_xvar);
  hWpos_pfmet_SigFromFit->SetName("hWpos_pfmet_SigFromFit");
  hWpos_pfmet_SigFromFit->Scale(nSigp.getVal()/hWpos_pfmet_SigFromFit->Integral());
  TH1D*hWpos_pfmet_EWKTTfromFit  = (TH1D*)ewkMetp.createHistogram("hWpos_pfmet_EWKTTfromFit",pTmTmet_xvar);
  hWpos_pfmet_EWKTTfromFit->SetName("hWpos_pfmet_EWKTTfromFit");
  hWpos_pfmet_EWKTTfromFit->Scale(nEWKp.getVal()/hWpos_pfmet_EWKTTfromFit->Integral());
  TH1D hWpos_pfmet_TotBKGfromFit    = (*hWpos_pfmet_EWKTTfromFit) + (*hWpos_pfmet_QCDfromFit);
  hWpos_pfmet_TotBKGfromFit.SetName("hWpos_pfmet_TotBKGfromFit");
  TH1D hWpos_pfmet_SigPlusBKGfromFit = (*hWpos_pfmet_SigFromFit) + (*hWpos_pfmet_EWKTTfromFit) + (*hWpos_pfmet_QCDfromFit);
  hWpos_pfmet_SigPlusBKGfromFit.SetName("hWpos_pfmet_SigPlusBKGfromFit");
  hWpos_pfmet_SigPlusBKGfromFit.Write();
  hWpos_pfmet_TotBKGfromFit.Write();
  hWpos_pfmet_QCDfromFit->Write();
  hWpos_pfmet_SigFromFit->Write();
  hWpos_pfmet_EWKTTfromFit->Write();
  hDataMetp->SetName("hWpos_pfmet_DATA");
  hDataMetp->Write();
  TH1D hWpos_pfmet_DATA_bkgSub = (*hDataMetp) - (hWpos_pfmet_TotBKGfromFit);
  hWpos_pfmet_DATA_bkgSub.SetName("hWpos_pfmet_DATA_bkgSub");
  hWpos_pfmet_DATA_bkgSub.Write();

  TH1D*hWpos_numbers=new TH1D("hWpos_numbers","hWpos_numbers",4,0.5,4.5);
  hWpos_numbers->Fill(1,hDataMetp->Integral());
  hWpos_numbers->Fill(2,nSigp.getVal());
  hWpos_numbers->Fill(3,nEWKp.getVal());
  hWpos_numbers->Fill(4,nQCDp.getVal());
  hWpos_numbers->Write();
  TH1D*hWpos_numbers_norm=new TH1D("hWpos_numbers_norm","hWpos_numbers_norm",4,0.5,4.5);
  hWpos_numbers_norm->Fill(1,hDataMetp->Integral()/hDataMetp->Integral());
  hWpos_numbers_norm->Fill(2,nSigp.getVal()/hDataMetp->Integral());
  hWpos_numbers_norm->Fill(3,nEWKp.getVal()/hDataMetp->Integral());
  hWpos_numbers_norm->Fill(4,nQCDp.getVal()/hDataMetp->Integral());
  hWpos_numbers_norm->Write();
  // rescale background fractions 
  TH1D*hWpos_numbers_norm_pfmet_gt_25=new TH1D("hWpos_numbers_norm_pfmet_gt_25","hWpos_numbers_norm_pfmet_gt_25",4,0.5,4.5);
  hWpos_numbers_norm_pfmet_gt_25->Fill(1,hDataMetp->Integral(hDataMetp->GetXaxis()->FindBin(25),-1)/hDataMetp->Integral(hDataMetp->GetXaxis()->FindBin(25),-1));
  hWpos_numbers_norm_pfmet_gt_25->Fill(2,hWpos_pfmet_SigFromFit->Integral(hWpos_pfmet_SigFromFit->GetXaxis()->FindBin(25),-1)/hDataMetp->Integral(hDataMetp->GetXaxis()->FindBin(25),-1));
  hWpos_numbers_norm_pfmet_gt_25->Fill(3,hWpos_pfmet_EWKTTfromFit->Integral(hWpos_pfmet_EWKTTfromFit->GetXaxis()->FindBin(25),-1)/hDataMetp->Integral(hDataMetp->GetXaxis()->FindBin(25),-1));
  hWpos_numbers_norm_pfmet_gt_25->Fill(4,hWpos_pfmet_QCDfromFit->Integral(hWpos_pfmet_QCDfromFit->GetXaxis()->FindBin(25),-1)/hDataMetp->Integral(hDataMetp->GetXaxis()->FindBin(25),-1));
  hWpos_numbers_norm_pfmet_gt_25->Write();
  fout->Write();
  // fout->Close();
  cout << "WPos QCD PDF SAVED" << endl;    

  
  // gBenchmark->Show("fitWm");
}


//=== FUNCTION DEFINITIONS ======================================================================================

//--------------------------------------------------------------------------------------------------
TH1D *makeDiffHist(TH1D* hData, TH1D* hFit, const TString name)
{
  TH1D *hDiff = new TH1D(name,"",hData->GetNbinsX(),hData->GetXaxis()->GetXmin(),hData->GetXaxis()->GetXmax());
  for(Int_t ibin=1; ibin<=hData->GetNbinsX(); ibin++) {
    
    Double_t diff = (hData->GetBinContent(ibin)-hFit->GetBinContent(ibin));
    
    Double_t err = sqrt(hData->GetBinContent(ibin));
    if(err==0) err= sqrt(hFit->GetBinContent(ibin));
    
    if(err>0) hDiff->SetBinContent(ibin,diff/err);
    else      hDiff->SetBinContent(ibin,0);
    hDiff->SetBinError(ibin,1);   
  }
  
  hDiff->GetYaxis()->SetTitleOffset(0.48);
  hDiff->GetYaxis()->SetTitleSize(0.13);
  hDiff->GetYaxis()->SetLabelSize(0.10);
  hDiff->GetYaxis()->SetNdivisions(104);
  hDiff->GetYaxis()->CenterTitle();
  hDiff->GetXaxis()->SetTitleOffset(1.2);
  hDiff->GetXaxis()->SetTitleSize(0.13);
  hDiff->GetXaxis()->SetLabelSize(0.12);
  hDiff->GetXaxis()->CenterTitle();
  
  return hDiff;
}

//--------------------------------------------------------------------------------------------------
void printCorrelations(ostream& os, RooFitResult *res)
{
  ios_base::fmtflags flags = os.flags();
  const RooArgList parlist = res->floatParsFinal();
  
  os << "  Correlation Matrix" << endl;
  os << " --------------------" << endl;
  for(Int_t i=0; i<parlist.getSize(); i++) {
    for(Int_t j=0; j<parlist.getSize(); j++) 
    os << "  " << setw(7) << setprecision(4) << fixed << res->correlationMatrix()(i,j);    
    os << endl;
  }
  os.flags(flags);
}

//--------------------------------------------------------------------------------------------------
void printChi2AndKSResults(ostream& os, 
const Double_t chi2prob, const Double_t chi2ndf, 
const Double_t ksprob, const Double_t ksprobpe)
{
  ios_base::fmtflags flags = os.flags();
  
  os << "  Chi2 Test" << endl;
  os << " -----------" << endl;
  os << "       prob = " << chi2prob << endl;
  os << "   chi2/ndf = " << chi2ndf << endl;
  os << endl;
  os << "  KS Test" << endl;
  os << " ---------" << endl;
  os << "   prob = " << ksprob << endl;
  os << "   prob = " << ksprobpe << " with 1000 pseudo-experiments" << endl;
  os << endl;

  os.flags(flags);
}

//--------------------------------------------------------------------------------------------------
void makeHTML(const TString outDir)
{
  ofstream htmlfile;
  char htmlfname[100];
  sprintf(htmlfname,"%s/WmunuFitPlots.html",outDir.Data());
  htmlfile.open(htmlfname);
  htmlfile << "<!DOCTYPE html" << endl;
  htmlfile << "    PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << endl;
  htmlfile << "<html>" << endl;
  htmlfile << "<head><title>Wmunu</title></head>" << endl;
  htmlfile << "<body bgcolor=\"EEEEEE\">" << endl;

  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl; 
  htmlfile << "<tr>" << endl;
  // htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmet.png\"><img src=\"fitmet.png\" alt=\"fitmet.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetp.png\"><img src=\"fitmetp.png\" alt=\"fitmetp.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetm.png\"><img src=\"fitmetm.png\" alt=\"fitmetm.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  // htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetlog.png\"><img src=\"fitmetlog.png\" alt=\"fitmetlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetplog.png\"><img src=\"fitmetplog.png\" alt=\"fitmetplog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetmlog.png\"><img src=\"fitmetmlog.png\" alt=\"fitmetmlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  // htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimet.png\"><img src=\"fitantimet.png\" alt=\"fitantimet.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimetp.png\"><img src=\"fitantimetp.png\" alt=\"fitantimetp.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimetm.png\"><img src=\"fitantimetm.png\" alt=\"fitantimetm.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  // htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimetlog.png\"><img src=\"fitantimetlog.png\" alt=\"fitantimetlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimetplog.png\"><img src=\"fitantimetplog.png\" alt=\"fitantimetplog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimetmlog.png\"><img src=\"fitantimetmlog.png\" alt=\"fitantimetmlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "</table>" << endl;
  htmlfile << "<hr />" << endl;
  
  htmlfile << "</body>" << endl;
  htmlfile << "</html>" << endl;
  htmlfile.close();  
}
