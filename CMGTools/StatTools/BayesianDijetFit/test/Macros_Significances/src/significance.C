
void significance()
{

// take parameters

int savefile=0;
int display=0;
int display1=0;
int display2=0;
int display3=0;
int pull1=0;
int pull2=0;
int pull3=0;
int sig1=1;
int sig2=0;
int sig3=1;
int exec1=1;
int exec2=0;
int exec3=1;


TFile *file0=TFile::Open("histograms_data_190456_195947_deltaeta_mass_ak5.root");
TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin10GeV;1");

 gStyle->SetOptFit(111); 

  if(display==1)
  {
  TCanvas *Canvas0 = new TCanvas("Canvas0","Canvas0",11,51,700,500);
  h_DEta_Mass->SetTitle("#Delta#eta VS Invariant Mass for data");
  h_DEta_Mass->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  h_DEta_Mass->GetXaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetXaxis()->SetTitleOffset(1.4);
  h_DEta_Mass->GetYaxis()->SetTitle("#Delta#eta");
  h_DEta_Mass->GetYaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetYaxis()->SetTitleOffset(1.6);
  h_DEta_Mass->GetZaxis()->SetTitle("number of events");
  h_DEta_Mass->GetZaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetZaxis()->SetTitleOffset(1.4);
  TLegend* leg = new TLegend(0.3, 0.25, .4, .3);
  }

  TCanvas *Canvas = new TCanvas("Canvas","Canvas",11,51,700,500);

  TH1D* hist_mass = new TH1D("hist_mass","M: Invariant Mass with 0<#Delta#eta<1.3",800,0,8000);
  hist_mass->Sumw2();
  if(display==1)
  {
  TCanvas *Canvas_0 = new TCanvas("Canvas_0","Canvas_0",11,51,700,500);
  hist_mass->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass->GetXaxis()->SetTitleOffset(1.1);
  hist_mass->GetYaxis()->SetTitle("number of events");
  hist_mass->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg0 = new TLegend(0.4, 0.35, .65, .5);
  }
  TF1 *f0= new TF1("func0","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",0,8000);
  f0->SetParameters(2.6,9.1,5.1);

  TH1D* hist_mass1 = new TH1D("hist_mass1","M1: Invariant mass with 0<#Delta#eta<0.5",800,0,8000);
  hist_mass1->Sumw2();
  if(display==1)
  {
  TCanvas *Canvas_1 = new TCanvas("Canvas_1","Canvas_1",11,51,700,500);
  hist_mass1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass1->GetXaxis()->SetTitleOffset(1.1);
  hist_mass1->GetYaxis()->SetTitle("number of events");
  hist_mass1->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg1 = new TLegend(0.4, 0.4, .6, .45);
  }
  TF1 *f1= new TF1("func1","[0]+[1]*x");
  f1->SetParameters(0.5,0.);

  TH1D* hist_mass2 = new TH1D("hist_mass2","M2: Invariant mass with 0.5<#Delta#eta<1",800,0,8000);
  hist_mass2->Sumw2();
  if(display==1)
  {
  TCanvas *Canvas_2 = new TCanvas("Canvas_2","Canvas_2",11,51,700,500);
  hist_mass2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass2->GetXaxis()->SetTitleOffset(1.1);
  hist_mass2->GetYaxis()->SetTitle("number of events");
  hist_mass2->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg2 = new TLegend(0.4, 0.4, .6, .45);
  }
  TF1 *f2= new TF1("func2","[0]+[1]*x");
  f2->SetParameters(0.5,0.);

  TH1D* hist_mass3 = new TH1D("hist_mass3","M3: Invariant mass with 1<#Delta#eta<1.3",800,0,8000);
  hist_mass3->Sumw2();
  if(display==1)
  {
  TCanvas *Canvas_3 = new TCanvas("Canvas_3","Canvas_3",11,51,700,500);
  hist_mass3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass3->GetXaxis()->SetTitleOffset(1.1);
  hist_mass3->GetYaxis()->SetTitle("number of events");
  hist_mass3->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg3 = new TLegend(0.4, 0.4, .6, .45);
  }
  TF1 *f3= new TF1("func3","[0]+[1]*x");
  f3->SetParameters(0.5,0.);


hist_mass=h_DEta_Mass->ProjectionX("hist_mass",0,13,"e");
hist_mass1=h_DEta_Mass->ProjectionX("hist_mass1",0,5,"e");
hist_mass2=h_DEta_Mass->ProjectionX("hist_mass2",6,10,"e");
hist_mass3=h_DEta_Mass->ProjectionX("hist_mass3",11,13,"e");


Double_t Ntot=hist_mass->GetEntries();
Double_t N1=hist_mass1->GetEntries();
Double_t N2=hist_mass2->GetEntries();
Double_t N3=hist_mass3->GetEntries();

cout<<"comparing the number of events"<<endl;
cout<<"for 0<DeltaEta<0.5: "<<N1/Ntot*100<<" %"<<endl;
cout<<"for 0.5<DeltaEta<1: "<<N2/Ntot*100<<" %"<<endl;
cout<<"for 1<DeltaEta<1.3: "<<N3/Ntot*100<<" %"<<endl;
cout<<"sum="<<N1/Ntot*100+N2/Ntot*100+N3/Ntot*100<<endl;



  TH1D*  hist_mass1ratio = (TH1D*) hist_mass1->Clone(" hist_mass1ratio");
  TH1D*  hist_mass2ratio = (TH1D*) hist_mass2->Clone(" hist_mass2ratio");
  TH1D*  hist_mass3ratio = (TH1D*) hist_mass3->Clone(" hist_mass3ratio");

if(display==1)
{
  TCanvas *Canvas_4 = new TCanvas("Canvas_4","Canvas_4",11,51,700,500);
  hist_mass1ratio->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass1ratio->GetXaxis()->SetTitleOffset(1.1);
  hist_mass1ratio->GetYaxis()->SetTitle("");
  hist_mass1ratio->SetTitle("M1/M for data");
  TLegend* leg4 = new TLegend(0.5, 0.2, .9, .3);
  TCanvas *Canvas_5 = new TCanvas("Canvas_5","Canvas_5",11,51,700,500);
  hist_mass2ratio->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass2ratio->GetXaxis()->SetTitleOffset(1.1);
  hist_mass2ratio->GetYaxis()->SetTitle("");
  hist_mass2ratio->SetTitle("M2/M for data");
  TLegend* leg5 = new TLegend(0.5, 0.2, .9, .3);
  TCanvas *Canvas_6 = new TCanvas("Canvas_6","Canvas_6",11,51,700,500);
  hist_mass3ratio->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass3ratio->GetXaxis()->SetTitleOffset(1.1);
  hist_mass3ratio->GetYaxis()->SetTitle("");
  hist_mass3ratio->SetTitle("M3/M for data");
  TLegend* leg6 = new TLegend(0.5, 0.2, .9, .3);
}

if(display==1)
{
  TH1D*  hist_massdisplay = (TH1D*) hist_mass->Clone(" hist_massdisplay");
  TH1D*  hist_mass1display = (TH1D*) hist_mass1->Clone(" hist_mass1display");
  TH1D*  hist_mass2display = (TH1D*) hist_mass2->Clone(" hist_mass2display");
  TH1D*  hist_mass3display = (TH1D*) hist_mass3->Clone(" hist_mass3display");
 Canvas0->cd();
 leg->AddEntry(h_DEta_Mass," data","l");
 h_DEta_Mass->Draw("surf2");
 leg->Draw();
 Canvas_0->cd();
 hist_massdisplay->Fit("func0","","",890,5000);
 hist_massdisplay->SetLineColor(kBlue-2);
 leg0->AddEntry((TObject*)0,"","");
 leg0->AddEntry(hist_massdisplay,"data   0<#Delta#eta<1.3","l");
 leg0->AddEntry((TObject*)0,"","");
 leg0->AddEntry(f0,"fit: P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 leg0->AddEntry((TObject*)0,"","");
 hist_massdisplay->Draw();
 leg0->Draw();
 Canvas_1->cd();
 hist_mass1display->SetLineColor(kBlue-2);
 leg1->AddEntry(hist_mass1display,"data   0<#Delta#eta<0.5","l");
 hist_mass1display->Draw();
 leg1->Draw();
 Canvas_2->cd();
 hist_mass2display->SetLineColor(kBlue-2);
 leg2->AddEntry(hist_mass2display,"data   0.5<#Delta#eta<1","l"); 
 hist_mass2display->Draw();
 leg2->Draw();
 Canvas_3->cd();
 hist_mass3display->SetLineColor(kBlue-2);
 leg3->AddEntry(hist_mass3display,"data   1<#Delta#eta<1.3","l");
 hist_mass3display->Draw();
 leg3->Draw();
 Canvas_4->cd();
 hist_mass1ratio->Divide(hist_mass1ratio,hist_mass,1,1,"B");
 hist_mass1ratio->Fit("func1","","",890,5000);
 hist_mass1ratio->SetLineColor(kBlue-2);
 leg4->AddEntry(hist_mass1ratio,"mass ( 0<#Delta#eta<0.5 ) / mass ( 0<#Delta#eta<1.3 )","l");
 leg4->AddEntry((TObject*)0,"","");
 leg4->AddEntry(f1,"fit: p0+p1#timesx","l");
 hist_mass1ratio->Draw();
 leg4->Draw();
 Canvas_5->cd();
 hist_mass2ratio->Divide(hist_mass2ratio,hist_mass,1,1,"B");
 hist_mass2ratio->Fit("func2","","",890,5000);
 hist_mass2ratio->SetLineColor(kBlue-2);
 leg5->AddEntry(hist_mass2ratio,"mass ( 0.5<#Delta#eta<1 ) / mass ( 0<#Delta#eta<1.3 )","l");
 leg5->AddEntry((TObject*)0,"","");
 leg5->AddEntry(f2,"fit: p0+p1#timesx","l");
 hist_mass2ratio->Draw();
 leg5->Draw();
 Canvas_6->cd();
 hist_mass3ratio->Divide(hist_mass3ratio,hist_mass,1,1,"B");
 hist_mass3ratio->Fit("func3","","",890,5000); 
 hist_mass3ratio->SetLineColor(kBlue-2);
 leg6->AddEntry(hist_mass3ratio,"mass ( 1<#Delta#eta<1.3 ) / mass ( 0<#Delta#eta<1.3 )","l");
 leg6->AddEntry((TObject*)0,"","");
 leg6->AddEntry(f3,"fit: p0+p1#timesx","l");
 hist_mass3ratio->Draw();
 leg6->Draw();
}
else
{
 Canvas->cd();
 hist_mass->Fit("func0","","",890,5000);
 hist_mass1ratio->Divide(hist_mass1,hist_mass,1,1,"B");
 hist_mass1ratio->Fit("func1","","",890,5000);
 hist_mass2ratio->Divide(hist_mass2,hist_mass,1,1,"B");
 hist_mass2ratio->Fit("func2","","",890,5000);
 hist_mass3ratio->Divide(hist_mass3,hist_mass,1,1,"B");
 hist_mass3ratio->Fit("func3","","",890,5000);
}


double P0=func0->GetParameter(0);
double P1=func0->GetParameter(1);
double P2=func0->GetParameter(2);
double P3=func1->GetParameter(0);
double P4=func1->GetParameter(1);
double P7=func3->GetParameter(0);
double P8=func3->GetParameter(1);
double P5=1-P3-P7;
double P6=-P4-P8;

  TF1 *background1= new TF1("background1","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))*([3]+[4]*x)",890,5000);
  TF1 *background2= new TF1("background2","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))*([3]+[4]*x)",890,5000);
  TF1 *background3= new TF1("background3","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))*([3]+[4]*x)",890,5000);

background1->SetParameters(P0,P1,P2,P3,P4);
background2->SetParameters(P0,P1,P2,P5,P6);
background3->SetParameters(P0,P1,P2,P7,P8);

/*
background1->SetParameters(P0,0,0,1,0);
background2->SetParameters(P0,0,0,1,0);
background3->SetParameters(P0,0,0,1,0);
*/
cout<<"P0: "<<P0<<endl;
cout<<"P1: "<<P1<<endl;
cout<<"P2: "<<P2<<endl;
cout<<"P3: "<<P3<<endl;
cout<<"P4: "<<P4<<endl;
cout<<"P5: "<<P5<<endl;
cout<<"P6: "<<P6<<endl;
cout<<"P7: "<<P7<<endl;
cout<<"P8: "<<P8<<endl;

  TF1 *background0fit= new TF1("background0fit","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))+0*[3]",890,5000);
  TF1 *background1fit= new TF1("background1fit","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))*([3]+[4]*x)+0*[5]",890,5000);
  TF1 *background2fit= new TF1("background2fit","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))*([3]+[4]*x)+0*[5]",890,5000);
  TF1 *background3fit= new TF1("background3fit","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))*([3]+[4]*x)+0*[5]",890,5000);


background0fit->FixParameter(0,P0);
background0fit->FixParameter(1,P1);
background0fit->FixParameter(2,P2);
background1fit->FixParameter(0,P0);
background1fit->FixParameter(1,P1);
background1fit->FixParameter(2,P2);
background1fit->FixParameter(3,P3);
background1fit->FixParameter(4,P4);
background2fit->FixParameter(0,P0);
background2fit->FixParameter(1,P1);
background2fit->FixParameter(2,P2);
background2fit->FixParameter(3,P5);
background2fit->FixParameter(4,P6);
background3fit->FixParameter(0,P0);
background3fit->FixParameter(1,P1);
background3fit->FixParameter(2,P2);
background3fit->FixParameter(3,P7);
background3fit->FixParameter(4,P8);

double chi2_0;
double chi2_1;
double chi2_2;
double chi2_3;
double NDF_0;
double NDF_1;
double NDF_2;
double NDF_3;

Canvas->cd();
hist_mass->Fit("background0fit","L","",890,5000);
hist_mass1->Fit("background1fit","L","",890,5000);
hist_mass2->Fit("background2fit","L","",890,5000);
hist_mass3->Fit("background3fit","L","",890,5000);

chi2_0=background0fit->GetChisquare();
chi2_1=background1fit->GetChisquare();
chi2_2=background2fit->GetChisquare();
chi2_3=background3fit->GetChisquare();
NDF_0=background0fit->GetNDF();
NDF_1=background1fit->GetNDF();
NDF_2=background2fit->GetNDF();
NDF_3=background3fit->GetNDF();


cout<<"chi2_0: "<<chi2_0<<"/"<<NDF_0<<endl;
cout<<"chi2_1: "<<chi2_1<<"/"<<NDF_1<<endl;
cout<<"chi2_2: "<<chi2_2<<"/"<<NDF_2<<endl;
cout<<"chi2_3: "<<chi2_3<<"/"<<NDF_3<<endl;


//*****************************************************************************

  if(savefile==1) TFile *outputfile = new TFile("significance.root","recreate");

//generate samples

  int f=5;
  int iend=10;
  int Nbins=371;
  double xmin;
  double xmax;
  xmin=890.;
  xmax=4600.;
  double binwidth=(xmax-xmin)/Nbins;


  double nevt=1093260;
  double nevt1=nevt*N1/Ntot;
  double nevt2=nevt*N2/Ntot;
  double nevt3=nevt*N3/Ntot;
cout<<"nevt: "<<nevt<<endl
    <<"nevt1: "<<nevt1<<endl
    <<"nevt2: "<<nevt2<<endl
    <<"nevt3: "<<nevt3<<endl;


  double r1=0.436786;
  double r2=0.375497;
  double r3=0.187717;
  double gint=90.*f;
  double gint1=gint*r1;
  double gint2=gint*r2;
  double gint3=gint*r3;
cout<<"gint: "<<gint<<endl
    <<"gint1: "<<gint1<<endl
    <<"gint2: "<<gint2<<endl
    <<"gint3: "<<gint3<<endl;


double gaussnorm=TMath::Sqrt(2*TMath::Pi())/binwidth;
double M1chi2_B;
double M1chi2_SpB;
double M1sig;
double M1significance;
double M1lnLB;
double M1lnLSpB;
double M1scalefactor;
double M1Nint;
double M1errorNint;
double M1pullpar0;
double M1pullpar1;
double M1pullpar2;
double M1pullpar3;
double M2chi2_B;
double M2chi2_B1;
double M2chi2_B2;
double M2chi2_B3;
double M2chi2_SpB;
double M2chi2_SpB1;
double M2chi2_SpB2;
double M2chi2_SpB3;
double M2sig;
double M2sig1;
double M2sig2;
double M2sig3;
double M2significance;
double M2significance1;
double M2significance2;
double M2significance3;
double M2lnLB1;
double M2lnLSpB1;
double M2lnLB2;
double M2lnLSpB2;
double M2lnLB3;
double M2lnLSpB3;
double M2lnLB;
double M2lnLSpB;
double M2scalefactor1;
double M2scalefactor2;
double M2scalefactor3;
double M2Nint1;
double M2Nint2;
double M2Nint3;
double M2errorNint1;
double M2errorNint2;
double M2errorNint3;
double M2pull1par0;
double M2pull1par1;
double M2pull1par2;
double M2pull1par3;
double M2pull2par0;
double M2pull2par1;
double M2pull2par2;
double M2pull2par3;
double M2pull3par0;
double M2pull3par1;
double M2pull3par2;
double M2pull3par3;
double M3chi2_B;
double M3chi2_B1;
double M3chi2_B2;
double M3chi2_B3;
double M3chi2_SpB;
double M3chi2_SpB1;
double M3chi2_SpB2;
double M3chi2_SpB3;
double M3sig;
double M3sig1;
double M3sig2;
double M3sig3;
double M3significance;
double M3significance1;
double M3significance2;
double M3significance3;
double M3lnLB1;
double M3lnLSpB1;
double M3lnLB2;
double M3lnLSpB2;
double M3lnLB3;
double M3lnLSpB3;
double M3lnLB;
double M3lnLSpB;
double M3scalefactor;
double M3Nint;
double M3errorNint;
double M3pull;
double M3norm0;
double M3norm1;
double M3norm2;
double M3norm3;


double mean=2000.;
double sigma=2000./20.;

  TF1 * gauss1 = new TF1 ("gauss1", "[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * gauss2 = new TF1 ("gauss2", "[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * gauss3 = new TF1 ("gauss3", "[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);

gauss1->SetParameters(1,mean,sigma);
gauss1->FixParameter(1,mean);
gauss1->FixParameter(2,sigma);
gauss2->SetParameters(1,mean,sigma);
gauss2->FixParameter(1,mean);
gauss2->FixParameter(2,sigma);
gauss3->SetParameters(1,mean,sigma);
gauss3->FixParameter(1,mean);
gauss3->FixParameter(2,sigma);


// ************ method 1

if(exec1==1)
{
if(display1==1)
{
  TCanvas *M1Canvas_1 = new TCanvas("M1Canvas_1","M1Canvas_1",11,51,700,500);
  TCanvas *M1Canvas_1bis = new TCanvas("M1Canvas_1bis","M1Canvas_1bis",11,51,700,500);
}

  TF1 * M1signalbackground_fit = new TF1 ("M1signalbackground_fit","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M1signal_fit= new TF1 ("M1signal_fit","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M1background_fit = new TF1 ("M1background_fit","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M1fit_background = new TF1 ("M1fit_background","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

M1signalbackground_fit->SetParameters(2.6,9,5,1,mean,sigma);
M1signalbackground_fit->FixParameter(4,mean);
M1signalbackground_fit->FixParameter(5,sigma);


  TH1D * M1pullpar0_Hist = new TH1D ("M1pullpar0_Hist", "Histogram of pull", 50, -10, 10);
  M1pullpar0_Hist->Sumw2();
  M1pullpar0_Hist->GetXaxis()->SetTitle("pull");
  M1pullpar0_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M1pullpar1_Hist = new TH1D ("M1pullpar1_Hist", "Histogram of pull", 50, -10, 10);
  M1pullpar1_Hist->Sumw2();
  M1pullpar1_Hist->GetXaxis()->SetTitle("pull");
  M1pullpar1_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M1pullpar2_Hist = new TH1D ("M1pullpar2_Hist", "Histogram of pull", 50, -10, 10);
  M1pullpar2_Hist->Sumw2();
  M1pullpar2_Hist->GetXaxis()->SetTitle("pull");
  M1pullpar2_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M1pullpar3_Hist = new TH1D ("M1pullpar3_Hist", "Histogram of pull", 50, -10, 10);
  M1pullpar3_Hist->Sumw2();
  M1pullpar3_Hist->GetXaxis()->SetTitle("pull");
  M1pullpar3_Hist->GetYaxis()->SetTitle("number of events");

if(pull1==1)
{
  TLegend* M1leg2 = new TLegend(0.1, 0.7, .6, .9);
  TCanvas *M1Canvas_2 = new TCanvas("M1Canvas_2","M1Canvas_2",11,51,700,500);
}

  TH1D * M1Sig_Hist_chi2 = new TH1D ("M1Sig_Hist_chi2", "histogram for the significance", 100, 0, 10);
  M1Sig_Hist_chi2->Sumw2();
  M1Sig_Hist_chi2->GetXaxis()->SetTitle("Significance");
  M1Sig_Hist_chi2->GetYaxis()->SetTitle("Number of events");
  TLegend* M1leg3 = new TLegend(0.15, 0.7, .75, .85);
  TCanvas *M1Canvas_3 = new TCanvas("M1Canvas_3","M1Canvas_3",11,51,700,500);


  TH1D * M1Sig_Hist_likelihood = new TH1D("M1Sig_Hist_likelihood","histogram for the significance",100,0,10);
  M1Sig_Hist_likelihood->Sumw2();
  M1Sig_Hist_likelihood->GetXaxis()->SetTitle("Significance");
  M1Sig_Hist_likelihood->GetYaxis()->SetTitle("Number of events");
  TLegend* M1leg4 = new TLegend(0.15, 0.7, .75, .85);
  TCanvas *M1Canvas_4 = new TCanvas("M1Canvas_4","M1Canvas_4",11,51,700,500);
}



//************ method 2

if(exec2==1)
{
  TF1 * M2signalbackground_fit1 = new TF1 ("M2signalbackground_fit1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M2signal_fit1= new TF1 ("M2signal_fit1","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M2background_fit1 = new TF1 ("M2background_fit1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M2fit_background1 = new TF1 ("M2fit_background1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

  TF1 * M2signalbackground_fit2 = new TF1 ("M2signalbackground_fit2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M2signal_fit2= new TF1 ("M2signal_fit2","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M2background_fit2 = new TF1 ("M2background_fit2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M2fit_background2 = new TF1 ("M2fit_background2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

  TF1 * M2signalbackground_fit3 = new TF1 ("M2signalbackground_fit3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M2signal_fit3= new TF1 ("M2signal_fit3","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M2background_fit3 = new TF1 ("M2background_fit3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M2fit_background3 = new TF1 ("M2fit_background3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

M2signalbackground_fit1->SetParameters(2.6,9,5,1,mean,sigma);
M2signalbackground_fit1->FixParameter(4,mean);
M2signalbackground_fit1->FixParameter(5,sigma);
M2signalbackground_fit2->SetParameters(2.6,9,5,1,mean,sigma);
M2signalbackground_fit2->FixParameter(4,mean);
M2signalbackground_fit2->FixParameter(5,sigma);
M2signalbackground_fit3->SetParameters(2.6,9,5,1,mean,sigma);
M2signalbackground_fit3->FixParameter(4,mean);
M2signalbackground_fit3->FixParameter(5,sigma);


if(display2==1)
{
  TCanvas *M2Canvas_1_1 = new TCanvas("M2Canvas_1_1","M2Canvas_1_1",11,51,700,500);
  TCanvas *M2Canvas_1_1bis = new TCanvas("M2Canvas_1_1bis","M2Canvas_1_1bis",11,51,700,500);
  TCanvas *M2Canvas_1_2 = new TCanvas("M2Canvas_1_2","M2Canvas_1_2",11,51,700,500);
  TCanvas *M2Canvas_1_2bis = new TCanvas("M2Canvas_1_2bis","M2Canvas_1_2bis",11,51,700,500);
  TCanvas *M2Canvas_1_3 = new TCanvas("M2Canvas_1_3","M2Canvas_1_3",11,51,700,500);
  TCanvas *M2Canvas_1_3bis = new TCanvas("M2Canvas_1_3bis","M2Canvas_1_3bis",11,51,700,500);
}

  TH1D * M2pull1par0_Hist = new TH1D ("M2pull1par0_Hist", "Histogram of pull", 50, -10, 10);
  M2pull1par0_Hist->Sumw2();
  M2pull1par0_Hist->GetXaxis()->SetTitle("pull");
  M2pull1par0_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull1par1_Hist = new TH1D ("M2pull1par1_Hist", "Histogram of pull", 50, -10, 10);
  M2pull1par1_Hist->Sumw2();
  M2pull1par1_Hist->GetXaxis()->SetTitle("pull");
  M2pull1par1_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull1par2_Hist = new TH1D ("M2pull1par2_Hist", "Histogram of pull", 50, -10, 10);
  M2pull1par2_Hist->Sumw2();
  M2pull1par2_Hist->GetXaxis()->SetTitle("pull");
  M2pull1par2_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull1par3_Hist = new TH1D ("M2pull1par3_Hist", "Histogram of pull", 50, -10, 10);
  M2pull1par3_Hist->Sumw2();
  M2pull1par3_Hist->GetXaxis()->SetTitle("pull");
  M2pull1par3_Hist->GetYaxis()->SetTitle("number of events");
if(pull2==1)
{
  TCanvas *M2Canvas_2_1 = new TCanvas("M2Canvas_2_1","M2Canvas_2_1",11,51,700,500);
}
  TH1D * M2pull2par0_Hist = new TH1D ("M2pull2par0_Hist", "Histogram of pull", 50, -10, 10);
  M2pull2par0_Hist->Sumw2();
  M2pull2par0_Hist->GetXaxis()->SetTitle("pull");
  M2pull2par0_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull2par1_Hist = new TH1D ("M2pull2par1_Hist", "Histogram of pull", 50, -10, 10);
  M2pull2par1_Hist->Sumw2();
  M2pull2par1_Hist->GetXaxis()->SetTitle("pull");
  M2pull2par1_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull2par2_Hist = new TH1D ("M2pull2par2_Hist", "Histogram of pull", 50, -10, 10);
  M2pull2par2_Hist->Sumw2();
  M2pull2par2_Hist->GetXaxis()->SetTitle("pull");
  M2pull2par2_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull2par3_Hist = new TH1D ("M2pull2par3_Hist", "Histogram of pull", 50, -10, 10);
  M2pull2par3_Hist->Sumw2();
  M2pull2par3_Hist->GetXaxis()->SetTitle("pull");
  M2pull2par3_Hist->GetYaxis()->SetTitle("number of events");
if(pull2==1)
{
  TCanvas *M2Canvas_2_2 = new TCanvas("M2Canvas_2_2","M2Canvas_2_2",11,51,700,500);
}
  TH1D * M2pull3par0_Hist = new TH1D ("M2pull3par0_Hist", "Histogram of pull", 50, -10, 10);
  M2pull3par0_Hist->Sumw2();
  M2pull3par0_Hist->GetXaxis()->SetTitle("pull");
  M2pull3par0_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull3par1_Hist = new TH1D ("M2pull3par1_Hist", "Histogram of pull", 50, -10, 10);
  M2pull3par1_Hist->Sumw2();
  M2pull3par1_Hist->GetXaxis()->SetTitle("pull");
  M2pull3par1_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull3par2_Hist = new TH1D ("M2pull3par2_Hist", "Histogram of pull", 50, -10, 10);
  M2pull3par2_Hist->Sumw2();
  M2pull3par2_Hist->GetXaxis()->SetTitle("pull");
  M2pull3par2_Hist->GetYaxis()->SetTitle("number of events");
  TH1D * M2pull3par3_Hist = new TH1D ("M2pull3par3_Hist", "Histogram of pull", 50, -10, 10);
  M2pull3par3_Hist->Sumw2();
  M2pull3par3_Hist->GetXaxis()->SetTitle("pull");
  M2pull3par3_Hist->GetYaxis()->SetTitle("number of events");
if(pull2==1)
{
  TCanvas *M2Canvas_2_3 = new TCanvas("M2Canvas_2_3","M2Canvas_2_3",11,51,700,500);
}
  TH1D * M2Sig_Hist1_chi2 = new TH1D ("M2Sig_Hist1_chi2", "histogram for the significance1", 100, 0, 10);
  M2Sig_Hist1_chi2->Sumw2();
  M2Sig_Hist1_chi2->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist1_chi2->GetYaxis()->SetTitle("Number of events");
if(sig2==1)
{
  TCanvas *M2Canvas_3_1 = new TCanvas("M2Canvas_3_1","M2Canvas_3_1",11,51,700,500);
}
  TH1D * M2Sig_Hist2_chi2 = new TH1D ("M2Sig_Hist2_chi2", "histogram for the significance2", 100, 0, 10);
  M2Sig_Hist2_chi2->Sumw2();
  M2Sig_Hist2_chi2->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist2_chi2->GetYaxis()->SetTitle("Number of events");
if(sig2==1)
{
  TCanvas *M2Canvas_3_2 = new TCanvas("M2Canvas_3_2","M2Canvas_3_2",11,51,700,500);
}
  TH1D * M2Sig_Hist3_chi2 = new TH1D ("M2Sig_Hist3_chi2", "histogram for the significance3", 100, 0, 10);
  M2Sig_Hist3_chi2->Sumw2();
  M2Sig_Hist3_chi2->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist3_chi2->GetYaxis()->SetTitle("Number of events");
if(sig2==1)
{
  TCanvas *M2Canvas_3_3 = new TCanvas("M2Canvas_3_3","M2Canvas_3_3",11,51,700,500);
}
  TH1D * M2Sig_Hist_chi2 = new TH1D ("M2Sig_Hist_chi2", "histogram for the significance", 100, 0, 10);
  M2Sig_Hist_chi2->Sumw2();
  M2Sig_Hist_chi2->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist_chi2->GetYaxis()->SetTitle("Number of events");
  TCanvas *M2Canvas_3_0 = new TCanvas("M2Canvas_3_0","M2Canvas_3_0",11,51,700,500);

  TH1D * M2Sig_Hist1_likelihood = new TH1D ("M2Sig_Hist1_likelihood", "histogram for the significance1", 100, 0, 10);
  M2Sig_Hist1_likelihood->Sumw2();
  M2Sig_Hist1_likelihood->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist1_likelihood->GetYaxis()->SetTitle("Number of events");
if(sig2==1)
{
  TCanvas *M2Canvas_4_1 = new TCanvas("M2Canvas_4_1","M2Canvas_4_1",11,51,700,500);
}
  TH1D * M2Sig_Hist2_likelihood = new TH1D ("M2Sig_Hist2_likelihood", "histogram for the significance2", 100, 0, 10);
  M2Sig_Hist2_likelihood->Sumw2();
  M2Sig_Hist2_likelihood->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist2_likelihood->GetYaxis()->SetTitle("Number of events");
if(sig2==1)
{
  TCanvas *M2Canvas_4_2 = new TCanvas("M2Canvas_4_2","M2Canvas_4_2",11,51,700,500);
}
  TH1D * M2Sig_Hist3_likelihood = new TH1D ("M2Sig_Hist3_likelihood", "histogram for the significance3", 100, 0, 10);
  M2Sig_Hist3_likelihood->Sumw2();
  M2Sig_Hist3_likelihood->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist3_likelihood->GetYaxis()->SetTitle("Number of events");
if(sig2==1)
{
  TCanvas *M2Canvas_4_3 = new TCanvas("M2Canvas_4_3","M2Canvas_4_3",11,51,700,500);
}
  TH1D * M2Sig_Hist_likelihood = new TH1D("M2Sig_Hist_likelihood","histogram for the significance",100,0,10);
  M2Sig_Hist_likelihood->Sumw2();
  M2Sig_Hist_likelihood->GetXaxis()->SetTitle("Significance");
  M2Sig_Hist_likelihood->GetYaxis()->SetTitle("Number of events");
  TCanvas *M2Canvas_4_0 = new TCanvas("M1Canvas_4_0","M1Canvas_4_0",11,51,700,500);

}
//*************** Method 3

if(exec3==1)
{
  TF1 * M3signalbackground_fit0 = new TF1 ("M3signalbackground_fit0","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);

  TF1 * M3signalbackground_fit1 = new TF1 ("M3signalbackground_fit1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M3signal_fit1= new TF1 ("M3signal_fit1","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M3background_fit1 = new TF1 ("M3background_fit1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M3fit_background1 = new TF1 ("M3fit_background1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

  TF1 * M3signalbackground_fit2 = new TF1 ("M3signalbackground_fit2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M3signal_fit2= new TF1 ("M3signal_fit2","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M3background_fit2 = new TF1 ("M3background_fit2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M3fit_background2 = new TF1 ("M3fit_background2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

  TF1 * M3signalbackground_fit3 = new TF1 ("M3signalbackground_fit3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * M3signal_fit3= new TF1 ("M3signal_fit3","[0]*exp(-0.5*pow((x-[1])/[2],2))", xmin, xmax);
  TF1 * M3background_fit3 = new TF1 ("M3background_fit3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])", xmin, xmax);
  TF1 * M3fit_background3 = new TF1 ("M3fit_background3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+0*[3]", xmin, xmax);

M3signalbackground_fit0->SetParameters(2.6,9,5,1,mean,sigma);
M3signalbackground_fit0->FixParameter(4,mean);
M3signalbackground_fit0->FixParameter(5,sigma);
M3signalbackground_fit1->SetParameters(2.6,9,5,1,mean,sigma);
M3signalbackground_fit1->FixParameter(4,mean);
M3signalbackground_fit1->FixParameter(5,sigma);
M3signalbackground_fit2->SetParameters(2.6,9,5,1,mean,sigma);
M3signalbackground_fit2->FixParameter(4,mean);
M3signalbackground_fit2->FixParameter(5,sigma);
M3signalbackground_fit3->SetParameters(2.6,9,5,1,mean,sigma);
M3signalbackground_fit3->FixParameter(4,mean);
M3signalbackground_fit3->FixParameter(5,sigma);

if(display3==1)
{
  TCanvas *M3Canvas_1_0 = new TCanvas("M3Canvas_1_0","M3Canvas_1_0",11,51,700,500);
  TCanvas *M3Canvas_1_1 = new TCanvas("M3Canvas_1_1","M3Canvas_1_1",11,51,700,500);
  TCanvas *M3Canvas_1_1bis = new TCanvas("M3Canvas_1_1bis","M3Canvas_1_1bis",11,51,700,500);
  TCanvas *M3Canvas_1_2 = new TCanvas("M3Canvas_1_2","M3Canvas_1_2",11,51,700,500);
  TCanvas *M3Canvas_1_2bis = new TCanvas("M3Canvas_1_2bis","M3Canvas_1_2bis",11,51,700,500);
  TCanvas *M3Canvas_1_3 = new TCanvas("M3Canvas_1_3","M3Canvas_1_3",11,51,700,500);
  TCanvas *M3Canvas_1_3bis = new TCanvas("M3Canvas_1_3bis","M3Canvas_1_3bis",11,51,700,500);
}

  TH1D * M3pull_Hist = new TH1D ("M3pull_Hist", "histogram for pull", 50, -10, 10);
  M3pull_Hist->Sumw2();
  M3pull_Hist->GetXaxis()->SetTitle("pull");
  M3pull_Hist->GetYaxis()->SetTitle("");
if(pull3==1)
{
  TCanvas *M3Canvas_2 = new TCanvas("M3Canvas_2","M3Canvas_2",11,51,700,500);
}

  TH1D * M3Sig_Hist1_chi2 = new TH1D ("M3Sig_Hist1_chi2", "histogram for the significance1", 100, 0, 10);
  M3Sig_Hist1_chi2->Sumw2();
  M3Sig_Hist1_chi2->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist1_chi2->GetYaxis()->SetTitle("Number of events");
if(sig3==1)
{
  TCanvas *M3Canvas_3_1 = new TCanvas("M3Canvas_3_1","M3Canvas_3_1",11,51,700,500);
}
  TH1D * M3Sig_Hist2_chi2 = new TH1D ("M3Sig_Hist2_chi2", "histogram for the significance2", 100, 0, 10);
  M3Sig_Hist2_chi2->Sumw2();
  M3Sig_Hist2_chi2->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist2_chi2->GetYaxis()->SetTitle("Number of events");
if(sig3==1)
{
  TCanvas *M3Canvas_3_2 = new TCanvas("M3Canvas_3_2","M3Canvas_3_2",11,51,700,500);
}
  TH1D * M3Sig_Hist3_chi2 = new TH1D ("M3Sig_Hist3_chi2", "histogram for the significance3", 100, 0, 10);
  M3Sig_Hist3_chi2->Sumw2();
  M3Sig_Hist3_chi2->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist3_chi2->GetYaxis()->SetTitle("Number of events");
if(sig3==1)
{
  TCanvas *M3Canvas_3_3 = new TCanvas("M3Canvas_3_3","M3Canvas_3_3",11,51,700,500);
}
  TH1D * M3Sig_Hist_chi2 = new TH1D ("M3Sig_Hist_chi2", "histogram for the significance", 100, 0, 10);
  M3Sig_Hist_chi2->Sumw2();
  M3Sig_Hist_chi2->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist_chi2->GetYaxis()->SetTitle("Number of events");
  TCanvas *M3Canvas_3_0 = new TCanvas("M3Canvas_3_0","M3Canvas_3_0",11,51,700,500);

  TH1D * M3Sig_Hist1_likelihood = new TH1D ("M3Sig_Hist1_likelihood", "histogram for the significance1", 100, 0, 10);
  M3Sig_Hist1_likelihood->Sumw2();
  M3Sig_Hist1_likelihood->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist1_likelihood->GetYaxis()->SetTitle("Number of events");
if(sig3==1)
{
  TCanvas *M3Canvas_4_1 = new TCanvas("M3Canvas_4_1","M3Canvas_4_1",11,51,700,500);
}
  TH1D * M3Sig_Hist2_likelihood = new TH1D ("M3Sig_Hist2_likelihood", "histogram for the significance2", 100, 0, 10);
  M3Sig_Hist2_likelihood->Sumw2();
  M3Sig_Hist2_likelihood->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist2_likelihood->GetYaxis()->SetTitle("Number of events");
if(sig3==1)
{
  TCanvas *M3Canvas_4_2 = new TCanvas("M3Canvas_4_2","M3Canvas_4_2",11,51,700,500);
}
  TH1D * M3Sig_Hist3_likelihood = new TH1D ("M3Sig_Hist3_likelihood", "histogram for the significance3", 100, 0, 10);
  M3Sig_Hist3_likelihood->Sumw2();
  M3Sig_Hist3_likelihood->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist3_likelihood->GetYaxis()->SetTitle("Number of events");
if(sig3==1)
{
  TCanvas *M3Canvas_4_3 = new TCanvas("M3Canvas_4_3","M3Canvas_4_3",11,51,700,500);
}
  TH1D * M3Sig_Hist_likelihood = new TH1D ("M3Sig_Hist_likelihood", "histogram for the significance", 100, 0, 10);
  M3Sig_Hist_likelihood->Sumw2();
  M3Sig_Hist_likelihood->GetXaxis()->SetTitle("Significance");
  M3Sig_Hist_likelihood->GetYaxis()->SetTitle("Number of events");
  TCanvas *M3Canvas_4_0 = new TCanvas("M3Canvas_4_0","M3Canvas_4_0",11,51,700,500);


}

//***********************************

for (int i=0;i<iend;i++)
{
cout<<"i: "<<i<<endl;

 TH1D * M_Hist1 = new TH1D ("M_Hist1", "Histogram of invariant mass with 0<#Delta#eta<0.5", Nbins, xmin, xmax);
  M_Hist1->Sumw2();
  M_Hist1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M_Hist1->GetYaxis()->SetTitle("Number of events");
 TH1D * M_Hist2 = new TH1D ("M_Hist2", "Histogram of invariant mass with 0.5<#Delta#eta<1", Nbins, xmin, xmax);
  M_Hist2->Sumw2();
  M_Hist2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M_Hist2->GetYaxis()->SetTitle("Number of events");
 TH1D * M_Hist3 = new TH1D ("M_Hist3", "Histogram of invariant mass with 1<#Delta#eta<1.3", Nbins, xmin, xmax);
  M_Hist3->Sumw2();
  M_Hist3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M_Hist3->GetYaxis()->SetTitle("Number of events");
 TH1D * M_Hist = new TH1D ("M_Hist", "Histogram of invariant mass with 0<#Delta#eta<1.3", Nbins, xmin, xmax);
  M_Hist->Sumw2();
  M_Hist->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M_Hist->GetYaxis()->SetTitle("Number of events");

if(display1==1)
{
  TLegend* M1legendbis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M1legend = new TLegend(0.6, 0.1, 1, .5);
}
if(display2==1)
{
  TLegend* M2legend1bis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M2legend1 = new TLegend(0.6, 0.1, 1, .5);
  TLegend* M2legend2bis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M2legend2 = new TLegend(0.6, 0.1, 1, .5);
  TLegend* M2legend3bis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M2legend3 = new TLegend(0.6, 0.1, 1, .5);
}
if(display3==1)
{
  TLegend* M3legend = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M3legend1bis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M3legend1 = new TLegend(0.6, 0.1, 1, .5);
  TLegend* M3legend2bis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M3legend2 = new TLegend(0.6, 0.1, 1, .5);
  TLegend* M3legend3bis = new TLegend(0.15, 0.15, .55, .4);
  TLegend* M3legend3 = new TLegend(0.6, 0.1, 1, .5);
}

	M_Hist1->FillRandom("background1",nevt1);
	M_Hist2->FillRandom("background2",nevt2);
	M_Hist3->FillRandom("background3",nevt3);
	M_Hist1->FillRandom("gauss1",gint1);
	M_Hist2->FillRandom("gauss2",gint2);
	M_Hist3->FillRandom("gauss3",gint3);

M_Hist->Add(M_Hist1,1);
M_Hist->Add(M_Hist2,1);
M_Hist->Add(M_Hist3,1);


//*********** Method 1

if(exec1==1)
{
  TH1D*  M1M_Hist = (TH1D*) M_Hist->Clone("M1M_Hist");
  TH1D*  M1M_Histbis = (TH1D*) M_Hist->Clone("M1M_Histbis");

if(display1==1)
{
M1Canvas_1->cd();
gPad->SetLogy();
M1M_Hist->Draw();
M1M_Hist->Fit(M1signalbackground_fit,"L");
M1chi2_SpB=M1signalbackground_fit->GetChisquare();
}
else
{
 Canvas->cd();
M1M_Hist->Fit(M1signalbackground_fit,"L");
M1chi2_SpB=M1signalbackground_fit->GetChisquare();
}

M1signal_fit->FixParameter(0,M1signalbackground_fit->GetParameter(3));
M1signal_fit->FixParameter(1,M1signalbackground_fit->GetParameter(4));
M1signal_fit->FixParameter(2,M1signalbackground_fit->GetParameter(5));
M1background_fit->FixParameter(0,M1signalbackground_fit->GetParameter(0));
M1background_fit->FixParameter(1,M1signalbackground_fit->GetParameter(1));
M1background_fit->FixParameter(2,M1signalbackground_fit->GetParameter(2));
M1fit_background->FixParameter(0,M1background_fit->GetParameter(0));
M1fit_background->FixParameter(1,M1background_fit->GetParameter(1));
M1fit_background->FixParameter(2,M1background_fit->GetParameter(2));
if(display1==1)
{
M1signal_fit->SetLineColor(kMagenta+2);
M1signal_fit->Draw("same");
M1background_fit->SetLineColor(kGreen+2);
M1background_fit->Draw("same");
 M1legend->AddEntry(M1M_Hist,"invariant mass","l");
 M1legend->AddEntry((TObject*)0,"","");
 M1legend->AddEntry(M1signalbackground_fit,"fit from background and signal","l");
 M1legend->AddEntry((TObject*)0,"","");
 M1legend->AddEntry(M1signalbackground_fit," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M1legend->AddEntry((TObject*)0,"","");
 M1legend->AddEntry((TObject*)0,"","");
 M1legend->AddEntry(M1signal_fit,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M1legend->AddEntry((TObject*)0,"","");
 M1legend->AddEntry(M1fit_background,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M1legend->AddEntry((TObject*)0,"","");
M1legend->Draw();
}

if(display1==1)
{
M1Canvas_1bis->cd();
gPad->SetLogy();
M1M_Histbis->Draw();
M1M_Histbis->Fit(M1fit_background,"L");
M1chi2_B=M1fit_background->GetChisquare();
M1fit_background->SetLineColor(kGreen+2);
 M1legendbis->AddEntry(M1M_Histbis,"invariant mass","l");
 M1legendbis->AddEntry((TObject*)0,"","");
 M1legendbis->AddEntry(M1fit_background,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M1legendbis->AddEntry((TObject*)0,"","");
M1legendbis->Draw();
}
else
{
Canvas->cd();
M1M_Histbis->Fit(M1fit_background,"L");
M1chi2_B=M1fit_background->GetChisquare();
}

cout<<"method1: chi2_SpB1: "<<M1chi2_SpB<<endl;
cout<<"method1: chi2_B1: "<<M1chi2_B<<endl;
if(M1chi2_B>M1chi2_SpB) M1sig=TMath::Sqrt(M1chi2_B-M1chi2_SpB);
else M1sig=0;
cout<<"method1: sig (chi2): "<<M1sig<<endl;



M1pullpar0=(M1signalbackground_fit->GetParameter(0)-P0)/M1signalbackground_fit->GetParError(0);
M1pullpar1=(M1signalbackground_fit->GetParameter(1)-P1)/M1signalbackground_fit->GetParError(1);
M1pullpar2=(M1signalbackground_fit->GetParameter(2)-P2)/M1signalbackground_fit->GetParError(2);

M1scalefactor=M1signalbackground_fit->GetParameter(5)*gaussnorm;
M1Nint=M1signalbackground_fit->GetParameter(3)*M1scalefactor;
M1errorNint=M1signalbackground_fit->GetParError(3)*M1scalefactor;
M1pullpar3=(M1Nint-gint1-gint2-gint3)/M1errorNint;
cout<<"method1: pull0: "<<M1pullpar0<<endl;
cout<<"method1: pull1: "<<M1pullpar1<<endl;
cout<<"method1: pull2: "<<M1pullpar2<<endl;
cout<<"method1: pull3: "<<M1pullpar3<<endl;


M1pullpar0_Hist->Fill(M1pullpar0);
M1pullpar1_Hist->Fill(M1pullpar1);
M1pullpar2_Hist->Fill(M1pullpar2);
M1pullpar3_Hist->Fill(M1pullpar3);
M1Sig_Hist_chi2->Fill(M1sig);

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist, M1background_fit, M1signalbackground_fit, M1signal_fit, M1significance,M1lnLB,M1lnLSpB);
cout<<"method1: sig (likelihood) "<<M1significance<<endl;
cout<<"method1: lnLB: "<<M1lnLB<<endl;
cout<<"method1: lnLSpB: "<<M1lnLSpB<<endl;

M1Sig_Hist_likelihood->Fill(M1significance);

}

//******** Method 2

if(exec2==1)
{
  TH1D*  M2M_Hist1 = (TH1D*) M_Hist1->Clone(" M2M_Hist1");
  TH1D*  M2M_Hist2 = (TH1D*) M_Hist2->Clone(" M2M_Hist2");
  TH1D*  M2M_Hist3 = (TH1D*) M_Hist3->Clone(" M2M_Hist3");
  TH1D*  M2M_Hist1bis = (TH1D*) M_Hist1->Clone(" M2M_Hist1bis");
  TH1D*  M2M_Hist2bis = (TH1D*) M_Hist2->Clone(" M2M_Hist2bis");
  TH1D*  M2M_Hist3bis = (TH1D*) M_Hist3->Clone(" M2M_Hist3bis");



//  1      ****************************  
         
if(display2==1)
{
M2Canvas_1_1->cd();
gPad->SetLogy();
M2M_Hist1->Draw();
M2M_Hist1->Fit(M2signalbackground_fit1,"L");
M2chi2_SpB1=M2signalbackground_fit1->GetChisquare();
}
else
{
Canvas->cd();
M2M_Hist1->Fit(M2signalbackground_fit1,"L");
M2chi2_SpB1=M2signalbackground_fit1->GetChisquare();
}


M2signal_fit1->FixParameter(0,M2signalbackground_fit1->GetParameter(3));
M2signal_fit1->FixParameter(1,M2signalbackground_fit1->GetParameter(4));
M2signal_fit1->FixParameter(2,M2signalbackground_fit1->GetParameter(5));
M2background_fit1->FixParameter(0,M2signalbackground_fit1->GetParameter(0));
M2background_fit1->FixParameter(1,M2signalbackground_fit1->GetParameter(1));
M2background_fit1->FixParameter(2,M2signalbackground_fit1->GetParameter(2));
M2fit_background1->FixParameter(0,M2background_fit1->GetParameter(0));
M2fit_background1->FixParameter(1,M2background_fit1->GetParameter(1));
M2fit_background1->FixParameter(2,M2background_fit1->GetParameter(2));
if(display2==1)
{
M2signal_fit1->SetLineColor(kMagenta+2);
M2signal_fit1->Draw("same");
M2background_fit1->SetLineColor(kGreen+2);
M2background_fit1->Draw("same");
 M2legend1->AddEntry(M2M_Hist1,"invariant mass","l");
 M2legend1->AddEntry((TObject*)0,"","");
 M2legend1->AddEntry(M2signalbackground_fit1,"fit from background and signal","l");
 M2legend1->AddEntry((TObject*)0,"","");
 M2legend1->AddEntry(M2signalbackground_fit1," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M2legend1->AddEntry((TObject*)0,"","");
 M2legend1->AddEntry((TObject*)0,"","");
 M2legend1->AddEntry(M2signal_fit1,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M2legend1->AddEntry((TObject*)0,"","");
 M2legend1->AddEntry(M2fit_background1,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M2legend1->AddEntry((TObject*)0,"","");
M2legend1->Draw();
}

if(display2==1)
{
M2Canvas_1_1bis->cd();
gPad->SetLogy();
M2M_Hist1bis->Draw();
M2M_Hist1bis->Fit(M2fit_background1,"L");
M2chi2_B1=M2fit_background1->GetChisquare();
M2fit_background1->SetLineColor(kGreen+2);
 M2legend1bis->AddEntry(M2M_Hist1bis,"invariant mass","l");
 M2legend1bis->AddEntry((TObject*)0,"","");
 M2legend1bis->AddEntry(M2fit_background1,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M2legend1bis->AddEntry((TObject*)0,"","");
M2legend1bis->Draw();
}
else
{
Canvas->cd();
M2M_Hist1bis->Fit(M2fit_background1,"L");
M2chi2_B1=M2fit_background1->GetChisquare();
}

cout<<"method2: chi2_SpB1: "<<M2chi2_SpB1<<endl;
cout<<"method2: chi2_B1: "<<M2chi2_B1<<endl;
if(M2chi2_B1>M2chi2_SpB1) M2sig1=TMath::Sqrt(M2chi2_B1-M2chi2_SpB1);
else M2sig1=0;
cout<<"method2: sig1 (chi2): "<<M2sig1<<endl;


M2pull1par0=(M2signalbackground_fit1->GetParameter(0)-P0*P3)/M2signalbackground_fit1->GetParError(0);
M2pull1par1=(M2signalbackground_fit1->GetParameter(1)-P1)/M2signalbackground_fit1->GetParError(1);
M2pull1par2=(M2signalbackground_fit1->GetParameter(2)-P2)/M2signalbackground_fit1->GetParError(2);

M2scalefactor1=M2signalbackground_fit1->GetParameter(5)*gaussnorm;
M2Nint1=M2signalbackground_fit1->GetParameter(3)*M2scalefactor1;
M2errorNint1=M2signalbackground_fit1->GetParError(3)*M2scalefactor1;
M2pull1par3=(M2Nint1-gint1)/M2errorNint1;
cout<<"method2: pull1par0: "<<M2pull1par0<<endl;
cout<<"method2: pull1par1: "<<M2pull1par1<<endl;
cout<<"method2: pull1par2: "<<M2pull1par2<<endl;
cout<<"method2: pull1par3: "<<M2pull1par3<<endl;

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist1, M2background_fit1, M2signalbackground_fit1, M2signal_fit1, M2significance1,M2lnLB1,M2lnLSpB1);
cout<<"method2: sig1 (likelihood) "<<M2significance1<<endl;
cout<<"method2: lnLB1: "<<M2lnLB1<<endl;
cout<<"method2: lnLSpB1: "<<M2lnLSpB1<<endl;


//  2      ****************************  

if(display2==1)
{
M2Canvas_1_2->cd();
gPad->SetLogy();
M2M_Hist2->Draw();
M2M_Hist2->Fit(M2signalbackground_fit2,"L");
M2chi2_SpB2=M2signalbackground_fit2->GetChisquare();
}
else
{
Canvas->cd();
M2M_Hist2->Fit(M2signalbackground_fit2,"L");
M2chi2_SpB2=M2signalbackground_fit2->GetChisquare();
}

M2signal_fit2->FixParameter(0,M2signalbackground_fit2->GetParameter(3));
M2signal_fit2->FixParameter(1,M2signalbackground_fit2->GetParameter(4));
M2signal_fit2->FixParameter(2,M2signalbackground_fit2->GetParameter(5));
M2background_fit2->FixParameter(0,M2signalbackground_fit2->GetParameter(0));
M2background_fit2->FixParameter(1,M2signalbackground_fit2->GetParameter(1));
M2background_fit2->FixParameter(2,M2signalbackground_fit2->GetParameter(2));
M2fit_background2->FixParameter(0,M2background_fit2->GetParameter(0));
M2fit_background2->FixParameter(1,M2background_fit2->GetParameter(1));
M2fit_background2->FixParameter(2,M2background_fit2->GetParameter(2));
if(display2==1)
{
M2signal_fit2->SetLineColor(kMagenta+2);
M2signal_fit2->Draw("same");
M2background_fit2->SetLineColor(kGreen+2);
M2background_fit2->Draw("same");
 M2legend2->AddEntry(M2M_Hist2,"invariant mass","l");
 M2legend2->AddEntry((TObject*)0,"","");
 M2legend2->AddEntry(M2signalbackground_fit2,"fit from background and signal","l");
 M2legend2->AddEntry((TObject*)0,"","");
 M2legend2->AddEntry(M2signalbackground_fit2," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M2legend2->AddEntry((TObject*)0,"","");
 M2legend2->AddEntry((TObject*)0,"","");
 M2legend2->AddEntry(M2signal_fit2,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M2legend2->AddEntry((TObject*)0,"","");
 M2legend2->AddEntry(M2fit_background2,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M2legend2->AddEntry((TObject*)0,"","");
M2legend2->Draw();
}

if(display2==1)
{
M2Canvas_1_2bis->cd();
gPad->SetLogy();
M2M_Hist2bis->Draw();
M2M_Hist2bis->Fit(M2fit_background2,"L");
M2chi2_B2=M2fit_background2->GetChisquare();
M2fit_background2->SetLineColor(kGreen+2);
 M2legend2bis->AddEntry(M2M_Hist2bis,"invariant mass","l");
 M2legend2bis->AddEntry((TObject*)0,"","");
 M2legend2bis->AddEntry(M2fit_background2,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M2legend2bis->AddEntry((TObject*)0,"","");
M2legend2bis->Draw();
}
else
{
Canvas->cd();
M2M_Hist2bis->Fit(M2fit_background2,"L");
M2chi2_B2=M2fit_background2->GetChisquare();
}

cout<<"method2: chi2_SpB2: "<<M2chi2_SpB2<<endl;
cout<<"method2: chi2_B2: "<<M2chi2_B2<<endl;
if(M2chi2_B2>M2chi2_SpB2) M2sig2=TMath::Sqrt(M2chi2_B2-M2chi2_SpB2);
else M2sig2=0;
cout<<"method2: sig2: "<<M2sig2<<endl;


M2pull2par0=(M2signalbackground_fit2->GetParameter(0)-P0*P5)/M2signalbackground_fit2->GetParError(0);
M2pull2par1=(M2signalbackground_fit2->GetParameter(1)-P1)/M2signalbackground_fit2->GetParError(1);
M2pull2par2=(M2signalbackground_fit2->GetParameter(2)-P2)/M2signalbackground_fit2->GetParError(2);

M2scalefactor2=M2signalbackground_fit2->GetParameter(5)*gaussnorm;
M2Nint2=M2signalbackground_fit2->GetParameter(3)*M2scalefactor2;
M2errorNint2=M2signalbackground_fit2->GetParError(3)*M2scalefactor2;
M2pull2par3=(M2Nint2-gint2)/M2errorNint2;
cout<<"method2: pull2par0: "<<M2pull2par0<<endl;
cout<<"method2: pull2par1: "<<M2pull2par1<<endl;
cout<<"method2: pull2par2: "<<M2pull2par2<<endl;
cout<<"method2: pull2par3: "<<M2pull2par3<<endl;

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist2, M2background_fit2, M2signalbackground_fit2, M2signal_fit2, M2significance2,M2lnLB2,M2lnLSpB2);
cout<<"method2: sig2 (likelihood) "<<M2significance2<<endl;
cout<<"method2: lnLB2: "<<M2lnLB2<<endl;
cout<<"method2: lnLSpB2: "<<M2lnLSpB2<<endl;


//  3      ****************************  

if(display2==1)
{
M2Canvas_1_3->cd();
gPad->SetLogy();
M2M_Hist3->Draw();
M2M_Hist3->Fit(M2signalbackground_fit3,"L");
M2chi2_SpB3=M2signalbackground_fit3->GetChisquare();
}
else
{
Canvas->cd();
M2M_Hist3->Fit(M2signalbackground_fit3,"L");
M2chi2_SpB3=M2signalbackground_fit3->GetChisquare();
}

M2signal_fit3->FixParameter(0,M2signalbackground_fit3->GetParameter(3));
M2signal_fit3->FixParameter(1,M2signalbackground_fit3->GetParameter(4));
M2signal_fit3->FixParameter(2,M2signalbackground_fit3->GetParameter(5));
M2background_fit3->FixParameter(0,M2signalbackground_fit3->GetParameter(0));
M2background_fit3->FixParameter(1,M2signalbackground_fit3->GetParameter(1));
M2background_fit3->FixParameter(2,M2signalbackground_fit3->GetParameter(2));
M2fit_background3->FixParameter(0,M2background_fit3->GetParameter(0));
M2fit_background3->FixParameter(1,M2background_fit3->GetParameter(1));
M2fit_background3->FixParameter(2,M2background_fit3->GetParameter(2));
if(display2==1)
{
M2signal_fit3->SetLineColor(kMagenta+2);
M2signal_fit3->Draw("same");
M2background_fit3->SetLineColor(kGreen+2);
M2background_fit3->Draw("same");
 M2legend3->AddEntry(M2M_Hist3,"invariant mass","l");
 M2legend3->AddEntry((TObject*)0,"","");
 M2legend3->AddEntry(M2signalbackground_fit3,"fit from background and signal","l");
 M2legend3->AddEntry((TObject*)0,"","");
 M2legend3->AddEntry(M2signalbackground_fit3," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M2legend3->AddEntry((TObject*)0,"","");
 M2legend3->AddEntry((TObject*)0,"","");
 M2legend3->AddEntry(M2signal_fit3,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M2legend3->AddEntry((TObject*)0,"","");
 M2legend3->AddEntry(M2fit_background3,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M2legend3->AddEntry((TObject*)0,"","");
M2legend3->Draw();
}

if(display2==1)
{
M2Canvas_1_3bis->cd();
gPad->SetLogy();
M2M_Hist3bis->Draw();
M2M_Hist3bis->Fit(M2fit_background3,"L");
M2chi2_B3=M2fit_background3->GetChisquare();
M2fit_background3->SetLineColor(kGreen+2);
 M2legend3bis->AddEntry(M2M_Hist3bis,"invariant mass","l");
 M2legend3bis->AddEntry((TObject*)0,"","");
 M2legend3bis->AddEntry(M2fit_background3,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M2legend3bis->AddEntry((TObject*)0,"","");
M2legend3bis->Draw();
}
else
{
Canvas->cd();
M2M_Hist3bis->Fit(M2fit_background3,"L");
M2chi2_B3=M2fit_background3->GetChisquare();
}


cout<<"method2: chi2_SpB3: "<<M2chi2_SpB3<<endl;
cout<<"method2: chi2_B3: "<<M2chi2_B3<<endl;
if(M2chi2_B3>M2chi2_SpB3) M2sig3=TMath::Sqrt(M2chi2_B3-M2chi2_SpB3);
else M2sig3=0;
cout<<"method2: sig3: "<<M2sig3<<endl;



M2pull3par0=(M2signalbackground_fit3->GetParameter(0)-P0*P7)/M2signalbackground_fit3->GetParError(0);
M2pull3par1=(M2signalbackground_fit3->GetParameter(1)-P1)/M2signalbackground_fit3->GetParError(1);
M2pull3par2=(M2signalbackground_fit3->GetParameter(2)-P2)/M2signalbackground_fit3->GetParError(2);

M2scalefactor3=M2signalbackground_fit3->GetParameter(5)*gaussnorm;
M2Nint3=M2signalbackground_fit3->GetParameter(3)*M2scalefactor3;
M2errorNint3=M2signalbackground_fit3->GetParError(3)*M2scalefactor3;
M2pull3par3=(M2Nint3-gint3)/M2errorNint3;
cout<<"method2: pull3par0: "<<M2pull3par0<<endl;
cout<<"method2: pull3par1: "<<M2pull3par1<<endl;
cout<<"method2: pull3par2: "<<M2pull3par2<<endl;
cout<<"method2: pull3par3: "<<M2pull3par3<<endl;

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist3, M2background_fit3, M2signalbackground_fit3, M2signal_fit3, M2significance3,M2lnLB3,M2lnLSpB3);
cout<<"method2: sig3 (likelihood) "<<M2significance3<<endl;
cout<<"method2: lnLB3: "<<M2lnLB3<<endl;
cout<<"method2: lnLSpB3: "<<M2lnLSpB3<<endl;



//         ***********************


M2chi2_B=M2chi2_B1+M2chi2_B2+M2chi2_B3;
M2chi2_SpB=M2chi2_SpB1+M2chi2_SpB2+M2chi2_SpB3;
cout<<"method2: chi2_SpB: "<<M2chi2_SpB<<endl;
cout<<"method2: chi2_B: "<<M2chi2_B<<endl;
if(M2chi2_B>M2chi2_SpB) M2sig=TMath::Sqrt(M2chi2_B-M2chi2_SpB);
else M2sig=0;
cout<<"method2: sig (chi2): "<<M2sig<<endl;
M2lnLB=M2lnLB1+M2lnLB2+M2lnLB3;
M2lnLSpB=M2lnLSpB1+M2lnLSpB2+M2lnLSpB3;
M2significance=TMath::Sqrt(-2*(M2lnLB-M2lnLSpB));
//M2significance=1;
cout<<"method2: sig (likelihood): "<<M2significance<<endl;



M2pull1par0_Hist->Fill(M2pull1par0);
M2pull1par1_Hist->Fill(M2pull1par1);
M2pull1par2_Hist->Fill(M2pull1par2);
M2pull1par3_Hist->Fill(M2pull1par3);
M2pull2par0_Hist->Fill(M2pull2par0);
M2pull2par1_Hist->Fill(M2pull2par1);
M2pull2par2_Hist->Fill(M2pull2par2);
M2pull2par3_Hist->Fill(M2pull2par3);
M2pull3par0_Hist->Fill(M2pull3par0);
M2pull3par1_Hist->Fill(M2pull3par1);
M2pull3par2_Hist->Fill(M2pull3par2);
M2pull3par3_Hist->Fill(M2pull3par3);


M2Sig_Hist1_chi2->Fill(M2sig1);
M2Sig_Hist2_chi2->Fill(M2sig2);
M2Sig_Hist3_chi2->Fill(M2sig3);
M2Sig_Hist_chi2->Fill(M2sig);
M2Sig_Hist1_likelihood->Fill(M2significance1);
M2Sig_Hist2_likelihood->Fill(M2significance2);
M2Sig_Hist3_likelihood->Fill(M2significance3);
M2Sig_Hist_likelihood->Fill(M2significance);
}

//******************* method3

if(exec3==1)
{
  TH1D*  M3M_Hist = (TH1D*) M_Hist->Clone(" M3M_Hist");
  TH1D*  M3M_Hist1 = (TH1D*) M_Hist1->Clone(" M3M_Hist1");
  TH1D*  M3M_Hist2 = (TH1D*) M_Hist2->Clone(" M3M_Hist2");
  TH1D*  M3M_Hist3 = (TH1D*) M_Hist3->Clone(" M3M_Hist3");
  TH1D*  M3M_Hist1bis = (TH1D*) M_Hist1->Clone(" M3M_Hist1bis");
  TH1D*  M3M_Hist2bis = (TH1D*) M_Hist2->Clone(" M3M_Hist2bis");
  TH1D*  M3M_Hist3bis = (TH1D*) M_Hist3->Clone(" M3M_Hist3bis");

//  0      *****************************

if(display3==1)
{
M3Canvas_1_0->cd();
gPad->SetLogy();
M3M_Hist->Draw();
M3M_Hist->Fit(M3signalbackground_fit0,"L");
M3norm0=M3signalbackground_fit0->GetParameter(3);
 M3legend->AddEntry(M3M_Hist,"invariant mass","l");
 M3legend->AddEntry((TObject*)0,"","");
 M3legend->AddEntry(M3signalbackground_fit0,"fit from background and signal","l");
 M3legend->AddEntry((TObject*)0,"","");
 M3legend->AddEntry(M3signalbackground_fit0," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M3legend->AddEntry((TObject*)0,"","");
M3legend->Draw();
}
else
{
Canvas->cd();
M3M_Hist->Fit(M3signalbackground_fit0,"L");
M3norm0=M3signalbackground_fit0->GetParameter(3);
}
M3norm1=r1*M3norm0;
M3norm2=r2*M3norm0;
M3norm3=r3*M3norm0;


cout<<"method3: normalization parameter 0: "<<M3norm0<<endl;
cout<<"method3: normalization parameter 1: "<<M3norm1<<endl;
cout<<"method3: normalization parameter 2: "<<M3norm2<<endl;
cout<<"method3: normalization parameter 3: "<<M3norm3<<endl;

M3signalbackground_fit1->FixParameter(3,M3norm1);
M3signalbackground_fit2->FixParameter(3,M3norm2);
M3signalbackground_fit3->FixParameter(3,M3norm3);

M3scalefactor=M3signalbackground_fit0->GetParameter(5)*gaussnorm;
M3Nint=M3signalbackground_fit0->GetParameter(3)*M3scalefactor;
M3errorNint=M3signalbackground_fit0->GetParError(3)*M3scalefactor;
M3pull=(M3Nint-gint1-gint2-gint3)/M3errorNint;
cout<<"method3: pull: "<<M3pull<<endl;


//  1      ****************************  
        
if(display3==1)
{
M3Canvas_1_1->cd();
gPad->SetLogy();
M3M_Hist1->Draw();
M3M_Hist1->Fit(M3signalbackground_fit1,"L");
M3chi2_SpB1=M3signalbackground_fit1->GetChisquare();
}
else
{
Canvas->cd();
M3M_Hist1->Fit(M3signalbackground_fit1,"L");
M3chi2_SpB1=M3signalbackground_fit1->GetChisquare();
}


M3signal_fit1->FixParameter(0,M3signalbackground_fit1->GetParameter(3));
M3signal_fit1->FixParameter(1,M3signalbackground_fit1->GetParameter(4));
M3signal_fit1->FixParameter(2,M3signalbackground_fit1->GetParameter(5));
M3background_fit1->FixParameter(0,M3signalbackground_fit1->GetParameter(0));
M3background_fit1->FixParameter(1,M3signalbackground_fit1->GetParameter(1));
M3background_fit1->FixParameter(2,M3signalbackground_fit1->GetParameter(2));
M3fit_background1->FixParameter(0,M3background_fit1->GetParameter(0));
M3fit_background1->FixParameter(1,M3background_fit1->GetParameter(1));
M3fit_background1->FixParameter(2,M3background_fit1->GetParameter(2));
if(display3==1)
{
M3signal_fit1->SetLineColor(kMagenta+2);
M3signal_fit1->Draw("same");
M3background_fit1->SetLineColor(kGreen+2);
M3background_fit1->Draw("same");
 M3legend1->AddEntry(M3M_Hist1,"invariant mass","l");
 M3legend1->AddEntry((TObject*)0,"","");
 M3legend1->AddEntry(M3signalbackground_fit1,"fit from background and signal","l");
 M3legend1->AddEntry((TObject*)0,"","");
 M3legend1->AddEntry(M3signalbackground_fit1," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M3legend1->AddEntry((TObject*)0,"","");
 M3legend1->AddEntry((TObject*)0,"","");
 M3legend1->AddEntry(M3signal_fit1,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M3legend1->AddEntry((TObject*)0,"","");
 M3legend1->AddEntry(M3fit_background1,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M3legend1->AddEntry((TObject*)0,"","");
M3legend1->Draw();
}

if(display3==1)
{
M3Canvas_1_1bis->cd();
gPad->SetLogy();
M3M_Hist1bis->Draw();
M3M_Hist1bis->Fit(M3fit_background1,"L");
M3chi2_B1=M3fit_background1->GetChisquare();
M3fit_background1->SetLineColor(kGreen+2);
 M3legend1bis->AddEntry(M3M_Hist1bis,"invariant mass","l");
 M3legend1bis->AddEntry((TObject*)0,"","");
 M3legend1bis->AddEntry(M3fit_background1,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M3legend1bis->AddEntry((TObject*)0,"","");
M3legend1bis->Draw();
}
else
{
Canvas->cd();
M3M_Hist1bis->Fit(M3fit_background1,"L");
M3chi2_B1=M3fit_background1->GetChisquare();
}

cout<<"method3: chi2_SpB1: "<<M3chi2_SpB1<<endl;
cout<<"method3: chi2_B1: "<<M3chi2_B1<<endl;
if(M3chi2_B1>M3chi2_SpB1) M3sig1=TMath::Sqrt(M3chi2_B1-M3chi2_SpB1);
else M3sig1=0;
cout<<"method3: sig1 (chi2): "<<M3sig1<<endl;

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist1, M3background_fit1, M3signalbackground_fit1, M3signal_fit1, M3significance1,M3lnLB1,M3lnLSpB1);
cout<<"method3: sig1 (likelihood) "<<M3significance1<<endl;
cout<<"method3: lnLB1: "<<M3lnLB1<<endl;
cout<<"method3: lnLSpB1: "<<M3lnLSpB1<<endl;



//  2      ****************************  

if(display3==1)
{
M3Canvas_1_2->cd();
gPad->SetLogy();
M3M_Hist2->Draw();
M3M_Hist2->Fit(M3signalbackground_fit2,"L");
M3chi2_SpB2=M3signalbackground_fit2->GetChisquare();
}
else   
{
Canvas->cd();
M3M_Hist2->Fit(M3signalbackground_fit2,"L");
M3chi2_SpB2=M3signalbackground_fit2->GetChisquare();
}

M3signal_fit2->FixParameter(0,M3signalbackground_fit2->GetParameter(3));
M3signal_fit2->FixParameter(1,M3signalbackground_fit2->GetParameter(4));
M3signal_fit2->FixParameter(2,M3signalbackground_fit2->GetParameter(5));
M3background_fit2->FixParameter(0,M3signalbackground_fit2->GetParameter(0));
M3background_fit2->FixParameter(1,M3signalbackground_fit2->GetParameter(1));
M3background_fit2->FixParameter(2,M3signalbackground_fit2->GetParameter(2));
M3fit_background2->FixParameter(0,M3background_fit2->GetParameter(0));
M3fit_background2->FixParameter(1,M3background_fit2->GetParameter(1));
M3fit_background2->FixParameter(2,M3background_fit2->GetParameter(2));
if(display3==1)
{
M3signal_fit2->SetLineColor(kMagenta+2);
M3signal_fit2->Draw("same");
M3background_fit2->SetLineColor(kGreen+2);
M3background_fit2->Draw("same");
 M3legend2->AddEntry(M3M_Hist2,"invariant mass","l");
 M3legend2->AddEntry((TObject*)0,"","");
 M3legend2->AddEntry(M3signalbackground_fit2,"fit from background and signal","l");
 M3legend2->AddEntry((TObject*)0,"","");
 M3legend2->AddEntry(M3signalbackground_fit2," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M3legend2->AddEntry((TObject*)0,"","");
 M3legend2->AddEntry((TObject*)0,"","");
 M3legend2->AddEntry(M3signal_fit2,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M3legend2->AddEntry((TObject*)0,"","");
 M3legend2->AddEntry(M3fit_background2,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M3legend2->AddEntry((TObject*)0,"","");
M3legend2->Draw();
}

if(display3==1)
{
M3Canvas_1_2bis->cd();
gPad->SetLogy();
M3M_Hist2bis->Draw();
M3M_Hist2bis->Fit(M3fit_background2,"L");
M3chi2_B2=M3fit_background2->GetChisquare();
M3fit_background2->SetLineColor(kGreen+2);
 M3legend2bis->AddEntry(M3M_Hist2bis,"invariant mass","l");
 M3legend2bis->AddEntry((TObject*)0,"","");
 M3legend2bis->AddEntry(M3fit_background2,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M3legend2bis->AddEntry((TObject*)0,"","");
M3legend2bis->Draw();
}
else
{
Canvas->cd();
M3M_Hist2bis->Fit(M3fit_background2,"L");
M3chi2_B2=M3fit_background2->GetChisquare();
}

cout<<"method3: chi2_SpB2: "<<M3chi2_SpB2<<endl;
cout<<"method3: chi2_B2: "<<M3chi2_B2<<endl;
if(M3chi2_B2>M3chi2_SpB2) M3sig2=TMath::Sqrt(M3chi2_B2-M3chi2_SpB2);
else M3sig2=0;
cout<<"method3: sig2 (chi2): "<<M3sig2<<endl;

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist2, M3background_fit2, M3signalbackground_fit2, M3signal_fit2, M3significance2,M3lnLB2,M3lnLSpB2);
cout<<"method3: sig2 (likelihood) "<<M3significance2<<endl;
cout<<"method3: lnLB2: "<<M3lnLB2<<endl;
cout<<"method3: lnLSpB2: "<<M3lnLSpB2<<endl;


//  3      ****************************  

if(display3==1)
{
M3Canvas_1_3->cd();
gPad->SetLogy();
M3M_Hist3->Draw();
M3M_Hist3->Fit(M3signalbackground_fit3,"L");
M3chi2_SpB3=M3signalbackground_fit3->GetChisquare();
}
else
{
Canvas->cd();
M3M_Hist3->Fit(M3signalbackground_fit3,"L");
M3chi2_SpB3=M3signalbackground_fit3->GetChisquare();
}

M3signal_fit3->FixParameter(0,M3signalbackground_fit3->GetParameter(3));
M3signal_fit3->FixParameter(1,M3signalbackground_fit3->GetParameter(4));
M3signal_fit3->FixParameter(2,M3signalbackground_fit3->GetParameter(5));
M3background_fit3->FixParameter(0,M3signalbackground_fit3->GetParameter(0));
M3background_fit3->FixParameter(1,M3signalbackground_fit3->GetParameter(1));
M3background_fit3->FixParameter(2,M3signalbackground_fit3->GetParameter(2));
M3fit_background3->FixParameter(0,M3background_fit3->GetParameter(0));
M3fit_background3->FixParameter(1,M3background_fit3->GetParameter(1));
M3fit_background3->FixParameter(2,M3background_fit3->GetParameter(2));
if(display3==1)
{
M3signal_fit3->SetLineColor(kMagenta+2);
M3signal_fit3->Draw("same");
M3background_fit3->SetLineColor(kGreen+2);
M3background_fit3->Draw("same");
 M3legend3->AddEntry(M3M_Hist3,"invariant mass","l");
 M3legend3->AddEntry((TObject*)0,"","");
 M3legend3->AddEntry(M3signalbackground_fit3,"fit from background and signal","l");
 M3legend3->AddEntry((TObject*)0,"","");
 M3legend3->AddEntry(M3signalbackground_fit3," P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}+P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","");
 M3legend3->AddEntry((TObject*)0,"","");
 M3legend3->AddEntry((TObject*)0,"","");
 M3legend3->AddEntry(M3signal_fit3,"signal fit:  P3#timese^{#frac{-0.5#times(x-P4)^{2}}{P5^{2}}}","l");
 M3legend3->AddEntry((TObject*)0,"","");
 M3legend3->AddEntry(M3fit_background3,"background fit:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}","l");
 M3legend3->AddEntry((TObject*)0,"","");
M3legend3->Draw();
}

if(display3==1)
{
M3Canvas_1_3bis->cd();
gPad->SetLogy();
M3M_Hist3bis->Draw();
M3M_Hist3bis->Fit(M3fit_background3,"L");
M3chi2_B3=M3fit_background3->GetChisquare();
M3fit_background3->SetLineColor(kGreen+2);
 M3legend3bis->AddEntry(M3M_Hist3bis,"invariant mass","l");
 M3legend3bis->AddEntry((TObject*)0,"","");
 M3legend3bis->AddEntry(M3fit_background3,"fit from background:  P0#times#frac{(1-x/8000)^{P1}}{(x/8000)^{P2}}  ","l");
 M3legend3bis->AddEntry((TObject*)0,"","");
M3legend3bis->Draw();
}
else
{
Canvas->cd();
M3M_Hist3bis->Fit(M3fit_background3,"L");
M3chi2_B3=M3fit_background3->GetChisquare();
}


cout<<"method3: chi2_SpB3: "<<M3chi2_SpB3<<endl;
cout<<"method3: chi2_B3: "<<M3chi2_B3<<endl;
if(M3chi2_B3>M3chi2_SpB3) M3sig3=sqrt(M3chi2_B3-M3chi2_SpB3);
else M3sig3=0;
cout<<"method3: sig3 (chi2): "<<M3sig3<<endl;

CalcSignificance(xmin,xmax,mean,sigma,f,binwidth,M_Hist3, M3background_fit3, M3signalbackground_fit3, M3signal_fit3, M3significance3,M3lnLB3,M3lnLSpB3);
cout<<"method3: sig3 (likelihood) "<<M3significance3<<endl;
cout<<"method3: lnLB3: "<<M3lnLB3<<endl;
cout<<"method3: lnLSpB3: "<<M3lnLSpB3<<endl;


//         ***********************


M3chi2_B=M3chi2_B1+M3chi2_B2+M3chi2_B3;
M3chi2_SpB=M3chi2_SpB1+M3chi2_SpB2+M3chi2_SpB3;
cout<<"method3: chi2_SpB: "<<M3chi2_SpB<<endl;
cout<<"method3: chi2_B: "<<M3chi2_B<<endl;
if(M3chi2_B>M3chi2_SpB) M3sig=TMath::Sqrt(M3chi2_B-M3chi2_SpB);
else M3sig=0;
cout<<"method3: sig (chi2): "<<M3sig<<endl;
M3lnLB=M3lnLB1+M3lnLB2+M3lnLB3;
M3lnLSpB=M3lnLSpB1+M3lnLSpB2+M3lnLSpB3;
M3significance=TMath::Sqrt(-2*(M3lnLB-M3lnLSpB));
//M3significance=1.05;
cout<<"method3: sig (likelihood): "<<M3significance<<endl;


M3pull_Hist->Fill(M3pull);
M3Sig_Hist1_chi2->Fill(M3sig1);
M3Sig_Hist2_chi2->Fill(M3sig2);
M3Sig_Hist3_chi2->Fill(M3sig3);
M3Sig_Hist_chi2->Fill(M3sig);
M3Sig_Hist1_likelihood->Fill(M3significance1);
M3Sig_Hist2_likelihood->Fill(M3significance2);
M3Sig_Hist3_likelihood->Fill(M3significance3);
M3Sig_Hist_likelihood->Fill(M3significance);
}



if(i<iend-1)
 {
 delete M_Hist;
 delete M_Hist1;
 delete M_Hist2;
 delete M_Hist3;
if(exec1==1)
{
 delete M1M_Hist;
 delete M1M_Histbis;
}
if(exec2==1)
{
 delete M2M_Hist1;
 delete M2M_Hist2;
 delete M2M_Hist3;
 delete M2M_Hist1bis;
 delete M2M_Hist2bis;
 delete M2M_Hist3bis;
}
if(exec3==1)
{
 delete M3M_Hist;
 delete M3M_Hist1;
 delete M3M_Hist2;
 delete M3M_Hist3;
 delete M3M_Hist1bis;
 delete M3M_Hist2bis;
 delete M3M_Hist3bis;
}
 }

}
//************************************

if(exec1==1)
{
if(pull1==1)
{
M1Canvas_2->cd();
M1pullpar0_Hist->SetLineColor(kOrange);
M1pullpar1_Hist->SetLineColor(kGreen+2);
M1pullpar2_Hist->SetLineColor(kMagenta);
M1pullpar3_Hist->SetLineColor(kBlue-2);
M1pullpar0_Hist->Draw();
M1pullpar1_Hist->Draw("sames");
M1pullpar2_Hist->Draw("sames");
M1pullpar3_Hist->Draw("sames");
if(savefile==1) M1pullpar0_Hist->Write();
if(savefile==1) M1pullpar1_Hist->Write();
if(savefile==1) M1pullpar2_Hist->Write();
if(savefile==1) M1pullpar3_Hist->Write();
}
else
{
Canvas->cd();
M1pullpar0_Hist->Draw();
M1pullpar1_Hist->Draw();
M1pullpar2_Hist->Draw();
M1pullpar3_Hist->Draw();
if(savefile==1) M1pullpar0_Hist->Write();
if(savefile==1) M1pullpar1_Hist->Write();
if(savefile==1) M1pullpar2_Hist->Write();
if(savefile==1) M1pullpar3_Hist->Write();
}
M1Canvas_3->cd();
M1Sig_Hist_chi2->Draw();
if(savefile==1) M1Sig_Hist_chi2->Write();
M1Canvas_4->cd();
M1Sig_Hist_likelihood->Draw();
if(savefile==1) M1Sig_Hist_likelihood->Write();
}

if(exec2==1)
{
if(pull2==1)
{
M2Canvas_2_1->cd();
M2pull1par0_Hist->SetLineColor(kOrange);
M2pull1par1_Hist->SetLineColor(kGreen+2);
M2pull1par2_Hist->SetLineColor(kMagenta);
M2pull1par3_Hist->SetLineColor(kBlue-2);
M2pull1par0_Hist->Draw();
M2pull1par1_Hist->Draw("sames");
M2pull1par2_Hist->Draw("sames");
M2pull1par3_Hist->Draw("sames");
if(savefile==1) M2pull1par0_Hist->Write();
if(savefile==1) M2pull1par1_Hist->Write();
if(savefile==1) M2pull1par2_Hist->Write();
if(savefile==1) M2pull1par3_Hist->Write();
M2Canvas_2_2->cd();
M2pull2par0_Hist->SetLineColor(kOrange);
M2pull2par1_Hist->SetLineColor(kGreen+2);
M2pull2par2_Hist->SetLineColor(kMagenta);
M2pull2par3_Hist->SetLineColor(kBlue-2);
M2pull2par0_Hist->Draw();
M2pull2par1_Hist->Draw("sames");
M2pull2par2_Hist->Draw("sames");
M2pull2par3_Hist->Draw("sames");
if(savefile==1) M2pull2par0_Hist->Write();
if(savefile==1) M2pull2par1_Hist->Write();
if(savefile==1) M2pull2par2_Hist->Write();
if(savefile==1) M2pull2par3_Hist->Write();
M2Canvas_2_3->cd();
M2pull3par0_Hist->SetLineColor(kOrange);
M2pull3par1_Hist->SetLineColor(kGreen+2);
M2pull3par2_Hist->SetLineColor(kMagenta);
M2pull3par3_Hist->SetLineColor(kBlue-2);
M2pull3par0_Hist->Draw();
M2pull3par1_Hist->Draw("sames");
M2pull3par2_Hist->Draw("sames");
M2pull3par3_Hist->Draw("sames");
if(savefile==1) M2pull3par0_Hist->Write();
if(savefile==1) M2pull3par1_Hist->Write();
if(savefile==1) M2pull3par2_Hist->Write();
if(savefile==1) M2pull3par3_Hist->Write();
}
else
{
Canvas->cd();
M2pull1par0_Hist->Draw();
M2pull1par1_Hist->Draw();
M2pull1par2_Hist->Draw();
M2pull1par3_Hist->Draw();
if(savefile==1) M2pull1par0_Hist->Write();
if(savefile==1) M2pull1par1_Hist->Write();
if(savefile==1) M2pull1par2_Hist->Write();
if(savefile==1) M2pull1par3_Hist->Write();
Canvas->cd();
M2pull2par0_Hist->Draw();
M2pull2par1_Hist->Draw();
M2pull2par2_Hist->Draw();
M2pull2par3_Hist->Draw();
if(savefile==1) M2pull2par0_Hist->Write();
if(savefile==1) M2pull2par1_Hist->Write();
if(savefile==1) M2pull2par2_Hist->Write();
if(savefile==1) M2pull2par3_Hist->Write();
Canvas->cd();
M2pull3par0_Hist->Draw();
M2pull3par1_Hist->Draw();
M2pull3par2_Hist->Draw();
M2pull3par3_Hist->Draw();
if(savefile==1) M2pull3par0_Hist->Write();
if(savefile==1) M2pull3par1_Hist->Write();
if(savefile==1) M2pull3par2_Hist->Write();
if(savefile==1) M2pull3par3_Hist->Write();
}
if(sig2==1)
{
M2Canvas_3_1->cd();
M2Sig_Hist1_chi2->Draw();
if(savefile==1) M2Sig_Hist1_chi2->Write();
M2Canvas_3_2->cd();
M2Sig_Hist2_chi2->Draw();
if(savefile==1) M2Sig_Hist2_chi2->Write();
M2Canvas_3_3->cd();
M2Sig_Hist3_chi2->Draw();
if(savefile==1) M2Sig_Hist3_chi2->Write();
M2Canvas_4_1->cd();
M2Sig_Hist1_likelihood->Draw();
if(savefile==1) M2Sig_Hist1_likelihood->Write();
M2Canvas_4_2->cd();
M2Sig_Hist2_likelihood->Draw();
if(savefile==1) M2Sig_Hist2_likelihood->Write();
M2Canvas_4_3->cd();
M2Sig_Hist3_likelihood->Draw();
if(savefile==1) M2Sig_Hist3_likelihood->Write();
}
else
{
Canvas->cd();
M2Sig_Hist1_chi2->Draw();
if(savefile==1) M2Sig_Hist1_chi2->Write();
Canvas->cd();
M2Sig_Hist2_chi2->Draw();
if(savefile==1) M2Sig_Hist2_chi2->Write();
Canvas->cd();
M2Sig_Hist3_chi2->Draw();
if(savefile==1) M2Sig_Hist3_chi2->Write();
Canvas->cd();
M2Sig_Hist1_likelihood->Draw();
if(savefile==1) M2Sig_Hist1_likelihood->Write();
Canvas->cd();
M2Sig_Hist2_likelihood->Draw();
if(savefile==1) M2Sig_Hist2_likelihood->Write();
Canvas->cd();
M2Sig_Hist3_likelihood->Draw();
if(savefile==1) M2Sig_Hist3_likelihood->Write();
}
M2Canvas_3_0->cd();
M2Sig_Hist_chi2->Draw();
if(savefile==1) M2Sig_Hist_chi2->Write();
M2Canvas_4_0->cd();
M2Sig_Hist_likelihood->Draw();
if(savefile==1) M2Sig_Hist_likelihood->Write();
}

if(exec3==1)
{
if(pull3==1)
{
M3Canvas_2->cd();
M3pull_Hist->Draw();
if(savefile==1) M3pull_Hist->Write();
}
else
{
Canvas->cd();
M3pull_Hist->Draw();
if(savefile==1) M3pull_Hist->Write();
}
if(sig3==1)
{
M3Canvas_3_1->cd();
M3Sig_Hist1_chi2->Draw();
if(savefile==1) M3Sig_Hist1_chi2->Write();
M3Canvas_3_2->cd();
M3Sig_Hist2_chi2->Draw();
if(savefile==1) M3Sig_Hist2_chi2->Write();
M3Canvas_3_3->cd();
M3Sig_Hist3_chi2->Draw();
if(savefile==1) M3Sig_Hist3_chi2->Write();
M3Canvas_4_1->cd();
M3Sig_Hist1_likelihood->Draw();
if(savefile==1) M3Sig_Hist1_likelihood->Write();
M3Canvas_4_2->cd();
M3Sig_Hist2_likelihood->Draw();
if(savefile==1) M3Sig_Hist2_likelihood->Write();
M3Canvas_4_3->cd();
M3Sig_Hist3_likelihood->Draw();
if(savefile==1) M3Sig_Hist3_likelihood->Write();
}
else
{
Canvas->cd();
M3Sig_Hist1_chi2->Draw();
if(savefile==1) M3Sig_Hist1_chi2->Write();
Canvas->cd();
M3Sig_Hist2_chi2->Draw();
if(savefile==1) M3Sig_Hist2_chi2->Write();
Canvas->cd();
M3Sig_Hist3_chi2->Draw();
if(savefile==1) M3Sig_Hist3_chi2->Write();
Canvas->cd();
M3Sig_Hist1_likelihood->Draw();
if(savefile==1) M3Sig_Hist1_likelihood->Write();
Canvas->cd();
M3Sig_Hist2_likelihood->Draw();
if(savefile==1) M3Sig_Hist2_likelihood->Write();
Canvas->cd();
M3Sig_Hist3_likelihood->Draw();
if(savefile==1) M3Sig_Hist3_likelihood->Write();
}
M3Canvas_3_0->cd();
M3Sig_Hist_chi2->Draw();
if(savefile==1) M3Sig_Hist_chi2->Write();
M3Canvas_4_0->cd();
M3Sig_Hist_likelihood->Draw();
if(savefile==1) M3Sig_Hist_likelihood->Write();
}

}




void CalcSignificance(int xmin,int xmax,double mean,double sigma,int f, int binwidth,TH1D* pData, TF1* bkg, TF1* sigbkg, TF1 * sig, double&Significance, double &lnLB, double &lnLSpB)
{

int NBin=pData->GetNbinsX();
int *BinCenter=new int[NBin];
for(int i=0;i<NBin;i++)
{
BinCenter[i]=pData->GetBinCenter(i+1);
}
int *BinContent=new int[NBin];
for(int i=0;i<NBin;i++)
{
BinContent[i]=pData->GetBinContent(i+1);
}
double *BkgContent=new double[NBin];
for(int i=0;i<NBin;i++)
{
BkgContent[i]=bkg->Eval(pData->GetBinCenter(i+1));
}
double *SigContent=new double[NBin];
for(int i=0;i<NBin;i++)
{
SigContent[i]=sig->Eval(pData->GetBinCenter(i+1));
}
double *SigBkgContent=new double[NBin];
for(int i=0;i<NBin;i++)
{
SigBkgContent[i]=sigbkg->Eval(pData->GetBinCenter(i+1));
}


   double lnLB = 0;

    for (int i = 1; i < NBin+1; i++){

     //  double mass = pData->GetBinCenter(i);
      // double w = pData->GetBinWidth(i);
      // int N = abs(pData->GetBinContent(i));
     //  double mu = bkg->Eval(mass);
	double mass=BinCenter[i-1];
	int N=BinContent[i-1];
	double mu=BkgContent[i-1];

	double Pi = -mu + N*log(mu);
      // double Pi = -mu + N*log(mu)-log(TMath::Factorial(N));
       //      cout << "N = " << N << " B = " << B << " S = " << S << "Pi = " << Pi << "" << endl;

	lnLB += Pi;

     }

cout<<"lnLB: "<<lnLB<<endl;


    double lnLSpB = 0;

    for (int i = 1; i < NBin+1; i++){

      // double mass = pData->GetBinCenter(i);
      // double w = pData->GetBinWidth(i);
      // int N = abs(pData->GetBinContent(i));
      // double mu = sigbkg->Eval(mass);
	double mass=BinCenter[i-1];
	int N=BinContent[i-1];
	double mu=SigBkgContent[i-1];
       double Pi = -mu + N*log(mu);
      // double Pi = -mu + N*log(mu)-log(TMath::Factorial(N));
       //      cout << "N = " << N << " B = " << B << " S = " << S << "Pi = " << Pi << "" << endl;

       lnLSpB += Pi;

     }


cout<<"lnLSpLB: "<<lnLSpB<<endl;

    if(lnLB<lnLSpB) Significance = TMath::Sqrt(-2*( lnLB - lnLSpB));
    else Significance=0;

 // cout<<"significance method1:"<<Significance<<endl;


double Significance2;
    for (int i = 1; i < NBin+1; i++){

      // double mass = pData->GetBinCenter(i);
      // double w = pData->GetBinWidth(i);
      // int N = abs(pData->GetBinContent(i));
      // double muSpB = sigbkg->Eval(mass);
      // double muS = sig->Eval(mass);
      // double muB = bkg->Eval(mass);
	double mass=BinCenter[i-1];
	int N=BinContent[i-1];
	double muB=BkgContent[i-1];
	double muSpB=SigBkgContent[i-1];
	double muS=SigContent[i-1];

      Significance2 += N*log(muB/muSpB)+muS;

     }

Significance2=TMath::Sqrt(-2*Significance2);
 // cout<<"significance method2:"<<Significance2<<endl;



}


