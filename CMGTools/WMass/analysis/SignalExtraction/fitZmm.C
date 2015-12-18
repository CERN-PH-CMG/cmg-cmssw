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
// #include "Utils/RecoilCorrector.hh"    // class to handle recoil corrections for MET

#include "Utils/ZSignals.hh"           // define models for Z signal PDFs
#include "Utils/ZBackgrounds.hh"       // define models for background PDFs

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

void fitZmm(const TString  outputDir="test",   // output directory
const Double_t lumi=4.75,        // integrated luminosity (/fb)
const Int_t    Ecm=7         // center-of-mass energy
) {
  gBenchmark->Start("fitZmm");
  
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
    
  // cout << gSystem->WorkingDirectory() << endl;
  // cout << Form("../JobOutputs/%s/output_DATA/ZanalysisOnDATA.root",outputDir.Data()) << endl;
  TFile*fData      = new TFile(Form("../JobOutputs/%s/output_DATA/ZanalysisOnDATA.root",outputDir.Data()));
  TFile*fZmm     = new TFile(Form("../JobOutputs/%s/output_DYJetsSig/ZanalysisOnDATA.root",outputDir.Data()));
  TFile*fEWKTT       = new TFile(Form("../JobOutputs/%s/output_EWKTT/ZanalysisOnDATA.root",outputDir.Data()));
  TH1D *hDataMass   = (TH1D*)fData->Get("Zmass_Sig_eta1p1_80419"); 
  TH1D *hDataMassm  = (TH1D*)hDataMass;  
  TH1D *hDataMassp  = (TH1D*)hDataMass;
  hDataMass->Print();
  TH1D *hZmmMass  = (TH1D*)fZmm->Get("Zmass_Sig_eta1p1_80419"); 
  TH1D *hZmmMassp = (TH1D*)hZmmMass;
  TH1D *hZmmMassm = (TH1D*)hZmmMass;
  hZmmMass->Print();
  TH1D *hEWKMass    = (TH1D*)fEWKTT->Get("Zmass_Sig_eta1p1_80419"); 
  TH1D *hEWKMassp   = (TH1D*)hEWKMass;
  TH1D *hEWKMassm   = (TH1D*)hEWKMass;
  hEWKMass->Print();

  TH1D *hAntiDataMass   = (TH1D*)fData->Get("Zmass_QCD_eta1p1_80419"); 
  TH1D *hAntiDataMassm  = (TH1D*)hAntiDataMass;
  TH1D *hAntiDataMassp  = (TH1D*)hAntiDataMass;
  hAntiDataMass->Print();
  TH1D *hAntiZmmMass  = (TH1D*)fZmm->Get("Zmass_QCD_eta1p1_80419"); 
  TH1D *hAntiZmmMassp = (TH1D*)hAntiZmmMass;
  TH1D *hAntiZmmMassm = (TH1D*)hAntiZmmMass;
  hAntiZmmMass->Print();
  TH1D *hAntiEWKMass    = (TH1D*)fEWKTT->Get("Zmass_QCD_eta1p1_80419"); 
  TH1D *hAntiEWKMassp   = (TH1D*)hAntiEWKMass; 
  TH1D *hAntiEWKMassm   = (TH1D*)hAntiEWKMass; 
  hAntiEWKMass->Print();
  
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
  RooRealVar nSig("nSig","nSig",0.7*(hDataMass->Integral()),0,hDataMass->Integral());
  RooRealVar nQCD("nQCD","nQCD",0.3*(hDataMass->Integral()),0,hDataMass->Integral());
  RooRealVar cewk("cewk","cewk",0.1,0,5) ;
  cewk.setVal(hEWKMass->Integral()/hZmmMass->Integral());
  cewk.setConstant(kTRUE);
  RooFormulaVar nEWK("nEWK","nEWK","cewk*nSig",RooArgList(nSig,cewk));
  RooRealVar nAntiSig("nAntiSig","nAntiSig",0.05*(hAntiDataMass->Integral()),0,hAntiDataMass->Integral());
  RooRealVar nAntiQCD("nAntiQCD","nAntiQCD",0.9*(hDataMass->Integral()),0,hDataMass->Integral());
  RooRealVar dewk("dewk","dewk",0.1,0,5) ;
  dewk.setVal(hAntiEWKMass->Integral()/hAntiZmmMass->Integral());
  dewk.setConstant(kTRUE);
  RooFormulaVar nAntiEWK("nAntiEWK","nAntiEWK","dewk*nAntiSig",RooArgList(nAntiSig,dewk));
  
  RooRealVar nSigp("nSigp","nSigp",0.7*(hDataMassp->Integral()),0,hDataMassp->Integral());
  RooRealVar nQCDp("nQCDp","nQCDp",0.3*(hDataMassp->Integral()),0,hDataMassp->Integral());
  RooRealVar cewkp("cewkp","cewkp",0.1,0,5) ;
  cewkp.setVal(hEWKMassp->Integral()/hZmmMassp->Integral());
  cewkp.setConstant(kTRUE);
  RooFormulaVar nEWKp("nEWKp","nEWKp","cewkp*nSigp",RooArgList(nSigp,cewkp));
  RooRealVar nAntiSigp("nAntiSigp","nAntiSigp",0.05*(hAntiDataMassp->Integral()),0,hAntiDataMassp->Integral());
  RooRealVar nAntiQCDp("nAntiQCDp","nAntiQCDp",0.9*(hAntiDataMassp->Integral()),0,hAntiDataMassp->Integral());
  RooRealVar dewkp("dewkp","dewkp",0.1,0,5) ;
  dewkp.setVal(hAntiEWKMassp->Integral()/hAntiZmmMassp->Integral());
  dewkp.setConstant(kTRUE);
  RooFormulaVar nAntiEWKp("nAntiEWKp","nAntiEWKp","dewkp*nAntiSigp",RooArgList(nAntiSigp,dewkp));
  
  RooRealVar nSigm("nSigm","nSigm",0.7*(hDataMassm->Integral()),0,hDataMassm->Integral());
  RooRealVar nQCDm("nQCDm","nQCDm",0.3*(hDataMassm->Integral()),0,hDataMassm->Integral());
  RooRealVar cewkm("cewkm","cewkm",0.1,0,5) ;
  cewkm.setVal(hEWKMassm->Integral()/hZmmMassm->Integral());
  cewkm.setConstant(kTRUE);
  RooFormulaVar nEWKm("nEWKm","nEWKm","cewkm*nSigm",RooArgList(nSigm,cewkm));  
  RooRealVar nAntiSigm("nAntiSigm","nAntiSigm",0.05*(hAntiDataMassm->Integral()),0,hAntiDataMassm->Integral());
  RooRealVar nAntiQCDm("nAntiQCDm","nAntiQCDm",0.9*(hAntiDataMassm->Integral()),0,hAntiDataMassm->Integral());
  RooRealVar dewkm("dewkm","dewkm",0.1,0,5) ;
  dewkm.setVal(hAntiEWKMassm->Integral()/hAntiZmmMassm->Integral());
  dewkm.setConstant(kTRUE);
  RooFormulaVar nAntiEWKm("nAntiEWKm","nAntiEWKm","dewkm*nAntiSigm",RooArgList(nAntiSigm,dewkm));


  //
  // Construct PDFs for fitting
  //
  double METMAX = hDataMass->GetXaxis()->GetXmax();
  int NBINS = hDataMass->GetNbinsX();
  RooRealVar zmass("zmass","zmass",hDataMass->GetXaxis()->GetXmin(),METMAX);
  zmass.setBins(NBINS);
  
  // Signal PDFs
  RooDataHist ZmmMass ("ZmmMET", "ZmmMET", RooArgSet(zmass),hZmmMass);  RooHistPdf pdfWm ("wm", "wm", zmass,ZmmMass, 1);
  RooDataHist ZmmMassp("ZmmMETp","ZmmMETp",RooArgSet(zmass),hZmmMassp); RooHistPdf pdfWmp("wmp","wmp",zmass,ZmmMassp,1);
  RooDataHist ZmmMassm("ZmmMETm","ZmmMETm",RooArgSet(zmass),hZmmMassm); RooHistPdf pdfWmm("wmm","wmm",zmass,ZmmMassm,1); 
  
  // EWK+top PDFs
  RooDataHist ewkMass ("ewkMET", "ewkMET", RooArgSet(zmass),hEWKMass);  RooHistPdf pdfEWK ("ewk", "ewk", zmass,ewkMass, 1);
  RooDataHist ewkMassp("ewkMETp","ewkMETp",RooArgSet(zmass),hEWKMassp); RooHistPdf pdfEWKp("ewkp","ewkp",zmass,ewkMassp,1); 
  RooDataHist ewkMassm("ewkMETm","ewkMETm",RooArgSet(zmass),hEWKMassm); RooHistPdf pdfEWKm("ewkm","ewkm",zmass,ewkMassm,1); 
  
  // QCD Pdfs
  // CLucaModel0 qcd("qcd",zmass);
  // CPepeModel0 qcdp("qcdp",zmass);
  // CPepeModel1 qcdm("qcdm",zmass);
  CExponential qcd("qcd",zmass);
  CErfExpo qcdp("qcdp",zmass);
  CDoubleExp qcdm("qcdm",zmass);
  
  // Signal + Background PDFs
  RooAddPdf pdfMass ("pdfMass", "pdfMass", RooArgList(pdfWm,pdfEWK,*(qcd.model)),   RooArgList(nSig,nEWK,nQCD));  
  RooAddPdf pdfMassp("pdfMassp","pdfMassp",RooArgList(pdfWmp,pdfEWKp,*(qcdp.model)),RooArgList(nSigp,nEWKp,nQCDp));
  RooAddPdf pdfMassm("pdfMassm","pdfMassm",RooArgList(pdfWmm,pdfEWKm,*(qcdm.model)),RooArgList(nSigm,nEWKm,nQCDm));
  
  
  // Anti-Signal PDFs
  RooDataHist aZmmMass ("aZmmMET", "aZmmMET", RooArgSet(zmass),hAntiZmmMass);  RooHistPdf apdfWm ("awm", "awm", zmass,aZmmMass, 1);
  RooDataHist aZmmMassp("aZmmMETp","aZmmMETp",RooArgSet(zmass),hAntiZmmMassp); RooHistPdf apdfWmp("awmp","awmp",zmass,aZmmMassp,1);
  RooDataHist aZmmMassm("aZmmMETm","aZmmMETm",RooArgSet(zmass),hAntiZmmMassm); RooHistPdf apdfWmm("awmm","awmm",zmass,aZmmMassm,1); 
  
  // Anti-EWK+top PDFs
  RooDataHist aewkMass ("aewkMET", "aewkMET", RooArgSet(zmass),hAntiEWKMass);  RooHistPdf apdfEWK ("aewk", "aewk", zmass,aewkMass, 1);
  RooDataHist aewkMassp("aewkMETp","aewkMETp",RooArgSet(zmass),hAntiEWKMassp); RooHistPdf apdfEWKp("aewkp","aewkp",zmass,aewkMassp,1); 
  RooDataHist aewkMassm("aewkMETm","aewkMETm",RooArgSet(zmass),hAntiEWKMassm); RooHistPdf apdfEWKm("aewkm","aewkm",zmass,aewkMassm,1); 
  
  // Anti-selection PDFs
  RooAddPdf apdfMass ("apdfMass", "apdfMass", RooArgList(apdfWm,apdfEWK,*(qcd.model)),   RooArgList(nAntiSig,nAntiEWK,nAntiQCD));  
  RooAddPdf apdfMassp("apdfMassp","apdfMassp",RooArgList(apdfWmp,apdfEWKp,*(qcdp.model)),RooArgList(nAntiSigp,nAntiEWKp,nAntiQCDp));
  RooAddPdf apdfMassm("apdfMassm","apdfMassm",RooArgList(apdfWmm,apdfEWKm,*(qcdm.model)),RooArgList(nAntiSigm,nAntiEWKm,nAntiQCDm));
  
  // PDF for simultaneous fit
  RooCategory rooCat("rooCat","rooCat");
  rooCat.defineType("Select");
  rooCat.defineType("Anti");
  
  RooSimultaneous pdfTotal("pdfTotal","pdfTotal",rooCat);
  pdfTotal.addPdf(pdfMass, "Select");
  pdfTotal.addPdf(apdfMass,"Anti");
  
  RooSimultaneous pdfTotalp("pdfTotalp","pdfTotalp",rooCat);
  pdfTotalp.addPdf(pdfMassp, "Select");
  pdfTotalp.addPdf(apdfMassp,"Anti");
  
  RooSimultaneous pdfTotalm("pdfTotalm","pdfTotalm",rooCat);
  pdfTotalm.addPdf(pdfMassm, "Select");
  pdfTotalm.addPdf(apdfMassm,"Anti");
  
  //
  // Perform fits
  //

  RooDataHist dataMass("dataMass", "dataMass", RooArgSet(zmass), hDataMass);
  RooDataHist antiMass("antiMass", "antiMass", RooArgSet(zmass), hAntiDataMass);
  RooDataHist dataTotal("dataTotal","dataTotal", RooArgList(zmass), Index(rooCat),
  Import("Select", dataMass),
  Import("Anti",   antiMass));
  RooFitResult *fitRes = pdfTotal.fitTo(dataTotal,Extended(),Minos(kTRUE),Save(kTRUE));
  
  RooDataHist dataMassp("dataMassp", "dataMassp", RooArgSet(zmass), hDataMassp);
  RooDataHist antiMassp("antiMassp", "antiMassp", RooArgSet(zmass), hAntiDataMassp);
  RooDataHist dataTotalp("dataTotalp","dataTotalp", RooArgList(zmass), Index(rooCat),
  Import("Select", dataMassp),
  Import("Anti",   antiMassp));
  RooFitResult *fitResp = pdfTotalp.fitTo(dataTotalp,Extended(),Minos(kTRUE),Save(kTRUE));
  
  RooDataHist dataMassm("dataMassm", "dataMassm", RooArgSet(zmass), hDataMassm);
  RooDataHist antiMassm("antiMassm", "antiMassm", RooArgSet(zmass), hAntiDataMassm);
  RooDataHist dataTotalm("dataTotalm","dataTotalm", RooArgList(zmass), Index(rooCat),
  Import("Select", dataMassm),
  Import("Anti",   antiMassm));
  RooFitResult *fitResm = pdfTotalm.fitTo(dataTotalm,Extended(),Minos(kTRUE),Save(kTRUE));
  
  //
  // Use histogram version of fitted PDFs to make ratio plots
  // (Will also use PDF histograms later for Chi^2 and KS tests)
  //
  TH1D *hPdfMass = (TH1D*)(pdfMass.createHistogram("hPdfMass", zmass));
  hPdfMass->Scale((nSig.getVal()+nEWK.getVal()+nQCD.getVal())/hPdfMass->Integral());
  TH1D *hMassDiff = makeDiffHist(hDataMass,hPdfMass,"hMassDiff");
  hMassDiff->SetMarkerStyle(kFullCircle);
  hMassDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfMassp = (TH1D*)(pdfMassp.createHistogram("hPdfMassp", zmass));
  hPdfMassp->Scale((nSigp.getVal()+nEWKp.getVal()+nQCDp.getVal())/hPdfMassp->Integral());
  TH1D *hMasspDiff = makeDiffHist(hDataMassp,hPdfMassp,"hMasspDiff");
  hMasspDiff->SetMarkerStyle(kFullCircle);
  hMasspDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfMassm = (TH1D*)(pdfMassm.createHistogram("hPdfMassm", zmass));
  hPdfMassm->Scale((nSigm.getVal()+nEWKm.getVal()+nQCDm.getVal())/hPdfMassm->Integral());
  TH1D *hMassmDiff = makeDiffHist(hDataMassm,hPdfMassm,"hMassmDiff");
  hMassmDiff->SetMarkerStyle(kFullCircle); 
  hMassmDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfAntiMass = (TH1D*)(apdfMass.createHistogram("hPdfAntiMass", zmass));
  hPdfMass->Scale((nAntiSig.getVal()+nAntiEWK.getVal()+nAntiQCD.getVal())/hPdfAntiMass->Integral());
  TH1D *hAntiMassDiff = makeDiffHist(hAntiDataMass,hPdfAntiMass,"hAntiMassDiff");
  hAntiMassDiff->SetMarkerStyle(kFullCircle);
  hAntiMassDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfAntiMassp = (TH1D*)(apdfMassp.createHistogram("hPdfAntiMassp", zmass));
  hPdfAntiMassp->Scale((nAntiSigp.getVal()+nAntiEWKp.getVal()+nAntiQCDp.getVal())/hPdfAntiMassp->Integral());
  TH1D *hAntiMasspDiff = makeDiffHist(hAntiDataMassp,hPdfAntiMassp,"hAntiMasspDiff");
  hAntiMasspDiff->SetMarkerStyle(kFullCircle);
  hAntiMasspDiff->SetMarkerSize(0.9);
  
  TH1D *hPdfAntiMassm = (TH1D*)(apdfMassm.createHistogram("hPdfAntiMassm", zmass));
  hPdfAntiMassm->Scale((nAntiSigm.getVal()+nAntiEWKm.getVal()+nAntiQCDm.getVal())/hPdfAntiMassm->Integral());
  TH1D *hAntiMassmDiff = makeDiffHist(hAntiDataMassm,hPdfAntiMassm,"hAntiMassmDiff");
  hAntiMassmDiff->SetMarkerStyle(kFullCircle); 
  hAntiMassmDiff->SetMarkerSize(0.9);
  
  
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
  RooPlot *wmframe = zmass.frame(Bins(NBINS));    
  dataMass.plotOn(wmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMass.plotOn(wmframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMass.plotOn(wmframe,LineColor(linecolorW));
  pdfMass.plotOn(wmframe,Components(RooArgSet(pdfEWK,*(qcd.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMass.plotOn(wmframe,Components(RooArgSet(pdfEWK,*(qcd.model))),LineColor(linecolorEWK));
  pdfMass.plotOn(wmframe,Components(RooArgSet(*(qcd.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMass.plotOn(wmframe,Components(RooArgSet(*(qcd.model))),LineColor(linecolorQCD));
  pdfMass.plotOn(wmframe,Components(RooArgSet(pdfWm)),LineColor(linecolorW),LineStyle(2));
  dataMass.plotOn(wmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMass->GetBinWidth(1));
  CPlot plotMass("fitmass",wmframe,"","",ylabel);
  plotMass.SetLegend(0.6,0.57,0.93,0.77);
  plotMass.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMass.GetLegend()->AddEntry(hDummyW,Form("W#rightarrow#mu#nu (%.1f\%)",nSig.getVal()/hDataMass->Integral()*100),"F");
  plotMass.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nEWK.getVal()/hDataMass->Integral()*100),"F");
  plotMass.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nQCD.getVal()/hDataMass->Integral()*100),"F");
  plotMass.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMass.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMass.SetYRange(0.1,1.1*(hDataMass->GetMaximum()));
  plotMass.Draw(c,kFALSE,format1,1);

  CPlot plotMassDiff("fitmass","","#slash{E}_{T} [GeV]","#chi");
  plotMassDiff.AddHist1D(hMassDiff,"EX0",ratioColor);
  plotMassDiff.SetYRange(-8,8);
  plotMassDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 0,METMAX, 0,kBlack,1);
  plotMassDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 5,METMAX, 5,kBlack,3);
  plotMassDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(),-5,METMAX,-5,kBlack,3);
  plotMassDiff.Draw(c,kTRUE,format1,2);
  
  plotMass.SetName("fitmasslog");
  plotMass.SetLogy();
  // plotMass.SetYRange(1e-3*(hDataMass->GetMaximum()),10*(hDataMass->GetMaximum()));
  plotMass.SetYRange(0.1,10*(hDataMass->GetMaximum()));
  plotMass.Draw(c,kTRUE,format1,1);
  
  RooPlot *awmframe = zmass.frame(Bins(NBINS));    
  antiMass.plotOn(awmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  apdfMass.plotOn(awmframe,FillColor(fillcolorW),DrawOption("F"));
  apdfMass.plotOn(awmframe,LineColor(linecolorW));
  apdfMass.plotOn(awmframe,Components(RooArgSet(apdfEWK,*(qcd.model))),FillColor(fillcolorEWK),DrawOption("F"));
  apdfMass.plotOn(awmframe,Components(RooArgSet(apdfEWK,*(qcd.model))),LineColor(linecolorEWK));
  apdfMass.plotOn(awmframe,Components(RooArgSet(*(qcd.model))),FillColor(fillcolorQCD),DrawOption("F"));
  apdfMass.plotOn(awmframe,Components(RooArgSet(*(qcd.model))),LineColor(linecolorQCD));
  apdfMass.plotOn(awmframe,Components(RooArgSet(apdfWm)),LineColor(linecolorW),LineStyle(2));
  antiMass.plotOn(awmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hAntiDataMass->GetBinWidth(1));
  CPlot plotAntiMass("fitantimass",awmframe,"","",ylabel);
  plotAntiMass.SetLegend(0.6,0.57,0.93,0.77);
  plotAntiMass.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotAntiMass.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nAntiSig.getVal()/hAntiDataMass->Integral()*100),"F");
  plotAntiMass.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nAntiEWK.getVal()/hAntiDataMass->Integral()*100),"F");
  plotAntiMass.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nAntiQCD.getVal()/hAntiDataMass->Integral()*100),"F");
  plotAntiMass.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotAntiMass.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotAntiMass.SetYRange(0.1,1.1*(hAntiDataMass->GetMaximum()));
  plotAntiMass.Draw(c,kFALSE,format1,1);

  CPlot plotAntiMassDiff("fitantimass","","#slash{E}_{T} [GeV]","#chi");
  plotAntiMassDiff.AddHist1D(hMassDiff,"EX0",ratioColor);
  plotAntiMassDiff.SetYRange(-8,8);
  plotAntiMassDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 0,METMAX, 0,kBlack,1);
  plotAntiMassDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 5,METMAX, 5,kBlack,3);
  plotAntiMassDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(),-5,METMAX,-5,kBlack,3);
  plotAntiMassDiff.Draw(c,kTRUE,format1,2);
  
  plotAntiMass.SetName("fitantimasslog");
  plotAntiMass.SetLogy();
  // plotAntiMass.SetYRange(1e-3*(hAntiDataMass->GetMaximum()),10*(hAntiDataMass->GetMaximum()));
  plotAntiMass.SetYRange(0.1,10*(hAntiDataMass->GetMaximum()));
  plotAntiMass.Draw(c,kTRUE,format1,1);
  
  //
  // W+ MET plot
  //
  RooPlot *wmpframe = zmass.frame(Bins(NBINS));    
  dataMassp.plotOn(wmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMassp.plotOn(wmpframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMassp.plotOn(wmpframe,LineColor(linecolorW));
  pdfMassp.plotOn(wmpframe,Components(RooArgSet(pdfEWKp,*(qcdp.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMassp.plotOn(wmpframe,Components(RooArgSet(pdfEWKp,*(qcdp.model))),LineColor(linecolorEWK));
  pdfMassp.plotOn(wmpframe,Components(RooArgSet(*(qcdp.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMassp.plotOn(wmpframe,Components(RooArgSet(*(qcdp.model))),LineColor(linecolorQCD));
  pdfMassp.plotOn(wmpframe,Components(RooArgSet(pdfWmp)),LineColor(linecolorW),LineStyle(2));
  dataMassp.plotOn(wmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMassp->GetBinWidth(1));
  CPlot plotMassp("fitmassp",wmpframe,"","",ylabel);
  plotMassp.SetLegend(0.6,0.57,0.93,0.77);
  plotMassp.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMassp.GetLegend()->AddEntry(hDummyW,Form("W^{+}#rightarrow#mu^{+}#nu (%.1f\%)",nSigp.getVal()/hDataMassp->Integral()*100),"F");
  plotMassp.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nEWKp.getVal()/hDataMassp->Integral()*100),"F");
  plotMassp.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nQCDp.getVal()/hDataMassp->Integral()*100),"F");
  plotMassp.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMassp.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMassp.SetYRange(0.1,1.1*(hDataMassp->GetMaximum()));
  plotMassp.Draw(c,kFALSE,format1,1);

  CPlot plotMasspDiff("fitmassp","","#slash{E}_{T} [GeV]","#chi");
  plotMasspDiff.AddHist1D(hMasspDiff,"EX0",ratioColor);
  plotMasspDiff.SetYRange(-8,8);
  plotMasspDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 0,METMAX, 0,kBlack,1);
  plotMasspDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 5,METMAX, 5,kBlack,3);
  plotMasspDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(),-5,METMAX,-5,kBlack,3);
  plotMasspDiff.Draw(c,kTRUE,format1,2);
  
  plotMassp.SetName("fitmassplog");
  plotMassp.SetLogy();
  // plotMassp.SetYRange(1e-3*(hDataMassp->GetMaximum()),10*(hDataMassp->GetMaximum()));
  plotMassp.SetYRange(0.1,10*(hDataMassp->GetMaximum()));
  plotMassp.Draw(c,kTRUE,format1,1);
  // plotMassp.Draw(c,kTRUE,l,1);

  RooPlot *awmpframe = zmass.frame(Bins(NBINS));    
  antiMassp.plotOn(awmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  apdfMassp.plotOn(awmpframe,FillColor(fillcolorW),DrawOption("F"));
  apdfMassp.plotOn(awmpframe,LineColor(linecolorW));
  apdfMassp.plotOn(awmpframe,Components(RooArgSet(apdfEWKp,*(qcdp.model))),FillColor(fillcolorEWK),DrawOption("F"));
  apdfMassp.plotOn(awmpframe,Components(RooArgSet(apdfEWKp,*(qcdp.model))),LineColor(linecolorEWK));
  apdfMassp.plotOn(awmpframe,Components(RooArgSet(*(qcdp.model))),FillColor(fillcolorQCD),DrawOption("F"));
  apdfMassp.plotOn(awmpframe,Components(RooArgSet(*(qcdp.model))),LineColor(linecolorQCD));
  apdfMassp.plotOn(awmpframe,Components(RooArgSet(apdfWmp)),LineColor(linecolorW),LineStyle(2));
  antiMassp.plotOn(awmpframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hAntiDataMassp->GetBinWidth(1));
  CPlot plotAntiMassp("fitantimassp",awmpframe,"","",ylabel);
  plotAntiMassp.SetLegend(0.6,0.57,0.93,0.77);
  plotAntiMassp.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotAntiMassp.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nAntiSigp.getVal()/hAntiDataMassp->Integral()*100),"F");
  plotAntiMassp.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nAntiEWKp.getVal()/hAntiDataMassp->Integral()*100),"F");
  plotAntiMassp.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nAntiQCDp.getVal()/hAntiDataMassp->Integral()*100),"F");
  plotAntiMassp.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotAntiMassp.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotAntiMassp.SetYRange(0.1,1.1*(hAntiDataMassp->GetMaximum()));
  plotAntiMassp.Draw(c,kFALSE,format1,1);

  CPlot plotAntiMasspDiff("fitantimassp","","#slash{E}_{T} [GeV]","#chi");
  plotAntiMasspDiff.AddHist1D(hAntiMasspDiff,"EX0",ratioColor);
  plotAntiMasspDiff.SetYRange(-8,8);
  plotAntiMasspDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 0,METMAX, 0,kBlack,1);
  plotAntiMasspDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 5,METMAX, 5,kBlack,3);
  plotAntiMasspDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(),-5,METMAX,-5,kBlack,3);
  plotAntiMasspDiff.Draw(c,kTRUE,format1,2);
  
  plotAntiMassp.SetName("fitantimassplog");
  plotAntiMassp.SetLogy();
  // plotAntiMassp.SetYRange(1e-3*(hAntiDataMassp->GetMaximum()),10*(hAntiDataMassp->GetMaximum()));
  plotAntiMassp.SetYRange(0.1,10*(hAntiDataMassp->GetMaximum()));
  plotAntiMassp.Draw(c,kTRUE,format1,1);
  
  //
  // W- MET plot
  //
  RooPlot *wmmframe = zmass.frame(Bins(NBINS)); 
  dataMassm.plotOn(wmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMassm.plotOn(wmmframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMassm.plotOn(wmmframe,LineColor(linecolorW));
  pdfMassm.plotOn(wmmframe,Components(RooArgSet(pdfEWKm,*(qcdm.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMassm.plotOn(wmmframe,Components(RooArgSet(pdfEWKm,*(qcdm.model))),LineColor(linecolorEWK));
  pdfMassm.plotOn(wmmframe,Components(RooArgSet(*(qcdm.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMassm.plotOn(wmmframe,Components(RooArgSet(*(qcdm.model))),LineColor(linecolorQCD));
  pdfMassm.plotOn(wmmframe,Components(RooArgSet(pdfWmm)),LineColor(linecolorW),LineStyle(2));
  dataMassm.plotOn(wmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMassm->GetBinWidth(1));
  CPlot plotMassm("fitmassm",wmmframe,"","",ylabel);
  plotMassm.SetLegend(0.6,0.57,0.93,0.77);
  plotMassm.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMassm.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nSigm.getVal()/hDataMassm->Integral()*100),"F");
  plotMassm.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nEWKm.getVal()/hDataMassm->Integral()*100),"F");
  plotMassm.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nQCDm.getVal()/hDataMassm->Integral()*100),"F");
  plotMassm.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMassm.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMassm.SetYRange(0.1,1.1*(hDataMassm->GetMaximum()));
  plotMassm.Draw(c,kFALSE,format1,1);

  CPlot plotMassmDiff("fitmassm","","#slash{E}_{T} [GeV]","#chi");
  plotMassmDiff.AddHist1D(hMassmDiff,"EX0",ratioColor);
  plotMassmDiff.SetYRange(-8,8);
  plotMassmDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 0,METMAX, 0,kBlack,1);
  plotMassmDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 5,METMAX, 5,kBlack,3);
  plotMassmDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(),-5,METMAX,-5,kBlack,3);
  plotMassmDiff.Draw(c,kTRUE,format1,2);
  
  plotMassm.SetName("fitmassmlog");
  plotMassm.SetLogy();
  // plotMassm.SetYRange(1e-3*(hDataMassm->GetMaximum()),10*(hDataMassm->GetMaximum()));
  plotMassm.SetYRange(0.1,10*(hDataMassm->GetMaximum()));
  plotMassm.Draw(c,kTRUE,format1,1);

  RooPlot *awmmframe = zmass.frame(Bins(NBINS)); 
  antiMassm.plotOn(awmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  apdfMassm.plotOn(awmmframe,FillColor(fillcolorW),DrawOption("F"));
  apdfMassm.plotOn(awmmframe,LineColor(linecolorW));
  apdfMassm.plotOn(awmmframe,Components(RooArgSet(apdfEWKm,*(qcdm.model))),FillColor(fillcolorEWK),DrawOption("F"));
  apdfMassm.plotOn(awmmframe,Components(RooArgSet(apdfEWKm,*(qcdm.model))),LineColor(linecolorEWK));
  apdfMassm.plotOn(awmmframe,Components(RooArgSet(*(qcdm.model))),FillColor(fillcolorQCD),DrawOption("F"));
  apdfMassm.plotOn(awmmframe,Components(RooArgSet(*(qcdm.model))),LineColor(linecolorQCD));
  apdfMassm.plotOn(awmmframe,Components(RooArgSet(apdfWmm)),LineColor(linecolorW),LineStyle(2));
  antiMassm.plotOn(awmmframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMassm->GetBinWidth(1));
  CPlot plotAntiMassm("fitantimassm",awmmframe,"","",ylabel);
  plotAntiMassm.SetLegend(0.6,0.57,0.93,0.77);
  plotAntiMassm.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotAntiMassm.GetLegend()->AddEntry(hDummyW,Form("W^{-}#rightarrow#mu^{-}#bar{#nu} (%.1f\%)",nAntiSigm.getVal()/hAntiDataMassm->Integral()*100),"F");
  plotAntiMassm.GetLegend()->AddEntry(hDummyEWK,Form("EWK+t#bar{t} (%.1f\%)",nAntiEWKm.getVal()/hAntiDataMassm->Integral()*100),"F");
  plotAntiMassm.GetLegend()->AddEntry(hDummyQCD,Form("QCD (%.1f\%)",nAntiQCDm.getVal()/hAntiDataMassm->Integral()*100),"F");
  plotAntiMassm.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotAntiMassm.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotAntiMassm.SetYRange(0.1,1.1*(hAntiDataMassm->GetMaximum()));
  plotAntiMassm.Draw(c,kFALSE,format1,1);

  CPlot plotAntiMassmDiff("fitantimassm","","#slash{E}_{T} [GeV]","#chi");
  plotAntiMassmDiff.AddHist1D(hAntiMassmDiff,"EX0",ratioColor);
  plotAntiMassmDiff.SetYRange(-8,8);
  plotAntiMassmDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 0,METMAX, 0,kBlack,1);
  plotAntiMassmDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(), 5,METMAX, 5,kBlack,3);
  plotAntiMassmDiff.AddLine(hAntiMassmDiff->GetXaxis()->GetXmin(),-5,METMAX,-5,kBlack,3);
  plotAntiMassmDiff.Draw(c,kTRUE,format1,2);
  
  plotAntiMassm.SetName("fitantimassmlog");
  plotAntiMassm.SetLogy();
  // plotAntiMassm.SetYRange(1e-3*(hAntiDataMassm->GetMaximum()),10*(hAntiDataMassm->GetMaximum()));
  plotAntiMassm.SetYRange(0.1,10*(hAntiDataMassm->GetMaximum()));
  plotAntiMassm.Draw(c,kTRUE,format1,1);

  
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
  
  chi2prob = hDataMass->Chi2Test(hPdfMass,"PUW");
  chi2ndf  = hDataMass->Chi2Test(hPdfMass,"CHI2/NDFUW");
  ksprob   = hDataMass->KolmogorovTest(hPdfMass);
  ksprobpe = hDataMass->KolmogorovTest(hPdfMass,"DX");
  sprintf(txtfname,"%s/fitresWm.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  flags = txtfile.flags();
  txtfile << setprecision(10);
  txtfile << " *** Yields *** " << endl;
  txtfile << "Selected: " << hDataMass->Integral() << endl;
  txtfile << "  Signal: " << nSig.getVal() << " +/- " << nSig.getPropagatedError(*fitRes) << endl;
  txtfile << "     QCD: " << nQCD.getVal() << " +/- " << nQCD.getPropagatedError(*fitRes) << endl;
  txtfile << "   Other: " << nEWK.getVal() << " +/- " << nEWK.getPropagatedError(*fitRes) << endl;
  txtfile << endl;
  txtfile.flags(flags);
  
  fitRes->printStream(txtfile,RooPrintable::kValue,RooPrintable::kVerbose);
  txtfile << endl;
  printCorrelations(txtfile, fitRes);
  txtfile << endl;
  printChi2AndKSResults(txtfile, chi2prob, chi2ndf, ksprob, ksprobpe);
  txtfile.close();
  
  chi2prob = hDataMassp->Chi2Test(hPdfMassp,"PUW");
  chi2ndf  = hDataMassp->Chi2Test(hPdfMassp,"CHI2/NDFUW");
  ksprob   = hDataMassp->KolmogorovTest(hPdfMassp);
  ksprobpe = hDataMassp->KolmogorovTest(hPdfMassp,"DX");  
  sprintf(txtfname,"%s/fitresWmp.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  flags = txtfile.flags();
  txtfile << setprecision(10);
  txtfile << " *** Yields *** " << endl;
  txtfile << "Selected: " << hDataMassp->Integral() << endl;
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

  chi2prob = hDataMassm->Chi2Test(hPdfMassm,"PUW");
  chi2ndf  = hDataMassm->Chi2Test(hPdfMassm,"CHI2/NDFUW");
  ksprob   = hDataMassm->KolmogorovTest(hPdfMassm);
  ksprobpe = hDataMassm->KolmogorovTest(hPdfMassm,"DX");  
  sprintf(txtfname,"%s/fitresWmm.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  flags = txtfile.flags();
  txtfile << setprecision(10);
  txtfile << " *** Yields *** " << endl;
  txtfile << "Selected: " << hDataMassm->Integral() << endl;
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
  TFile*fout=new TFile(Form("%s/Zmass_SigBkg_FitResults.root",outputDir2.Data()),"RECREATE");
  fout->cd();
  // TH1D*hprova = (TH1D*) qcd.model->createHistogram("hprova","x",200);
  TH1D*hWpos_Zmass_QCDfromFit    = (TH1D*)qcd.model->createHistogram("hWpos_Zmass_QCDfromFit",zmass);
  hWpos_Zmass_QCDfromFit->SetName("hWpos_Zmass_QCDfromFit");
  hWpos_Zmass_QCDfromFit->Scale(nQCD.getVal()/hWpos_Zmass_QCDfromFit->Integral());
  TH1D*hWpos_Zmass_SigFromFit    = (TH1D*)ZmmMass.createHistogram("hWpos_Zmass_SigFromFit",zmass);
  hWpos_Zmass_SigFromFit->SetName("hWpos_Zmass_SigFromFit");
  hWpos_Zmass_SigFromFit->Scale(nSig.getVal()/hWpos_Zmass_SigFromFit->Integral());
  TH1D*hWpos_Zmass_EWKTTfromFit  = (TH1D*)ewkMass.createHistogram("hWpos_Zmass_EWKTTfromFit",zmass);
  hWpos_Zmass_EWKTTfromFit->SetName("hWpos_Zmass_EWKTTfromFit");
  hWpos_Zmass_EWKTTfromFit->Scale(nEWK.getVal()/hWpos_Zmass_EWKTTfromFit->Integral());
  TH1D hWpos_Zmass_TotBKGfromFit    = (*hWpos_Zmass_EWKTTfromFit) + (*hWpos_Zmass_QCDfromFit);
  hWpos_Zmass_TotBKGfromFit.SetName("hWpos_Zmass_TotBKGfromFit");
  TH1D hWpos_Zmass_SigPlusBKGfromFit = (*hWpos_Zmass_SigFromFit) + (*hWpos_Zmass_EWKTTfromFit) + (*hWpos_Zmass_QCDfromFit);
  hWpos_Zmass_SigPlusBKGfromFit.SetName("hWpos_Zmass_SigPlusBKGfromFit");
  hWpos_Zmass_SigPlusBKGfromFit.Write();
  hWpos_Zmass_TotBKGfromFit.Write();
  hWpos_Zmass_QCDfromFit->Write();
  hWpos_Zmass_SigFromFit->Write();
  hWpos_Zmass_EWKTTfromFit->Write();
  hDataMass->SetName("hWpos_Zmass_DATA");
  hDataMass->Write();
  TH1D hWpos_Zmass_DATA_bkgSub = (*hDataMass) - (hWpos_Zmass_TotBKGfromFit);
  hWpos_Zmass_DATA_bkgSub.SetName("hWpos_Zmass_DATA_bkgSub");
  hWpos_Zmass_DATA_bkgSub.Write();

  TH1D*hWpos_numbers=new TH1D("hWpos_numbers","hWpos_numbers",4,0.5,4.5);
  hWpos_numbers->Fill(1,hDataMass->Integral());
  hWpos_numbers->Fill(2,nSig.getVal());
  hWpos_numbers->Fill(3,nEWK.getVal());
  hWpos_numbers->Fill(4,nQCD.getVal());
  hWpos_numbers->Write();
  TH1D*hWpos_numbers_norm=new TH1D("hWpos_numbers_norm","hWpos_numbers_norm",4,0.5,4.5);
  hWpos_numbers_norm->Fill(1,hDataMass->Integral()/hDataMass->Integral());
  hWpos_numbers_norm->Fill(2,nSig.getVal()/hDataMass->Integral());
  hWpos_numbers_norm->Fill(3,nEWK.getVal()/hDataMass->Integral());
  hWpos_numbers_norm->Fill(4,nQCD.getVal()/hDataMass->Integral());
  hWpos_numbers_norm->Write();
  fout->Close();
  cout << "QCD PDF SAVED" << endl;    

  
  gBenchmark->Show("fitZmm");
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
  sprintf(htmlfname,"%s/ZmmFitPlots.html",outDir.Data());
  htmlfile.open(htmlfname);
  htmlfile << "<!DOCTYPE html" << endl;
  htmlfile << "    PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << endl;
  htmlfile << "<html>" << endl;
  htmlfile << "<head><title>Zmm</title></head>" << endl;
  htmlfile << "<body bgcolor=\"EEEEEE\">" << endl;

  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl; 
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmass.png\"><img src=\"fitmass.png\" alt=\"fitmass.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmassp.png\"><img src=\"fitmassp.png\" alt=\"fitmassp.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmassm.png\"><img src=\"fitmassm.png\" alt=\"fitmassm.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmasslog.png\"><img src=\"fitmasslog.png\" alt=\"fitmasslog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmassplog.png\"><img src=\"fitmassplog.png\" alt=\"fitmassplog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmassmlog.png\"><img src=\"fitmassmlog.png\" alt=\"fitmassmlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimass.png\"><img src=\"fitantimass.png\" alt=\"fitantimass.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimassp.png\"><img src=\"fitantimassp.png\" alt=\"fitantimassp.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimassm.png\"><img src=\"fitantimassm.png\" alt=\"fitantimassm.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimasslog.png\"><img src=\"fitantimasslog.png\" alt=\"fitantimasslog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimassplog.png\"><img src=\"fitantimassplog.png\" alt=\"fitantimassplog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitantimassmlog.png\"><img src=\"fitantimassmlog.png\" alt=\"fitantimassmlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "</table>" << endl;
  htmlfile << "<hr />" << endl;
  
  htmlfile << "</body>" << endl;
  htmlfile << "</html>" << endl;
  htmlfile.close();  
}
