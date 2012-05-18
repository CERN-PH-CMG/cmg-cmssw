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
#include "TVirtualFitter.h"

#include <stdio.h>
#include <math.h>



// Default fit with 4 parameters
Double_t fitQCD4( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0]*pow(1.-x,p[1])/pow(x,p[2]+p[3]*log(x));
}

// 3 parameters
Double_t fitQCD3( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0]*pow(1.-x,p[1])/pow(x,p[2]);
}

// 2 parameters alternative
Double_t fitQCD2( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0]/pow(x,p[1]);
}

// 1 parameter
Double_t fitQCD1( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0];
}

double lumi = 4997.0;

void F_test_default_PYTHIA (double alpha=0.1) {

  cout << endl;
  cout << " F - test " << endl;
  cout << " -------- " << endl;
  cout << endl;
  cout << " Will test pseudo-data with a size alpha = " << alpha << endl;
  cout << endl;

  gStyle->SetOptFit(111111);
  gStyle->SetOptStat(0);

  // Input Files  
  //  TFile *inputFile = TFile::Open("../../data/histograms_data_HT_Run2011AB_160404_180252_Fat30_ak5_4677pbm1.root", "READ");

  //  TFile *inputFile = TFile::Open("../../data/histograms_data_HT_Run2011AB_160404_180252_PF_ak5_4677pbm1.root", "READ");

  TFile *inputFile = TFile::Open("../../data/histograms_Fat30_summer11_mc_ak5.root", "READ");

  // Histograms 
  hDijetMass = (TH1F*) inputFile->Get("h_DijetMass_data_fat;1");

  double lumi = 4974.0*5;
  double scale = 1.22;

  hDijetMass->Scale(lumi*scale);

  TRandom3* gRandom3 = new TRandom3(1);

  for (int i = 36; i < hDijetMass->GetNbinsX(); i++){

    double binN = gRandom3->Poisson(hDijetMass->GetBinContent(i));
    hDijetMass->SetBinContent(i,binN);
  }


  TH1D * Data = hDijetMass.Clone("Data");
  //  double mMin = 890.0;
  //  double mMax = 4300.0;
  double mMin = 940.0;
  double mMax = 4500.0;

  double binMin = hDijetMass->FindBin(mMin);
  double binMax = hDijetMass->FindBin(mMax);
  double Nbins = binMax-binMin;

  cout << "binMin = " << binMin << " binMax = " << binMax << " Nbins = " << Nbins << endl;


  hDijetMass->GetXaxis().SetRangeUser(mMin,mMax);

  //  double mMax = 4171;


  float a = 0.3173/2;
  float n,nl,nh;
  float n, dm, mass, xl, xh;
  float vx[1000],vy[1000],vexl[1000],vexh[1000],veyl[1000],veyh[1000];
  int i;
  float y, yplus, yminus, cplus, cminus,e;

  for(i=0;i<hDijetMass->GetNbinsX();i++)
    {
      n    = hDijetMass->GetBinContent(i+1);
      dm   = hDijetMass->GetBinWidth(i+1);
      mass = hDijetMass->GetBinCenter(i+1);
      xl   = hDijetMass->GetBinLowEdge(i+1);	
      xh   = xl+dm; 	
      vx[i]   = (xl+xh)/2.;
      vexl[i] = dm/2.;
      vexh[i] = dm/2.;
      vy[i]   = n / dm; 

     
      if (n<25 && mass>mMin && mass<=mMax)
       	{
	  nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
	  nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
	  veyl[i] = nl/dm;
	  veyh[i] = nh/dm;  
	}
      else if (n<25 && mass<=mMin && mass>mMax && n>0)
	{
	  nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
	  nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
	  veyl[i] = nl/(dm);
	  veyh[i] = nh/(dm);  
	}
      else if (n>=25 && mass >= 889)
	{
	  veyl[i] = sqrt(n)/(dm);
	  veyh[i] = sqrt(n)/(dm);
	} 
      else 
	{     
	  vy[i] = -1.0;
	  veyl[i] = 0;
	  veyh[i] = 0;  
	}

 
    }
  // these are the graph format of data.

  TGraphAsymmErrors *Data0 = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
  TGraphAsymmErrors *Data1 = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
  TGraphAsymmErrors *Data2 = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
  TGraphAsymmErrors *Data3 = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);

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

  double y0[3000];
  double y1[3000];
  double y2[3000];
  double y3[3000];
  for (int i=binMin; i<binMax+1; i++) {
    double x, yP;
    Data0->GetPoint(i, x, yP);
    y0[i] = yP;
    y1[i] = yP;
    y2[i] = yP;
    y3[i] = yP;
  }

  TF1 * Pol0 = new TF1("Pol0",fitQCD1,mMin,mMax,1);
  TF1 * Pol1 = new TF1("Pol1",fitQCD2,mMin,mMax,2);
  TF1 * Pol2 = new TF1("Pol2",fitQCD3,mMin,mMax,3);
  TF1 * Pol3 = new TF1("Pol3",fitQCD4,mMin,mMax,4);
  Pol0->SetParameter(0,1.73132e-05*lumi/1.3);
  Pol1->SetParameter(0,1.73132e-05*lumi/1.3);
  Pol1->SetParameter(1,6.80678e+00);
  Pol2->SetParameter(0,1.73132e-05*lumi/1.3);
  Pol2->SetParameter(1,6.80678e+00);
  Pol2->SetParameter(2,6.33620e+00);
  Pol3->SetParameter(0,1.73132e-05*lumi/1.3);
  Pol3->SetParameter(1,6.80678e+00);
  Pol3->SetParameter(2,6.33620e+00);
  Pol3->SetParameter(3,1.93728e-01);
  double Npar0=1.;
  double Npar1=2.;
  double Npar2=3.;
  double Npar3=4.;

  Data0->Fit("Pol0","N");
  Data1->Fit("Pol1","N");
  Data2->Fit("Pol2","QN");
  Data2->Fit("Pol2","N");
  Data3->Fit("Pol3","QN");
  Data3->Fit("Pol3","QN");
  Data3->Fit("Pol3","N");

  // Calculate residual sums of squares for the three hypotheses
  // -----------------------------------------------------------
  double rss0=0;
  double rss1=0;
  double rss2=0;
  double rss3=0;
  for (int i=binMin; i<binMax+1; i++) {
    double x, yP;
    Data0->GetPoint(i, x,y);
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

  cout << "Rss0 = " << rss0 << endl;
  cout << "Rss1 = " << rss1 << endl;
  cout << "Rss2 = " << rss2 << endl;
  cout << "Rss3 = " << rss3 << endl;
  cout << "Ftest 10 result = " << Ftest_10 << endl;
  cout << "Ftest 21 result = " << Ftest_21 << endl;
  cout << "Ftest 32 result = " << Ftest_32 << endl;

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
  Data0->Draw("APE");
  Pol0->SetLineColor(kRed);
  Pol0->SetLineWidth(1);
  Pol0->Draw("SAME");
  Ftest10->cd(2);
  Data1->SetMinimum(0);
  Data1->SetMarkerStyle(20);
  Data1->SetMarkerSize(0.7);
  Data1->Draw("APE");
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
  Data1->Draw("APE");
  Pol1->SetLineColor(kRed);
  Pol1->SetLineWidth(1);
  Pol1->Draw("SAME");
  Ftest21->cd(2);
  Data2->SetMinimum(0);
  Data2->SetMarkerStyle(20);
  Data2->SetMarkerSize(0.7);
  Data2->Draw("APE");
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
  Data2->Draw("APE");
  Pol2->SetLineColor(kRed);
  Pol2->SetLineWidth(1);
  Pol2->Draw("SAME");
  Ftest32->cd(2);
  Data3->SetMinimum(0);
  Data3->SetMarkerStyle(20);
  Data3->SetMarkerSize(0.7);
  Data3->Draw("APE");
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
  Data0->Draw("APE");
  P2->cd(2);
  Data1->Fit("Pol1", "Q");
  Data1->Draw("PE");
  P2->cd(3);
  Data2->Fit("Pol2", "Q");
  Data2->Draw("APE");
  P2->cd(4);
  Data3->Fit("Pol3", "Q");
  Data3->Draw("APE");
  P2->Print("F_test_0.jpg");
  
}
