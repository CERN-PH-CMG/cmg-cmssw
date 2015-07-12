//================================================================================================
//
// Perform fit to extract W->enu signal
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
#include "Utils/RecoilCorrector.hh"    // class to handle recoil corrections for MET

// RooFit headers
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgList.h"
#include "RooDataHist.h"
#include "RooFormulaVar.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
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

void fitWe(const TString  outputDir="test",   // output directory
           const Double_t lumi=18.7,        // integrated luminosity (/fb)
	   const Int_t    Ecm=8,         // center-of-mass energy
	   const Int_t    doPU=1         // option for PU-reweighting
) {
  gBenchmark->Start("fitWe");

  //--------------------------------------------------------------------------------------------------------------
  // Settings 
  //==============================================================================================================   
  
  // MET histogram binning and range
  const Int_t    NBINS  = 50;
  const Double_t METMAX = 100;

  const Double_t PT_CUT  = 25;
  const Double_t ETA_CUT = 2.5;

  // file format for output plots
  const TString format("png"); 
    
  // file name with recoil correction
  TString recoilfname("../RecoilSyst/ZeeData/fits.root");
  
  // file name(s) with PU weights
  TString pufname("");
  if(doPU>0) {
    if(doPU==1) { pufname = "Utils/PileupReweighting.Summer11DYmm_To_Run2011A.root"; }
    else        { cout << "Invalid option for PU re-weighting! Aborting..." << endl; assert(0); }
  }
  
  //
  // input ntuple file names
  //
  enum { eData, eWenu, eEWK };  // data type enum
  vector<TString> fnamev;
  vector<Int_t>   typev;
  
  fnamev.push_back("../Selection/Wenu/ntuples/data_m23_select.root");   typev.push_back(eData);
  fnamev.push_back("../Selection/Wenu/ntuples/we_select.root");         typev.push_back(eWenu);
  fnamev.push_back("../Selection/Wenu/ntuples/ewk_select.root");        typev.push_back(eEWK);
  fnamev.push_back("../Selection/Wenu/ntuples/top_select.root");        typev.push_back(eEWK);


  //--------------------------------------------------------------------------------------------------------------
  // Main analysis code 
  //==============================================================================================================  
  
  // Create output directory
  gSystem->mkdir(outputDir,kTRUE);
  CPlot::sOutDir = outputDir;  
  
  // Get pile-up weights
  TFile *pufile    = 0;
  TH1D  *puWeights = 0;
  if(doPU>0) {
    pufile = new TFile(pufname);
    assert(pufile);
    puWeights = (TH1D*)pufile->Get("puWeights");
    assert(puWeights);
  }
  
  // Access recoil corrections
  //RecoilCorrector recoilCorr(recoilfname);
  
  //
  // Declare MET histograms
  //
  TH1D *hDataMet  = new TH1D("hDataMet", "",NBINS,0,METMAX); hDataMet->Sumw2();
  TH1D *hDataMetm = new TH1D("hDataMetm","",NBINS,0,METMAX); hDataMetm->Sumw2();  
  TH1D *hDataMetp = new TH1D("hDataMetp","",NBINS,0,METMAX); hDataMetp->Sumw2();
  TH1D *hWenuMet  = new TH1D("hWenuMet", "",NBINS,0,METMAX); hWenuMet->Sumw2();
  TH1D *hWenuMetp = new TH1D("hWenuMetp","",NBINS,0,METMAX); hWenuMetp->Sumw2();
  TH1D *hWenuMetm = new TH1D("hWenuMetm","",NBINS,0,METMAX); hWenuMetm->Sumw2();
  TH1D *hEWKMet   = new TH1D("hEWKMet",  "",NBINS,0,METMAX); hEWKMet->Sumw2();
  TH1D *hEWKMetp  = new TH1D("hEWKMetp", "",NBINS,0,METMAX); hEWKMetp->Sumw2();
  TH1D *hEWKMetm  = new TH1D("hEWKMetm", "",NBINS,0,METMAX); hEWKMetm->Sumw2();

  //
  // Declare variables to read in ntuple
  //
  UInt_t  runNum, lumiSec, evtNum;
  UInt_t  npv, npu;
  Float_t genWPt, genWPhi;
  Float_t scale1fb;
  Float_t met, metPhi, sumEt, mt, u1, u2;
  Int_t   q;
  LorentzVector *lep=0;
  LorentzVector *sc=0;
    
  TFile *infile=0;
  TTree *intree=0;

  //
  // Loop over files
  //
  for(UInt_t ifile=0; ifile<fnamev.size(); ifile++) {
    
    // Read input file and get the TTrees
    cout << "Processing " << fnamev[ifile] << "..." << endl;
    infile = new TFile(fnamev[ifile]);	  assert(infile);
    intree = (TTree*)infile->Get("Events"); assert(intree);

    intree->SetBranchAddress("runNum",   &runNum);    // event run number
    intree->SetBranchAddress("lumiSec",  &lumiSec);   // event lumi section
    intree->SetBranchAddress("evtNum",   &evtNum);    // event number
    intree->SetBranchAddress("npv",      &npv);       // number of primary vertices
    intree->SetBranchAddress("npu",      &npu);       // number of in-time PU events (MC)
    intree->SetBranchAddress("genVPt",   &genWPt);    // GEN W boson pT (signal MC)
    intree->SetBranchAddress("genVPhi",  &genWPhi);   // GEN W boson phi (signal MC)
    intree->SetBranchAddress("scale1fb", &scale1fb);  // event weight per 1/fb (MC)
    intree->SetBranchAddress("met",      &met);       // MET
    intree->SetBranchAddress("metPhi",   &metPhi);    // phi(MET)
    intree->SetBranchAddress("sumEt",    &sumEt);     // Sum ET
    intree->SetBranchAddress("mt",       &mt);        // transverse mass
    intree->SetBranchAddress("u1",       &u1);        // parallel component of recoil
    intree->SetBranchAddress("u2",       &u2);        // perpendicular component of recoil
    intree->SetBranchAddress("q",        &q);         // lepton charge
    intree->SetBranchAddress("lep",      &lep);       // lepton 4-vector
    intree->SetBranchAddress("sc",       &sc);        // electron Supercluster 4-vector
  
    //
    // loop over events
    //
    for(UInt_t ientry=0; ientry<intree->GetEntries(); ientry++) {
      intree->GetEntry(ientry);
      
      if(mt< 40)  continue;	// LUCA ADD
      TLorentzVector muPosP, muNegP, JpsiP;
      muPosP.SetPtEtaPhiM(lep->Pt(),lep->Eta(),lep->Phi(),lep->M()); // always use the muon
      muNegP.SetPtEtaPhiM(met,0,metPhi,0); // always use the neutrino
      JpsiP = muPosP + muNegP;
      
      // if(JpsiP.Pt() < 15 
      // || JpsiP.Pt() > 25 )  continue;	

      
      if(sc->Pt()        < PT_CUT)  continue;	
      if(fabs(sc->Eta()) > ETA_CUT) continue;
   
      if(typev[ifile]==eData) {
        hDataMet->Fill(met);
	if(q>0) { hDataMetp->Fill(met); } 
	else    { hDataMetm->Fill(met); }
      
      } else {
        Double_t weight = 1;
        weight *= scale1fb*lumi;
        if(puWeights)
	  weight *= puWeights->GetBinContent(npu+1);
	
	if(typev[ifile]==eWenu) {
          Double_t corrMet=met, corrMetPhi=metPhi;
        
	  // apply recoil corrections to W MC
	  //recoilCorr.Correct(corrMet,corrMetPhi,genWPt,genWPhi,lep->Pt(),lep->Phi());
	
          hWenuMet->Fill(corrMet,weight);
	  if(q>0) { hWenuMetp->Fill(corrMet,weight); } 
	  else    { hWenuMetm->Fill(corrMet,weight); }
        }
        if(typev[ifile]==eEWK) {
          hEWKMet->Fill(met,weight);
	  if(q>0) { hEWKMetp->Fill(met,weight); }
	  else    { hEWKMetm->Fill(met,weight); }
        }
      }
    }
  }  
  delete infile;
  infile=0, intree=0;   
  
  //
  // Declare fit parameters for signal and background yields
  // Note: W signal and EWK+top PDFs are constrained to the ratio described in MC
  //
  RooRealVar nSig("nSig","nSig",0.7*(hDataMet->Integral()),0,hDataMet->Integral());
  RooRealVar nQCD("nQCD","nQCD",0.3*(hDataMet->Integral()),0,hDataMet->Integral());
  RooRealVar cewk("cewk","cewk",0.1,0,5) ;
  cewk.setVal(hEWKMet->Integral()/hWenuMet->Integral());
  cewk.setConstant(kTRUE);
  RooFormulaVar nEWK("nEWK","nEWK","cewk*nSig",RooArgList(nSig,cewk));
  
  RooRealVar nSigp("nSigp","nSigp",0.7*(hDataMetp->Integral()),0,hDataMetp->Integral());
  RooRealVar nQCDp("nQCDp","nQCDp",0.3*(hDataMetp->Integral()),0,hDataMetp->Integral());
  RooRealVar cewkp("cewkp","cewkp",0.1,0,5) ;
  cewkp.setVal(hEWKMetp->Integral()/hWenuMetp->Integral());
  cewkp.setConstant(kTRUE);
  RooFormulaVar nEWKp("nEWKp","nEWKp","cewkp*nSigp",RooArgList(nSigp,cewkp));
  
  RooRealVar nSigm("nSigm","nSigm",0.7*(hDataMetm->Integral()),0,hDataMetm->Integral());
  RooRealVar nQCDm("nQCDm","nQCDm",0.3*(hDataMetm->Integral()),0,hDataMetm->Integral());
  RooRealVar cewkm("cewkm","cewkm",0.1,0,5) ;
  cewkm.setVal(hEWKMetm->Integral()/hWenuMetm->Integral());
  cewkm.setConstant(kTRUE);
  RooFormulaVar nEWKm("nEWKm","nEWKm","cewkm*nSigm",RooArgList(nSigm,cewkm));  
  
  //
  // Construct PDFs for fitting
  //
  RooRealVar pfmet("pfmet","pfmet",0,METMAX);
  pfmet.setBins(NBINS);
   
  // Signal PDFs
  RooDataHist wenuMet ("wenuMET", "wenuMET", RooArgSet(pfmet),hWenuMet);  RooHistPdf pdfWe ("we", "we", pfmet,wenuMet, 1);
  RooDataHist wenuMetp("wenuMETp","wenuMETp",RooArgSet(pfmet),hWenuMetp); RooHistPdf pdfWep("wep","wep",pfmet,wenuMetp,1);
  RooDataHist wenuMetm("wenuMETm","wenuMETm",RooArgSet(pfmet),hWenuMetm); RooHistPdf pdfWem("wem","wem",pfmet,wenuMetm,1); 
  
  // EWK+top PDFs
  RooDataHist ewkMet ("ewkMET", "ewkMET", RooArgSet(pfmet),hEWKMet);  RooHistPdf pdfEWK ("ewk", "ewk", pfmet,ewkMet, 1);
  RooDataHist ewkMetp("ewkMETp","ewkMETp",RooArgSet(pfmet),hEWKMetp); RooHistPdf pdfEWKp("ewkp","ewkp",pfmet,ewkMetp,1); 
  RooDataHist ewkMetm("ewkMETm","ewkMETm",RooArgSet(pfmet),hEWKMetm); RooHistPdf pdfEWKm("ewkm","ewkm",pfmet,ewkMetm,1); 
  
  // QCD Pdfs
  CPepeModel1 qcd("qcd",pfmet);
  CPepeModel1 qcdp("qcdp",pfmet);
  CPepeModel1 qcdm("qcdm",pfmet);
  
  // Signal + Background PDFs
  RooAddPdf pdfMet ("pdfMet", "pdfMet", RooArgList(pdfWe,pdfEWK,*(qcd.model)),   RooArgList(nSig,nEWK,nQCD));  
  RooAddPdf pdfMetp("pdfMetp","pdfMetp",RooArgList(pdfWep,pdfEWKp,*(qcdp.model)),RooArgList(nSigp,nEWKp,nQCDp));
  RooAddPdf pdfMetm("pdfMetm","pdfMetm",RooArgList(pdfWem,pdfEWKm,*(qcdm.model)),RooArgList(nSigm,nEWKm,nQCDm));
    
  //
  // Perform fits
  //
  RooDataHist dataMet("dataMet", "dataMet", RooArgSet(pfmet),hDataMet);
  RooFitResult *fitRes = pdfMet.fitTo(dataMet,Extended(),Minos(kTRUE),Save(kTRUE));
  
  RooDataHist dataMetp("dataMetp","dataMetp",RooArgSet(pfmet),hDataMetp);
  RooFitResult *fitResp = pdfMetp.fitTo(dataMetp,Extended(),Minos(kTRUE),Save(kTRUE));
  
  RooDataHist dataMetm("dataMetm","dataMetm",RooArgSet(pfmet),hDataMetm);
  RooFitResult *fitResm = pdfMetm.fitTo(dataMetm,Extended(),Minos(kTRUE),Save(kTRUE));
    
  //
  // Use histogram version of fitted PDFs to make ratio plots
  // (Will also use PDF histograms later for Chi^2 and KS tests)
  //
  TH1D *hPdfMet = (TH1D*)(pdfMet.createHistogram("hPdfMet", pfmet));
  hPdfMet->Scale((nSig.getVal()+nEWK.getVal()+nQCD.getVal())/hPdfMet->Integral());
  TH1D *hMetDiff = makeDiffHist(hDataMet,hPdfMet,"hMetDiff");
  hMetDiff->SetMarkerStyle(kFullCircle);
  hMetDiff->SetMarkerSize(0.9);
   
  TH1D *hPdfMetp = (TH1D*)(pdfMetp.createHistogram("hPdfMetp", pfmet));
  hPdfMetp->Scale((nSigp.getVal()+nEWKp.getVal()+nQCDp.getVal())/hPdfMetp->Integral());
  TH1D *hMetpDiff = makeDiffHist(hDataMetp,hPdfMetp,"hMetpDiff");
  hMetpDiff->SetMarkerStyle(kFullCircle);
  hMetpDiff->SetMarkerSize(0.9);
    
  TH1D *hPdfMetm = (TH1D*)(pdfMetm.createHistogram("hPdfMetm", pfmet));
  hPdfMetm->Scale((nSigm.getVal()+nEWKm.getVal()+nQCDm.getVal())/hPdfMetm->Integral());
  TH1D *hMetmDiff = makeDiffHist(hDataMetm,hPdfMetm,"hMetmDiff");
  hMetmDiff->SetMarkerStyle(kFullCircle); 
  hMetmDiff->SetMarkerSize(0.9);
   
  
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
  RooPlot *weframe = pfmet.frame(Bins(NBINS));    
  dataMet.plotOn(weframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMet.plotOn(weframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMet.plotOn(weframe,LineColor(linecolorW));
  pdfMet.plotOn(weframe,Components(RooArgSet(pdfEWK,*(qcd.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMet.plotOn(weframe,Components(RooArgSet(pdfEWK,*(qcd.model))),LineColor(linecolorEWK));
  pdfMet.plotOn(weframe,Components(RooArgSet(*(qcd.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMet.plotOn(weframe,Components(RooArgSet(*(qcd.model))),LineColor(linecolorQCD));
  pdfMet.plotOn(weframe,Components(RooArgSet(pdfWe)),LineColor(linecolorW),LineStyle(2));
  dataMet.plotOn(weframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMet->GetBinWidth(1));
  CPlot plotMet("fitmet",weframe,"","",ylabel);
  plotMet.SetLegend(0.68,0.57,0.93,0.77);
  plotMet.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMet.GetLegend()->AddEntry(hDummyW,"W#rightarrowe#nu","F");
  plotMet.GetLegend()->AddEntry(hDummyEWK,"EWK+t#bar{t}","F");
  plotMet.GetLegend()->AddEntry(hDummyQCD,"QCD","F");
  plotMet.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMet.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMet.SetYRange(0.1,1.1*(hDataMet->GetMaximum()));
//  plotMet.Draw(c,kFALSE,format,1);
  plotMet.Draw(c,kTRUE,format,1);

  CPlot plotMetDiff("fitmet","","#slash{E}_{T} [GeV]","#chi");
  plotMetDiff.AddHist1D(hMetDiff,"EX0",ratioColor);
  plotMetDiff.SetYRange(-8,8);
  plotMetDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotMetDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotMetDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotMetDiff.Draw(c,kTRUE,format,2);
  
  plotMet.SetName("fitmetlog");
  plotMet.SetLogy();
  plotMet.SetYRange(1e-3*(hDataMet->GetMaximum()),10*(hDataMet->GetMaximum()));
  plotMet.Draw(c,kTRUE,format,1);
    
  //
  // W+ MET plot
  //
  RooPlot *wepframe = pfmet.frame(Bins(NBINS));    
  dataMetp.plotOn(wepframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMetp.plotOn(wepframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMetp.plotOn(wepframe,LineColor(linecolorW));
  pdfMetp.plotOn(wepframe,Components(RooArgSet(pdfEWKp,*(qcdp.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMetp.plotOn(wepframe,Components(RooArgSet(pdfEWKp,*(qcdp.model))),LineColor(linecolorEWK));
  pdfMetp.plotOn(wepframe,Components(RooArgSet(*(qcdp.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMetp.plotOn(wepframe,Components(RooArgSet(*(qcdp.model))),LineColor(linecolorQCD));
  pdfMetp.plotOn(wepframe,Components(RooArgSet(pdfWep)),LineColor(linecolorW),LineStyle(2));
  dataMetp.plotOn(wepframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));  
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMetp->GetBinWidth(1));
  CPlot plotMetp("fitmetp",wepframe,"","",ylabel);
  plotMetp.SetLegend(0.68,0.57,0.93,0.77);
  plotMetp.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMetp.GetLegend()->AddEntry(hDummyW,"W^{+}#rightarrowe^{+}#nu","F");
  plotMetp.GetLegend()->AddEntry(hDummyEWK,"EWK+t#bar{t}","F");
  plotMetp.GetLegend()->AddEntry(hDummyQCD,"QCD","F");
  plotMetp.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMetp.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMetp.SetYRange(0.1,1.1*(hDataMetp->GetMaximum()));
  plotMetp.Draw(c,kFALSE,format,1);

  CPlot plotMetpDiff("fitmetp","","#slash{E}_{T} [GeV]","#chi");
  plotMetpDiff.AddHist1D(hMetpDiff,"EX0",ratioColor);
  plotMetpDiff.SetYRange(-8,8);
  plotMetpDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotMetpDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotMetpDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotMetpDiff.Draw(c,kTRUE,format,2);
  
  plotMetp.SetName("fitmetplog");
  plotMetp.SetLogy();
  plotMetp.SetYRange(1e-3*(hDataMetp->GetMaximum()),10*(hDataMetp->GetMaximum()));
  plotMetp.Draw(c,kTRUE,format,1);
  
  //
  // W- MET plot
  //
  RooPlot *wemframe = pfmet.frame(Bins(NBINS)); 
  dataMetm.plotOn(wemframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  pdfMetm.plotOn(wemframe,FillColor(fillcolorW),DrawOption("F"));
  pdfMetm.plotOn(wemframe,LineColor(linecolorW));
  pdfMetm.plotOn(wemframe,Components(RooArgSet(pdfEWKm,*(qcdm.model))),FillColor(fillcolorEWK),DrawOption("F"));
  pdfMetm.plotOn(wemframe,Components(RooArgSet(pdfEWKm,*(qcdm.model))),LineColor(linecolorEWK));
  pdfMetm.plotOn(wemframe,Components(RooArgSet(*(qcdm.model))),FillColor(fillcolorQCD),DrawOption("F"));
  pdfMetm.plotOn(wemframe,Components(RooArgSet(*(qcdm.model))),LineColor(linecolorQCD));
  pdfMetm.plotOn(wemframe,Components(RooArgSet(pdfWem)),LineColor(linecolorW),LineStyle(2));
  dataMetm.plotOn(wemframe,MarkerStyle(kFullCircle),MarkerSize(0.9),DrawOption("ZP"));
  
  sprintf(ylabel,"Events / %.1f GeV",hDataMetm->GetBinWidth(1));
  CPlot plotMetm("fitmetm",wemframe,"","",ylabel);
  plotMetm.SetLegend(0.68,0.57,0.93,0.77);
  plotMetm.GetLegend()->AddEntry(hDummyData,"data","PL");
  plotMetm.GetLegend()->AddEntry(hDummyW,"W^{-}#rightarrowe^{-}#bar{#nu}","F");
  plotMetm.GetLegend()->AddEntry(hDummyEWK,"EWK+t#bar{t}","F");
  plotMetm.GetLegend()->AddEntry(hDummyQCD,"QCD","F");
  plotMetm.AddTextBox(lumitext,0.55,0.80,0.90,0.86,0);
  plotMetm.AddTextBox("CMS Preliminary",0.63,0.92,0.95,0.99,0);
  plotMetm.SetYRange(0.1,1.1*(hDataMetm->GetMaximum()));
  plotMetm.Draw(c,kFALSE,format,1);

  CPlot plotMetmDiff("fitmetm","","#slash{E}_{T} [GeV]","#chi");
  plotMetmDiff.AddHist1D(hMetmDiff,"EX0",ratioColor);
  plotMetmDiff.SetYRange(-8,8);
  plotMetmDiff.AddLine(0, 0,METMAX, 0,kBlack,1);
  plotMetmDiff.AddLine(0, 5,METMAX, 5,kBlack,3);
  plotMetmDiff.AddLine(0,-5,METMAX,-5,kBlack,3);
  plotMetmDiff.Draw(c,kTRUE,format,2);
  
  plotMetm.SetName("fitmetmlog");
  plotMetm.SetLogy();
  plotMetm.SetYRange(1e-3*(hDataMetm->GetMaximum()),10*(hDataMetm->GetMaximum()));
  plotMetm.Draw(c,kTRUE,format,1);

    
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
  
  chi2prob = hDataMet->Chi2Test(hPdfMet,"PUW");
  chi2ndf  = hDataMet->Chi2Test(hPdfMet,"CHI2/NDFUW");
  ksprob   = hDataMet->KolmogorovTest(hPdfMet);
  ksprobpe = hDataMet->KolmogorovTest(hPdfMet,"DX");
  sprintf(txtfname,"%s/fitresWe.txt",CPlot::sOutDir.Data());
  txtfile.open(txtfname);
  assert(txtfile.is_open());
  
  flags = txtfile.flags();
  txtfile << setprecision(10);
  txtfile << " *** Yields *** " << endl;
  txtfile << "Selected: " << hDataMet->Integral() << endl;
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
  
  chi2prob = hDataMetp->Chi2Test(hPdfMetp,"PUW");
  chi2ndf  = hDataMetp->Chi2Test(hPdfMetp,"CHI2/NDFUW");
  ksprob   = hDataMetp->KolmogorovTest(hPdfMetp);
  ksprobpe = hDataMetp->KolmogorovTest(hPdfMetp,"DX");  
  sprintf(txtfname,"%s/fitresWep.txt",CPlot::sOutDir.Data());
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
  sprintf(txtfname,"%s/fitresWem.txt",CPlot::sOutDir.Data());
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

  makeHTML(outputDir);
  
  cout << endl;
  cout << "  <> Output saved in " << outputDir << "/" << endl;    
  cout << endl;     
  
  gBenchmark->Show("fitWe");
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
  sprintf(htmlfname,"%s/WenuFitPlots.html",outDir.Data());
  htmlfile.open(htmlfname);
  htmlfile << "<!DOCTYPE html" << endl;
  htmlfile << "    PUBLIC \"-//W3C//DTD HTML 3.2//EN\">" << endl;
  htmlfile << "<html>" << endl;
  htmlfile << "<head><title>Wenu</title></head>" << endl;
  htmlfile << "<body bgcolor=\"EEEEEE\">" << endl;

  htmlfile << "<table border=\"0\" cellspacing=\"5\" width=\"100%\">" << endl; 
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmet.png\"><img src=\"fitmet.png\" alt=\"fitmet.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetp.png\"><img src=\"fitmetp.png\" alt=\"fitmetp.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetm.png\"><img src=\"fitmetm.png\" alt=\"fitmetm.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "<tr>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetlog.png\"><img src=\"fitmetlog.png\" alt=\"fitmetlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetplog.png\"><img src=\"fitmetplog.png\" alt=\"fitmetplog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"><a target=\"_blank\" href=\"fitmetmlog.png\"><img src=\"fitmetmlog.png\" alt=\"fitmetmlog.png\" width=\"100%\"></a></td>" << endl;
  htmlfile << "<td width=\"25%\"></td>" << endl;
  htmlfile << "</tr>" << endl;
  htmlfile << "</table>" << endl;
  htmlfile << "<hr />" << endl;
  
  htmlfile << "</body>" << endl;
  htmlfile << "</html>" << endl;
  htmlfile.close();  
}
