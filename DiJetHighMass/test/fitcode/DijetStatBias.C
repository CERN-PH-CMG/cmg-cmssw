#include "TH2.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TCanvas.h" 
#include "TFile.h"
#include "TMath.h"
#include "TMinuit.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TRandom.h"
#include "TRandom3.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void DijetStatBias (int Npexp = 250, bool fixp4=false, double kappa=0)
{

  // This macro checks the result of exo-11-095
  // where a 4-par function f(x) is used to fit points
  // in a two-body spectrum, to extract limits on potential signals.
  // We check for a bias in signal size as returned by the simple
  // fit to f(x) plus a Gaussian signal shape (with fixed mass and sigma)
  // where the f(x) parameters are fixed to the no-signal fit result.
  //
  // NNBB The generated spectra are just a mock-up of those of EXO-11-095;
  // histograms have fixed 100-GeV bins (not a function of mass as in the
  // original ones); the fitting method is also much simplified. The real
  // bias, if present, must be checked with the actual methodology used in 
  // the analysis.
  //
  // Npexp = number of toys per mass point (5 mass points at 1600,2000,2400,2800,3200 GeV)
  // fixp4 = whether to fix bgr shape from bgr-only fit in b+s fit or not
  // kappa = signal/background at signal peak 
  // --------------------------------------------

  // Change preset generator
  // -----------------------
  delete gRandom;
  TRandom3 * myRNG = new TRandom3();
  gRandom = myRNG;
  // To read and write random data:
  // ------------------------------
  // gRandom->ReadRandom("random.dat");
  // gRandom->WriteRandom("random.dat");

  cout << "***************************************************" << endl;

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111);

  int Nbins=33;
  double xmin;
  double xmax;
  xmin=900.;
  xmax=4200.;

  TH1D * M12_meas = new TH1D ("M12_meas", "M12", Nbins, xmin, xmax);
  TF1 * P3 = new TF1 ("P3","[0]*pow((1-x/7000),[1])/pow(x/7000,[2])", xmin, xmax);
  TF1 * P4 = new TF1 ("P4","[0]*pow((1-x/7000),[1])/pow(x/7000,[2]+[3]*log(x/7000))",
                      xmin,xmax);
  P4->SetLineColor(kBlue);
  P4->SetLineWidth(1);
  TF1 * G = new TF1 ("G", "[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * P4G = new TF1 ("P4G","[0]*pow((1-x/7000),[1])/pow(x/7000,[2]+[3]*log(x/7000))+[4]*exp(-0.5*pow((x-[5])/[6],2))", xmin, xmax);
  P4G->SetLineColor(kRed);
  P4G->SetLineWidth(1);

  TH1D * Pull[10];
  TH1D * Chi2[10];
  char name[20];
  char nameChi2[10];
  for (int i=0; i<5; i++) {
    sprintf (name, "Pull%d", i);
    sprintf (nameChi2, "Chi2%d", i);
    Pull[i]= new TH1D (name, name, 50, -10., 10.);
    Chi2[i]= new TH1D (nameChi2, nameChi2, 50, 0, 200.);
  }
  TH1D * M12[10];
  for (int i=0; i<5; i++) {
    sprintf (name, "M12%d", i);
    M12[i] = new TH1D (name,name, Nbins, xmin, xmax);
  }

  // Actual param values in main fit of Exo-11-095
  //   double p40 = 0.000002071;
  //   double p41 = 10.04;
  //   double p42 = 6.607;
  //   double p43 = 0.4619;

  double p40 = 1.73132e-05*4974*100;
  double p41 = 6.80678e+00;
  double p42 = 6.33620e+00;
  double p43 = 1.93728e-01;
  P4->SetParameters(p40,p41,p42,p43);

  double p30 = 4.0168;
  double p31 = 6.15835;
  double p32 = 5.95065;
  P3->SetParameters(p30,p31,p32);

  double p400[10];
  double p410[10];
  double p420[10];
  double p430[10];

  // Loop on five mass hypotheses: 1600,2000,2400,2800,3200 GeV
  // ----------------------------------------------------------
  for (int i=0; i<5; i++) {

    // We generate a signal at i*400+1600 GeV
    // of width m/20
    // and normalization set by user (see kappa)
    // -----------------------------------------
    double gp1 = (double)i*400+1600;
    double gp2 = gp1/20.;  
    double gp0 = P4->Eval(gp1); // NNBB P4 for now


    double f = 0;
    f = sqrt(P4->Integral(gp1, gp1+100)/100)*kappa;
       
    /*
    if (i == 0) f = 1000.;
    if (i == 1) f = 500.;
    if (i == 2) f = 250.;
    if (i == 3) f = 100.;
    if (i == 4) f = 20.;
    */
    


    G->SetParameters(gp0,gp1,gp2);
    double dint = G->Integral(xmin,xmax);

    double gp0 = gp0/dint*f;

    G->SetParameter(0, gp0);
    double dint = G->Integral(xmin,xmax);


    int gint = abs(G->Integral(xmin,xmax));
    G->FixParameter(1,gp1);
    G->FixParameter(2,gp2);

    cout << "gp1 = " << gp1 << " gp2 = " << gp2 << " gp0 = " << gp0 << " P4 total integral = " << P4->Integral(xmin, xmax) << " integral = " << dint << " gint = " << gint << endl;

    double binMiddle = 0;


    for (int j=0; j<Npexp; j++) {
      P4->SetParameters(p40,p41,p42,p43);
      double P4GParam = gp0*(xmax-xmin)/(double)Nbins;
      P4G->SetParameters(p40,p41,p42,p43,P4GParam,gp1,gp2);
      P4G->SetParLimits(4,0, 1e5);
      P4G->FixParameter(5,gp1);
      P4G->FixParameter(6,gp2);

      // Generate pseudo-data according to original P4
      // ---------------------------------------------
      M12_meas->Reset();
      M12_meas->FillRandom("P4",1000000); // NB we generate with same shape for now
      M12_meas->FillRandom("G",gint);
      // Get P4 parameters from bgr-only fit
      // -----------------------------------

      if (j==0) {
	binMiddle = M12_meas->FindBin(gp1);
	cout << "binMiddle = " << binMiddle << " binCenter = " << M12_meas->GetBinCenter(binMiddle) << " binContent = "
	     << M12_meas->GetBinContent(binMiddle) << endl;
      }

      //      M12_meas->Sumw2();

      //      M12_meas->SetBinError(binMiddle-2, 1.e10);
      //      M12_meas->SetBinError(binMiddle-1, 1.e10);
      //      M12_meas->SetBinError(binMiddle, 1.e10);
      //      M12_meas->SetBinError(binMiddle+1, 1.e10);
      
      M12_meas->Fit("P4","QNLL");

      double chi2 = P4->GetChisquare();

      if (fixp4) {
        P4G->FixParameter(0,P4->GetParameter(0));
        P4G->FixParameter(1,P4->GetParameter(1));
        P4G->FixParameter(2,P4->GetParameter(2));
        P4G->FixParameter(3,P4->GetParameter(3));
      }
      
      //      M12_meas->SetBinError(binMiddle-2, sqrt(M12_meas->GetBinContent(binMiddle-2)));
      //      M12_meas->SetBinError(binMiddle-1, sqrt(M12_meas->GetBinContent(binMiddle-1)));
      //      M12_meas->SetBinError(binMiddle, sqrt(M12_meas->GetBinContent(binMiddle)));
      //      M12_meas->SetBinError(binMiddle+1, sqrt(M12_meas->GetBinContent(binMiddle+1)));
      
      M12_meas->Fit("P4G","QNLL");
      if (!fixp4) chi2 = P4G->GetChisquare();


      //      M12_meas->Draw("H");
      double pull;

      if (!fixp4) chi2 = P4G->GetChisquare();

      if (P4G->GetParError(4) > 0) {
	double binWidth = (xmax-xmin)/Nbins;
	double fittedRate = gp0*binWidth;
	double param =  P4G->GetParameter(4);
	double paramErr = P4G->GetParError(4);
	double totNumEvents = M12_meas->Integral();
        pull = (param-fittedRate)/paramErr;
	//	if (j%50 == 0 && i < 4) 
	cout << param << " " << fittedRate << " bin width = " << binWidth << " P4G error " << paramErr << " integral = " << gint << " total number of events = " << totNumEvents << endl;
        Pull[i]->Fill(pull);
	Chi2[i]->Fill(chi2);
      }

      // Store one spectrum for display purpose
      // --------------------------------------
      if (j==0) {
        p400[i]=P4->GetParameter(0);
        p410[i]=P4->GetParameter(1);
        p420[i]=P4->GetParameter(2);
        p430[i]=P4->GetParameter(3);
        for (int k=1; k<=Nbins; k++) {
          M12[i]->SetBinContent(k,M12_meas->GetBinContent(k));
        }
	M12[i]->Sumw2();
	

	//	M12[i]->SetBinError(binMiddle-2, 1e10);
	//	M12[i]->SetBinError(binMiddle-1, 1e10);
	//	M12[i]->SetBinError(binMiddle, 1e10);
	//	M12[i]->SetBinError(binMiddle+1, 1e10);

	M12[i]->Fit("P4G","QLL");
	
      }
    }
  }

  // Plot results
  // ------------

  TF1* gauss = new TF1("gauss", "[2]*TMath::Gaus(x, [0], [1])");

  TCanvas * Res = new TCanvas ("Res","Res", 1200, 800);
  Res->Divide(4,5);
  for (int i=1; i<=5; i++) {
    Res->cd(i*4-3);

    Pull[i-1]->SetLabelSize(0.07, "X");

    Pull[i-1]->Draw();

    gauss->SetParameters(Pull[i-1]->GetMean(), 1, Pull[i-1]->Integral()*sqrt(2*TMath::Pi())* 1);
    //    Pull[i-1]->Fit("gauss");

    TPaveText* p = new TPaveText(0.2, 0.7, 0.4, 0.9,"NDC");
    //    p->AddText(Form("Bias = %.1f",gauss->GetParameter(0)));
    p->AddText(Form("Bias = %.1f",Pull[i-1]->GetMean()));
    p->Draw();

    Res->cd(i*4-2);
    Res->GetPad(i*4-2)->SetLogy();
    if (fixp4) M12[i-1]->Fit("P4");
    if (!fixp4) M12[i-1]->Fit("P4G");
    M12[i-1]->Draw("PE");

    Res->cd(i*4-1);
    TH1F* LocalPull = M12[i-1]->Clone("LocalPull");
    LocalPull->Clear();
    
    
    for (int k = 1; k < LocalPull->GetNbinsX()+1; k++){

      double binCenter = M12[i-1]->GetBinCenter(k);
      double bkg =  P4->Eval(binCenter);
      if (!fixp4) bkg =  P4G->Eval(binCenter);
      double val =  M12[i-1]->GetBinContent(k);

      if (val > 0.1) {
	LocalPull->SetBinContent(k, (val-bkg)/sqrt(val));
	LocalPull->SetBinError(k, 1);
      }

    }

    LocalPull->SetMaximum(6);
    LocalPull->SetMinimum(-4);
    LocalPull->Draw();
    LocalPull->SetStats(0);

    TLine* L = new TLine(xmin, 0, xmax, 0);
    L->SetLineColor(kRed);
    L->SetLineStyle(1);
    L->Draw("SAME");

    TPaveText* p3 = new TPaveText(0.2, 0.7, 0.4, 0.9,"NDC");
    if (fixp4) p3->AddText(Form("chi2 = %.1f",P4->GetChisquare()));
    else p3->AddText(Form("chi2 = %.1f",P4G->GetChisquare()));
    p3->Draw();

    Res->cd(i*4);
    Chi2[i-1]->Draw();
    TPaveText* p4 = new TPaveText(0.6, 0.7, 0.9, 0.9,"NDC");
    p4->AddText(Form("Mean #chi2 = %.1f",Chi2[i-1]->GetMean()));
    p4->Draw();


  }
  if (fixp4) {
    Res->Print(Form("EXO11095Bias_5pct_%.0fSqrtB.png",kappa));
  } else {
    Res->Print(Form("EXO11095noBias_5pct_%.0fSqrtB.png",kappa));
  }


  // End of program
  // --------------
  gROOT->Time();

}
