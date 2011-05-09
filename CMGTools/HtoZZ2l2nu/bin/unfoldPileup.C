#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>

#include "TRandom.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"

#include "RooUnfoldResponse.h"
#include "RooUnfoldBayes.h"
#include "RooUnfoldInvert.h"

#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

#endif

using namespace std;

/**
   @short compile me like this
   gSystem->AddIncludePath("-I/afs/cern.ch/user/p/psilva/scratch0/RooUnfold-1.0.3/src/")
   gSystem->AddIncludePath("-I${CMSSW_BASE}/src")
   gSystem->SetDynamicPath(gSystem->GetDynamicPath() + TString(":/afs/cern.ch/user/p/psilva/scratch0/RooUnfold-1.0.3:${CMSSW_BASE}/lib/${SCRAM_ARCH}"))
   gSystem->Load("libRooUnfold");
   gSystem->Load("libCMGToolsHtoZZ2l2nu");
   .L unfoldPileup.C+

   @short run me like this
   unfoldPileup(file);
*/


void unfoldPileup(TString url, TString dataurl)
{
  //open file
  TFile *fin=TFile::Open(url);
  TH2F *hgen_vs_reco = (TH2F *) fin->Get("nreco_ngen")->Clone("ngen_nreco");
  TH2F *hreco_vs_gen = (TH2F *) fin->Get("nreco_ngen");
  hgen_vs_reco->Reset("ICE");
  for( Int_t xbin=1; xbin<=hgen_vs_reco->GetXaxis()->GetNbins(); xbin++)
    for( Int_t ybin=1; ybin<=hgen_vs_reco->GetXaxis()->GetNbins(); ybin++)
      {
	double val=hreco_vs_gen->GetBinContent(xbin,ybin);
	double err=hreco_vs_gen->GetBinError(xbin,ybin);
	hgen_vs_reco->SetBinContent(ybin,xbin,val);
	hgen_vs_reco->SetBinError(ybin,xbin,err);
      }
  hgen_vs_reco->SetDirectory(0);
  hgen_vs_reco->SetTitle("Generated vs. reco");
  hgen_vs_reco->SetMarkerColor(1);
  TH1F *hgen = (TH1F *) fin->Get("ngen");
  hgen->SetDirectory(0);
  hgen->SetTitle("Generated");
  hgen->SetMarkerColor(1);
  fin->Close();

  fin = TFile::Open(dataurl); 
  TH1F *hdata = (TH1F *) fin->Get("hdata");
  hdata->SetDirectory(0);
  hdata->SetTitle("Data");
  fin->Close();

  //unfold
  RooUnfoldResponse response (hgen_vs_reco->ProjectionX(), hgen, hgen_vs_reco);
  RooUnfoldBayes unfold (&response, hdata,4 );
  TH1D *hunfold = (TH1D*) unfold.Hreco(RooUnfold::kCovariance)->Clone("hunfold");
  hunfold->SetTitle("Unfolded data");
  hunfold->SetMarkerColor(1);

  //draw
  setStyle();
  gStyle->SetPalette(1);  
  TCanvas *c= getNewCanvas("c1","c1",false);
  c->SetWindowSize(1000,1000);
  c->Divide(2,2);
  c->cd(1);  hgen_vs_reco->DrawNormalized("colz");
  c->cd(2);  hgen->DrawNormalized();
  c->cd(3);  hdata->DrawNormalized();
  c->cd(4);  hunfold->DrawNormalized();
  c->SaveAs("UnfoldedPileupInData.C");
}
