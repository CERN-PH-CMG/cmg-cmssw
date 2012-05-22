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

void DijetStatBias (int Npexp = 500, bool fixp4=true, double kappa=0.001)
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
  char name[20];
  for (int i=0; i<5; i++) {
    sprintf (name, "Pull%d", i);
    Pull[i]= new TH1D (name, name, 100, -10., 10.);
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
    double gp2 = gp1/10.;  
    double gp0 = kappa*P4->Eval(gp1); // NNBB P4 for now
    double f = 0;
    if (i == 0) f = 1000.;
    if (i == 1) f = 500.;
    if (i == 2) f = 250.;
    if (i == 3) f = 100.;
    if (i == 4) f = 20.;


    G->SetParameters(gp0,gp1,gp2);
    double dint = G->Integral(xmin,xmax);

    double gp0 = gp0/dint*f;

    G->SetParameter(0, gp0);
    double dint = G->Integral(xmin,xmax);


    int gint = abs(G->Integral(xmin,xmax));
    G->FixParameter(1,gp1);
    G->FixParameter(2,gp2);

    cout << "gp1 = " << gp1 << " gp2 = " << gp2 << " gp0 = " << gp0 << " P4->Eval(gp1) = " << P4->Eval(gp1)*100 << " integral = " << dint << " gint = " << gint << endl;

    double binMiddle = 0;


    for (int j=0; j<Npexp; j++) {
      P4->SetParameters(p40,p41,p42,p43);
      double P4GParam = gp0*(xmax-xmin)/(double)Nbins;
      P4G->SetParameters(p40,p41,p42,p43,P4GParam,gp1,gp2);
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
	cout << "binMiddle = " << binMiddle << " binCenter = " << M12_meas->GetBinCenter(binMiddle) << endl;
      }

      M12_meas->Sumw2();

      M12_meas->SetBinError(binMiddle-2, 1.e10);
      M12_meas->SetBinError(binMiddle-1, 1.e10);
      M12_meas->SetBinError(binMiddle, 1.e10);
      M12_meas->SetBinError(binMiddle+1, 1.e10);
      
      M12_meas->Fit("P4","QN");

      if (fixp4) {
        P4G->FixParameter(0,P4->GetParameter(0));
        P4G->FixParameter(1,P4->GetParameter(1));
        P4G->FixParameter(2,P4->GetParameter(2));
        P4G->FixParameter(3,P4->GetParameter(3));
      }

      //      M12_meas->SetBinError(binMiddle-2, sqrt(M12_meas->GetBinContent(binMiddle-2)));
      M12_meas->SetBinError(binMiddle-1, sqrt(M12_meas->GetBinContent(binMiddle-1)));
      M12_meas->SetBinError(binMiddle, sqrt(M12_meas->GetBinContent(binMiddle)));
      //      M12_meas->SetBinError(binMiddle+1, sqrt(M12_meas->GetBinContent(binMiddle+1)));
      
      M12_meas->Fit("P4G","QNLL");
      //      M12_meas->Draw("H");
      double pull;
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
	

	M12[i]->SetBinError(binMiddle-2, 1e10);
	M12[i]->SetBinError(binMiddle-1, 1e10);
	M12[i]->SetBinError(binMiddle, 1e10);
	M12[i]->SetBinError(binMiddle+1, 1e10);

	M12[i]->Fit("P4G","LL");
	
      }
    }
  }

  // Plot results
  // ------------
  TCanvas * Res = new TCanvas ("Res","Res", 500, 500);
  Res->Divide(2,5);
  for (int i=1; i<=5; i++) {
    Res->cd(i*2-1);
    Pull[i-1]->Draw();
    Res->cd(i*2);
    Res->GetPad(i*2)->SetLogy();
    M12[i-1]->Draw("PE");
//     P4->SetParameters(p400[i],p410[i],p420[i],p430[i]);
//     P4->Draw("SAME");
  }
  if (fixp4) {
    Res->Print("EXO11095noBias_withExclusion_10pct.png");
  } else {
    Res->Print("EXO11095noBias_10pct.png");
  }
  // End of program
  // --------------
  gROOT->Time();

}
