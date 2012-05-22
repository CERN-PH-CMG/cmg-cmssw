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
    return p[0]*pow(1.-x+p[3]*x*x,p[1])/pow(x,p[2]);
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
    return p[0]*pow(1.-x,p[1])/pow(x,p[2]);
}


void DijetMass_chiyoung_1Vtag(){

  string sReco("pf");

  Init("qW");


  double mMin = 890.0;
  // double mMin = 565.0;

//  lumi = 1025.;
//  lumi = 2509.0;
  //  lumi = 2168.0;
//  lumi = 4677.0;
  lumi = 5000.0;


  Int_t ci_g, ci_b;   // for color index setting
  ci_g = TColor::GetColor("#006600");
  ci_b = TColor::GetColor("#000099");
  
  // Set TDR Style	
  gROOT->ProcessLine(".L tdrStyle.C");
  gROOT->ProcessLine("setTDRStyle()");
  
  // Input Files  
  TFile *inputFile = TFile::Open("lumi46fb_dataMC_data_1tag.root", "READ");



  // Histograms 
  hDijetMass = (TH1F*) inputFile->Get("mjj norm-data-tagged 0False 1tag;1");
  //inputFileMC = TFile::Open("histograms_Fat30_summer11_mc_ak5.root", "UPDATE");
  //hQCD = (TH1F*)inputFileMC->Get("h_DijetMass_data_fat;1");
  
  //cout <<"Calc scale factor" << endl;
  //scaleFactor = hDijetMass->Integral(37, hDijetMass->GetNbinsX()) / hQCD->Integral(37, hDijetMass->GetNbinsX()) / lumi;
  //cout <<"scaleFactor = " << scaleFactor << endl;

                                      
  //hQCD_Xsec = (TH1F*)hDijetMass->Clone("hQCD_Xsec");
  //hQCD_Xsec->Reset();
  
  //  TH1F *hJESplus_temp = (TH1F*)inputFile->Get("fit_DijetMass_mc_up_wide");
  //  TH1F *hJESminus_temp = (TH1F*)inputFile->Get("fit_DijetMass_mc_down_wide");
  
  //hJESplus_temp = (TH1F*)inputFileMC->Get("h_DijetMass_data_fat_up;1");
  //hJESminus_temp = (TH1F*)inputFileMC->Get("h_DijetMass_data_fat_do;1");

  //TH1F *hJESplus_temp2 =  (TH1F*)hJESplus_temp->Clone("hJESplus_temp2");
  //hJESplus_temp2->Reset();
  //TH1F *hJESminus_temp2 = (TH1F*)hJESplus_temp->Clone("hJESminus_temp2");
  //hJESminus_temp2->Reset();

  //TH1F *hJESplus =  (TH1F*)hJESplus_temp->Clone("hJESplus");
  //hJESplus->Reset();
  //TH1F *hJESminus = (TH1F*)hJESplus_temp->Clone("hJESminus");
  //hJESminus->Reset();

  //TH1F *hJESplus2 = (TH1F*)hJESplus_temp->Clone("hJESplus2");
  //hJESplus2->Reset();
  //TH1F *hJESminus2 = (TH1F*)hJESplus_temp->Clone("hJESminus2");
  //hJESminus2->Reset(); 
  
  TH1F *hPulls_3par = (TH1F*)hDijetMass->Clone("Pulls_3par");
  TH1F *hPulls_4par = (TH1F*)hDijetMass->Clone("Pulls_4par");
  TH1F *hPulls = (TH1F*)hDijetMass->Clone("Pulls");
  //  TH1F *hPulls_add = (TH1F*)inputFile->Get("h_DijetMass_mc_wide");
  hPulls_add = (TH1F*) hDijetMass->Clone("hPulls_add");

  hPulls_3par->Reset();
  hPulls_4par->Reset();
  hPulls->Reset();
  hPulls_add->Reset();
  
  // Pave text
  TPaveText *pave_fit = new TPaveText(0.60,0.50,0.90,0.65,"NDC");
  //  pave_fit->AddText("CMS Preliminary");

  
  pave_fit->AddText(" #sqrt{s} = 7 TeV");
  pave_fit->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  //  pave_fit->AddText("M_{jj} > 890 GeV");
  if (sReco.find("pf") != string::npos)   pave_fit->AddText("Anti-K_{T} R=0.5");
  else   pave_fit->AddText("Wide Jets");

  pave_fit->SetFillColor(0);
  pave_fit->SetLineColor(0);
  pave_fit->SetFillStyle(0);
  pave_fit->SetBorderSize(0);
  pave_fit->SetTextFont(42);
  pave_fit->SetTextSize(0.04);
  pave_fit->SetTextAlign(12); 
  
  TPaveText *pave = new TPaveText(0.55,0.72,0.9,0.9,"NDC");
  
  //  pave->AddText("CMS Preliminary");
  

  pave->AddText(Form("data (%.1f fb^{-1})", lumi/1000.)); 
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
  pave->SetTextSize(0.04);
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
  
  
  //hQCD->Scale(scaleFactor*lumi); 
  //hJESplus_temp->Scale(scaleFactor*lumi);
  //hJESminus_temp->Scale(scaleFactor*lumi);
  
   
  /*for(int i=0; i<hDijetMass->GetNbinsX(); i++){
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
  */ 
  cout << "make hJESplus temp, this is hJESplus differential cross sction format" << endl;
  

  
  /*for(int i=0; i<hDijetMass->GetNbinsX(); i++){
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
  */ 
 
  cout << "Fit QCD " << endl << endl << endl;


   // QCD Fit -- fit to qcd
   TF1 *f_qcd = new TF1("fit_qcd",fitQCD,mMin,3300.0,4); 
   gStyle->SetOptFit(1111); 
   f_qcd->SetParameter(0,6.15613e+18);
   f_qcd->SetParameter(1,-3.75321e+00);
   f_qcd->SetParameter(2,6.32703e+00);
   f_qcd->SetParameter(3,6.33149e+00);
   //hQCD_Xsec->Fit("fit_qcd", "R");

   cout << "NDF = " << fit_qcd->GetNDF() << " FCN = " << fit_qcd->GetChisquare() << endl;

  cout << "Fit QCD Up" << endl << endl << endl;

   // QCD up Fit -- fit to JES plus
   TF1 *f_qcd_up = new TF1("fit_qcd_up",fitQCD,mMin,3300.0,4); 
   gStyle->SetOptFit(1111); 
   f_qcd_up->SetParameter(0,1.47700e+19);
   f_qcd_up->SetParameter(1,-4.03984e+00);
   f_qcd_up->SetParameter(2,6.44822e+00);
   f_qcd_up->SetParameter(3,5.36806e+00);
   //hJESplus_temp2->Fit("fit_qcd_up","R");

   cout << "Fit QCD Do" << endl << endl << endl;

   // QCD down Fit -- fit to JES minus
   TF1 *f_qcd_down = new TF1("fit_qcd_down",fitQCD,mMin,3300.0,4); 
   gStyle->SetOptFit(1111); 
   f_qcd_down->SetParameter(0,2.12985e+19);
   f_qcd_down->SetParameter(1,-4.63606e+00);
   f_qcd_down->SetParameter(2,6.54632e+00);
   f_qcd_down->SetParameter(3,4.92684e+00);
   //hJESminus_temp2->Fit("fit_qcd_down","R");

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

     
	if (n<25 && mass>565 && mass<=3300)
       	{
		nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
		nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
		veyl[i] = nl/(lumi*dm);
		veyh[i] = nh/(lumi*dm);  
	}
	else if (n<25 && mass>3300 && n>0)
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
	//hJESplus->SetBinContent(i+1,1.0*f_qcd_up->Eval(mass ) );
        //hJESminus->SetBinContent(i+1,1.0*f_qcd_down->Eval(mass ) );
	  
    }

   // these are the graph format of data.

    TGraphAsymmErrors *g = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
    TGraphAsymmErrors *g2 = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
    TGraphAsymmErrors *gDefault = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
    TGraphAsymmErrors *g_3par = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);
    TGraphAsymmErrors *g_4par = new TGraphAsymmErrors(i,vx,vy,vexl,vexh,veyl,veyh);


  // Fit to data    
  // Fit to data anly at low mass    
  TF1 *fit_lowMass = new TF1("fit_lowMass",fitQCD1,mMin,3300.0,4); // 4 Par. Fit
  gStyle->SetOptFit(1111); 
  fit_lowMass->SetParameter(0,1.73132e-05);
  fit_lowMass->SetParameter(1,6.80678e+00);
  fit_lowMass->SetParameter(2,6.33620e+00);
  fit_lowMass->SetParameter(3,1.93728e-01);
  fit_lowMass->SetLineWidth(2);
  fit_lowMass->SetLineColor(kMagenta);
  g->Fit("fit_lowMass","","",mMin,2000.0);
  g->Fit("fit_lowMass","","",mMin,2000.0);
 
  double P0 = fit_lowMass->GetParameter(0), P1 = fit_lowMass->GetParameter(1);
  double P2 = fit_lowMass->GetParameter(2);

  fit_lowMass->FixParameter(0, P0); fit_lowMass->FixParameter(1, P1);
  fit_lowMass->FixParameter(2, P2);

  g->Fit("fit_lowMass","","",mMin,3300.0);


    // Fit to data    
    TF1 *fit = new TF1("fit",fitQCD1,mMin,3300.0,4); // 4 Par. Fit
    gStyle->SetOptFit(0); 
    fit->SetParameter(0,1.73132e-05);
    fit->SetParameter(1,6.80678e+00);
    fit->SetParameter(2,6.33620e+00);
    fit->SetParameter(3,1.93728e-01);
    fit->SetLineWidth(2);
    fit->SetLineColor(4);

    g->Fit("fit","","",mMin,3300.0);	
    g->Fit("fit","","",mMin,3300.0);	
    TFitResultPtr fitResult=g->Fit("fit","S","",mMin,3300.0);	
    
    //Alternate Fits 4 parameter
    TF1 *f_4par = new TF1("fit_4par",fitQCD,mMin,3300.0,4); // 4 Par. Fit
    gStyle->SetOptFit(0); 
    f_4par->SetParameter(0,1.73132e-05);
    f_4par->SetParameter(1,6.80678e+00);
    f_4par->SetParameter(2,6.33620e+00);
    f_4par->SetParameter(3,6.49182e-02);

    f_4par->SetLineWidth(2);
    f_4par->SetLineColor(TColor::GetColor("#009900"));

    g_4par->Fit("fit_4par","","",mMin,3300.0);	
    g_4par->Fit("fit_4par","","",mMin,3300.0);
    g_4par->Fit("fit_4par","","",mMin,3300.0);

    //Alternate Fits 3 parameter
    TF1 *f_3par = new TF1("fit_3par",fitQCD2,mMin,3300.0,3); // 3 Par. Fit
    gStyle->SetOptFit(0);
    f_3par->SetParameter(0,1.73132e-05);
    f_3par->SetParameter(1,6.80678e+00);
    f_3par->SetParameter(2,6.33620e+00); 
    f_3par->SetLineWidth(2);
    f_3par->SetLineColor(2);
    f_3par->SetLineStyle(2);

    g_3par->Fit("fit_3par","","",mMin,3300.0);	
    g_3par->Fit("fit_3par","","",mMin,3300.0);	
    g_3par->Fit("fit_3par","","",mMin,3300.0);	








  gStyle->SetOptFit(0000);


  TObjArray* aData = new TObjArray(30);
  TObjArray* aFit = new TObjArray(30);

  TObjArray* aRatio = new TObjArray(30);

  TGraphAsymmErrors *gRatio_4par = new TGraphAsymmErrors(*gDefault);
  TGraphAsymmErrors *gRatio_3par = new TGraphAsymmErrors(*gDefault);
  TGraphAsymmErrors *gRatio_lowMass = new TGraphAsymmErrors(*gDefault);

  TObjArray* aPull = new TObjArray(30);

  TGraphAsymmErrors *gPull_4par = new TGraphAsymmErrors(*gDefault);
  TGraphAsymmErrors *gPull_3par = new TGraphAsymmErrors(*gDefault);
  TGraphAsymmErrors *gPull_lowMass = new TGraphAsymmErrors(*gDefault);


  double nbkgval=fit->GetParameter(0);
  double p1val=fit->GetParameter(1);
  double p2val=fit->GetParameter(2);
  double p3val=fit->GetParameter(3);

  TMatrixDSym covarianceMatrix=fitResult->GetCovarianceMatrix();

        // Code taken from TFitResult.RandomizePars
        Int_t nPar= fit->GetNpar();
        // calculate the elements of the upper-triangular matrix L that gives Lt*L = C
        // where Lt is the transpose of L (the "square-root method")
        TMatrix L(nPar,nPar);
        for(Int_t iPar= 0; iPar < nPar; iPar++) {
     	  // calculate the diagonal term first
     	  L(iPar,iPar)= covarianceMatrix(iPar,iPar);
     	  for(Int_t k= 0; k < iPar; k++) {
     	    Double_t tmp= L(k,iPar);
     	    L(iPar,iPar)-= tmp*tmp;
     	  }
     	  L(iPar,iPar)= sqrt(L(iPar,iPar));
     	  // then the off-diagonal terms
     	  for(Int_t jPar= iPar+1; jPar < nPar; jPar++) {
     	    L(iPar,jPar)= covarianceMatrix(iPar,jPar);
     	    for(Int_t k= 0; k < iPar; k++) {
     	      L(iPar,jPar)-= L(k,iPar)*L(k,jPar);
     	    }
     	    L(iPar,jPar)/= L(iPar,iPar);
     	  }
        }
        // remember Lt
        TMatrix* _Lt= new TMatrix(TMatrix::kTransposed,L);
        TVectorD eigenValues(4);
        TMatrixD eigenVectors=covarianceMatrix.EigenVectors(eigenValues);
        std::cout << "EigenVectors" << std::endl;
        for(Int_t k= 0; k < nPar; k++) {
     	  std::cout << k << ": ";
     	  for(Int_t l= 0; l < nPar; l++) {
     	    std::cout << eigenVectors[k][l] << " ";
     	  }
     	  std::cout << std::endl;
        }

        TF1* variations[6];
      
         TVector gv(nPar);
         Double_t pars[4];
	 for(Int_t vi=1; vi<nPar;vi++){
	  for(Int_t k= 0; k < nPar; k++) gv(k)=eigenVectors[vi][k];
	  // multiply this vector by Lt to introduce the appropriate correlations
	  gv*= (*_Lt);
	  stringstream ss;
	  ss << "up" << vi;
	  TF1* variationUp=new TF1(ss.str().c_str(),fitQCD1,mMin,3300.0,4);
	  pars[0]=nbkgval;
	  pars[1]=p1val+gv(1);
	  pars[2]=p2val+gv(2);
	  pars[3]=p3val+gv(3);
	  variationUp->SetParameters(pars);
	  variations[2*vi-2]=variationUp;
	  for(Int_t k= 0; k < nPar; k++) gv(k)=-eigenVectors[vi][k];
	  // multiply tLos vector by Lt to introduce the appropriate correlations
	  gv*= (*_Lt);
	  ss << "down";
	  TF1* variationDown=new TF1(ss.str().c_str(),fitQCD1,mMin,3300.0,4);
	  pars[0]=nbkgval;
	  pars[1]=p1val+gv(1);
	  pars[2]=p2val+gv(2);
	  pars[3]=p3val+gv(3);
	  variationDown->SetParameters(pars);
	  variations[2*vi-1]=variationDown;
	 }

  double fMass, Xsec;

  for (int iWindow = 0; iWindow < 15; iWindow++){

    TGraphAsymmErrors *gWindow = new TGraphAsymmErrors(*gDefault);
    TGraphAsymmErrors *gRatio = new TGraphAsymmErrors(*gDefault);
    TGraphAsymmErrors *gPull = new TGraphAsymmErrors(*gDefault);

    TF1 *fit_window = new TF1(Form("fit_window_%d",iWindow),fitQCD1,mMin,3300.0,4); // 4 Par. Fit
    fit_window->SetParameter(0,1.73132e-05);
    fit_window->SetParameter(1,6.80678e+00);
    fit_window->SetParameter(2,6.33620e+00);
    fit_window->SetParameter(3,1.93728e-01);
    fit_window->SetLineWidth(2);
    fit_window->SetLineColor(4);

    gWindow->SetPointEYhigh(1+iWindow, 1000);
    gWindow->SetPointEYlow(1+iWindow, 1000);

    gWindow->SetPointEYhigh(2+iWindow, 1000);
    gWindow->SetPointEYlow(2+iWindow, 1000);

    gWindow->SetPointEYhigh(3+iWindow, 1000);
    gWindow->SetPointEYlow(3+iWindow, 1000);


    aData->AddAt(gWindow, iWindow);
    
    gWindow->Fit(Form("fit_window_%d",iWindow),"","",mMin,3300.0);
    gWindow->Fit(Form("fit_window_%d",iWindow),"","",mMin,3300.0);


    for (int i = 0; i < gRatio->GetN(); i++){
      gRatio->GetPoint(i, fMass, Xsec);

      float XsecWindow = fit_window->Eval(fMass,0,0);
      float XsecDefault = fit->Eval(fMass,0,0);
      
      if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio->SetPoint(i, fMass, XsecWindow/XsecDefault); 
      if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

      if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio->SetPoint(i, fMass, 1.0); 
      if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 


    if (iWindow == 0){

	float XsecWindow_4par = f_4par->Eval(fMass,0,0);
	float XsecWindow_3par = f_3par->Eval(fMass,0,0);
	float XsecWindow_lowMass = fit_lowMass->Eval(fMass,0,0);

	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio_4par->SetPoint(i, fMass, XsecWindow_4par/XsecDefault); 
	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio_4par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio_3par->SetPoint(i, fMass, XsecWindow_3par/XsecDefault); 
	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio_3par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio_lowMass->SetPoint(i, fMass, XsecWindow_lowMass/XsecDefault); 
	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gRatio_lowMass->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 


	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio_4par->SetPoint(i, fMass, 1.0); 
	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio_4par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio_3par->SetPoint(i, fMass, 1.0); 
	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio_3par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio_lowMass->SetPoint(i, fMass, 1.0); 
	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gRatio_lowMass->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

      }

    }

    for (int i = 0; i < gPull->GetN(); i++){
      gPull->GetPoint(i, fMass, Xsec);

      float XsecWindow = fit_window->Eval(fMass,0,0);
      float XsecDefault = fit->Eval(fMass,0,0);
      float XsecVariationDefault = 0;
      for(Int_t k= 0; k < (nPar-1)*2; k++) XsecVariationDefault+=pow(variations[k]->Eval(fMass)-XsecDefault,2);
      XsecVariationDefault=sqrt(XsecVariationDefault);
      
      if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull->SetPoint(i, fMass, (XsecWindow-XsecDefault)/XsecVariationDefault); 
      if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

      if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull->SetPoint(i, fMass, 0.0); 
      if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 


    if (iWindow == 0){

	float XsecWindow_4par = f_4par->Eval(fMass,0,0);
	float XsecWindow_3par = f_3par->Eval(fMass,0,0);
	float XsecWindow_lowMass = fit_lowMass->Eval(fMass,0,0);

	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull_4par->SetPoint(i, fMass, (XsecWindow_4par-XsecDefault)/XsecVariationDefault); 
	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull_4par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull_3par->SetPoint(i, fMass, (XsecWindow_3par-XsecDefault)/XsecVariationDefault); 
	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull_3par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull_lowMass->SetPoint(i, fMass, (XsecWindow_lowMass-XsecDefault)/XsecVariationDefault); 
	if (Xsec > 1e-10 || (fMass < 3500.0 && fMass > mMin)) gPull_lowMass->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 


	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull_4par->SetPoint(i, fMass, 0.0); 
	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull_4par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull_3par->SetPoint(i, fMass, 0.0); 
	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull_3par->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull_lowMass->SetPoint(i, fMass, 0.0); 
	if (Xsec < 1e-10 && (fMass > 3500.0 || fMass < mMin)) gPull_lowMass->SetPointError(i, 1e-10, 1e-10, 1e-10, 1e-10); 

      }


    }


    aFit->AddAt(fit_window, iWindow);
    aRatio->AddAt(gRatio, iWindow);
    aPull->AddAt(gPull, iWindow);

  }

  cout <<"Calculating default fit variations done" << endl;













  //Dijet Mass Cross Section with Fit	
  TCanvas* c0 = new TCanvas("c0","DijetMass Cross Section with Fit");
  c0->SetLogy(1);
  g->SetTitle("");
  g->SetLineColor(1);
  g->SetFillColor(1);
  g->SetMarkerColor(1);
  g->SetMarkerStyle(20);
  g->GetXaxis()->SetTitle("Dijet Mass (GeV)");
  g->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");
  g->GetXaxis()->SetRangeUser(890,3300.0);
  g->GetYaxis()->SetRangeUser(0.0000003,0.02);
  g->DrawClone("APZ");

  for (int iWindow = 0; iWindow < 15; iWindow++){

    ((TF1*) aFit->At(iWindow))->SetLineColor(iWindow+1);
    ((TF1*) aFit->At(iWindow))->Draw("SAME");

  }

  c0->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFit.png");
  c0->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFit.pdf");



 //Dijet Mass Cross Section with Fit	
  TCanvas* c01 = new TCanvas("c01","DijetMass Cross Section with Window Fit");
  c01->Divide(5,3);

 for (int iWindow = 0; iWindow < 15; iWindow++){ 

   c01->cd(iWindow+1);
   ((TGraphAsymmErrors*) aData->At(iWindow))->SetTitle("");
   ((TGraphAsymmErrors*) aData->At(iWindow))->SetLineColor(1);
   ((TGraphAsymmErrors*) aData->At(iWindow))->SetFillColor(1);
   ((TGraphAsymmErrors*) aData->At(iWindow))->SetMarkerColor(1);
   ((TGraphAsymmErrors*) aData->At(iWindow))->SetMarkerStyle(20);
   ((TGraphAsymmErrors*) aData->At(iWindow))->SetMarkerSize(0.5);
   ((TGraphAsymmErrors*) aData->At(iWindow))->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   ((TGraphAsymmErrors*) aData->At(iWindow))->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");
   ((TGraphAsymmErrors*) aData->At(iWindow))->GetXaxis()->SetRangeUser(890,3300.0);
   ((TGraphAsymmErrors*) aData->At(iWindow))->GetYaxis()->SetRangeUser(0.0000003,60);
   ((TGraphAsymmErrors*) aData->At(iWindow))->Draw("APZ");

   gPad->SetLogy();

 }

c01->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFits.png");
c01->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFits.pdf");











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
    g->GetXaxis()->SetRangeUser(890,3300.0);
    g->GetYaxis()->SetRangeUser(0.0000001,2);
    g->Draw("APZ");


    TLegend *leg = new TLegend(0.25,0.77,0.48,0.92);
    leg->SetTextSize(0.04);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->AddEntry(g,Form("Single W/Z-tag data (%.1f fb^{-1})", lumi/1000.),"PL"); 
    leg->AddEntry(fit,"Fit","L");
    leg->Draw("same");
    pave_fit->Draw("same");
    c1->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithFit.png");
    c1->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithFit.pdf");


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
    g2->GetXaxis()->SetRangeUser(890,3300);
    g2->GetYaxis()->SetRangeUser(0.0000001,2);
    g2->Draw("APZ");
    g2->Fit("fit","","sames",mMin,3300.0);

    TString status_default= gMinuit->fCstatu.Data();
    g2->Fit("fit_4par","+","sames",mMin,3300.0);
    TString status_4par= gMinuit->fCstatu.Data();
    g2->Fit("fit_3par","+","sames",mMin,3300.0);
    TString status_3par= gMinuit->fCstatu.Data();
    TLegend *leg = new TLegend(0.18,0.78,0.38,0.92);
    leg->SetTextSize(0.04);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->AddEntry(g,Form("Single W/Z-tag data (%.1f fb^{-1})", lumi/1000.),"PL");
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
    /*
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
    htmp->GetXaxis()->SetRangeUser(890,3300);
    htmp->SetFillColor(5);
    htmp->SetLineColor(1);
    htmp->SetLineWidth(1); 
    htmp->SetLineColor(5);
    */
    c2->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithAllFit.png");
    c2->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithAllFit.pdf");

    /*
    //DijetMass cross section with Fit and QCD MC 
    TCanvas *c3 = new TCanvas("c3","DijetMass cross section with Fit and QCD MC");
    c3->SetLogy();
    h->Draw("C");
    h->GetXaxis()->SetRangeUser(890,3300);
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
    leg->SetTextSize(0.04);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->AddEntry(g2, Form("data (%.1f fb^{-1})", lumi/1000.),"PL");
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
	  
	  hJESplus2->SetBinContent(i+1, hJESplus->GetBinContent(i+1) / qcd);
	  hJESminus2->SetBinContent(i+1, hJESminus->GetBinContent(i+1) / qcd);

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
   c3->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithFitAndQCDMC.png");
   c3->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithFitAndQCDMC.pdf");

   // Data ovet PTYHIA QCD MC	
   TCanvas *c4 = new TCanvas("Ratio","Data/PYTHIA");

 
   h2->Draw("C");  
   h2->SetTitle("");
   h2->GetXaxis()->SetRangeUser(890.,3300.);
   h2->GetYaxis()->SetRangeUser(0.,3.);
   h2->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   h2->GetXaxis()->SetTitleSize(0.06);
   h2->GetYaxis()->SetTitle("Data / PYTHIA"); 

   
   //   htmp2->Draw("same");
   gratio->Draw("PSAME");
   TLine *l = new TLine(890, 1., 3300., 1.);
   l->Draw("same");

   TLegend *leg = new TLegend(0.18,0.32,0.40,0.47);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->AddEntry(gratio,Form("data (%.1f fb^{-1})", lumi/1000.),"PL"); 
   leg->AddEntry(l,"QCD Pythia + CMS Simulation","L");
   leg->AddEntry(htmp2,"JES Uncertainty","F");
   leg->Draw("same");
   pave_fit->Draw("same");
   
   c4->Update();
   
   c4->SaveAs("Plots_1Vtag/RatioDataPYTHIA.png");
   c4->SaveAs("Plots_1Vtag/RatioDataPYTHIA.pdf");

    */

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
   double diquark1 = 1000;
   double diquark2 = 2000;
   double wprime1 = 1000;
   double wprime2 = 2000;


   unsigned int qbin1 = 0;
   unsigned int qbin2 = 2;
   unsigned int sbin1 = 0;
   unsigned int sbin2 = 2;
   
   std::vector<double> v_wprime1, v_wprime1_mjj, v_wprime2, v_wprime2_mjj, v_diquark1, v_diquark1_mjj, v_diquark2, v_diquark2_mjj, v_wprime1_2, v_wprime2_2, v_diquark1_2, v_diquark2_2, v_diquark1_3, v_diquark2_3;

   for(int i=0;i<hDijetMass->GetNbinsX();i++)
    {
     double dm = hDijetMass->GetBinWidth(i+1);
     dm*=10000.;
     double mass = hDijetMass->GetBinCenter(i+1);
     double fitt = fit->Eval(mass,0,0);
     if(mass>0.6*diquark1 && mass<1.26*diquark1){
                v_diquark1.push_back(QstarBinnedProb(mass,diquark1) * diquark_newcut[qbin1] / dm);
		v_diquark1_2.push_back(((QstarBinnedProb(mass,diquark1) * diquark_newcut[qbin1] / dm)+fitt)/fitt);
		v_diquark1_3.push_back(((QstarBinnedProb(mass,diquark1) * diquark_newcut[qbin1] / dm))/fitt);
		v_diquark1_mjj.push_back(mass);

		}

     if(mass>0.6*diquark2 && mass<1.28*diquark2){
		v_diquark2.push_back(QstarBinnedProb(mass,diquark2) * diquark_newcut[qbin2] / dm);
		v_diquark2_2.push_back(((QstarBinnedProb(mass,diquark2) * diquark_newcut[qbin2] / dm)+fitt)/fitt); 
		v_diquark2_3.push_back(((QstarBinnedProb(mass,diquark2) * diquark_newcut[qbin2] / dm))/fitt); 
		v_diquark2_mjj.push_back(mass);
		}

     if(mass>0.6*wprime1 && mass<1.26*wprime1){
 		v_wprime1.push_back(QstarBinnedProb(mass,wprime1) * wprime_newcut[sbin1] / dm);
 		v_wprime1_2.push_back(((QstarBinnedProb(mass,wprime1) * wprime_newcut[sbin1] / dm)+fitt)/fitt);
		v_wprime1_mjj.push_back(mass);
		}

     if(mass>0.6*wprime2 && mass<1.28*wprime2){
		v_wprime2.push_back(QstarBinnedProb(mass,wprime2) * wprime_newcut[sbin2] / dm);
		v_wprime2_2.push_back(((QstarBinnedProb(mass,wprime2) * wprime_newcut[sbin2] / dm)+fitt)/fitt);
		v_wprime2_mjj.push_back(mass);
		}		
	}

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

   TH1F *vFrame = gPad->DrawFrame(890.,0.0000001,3300.0,2.0);
   vFrame->SetTitle("");
   vFrame->SetXTitle("Dijet Mass (GeV)");
   vFrame->GetXaxis()->SetTitleSize(0.06);
   vFrame->SetYTitle("d#sigma/dm (pb/GeV)");

   g->Draw("PZ");

   //gr_diquark1->Draw("csame");
   gr_diquark2->Draw("csame");
   //gr_wprime1->Draw("csame");
   //   gr_wprime2->Draw("csame");

   TPaveText *pt_c5_wprime1 = new TPaveText(0.35,0.7,0.50,0.8,"NDC");
   pt_c5_wprime1->SetTextSize(0.04);
   pt_c5_wprime1->SetFillColor(0);
   pt_c5_wprime1->SetFillStyle(0);
   pt_c5_wprime1->SetBorderSize(0);
   pt_c5_wprime1->SetTextColor(ci_g);
   pt_c5_wprime1->AddText("q*->qZ (1 TeV)");

   TPaveText *pt_c5_wprime2 = new TPaveText(0.65,0.45,0.8,0.55,"NDC");
   pt_c5_wprime2->SetTextSize(0.04);
   pt_c5_wprime2->SetFillColor(0);
   pt_c5_wprime2->SetFillStyle(0);
   pt_c5_wprime2->SetBorderSize(0);
   pt_c5_wprime2->SetTextColor(ci_g);
   pt_c5_wprime2->AddText("q*->qZ (2 TeV)");

   TPaveText *pt_c5_diquark1 = new TPaveText(0.18,0.35,0.33,0.45,"NDC");
   pt_c5_diquark1->SetTextSize(0.04);
   pt_c5_diquark1->SetFillColor(0);
   pt_c5_diquark1->SetFillStyle(0);
   pt_c5_diquark1->SetBorderSize(0);
   pt_c5_diquark1->SetTextColor(2);
   pt_c5_diquark1->AddText("q*->qW (1 TeV)");

   TPaveText *pt_c5_diquark2 = new TPaveText(0.32,0.25,0.47,0.35,"NDC");
   pt_c5_diquark2->SetTextSize(0.04);
   pt_c5_diquark2->SetFillColor(0);
   pt_c5_diquark2->SetFillStyle(0);
   pt_c5_diquark2->SetBorderSize(0);
   pt_c5_diquark2->SetTextColor(2);
   pt_c5_diquark2->AddText("q*->qW (2 TeV)");

   //pt_c5_wprime1->Draw("sames");
   //pt_c5_wprime2->Draw("sames");
   //pt_c5_diquark1->Draw("sames");
     pt_c5_diquark2->Draw("sames");
	
   TLegend *leg2 = new TLegend(0.22,0.78,0.45,0.93);
   leg2->SetTextSize(0.04);
   leg2->SetBorderSize(0);
   leg2->SetFillColor(0);
   leg2->SetFillStyle(0);
   leg2->AddEntry(g,Form("Single W/Z-tag data (%.1f fb^{-1})", lumi),"PL");
   leg2->AddEntry(fit,"Fit","L");
   leg2->AddEntry(gr_diquark1,"q*->qW","L");
   //leg2->AddEntry(gr_wprime1,"q*->qZ","L");
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
				
	if(m<=565 || m>3300){
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
   c5->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithSignal.png");
   c5->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithSignal.pdf");




 //Dijet Mass Cross Section with Fit	
  TCanvas* c02 = new TCanvas("c02","DijetMass Cross Section with Window Fit");
  ((TGraphAsymmErrors*) aRatio->At(0))->SetTitle("");
  ((TGraphAsymmErrors*) aRatio->At(0))->SetLineColor(1);
  ((TGraphAsymmErrors*) aRatio->At(0))->SetFillColor(1);
  ((TGraphAsymmErrors*) aRatio->At(0))->GetXaxis()->SetTitle("Dijet Mass (GeV)");
  ((TGraphAsymmErrors*) aRatio->At(0))->GetYaxis()->SetTitle("Fit/Default fit");
  ((TGraphAsymmErrors*) aRatio->At(0))->GetXaxis()->SetRangeUser(890,3300.0);
  ((TGraphAsymmErrors*) aRatio->At(0))->GetYaxis()->SetRangeUser(0.0,2.0);
  ((TGraphAsymmErrors*) aRatio->At(0))->Draw("AL");

  for (int iWindow = 1; iWindow < 15; iWindow++){

    ((TGraphAsymmErrors*) aRatio->At(iWindow))->SetLineColor(iWindow+1);
    ((TGraphAsymmErrors*) aRatio->At(iWindow))->Draw("SAME");

  }

  gRatio_4par->SetLineWidth(3);
  gRatio_4par->SetLineStyle(1);
  gRatio_4par->SetLineColor(TColor::GetColor("#009900"));


  gRatio_3par->SetLineWidth(3);
  gRatio_3par->SetLineStyle(1);
  gRatio_3par->SetLineColor(kBlue);
  
  gRatio_lowMass->SetLineWidth(3);
  gRatio_lowMass->SetLineStyle(1);
  gRatio_lowMass->SetLineColor(kMagenta);

  gRatio_4par->Draw("SAME");
  gRatio_3par->Draw("SAME"); 
  gRatio_lowMass->Draw("SAME"); 


  hratio->Draw("SAMEP");

  TLegend *legw = new TLegend(0.18,0.78,0.38,0.92);
  legw->SetTextSize(0.03146853);
  legw->SetLineColor(1);
  legw->SetLineStyle(1);
  legw->SetLineWidth(1);
  legw->SetFillColor(0);
  legw->AddEntry(hratio,Form("CMS  (%.3f fb^{-1})", lumi/1000.),"PL");
  legw->AddEntry(((TGraphAsymmErrors*) aRatio->At(0)),"15 fits with window/Default Fit (4 par.)","L");
  legw->AddEntry(gRatio_lowMass,"Default Fit up to 2.0 TeV/Default Fit (4 par.)","L");
  legw->AddEntry(gRatio_4par,"Alternate Fit A (4 Par.)/Default Fit (4 par.)","L");
  legw->AddEntry(gRatio_3par,"Alternate Fit B (3 Par.)/Default Fit (4 par.)","L");
  legw->Draw("same");



  c02->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFitRatio.png");
  c02->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFitRatio.pdf");









 //Dijet Mass Cross Section with Fit pulls
  TCanvas* c100 = new TCanvas("c100","DijetMass Cross Section with Window Fit");
  ((TGraphAsymmErrors*) aPull->At(0))->SetTitle("");
  ((TGraphAsymmErrors*) aPull->At(0))->SetLineColor(1);
  ((TGraphAsymmErrors*) aPull->At(0))->SetFillColor(1);
  ((TGraphAsymmErrors*) aPull->At(0))->GetXaxis()->SetTitle("Dijet Mass (GeV)");
  ((TGraphAsymmErrors*) aPull->At(0))->GetYaxis()->SetTitle("(Fit-Default fit)/(Default fit variation)");
  ((TGraphAsymmErrors*) aPull->At(0))->GetXaxis()->SetRangeUser(890,3300.0);
  ((TGraphAsymmErrors*) aPull->At(0))->GetYaxis()->SetRangeUser(-6.0,6.0);
  ((TGraphAsymmErrors*) aPull->At(0))->Draw("AL");

  for (int iWindow = 1; iWindow < 15; iWindow++){

    ((TGraphAsymmErrors*) aPull->At(iWindow))->SetLineColor(iWindow+1);
    ((TGraphAsymmErrors*) aPull->At(iWindow))->Draw("SAME");

  }

  gPull_4par->SetLineWidth(3);
  gPull_4par->SetLineStyle(1);
  gPull_4par->SetLineColor(TColor::GetColor("#009900"));

  gPull_3par->SetLineWidth(3);
  gPull_3par->SetLineStyle(1);
  gPull_3par->SetLineColor(kBlue);
  
  gPull_lowMass->SetLineWidth(3);
  gPull_lowMass->SetLineStyle(1);
  gPull_lowMass->SetLineColor(kMagenta);

  gPull_4par->Draw("SAME");
  gPull_3par->Draw("SAME"); 
  gPull_lowMass->Draw("SAME"); 

  TLegend *legw2 = new TLegend(0.18,0.78,0.38,0.92);
  legw2->SetTextSize(0.03146853);
  legw2->SetLineColor(1);
  legw2->SetLineStyle(1);
  legw2->SetLineWidth(1);
  legw2->SetFillColor(0);
  legw2->AddEntry(((TGraphAsymmErrors*) aPull->At(0)),"15 fits with window - Default Fit (4 par.)","L");
  legw2->AddEntry(gPull_lowMass,"Default Fit up to 2.0 TeV - Default Fit (4 par.)","L");
  legw2->AddEntry(gPull_4par,"Alternate Fit A (4 Par.) - Default Fit (4 par.)","L");
  legw2->AddEntry(gPull_3par,"Alternate Fit B (3 Par.) - Default Fit (4 par.)","L");
  legw2->Draw("same");



  c100->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFitPull.png");
  c100->SaveAs("Plots_1Vtag/DijetMassCrossSectionWithWindowFitPull.pdf");






   TCanvas* c6 = new TCanvas("c6","(Data-Fit)/Fit");
   hDiff->SetTitle("");
   hDiff->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   hDiff->GetYaxis()->SetTitle("(Data-Fit)/Fit");
   hDiff->GetXaxis()->SetRangeUser(890.,3300.);
   hDiff->GetYaxis()->SetRangeUser(-1.5,3.);
   hDiff->Draw("APZ");
   //gr_diquark1_3->Draw("csame");
   gr_diquark2_3->Draw("csame");
   l = new TLine(890, 0.0, 3300, 0.0);
   l->SetLineStyle(2);
   l->Draw("same");
   pave->Draw("same");

   TPaveText *pt_c6_diquark1 = new TPaveText(0.20,0.3,0.35,0.4,"NDC");
   pt_c6_diquark1->SetTextSize(0.04);
   pt_c6_diquark1->SetFillColor(0);
   pt_c6_diquark1->SetTextColor(2);
   pt_c6_diquark1->SetFillStyle(0);
   pt_c6_diquark1->SetBorderSize(0);
   pt_c6_diquark1->AddText("q* (1 TeV)");

   TPaveText *pt_c6_diquark2 = new TPaveText(0.40,0.3,0.55,0.4,"NDC");
   pt_c6_diquark2->SetTextSize(0.04);
   pt_c6_diquark2->SetFillColor(0);
   pt_c6_diquark2->SetTextColor(2);
   pt_c6_diquark2->SetFillStyle(0);
   pt_c6_diquark2->SetBorderSize(0);
   pt_c6_diquark2->AddText("q* (2 TeV)");

   //pt_c6_diquark1->Draw("sames");
   pt_c6_diquark2->Draw("sames");

   c6->SaveAs("Plots_1Vtag/DataMinusFitDividedByFit.png");
   c6->SaveAs("Plots_1Vtag/DataMinusFitDividedByFit.pdf");


   TCanvas* c7 = new TCanvas("c7","(Data-Fit)/Error");
   hPulls->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   hPulls->GetYaxis()->SetTitle("(Data-Fit)/Error");
   hPulls->GetXaxis()->SetRangeUser(890.,3300.);
   hPulls->GetYaxis()->SetRangeUser(-3.9,3.9);
   hPulls->SetLineWidth(1);
   hPulls->Draw("ep");
   l->Draw("same");  
	
   c7->SaveAs("Plots_1Vtag/DataMinusFitDividedByError.png");
   c7->SaveAs("Plots_1Vtag/DataMinusFitDividedByError.pdf");

   TCanvas* c8 = new TCanvas("c8","(Data-Fit)/Fit for the All Fits");
   hDiff->Draw("APZ");
   hDiff_4par->Draw("pzsame");
   hDiff_3par->Draw("pzsame");
   l->Draw("same"); 
   TLegend *leg = new TLegend(0.4,0.6,0.85,0.92);
   leg->SetFillColor(0);
   leg->SetTextSize(0.04);
   leg->AddEntry(hPulls,"Default Fit (4 Par.)","PL");
   leg->AddEntry(hPulls_4par,"Alternate Fit A (4 Par.)","PL");
   leg->AddEntry(hPulls_3par,"Alternate Fit B (3 Par.)","PL");
   leg->Draw("same");
   c8->SaveAs("Plots_1Vtag/DataMinusFitDividedByFitForAllFits.png");
   c8->SaveAs("Plots_1Vtag/DataMinusFitDividedByFitForAllFits.pdf");


   TCanvas* c9 = new TCanvas("c9","(Data-Fit)/Error the All Fits");
   hPulls->Draw("ep");
   hPulls_4par->Draw("epsame");
   hPulls_3par->Draw("epsame");
   l->Draw("same");	
   TLegend *leg = new TLegend(0.65,0.7,0.92,0.92);
   leg->SetFillColor(0);
   leg->SetTextSize(0.04);
   leg->AddEntry(hPulls,"Default Fit (4 Par.)","PL");
   leg->AddEntry(hPulls_4par,"Alternate Fit A (4 Par.)","PL");
   leg->AddEntry(hPulls_3par,"Alternate Fit B (3 Par.)","PL");
   leg->Draw("same");

   c9->SaveAs("Plots_1Vtag/DataMinusFitDividedByErrorForAllFits.png");
   c9->SaveAs("Plots_1Vtag/DataMinusFitDividedByErrorForAllFits.pdf");	


   TCanvas* c10 = new TCanvas("c10","Data/Fit with the All signal");
   c10->SetLogy(1);
   hratio->SetTitle("");
   hratio->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   hratio->GetXaxis()->SetTitleSize(0.06);
   hratio->GetYaxis()->SetTitle("Data/Fit");
   hratio->SetMarkerStyle(20);
   hratio->GetXaxis()->SetRangeUser(890.,3300.);
   hratio->GetYaxis()->SetRangeUser(0.5,6.);
   hratio->Draw("APZ");
   l->Draw("same");
	
   //gr_diquark1_2->Draw("csame");
   gr_diquark2_2->Draw("csame");
   

   TPaveText *pt_c10_diquark1 = new TPaveText(0.2,0.5,0.35,0.6, "NDC");
   pt_c10_diquark1->SetTextSize(0.04);
   pt_c10_diquark1->SetFillColor(0);
   pt_c10_diquark1->SetTextColor(2);
   pt_c10_diquark1->SetFillStyle(0);
   pt_c10_diquark1->SetBorderSize(0);
   pt_c10_diquark1->AddText("q* (1 TeV)");

   TPaveText *pt_c10_diquark2 = new TPaveText(0.5,0.5,0.65,0.6,"NDC");
   pt_c10_diquark2->SetTextSize(0.04);
   pt_c10_diquark2->SetFillColor(0);
   pt_c10_diquark2->SetTextColor(2);
   pt_c10_diquark2->SetFillStyle(0);
   pt_c10_diquark2->SetBorderSize(0);
   pt_c10_diquark2->AddText("q* (2 TeV)");

   //pt_c10_diquark1->Draw("sames");
   pt_c10_diquark2->Draw("sames");

   pave->Draw("smaes");

   c10->SaveAs("Plots_1Vtag/DataMinusFitWithAllSignal.png");
   c10->SaveAs("Plots_1Vtag/DataMinusFitWithAllSignal.pdf");


   TCanvas* c11 = new TCanvas("c11","default fit and pull");

   c11->Divide(1,2,0,0,0);
   c11->cd(1);
   
   p11_1 = (TPad*)c11->GetPad(1);
   p11_1->SetPad(0.01,0.22,0.99,0.98);
   p11_1->SetLogy();
   p11_1->SetRightMargin(0.05);
   p11_1->SetTopMargin(0.05);
   
   TH1F *vFrame = p11_1->DrawFrame(890.,0.0000001,3300.0,2.0);
   vFrame->SetTitle("");
   vFrame->SetXTitle("Dijet Mass (GeV)");
   vFrame->GetXaxis()->SetTitleSize(0.06);
   vFrame->SetYTitle("d#sigma/dm (pb/GeV)");

   //h->SetTitle("");

   //h->Draw("SAMEC");

   //   h->GetXaxis()->SetRangeUser(890.,3300.);
   //   h->GetYaxis()->SetRangeUser(0.0001,50.);
   // h->GetXaxis()->SetTitle("Dijet Mass (GeV)");
   //  h->GetXaxis()->SetTitleSize(0.06);
   //  h->GetYaxis()->SetTitle("d#sigma/dm (pb/GeV)");


   //f_qcd->Draw("same");
   g->Draw("sameP");
   TLegend *leg = new TLegend(0.28,0.70,0.49,0.93);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->AddEntry(g,Form("Single W/Z-tag data (%.1f fb^{-1})", lumi/1000.),"PL"); 
   leg->AddEntry(fit,"Fit","L");
   //leg->AddEntry(f_qcd,"QCD Pythia","L");
   //leg->AddEntry(htmp,"JES Uncertainty","F");
   leg->AddEntry(gr_diquark1,"q*->qW","L");
   //leg->AddEntry(gr_wprime1,"q*->qZ","L");
   leg->Draw("same");

   pave_fit->Draw("same");

   //gr_diquark1->Draw("csame");
   gr_diquark2->Draw("csame");
   //gr_wprime1->Draw("csame");
   //   gr_wprime2->Draw("csame");

   TPaveText *pt_c11_wprime1 = new TPaveText(0.15,0.25,0.30,0.4,"NDC");
   pt_c11_wprime1->SetTextSize(0.04);
   pt_c11_wprime1->SetFillColor(0);
   pt_c11_wprime1->SetFillStyle(0);
   pt_c11_wprime1->SetBorderSize(0);
   pt_c11_wprime1->SetTextColor(TColor::GetColor("#006600"));
   pt_c11_wprime1->AddText("q*->qZ (1 TeV)");

   TPaveText *pt_c11_wprime2 = new TPaveText(0.60,0.45,0.75,0.55,"NDC");
   pt_c11_wprime2->SetTextSize(0.04);
   pt_c11_wprime2->SetFillColor(0);
   pt_c11_wprime2->SetFillStyle(0);
   pt_c11_wprime2->SetBorderSize(0);
   pt_c11_wprime2->SetTextColor(TColor::GetColor("#006600"));
   pt_c11_wprime2->AddText("q*->qZ (2 TeV)");

   TPaveText *pt_c11_diquark1 = new TPaveText(0.26,0.40,0.45,0.53,"NDC");
   pt_c11_diquark1->SetTextSize(0.04);
   pt_c11_diquark1->SetFillColor(0);
   pt_c11_diquark1->SetFillStyle(0);
   pt_c11_diquark1->SetBorderSize(0);
   pt_c11_diquark1->SetTextColor(2);
   pt_c11_diquark1->AddText("q*->qW (1 TeV)");

   TPaveText *pt_c11_diquark2 = new TPaveText(0.40,0.15,0.55,0.20,"NDC");
   pt_c11_diquark2->SetTextSize(0.04);
   pt_c11_diquark2->SetFillColor(0);
   pt_c11_diquark2->SetFillStyle(0);
   pt_c11_diquark2->SetBorderSize(0);
   pt_c11_diquark2->SetTextColor(2);
   pt_c11_diquark2->AddText("q*->qW (2 TeV)");

   //pt_c11_wprime1->Draw("sames");
   //pt_c11_wprime2->Draw("sames");
   //pt_c11_diquark1->Draw("sames");
   pt_c11_diquark2->Draw("sames");
   
   c11->cd(2);
   p11_2 = (TPad*)c11->GetPad(2);
   p11_2->SetPad(0.01,0.02,0.99,0.24);
   p11_2->SetBottomMargin(0.35);
   p11_2->SetRightMargin(0.05);
   p11_2->SetGridx();
   c11_2->SetTickx(50);


   TH1F *vFrame2 = p11_2->DrawFrame(890., -3.31, 3300.0, 3.31);
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

   TLine *line = new TLine(890.,0,3300,0);
   line->Draw("");
   
   c11->SaveAs("Plots_1Vtag/DefaultFitAndPull.png");
   c11->SaveAs("Plots_1Vtag/DefaultFitAndPull.pdf");
   //c11->SaveAs("Plots_1Vtag/DefaultFitAndPullNoStat.png");
   //c11->SaveAs("Plots_1Vtag/DefaultFitAndPullNoStat.pdf");

}
