
//#ifndef __CINT__
//#include "RooGlobalFunc.h"
//#endif
//#include "RooRealVar.h"
//#include "RooDataSet.h"
//#include "RooDataHist.h"
//#include "RooCBShape.h"
//#include "RooFitResult.h"
//#include "RooPlot.h"
//#include "RooBinning.h"
//#include "TCanvas.h"
//#include "TString.h"
//#include "TTree.h"
//#include "TH1.h"
//#include "TF1.h"
//#include "TFile.h"
//#include "TGraphErrors.h"

using namespace RooFit ;


//string sTitle("histograms_Fat30_mc_RSGravitonToJJ_M-");
//string sRegion("_TuneD6T_ak5QQtoQQ");


string sTitle("histograms_Fat30_mc_RSGravitonToJJ_M-");
string sRegion("_TuneD6T_ak5GGtoGG");

//string sTitle("histograms_Fat30_mc_QstarToJJ_M-");
//string sRegion("_TuneD6T_ak5");

int Nbins = 5;
bool bPF = false;



//string sTitle("histograms_Fat30_mc_QstarToJJ_M-");
//string sRegion("_TuneD6T_ak7");

//string sTitle("histograms_Fat30_mc_RSGravitonToJJ_M-");
//string sRegion("_TuneD6T_ak7QQtoQQ");

//string sTitle("histograms_Fat30_mc_RSGravitonToJJ_M-");
//string sRegion("_TuneD6T_ak7GGtoGG");

//int Nbins = 6;
//bool bPF = true;



RooFitResult* FitHistWithCBShape(TH1* h)
{
  RooFitResult *res = 0;

  double avg = 0.97;
  double rms = 0.05;
  double xmin = 0.3;
  double xmax = 1.3;

  RooRealVar E("E","Normalised Resonance Mass", xmin, xmax, "GeV");
  RooDataHist dataSet("dataSet", "dataSet", E, h ); 

  RooPlot* frame = E.frame();
  dataSet.plotOn(frame);

  RooRealVar mean("mean","mean", avg, 0.8, 1.1) ;
  RooRealVar sigma("sigma","width", rms, 0.001, 0.2);
  RooRealVar alphaLow("alpha","alpha", 2.0, 0, 5.0);
  RooRealVar nLow("nLow","n", 1.0, 0.00001, 10000.0);
  RooRealVar alphaHigh("alphaHigh","alphaHigh", 2.0, 0, 5.0);
  RooRealVar nHigh("nHigh","nHigh", 3.0, 0.0000000001, 100000.0);

  RooFormulaVar minusE("minusE","Negate the data set","-1*E", E) ;
  RooFormulaVar minusMean("minusMean","Negate the mean","-1*mean", mean) ;


/*
  TH1D* toto = new TH1D("toto", "toto", 100, 0., 7000);

  minusE.fillHistogram(toto, RooArgList(E));

  toto->Draw();
*/

  RooCBShape CBallLow("CBallLow","Crystal Ball for lower tail", E, mean, sigma, alphaLow, nLow);
  RooCBShape CBallHigh("CBallHigh","Crystal Ball for highr tail", minusE, minusMean, sigma, alphaHigh, nHigh);  

  RooRealVar cb1frac("cb1frac","fraction of cb1", 0.5, 0, 1.0) ;

  RooAddPdf sum("sum","cb1+cb2",RooArgList(CBallLow, CBallHigh), cb1frac) ;

  res = sum.fitTo(dataSet, Save() );
//  minusE.plotOn(frame);
  sum.plotOn(frame);
	
//  sum.plotOn(frame);
  frame->Draw();
  return res;

}



RooFitResult* Resolution(int indx)
{

  vector <int> mass; 
  mass.push_back(500); mass.push_back(700); mass.push_back(1200); mass.push_back(2000); mass.push_back(3500); mass.push_back(4000); 

  string sName = sTitle + "" + Form("%d", mass[indx]) + "" + sRegion + ".root";

  if( indx<0 || indx>5 ) return;
  TFile* fin = new TFile( sName.c_str() );

  TH1D* hist = ((TH2D*)fin->Get("h_DijetMass_data_MassShape_fat;1"))->ProjectionX("hist", indx+1, indx+1);
  if (bPF) hist = ((TH2D*)fin->Get("h_DijetMass_data_MassShape_pf;1"))->ProjectionX("hist", indx+1, indx+1);
  RooFitResult *r = FitHistWithCBShape(hist);

  r->Print();

  
  TString sNamePng(sName.c_str());
  sNamePng.ReplaceAll(".root", ".png");
  cout << "sNamePng = " << sNamePng.Data() << endl;
  
  gPad->SaveAs(sNamePng.Data());

  fin->Close();

  return r;

}


void FitterExample(){

  vector <int> mass; 
  mass.push_back(500); mass.push_back(700); mass.push_back(1200); mass.push_back(2000); mass.push_back(3500); mass.push_back(4000); 


   TGraphErrors* gSigma = new TGraphErrors(Nbins);

   gSigma->SetTitle("#sigma/#mu");

   TGraphErrors* gMean = new TGraphErrors(Nbins);

   gMean->SetTitle("#mu");


   for (int i = 0; i < Nbins; i++){
      RooFitResult *r = Resolution(i);


      double sVal = ((RooAbsReal*) r->floatParsFinal().find("sigma"))->getVal();
      double sError = ((RooAbsReal*) r->floatParsFinal().find("sigma"))->getPropagatedError(*r);
 
      double sMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getVal();
      double sErrorMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getPropagatedError(*r);

 

      cout << "Mass  =  " << mass[i] << " sMean = " << sMean << " errorMean = " << sErrorMean << " dSigma = " << sVal << " error = " << sError<< endl;

      if (sError/sMean < 0.003) sError = 0.003*sMean;
      gSigma->SetPoint(i, mass[i], sVal/sMean);
      gSigma->SetPointError(i, 0, sError/sMean);

      gMean->SetPoint(i, mass[i], sMean);
      gMean->SetPointError(i, 0, sErrorMean);


   }
   
   for (int i = 0; i < Nbins; i++){

  

     double Mass, dSigma, dMean, dMeanError;
      
     double sMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getVal();

      gSigma->GetPoint(i, Mass, dSigma);
      gMean->GetPoint(i, Mass, dMean);

      cout << Mass << " " << dMean << " " << gMean->GetErrorY(i) << " " << dSigma << " " << gSigma->GetErrorY(i) << " " << endl;
   }

   string sName = sTitle  + "AllMasses" +  sRegion;
   if (bPF) sName = sName + "_pf.txt";
   else  sName = sName + "_Fat30.txt";

   ofstream outFileTxt;
   outFileTxt.open(sName.c_str());
   
   
   for (int i = 0; i < Nbins; i++){
  
     double Mass, dSigma, dMean;

      gMean->GetPoint(i, Mass, dMean);
      gSigma->GetPoint(i, Mass, dSigma);

      double sMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getVal();


      string sName = sTitle + "" + Form("%d", mass[i]) + "" + sRegion + ".root";
      TFile* fin = new TFile( sName.c_str() );
      TH1D* hist = ((TH2D*)fin->Get("h_DijetMass_data_MassShape_fat;1"))->ProjectionX("hist", i+1, i+1);
      if (bPF) hist = ((TH2D*)fin->Get("h_DijetMass_data_MassShape_pf;1"))->ProjectionX("hist", i+1, i+1);

      double RMS = hist->GetRMS();


      outFileTxt << Mass << " " << dMean << " " << gMean->GetErrorY(i) << " " << dSigma << " " << gSigma->GetErrorY(i) << " " << RMS/sMean << endl;
      fin->Close();
   }

   outFileTxt.close();

   TF1* fReso = new TF1("fReso", "[0]+[1]/sqrt(x)");

   TCanvas* c = new TCanvas();

   c->Divide(1,2);
   c->cd(1);

   gSigma->Draw("APE");
   gSigma->Fit(fReso);

   c->cd(2);
   gMean->Draw("APE");

   TLine* L = new TLine(400, 1, 4200, 1);
   L->SetLineColor(kRed);
   L->SetLineWidth(2);
   L->Draw("SAME");

   string sName = sTitle + "AllMasses" + sRegion;
   if (bPF) sName = sName + "_pf.png";
   else  sName = sName + "_Fat30.png";



   c->SaveAs(sName.c_str());
   
}
