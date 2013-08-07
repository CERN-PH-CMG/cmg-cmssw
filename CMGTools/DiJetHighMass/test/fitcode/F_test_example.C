#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"
#include "TChain.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TMath.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TVirtualFitter.h"

#include <stdio.h>
#include <math.h>

void F_test_commented (int Nbins=20, int Ndata=5000, double alpha=0.1) {

  cout << endl;
  cout << " F - test " << endl;
  cout << " -------- " << endl;
  cout << endl;
  cout << " Will test pseudo-data with a size alpha = " << alpha << endl;
  cout << endl;
  if (Nbins>1000) {
    cout << "Too many bins, max 1000. Stopping. Edit code and increase arrays size." << endl;
    return;
  }

  gStyle->SetOptFit(111111);
  gStyle->SetOptStat(0);

  // Change preset generator
  // -----------------------
  delete gRandom;
  TRandom3 * myRNG = new TRandom3();
  gRandom = myRNG;
  // To read and write random data:
  // ------------------------------
  // gRandom->ReadRandom("random.dat");
  // gRandom->WriteRandom("random.dat");

  TH1D * Data0 = new TH1D ("Data0", "", Nbins, 0., 100.);
  TH1D * Data1 = new TH1D ("Data1", "", Nbins, 0., 100.);
  TH1D * Data2 = new TH1D ("Data2", "", Nbins, 0., 100.);
  TH1D * Data3 = new TH1D ("Data3", "", Nbins, 0., 100.);

  // F distributions
  // ---------------
  double xmaxF = 20.;
  int NbinsF = 100;
  TH1D * Fdistr10 = new TH1D ("Fdistr10", "", NbinsF, 0., xmaxF);
  TH1D * Fdistr21 = new TH1D ("Fdistr21", "", NbinsF, 0., xmaxF);
  TH1D * Fdistr32 = new TH1D ("Fdistr32", "", NbinsF, 0., xmaxF);
  TH1D * Fdistr10extr = new TH1D ("Fdistr10extr", "", NbinsF, 0., xmaxF);
  TH1D * Fdistr21extr = new TH1D ("Fdistr21extr", "", NbinsF, 0., xmaxF);
  TH1D * Fdistr32extr = new TH1D ("Fdistr32extr", "", NbinsF, 0., xmaxF);
  TH1D * Finteg10 = new TH1D ("Finteg10", "", NbinsF, 0., xmaxF);
  TH1D * Finteg21 = new TH1D ("Finteg21", "", NbinsF, 0., xmaxF);
  TH1D * Finteg32 = new TH1D ("Finteg32", "", NbinsF, 0., xmaxF);

  for (int i=0; i<Ndata; i++) {
    double x = gRandom->Uniform(0.,20.);
    // Just for fun, give some quadratic dependence to the data
    // --------------------------------------------------------
    Data0->Fill(x*x-15*x);
    Data1->Fill(x*x-15*x);
    Data2->Fill(x*x-15*x);
    Data3->Fill(x*x-15*x);
  }
  double y0[1000];
  double y1[1000];
  double y2[1000];
  double y3[1000];
  for (int i=0; i<Nbins; i++) {
    y0[i] = Data0->GetBinContent(i+1);
    y1[i] = Data1->GetBinContent(i+1);
    y2[i] = Data2->GetBinContent(i+1);
    y3[i] = Data3->GetBinContent(i+1);
  }

  TF1 * Pol0 = new TF1 ("Pol0","[0]", 0., 100.);
  TF1 * Pol1 = new TF1 ("Pol1","[0]+[1]*x", 0., 100.);
  TF1 * Pol2 = new TF1 ("Pol2","[0]+[1]*x+[2]*x*x", 0., 100.);
  TF1 * Pol3 = new TF1 ("Pol3","[0]+[1]*x+[2]*x*x+[3]*x*x*x", 0., 100.);
  Pol0->SetParameter(0,1.);
  Pol1->SetParameter(0,1.);
  Pol1->SetParameter(1,0.);
  Pol2->SetParameter(0,1.);
  Pol2->SetParameter(1,0.);
  Pol2->SetParameter(2,0.);
  Pol3->SetParameter(0,1.);
  Pol3->SetParameter(1,0.);
  Pol3->SetParameter(2,0.);
  Pol3->SetParameter(3,0.);
  double Npar0=1.;
  double Npar1=2.;
  double Npar2=3.;
  double Npar3=4.;

  Data0->Fit("Pol0","QN");
  double p00 = Pol0->GetParameter(0);
  Data1->Fit("Pol1","QN");
  double p10 = Pol1->GetParameter(0);
  double p11 = Pol1->GetParameter(1);
  Data2->Fit("Pol2","QN");
  double p20 = Pol2->GetParameter(0);
  double p21 = Pol2->GetParameter(1);
  double p22 = Pol2->GetParameter(2);
  Data3->Fit("Pol3","QN");
  Data3->Fit("Pol3","QN");
  double p30 = Pol3->GetParameter(0);
  double p31 = Pol3->GetParameter(1);
  double p32 = Pol3->GetParameter(2);
  double p33 = Pol3->GetParameter(3);

  // Calculate residual sums of squares for the three hypotheses
  // -----------------------------------------------------------
  double rss0=0;
  double rss1=0;
  double rss2=0;
  double rss3=0;
  for (int i=0; i<Nbins; i++) {
    double x = (i+0.5)*100./(double)Nbins;
    rss0+= pow(y0[i]-Pol0->Eval(x),2);
    rss1+= pow(y1[i]-Pol1->Eval(x),2);
    rss2+= pow(y2[i]-Pol2->Eval(x),2);
    rss3+= pow(y3[i]-Pol3->Eval(x),2);
  }

  // F-values
  // --------
  double p1_10 = Npar1-Npar0;
  double p2_10 = Nbins-Npar1;
  double p1_21 = Npar2-Npar1;
  double p2_21 = Nbins-Npar2;
  double p1_32 = Npar3-Npar2;
  double p2_32 = Nbins-Npar3;
  double Ftest_10 = (rss0-rss1)/p1_10 / (rss1/p2_10);
  double Ftest_21 = (rss1-rss2)/p1_21 / (rss2/p2_21);
  double Ftest_32 = (rss2-rss3)/p1_32 / (rss3/p2_32);

/*   cout << "Rss0 = " << rss0 << endl; */
/*   cout << "Rss1 = " << rss1 << endl; */
/*   cout << "Rss2 = " << rss2 << endl; */
/*   cout << "Rss3 = " << rss3 << endl; */
/*   cout << "Ftest 10 result = " << Ftest_10 << endl; */
/*   cout << "Ftest 21 result = " << Ftest_21 << endl; */
/*   cout << "Ftest 32 result = " << Ftest_32 << endl; */

  double good_CL10 =  1.-TMath::FDistI(Ftest_10,p1_10,p2_10);
  double good_CL21 =  1.-TMath::FDistI(Ftest_21,p1_21,p2_21);
  double good_CL32 =  1.-TMath::FDistI(Ftest_32,p1_32,p2_32);
  cout << " Ftest10 = " << 1.-TMath::FDistI(Ftest_10,p1_10,p2_10) << endl;
  cout << " Ftest21 = " << 1.-TMath::FDistI(Ftest_21,p1_21,p2_21) << endl;
  cout << " Ftest32 = " << 1.-TMath::FDistI(Ftest_32,p1_32,p2_32) << endl;

  cout << endl;
  cout << " F - test results: " << endl;
  cout << " ----------------- " << endl;
  cout << endl;
  if (good_CL10>alpha) {
    cout << " No need for anything more than a constant fit." << endl;
  } else if (good_CL21>alpha) {
    cout << " A linear fit is sufficient to describe these data." << endl;
  } else if (good_CL32>alpha) {
    cout << " A quadratic fit appears adequate to describe these data." << endl;
  } else {
    cout << " A cubic fit is needed for these data. " << endl;
    cout << " You should check for higher-order polynomials. " << endl;
  }
  cout << endl;

  // Create the histograms of F distributions and cumulatives
  // --------------------------------------------------------
  for (int i=0; i<NbinsF; i++) {
    double x = (i+0.5)*xmaxF/(double)NbinsF;
    double Fvalue10 = sqrt(pow(p1_10*x,p1_10)*pow(p2_10,p2_10)/pow(p1_10*x+p2_10,p1_10+p2_10))/(x*TMath::Beta(0.5*p1_10,0.5*p2_10));
    double Fvalue21 = sqrt(pow(p1_21*x,p1_21)*pow(p2_21,p2_21)/pow(p1_21*x+p2_21,p1_21+p2_21))/(x*TMath::Beta(0.5*p1_21,0.5*p2_21));
    argbeta_10 = p1_10*x/(p1_10*x+p2_10);
    double Fvalue32;
    double Fsum10 = TMath::BetaIncomplete(argbeta_10,0.5*p1_10,0.5*p2_10)/TMath::Beta(0.5*p1_10,0.5*p2_10);
    argbeta_21 = p1_21*x/(p1_21*x+p2_21);
    double Fsum21 = TMath::BetaIncomplete(argbeta_21,0.5*p1_21,0.5*p2_21)/TMath::Beta(0.5*p1_21,0.5*p2_21);    
    double Fsum32;
    // Actually do it with built-in functions - the above work strange in some regimes
    // -------------------------------------------------------------------------------
    Fvalue10 = TMath::FDist(x,p1_10,p2_10);
    Fvalue21 = TMath::FDist(x,p1_21,p2_21);
    Fvalue32 = TMath::FDist(x,p1_32,p2_32);
    Fsum10 = TMath::FDistI(x,p1_10,p2_10);
    Fsum21 = TMath::FDistI(x,p1_21,p2_21);
    Fsum32 = TMath::FDistI(x,p1_32,p2_32);
    
    Fdistr10->SetBinContent(i+1,Fvalue10);
    Fdistr21->SetBinContent(i+1,Fvalue21);
    Fdistr32->SetBinContent(i+1,Fvalue32);
    if (x>Ftest_10) Fdistr10extr->SetBinContent(i+1,Fvalue10);
    if (x>Ftest_21) Fdistr21extr->SetBinContent(i+1,Fvalue21);
    if (x>Ftest_32) Fdistr32extr->SetBinContent(i+1,Fvalue32);
    Finteg10->SetBinContent(i+1,Fsum10);
    Finteg21->SetBinContent(i+1,Fsum21);
    Finteg32->SetBinContent(i+1,Fsum32);
  }

/*   TCanvas * Ftest = new TCanvas ("Ftest", "F test", 500, 500); */
/*   Ftest->Divide(1,3); */
/*   Ftest->cd(1); */
/*   Data0->SetMinimum(0); */
/*   Data0->SetMarkerStyle(20); */
/*   Data0->SetMarkerSize(0.3); */
/*   Data0->Draw("PE"); */
/*   Pol0->SetLineColor(kRed); */
/*   Pol0->SetLineWidth(1); */
/*   Pol0->Draw("SAME"); */
/*   Ftest->cd(2); */
/*   Data1->SetMinimum(0); */
/*   Data1->SetMarkerStyle(20); */
/*   Data1->SetMarkerSize(0.3); */
/*   Data1->Draw("PE"); */
/*   Pol1->SetLineColor(kRed); */
/*   Pol1->SetLineWidth(1); */
/*   Pol1->Draw("SAME"); */
/*   Ftest->cd(3); */
/*   Data2->SetMinimum(0); */
/*   Data2->SetMarkerStyle(20); */
/*   Data2->SetMarkerSize(0.3); */
/*   Data2->Draw("PE"); */
/*   Pol2->SetLineColor(kRed); */
/*   Pol2->SetLineWidth(1); */
/*   Pol2->Draw("SAME"); */

  Fdistr10extr->SetFillColor(kBlue);
  Fdistr21extr->SetFillColor(kBlue);
  Fdistr32extr->SetFillColor(kBlue);
  TArrow * Arrow1 = new TArrow (Ftest_10,0.8,Ftest_10,0.1);
  TArrow * Arrow2 = new TArrow (Ftest_21,0.8,Ftest_21,0.1);
  TArrow * Arrow3 = new TArrow (Ftest_32,0.8,Ftest_32,0.1);
  TArrow * Arrow1i = new TArrow (Ftest_10,0.1,Ftest_10,0.8);
  TArrow * Arrow2i = new TArrow (Ftest_21,0.1,Ftest_21,0.8);
  TArrow * Arrow3i = new TArrow (Ftest_32,0.1,Ftest_32,0.8);
  Arrow1->SetLineWidth(2);
  Arrow1i->SetLineWidth(2);
  Arrow2->SetLineWidth(2);
  Arrow2i->SetLineWidth(2);
  Arrow3->SetLineWidth(2);
  Arrow3i->SetLineWidth(2);
  Arrow1->SetLineColor(kBlue);
  Arrow1i->SetLineColor(kBlue);
  Arrow2->SetLineColor(kBlue);
  Arrow2i->SetLineColor(kBlue);
  Arrow3->SetLineColor(kBlue);
  Arrow3i->SetLineColor(kBlue);

  TCanvas * Ftest10 = new TCanvas ("Ftest10", "F test", 500, 500);
  Ftest10->Divide(2,2);
  Ftest10->cd(1);
  Data0->SetMinimum(0);
  Data0->SetMarkerStyle(20);
  Data0->SetMarkerSize(0.7);
  Data0->Draw("PE");
  Pol0->SetLineColor(kRed);
  Pol0->SetLineWidth(1);
  Pol0->Draw("SAME");
  Ftest10->cd(2);
  Data1->SetMinimum(0);
  Data1->SetMarkerStyle(20);
  Data1->SetMarkerSize(0.7);
  Data1->Draw("PE");
  Pol1->SetLineColor(kRed);
  Pol1->SetLineWidth(1);
  Pol1->Draw("SAME");
  Ftest10->cd(3);
  Ftest10->GetPad(3)->SetLogy();
  Fdistr10->Draw();
  Fdistr10extr->Draw("SAME");
  //Arrow1->Draw();
  Ftest10->cd(4);
  Finteg10->SetMinimum(0);
  Finteg10->Draw();
  Arrow1i->Draw();
  Ftest10->Print("F_test_1.jpg");

  TCanvas * Ftest21 = new TCanvas ("Ftest21", "F test", 500, 500);
  Ftest21->Divide(2,2);
  Ftest21->cd(1);
  Data1->SetMinimum(0);
  Data1->SetMarkerStyle(20);
  Data1->SetMarkerSize(0.7);
  Data1->Draw("PE");
  Pol1->SetLineColor(kRed);
  Pol1->SetLineWidth(1);
  Pol1->Draw("SAME");
  Ftest21->cd(2);
  Data2->SetMinimum(0);
  Data2->SetMarkerStyle(20);
  Data2->SetMarkerSize(0.7);
  Data2->Draw("PE");
  Pol2->SetLineColor(kRed);
  Pol2->SetLineWidth(1);
  Pol2->Draw("SAME");
  Ftest21->cd(3);
  Ftest21->GetPad(3)->SetLogy();
  Fdistr21->Draw();
  Fdistr21extr->Draw("SAME");
  //Arrow2->Draw();
  Ftest21->cd(4);
  Finteg21->SetMinimum(0);
  Finteg21->Draw();
  Arrow2i->Draw();
  Ftest21->Print("F_test_2.jpg");

  TCanvas * Ftest32 = new TCanvas ("Ftest32", "F test", 500, 500);
  Ftest32->Divide(2,2);
  Ftest32->cd(1);
  Data2->SetMinimum(0);
  Data2->SetMarkerStyle(20);
  Data2->SetMarkerSize(0.7);
  Data2->Draw("PE");
  Pol2->SetLineColor(kRed);
  Pol2->SetLineWidth(1);
  Pol2->Draw("SAME");
  Ftest32->cd(2);
  Data3->SetMinimum(0);
  Data3->SetMarkerStyle(20);
  Data3->SetMarkerSize(0.7);
  Data3->Draw("PE");
  Pol3->SetLineColor(kRed);
  Pol3->SetLineWidth(1);
  Pol3->Draw("SAME");
  Ftest32->cd(3);
  Ftest32->GetPad(3)->SetLogy();
  Fdistr32->Draw();
  Fdistr32extr->Draw("SAME");
  //Arrow2->Draw();
  Ftest32->cd(4);
  Finteg32->SetMinimum(0);
  Finteg32->Draw();
  Arrow3i->Draw();
  Ftest32->Print("F_test_3.jpg");

  TCanvas * P2 = new TCanvas ("P2","",300,600);
  P2->Divide(2,2);
  P2->cd(1);
  Data0->Fit("Pol0", "Q");
  Data0->Draw("PE");
  P2->cd(2);
  Data1->Fit("Pol1", "Q");
  Data1->Draw("PE");
  P2->cd(3);
  Data2->Fit("Pol2", "Q");
  Data2->Draw("PE");
  P2->cd(4);
  Data3->Fit("Pol3", "Q");
  Data3->Draw("PE");
  P2->Print("F_test_0.jpg");
  
}
