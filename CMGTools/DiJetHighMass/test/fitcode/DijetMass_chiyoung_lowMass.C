//Dijet Mass spectrum is compared to pythia QCD prediction, smooth fit and dijet resonance signals.
//Author: Sertac Ozturk (Cukurova Univ. & FNAL) //sertac@fnal.gov , sertac.ozturk@cern.ch32
//Modified by chiyoung Jeong -- chiyoung.jeong@gmail.com

#include "QstarBinned_qg.h"
#include "ResonancesCrossSection.h"
#include "vector.h"

double lumi = 1.0;
double scaleFactor = 1.0;

TH1F *hJESplus_temp = new TH1F();
TH1F *hJESminus_temp = new TH1F();
TH1F *hQCD = new TH1F();
TH1F* hDijetMass = new TH1F();
TH1F *hQCD_Xsec = new TH1F();
TH1F *hPulls_add = new TH1F();
TFile *inputFileMC;
TFile *inputFile;

// QCD fit function -- alternate 4 parameter fit function -- also used for QCD fit.
Double_t fitQCD( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0]*pow(1.-x+p[3]*x*x,p[1])/pow(m[0],p[2]);
}

// Default fit
Double_t fitQCD1( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0]*pow(1.-x,p[1])/pow(x,p[2]+p[3]*log(x));
}

// QCD fit function -- alternate 3 parameter fit function -- also used for QCD fit.
Double_t fitQCD2( Double_t *m, Double_t *p)
{
    double x=m[0]/7000.;
    return p[0]*pow(1.-x,p[1])/pow(m[0],p[2]);
}

Double_t fitQCD3( Double_t *m, Double_t *p)
{  
    return p[0]/pow(m[0]+p[1],p[2]);
}  


void DijetMass_chiyoung_lowMass(){

  // definition of color 
  //Init("Q*");

  string sReco("fat30");
  //string sReco("pf");

  
  if ( sReco.find("pf") != string::npos ) Init("QQPF");
  else Init("QQFat");
  

  double mMinFit = 10000.;

  double mMin = 526.0;

  //  double mMin = 565.0;

  // double mMin = 606.0;

//  lumi = 1025.;
//  lumi = 2509.0;
  //  lumi = 2168.0;
  lumi = 129.0;


  Int_t ci_g, ci_b;   // for color index setting
  ci_g = TColor::GetColor("#006600");
  ci_b = TColor::GetColor("#000099");
  
  // Set TDR Style	
  gROOT->ProcessLine(".L tdrStyle.C");
  gROOT->ProcessLine("setTDRStyle()");
  
  // Input Files  
  TFile *inputFile = TFile::Open("../../data/finalResults_DATA_lowMass_118pbm1.root", "READ");



  // Histograms 
  if (sReco.find("pf") != string::npos)  hDijetMass = (TH1F*) inputFile->Get("M_PFJet1PFJet2_VarBin;1");
  else  hDijetMass = (TH1F*) inputFile->Get("M_FatPFJet1FatPFJet2_VarBin;1");
  //  TH1F *hQCDMC = (TH1F*)inputFile->Get("fit_DijetMass_mc_wide");
  //  inputFileMC = TFile::Open("histograms_summer11_mc_ak5.root", "UPDATE");
  if ( sReco.find("pf") != string::npos ) {
    mMinFit = 944.;
    inputFileMC = TFile::Open("../../data/histograms_summer11_mc_ak5.root", "UPDATE");
    hQCD = (TH1F*)inputFileMC->Get("h_DijetMass_data_pf;1");
    hJESplus_temp = (TH1F*)inputFileMC->Get("h_DijetMass_data_pf_up;1");
    hJESminus_temp = (TH1F*)inputFileMC->Get("h_DijetMass_data_pf_do;1");
  }
  else {
    mMinFit = 890.;
    inputFileMC = TFile::Open("../../data/histograms_Fat30_summer11_mc_ak5.root", "UPDATE");
    hQCD = (TH1F*)inputFileMC->Get("h_DijetMass_data_fat;1");
    hJESplus_temp = (TH1F*)inputFileMC->Get("h_DijetMass_data_fat_up;1");
    hJESminus_temp = (TH1F*)inputFileMC->Get("h_DijetMass_data_fat_do;1");
  }
  
  cout <<"Calc scale factor" << endl;
  scaleFactor = hDijetMass->Integral(37, hDijetMass->GetNbinsX()) / hQCD->Integral(37, hDijetMass->GetNbinsX()) / lumi;
  cout <<"scaleFactor = " << scaleFactor << endl;

                                      
  hQCD_Xsec = (TH1F*)hDijetMass->Clone("hQCD_Xsec");
  hQCD_Xsec->Reset();
  
  //  TH1F *hJESplus_temp = (TH1F*)inputFile->Get("fit_DijetMass_mc_up_wide");
  //  TH1F *hJESminus_temp = (TH1F*)inputFile->Get("fit_DijetMass_mc_down_wide");
  


  TH1F *hJESplus_temp2 =  (TH1F*)hJESplus_temp->Clone("hJESplus_temp2");
  hJESplus_temp2->Reset();
  TH1F *hJESminus_temp2 = (TH1F*)hJESplus_temp->Clone("hJESminus_temp2");
  hJESminus_temp2->Reset();

  /*
  TH1F *hJESplus =  (TH1F*)hJESplus_temp->Clone("hJESplus");
  hJESplus->Reset();
  TH1F *hJESminus = (TH1F*)hJESplus_temp->Clone("hJESminus");
  hJESminus->Reset();
  */

  TH1F* hJESplus = new TH1F("hJESplus", "", nMassBinsShortLow, massBoundariesShortLow);
  TH1F* hJESminus = new TH1F("hJESminus", "", nMassBinsShortLow, massBoundariesShortLow);


  TH1F *hJESplus2 = (TH1F*)hJESplus_temp->Clone("hJESplus2");
  hJESplus2->Reset();
  TH1F *hJESminus2 = (TH1F*)hJESplus_temp->Clone("hJESminus2");
  hJESminus2->Reset(); 
  
  TH1F *hPulls_3par = (TH1F*)hDijetMass->Clone("Pulls_3par");
  TH1F *hPulls_4par = (TH1F*)hDijetMass->Clone("Pulls_4par");
  TH1F *hPulls = (TH1F*)hDijetMass->Clone("Pulls");
  //  TH1F *hPulls_add = (TH1F*)inputFile->Get("h_DijetMass_mc_wide");
  hPulls_add = (TH1F*) hQCD->Clone("hPulls_add");

  hPulls_3par->Reset();
  hPulls_4par->Reset();
  hPulls->Reset();
  hPulls_add->Reset();
  
  // Pave text
  TPaveText *pave_fit = new TPaveText(0.18,0.15,0.40,0.27,"NDC");
  //  pave_fit->AddText("CMS Preliminary");

  
  pave_fit->AddText(" #sqrt{s} = 7 TeV");
  pave_fit->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  //  pave_fit->AddText("M_{jj} > 890 GeV");
  if (sReco.find("pf") != string::npos)   pave_fit->AddText("PF AK5 Jets");
  else   pave_fit->AddText("Wide Jets");

  pave_fit->SetFillColor(0);
  pave_fit->SetLineColor(0);
  pave_fit->SetFillStyle(0);
  pave_fit->SetBorderSize(0);
  pave_fit->SetTextFont(42);
  pave_fit->SetTextSize(0.03);
  pave_fit->SetTextAlign(12); 
  
  TPaveText *pave = new TPaveText(0.50,0.72,0.85,0.9,"NDC");
  

  // Pave text
  TPaveText *pave_fit_publi = new TPaveText(0.18,0.10,0.40,0.23,"NDC");
  //  pave_fit->AddText("CMS Preliminary");

  
  pave_fit_publi->AddText(" #sqrt{s} = 7 TeV");
  pave_fit_publi->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  //  pave_fit->AddText("M_{jj} > 890 GeV");
  if (sReco.find("pf") != string::npos)   pave_fit_publi->AddText("PF AK5 Jets");
  else   pave_fit_publi->AddText("Wide Jets");

  pave_fit_publi->SetFillColor(0);
  pave_fit_publi->SetLineColor(0);
  pave_fit_publi->SetFillStyle(0);
  pave_fit_publi->SetBorderSize(0);
  pave_fit_publi->SetTextFont(42);
  pave_fit_publi->SetTextSize(0.04);
  pave_fit_publi->SetTextAlign(12); 
  
  TPaveText *pave = new TPaveText(0.50,0.72,0.85,0.9,"NDC");


  //  pave->AddText("CMS Preliminary");
  

  pave->AddText(Form("CMS Preliminary (%.2f fb^{-1})", lumi/1000.)); 
//  pave->AddText("Coucou"); 
  pave->AddText(" #sqrt{s} = 7 TeV");
  pave->AddText(Form("M_{jj} > %.1f GeV", mMin));
  pave->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  if (sReco.find("pf") != string::npos)   pave->AddText("PF AK5 Jets");
  else pave->AddText("Wide Jets");
  pave->SetFillColor(0);
  pave->SetLineColor(0);
  pave->SetFillStyle(0);
  pave->SetBorderSize(0);
  pave->SetTextFont(42);
  pave->SetTextSize(0.03);
  pave->SetTextAlign(12); 
  
 
  cout << " make hQCD_Xsec histogram " << endl;
    
  /*
  hQCD->Sumw2();
  hQCD_Xsec->Sumw2(); 

  
  hJESplus_temp->Sumw2();
  hJESplus_temp2->Sumw2();
  hJESminus_temp->Sumw2();
  hJESminus_temp2->Sumw2();
  */
  
  
  hQCD->Scale(scaleFactor*lumi); 
  hJESplus_temp->Scale(scaleFactor*lumi);
  hJESminus_temp->Scale(scaleFactor*lumi);
  
   
  for(int i=0; i<hDijetMass->GetNbinsX(); i++){
    double n   = hQCD->GetBinContent(i+1);
    double err = hQCD->GetBinError(i+1);    
    double dm  = hQCD->GetBinWidth(i+1);
    double mjj = hQCD->GetBinCenter(i+1);
    
    
    if(n>=1.0){
      hQCD_Xsec->SetBinContent(i+1, n/(dm*lumi));
      hQCD_Xsec->SetBinError(i+1, err/(dm*lumi)*10);

      cout << "n = " << n/(dm*lumi) << "err = " << err/(dm*lumi) << endl;

    }
    else{
      hQCD_Xsec->SetBinContent(i+1, 0.);
      hQCD_Xsec->SetBinError(i+1, 0.);
    }
    
  }
    
  cout << "make hJESplus temp, this is hJESplus differential cross sction format" << endl;
  

  
  for(int i=0; i<hDijetMass->GetNbinsX(); i++){
    double n   = hJESplus_temp->GetBinContent(i+1);
    double err = hJESplus_temp->GetBinError(i+1);    
    double dm  = hJESplus_temp->GetBinWidth(i+1);
    double mjj = hJESplus_temp->GetBinCenter(i+1);
    
    
    if(n>=1.0){
      hJESplus_temp2->SetBinContent(i+1, n/(dm*lumi));
      hJESplus_temp2->SetBinError(i+1, err/(dm*lumi)*10);
    }
    else{
      hJESplus_temp2->SetBinContent(i+1, 0.);
      hJESplus_temp2->SetBinError(i+1, 0.);
    }
    
  }
  
  // make hJESminus temp, this is hJESminus differential cross sction format

  for(int i=0; i<hDijetMass->GetNbinsX(); i++){
    double n   = hJESminus_temp->GetBinContent(i+1);
    double err = hJESminus_temp->GetBinError(i+1);    
    double dm  = hJESminus_temp->GetBinWidth(i+1);
    double mjj = hJESminus_temp->GetBinCenter(i+1);
    
    
    if(n>=1.0){
      hJESminus_temp2->SetBinContent(i+1, n/(dm*lumi));
      hJESminus_temp2->SetBinError(i+1, err/(dm*lumi)*10);
    }
    else{
      hJESminus_temp2->SetBinContent(i+1, 0.);
      hJESminus_temp2->SetBinError(i+1, 0.);
    }
  }
   
 
  cout << "Fit QCD " << endl << endl << endl;


   // QCD Fit -- fit to qcd
   TF1 *f_qcd = new TF1("fit_qcd",fitQCD,mMinFit,3000.0,4); 
   gStyle->SetOptFit(1111); 
   f_qcd->SetParameter(0,6.15613e+18);
   f_qcd->SetParameter(1,-3.75321e+00);
   f_qcd->SetParameter(2,6.32703e+00);
   f_qcd->SetParameter(3,6.33149e+00);
   hQCD_Xsec->Fit("fit_qcd", "R");

   cout << "NDF = " << fit_qcd->GetNDF() << " FCN = " << fit_qcd->GetChisquare() << endl;

  cout << "Fit QCD Up" << endl << endl << endl;

   // QCD up Fit -- fit to JES plus
   TF1 *f_qcd_up = new TF1("fit_qcd_up",fitQCD,mMinFit,3000.0,4); 
   gStyle->SetOptFit(1111); 
   f_qcd_up->SetParameter(0,1.47700e+19);
   f_qcd_up->SetParameter(1,-4.03984e+00);
   f_qcd_up->SetParameter(2,6.44822e+00);
   f_qcd_up->SetParameter(3,5.36806e+00);
   hJESplus_temp2->Fit("fit_qcd_up","R");

   cout << "Fit QCD Do" << endl << endl << endl;

   // QCD down Fit -- fit to JES minus
   TF1 *f_qcd_down = new TF1("fit_qcd_down",fitQCD,mMinFit,3000.0,4); 
   gStyle->SetOptFit(1111); 
   f_qcd_down->SetParameter(0,2.12985e+19);
   f_qcd_down->SetParameter(1,-4.63606e+00);
   f_qcd_down->SetParameter(2,6.54632e+00);
   f_qcd_down->SetParameter(3,4.92684e+00);
   hJESminus_temp2->Fit("fit_qcd_down","R");

   // Differantial Cross Section -- make hQCD_xsec in graph format.
   // JES plus and minus is also smoothed with fit to make JES band.

   double a = 0.3173/2;
   double n,nl,nh;
   double n, dm, mass, xl, xh;
   double vx[1000],vy[1000],vexl[1000],vexh[1000],veyl[1000],veyh[1000];
   int i;
   double y, yplus, yminus, cplus, cminus,e;

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
        vy[i]   = n / (dm*lumi); 

     
	if (n<25 && mass>565 && mass<=2895)
       	{
		nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
		nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
		veyl[i] = nl/(lumi*dm);
		veyh[i] = nh/(lumi*dm);  
	}
	else if (n<25 && mass<=565 && mass>2895 && n>0)
	{
		nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
		nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
		veyl[i] = nl/(lumi*dm);
		veyh[i] = nh/(lumi*dm);  
	}
	else if (n>=25 && mass >= mMin)
	{
		veyl[i] = sqrt(n)/(lumi*dm);
		veyh[i] = sqrt(n)/(lumi*dm);
	} 
	else 
	  {     
	    vy[i] = -1.0;
	    veyl[i] = 0;
	    veyh[i] = 0;  
	}

      for (int j = 1; j < hJESplus->GetNbinsX()+1; j++){
	if (fabs(mass-hJESplus->GetBinCenter(j)) < 1e-5){
	hJESplus->SetBinContent(j,1.0*f_qcd_up->Eval(mass ) );
        hJESminus->SetBinContent(j,1.0*f_qcd_down->Eval(mass ) );
	}
      }
    }

   // these are the graph format of data.

    TGraphAsymmErrors *g = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
    TGraphAsymmErrors *g2 = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
    TGraphAsymmErrors *gFinal = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);

    // Fit to data    
    TF1 *fit = new TF1("fit",fitQCD1,mMinFit,3000.0,4); // 4 Par. Fit
    gStyle->SetOptFit(1111); 
    fit->SetParameter(0,1.73132e-05);
    fit->SetParameter(1,6.80678e+00);
    fit->SetParameter(2,6.33620e+00);
    fit->SetParameter(3,1.93728e-01);
    fit->SetLineWidth(2);
    fit->SetLineColor(4);



    g->Fit("fit","","",mMin,2895.0);	
    
    //Alternate Fits 4 parameter
    TF1 *f_4par = new TF1("fit_4par",fitQCD,mMin,3000.0,4); // 4 Par. Fit
    gStyle->SetOptFit(1111); 
    f_4par->SetParameter(0,3.08269e+16);
    f_4par->SetParameter(1,8.23385e+00);
    f_4par->SetParameter(2,5.37045e+00);
    f_4par->SetParameter(3,6.49182e-02);

    f_4par->SetLineWidth(2);
    f_4par->SetLineColor(TColor::GetColor("#009900"));


    //Alternate Fits 3 parameter
    TF1 *f_3par = new TF1("fit_3par",fitQCD2,mMin,2895.0,3); // 3 Par. Fit
    gStyle->SetOptFit(1111);
    f_3par->SetParameter(0,6.32475e+15);
    f_3par->SetParameter(1,9.89115e+00);
    f_3par->SetParameter(2,5.18216e+00); 
    f_3par->SetLineWidth(2);
    f_3par->SetLineColor(2);
    f_3par->SetLineStyle(2);

    //Dijet Mass Cross Section with Fit	
    TCanvas* c1 = new TCanvas("c1","DijetMass Cross Section with Fit");
    c1->SetLogy(1);
    g->SetTitle("");
    g->SetLineColor(1);
    g->SetFillColor(1);
    g->SetMarkerColor(1);
    g->SetMarkerStyle(20);
    g->GetXaxis()->SetTitle("Dijet Mass (GeV)");
    g->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");
    g->GetXaxis()->SetRangeUser(500,3000.0);
    g->GetYaxis()->SetRangeUser(0.000001,200);
    g->Draw("APZ");


    TLegend *leg = new TLegend(0.25,0.77,0.48,0.92);
    leg->SetTextSize(0.03146853);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->AddEntry(g,Form("CMS  (%.3f fb^{-1})", lumi/1000.),"PL"); 
    leg->AddEntry(fit,"Fit","L");
    leg->Draw("same");
    pave_fit->Draw("same");
    c1->SaveAs("Plots_lowMass/DijetMassCrossSectionWithFit.png");
    c1->SaveAs("Plots_lowMass/DijetMassCrossSectionWithFit.eps");


    //Dijet Mass cross section with all considered fits	
    TCanvas* c2 = new TCanvas("c2","DijetMass Cross Section with All Fit");
    c2->SetLogy(1);
    g2->SetLineColor(1);
    g2->SetFillColor(1);
    g2->SetMarkerColor(1);
    g2->SetMarkerStyle(20);
    g2->SetTitle("");
    g2->GetXaxis()->SetTitle("Dijet Mass (GeV)");
    g2->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");
    g2->GetXaxis()->SetRangeUser(700,3000);
    g2->GetYaxis()->SetRangeUser(0.000001,200);
    g2->Draw("APZ");
    g2->Fit("fit","","sames",mMin,2895.0);

    TString status_default= gMinuit->fCstatu.Data();
    g2->Fit("fit_4par","+","sames",mMin,2895.0);
    TString status_4par= gMinuit->fCstatu.Data();
    g2->Fit("fit_3par","+","sames",mMin,2895.0);
    TString status_3par= gMinuit->fCstatu.Data();
    TLegend *leg = new TLegend(0.18,0.78,0.38,0.92);
    leg->SetTextSize(0.03146853);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->AddEntry(g,Form("CMS  (%.3f fb^{-1})", lumi/1000.),"PL");
    leg->AddEntry(fit,"Default Fit (4 Par.)","L");
    leg->AddEntry(fit_4par,"Alternate Fit A (4 Par.)","L");
    leg->AddEntry(fit_3par,"Alternate Fit B (3 Par.)","L");
    leg->Draw("same");
    pave_fit->Draw("same");
	
    //Results of the fits
    cout << "*********************************************************"<<endl;
    double chi_fit = fit->GetChisquare();
    double ndf_fit = fit->GetNDF();
    cout << "Chisquare/ndf for Default Fit: " << chi_fit << "/" << ndf_fit << " : " << chi_fit/ndf_fit << endl;	
    cout << "Status: "<<status_default<<endl;
    cout << "*********************************************************"<<endl;
    double chi_4par = f_4par->GetChisquare();
    double ndf_4par = f_4par->GetNDF();
    cout << "Chisquare/ndf for 4 par. Fit: " << chi_4par << "/" << ndf_4par << " : " << chi_4par/ndf_4par << endl;
    cout << "Status: "<<status_4par<<endl;
    cout << "*********************************************************"<<endl;
    double chi_3par = f_3par->GetChisquare();
    double ndf_3par = f_3par->GetNDF();
    cout << "Chisquare/ndf for 3 par. Fit: " << chi_3par << "/" << ndf_3par << " : " << chi_3par/ndf_3par << endl;
    cout << "Status: "<<status_3par<<endl;
    cout << "*********************************************************"<<endl;
    //The end of Fit		

    // Data is compared with QCD MC -- perparation step

    THStack *h = new THStack("JES","JES");
    TH1F *htmp = (TH1F*)hJESplus->Clone("tmp");
    htmp->Add(hJESminus,-1);
    h->Add(hJESminus);
    h->Add(htmp);

    //hJESplus->SetFillColor(3);
    hJESminus->SetFillColor(10);
    hJESminus->SetLineColor(5);
    hJESminus->SetLineWidth(0);
    //    hJESminus->SetLineColor(5);
    htmp->GetXaxis()->SetRangeUser(500,3000);
    htmp->SetFillColor(5);
    htmp->SetLineColor(1);
    htmp->SetLineWidth(1); 
    htmp->SetLineColor(5);
    c2->SaveAs("Plots_lowMass/DijetMassCrossSectionWithAllFit.png");
    c2->SaveAs("Plots_lowMass/DijetMassCrossSectionWithAllFit.eps");


    //DijetMass cross section with Fit and QCD MC 
    TCanvas *c3 = new TCanvas("c3","DijetMass cross section with Fit and QCD MC");
    c3->SetLogy();
    h->Draw("C");
    h->GetXaxis()->SetRangeUser(500,3000);
    h->GetXaxis()->SetTitle("Dijet Mass (GeV)");
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");

    g->SetLineColor(1);
    g->SetFillColor(1);
    g->SetMarkerColor(1);
    g->SetMarkerStyle(20);
    f_qcd->SetLineWidth(2);
    f_qcd->SetLineStyle(2);
    f_qcd->SetLineColor(2);
    f_qcd->Draw("same");
    fit->SetLineWidth(2);	
    g->Draw("samePZ");
    
    TLegend *leg = new TLegend(0.22,0.78,0.45,0.93);
    leg->SetTextSize(0.03146853);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->AddEntry(g2, Form("CMS  (%.3f fb^{-1})", lumi/1000.),"PL");
    leg->AddEntry(fit,"Fit","L");
    leg->AddEntry(f_qcd,Form("%.2f#times Pythia", scaleFactor),"L");
    leg->AddEntry(htmp,"JES Uncertainty","F");
    leg->Draw("same");
    pave_fit->Draw("same");
    
    // Data over PTYHIA QCD MC
    double pulls[1000], eyh_pulls[1000],  eyl_pulls[1000];
    for(int i=0;i<hDijetMass->GetNbinsX();i++)
      {
	double data = vy[i];
	double error = veyh[i];
	double m = vx[i];
	double qcd = fit_qcd->Eval(m,0,0);
	if(data>=qcd) double error = veyl[i];
	if (data<qcd) double error = veyh[i];
	if(qcd != 0.){
	  pulls[i]=data/qcd;
	  eyh_pulls[i] = veyh[i]/qcd;
	  eyl_pulls[i] = veyl[i]/qcd;
	  
	  hJESplus2->SetBinContent(i+1, 1.0*f_qcd_up->Eval(m) / qcd);
	  hJESminus2->SetBinContent(i+1, 1.0*f_qcd_down->Eval(m) / qcd);

	}
      }
      
   TGraphAsymmErrors *gratio = new TGraphAsymmErrors(i,vx,pulls,vexl,vexh,eyl_pulls,eyh_pulls); 

   THStack *h2 = new THStack("JES","JES");
   TH1F *htmp2 = (TH1F*)hJESplus2->Clone("tmp2");
   htmp2->Add(hJESminus2,-1);
   h2->Add(hJESminus2);
   h2->Add(htmp2);  
   htmp2->SetFillColor(5);
   htmp2->SetLineColor(5);
   hJESplus2->SetFillColor(5);
   hJESplus2->SetLineColor(5);
   hJESminus2->SetFillColor(10);
   hJESminus2->SetLineColor(5);
   c3->SaveAs("Plots_lowMass/DijetMassCrossSectionWithFitAndQCDMC.png");
   c3->SaveAs("Plots_lowMass/DijetMassCrossSectionWithFitAndQCDMC.eps");

   // Data ovet PTYHIA QCD MC	
   TCanvas *c4 = new TCanvas("Ratio","Data/PYTHIA");

 
   h2->Draw("C");  
   h2->SetTitle("");
   h2->GetXaxis()->SetRangeUser(500.,3000.);
   h2->GetYaxis()->SetRangeUser(0.,3.);
   h2->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   h2->GetXaxis()->SetTitleSize(0.06);
   h2->GetYaxis()->SetTitle("Data / PYTHIA"); 

   
   //   htmp2->Draw("same");
   gratio->Draw("PSAME");
   TLine *l = new TLine(500, 1., 3000., 1.);
   l->Draw("same");

   TLegend *leg = new TLegend(0.18,0.32,0.40,0.47);
   leg->SetTextSize(0.03146853);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->AddEntry(gratio,Form("CMS  (%.3f fb^{-1})", lumi/1000.),"PL"); 
   leg->AddEntry(l,"QCD Pythia + CMS Simulation","L");
   leg->AddEntry(htmp2,"JES Uncertainty","F");
   leg->Draw("same");
   pave_fit->Draw("same");
   
   c4->Update();
   
   c4->SaveAs("Plots_lowMass/RatioDataPYTHIA.png");
   c4->SaveAs("Plots_lowMass/RatioDataPYTHIA.eps");


  // The end of comparison between data and PTYHIA QCD MC
   
   // Dijet Resonance Signals	
   TH1F *h_diquark1 = (TH1F*)hDijetMass->Clone("h_diquark1");
   TH1F *h_diquark2 = (TH1F*)hDijetMass->Clone("h_diquark2");
   TH1F *h_wprime1 = (TH1F*)hDijetMass->Clone("h_wprime1");
   TH1F *h_wprime2 = (TH1F*)hDijetMass->Clone("h_wprime2");
   h_diquark1->Reset();
   h_diquark2->Reset();
   h_wprime1->Reset();
   h_wprime2->Reset();

   // Mass of excited quarks and wprimes
   double diquark1 = 700;
   double diquark2 = 1500;
   double wprime1 = 700;
   double wprime2 = 1500;


   unsigned int qbin1 = (diquark1 / 100) -5;
   unsigned int qbin2 = (diquark2 / 100) -5;
   unsigned int sbin1 = (wprime1 / 100) -5;
   unsigned int sbin2 = (wprime2 / 100) - 5;
   
   std::vector<double> v_wprime1, v_wprime1_mjj, v_wprime2, v_wprime2_mjj, v_diquark1, v_diquark1_mjj, v_diquark2, v_diquark2_mjj, v_wprime1_2, v_wprime2_2, v_diquark1_2, v_diquark2_2, v_diquark1_3, v_diquark2_3;

   double sumProbaDiquark = 0;
   double sumProbaDiquark2 = 0;

   for(int i=0;i<hDijetMass->GetNbinsX();i++)
    {
     double dm = hDijetMass->GetBinWidth(i+1);
     double mass = hDijetMass->GetBinCenter(i+1);
     double fitt = fit->Eval(mass,0,0);
     if(mass>0.6*diquark1 && mass<1.40*diquark1){
       double prob = QstarBinnedProb(mass,diquark1);
       sumProbaDiquark += prob;
       cout << "mass = " << mass << "QstarBinnedProb(mass,diquark1) = " << prob << endl;
       if (prob > 1e-4){
	 v_diquark1.push_back(prob * diquark_newcut[qbin1] / dm);
	 v_diquark1_2.push_back(((prob * diquark_newcut[qbin1] / dm)+fitt)/fitt);
	 v_diquark1_3.push_back(((prob * diquark_newcut[qbin1] / dm))/fitt);
	 v_diquark1_mjj.push_back(mass);
       }

		}

     if(mass>0.6*diquark2 && mass<1.40*diquark2){
       double prob = QstarBinnedProb(mass,diquark2);
       sumProbaDiquark2 += prob;
       if (prob > 1e-4){
	 v_diquark2.push_back(prob * diquark_newcut[qbin2] / dm);
	 v_diquark2_2.push_back(((prob * diquark_newcut[qbin2] / dm)+fitt)/fitt); 
	 v_diquark2_3.push_back(((prob * diquark_newcut[qbin2] / dm))/fitt); 
	 v_diquark2_mjj.push_back(mass);
       }
     }
     if(mass>0.6*wprime1 && mass<1.40*wprime1){
       double prob = QstarBinnedProb(mass,wprime1);
       if (prob > 1e-4){
 		v_wprime1.push_back(prob * wprime_newcut[sbin1] / dm);
 		v_wprime1_2.push_back(((prob * wprime_newcut[sbin1] / dm)+fitt)/fitt);
		v_wprime1_mjj.push_back(mass);
       }
     }

     if(mass>0.6*wprime2 && mass<1.40*wprime2){
       double prob = QstarBinnedProb(mass,wprime2);
       if (prob > 1e-4){
		v_wprime2.push_back(QstarBinnedProb(mass,wprime2) * wprime_newcut[sbin2] / dm);
		v_wprime2_2.push_back(((QstarBinnedProb(mass,wprime2) * wprime_newcut[sbin2] / dm)+fitt)/fitt);
		v_wprime2_mjj.push_back(mass);
       }	
     }		
    }

   cout << "sumProbaDiquark = " << sumProbaDiquark << endl;
   cout << "sumProbaDiquark2 = " << sumProbaDiquark2 << endl;

   const unsigned size1 = v_diquark1.size();
   double q1[size1], q1_2[size1], q1_3[size1], mass1[size1];
   for(int i=0; i<size1; i++){
      q1[i] = v_diquark1[i];
      q1_2[i] = v_diquark1_2[i];
      q1_3[i] = v_diquark1_3[i];
      mass1[i] = v_diquark1_mjj[i];
    }
   const unsigned size2 = v_diquark2.size();
   std::cout << "size2 = " << size2 << std::endl;
   double q2[size2], q2_2[size2], q2_3[size2], mass2[size2];
   for(int i=0; i<size2; i++){
      q2[i] = v_diquark2[i];
      std::cout << "q2[" << i << "] = " << q2[i] << std::endl;
      q2_2[i] = v_diquark2_2[i];
      q2_3[i] = v_diquark2_3[i];
      mass2[i] = v_diquark2_mjj[i];
      std::cout << "mass2[" << i << "] = " << mass2[i] << std::endl;
   }
   const unsigned size3 = v_wprime1.size();
   double s1[size3], s1_2[size3], mass3[size3];
   for(int i=0; i<size3; i++){
      s1[i] = v_wprime1[i];
      s1_2[i] = v_wprime1_2[i];
      mass3[i] = v_wprime1_mjj[i];
   }
   const unsigned size4 = v_wprime2.size();
   double s2[size4], s2_2[size4], mass4[size4];
   for(int i=0; i<size4; i++){
      s2[i] = v_wprime2[i];
      s2_2[i] = v_wprime2_2[i];
      mass4[i] = v_wprime2_mjj[i];
   }
		
   TGraph* gr_diquark1 = new TGraph(size1, mass1, q1);  
   TGraph* gr_diquark2 = new TGraph(size2, mass2, q2);
   TGraph* gr_wprime1 = new TGraph(size3, mass3, s1);
   TGraph* gr_wprime2 = new TGraph(size4, mass4, s2);
	
   TGraph* gr_diquark1_2 = new TGraph(size1, mass1, q1_2);  
   TGraph* gr_diquark2_2 = new TGraph(size2, mass2, q2_2);
   TGraph* gr_wprime1_2 = new TGraph(size3-1, mass3, s1_2);
   TGraph* gr_wprime2_2 = new TGraph(size4-1, mass4, s2_2);
	
   TGraph* gr_diquark1_3 = new TGraph(size1, mass1, q1_3);  
   TGraph* gr_diquark2_3 = new TGraph(size2, mass2, q2_3);

   gr_wprime1->SetLineColor(ci_g);
   gr_wprime1->SetLineStyle(7);
   gr_wprime1->SetLineWidth(2);
   gr_diquark1->SetLineColor(2);
   gr_diquark1->SetLineStyle(5);
   gr_diquark1->SetLineWidth(2);
   gr_diquark2->SetLineColor(2);
   gr_diquark2->SetLineStyle(5);
   gr_diquark2->SetLineWidth(2);
   gr_wprime2->SetLineColor(ci_g);
   gr_wprime2->SetLineStyle(7);
   gr_wprime2->SetLineWidth(2);

   gr_wprime1_2->SetLineColor(ci_g);
   gr_wprime1_2->SetLineStyle(7);
   gr_wprime1_2->SetLineWidth(2);
   gr_diquark1_2->SetLineColor(2);
   gr_diquark1_2->SetLineStyle(5);
   gr_diquark1_2->SetLineWidth(2);
   gr_diquark2_2->SetLineColor(2);
   gr_diquark2_2->SetLineStyle(5);
   gr_diquark2_2->SetLineWidth(2);
   gr_wprime2_2->SetLineColor(ci_g);
   gr_wprime2_2->SetLineStyle(7);
   gr_wprime2_2->SetLineWidth(2);

   gr_diquark1_3->SetLineColor(2);
   gr_diquark1_3->SetLineStyle(5);
   gr_diquark1_3->SetLineWidth(2);
   gr_diquark2_3->SetLineColor(2);
   gr_diquark2_3->SetLineStyle(5);
   gr_diquark2_3->SetLineWidth(2);





   TCanvas *c5 = new TCanvas("c5","Dijet mass cross section with the signals");
   c5->SetLogy();

   TH1F *vFrame = gPad->DrawFrame(500.0,0.000001,3000.0,200.0);
   vFrame->SetTitle("");
   vFrame->SetXTitle("Dijet Mass (GeV)");
   vFrame->GetXaxis()->SetTitleSize(0.06);
   vFrame->SetYTitle("d#sigma/dm (pb/GeV)");

   g->Draw("PZ");

   gr_diquark1->Draw("same");
   //   gr_diquark2->Draw("same");
   gr_wprime1->Draw("same");
   //   gr_wprime2->Draw("same");

   TPaveText *pt_c5_wprime1 = new TPaveText(0.35,0.7,0.50,0.8,"NDC");
   pt_c5_wprime1->SetFillColor(0);
   pt_c5_wprime1->SetFillStyle(0);
   pt_c5_wprime1->SetBorderSize(0);
   pt_c5_wprime1->SetTextColor(ci_g);
   pt_c5_wprime1->AddText("Diquark (0.7 TeV)");

   TPaveText *pt_c5_wprime2 = new TPaveText(0.65,0.45,0.8,0.55,"NDC");
   pt_c5_wprime2->SetFillColor(0);
   pt_c5_wprime2->SetFillStyle(0);
   pt_c5_wprime2->SetBorderSize(0);
   pt_c5_wprime2->SetTextColor(ci_g);
   pt_c5_wprime2->AddText("Diquark (1.5 TeV)");

   TPaveText *pt_c5_diquark1 = new TPaveText(0.18,0.35,0.33,0.45,"NDC");
   pt_c5_diquark1->SetFillColor(0);
   pt_c5_diquark1->SetFillStyle(0);
   pt_c5_diquark1->SetBorderSize(0);
   pt_c5_diquark1->SetTextColor(2);
   pt_c5_diquark1->AddText("Diquark (0.7 TeV)");

   TPaveText *pt_c5_diquark2 = new TPaveText(0.32,0.25,0.47,0.35,"NDC");
   pt_c5_diquark2->SetFillColor(0);
   pt_c5_diquark2->SetFillStyle(0);
   pt_c5_diquark2->SetBorderSize(0);
   pt_c5_diquark2->SetTextColor(2);
   pt_c5_diquark2->AddText("Diquark (1.5 TeV)");

   pt_c5_wprime1->Draw("sames");
   //pt_c5_wprime2->Draw("sames");
   pt_c5_diquark1->Draw("sames");
   //   pt_c5_diquark2->Draw("sames");
	
   TLegend *leg2 = new TLegend(0.22,0.78,0.45,0.93);
   leg2->SetTextSize(0.03146853);
   leg2->SetBorderSize(0);
   leg2->SetFillColor(0);
   leg2->SetFillStyle(0);
   leg2->AddEntry(g,Form("CMS  (%.3f fb^{-1})", lumi),"PL");
   leg2->AddEntry(fit,"Fit","L");
   leg2->AddEntry(gr_diquark1,"Diquark","L");
   leg2->AddEntry(gr_wprime1,"W'","L");
   leg2->Draw("same");
   pave_fit->Draw("same");

   double ratio[1000], pulls_3par[1000], pulls_4par[1000], pulls[1000], eyh_pulls_3par[1000], eyl_pulls_3par[1000], eyh_pulls_4par[1000], eyl_pulls_4par[1000], eyh_pulls[1000], eyl_pulls[1000]; 
   double calculating_chi2=0.0;
   for(int i=0;i<hDijetMass->GetNbinsX();i++)
    {
	double data = vy[i];
	double error = veyh[i];
	double m = vx[i];
	double fit_3p = f_3par->Eval(m,0,0);
	double fit_4p = f_4par->Eval(m,0,0);
	double fit_default = fit->Eval(m,0,0);  

	if(data>=fit_3p) double error3p = veyl[i];
	if (data<fit_3p) double error3p = veyh[i];
	if(data>=fit_4p) double error4p = veyl[i];
	if (data<fit_4p) double error4p = veyh[i];
	if(data>=fit_default) double error = veyl[i];
	if (data<fit_default) double error = veyh[i];
					
	pulls_3par[i]=(data-fit_3p)/fit_3p;
	eyh_pulls_3par[i] = veyh[i]/fit_3p;
	eyl_pulls_3par[i] = veyl[i]/fit_3p;
	
	pulls_4par[i]=(data-fit_4p)/fit_4p;
	eyh_pulls_4par[i] = veyh[i]/fit_4p;
	eyl_pulls_4par[i] = veyl[i]/fit_4p;
			
	pulls[i]=(data-fit_default)/fit_default;
	ratio[i] = data / fit_default;
	eyh_pulls[i] = veyh[i]/fit_default;
	eyl_pulls[i] = veyl[i]/fit_default;
				
	if(m<=565 || m>2895){
		pulls_3par[i] = -999;
		pulls_4par[i] = -999;
		pulls[i] = -999;
	}
		
	if(error != 0.){
	  //	  std::cout << "m = " << m << std::endl;
	  double q_star=0.0;
		hPulls_3par->SetBinContent(i+1,(data-fit_3p)/error3p);
		hPulls_3par->SetBinError(i+1,1.);
		hPulls_4par->SetBinContent(i+1,(data-fit_4p)/error4p);
		hPulls_4par->SetBinError(i+1,1.);
		hPulls->SetBinContent(i+1,(data-fit_default)/error);
		hPulls->SetBinError(i+1,1.);
		hPulls_add->SetBinContent(i+1,(data-fit_default)/error);
		hPulls_add->SetBinError(i+1,1.);

		//	std::cout << "size2 = " << size2 << std::endl;
		cout << "bin " << i+1 << "\t [" << hPulls_4par->GetBinLowEdge(i+1) << ", " 
		     << hPulls_4par->GetBinLowEdge(i+1)+hPulls_4par->GetBinWidth(i+1) << "]\t chi = " 
		     << Form("%.1f", (data-fit_default)/error) << "\t chi2 = " << Form("%.1f", (data-fit_default)/error*(data-fit_default)/error) << endl;
		for (int j=0;j<size2;j++)
		  {
		    if (mass2[j] == m)
		      {
			//			std::cout << "m = " << m << "q2[" << j << "] = " << q2[j] << std::endl;
			q_star = q2[j];
		      }
		      
		  }
		//		std::cout << "data = " << data << " ,   fit_default = " << fit_default << std::endl;
		
		//		calculating_chi2 += ((data-fit_default-q_star)/error)*((data-fit_default-q_star)/error);
		calculating_chi2 += ((data-fit_default)/error)*((data-fit_default)/error);
           }
     }
	 
   std::cout << "chi2 = " << calculating_chi2 << std::endl;

   TGraphAsymmErrors *hDiff_3par = new TGraphAsymmErrors(i,vx,pulls_3par,vexl,vexh,eyl_pulls_3par,eyh_pulls_3par);  
   TGraphAsymmErrors *hDiff_4par = new TGraphAsymmErrors(i,vx,pulls_4par,vexl,vexh,eyl_pulls_4par,eyh_pulls_4par);
   TGraphAsymmErrors *hDiff = new TGraphAsymmErrors(i,vx,pulls,vexl,vexh,eyl_pulls,eyh_pulls);
   TGraphAsymmErrors *hratio = new TGraphAsymmErrors(i,vx,ratio,vexl,vexh,eyl_pulls,eyh_pulls);
	
   hPulls_3par->SetMarkerStyle(26);
   hPulls_3par->SetMarkerColor(4);
   hPulls_3par->SetLineColor(4);
   hDiff_3par->SetMarkerStyle(26);
   hDiff_3par->SetMarkerColor(4);
   hDiff_3par->SetLineColor(4);

   hPulls_4par->SetMarkerStyle(25);
   hPulls_4par->SetMarkerColor(TColor::GetColor("#006600"));
   hPulls_4par->SetLineColor(TColor::GetColor("#006600"));
   hDiff_4par->SetMarkerStyle(25);
   hDiff_4par->SetMarkerColor(TColor::GetColor("#006600"));
   hDiff_4par->SetLineColor(TColor::GetColor("#006600"));

   hPulls->SetMarkerStyle(20);
   hPulls->SetMarkerColor(1);
   hPulls->SetLineColor(1);
   hDiff->SetMarkerStyle(20);
   hDiff->SetMarkerColor(1);
   hDiff->SetLineColor(1);
	
   hratio->SetMarkerStyle(20);
   hratio->SetMarkerColor(1);
   hratio->SetLineColor(1);	
   c5->SaveAs("Plots_lowMass/DijetMassCrossSectionWithSignal.png");
   c5->SaveAs("Plots_lowMass/DijetMassCrossSectionWithSignal.eps");


   TCanvas* c6 = new TCanvas("c6","(Data-Fit)/Fit");
   hDiff->SetTitle("");
   hDiff->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   hDiff->GetYaxis()->SetTitle("(Data-Fit)/Fit");
   hDiff->GetXaxis()->SetRangeUser(500.,3000.);
   hDiff->GetYaxis()->SetRangeUser(-1.5,3.);
   hDiff->Draw("APZ");
   gr_diquark1_3->Draw("sameC");
   gr_diquark2_3->Draw("sameC");
   l = new TLine(500, 0.0, 3000, 0.0);
   l->SetLineStyle(2);
   l->Draw("same");
   pave->Draw("same");

   TPaveText *pt_c6_diquark1 = new TPaveText(0.15,0.50,0.35,0.60,"NDC");
   pt_c6_diquark1->SetFillColor(0);
   pt_c6_diquark1->SetTextColor(2);
   pt_c6_diquark1->SetFillStyle(0);
   pt_c6_diquark1->SetBorderSize(0);
   pt_c6_diquark1->SetTextSize(0.03);
   pt_c6_diquark1->AddText("D (0.7 TeV)");

   TPaveText *pt_c6_diquark2 = new TPaveText(0.40,0.50,0.60,0.60,"NDC");
   pt_c6_diquark2->SetFillColor(0);
   pt_c6_diquark2->SetTextColor(2);
   pt_c6_diquark2->SetFillStyle(0);
   pt_c6_diquark2->SetBorderSize(0);
   pt_c6_diquark2->SetTextSize(0.03);
   pt_c6_diquark2->AddText("D (1.5 TeV)");

   pt_c6_diquark1->Draw("same");
   pt_c6_diquark2->Draw("same");

   c6->SaveAs("Plots_lowMass/DataMinusFitDividedByFit.png");
   c6->SaveAs("Plots_lowMass/DataMinusFitDividedByFit.eps");


   TCanvas* c7 = new TCanvas("c7","(Data-Fit)/Error");
   hPulls->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   hPulls->GetYaxis()->SetTitle("(Data-Fit)/Error");
   hPulls->GetXaxis()->SetRangeUser(500.,3000.);
   hPulls->GetYaxis()->SetRangeUser(-3.9,3.9);
   hPulls->SetLineWidth(1);
   hPulls->Draw("ep");
   l->Draw("same");  
	
   c7->SaveAs("Plots_lowMass/DataMinusFitDividedByError.png");
   c7->SaveAs("Plots_lowMass/DataMinusFitDividedByError.eps");

   TCanvas* c8 = new TCanvas("c8","(Data-Fit)/Fit for the All Fits");
   hDiff->Draw("APZ");
   hDiff_4par->Draw("pzsame");
   hDiff_3par->Draw("pzsame");
   l->Draw("same"); 
   TLegend *leg = new TLegend(0.4,0.6,0.85,0.92);
   leg->SetFillColor(0);
   leg->AddEntry(hPulls,"Default Fit (4 Par.)","PL");
   leg->AddEntry(hPulls_4par,"Alternate Fit A (4 Par.)","PL");
   leg->AddEntry(hPulls_3par,"Alternate Fit B (3 Par.)","PL");
   leg->Draw("same");
   c8->SaveAs("Plots_lowMass/DataMinusFitDividedByFitForAllFits.png");
   c8->SaveAs("Plots_lowMass/DataMinusFitDividedByFitForAllFits.eps");


   TCanvas* c9 = new TCanvas("c9","(Data-Fit)/Error the All Fits");
   hPulls->Draw("ep");
   hPulls_4par->Draw("epsame");
   hPulls_3par->Draw("epsame");
   l->Draw("same");	
   TLegend *leg = new TLegend(0.65,0.7,0.92,0.92);
   leg->SetFillColor(0);
   leg->AddEntry(hPulls,"Default Fit (4 Par.)","PL");
   leg->AddEntry(hPulls_4par,"Alternate Fit A (4 Par.)","PL");
   leg->AddEntry(hPulls_3par,"Alternate Fit B (3 Par.)","PL");
   leg->Draw("same");

   c9->SaveAs("Plots_lowMass/DataMinusFitDividedByErrorForAllFits.png");
   c9->SaveAs("Plots_lowMass/DataMinusFitDividedByErrorForAllFits.eps");	


   TCanvas* c10 = new TCanvas("c10","Data/Fit with the All signal");
   c10->SetLogy(1);
   hratio->SetTitle("");
   hratio->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   hratio->GetXaxis()->SetTitleSize(0.06);
   hratio->GetYaxis()->SetTitle("Data/Fit");
   hratio->SetMarkerStyle(20);
   hratio->GetXaxis()->SetRangeUser(500.,3000.);
   hratio->GetYaxis()->SetRangeUser(0.5,6.);
   hratio->Draw("APZ");
   l->Draw("same");
	
   gr_diquark1_2->Draw("Lsame");
   //  gr_diquark2_2->Draw("Lsame");
   

   TPaveText *pt_c10_diquark1 = new TPaveText(0.2,0.5,0.35,0.6, "NDC");
   pt_c10_diquark1->SetFillColor(0);
   pt_c10_diquark1->SetTextColor(2);
   pt_c10_diquark1->SetFillStyle(0);
   pt_c10_diquark1->SetBorderSize(0);
   pt_c10_diquark1->AddText("Diquark (0.7 TeV)");

   TPaveText *pt_c10_diquark2 = new TPaveText(0.5,0.5,0.65,0.6,"NDC");
   pt_c10_diquark2->SetFillColor(0);
   pt_c10_diquark2->SetTextColor(2);
   pt_c10_diquark2->SetFillStyle(0);
   pt_c10_diquark2->SetBorderSize(0);
   pt_c10_diquark2->AddText("Diquark (1.5 TeV)");

   pt_c10_diquark1->Draw("sames");
   //  pt_c10_diquark2->Draw("sames");

   pave->Draw("smaes");

   c10->SaveAs("Plots_lowMass/DataMinusFitWithAllSignal.png");
   c10->SaveAs("Plots_lowMass/DataMinusFitWithAllSignal.eps");







   TCanvas* c11 = new TCanvas("c11","default fit and pull");

   c11->Divide(1,2,0,0,0);
   c11->cd(1);
   
   p11_1 = (TPad*)c11->GetPad(1);
   p11_1->SetPad(0.01,0.22,0.99,0.98);
   p11_1->SetLogy();
   p11_1->SetRightMargin(0.05);
   p11_1->SetTopMargin(0.05);
   
   TH1F *vFrame = p11_1->DrawFrame(500.0,0.000001,3000.0,200.0);
   vFrame->SetTitle("");
   vFrame->SetXTitle("Dijet Mass (GeV)");
   vFrame->GetXaxis()->SetTitleSize(0.06);
   vFrame->SetYTitle("d#sigma/dm (pb/GeV)");

   h->SetTitle("");

   h->Draw("SAMEC");

   //   h->GetXaxis()->SetRangeUser(500.,3000.);
   //   h->GetYaxis()->SetRangeUser(0.00001,50.);
   // h->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   //  h->GetXaxis()->SetTitleSize(0.06);
   //  h->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");


   f_qcd->Draw("same");
   g->Draw("sameP");
   TLegend *leg = new TLegend(0.28,0.70,0.49,0.93);
   leg->SetTextSize(0.03146853);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->AddEntry(g,Form("CMS  (%.3f fb^{-1})", lumi/1000.),"PL"); 
   leg->AddEntry(fit,"Fit","L");
   leg->AddEntry(f_qcd,"QCD Pythia","L");
   leg->AddEntry(htmp,"JES Uncertainty","F");
   leg->AddEntry(gr_diquark1,"Diquark","L");
   leg->AddEntry(gr_wprime1,"W'","L");
   leg->Draw("same");

   pave_fit->Draw("same");

   gr_diquark1->Draw("same");
   //   gr_diquark2->Draw("same");
   gr_wprime1->Draw("same");
   //   gr_wprime2->Draw("same");

   TPaveText *pt_c11_wprime1 = new TPaveText(0.15,0.25,0.30,0.4,"NDC");
   pt_c11_wprime1->SetFillColor(0);
   pt_c11_wprime1->SetFillStyle(0);
   pt_c11_wprime1->SetBorderSize(0);
   pt_c11_wprime1->SetTextColor(TColor::GetColor("#006600"));
   pt_c11_wprime1->AddText("W' (0.7 TeV)");

   TPaveText *pt_c11_wprime2 = new TPaveText(0.60,0.45,0.75,0.55,"NDC");
   pt_c11_wprime2->SetFillColor(0);
   pt_c11_wprime2->SetFillStyle(0);
   pt_c11_wprime2->SetBorderSize(0);
   pt_c11_wprime2->SetTextColor(TColor::GetColor("#006600"));
   pt_c11_wprime2->AddText("W' (1.5 TeV)");

   TPaveText *pt_c11_diquark1 = new TPaveText(0.26,0.40,0.45,0.53,"NDC");
   pt_c11_diquark1->SetFillColor(0);
   pt_c11_diquark1->SetFillStyle(0);
   pt_c11_diquark1->SetBorderSize(0);
   pt_c11_diquark1->SetTextColor(2);
   pt_c11_diquark1->AddText("Diquark (0.7 TeV)");

   TPaveText *pt_c11_diquark2 = new TPaveText(0.40,0.15,0.55,0.20,"NDC");
   pt_c11_diquark2->SetFillColor(0);
   pt_c11_diquark2->SetFillStyle(0);
   pt_c11_diquark2->SetBorderSize(0);
   pt_c11_diquark2->SetTextColor(2);
   pt_c11_diquark2->AddText("Diquark (1.5 TeV)");

   pt_c11_wprime1->Draw("sames");
   //   pt_c11_wprime2->Draw("sames");
   pt_c11_diquark1->Draw("sames");
   //   pt_c11_diquark2->Draw("sames");
   
   c11->cd(2);
   p11_2 = (TPad*)c11->GetPad(2);
   p11_2->SetPad(0.01,0.02,0.99,0.24);
   p11_2->SetBottomMargin(0.35);
   p11_2->SetRightMargin(0.05);
   p11_2->SetGridx();
   c11_2->SetTickx(50);


   TH1F *vFrame2 = p11_2->DrawFrame(500.0, -3.31, 3000.0, 3.31);
   vFrame2->SetTitle("");
   vFrame2->SetXTitle("Dijet Mass (GeV)");
   vFrame2->GetXaxis()->SetTitleSize(0.06);
   vFrame2->SetYTitle("Significance");
   vFrame2->GetYaxis()->SetTitleSize(0.12);
   vFrame2->GetYaxis()->SetLabelSize(0.10);
   vFrame2->GetYaxis()->SetTitleOffset(0.50);
   vFrame2->GetXaxis()->SetTitleOffset(0.90);
   vFrame2->GetXaxis()->SetTitleSize(0.18);
   vFrame2->GetXaxis()->SetLabelSize(0.18);


    hPulls_add->SetLineWidth(0);
   hPulls_add->SetFillColor(2);
   hPulls_add->SetLineColor(2);

   hPulls_add->Draw("SAMEHIST");

   TLine *line = new TLine(500.,0,3000,0);
   line->Draw("");
   
   c11->SaveAs("Plots_lowMass/DefaultFitAndPull.png");
   c11->SaveAs("Plots_lowMass/DefaultFitAndPull.eps");

   














   TCanvas* c12 = new TCanvas("c12","default fit and pull");

   c12->Divide(1,2,0,0,0);
   c12->cd(1);
   
   p12_1 = (TPad*)c12->GetPad(1);
   p12_1->SetPad(0.01,0.22,0.99,0.98);
   p12_1->SetLogy();
   p12_1->SetRightMargin(0.05);
   p12_1->SetTopMargin(0.05);
   
   TH1F *vFrame = p12_1->DrawFrame(489.0,0.000001,3019.0,200.0);
   vFrame->SetTitle("");
   vFrame->SetXTitle("");
   vFrame->GetXaxis()->SetLabelSize(0.00);
   vFrame->SetYTitle("d#sigma/dm (pb/GeV)");
   vFrame->Draw();
   p12_1->Update();

   

   h->Draw("SAMEC");

   //   h->GetXaxis()->SetRangeUser(500.,3000.);
   //   h->GetYaxis()->SetRangeUser(0.00001,50.);
   // h->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   //  h->GetXaxis()->SetTitleSize(0.06);
   //  h->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");


   f_qcd->Draw("same");

    TF1 *fitFinal = new TF1("fitFinal",fitQCD1,mMinFit,3000.0,4); // 4 Par. Fit
    gStyle->SetOptFit(0000); 
    fitFinal->SetParameter(0,1.73132e-05);
    fitFinal->SetParameter(1,6.80678e+00);
    fitFinal->SetParameter(2,6.33620e+00);
    fitFinal->SetParameter(3,1.93728e-01);
    fitFinal->SetLineWidth(2);
    fitFinal->SetLineColor(4);

    gFinal->Fit("fitFinal","","",mMin,2895.0);
    gFinal->Draw("samePZ");


   TLegend *leg = new TLegend(0.44,0.60,0.86,0.93);
   leg->SetTextSize(0.03146853);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->AddEntry(g,Form("CMS Preliminary (%.2f fb^{-1})", lumi/1000.),"PL"); 
   leg->AddEntry(fit,"Fit","L");
   leg->AddEntry(f_qcd,"QCD Pythia + CMS Simulation","L");
   leg->AddEntry(htmp,"JES Uncertainty + CMS Simulation","F");
   leg->AddEntry(gr_diquark1,"E_{6} Diquark + CMS Simulation","L");
   leg->AddEntry(gr_wprime1,"W'","L");
   leg->Draw("same");

   pave_fit_publi->Draw("same");

   gr_diquark1->Draw("sameC");
   gr_diquark2->Draw("sameC");
   gr_wprime1->Draw("sameC");
   //   gr_wprime2->Draw("same");

   TPaveText *pt_c12_wprime1 = new TPaveText(0.15,0.25,0.32,0.35,"NDC");
   pt_c12_wprime1->SetFillColor(0);
   pt_c12_wprime1->SetFillStyle(0);
   pt_c12_wprime1->SetBorderSize(0);
   pt_c12_wprime1->SetTextSize(0.04);
   pt_c12_wprime1->SetTextColor(TColor::GetColor("#006600"));
   pt_c12_wprime1->AddText("W' (0.7 TeV)");

   TPaveText *pt_c12_wprime2 = new TPaveText(0.60,0.45,0.75,0.55,"NDC");
   pt_c12_wprime2->SetFillColor(0);
   pt_c12_wprime2->SetFillStyle(0);
   pt_c12_wprime2->SetBorderSize(0);
   pt_c12_wprime2->SetTextSize(0.04);
   pt_c12_wprime2->SetTextColor(TColor::GetColor("#006600"));
   pt_c12_wprime2->AddText("W' (1.5 TeV)");

   TPaveText *pt_c12_diquark1 = new TPaveText(0.24,0.42,0.46,0.55,"NDC");
   pt_c12_diquark1->SetFillColor(0);
   pt_c12_diquark1->SetFillStyle(0);
   pt_c12_diquark1->SetBorderSize(0);
   pt_c12_diquark1->SetTextColor(2);
   pt_c12_diquark1->SetTextSize(0.04);
   pt_c12_diquark1->AddText("D (0.7 TeV)");

   TPaveText *pt_c12_diquark2 = new TPaveText(0.35,0.26,0.52,0.40,"NDC");
   pt_c12_diquark2->SetFillColor(0);
   pt_c12_diquark2->SetFillStyle(0);
   pt_c12_diquark2->SetBorderSize(0);
   pt_c12_diquark2->SetTextColor(2);
   pt_c12_diquark2->SetTextSize(0.04);
   pt_c12_diquark2->AddText("D (1.5 TeV)");

   pt_c12_wprime1->Draw("sames");
   pt_c12_wprime2->Draw("sames");
   pt_c12_diquark1->Draw("sames");
   pt_c12_diquark2->Draw("sames");
   
   c12->cd(2);
   p12_2 = (TPad*)c12->GetPad(2);
   p12_2->SetPad(0.01,0.02,0.99,0.24);
   p12_2->SetBottomMargin(0.35);
   p12_2->SetRightMargin(0.05);
   p12_2->SetGridx();
   c12_2->SetTickx(50);


   TH1F *vFrame2 = p12_2->DrawFrame(489.0, -2.99, 3019.0, 2.99);
   vFrame2->SetTitle("");
   vFrame2->SetXTitle("Dijet Mass (GeV)");
   vFrame2->GetXaxis()->SetTitleSize(0.06);
   vFrame2->SetYTitle("Significance");
   vFrame2->GetYaxis()->SetTitleSize(0.12);
   vFrame2->GetYaxis()->SetLabelSize(0.10);
   vFrame2->GetYaxis()->SetTitleOffset(0.50);
   vFrame2->GetXaxis()->SetTitleOffset(0.90);
   vFrame2->GetXaxis()->SetTitleSize(0.18);
   vFrame2->GetXaxis()->SetLabelSize(0.18);


    hPulls_add->SetLineWidth(0);
   hPulls_add->SetFillColor(2);
   hPulls_add->SetLineColor(2);

   hPulls_add->Draw("SAMEHIST");

   TLine *line = new TLine(489.,0,3019,0);
   line->Draw("");
   
   c12->SaveAs("Plots_lowMass/DefaultFitAndPull_noStat.png");
   c12->SaveAs("Plots_lowMass/DefaultFitAndPull_noStat.eps");







   

}

