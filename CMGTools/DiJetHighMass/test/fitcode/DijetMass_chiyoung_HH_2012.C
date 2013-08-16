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
TH1F *hSignal = new TH1F();
TFile *inputFileMC;
TFile *inputFile;

//double mMin = 1058.0;
double mMin = 890.0;
double mMax = 2659.0;

// QCD fit function -- alternate 4 parameter fit function -- also used for QCD fit.
Double_t fitQCD( Double_t *m, Double_t *p)
{
  double x=m[0]/8000.;
  return p[0]*pow(1.-x+p[3]*x*x,p[1])/pow(m[0],p[2]);
}

// Default fit
Double_t fitQCD1( Double_t *m, Double_t *p)
{
  double x=m[0]/8000.;
  return p[0]*pow(1.-x,p[1])/pow(x,p[2]+p[3]*log(x));
}

// QCD fit function -- alternate 3 parameter fit function -- also used for QCD fit.
Double_t fitQCD2( Double_t *m, Double_t *p)
{
  double x=m[0]/8000.;
  return p[0]*pow(1.-x,p[1])/pow(m[0],p[2]);
}

Double_t fitQCD3( Double_t *m, Double_t *p)
{  
  return p[0]/pow(m[0]+p[1],p[2]);
}  


void DijetMass_chiyoung_HH_2012(){

  // definition of color 
  Init("Q*");


  lumi = 20000.;

  Int_t ci_g, ci_b;   // for color index setting
  ci_g = TColor::GetColor("#006600");
  ci_b = TColor::GetColor("#000099");
  
  // Set TDR Style	
  gROOT->ProcessLine(".L tdrStyle.C");
  gROOT->ProcessLine("setTDRStyle()");
  

  TFile *inputFile = TFile::Open("../../data/HH_histo_dijetmass_rebin_signal1500.root", "READ");

  hSignal = (TH1F*) inputFile->Get("dijetWtag_Moriond_HHPy61500.root;1");

  // Input Files  
  TFile *inputFile = TFile::Open("../../data/HH_histo_dijetmass_rebin_data.root", "READ");


  // Histograms 
  hDijetMass = (TH1F*) inputFile->Get("dijetWtag_Moriond_Run2012.root;1");
  inputFileMC = TFile::Open("../../data/HH_histo_dijetmass_rebin_qcd.root", "UPDATE");
  hQCD = (TH1F*)inputFileMC->Get("dijetWtag_Moriond_QCD500.root;1");
  
  cout <<"Calc scale factor" << endl;
  scaleFactor = hDijetMass->Integral(38, hDijetMass->GetNbinsX()) / hQCD->Integral(38, hDijetMass->GetNbinsX()) / lumi;
  cout <<"scaleFactor = " << scaleFactor << endl;

                                      
  hQCD_Xsec = (TH1F*)hDijetMass->Clone("hQCD_Xsec");
  hQCD_Xsec->Reset();
  
  hJESplus_temp = (TH1F*)hQCD->Clone("hJESplus_temp;1");//(TH1F*)inputFileMC->Get("dijetWtag_Moriond_QCD500.root;1");
  hJESminus_temp = (TH1F*)hQCD->Clone("hJESminus_temp");//(TH1F*)inputFileMC->Get("dijetWtag_Moriond_QCD500.root;1");


  
  TH1F* hJESplus = new TH1F("hJESplus", "", nMassBinsShort2012, massBoundariesShort2012);
  TH1F* hJESminus = new TH1F("hJESminus", "", nMassBinsShort2012, massBoundariesShort2012);
 


  TH1F *hJESplus_temp2 =  (TH1F*)hJESplus_temp->Clone("hJESplus_temp2");
  hJESplus_temp2->Reset();
  TH1F *hJESminus_temp2 = (TH1F*)hJESplus_temp->Clone("hJESminus_temp2");
  hJESminus_temp2->Reset();


  TH1F *hJESplus2 = (TH1F*)hJESplus_temp->Clone("hJESplus2");
  hJESplus2->Reset();
  TH1F *hJESminus2 = (TH1F*)hJESminus_temp->Clone("hJESminus2");
  hJESminus2->Reset(); 
  



  //  TH1F *hPulls_add = (TH1F*)inputFile->Get("h_DijetMass_mc_wide");
  hPulls_add = (TH1F*) hQCD->Clone("hPulls_add");

  hPulls_add->Reset();
  

  // Pave text
  TPaveText *pave_fit_publi = new TPaveText(0.18,0.16,0.40,0.28,"NDC");
  
  pave_fit_publi->AddText("|#eta_{1,2}| < 2.5");
  pave_fit_publi->AddText("|#Delta#eta_{12}| < 1.3");
  pave_fit_publi->SetFillColor(0);
  pave_fit_publi->SetLineColor(0);
  pave_fit_publi->SetFillStyle(0);
  pave_fit_publi->SetBorderSize(0);
  pave_fit_publi->SetTextFont(42);
  pave_fit_publi->SetTextSize(0.04);
  pave_fit_publi->SetTextAlign(12); 

  TPaveText *pave = new TPaveText(0.30,0.72,0.65,0.9,"NDC");
 
 
  cout << " make hQCD_Xsec histogram " << endl;
    
  
  hQCD->Scale(scaleFactor*lumi); 
  hJESplus_temp->Scale(scaleFactor*lumi);
  hJESminus_temp->Scale(scaleFactor*lumi);
  
   
  for(int i=0; i<hDijetMass->GetNbinsX(); i++){
    float n   = hQCD->GetBinContent(i+1);
    float err = hQCD->GetBinError(i+1);    
    float dm  = hQCD->GetBinWidth(i+1);
    float mjj = hQCD->GetBinCenter(i+1);
    
    float nSig   = hSignal->GetBinContent(i+1);

    float genLumi = 1./2.8522E-7;

    if(n>=1.0){
      hQCD_Xsec->SetBinContent(i+1, n/(dm*lumi));
      hQCD_Xsec->SetBinError(i+1, err/(dm*lumi)*4);
      hSignal->SetBinContent(i+1, nSig/(dm*genLumi));
    }
    else{
      hQCD_Xsec->SetBinContent(i+1, 0.);
      hQCD_Xsec->SetBinError(i+1, 0.);
      hSignal->SetBinContent(i+1, 0.);
    }
    
  }
 
  

  
  for(int i=0; i<hDijetMass->GetNbinsX(); i++){
    float n   = hJESplus_temp->GetBinContent(i+1);
    float err = hJESplus_temp->GetBinError(i+1);    
    float dm  = hJESplus_temp->GetBinWidth(i+1);
    float mjj = hJESplus_temp->GetBinCenter(i+1);

    if(n>=1.0){
      hJESplus_temp2->SetBinContent(i+1, n/(dm*lumi));
      hJESplus_temp2->SetBinError(i+1, err/(dm*lumi)*4);
    }
    else{
      hJESplus_temp2->SetBinContent(i+1, 0.);
      hJESplus_temp2->SetBinError(i+1, 0.);
    }
    
  }
  
  // make hJESminus temp, this is hJESminus differential cross sction format

  for(int i=0; i<hDijetMass->GetNbinsX(); i++){
    float n   = hJESminus_temp->GetBinContent(i+1);
    float err = hJESminus_temp->GetBinError(i+1);    
    float dm  = hJESminus_temp->GetBinWidth(i+1);
    float mjj = hJESminus_temp->GetBinCenter(i+1);
    
    
    
    if(n>=1.0){
      hJESminus_temp2->SetBinContent(i+1, n/(dm*lumi));
      hJESminus_temp2->SetBinError(i+1, err/(dm*lumi)*4);
    }
    else{
      hJESminus_temp2->SetBinContent(i+1, 0.);
      hJESminus_temp2->SetBinError(i+1, 0.);
    }
    

  }
   
 
  cout << "Fit QCD " << endl << endl << endl;


  // QCD Fit -- fit to qcd
  TF1 *f_qcd = new TF1("fit_qcd",fitQCD1, 943.0,4686.0,4); 
  gStyle->SetOptFit(1111); 
  f_qcd->SetParameter(0,6.15613e+18*2);
  f_qcd->SetParameter(1,-3.75321e+00);
  f_qcd->SetParameter(2,6.32703e+00);
  f_qcd->SetParameter(3,6.33149e+00);
  hQCD_Xsec->Fit("fit_qcd", "R");  
  hQCD_Xsec->Fit("fit_qcd", "R");
  hQCD_Xsec->Fit("fit_qcd", "R");

  cout << "NDF = " << fit_qcd->GetNDF() << " FCN = " << fit_qcd->GetChisquare() << endl;

  cout << "Fit QCD Up" << endl << endl << endl;

  // QCD up Fit -- fit to JES plus
  TF1 *f_qcd_up = new TF1("fit_qcd_up",fitQCD1,943.,4686.0,4); 
  gStyle->SetOptFit(1111); 
  f_qcd_up->SetParameter(0,1.47700e+19*2);
  f_qcd_up->SetParameter(1,-4.03984e+00);
  f_qcd_up->SetParameter(2,6.44822e+00);
  f_qcd_up->SetParameter(3,5.36806e+00);
  hJESplus_temp2->Fit("fit_qcd_up","R");
  hJESplus_temp2->Fit("fit_qcd_up","R");
  hJESplus_temp2->Fit("fit_qcd_up","R");

  cout << "Fit QCD Do" << endl << endl << endl;

  // QCD down Fit -- fit to JES minus
  TF1 *f_qcd_down = new TF1("fit_qcd_down",fitQCD1,943.,4686.0,4); 
  gStyle->SetOptFit(1111); 
  f_qcd_down->SetParameter(0,2.12985e+19*2);
  f_qcd_down->SetParameter(1,-4.63606e+00);
  f_qcd_down->SetParameter(2,6.54632e+00);
  f_qcd_down->SetParameter(3,4.92684e+00);
  hJESminus_temp2->Fit("fit_qcd_down","R");
  hJESminus_temp2->Fit("fit_qcd_down","R");
  hJESminus_temp2->Fit("fit_qcd_down","R");

  // Differantial Cross Section -- make hQCD_xsec in graph format.
  // JES plus and minus is also smoothed with fit to make JES band.

  float a = 0.3173/2;
  float n,nl,nh;
  float n, dm, mass, xl, xh;
  float vx[1200],vy[1200],vexl[1200],vexh[1200],veyl[1200],veyh[1200];
  float vxWithSignal[1200],vyWithSignal[1200],vexlWithSignal[1200],vexhWithSignal[1200],veylWithSignal[1200],veyhWithSignal[1200];
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
      vy[i]   = n / (dm*lumi); 

     
      if (n<25 && mass>889 && mass<=2659.0)
       	{
	  nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
	  nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
	  veyl[i] = nl/(lumi*dm);
	  veyh[i] = nh/(lumi*dm);  
	}
      else if (n<25 && mass<=889 && mass>4686 && n>0)
	{
	  nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
	  nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
	  veyl[i] = nl/(lumi*dm);
	  veyh[i] = nh/(lumi*dm);  
	}
      else if (n>=25 && mass >= 889)
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
	  break;
	}	  
      }
    }
  // these are the graph format of data.

  TGraphAsymmErrors *gFinal = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);

  // Fit to data    
  // Fit to data anly at low mass    


  TF1 *fit = new TF1("fit",fitQCD1,mMin,mMax,4); // 4 Par. Fit
  gStyle->SetOptFit(1111); 
  fit->SetParameter(0,1.99e-05);
  fit->SetParameter(1,5.689);
  fit->SetParameter(2,6.321);
  fit->SetParameter(3,0.221);
  fit->SetLineWidth(2);
  fit->SetLineColor(4);

  gFinal->Fit("fit","","",mMin,mMax);	
  TFitResultPtr fitResult=gFinal->Fit("fit","S","",mMin,mMax);	
   





  TObjArray* aData = new TObjArray(30);
  TObjArray* aRatio = new TObjArray(30);
  TObjArray* aPull = new TObjArray(30);


gStyle->SetOptFit(1111);

 double chi2 = 0;
 int ndf = 0;

  for(int i=0;i<hDijetMass->GetNbinsX();i++)
    {
      float data = vy[i];
      float error = veyh[i];
      float m = vx[i];
      float fit_default = fit->Eval(m,0,0);  

      if(data>=fit_default) float error = veyl[i];
      if (data<fit_default) float error = veyh[i];
			
 		

		
		
      if(error != 0. && m > mMin && m < mMax){

	ndf++;

	double dPull = (data-fit_default)/error;

	hPulls_add->SetBinContent(i+1,dPull);
	hPulls_add->SetBinError(i+1,1.);

	cout << " dPull*dPull = " <<  dPull*dPull << endl;
	chi2 += dPull*dPull;


      } else {
	hPulls_add->SetBinContent(i+1, 0);
	hPulls_add->SetBinError(i+1, 0);
      }
    }


  cout << "chi2 = " << chi2 << " ndf = " << ndf - 4<< endl; 



TCanvas* c12 = new TCanvas("c12","default fit and pull");


c12->Divide(1,2,0,0,0);
c12->cd(1);

c12->SetTopMargin(0.30);

   
p12_1 = (TPad*)c12->GetPad(1);
p12_1->SetPad(0.01,0.22,0.99,0.98);
p12_1->SetLogy();
p12_1->SetRightMargin(0.05);
p12_1->SetTopMargin(0.07);
   
TH1F *vFrame = p12_1->DrawFrame(838.0,0.00000001,2780,0.1);
vFrame->SetTitle("");
vFrame->SetXTitle("Dijet Mass (GeV)");
vFrame->GetXaxis()->SetTitleSize(0.01);
vFrame->SetYTitle("d#sigma/dm (pb/GeV)");
vFrame->Draw();



  THStack *h = new THStack("JES","JES");
  TH1F *htmp = (TH1F*)hJESplus->Clone("tmp");
  htmp->Add(hJESminus,-1);
  h->Add(hJESminus);
  h->Add(htmp);


h->Draw("sameCA");
p12_1->Update();

TPaveText* T = new TPaveText(0.13, 0.93, 0.3, 0.99, "brNDC");
T->SetFillColor(0);
T->SetFillStyle(0);
T->SetBorderSize(0);
T->SetLineColor(0);
T->SetTextSize(0.04);
T->SetTextAlign(12);
T->AddText("CMS");



TPaveText* T1 = new TPaveText(0.5, 0.93, 0.7, 0.99, "brNDC");
T1->SetFillColor(0);
T1->SetFillStyle(0);
T1->SetBorderSize(0);
T1->SetLineColor(0);
T1->SetTextSize(0.04);
T1->SetTextAlign(12);
T1->AddText("L = 20 fb^{-1}");



TPaveText* T2 = new TPaveText(0.81, 0.93, 0.95, 0.99, "brNDC");
T2->SetFillColor(0);
T2->SetFillStyle(0);
T2->SetBorderSize(0);
T2->SetLineColor(0);
T2->SetTextSize(0.04);
T2->SetTextAlign(12);
T2->AddText("#sqrt{s} = 8 TeV");

T->Draw();
T1->Draw();
T2->Draw();




f_qcd->Draw("same");

 

TF1 *fitFinal = new TF1("fitFinal",fitQCD1,mMin,mMax,4); // 4 Par. Fit
gStyle->SetOptFit(0000); 
fitFinal->SetParameter(0,1.99e-05);
fitFinal->SetParameter(1,5.689);
fitFinal->SetParameter(2,6.321);
fitFinal->SetParameter(3,0.221);
fitFinal->SetLineWidth(2);
fitFinal->SetLineColor(4);
gFinal->Fit("fitFinal","","",mMin, mMax);
gFinal->Fit("fitFinal","","",mMin, mMax);
gFinal->Fit("fitFinal","","",mMin, mMax);
gFinal->Fit("fitFinal","","",mMin, mMax);
gFinal->Fit("fitFinal","","",mMin, mMax);
gFinal->Draw("samePZ");

 hSignal->SetLineStyle(2);
 hSignal->SetLineColor(kRed);
 hSignal->Draw("SAMEC");
    
//    c12->Update();
//   g->PaintStats(0);


TLegend *leg = new TLegend(0.44,0.55,0.86,0.90);
leg->SetTextSize(0.035);
leg->SetLineColor(1);
leg->SetLineStyle(1);
leg->SetLineWidth(1);
leg->SetFillColor(0);
leg->AddEntry(gFinal,"Data", "PEL"); //Form("CMS  L = %.1f fb^{-1}   ", lumi/1000.),"PL"); 
leg->AddEntry(fit,"Fit","L");
leg->AddEntry(f_qcd,"QCD Pythia","L");
leg->AddEntry(htmp,"Jet Energy Scale Uncertainty","F");
//  leg->AddEntry(gr_qstar1,"Excited Quark + CMS Simulation","L");
//   leg->AddEntry(gr_string1,"String Resonance + CMS Simulation","L");
leg->Draw("same");

pave_fit_publi->Draw("same");


TPaveText *pt_c12_qstar1 = new TPaveText(0.23,0.31,0.40,0.41,"NDC");
pt_c12_qstar1->SetFillColor(0);
pt_c12_qstar1->SetFillStyle(0);
pt_c12_qstar1->SetBorderSize(0);
pt_c12_qstar1->SetTextColor(2);
pt_c12_qstar1->SetTextSize(0.04);
pt_c12_qstar1->AddText("M(R->HH) = 1.5 TeV");


//    pt_c12_string1->Draw("sames");
pt_c12_qstar1->Draw("sames");
//   pt_c12_qstar2->Draw("sames");
   
c12->cd(2);
p12_2 = (TPad*)c12->GetPad(2);
p12_2->SetPad(0.01,0.02,0.99,0.24);
p12_2->SetBottomMargin(0.35);
p12_2->SetRightMargin(0.05);
p12_2->SetGridx();
c12_2->SetTickx(50);


TH1F *vFrame2 = p12_2->DrawFrame(838.0, -2.49, 2780., 2.49);
vFrame2->SetTitle("");
vFrame2->SetXTitle("Dijet Mass (GeV)");
vFrame2->GetXaxis()->SetTitleSize(0.06);
vFrame2->SetYTitle("(Data-Fit)/#sigma_{Data} ");
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

TLine *line = new TLine(820.,0,2659.,0);
line->Draw("");
   
    
c12->SaveAs("Plots/DefaultFitAndPull_noStat.png");
c12->SaveAs("Plots/DefaultFitAndPull_noStat.eps");
    

 

}

