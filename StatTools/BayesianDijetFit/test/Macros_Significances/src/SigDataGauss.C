


// code used to  scan the significance over the masses using a gaussian function description for signal
// for sqrt(s)= 8 TeV only, else need to change 8000--> CollisionMass in all background and signal plus background functions


// 1) choose the file to open (filenumber), the histogram must be DeltaEta VS mass or mass VS DeltaEta
//     =1 --> 3.9 fb-1
//     =2 --> 5.6 fb-1
//     =3 --> 7.0 fb-1
//     the number of bins along the mass axis must be divisible by 10
//     here, the produced histograms binning is 10 GeV, also creates 100 GeV binning histograms (Rebin) which is used only for fit rediduals display
//     if one take another histogram, make sure that the projection is done on the good axis and if it is needed to rebin it
// 2) choose the range of deltaeta and the intermediate bins (Detastart,Detaend,Detacut1,Detacut2) using the bin number (the counting starts from 1 for the 1st bin)
//    also change titles and legends
// 3) choose which method to apply:
//    method1: total histogram with all bins of DeltaEta (exec1=1)
//    method2: histograms for each DeltaEta bin independently (exec2=1)
//    method3: histograms for each DeltaEta bin with a constrain on the signal (exec3=1)
// 4) choose one want to save the histograms and canvas in root files (method1 --> savefile1=1 , method2 --> savefile2=1 , method3 --> savefile3=1 )
// 5) if one want to calculate the P-Value --> iPValue=1  (but for a given mass only, else it replaces the histogram)
//    choose the number of pseudo experiments one want to generate: iendPValue
//    to save the PValue histogram: savefilePValue=1
//    the range (xmin and xmax) of the generated histogram must be the same than for data
// 6) some additional plots or displays on screen: iprint=1
// 7) define the signal ratio r1 , r2, r3  (here: isotropic decay description) for each DeltaEta bin
// 8) define the range for fit and function descritpion (xmin, xmax)
//    if the pvalue is calculated, xmin and xmax must be divisible by the binwidth, else it can be any real values
// 9) define the number of values to calculate the significance (Nvalues)
//10) Calculation of significance for a given mass only --> meantest= given mass , else put 0. to scan over all Nvalues masses 



void SigDataGauss()
{

// variables definition and initialization

int filenumber=3;
int Detastart=1; // this is the bin number
int Detacut1=5; // last bin for the 1st region
int Detacut2=10; // last bin for the 2nd region
int Detaend=13;


int savefile1=0; //method1
int savefile2=0; //method2
int savefile3=0; //method3

int exec1=1; //method1
int exec2=1; //method2
int exec3=1; //method3
int iPValue=0; //generate pseudo experiment and calculate pvalue --> iPValue==1
int iendPValue=10000; //number of pseudo experiments
int savefilePValue=0; // to save the PValue plot -->savefilePValue=1
int iprint=0;
double r1=0.436786; //ratio for signal
double r2=0.375497;
double r3=0.187717;
double meantest=0.; // scan over all masses --> meantest=0. else choose the mass you want
double xmin=890.; // range for the input histograms and fits
double xmax=4600;

int Nvalues=50; // number of vaulues to calculate the significance

//*******************************************************************************

// first part: display of data histograms


if(filenumber==1)
{
TFile *file0=TFile::Open("histograms_data_190456_195947_deltaeta_mass_ak5.root");
TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin10GeV;1");
}

if(filenumber==2)
{
TFile *file0=TFile::Open("histograms_delta_Mass_ak5.root");
TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat;1");
}

if(filenumber==3)
{
TFile *file0=TFile::Open("histograms_data_7lumi_deltaeta_mass.root");
TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin1GeV;1");
}

 gStyle->SetOptFit(111); 


  TCanvas *Canvas = new TCanvas("Canvas","Canvas",11,51,700,500);
if(filenumber==1)
{
  h_DEta_Mass->SetTitle("#Delta#eta VS Invariant Mass for data");
  h_DEta_Mass->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  h_DEta_Mass->GetXaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetXaxis()->SetTitleOffset(1.4);
  h_DEta_Mass->GetYaxis()->SetTitle("#Delta#eta");
  h_DEta_Mass->GetYaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetYaxis()->SetTitleOffset(1.6);
}
if(filenumber==2 || filenumber==3)
{
  h_DEta_Mass->SetTitle("Invariant Mass VS #Delta#eta for data");
  h_DEta_Mass->GetYaxis()->SetTitle("Invariant Mass (GeV)");
  h_DEta_Mass->GetYaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetYaxis()->SetTitleOffset(1.4);
  h_DEta_Mass->GetXaxis()->SetTitle("#Delta#eta");
  h_DEta_Mass->GetXaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetXaxis()->SetTitleOffset(1.6);
}
  h_DEta_Mass->GetZaxis()->SetTitle("number of events");
  h_DEta_Mass->GetZaxis()->SetTitleSize(0.04);
  h_DEta_Mass->GetZaxis()->SetTitleOffset(1.4);
  TLegend* leg = new TLegend(0.3, 0.25, .4, .3);


  TH1D* hist_mass = new TH1D();
  hist_mass->Sumw2();
  TCanvas *Canvas_0 = new TCanvas("Canvas_0","Canvas_0",11,51,700,500);
  TLegend* leg0 = new TLegend(0.4, 0.4, .6, .45);


  TH1D* hist_mass1 = new TH1D();
  hist_mass1->Sumw2();
  TCanvas *Canvas_1 = new TCanvas("Canvas_1","Canvas_1",11,51,700,500);
  TLegend* leg1 = new TLegend(0.4, 0.4, .6, .45);


  TH1D* hist_mass2 = new TH1D();
  hist_mass2->Sumw2();
  TCanvas *Canvas_2 = new TCanvas("Canvas_2","Canvas_2",11,51,700,500);
  TLegend* leg2 = new TLegend(0.4, 0.4, .6, .45);

  TH1D* hist_mass3 = new TH1D();
  hist_mass3->Sumw2();
  TCanvas *Canvas_3 = new TCanvas("Canvas_3","Canvas_3",11,51,700,500);
  TLegend* leg3 = new TLegend(0.4, 0.4, .6, .45);



if(filenumber==1)
{
hist_mass=h_DEta_Mass->ProjectionX("hist_mass",Detastart,Detaend,"e");
hist_mass1=h_DEta_Mass->ProjectionX("hist_mass1",Detastart,Detacut1,"e");
hist_mass2=h_DEta_Mass->ProjectionX("hist_mass2",Detacut1+1,Detacut2,"e");
hist_mass3=h_DEta_Mass->ProjectionX("hist_mass3",Detacut2+1,Detaend,"e");
}

if(filenumber==2 || filenumber==3)
{

hist_mass=h_DEta_Mass->ProjectionY("hist_mass",Detastart,Detaend,"e");
hist_mass1=h_DEta_Mass->ProjectionY("hist_mass1",Detastart,Detacut1,"e");
hist_mass2=h_DEta_Mass->ProjectionY("hist_mass2",Detacut1+1,Detacut2,"e");
hist_mass3=h_DEta_Mass->ProjectionY("hist_mass3",Detacut2+1,Detaend,"e");
}


if(filenumber==2 || filenumber==3)
{
hist_mass->Rebin(10);
hist_mass1->Rebin(10);
hist_mass2->Rebin(10);
hist_mass3->Rebin(10);
}

  hist_mass->SetTitle("Invariant Mass with 0<#Delta#eta<1.3");
  hist_mass->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass->GetXaxis()->SetTitleOffset(1.1);
  hist_mass->GetYaxis()->SetTitle("number of events");
  hist_mass->GetYaxis()->SetTitleOffset(1.5);
  hist_mass1->SetTitle("Invariant Mass with 0<#Delta#eta<0.5");
  hist_mass1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass1->GetXaxis()->SetTitleOffset(1.1);
  hist_mass1->GetYaxis()->SetTitle("number of events");
  hist_mass1->GetYaxis()->SetTitleOffset(1.5);
  hist_mass2->SetTitle("Invariant Mass with 0.5<#Delta#eta<1");
  hist_mass2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass2->GetXaxis()->SetTitleOffset(1.1);
  hist_mass2->GetYaxis()->SetTitle("number of events");
  hist_mass2->GetYaxis()->SetTitleOffset(1.5);
  hist_mass3->SetTitle("Invariant Mass with 1<#Delta#eta<1.3");
  hist_mass3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass3->GetXaxis()->SetTitleOffset(1.1);
  hist_mass3->GetYaxis()->SetTitle("number of events");
  hist_mass3->GetYaxis()->SetTitleOffset(1.5);


  TH1D*  hist_massRebin = (TH1D*) hist_mass->Clone("hist_massRebin");
  TH1D*  hist_mass1Rebin = (TH1D*) hist_mass1->Clone("hist_mass1Rebin");
  TH1D*  hist_mass2Rebin = (TH1D*) hist_mass2->Clone("hist_mass2Rebin");
  TH1D*  hist_mass3Rebin = (TH1D*) hist_mass3->Clone("hist_mass3Rebin");

hist_massRebin->Rebin(10);
hist_mass1Rebin->Rebin(10);
hist_mass2Rebin->Rebin(10);
hist_mass3Rebin->Rebin(10);


Double_t Ntot=hist_mass->GetEntries();
Double_t N1=hist_mass1->GetEntries();
Double_t N2=hist_mass2->GetEntries();
Double_t N3=hist_mass3->GetEntries();

cout<<"comparing the number of events"<<endl;
cout<<"for 0<DeltaEta<0.5: "<<N1/Ntot*100<<" %"<<endl;
cout<<"for 0.5<DeltaEta<1: "<<N2/Ntot*100<<" %"<<endl;
cout<<"for 1<DeltaEta<1.3: "<<N3/Ntot*100<<" %"<<endl;
cout<<"sum="<<N1/Ntot*100+N2/Ntot*100+N3/Ntot*100<<endl;


 Canvas->cd();
 leg->AddEntry(h_DEta_Mass," data","l");
 h_DEta_Mass->Draw("surf2");
 leg->Draw();
 Canvas_0->cd();
 hist_mass->SetLineColor(kBlue-2);
 leg0->AddEntry(hist_mass,"data   0<#Delta#eta<1.3","l");
 hist_mass->Draw();
 leg0->Draw();
 Canvas_1->cd();
 hist_mass1->SetLineColor(kBlue-2);
 leg1->AddEntry(hist_mass1,"data   0<#Delta#eta<0.5","l");
 hist_mass1->Draw();
 leg1->Draw();
 Canvas_2->cd();
 hist_mass2->SetLineColor(kBlue-2);
 leg2->AddEntry(hist_mass2,"data   0.5<#Delta#eta<1","l"); 
 hist_mass2->Draw();
 leg2->Draw();
 Canvas_3->cd();
 hist_mass3->SetLineColor(kBlue-2);
 leg3->AddEntry(hist_mass3,"data   1<#Delta#eta<1.3","l");
 hist_mass3->Draw();
 leg3->Draw();


// *************************************************************************

// second part: fit and significance calculation


  if(savefile1==1) TFile *outputfileM1Mass = new TFile("M1MassSig.root","recreate");
  if(savefile2==1) TFile *outputfileM2Mass1 = new TFile("M2MassSig1.root","recreate");
  if(savefile2==1) TFile *outputfileM2Mass2 = new TFile("M2MassSig2.root","recreate");
  if(savefile2==1) TFile *outputfileM2Mass3 = new TFile("M2MassSig3.root","recreate");
  if(savefile3==1) TFile *outputfileM3Mass1 = new TFile("M3MassSig1.root","recreate");
  if(savefile3==1) TFile *outputfileM3Mass2 = new TFile("M3MassSig2.root","recreate");
  if(savefile3==1) TFile *outputfileM3Mass3 = new TFile("M3MassSig3.root","recreate");
  if(savefile1==1) TFile *outputfileM1Pull = new TFile("M1PullSig.root","recreate");
  if(savefile2==1) TFile *outputfileM2Pull1 = new TFile("M2PullSig1.root","recreate");
  if(savefile2==1) TFile *outputfileM2Pull2 = new TFile("M2PullSig2.root","recreate");
  if(savefile2==1) TFile *outputfileM2Pull3 = new TFile("M2PullSig3.root","recreate");
  if(savefile3==1) TFile *outputfileM3Pull1 = new TFile("M3PullSig1.root","recreate");
  if(savefile3==1) TFile *outputfileM3Pull2 = new TFile("M3PullSig2.root","recreate");
  if(savefile3==1) TFile *outputfileM3Pull3 = new TFile("M3PullSig3.root","recreate");
  if(savefile1==1) TFile *outputfileM1PullRebin = new TFile("M1PullRebinSig.root","recreate");
  if(savefile2==1) TFile *outputfileM2Pull1Rebin = new TFile("M2PullRebinSig1.root","recreate");
  if(savefile2==1) TFile *outputfileM2Pull2Rebin = new TFile("M2PullRebinSig2.root","recreate");
  if(savefile2==1) TFile *outputfileM2Pull3Rebin = new TFile("M2PullRebinSig3.root","recreate");
  if(savefile3==1) TFile *outputfileM3Pull1Rebin = new TFile("M3PullRebinSig1.root","recreate");
  if(savefile3==1) TFile *outputfileM3Pull2Rebin = new TFile("M3PullRebinSig2.root","recreate");
  if(savefile3==1) TFile *outputfileM3Pull3Rebin = new TFile("M3PullRebinSig3.root","recreate");
  if(savefile1==1) TFile *outputfileM1Sig = new TFile("M1SigSig.root","recreate");
  if(savefile2==1) TFile *outputfileM2Sig = new TFile("M2SigSig.root","recreate");
  if(savefile3==1) TFile *outputfileM3Sig = new TFile("M3SigSig.root","recreate");



int NBin=hist_mass->GetNbinsX();
double minHist=hist_mass->GetXaxis()->GetBinLowEdge(1);
double maxHist=hist_mass->GetXaxis()->GetBinUpEdge(NBin);
int BinWidth=(maxHist-minHist)/NBin;
if(meantest!=0) Nvalues=1;
if(meantest!=0) iprint=1;
double mean;
double sigma;
// method1
double M1Significance;
double *M1significance=new double[Nvalues];
double M1lnLB;
double M1lnLSpB;
double *x1=new double[Nvalues];
double *y1=new double[Nvalues];
// method2
double M2Significance1;
double M2Significance2;
double M2Significance3;
double *M2significance=new double[Nvalues];
double *M2significance1=new double[Nvalues];
double *M2significance2=new double[Nvalues];
double *M2significance3=new double[Nvalues];
double M2lnLB;
double M2lnLSpB;
double M2lnLB1;
double M2lnLSpB1;
double M2lnLB2;
double M2lnLSpB2;
double M2lnLB3;
double M2lnLSpB3;
double *x2=new double[Nvalues];
double *y2=new double[Nvalues];
// method3
double M3Significance1;
double M3Significance2;
double M3Significance3;
double *M3significance=new double[Nvalues];
double *M3significance1=new double[Nvalues];
double *M3significance2=new double[Nvalues];
double *M3significance3=new double[Nvalues];
double M3lnLB;
double M3lnLSpB;
double M3lnLB1;
double M3lnLSpB1;
double M3lnLB2;
double M3lnLSpB2;
double M3lnLB3;
double M3lnLSpB3;
double *x3=new double[Nvalues];
double *y3=new double[Nvalues];
double norm0;
double norm1;
double norm2;
double norm3;

// *************************************************************************

if(exec1==1)
{
  TH1D*  M1M_Hist = (TH1D*) hist_mass->Clone("M1M_Hist");
  M1M_Hist->SetTitle("Method 1: Invariant Mass (GeV) for 0<#Delta#eta<1.3");
  M1M_Hist->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M1M_Hist->GetXaxis()->SetTitleOffset(1.1);
  M1M_Hist->GetYaxis()->SetTitle("number of events");
  TH1D*  M1M_Histbis = (TH1D*) M1M_Hist->Clone("M1M_Histbis");
  TCanvas *M1Canvas = new TCanvas("M1Canvas","M1Canvas",11,51,700,500);
  TF1 *M1SigBkg= new TF1("M1SigBkg","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M1Bkg= new TF1("M1Bkg","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1 *M1FitBkg= new TF1("M1FitBkg","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))+[3]*0",xmin,xmax);
  TH1D*  M1M_HistRebin = (TH1D*) hist_massRebin->Clone("M1M_HistRebin");
  M1M_HistRebin->SetTitle("Method 1: Invariant Mass (GeV) for 0<#Delta#eta<1.3");
  M1M_HistRebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  TH1D*  M1M_HistRebinbis = (TH1D*) M1M_HistRebin->Clone("M1M_HistRebinbis");
  TCanvas *M1CanvasRebin = new TCanvas("M1CanvasRebin","M1CanvasRebin",11,51,700,500);
  TF1 *M1SigBkgRebin= new TF1("M1SigBkgRebin","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M1BkgRebin= new TF1("M1BkgRebin","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1 *M1FitBkgRebin= new TF1("M1FitBkgRebin","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))+[3]*0",xmin,xmax);

  TH1D* M1M_divHist = new TH1D("M1M_divHist","Method 1: fit residuals for 0<#Delta#eta<1.3",NBin,minHist,maxHist);
  M1M_divHist->Sumw2();
  TH1D* M1M_divHistSignal = new TH1D("M1M_divHistSignal","M1: signal",NBin,minHist,maxHist);
  M1M_divHistSignal->Sumw2();
  TCanvas *M1CanvasDiv = new TCanvas("M1CanvasDiv","M1CanvasDiv",11,51,700,500);
  TH1D* M1M_divHistRebin = new TH1D("M1M_divHistRebin","Method 1: fit residuals for 0<#Delta#eta<1.3",NBin/10,minHist,maxHist);
  M1M_divHistRebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M1M_divHistRebin->Sumw2();
  TH1D* M1M_divHistSignalRebin = new TH1D("M1M_divHistSignalRebin","M1: signal Rebin",NBin/10,minHist,maxHist);
  M1M_divHistSignalRebin->Sumw2();
  TCanvas *M1CanvasDivRebin = new TCanvas("M1CanvasDivRebin","M1CanvasDivRebin",11,51,700,500);
}

if(exec2==1)
{
  TH1D*  M2M_Hist1 = (TH1D*) hist_mass1->Clone("M2M_Hist1");
  M2M_Hist1->SetTitle("Method2 : Invariant Mass (GeV) for 0<#Delta#eta<0.5");
  M2M_Hist1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2M_Hist1->GetXaxis()->SetTitleOffset(1.1);
  M2M_Hist1->GetYaxis()->SetTitle("number of events");
  TCanvas *M2Canvas1 = new TCanvas("M2Canvas1","M2Canvas1",11,51,700,500);
  TF1 *M2SigBkg1= new TF1("M2SigBkg1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M2Bkg1= new TF1("M2Bkg1","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);

  TH1D* M2M_divHist1 = new TH1D("M2M_divHist1","Method 2: fit residuals for 0<#Delta#eta<0.5",NBin,minHist,maxHist);
  M2M_divHist1->Sumw2();
  TH1D* M2M_divHist1Signal = new TH1D("M2M_divHist1Signal","M1: Signal",NBin,minHist,maxHist);
  M2M_divHist1Signal->Sumw2();
  TCanvas *M2CanvasDiv1 = new TCanvas("M2CanvasDiv1","M2CanvasDiv1",11,51,700,500);
  TH1D* M2M_divHist1Rebin = new TH1D("M2M_divHist1Rebin","Method 2: fit residuals for 0<#Delta#eta<0.5",NBin/10,minHist,maxHist);
  M2M_divHist1Rebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2M_divHist1Rebin->Sumw2();
  TH1D* M2M_divHist1SignalRebin = new TH1D("M2M_divHist1SignalRebin","M2: Signal Rebin",NBin/10,minHist,maxHist);
  M2M_divHist1SignalRebin->Sumw2();
  TCanvas *M2CanvasDiv1Rebin = new TCanvas("M2CanvasDiv1Rebin","M2CanvasDiv1Rebin",11,51,700,500);

  TH1D*  M2M_Hist2 = (TH1D*) hist_mass2->Clone("M2M_Hist2");
  M2M_Hist2->SetTitle("Method2 : Invariant Mass (GeV) for 0.5<#Delta#eta<1");
  M2M_Hist2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2M_Hist2->GetXaxis()->SetTitleOffset(1.1);
  M2M_Hist2->GetYaxis()->SetTitle("number of events");
  TCanvas *M2Canvas2 = new TCanvas("M2Canvas2","M2Canvas2",11,51,700,500);
  TF1 *M2SigBkg2= new TF1("M2SigBkg2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M2Bkg2= new TF1("M2Bkg2","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);

  TH1D* M2M_divHist2 = new TH1D("M2M_divHist2","Method 2: fit residuals for 0.5<#Delta#eta<1",NBin,minHist,maxHist);
  M2M_divHist2->Sumw2();
  TH1D* M2M_divHist2Signal = new TH1D("M2M_divHist2Signal","M2: Signal",NBin,minHist,maxHist);
  M2M_divHist2Signal->Sumw2();
  TCanvas *M2CanvasDiv2 = new TCanvas("M2CanvasDiv2","M2CanvasDiv2",11,51,700,500);
  TH1D* M2M_divHist2Rebin = new TH1D("M2M_divHist2Rebin","Method 2: fit residuals for 0.5<#Delta#eta<1",NBin/10,minHist,maxHist);
  M2M_divHist2Rebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2M_divHist2Rebin->Sumw2();
  TH1D* M2M_divHist2SignalRebin = new TH1D("M2M_divHist2SignalRebin","M2: Signal Rebin",NBin/10,minHist,maxHist);
  M2M_divHist2SignalRebin->Sumw2();
  TCanvas *M2CanvasDiv2Rebin = new TCanvas("M2CanvasDiv2Rebin","M2CanvasDiv2Rebin",11,51,700,500);

  TH1D*  M2M_Hist3 = (TH1D*) hist_mass3->Clone("M2M_Hist3");
  M2M_Hist3->SetTitle("Method2 : Invariant Mass (GeV) for 1<#Delta#eta<1.3");
  M2M_Hist3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2M_Hist3->GetXaxis()->SetTitleOffset(1.1);
  M2M_Hist3->GetYaxis()->SetTitle("number of events");
  TCanvas *M2Canvas3 = new TCanvas("M2Canvas3","M2Canvas3",11,51,700,500);
  TF1 *M2SigBkg3= new TF1("M2SigBkg3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M2Bkg3= new TF1("M2Bkg3","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);

  TH1D* M2M_divHist3 = new TH1D("M2M_divHist3","Method 2: fit residuals for 1<#Delta#eta<1.3",NBin,minHist,maxHist);
  M2M_divHist3->Sumw2();
  TH1D* M2M_divHist3Signal = new TH1D("M2M_divHist3Signal","M2: Signal",NBin,minHist,maxHist);
  M2M_divHist3Signal->Sumw2();
  TCanvas *M2CanvasDiv3 = new TCanvas("M2CanvasDiv3","M2CanvasDiv3",11,51,700,500);
  TH1D* M2M_divHist3Rebin = new TH1D("M2M_divHist3Rebin","Method 2: fit residuals for 1<#Delta#eta<1.3",NBin/10,minHist,maxHist);
  M2M_divHist3Rebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2M_divHist3Rebin->Sumw2();
  TH1D* M2M_divHist3SignalRebin = new TH1D("M2M_divHist3SignalRebin","M2: Signal Rebin",NBin/10,minHist,maxHist);
  M2M_divHist3SignalRebin->Sumw2();
  TCanvas *M2CanvasDiv3Rebin = new TCanvas("M2CanvasDiv3Rebin","M2CanvasDiv3Rebin",11,51,700,500);
}


if(exec3==1)
{
  TH1D*  M3M_Hist = (TH1D*) hist_mass->Clone("M3M_Hist");
  M3M_Hist->SetTitle("Method3 : Invariant Mass (GeV) for 0<#Delta#eta<1.3");
  M3M_Hist->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_Hist->GetXaxis()->SetTitleOffset(1.1);
  M3M_Hist->GetYaxis()->SetTitle("number of events");
  TCanvas *M3Canvas0 = new TCanvas("M3Canvas0","M3Canvas0",11,51,700,500);
  TF1 *M3SigBkg0= new TF1("M3SigBkg0","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);

  TH1D*  M3M_Hist1 = (TH1D*) hist_mass1->Clone("M3M_Hist1");
  M3M_Hist1->SetTitle("Method3 : Invariant Mass (GeV) for 0<#Delta#eta<0.5");
  M3M_Hist1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_Hist1->GetXaxis()->SetTitleOffset(1.1);
  M3M_Hist1->GetYaxis()->SetTitle("number of events");
  TCanvas *M3Canvas1 = new TCanvas("M3Canvas1","M3Canvas1",11,51,700,500);
  TF1 *M3SigBkg1= new TF1("M3SigBkg1","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M3Bkg1= new TF1("M3Bkg1","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);

  TH1D* M3M_divHist1 = new TH1D("M3M_divHist1","Method 3: fit residuals for 0<#Delta#eta<0.5",NBin,minHist,maxHist);
  M3M_divHist1->Sumw2();
  TH1D* M3M_divHist1Signal = new TH1D("M3M_divHist1Signal","M3: Signal",NBin,minHist,maxHist);
  M3M_divHist1Signal->Sumw2();
  TCanvas *M3CanvasDiv1 = new TCanvas("M3CanvasDiv1","M3CanvasDiv1",11,51,700,500);
  TH1D* M3M_divHist1Rebin = new TH1D("M3M_divHist1Rebin","Method 3: fit residuals for 0<#Delta#eta<0.5",NBin/10,minHist,maxHist);
  M3M_divHist1Rebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_divHist1Rebin->Sumw2();
  TH1D* M3M_divHist1SignalRebin = new TH1D("M3M_divHist1SignalRebin","M3: Signal Rebin",NBin/10,minHist,maxHist);
  M3M_divHist1SignalRebin->Sumw2();
  TCanvas *M3CanvasDiv1Rebin = new TCanvas("M3CanvasDiv1Rebin","M3CanvasDiv1Rebin",11,51,700,500);


  TH1D*  M3M_Hist2 = (TH1D*) hist_mass2->Clone("M3M_Hist2");
  M3M_Hist2->SetTitle("Method3 : Invariant Mass (GeV) for 0.5<#Delta#eta<1");
  M3M_Hist2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_Hist2->GetXaxis()->SetTitleOffset(1.1);
  M3M_Hist2->GetYaxis()->SetTitle("number of events");
  TCanvas *M3Canvas2 = new TCanvas("M3Canvas2","M3Canvas2",11,51,700,500);
  TF1 *M3SigBkg2= new TF1("M3SigBkg2","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M3Bkg2= new TF1("M3Bkg2","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);

  TH1D* M3M_divHist2 = new TH1D("M3M_divHist2","Method 3: fit residuals for 0.5<#Delta#eta<1",NBin,minHist,maxHist);
  M3M_divHist2->Sumw2();
  TH1D* M3M_divHist2Signal = new TH1D("M3M_divHist2Signal","M3: Signal",NBin,minHist,maxHist);
  M3M_divHist2Signal->Sumw2();
  TCanvas *M3CanvasDiv2 = new TCanvas("M3CanvasDiv2","M3CanvasDiv2",11,51,700,500);
  TH1D* M3M_divHist2Rebin = new TH1D("M3M_divHist2Rebin","Method 3: fit residuals for 0.5<#Delta#eta<1",NBin/10,minHist,maxHist);
  M3M_divHist2Rebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_divHist2Rebin->Sumw2();
  TH1D* M3M_divHist2SignalRebin = new TH1D("M3M_divHist2SignalRebin","M3: Signal Rebin",NBin/10,minHist,maxHist);
  M3M_divHist2SignalRebin->Sumw2();
  TCanvas *M3CanvasDiv2Rebin = new TCanvas("M3CanvasDiv2Rebin","M3CanvasDiv2Rebin",11,51,700,500);


  TH1D*  M3M_Hist3 = (TH1D*) hist_mass3->Clone("M3M_Hist3");
  M3M_Hist3->SetTitle("Method3 : Invariant Mass (GeV) for 1<#Delta#eta<1.3");
  M3M_Hist3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_Hist3->GetXaxis()->SetTitleOffset(1.1);
  M3M_Hist3->GetYaxis()->SetTitle("number of events");
  TCanvas *M3Canvas3 = new TCanvas("M3Canvas3","M3Canvas3",11,51,700,500);
  TF1 *M3SigBkg3= new TF1("M3SigBkg3","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))",xmin,xmax);
  TF1 *M3Bkg3= new TF1("M3Bkg3","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);

  TH1D* M3M_divHist3 = new TH1D("M3M_divHist3","Method 3: fit residuals for 1<#Delta#eta<1.3",NBin,minHist,maxHist);
  M3M_divHist3->Sumw2();
  TH1D* M3M_divHist3Signal = new TH1D("M3M_divHist3Signal","M3: Signal",NBin,minHist,maxHist);
  M3M_divHist3Signal->Sumw2();
  TCanvas *M3CanvasDiv3 = new TCanvas("M3CanvasDiv3","M3CanvasDiv3",11,51,700,500);
  TH1D* M3M_divHist3Rebin = new TH1D("M3M_divHist3Rebin","Method 3: fit residuals for 1<#Delta#eta<1.3",NBin/10,minHist,maxHist);
  M3M_divHist3Rebin->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_divHist3Rebin->Sumw2();
  TH1D* M3M_divHist3SignalRebin = new TH1D("M3M_divHist3SignalRebin","M3: Signal Rebin",NBin/10,minHist,maxHist);
  M3M_divHist3SignalRebin->Sumw2();
  TCanvas *M3CanvasDiv3Rebin = new TCanvas("M3CanvasDiv3Rebin","M3CanvasDiv3Rebin",11,51,700,500);

cout<< M3M_Hist->Integral(0,800)<<endl;
cout<< M3M_Hist1->Integral(0,800)<<endl;
cout<< M3M_Hist2->Integral(0,800)<<endl;
cout<< M3M_Hist3->Integral(0,800)<<endl;
}

// **************************************************************************

for (int j=0;j<Nvalues;j++)
{
cout<<"value nb "<<j+1<<endl;

if(meantest==0) mean =xmin+(j+1)*(xmax-xmin)/(Nvalues+1);
else mean=meantest;
sigma=0.05*mean;
cout<<"mean: "<<mean<<endl;
cout<<"sigma: "<<sigma<<endl;
int imean=mean;

// ******************************************* method1

if(exec1==1)
{
  TLegend* M1LegMass =  new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M1LegRebin = new TLegend(0.3, 0.2, .5, .35);

M1SigBkg->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M1SigBkg->FixParameter(4,mean);
M1SigBkg->FixParameter(5,sigma);

M1M_Hist->SetName(Form("M1M_Hist_%d",imean));
M1Canvas->cd();
gPad->SetLogy();
M1SigBkg->SetLineColor(kRed);
M1M_Hist->Fit("M1SigBkg","L","",xmin,xmax);

M1Bkg->FixParameter(0,M1SigBkg->GetParameter(0));
M1Bkg->FixParameter(1,M1SigBkg->GetParameter(1));
M1Bkg->FixParameter(2,M1SigBkg->GetParameter(2));
M1FitBkg->SetLineColor(kGreen);
M1FitBkg->FixParameter(0,M1SigBkg->GetParameter(0));
M1FitBkg->FixParameter(1,M1SigBkg->GetParameter(1));
M1FitBkg->FixParameter(2,M1SigBkg->GetParameter(2));
M1M_Histbis->Fit("M1FitBkg","L","",xmin,xmax);
M1M_Hist->Draw();
M1FitBkg->Draw("same");
//cout<<" no rebin: "<<M1FitBkg->GetChisquare()<<endl;


M1SigBkgRebin->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M1SigBkgRebin->FixParameter(4,mean);
M1SigBkgRebin->FixParameter(5,sigma);

M1M_HistRebin->SetName(Form("M1M_HistRebin_%d",imean));
M1CanvasRebin->cd();
gPad->SetLogy();
M1SigBkgRebin->SetLineColor(kRed);
M1M_HistRebin->Fit("M1SigBkgRebin","","",xmin,xmax);

M1BkgRebin->FixParameter(0,M1SigBkgRebin->GetParameter(0));
M1BkgRebin->FixParameter(1,M1SigBkgRebin->GetParameter(1));
M1BkgRebin->FixParameter(2,M1SigBkgRebin->GetParameter(2));
M1FitBkgRebin->SetLineColor(kGreen);
M1FitBkgRebin->FixParameter(0,M1SigBkgRebin->GetParameter(0));
M1FitBkgRebin->FixParameter(1,M1SigBkgRebin->GetParameter(1));
M1FitBkgRebin->FixParameter(2,M1SigBkgRebin->GetParameter(2));
M1M_HistRebinbis->Fit("M1FitBkgRebin","","",xmin,xmax);
M1M_HistRebin->Draw();
M1FitBkgRebin->Draw("same");
//cout<<"rebin: "<<M1FitBkgRebin->GetChisquare()<<endl;

/*
for(int k=1; k<80;k++)
{
cout<<" bin: "<<k<<endl;
cout<<"histo: "<<M1M_HistRebin->GetBinContent(k)<<endl;
cout<<"integral: "<<(M1FitBkgRebin->Integral(M1M_HistRebin->GetBinLowEdge(k),M1M_HistRebin->GetBinLowEdge(k+1)))/100.<<endl;
cout<<"value at bin center: "<<M1FitBkgRebin->Eval(M1M_HistRebin->GetBinCenter(k))<<endl;
}
*/

if(savefile1==1) outputfileM1Mass->cd();
//if(savefile1==1) M1M_Hist->Write();
if(savefile1==1) M1Canvas->Write(Form("M1M_Hist_%d",imean));

if(iprint==1)
{
cout<<"method1 SigBkg par0: "<<M1SigBkg->GetParameter(0)<<endl;
cout<<"method1 SigBkg par1: "<<M1SigBkg->GetParameter(1)<<endl;
cout<<"method1 SigBkg par2: "<<M1SigBkg->GetParameter(2)<<endl;
cout<<"method1 SigBkg par3: "<<M1SigBkg->GetParameter(3)<<endl;
cout<<"method1 SigBkg par4: "<<M1SigBkg->GetParameter(4)<<endl;
cout<<"method1 SigBkg par5: "<<M1SigBkg->GetParameter(5)<<endl;
cout<<"method1 Bkg par0: "<<M1Bkg->GetParameter(0)<<endl;
cout<<"method1 Bkg par1: "<<M1Bkg->GetParameter(1)<<endl;
cout<<"method1 Bkg par2: "<<M1Bkg->GetParameter(2)<<endl;
}

if(M1SigBkg->GetParameter(3)<0) {
M1significance[j]=0;
M1lnLB=0;
M1lnLSpB=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass, M1Bkg, M1SigBkg, M1Significance, M1lnLB,M1lnLSpB);
M1significance[j]=M1Significance;
}
cout<<"M1significance nb "<<j+1<<" (mass = "<<mean<<") : "<<M1significance[j]<<endl;


M1CanvasDiv->cd();
M1M_divHist->SetLineColor(kBlack);
M1M_divHist->SetLineWidth(2);
M1M_divHist->SetMarkerStyle(20);
M1M_divHist->SetMarkerColor(kBlack);
M1M_divHistSignal->SetLineColor(kRed);
M1M_divHistSignal->SetLineWidth(2);
M1M_divHistSignal->SetMarkerStyle(21);
M1M_divHistSignal->SetMarkerSize(0.7);
M1M_divHistSignal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass, M1Bkg,M1SigBkg,M1M_divHist,M1M_divHistSignal);
M1M_divHist->SetName(Form("M1M_divHist_%d",imean));
M1M_divHist->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M1M_divHist->Draw();
M1M_divHistSignal->Draw("samehist");
if(savefile1==1) outputfileM1Pull->cd();
//if(savefile1==1) M1M_divHist->Write();
if(savefile1==1) M1CanvasDiv->Write(Form("M1M_divHist_%d",imean));

M1CanvasDivRebin->cd();
M1M_divHistRebin->SetLineColor(kBlack);
M1M_divHistRebin->SetLineWidth(2);
M1M_divHistRebin->SetMarkerStyle(20);
M1M_divHistRebin->SetMarkerColor(kBlack);
M1M_divHistSignalRebin->SetLineColor(kRed);
M1M_divHistSignalRebin->SetLineWidth(2);
M1M_divHistSignalRebin->SetMarkerStyle(21);
M1M_divHistSignalRebin->SetMarkerSize(0.7);
M1M_divHistSignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass, hist_massRebin, M1Bkg,M1SigBkg,M1M_divHistRebin,M1M_divHistSignalRebin);
M1M_divHistRebin->SetName(Form("M1M_divHistRebin_%d",imean));
M1M_divHistRebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M1M_divHistRebin->SetMinimum(-4);
M1M_divHistRebin->SetMaximum(4);
M1M_divHistRebin->Draw();
M1M_divHistSignalRebin->Draw("samehist");
M1LegRebin->AddEntry((TObject*)0,"","");
M1LegRebin->AddEntry(M1M_divHistRebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M1LegRebin->AddEntry((TObject*)0,"","");
M1LegRebin->AddEntry(M1M_divHistSignalRebin,"#frac{Signal}{Error(data)}","l");
M1LegRebin->AddEntry((TObject*)0,"","");
M1LegRebin->Draw();
if(savefile1==1) outputfileM1PullRebin->cd();
//if(savefile1==1) M1M_divHistRebin->Write();
if(savefile1==1) M1CanvasDivRebin->Write(Form("M1M_divHistRebin_%d",imean));




if(iPValue==1) PseudoExp(xmin,xmax,BinWidth, Ntot, iendPValue,savefilePValue, M1Bkg, mean, sigma,M1significance[j]);

}//exec1

// *********************************** method2


if(exec2==1)
{
  TLegend* M2LegMass1 = new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M2LegRebin1 = new TLegend(0.3, 0.2, .5, .35);
  TLegend* M2LegMass2 =  new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M2LegRebin2 = new TLegend(0.3, 0.2, .5, .35);
  TLegend* M2LegMass3 =  new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M2LegRebin3 = new TLegend(0.3, 0.2, .5, .35);


M2SigBkg1->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M2SigBkg1->FixParameter(4,mean);
M2SigBkg1->FixParameter(5,sigma);

M2M_Hist1->SetName(Form("M2M_Hist1_%d",imean));
M2Canvas1->cd();
gPad->SetLogy();
M2SigBkg1->SetLineColor(kRed);
M2M_Hist1->Fit("M2SigBkg1","L","",xmin,xmax);

M2Bkg1->FixParameter(0,M2SigBkg1->GetParameter(0));
M2Bkg1->FixParameter(1,M2SigBkg1->GetParameter(1));
M2Bkg1->FixParameter(2,M2SigBkg1->GetParameter(2));
M2Bkg1->SetLineColor(kGreen);
M2Bkg1->Draw("same");

if(savefile2==1) outputfileM2Mass1->cd();
//if(savefile2==1) M2M_Hist1->Write();
if(savefile2==1) M2Canvas1->Write(Form("M2M_Hist1_%d",imean));

if(iprint==1)
{
cout<<"method2 SigBkg1 par0: "<<M2SigBkg1->GetParameter(0)<<endl;
cout<<"method2 SigBkg1 par1: "<<M2SigBkg1->GetParameter(1)<<endl;
cout<<"method2 SigBkg1 par2: "<<M2SigBkg1->GetParameter(2)<<endl;
cout<<"method2 SigBkg1 par3: "<<M2SigBkg1->GetParameter(3)<<endl;
cout<<"method2 SigBkg1 par4: "<<M2SigBkg1->GetParameter(4)<<endl;
cout<<"method2 SigBkg1 par5: "<<M2SigBkg1->GetParameter(5)<<endl;
cout<<"method2 Bkg1 par0: "<<M2Bkg1->GetParameter(0)<<endl;
cout<<"method2 Bkg1 par1: "<<M2Bkg1->GetParameter(1)<<endl;
cout<<"method2 Bkg1 par2: "<<M2Bkg1->GetParameter(2)<<endl;
}

if(M2SigBkg1->GetParameter(3)<0) {
M2significance1[j]=0;
M2lnLB1=0;
M2lnLSpB1=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass1, M2Bkg1, M2SigBkg1, M2Significance1, M2lnLB1,M2lnLSpB1);
M2significance1[j]=M2Significance1;
}
cout<<"M2significance1 nb "<<j+1<<" (mass = "<<mean<<") : "<<M2significance1[j]<<endl;


M2CanvasDiv1->cd();
M2M_divHist1->SetLineColor(kBlack);
M2M_divHist1->SetLineWidth(2);
M2M_divHist1->SetMarkerStyle(20);
M2M_divHist1->SetMarkerColor(kBlack);
M2M_divHist1Signal->SetLineColor(kRed);
M2M_divHist1Signal->SetLineWidth(2);
M2M_divHist1Signal->SetMarkerStyle(21);
M2M_divHist1Signal->SetMarkerSize(0.7);
M2M_divHist1Signal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass1, M2Bkg1,M2SigBkg1,M2M_divHist1,M2M_divHist1Signal);
M2M_divHist1->SetName(Form("M2M_divHist1_%d",imean));
M2M_divHist1->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M2M_divHist1->Draw();
M2M_divHist1Signal->Draw("samehist");
if(savefile2==1) outputfileM2Pull1->cd();
if(savefile2==1) M2CanvasDiv1->Write(Form("M2M_divHist1_%d",imean));


M2CanvasDiv1Rebin->cd();
M2M_divHist1Rebin->SetLineColor(kBlack);
M2M_divHist1Rebin->SetLineWidth(2);
M2M_divHist1Rebin->SetMarkerStyle(20);
M2M_divHist1Rebin->SetMarkerColor(kBlack);
M2M_divHist1SignalRebin->SetLineColor(kRed);
M2M_divHist1SignalRebin->SetLineWidth(2);
M2M_divHist1SignalRebin->SetMarkerStyle(21);
M2M_divHist1SignalRebin->SetMarkerSize(0.7);
M2M_divHist1SignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass1, hist_mass1Rebin, M2Bkg1,M2SigBkg1,M2M_divHist1Rebin,M2M_divHist1SignalRebin);
M2M_divHist1Rebin->SetName(Form("M2M_divHist1Rebin_%d",imean));
M2M_divHist1Rebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M2M_divHist1Rebin->SetMinimum(-4);
M2M_divHist1Rebin->SetMaximum(4);
M2M_divHist1Rebin->Draw();
M2M_divHist1SignalRebin->Draw("samehist");
M2LegRebin1->AddEntry((TObject*)0,"","");
M2LegRebin1->AddEntry(M2M_divHist1Rebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M2LegRebin1->AddEntry((TObject*)0,"","");
M2LegRebin1->AddEntry(M2M_divHist1SignalRebin,"#frac{Signal}{Error(data)}","l");
M2LegRebin1->AddEntry((TObject*)0,"","");
M2LegRebin1->Draw();
if(savefile2==1) outputfileM2Pull1Rebin->cd();
if(savefile2==1) M2CanvasDiv1Rebin->Write(Form("M2M_divHist1Rebin_%d",imean));



//  ***********                   **************


M2SigBkg2->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M2SigBkg2->FixParameter(4,mean);
M2SigBkg2->FixParameter(5,sigma);

M2M_Hist2->SetName(Form("M2M_Hist2_%d",imean));
M2Canvas2->cd();
gPad->SetLogy();
M2SigBkg2->SetLineColor(kRed);
M2M_Hist2->Fit("M2SigBkg2","L","",xmin,xmax);

M2Bkg2->FixParameter(0,M2SigBkg2->GetParameter(0));
M2Bkg2->FixParameter(1,M2SigBkg2->GetParameter(1));
M2Bkg2->FixParameter(2,M2SigBkg2->GetParameter(2));
M2Bkg2->SetLineColor(kGreen);
M2Bkg2->Draw("same");

if(savefile2==1) outputfileM2Mass2->cd();
//if(savefile2==1) M2M_Hist2->Write();
if(savefile2==1) M2Canvas2->Write(Form("M2M_Hist2_%d",imean));

if(iprint==1)
{
cout<<"method2 SigBkg2 par0: "<<M2SigBkg2->GetParameter(0)<<endl;
cout<<"method2 SigBkg2 par1: "<<M2SigBkg2->GetParameter(1)<<endl;
cout<<"method2 SigBkg2 par2: "<<M2SigBkg2->GetParameter(2)<<endl;
cout<<"method2 SigBkg2 par3: "<<M2SigBkg2->GetParameter(3)<<endl;
cout<<"method2 SigBkg2 par4: "<<M2SigBkg2->GetParameter(4)<<endl;
cout<<"method2 SigBkg2 par5: "<<M2SigBkg2->GetParameter(5)<<endl;
cout<<"method2 Bkg2 par0: "<<M2Bkg2->GetParameter(0)<<endl;
cout<<"method2 Bkg2 par1: "<<M2Bkg2->GetParameter(1)<<endl;
cout<<"method2 Bkg2 par2: "<<M2Bkg2->GetParameter(2)<<endl;
}

if(M2SigBkg2->GetParameter(3)<0) {
M2significance2[j]=0;
M2lnLB2=0;
M2lnLSpB2=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass2, M2Bkg2, M2SigBkg2, M2Significance2, M2lnLB2,M2lnLSpB2);
M2significance2[j]=M2Significance2;
}
cout<<"M2significance2 nb "<<j+1<<" (mass = "<<mean<<") : "<<M2significance2[j]<<endl;

M2CanvasDiv2->cd();
M2M_divHist2->SetLineColor(kBlack);
M2M_divHist2->SetLineWidth(2);
M2M_divHist2->SetMarkerStyle(20);
M2M_divHist2->SetMarkerColor(kBlack);
M2M_divHist2Signal->SetLineColor(kRed);
M2M_divHist2Signal->SetLineWidth(2);
M2M_divHist2Signal->SetMarkerStyle(21);
M2M_divHist2Signal->SetMarkerSize(0.7);
M2M_divHist2Signal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass2, M2Bkg2,M2SigBkg2,M2M_divHist2,M2M_divHist2Signal);
M2M_divHist2->SetName(Form("M2M_divHist2_%d",imean));
M2M_divHist2->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M2M_divHist2->Draw();
M2M_divHist2Signal->Draw("samehist");
if(savefile2==1) outputfileM2Pull2->cd();
if(savefile2==1) M2CanvasDiv2->Write(Form("M2M_divHist2_%d",imean));


M2CanvasDiv2Rebin->cd();
M2M_divHist2Rebin->SetLineColor(kBlack);
M2M_divHist2Rebin->SetLineWidth(2);
M2M_divHist2Rebin->SetMarkerStyle(20);
M2M_divHist2Rebin->SetMarkerColor(kBlack);
M2M_divHist2SignalRebin->SetLineColor(kRed);
M2M_divHist2SignalRebin->SetLineWidth(2);
M2M_divHist2SignalRebin->SetMarkerStyle(21);
M2M_divHist2SignalRebin->SetMarkerSize(0.7);
M2M_divHist2SignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass2, hist_mass2Rebin, M2Bkg2,M2SigBkg2,M2M_divHist2Rebin,M2M_divHist2SignalRebin);
M2M_divHist2Rebin->SetName(Form("M2M_divHist2Rebin_%d",imean));
M2M_divHist2Rebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M2M_divHist2Rebin->SetMinimum(-4);
M2M_divHist2Rebin->SetMaximum(4);
M2M_divHist2Rebin->Draw();
M2M_divHist2SignalRebin->Draw("samehist");
M2LegRebin2->AddEntry((TObject*)0,"","");
M2LegRebin2->AddEntry(M2M_divHist2Rebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M2LegRebin2->AddEntry((TObject*)0,"","");
M2LegRebin2->AddEntry(M2M_divHist2SignalRebin,"#frac{Signal}{Error(data)}","l");
M2LegRebin2->AddEntry((TObject*)0,"","");
M2LegRebin2->Draw();
if(savefile2==1) outputfileM2Pull2Rebin->cd();
if(savefile2==1) M2CanvasDiv2Rebin->Write(Form("M2M_divHist2Rebin_%d",imean));


//  **************                      ***************


M2SigBkg3->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M2SigBkg3->FixParameter(4,mean);
M2SigBkg3->FixParameter(5,sigma);

M2M_Hist3->SetName(Form("M2M_Hist3_%d",imean));
M2Canvas3->cd();
gPad->SetLogy();
M2SigBkg3->SetLineColor(kRed);
M2M_Hist3->Fit("M2SigBkg3","L","",xmin,xmax);

M2Bkg3->FixParameter(0,M2SigBkg3->GetParameter(0));
M2Bkg3->FixParameter(1,M2SigBkg3->GetParameter(1));
M2Bkg3->FixParameter(2,M2SigBkg3->GetParameter(2));
M2Bkg3->SetLineColor(kGreen);
M2Bkg3->Draw("same");

if(savefile2==1) outputfileM2Mass3->cd();
//if(savefile2==1) M2M_Hist3->Write();
if(savefile2==1) M2Canvas3->Write(Form("M2M_Hist3_%d",imean));

if(iprint==1)
{
cout<<"method2 SigBkg3 par0: "<<M2SigBkg3->GetParameter(0)<<endl;
cout<<"method2 SigBkg3 par1: "<<M2SigBkg3->GetParameter(1)<<endl;
cout<<"method2 SigBkg3 par2: "<<M2SigBkg3->GetParameter(2)<<endl;
cout<<"method2 SigBkg3 par3: "<<M2SigBkg3->GetParameter(3)<<endl;
cout<<"method2 SigBkg3 par4: "<<M2SigBkg3->GetParameter(4)<<endl;
cout<<"method2 SigBkg3 par5: "<<M2SigBkg3->GetParameter(5)<<endl;
cout<<"method2 Bkg3 par0: "<<M2Bkg3->GetParameter(0)<<endl;
cout<<"method2 Bkg3 par1: "<<M2Bkg3->GetParameter(1)<<endl;
cout<<"method2 Bkg3 par2: "<<M2Bkg3->GetParameter(2)<<endl;
}


if(M2SigBkg3->GetParameter(3)<0) {
M2significance3[j]=0;
M2lnLB3=0;
M2lnLSpB3=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass3, M2Bkg3, M2SigBkg3, M2Significance3, M2lnLB3,M2lnLSpB3);
M2significance3[j]=M2Significance3;
}
cout<<"M2significance3 nb "<<j+1<<" (mass = "<<mean<<") : "<<M2significance3[j]<<endl;

M2CanvasDiv3->cd();
M2M_divHist3->SetLineColor(kBlack);
M2M_divHist3->SetLineWidth(2);
M2M_divHist3->SetMarkerStyle(20);
M2M_divHist3->SetMarkerColor(kBlack);
M2M_divHist3Signal->SetLineColor(kRed);
M2M_divHist3Signal->SetLineWidth(2);
M2M_divHist3Signal->SetMarkerStyle(21);
M2M_divHist3Signal->SetMarkerSize(0.7);
M2M_divHist3Signal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass3, M2Bkg3,M2SigBkg3,M2M_divHist3,M2M_divHist3Signal);
M2M_divHist3->SetName(Form("M2M_divHist3_%d",imean));
M2M_divHist3->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M2M_divHist3->Draw();
M2M_divHist3Signal->Draw("samehist");
if(savefile2==1) outputfileM2Pull3->cd();
if(savefile2==1) M2CanvasDiv3->Write(Form("M2M_divHist3_%d",imean));


M2CanvasDiv3Rebin->cd();
M2M_divHist3Rebin->SetLineColor(kBlack);
M2M_divHist3Rebin->SetLineWidth(2);
M2M_divHist3Rebin->SetMarkerStyle(20);
M2M_divHist3Rebin->SetMarkerColor(kBlack);
M2M_divHist3SignalRebin->SetLineColor(kRed);
M2M_divHist3SignalRebin->SetLineWidth(2);
M2M_divHist3SignalRebin->SetMarkerStyle(21);
M2M_divHist3SignalRebin->SetMarkerSize(0.7);
M2M_divHist3SignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass3, hist_mass3Rebin, M2Bkg3,M2SigBkg3,M2M_divHist3Rebin,M2M_divHist3SignalRebin);
M2M_divHist3Rebin->SetName(Form("M2M_divHist3Rebin_%d",imean));
M2M_divHist3Rebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M2M_divHist3Rebin->SetMinimum(-4);
M2M_divHist3Rebin->SetMaximum(4);
M2M_divHist3Rebin->Draw();
M2M_divHist3SignalRebin->Draw("samehist");
M2LegRebin3->AddEntry((TObject*)0,"","");
M2LegRebin3->AddEntry(M2M_divHist3Rebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M2LegRebin3->AddEntry((TObject*)0,"","");
M2LegRebin3->AddEntry(M2M_divHist3SignalRebin,"#frac{Signal}{Error(data)}","l");
M2LegRebin3->AddEntry((TObject*)0,"","");
M2LegRebin3->Draw();
if(savefile2==1) outputfileM2Pull3Rebin->cd();
if(savefile2==1) M2CanvasDiv3Rebin->Write(Form("M2M_divHist3Rebin_%d",imean));


//   ************              **************


M2lnLB=M2lnLB1+M2lnLB2+M2lnLB3;
M2lnLSpB=M2lnLSpB1+M2lnLSpB2+M2lnLSpB3;
//cout<<"M2lnLB: "<<M2lnLB<<endl;
//cout<<"M2lnLSpLB: "<<M2lnLSpB<<endl;
    if(M2lnLB<M2lnLSpB) M2significance[j] = TMath::Sqrt(-2*( M2lnLB - M2lnLSpB));
    else M2significance[j]=0;

}



// *********************************** method3


if(exec3==1)
{
  TLegend* M3LegMass0 = new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M3LegMass1 = new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M3LegRebin1 = new TLegend(0.3, 0.2, .5, .35);
  TLegend* M3LegMass2 = new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M3LegRebin2 = new TLegend(0.3, 0.2, .5, .35);
  TLegend* M3LegMass3 = new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M3LegRebin3 = new TLegend(0.3, 0.2, .5, .35);


M3SigBkg0->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M3SigBkg0->FixParameter(4,mean);
M3SigBkg0->FixParameter(5,sigma);

M3M_Hist->SetName(Form("M3M_Hist_%d",imean));
M3Canvas0->cd();
gPad->SetLogy();
M3SigBkg0->SetLineColor(kRed);
M3M_Hist->Fit("M3SigBkg0","L","",xmin,xmax);
norm0=M3SigBkg0->GetParameter(3);
norm1=norm0*r1;
norm2=norm0*r2;
norm3=norm0*r3;


M3SigBkg1->SetParameters(2.6,9.1,5.1,norm1,mean,sigma);
M3SigBkg1->FixParameter(3,norm1);
M3SigBkg1->FixParameter(4,mean);
M3SigBkg1->FixParameter(5,sigma);

M3Canvas1->cd();
gPad->SetLogy();
M3SigBkg1->SetLineColor(kRed);
M3M_Hist1->Fit("M3SigBkg1","L","",xmin,xmax);

M3Bkg1->FixParameter(0,M3SigBkg1->GetParameter(0));
M3Bkg1->FixParameter(1,M3SigBkg1->GetParameter(1));
M3Bkg1->FixParameter(2,M3SigBkg1->GetParameter(2));
M3Bkg1->SetLineColor(kGreen);
M3Bkg1->Draw("same");

if(savefile3==1) outputfileM3Mass1->cd();
//if(savefile3==1) M3M_Hist1->Write();
if(savefile3==1) M3Canvas1->Write(Form("M3M_Hist1_%d",imean));


if(iprint==1)
{
cout<<"method3 SigBkg1 par0: "<<M3SigBkg1->GetParameter(0)<<endl;
cout<<"method3 SigBkg1 par1: "<<M3SigBkg1->GetParameter(1)<<endl;
cout<<"method3 SigBkg1 par2: "<<M3SigBkg1->GetParameter(2)<<endl;
cout<<"method3 SigBkg1 par3: "<<M3SigBkg1->GetParameter(3)<<endl;
cout<<"method3 SigBkg1 par4: "<<M3SigBkg1->GetParameter(4)<<endl;
cout<<"method3 SigBkg1 par5: "<<M3SigBkg1->GetParameter(5)<<endl;
cout<<"method3 Bkg1 par0: "<<M3Bkg1->GetParameter(0)<<endl;
cout<<"method3 Bkg1 par1: "<<M3Bkg1->GetParameter(1)<<endl;
cout<<"method3 Bkg1 par2: "<<M3Bkg1->GetParameter(2)<<endl;
}

if(M3SigBkg1->GetParameter(3)<0) {
M3significance1[j]=0;
M3lnLB1=0;
M3lnLSpB1=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass1, M3Bkg1, M3SigBkg1, M3Significance1, M3lnLB1,M3lnLSpB1);
M3significance1[j]=M3Significance1;
}
cout<<"M3significance1 nb "<<j+1<<" (mass = "<<mean<<") : "<<M3significance1[j]<<endl;

M3CanvasDiv1->cd();
M3M_divHist1->SetLineColor(kBlack);
M3M_divHist1->SetLineWidth(2);
M3M_divHist1->SetMarkerStyle(20);
M3M_divHist1->SetMarkerColor(kBlack);
M3M_divHist1Signal->SetLineColor(kRed);
M3M_divHist1Signal->SetLineWidth(2);
M3M_divHist1Signal->SetMarkerStyle(21);
M3M_divHist1Signal->SetMarkerSize(0.7);
M3M_divHist1Signal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass1, M3Bkg1,M3SigBkg1,M3M_divHist1,M3M_divHist1Signal);
M3M_divHist1->SetName(Form("M3M_divHist1_%d",imean));
M3M_divHist1->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M3M_divHist1->Draw();
M3M_divHist1Signal->Draw("samehist");
if(savefile3==1) outputfileM3Pull1->cd();
if(savefile3==1) M3CanvasDiv1->Write(Form("M3M_divHist1_%d",imean));


M3CanvasDiv1Rebin->cd();
M3M_divHist1Rebin->SetLineColor(kBlack);
M3M_divHist1Rebin->SetLineWidth(2);
M3M_divHist1Rebin->SetMarkerStyle(20);
M3M_divHist1Rebin->SetMarkerColor(kBlack);
M3M_divHist1SignalRebin->SetLineColor(kRed);
M3M_divHist1SignalRebin->SetLineWidth(2);
M3M_divHist1SignalRebin->SetMarkerStyle(21);
M3M_divHist1SignalRebin->SetMarkerSize(0.7);
M3M_divHist1SignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass1, hist_mass1Rebin, M3Bkg1,M3SigBkg1,M3M_divHist1Rebin,M3M_divHist1SignalRebin);
M3M_divHist1Rebin->SetName(Form("M3M_divHist1Rebin_%d",imean));
M3M_divHist1Rebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M3M_divHist1Rebin->SetMinimum(-4);
M3M_divHist1Rebin->SetMaximum(4);
M3M_divHist1Rebin->Draw();
M3M_divHist1SignalRebin->Draw("samehist");
M3LegRebin1->AddEntry((TObject*)0,"","");
M3LegRebin1->AddEntry(M3M_divHist1Rebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M3LegRebin1->AddEntry((TObject*)0,"","");
M3LegRebin1->AddEntry(M3M_divHist1SignalRebin,"#frac{Signal}{Error(data)}","l");
M3LegRebin1->AddEntry((TObject*)0,"","");
M3LegRebin1->Draw();
if(savefile3==1) outputfileM3Pull1Rebin->cd();
if(savefile3==1) M3CanvasDiv1Rebin->Write(Form("M3M_divHist1Rebin_%d",imean));


//  ***********                   **************


M3SigBkg2->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M3SigBkg2->FixParameter(3,norm2);
M3SigBkg2->FixParameter(4,mean);
M3SigBkg2->FixParameter(5,sigma);

M3M_Hist2->SetName(Form("M3M_Hist2_%d",imean));
M3Canvas2->cd();
gPad->SetLogy();
M3SigBkg2->SetLineColor(kRed);
M3M_Hist2->Fit("M3SigBkg2","L","",xmin,xmax);

M3Bkg2->FixParameter(0,M3SigBkg2->GetParameter(0));
M3Bkg2->FixParameter(1,M3SigBkg2->GetParameter(1));
M3Bkg2->FixParameter(2,M3SigBkg2->GetParameter(2));
M3Bkg2->SetLineColor(kGreen);
M3Bkg2->Draw("same");

if(savefile3==1) outputfileM3Mass2->cd();
if(savefile3==1) M3M_Hist2->Write();
if(savefile3==1) M3Canvas2->Write(Form("M3M_Hist2_%d",imean));


if(iprint==1)
{
cout<<"method3 SigBkg2 par0: "<<M3SigBkg2->GetParameter(0)<<endl;
cout<<"method3 SigBkg2 par1: "<<M3SigBkg2->GetParameter(1)<<endl;
cout<<"method3 SigBkg2 par2: "<<M3SigBkg2->GetParameter(2)<<endl;
cout<<"method3 SigBkg2 par3: "<<M3SigBkg2->GetParameter(3)<<endl;
cout<<"method3 SigBkg2 par4: "<<M3SigBkg2->GetParameter(4)<<endl;
cout<<"method3 SigBkg2 par5: "<<M3SigBkg2->GetParameter(5)<<endl;
cout<<"method3 Bkg2 par0: "<<M3Bkg2->GetParameter(0)<<endl;
cout<<"method3 Bkg2 par1: "<<M3Bkg2->GetParameter(1)<<endl;
cout<<"method3 Bkg2 par2: "<<M3Bkg2->GetParameter(2)<<endl;
}

if(M3SigBkg2->GetParameter(3)<0) {
M3significance2[j]=0;
M3lnLB2=0;
M3lnLSpB2=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass2, M3Bkg2, M3SigBkg2, M3Significance2, M3lnLB2,M3lnLSpB2);
M3significance2[j]=M3Significance2;
}
cout<<"M3significance2 nb "<<j+1<<" (mass = "<<mean<<") : "<<M3significance2[j]<<endl;


M3CanvasDiv2->cd();
M3M_divHist2->SetLineColor(kBlack);
M3M_divHist2->SetLineWidth(2);
M3M_divHist2->SetMarkerStyle(20);
M3M_divHist2->SetMarkerColor(kBlack);
M3M_divHist2Signal->SetLineColor(kRed);
M3M_divHist2Signal->SetLineWidth(2);
M3M_divHist2Signal->SetMarkerStyle(21);
M3M_divHist2Signal->SetMarkerSize(0.7);
M3M_divHist2Signal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass2, M3Bkg2,M3SigBkg2,M3M_divHist2,M3M_divHist2Signal);
M3M_divHist2->SetName(Form("M3M_divHist2_%d",imean));
M3M_divHist2->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M3M_divHist2->Draw();
M3M_divHist2Signal->Draw("samehist");
if(savefile3==1) outputfileM3Pull2->cd();
if(savefile3==1) M3CanvasDiv2->Write(Form("M3M_divHist2_%d",imean));


M3CanvasDiv2Rebin->cd();
M3M_divHist2Rebin->SetLineColor(kBlack);
M3M_divHist2Rebin->SetLineWidth(2);
M3M_divHist2Rebin->SetMarkerStyle(20);
M3M_divHist2Rebin->SetMarkerColor(kBlack);
M3M_divHist2SignalRebin->SetLineColor(kRed);
M3M_divHist2SignalRebin->SetLineWidth(2);
M3M_divHist2SignalRebin->SetMarkerStyle(21);
M3M_divHist2SignalRebin->SetMarkerSize(0.7);
M3M_divHist2SignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass2, hist_mass2Rebin, M3Bkg2,M3SigBkg2,M3M_divHist2Rebin,M3M_divHist2SignalRebin);
M3M_divHist2Rebin->SetName(Form("M3M_divHist2Rebin_%d",imean));
M3M_divHist2Rebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M3M_divHist2Rebin->SetMinimum(-4);
M3M_divHist2Rebin->SetMaximum(4);
M3M_divHist2Rebin->Draw();
M3M_divHist2SignalRebin->Draw("samehist");
M3LegRebin2->AddEntry((TObject*)0,"","");
M3LegRebin2->AddEntry(M3M_divHist2Rebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M3LegRebin2->AddEntry((TObject*)0,"","");
M3LegRebin2->AddEntry(M3M_divHist2SignalRebin,"#frac{Signal}{Error(data)}","l");
M3LegRebin2->AddEntry((TObject*)0,"","");
M3LegRebin2->Draw();
if(savefile3==1) outputfileM3Pull2Rebin->cd();
if(savefile3==1) M3CanvasDiv2Rebin->Write(Form("M3M_divHist2Rebin_%d",imean));

//  **************                      ***************


M3SigBkg3->SetParameters(2.6,9.1,5.1,1,mean,sigma);
M3SigBkg3->FixParameter(3,norm3);
M3SigBkg3->FixParameter(4,mean);
M3SigBkg3->FixParameter(5,sigma);

M3M_Hist3->SetName(Form("M3M_Hist3_%d",imean));
M3Canvas3->cd();
gPad->SetLogy();
M3SigBkg3->SetLineColor(kRed);
M3M_Hist3->Fit("M3SigBkg3","L","",xmin,xmax);

M3Bkg3->FixParameter(0,M3SigBkg3->GetParameter(0));
M3Bkg3->FixParameter(1,M3SigBkg3->GetParameter(1));
M3Bkg3->FixParameter(2,M3SigBkg3->GetParameter(2));
M3Bkg3->SetLineColor(kGreen);
M3Bkg3->Draw("same");

if(savefile3==1) outputfileM3Mass3->cd();
//if(savefile3==1) M3M_Hist3->Write();
if(savefile3==1) M3Canvas3->Write(Form("M3M_Hist3_%d",imean));


if(iprint==1)
{
cout<<"method3 SigBkg3 par0: "<<M3SigBkg3->GetParameter(0)<<endl;
cout<<"method3 SigBkg3 par1: "<<M3SigBkg3->GetParameter(1)<<endl;
cout<<"method3 SigBkg3 par2: "<<M3SigBkg3->GetParameter(2)<<endl;
cout<<"method3 SigBkg3 par3: "<<M3SigBkg3->GetParameter(3)<<endl;
cout<<"method3 SigBkg3 par4: "<<M3SigBkg3->GetParameter(4)<<endl;
cout<<"method3 SigBkg3 par5: "<<M3SigBkg3->GetParameter(5)<<endl;
cout<<"method3 Bkg3 par0: "<<M3Bkg3->GetParameter(0)<<endl;
cout<<"method3 Bkg3 par1: "<<M3Bkg3->GetParameter(1)<<endl;
cout<<"method3 Bkg3 par2: "<<M3Bkg3->GetParameter(2)<<endl;
}


if(M3SigBkg3->GetParameter(3)<0) {
M3significance3[j]=0;
M3lnLB3=0;
M3lnLSpB3=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass3, M3Bkg3, M3SigBkg3, M3Significance3, M3lnLB3,M3lnLSpB3);
M3significance3[j]=M3Significance3;
}
cout<<"M3significance3 nb "<<j+1<<" (mass = "<<mean<<") : "<<M3significance3[j]<<endl;

M3CanvasDiv3->cd();
M3M_divHist3->SetLineColor(kBlack);
M3M_divHist3->SetLineWidth(2);
M3M_divHist3->SetMarkerStyle(20);
M3M_divHist3->SetMarkerColor(kBlack);
M3M_divHist3Signal->SetLineColor(kRed);
M3M_divHist3Signal->SetLineWidth(2);
M3M_divHist3Signal->SetMarkerStyle(21);
M3M_divHist3Signal->SetMarkerSize(0.7);
M3M_divHist3Signal->SetMarkerColor(kRed);
CalcPull(xmin,xmax,NBin,BinWidth,hist_mass3, M3Bkg3,M3SigBkg3,M3M_divHist3,M3M_divHist3Signal);
M3M_divHist3->SetName(Form("M3M_divHist3_%d",imean));
M3M_divHist3->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M3M_divHist3->Draw();
M3M_divHist3Signal->Draw("samehist");
if(savefile3==1) outputfileM3Pull3->cd();
if(savefile3==1) M3CanvasDiv3->Write(Form("M3M_divHist3_%d",imean));


M3CanvasDiv3Rebin->cd();
M3M_divHist3Rebin->SetLineColor(kBlack);
M3M_divHist3Rebin->SetLineWidth(2);
M3M_divHist3Rebin->SetMarkerStyle(20);
M3M_divHist3Rebin->SetMarkerColor(kBlack);
M3M_divHist3SignalRebin->SetLineColor(kRed);
M3M_divHist3SignalRebin->SetLineWidth(2);
M3M_divHist3SignalRebin->SetMarkerStyle(21);
M3M_divHist3SignalRebin->SetMarkerSize(0.7);
M3M_divHist3SignalRebin->SetMarkerColor(kRed);
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass3, hist_mass3Rebin, M3Bkg3,M3SigBkg3,M3M_divHist3Rebin,M3M_divHist3SignalRebin);
M3M_divHist3Rebin->SetName(Form("M3M_divHist3Rebin_%d",imean));
M3M_divHist3Rebin->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth/10),TMath::Floor(xmax/BinWidth/10));
M3M_divHist3Rebin->SetMinimum(-4);
M3M_divHist3Rebin->SetMaximum(4);
M3M_divHist3Rebin->Draw();
M3M_divHist3SignalRebin->Draw("samehist");
M3LegRebin3->AddEntry((TObject*)0,"","");
M3LegRebin3->AddEntry(M3M_divHist3Rebin,"#frac{Data - Fit(bkg)}{Error(data)}","l");
M3LegRebin3->AddEntry((TObject*)0,"","");
M3LegRebin3->AddEntry(M3M_divHist3SignalRebin,"#frac{Signal}{Error(data)}","l");
M3LegRebin3->AddEntry((TObject*)0,"","");
M3LegRebin3->Draw();
if(savefile3==1) outputfileM3Pull3Rebin->cd();
if(savefile3==1) M3CanvasDiv3Rebin->Write(Form("M3M_divHist3Rebin_%d",imean));


//   ************              **************


M3lnLB=M3lnLB1+M3lnLB2+M3lnLB3;
M3lnLSpB=M3lnLSpB1+M3lnLSpB2+M3lnLSpB3;
//cout<<"M3lnLB: "<<M3lnLB<<endl;
//cout<<"M3lnLSpLB: "<<M3lnLSpB<<endl;
    if(M3lnLB<M3lnLSpB) M3significance[j] = TMath::Sqrt(-2*( M3lnLB - M3lnLSpB));
    else M3significance[j]=0;

}

if(j<Nvalues-1)
{

if(exec1==1)
{
delete M1LegMass;
delete M1LegRebin;
}
if(exec2==1)
{
delete M2LegMass1;
delete M2LegRebin1;
delete M2LegMass2;
delete M2LegRebin2;
delete M2LegMass3;
delete M2LegRebin3;
}
if(exec3==1)
{
delete M3LegMass0;
delete M3LegMass1;
delete M3LegRebin1;
delete M3LegMass2;
delete M3LegRebin2;
delete M3LegMass3;
delete M3LegRebin3;
}

}

}// loop j

// ******************************************************************

for (int j=0;j<Nvalues;j++)
{
if(meantest==0) mean = xmin+(j+1)*(xmax-xmin)/(Nvalues+1);
else mean=meantest;

if(exec1==1)
{
cout<<"M1significance nb "<<j+1<<" (mass = "<<mean<<") : "<<M1significance[j]<<endl;
x1[j]=mean;
y1[j]=M1significance[j];
//cout<<"j: "<<j<<"  x1[j]: "<<x1[j]<<"  y1[j]: "<<y1[j]<<endl;
}
if(exec2==1)
{
cout<<"M2significance nb "<<j+1<<" (mass = "<<mean<<") : "<<M2significance[j]<<endl;
x2[j]=mean;
y2[j]=M2significance[j];
//cout<<"j: "<<j<<"  x2[j]: "<<x2[j]<<"  y2[j]: "<<y2[j]<<endl;
}
if(exec3==1)
{
cout<<"M3significance nb "<<j+1<<" (mass = "<<mean<<") : "<<M3significance[j]<<endl;
x3[j]=mean;
y3[j]=M3significance[j];
//cout<<"j: "<<j<<"  x3[j]: "<<x3[j]<<"  y3[j]: "<<y3[j]<<endl;
}
}

if(exec1==1)
{
TGraph *M1graph= new TGraph(Nvalues,x1,y1);
M1graph->SetTitle("method1: Significance as a function of the Invariant Mass");
M1graph->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M1graph->GetYaxis()->SetTitle("Significance");
M1graph->SetLineColor(2);
M1graph->SetLineWidth(2);
M1graph->SetMarkerColor(4);
M1graph->SetMarkerStyle(21);
M1graph->SetMarkerSize(0.7);

TCanvas *M1CanvasSig = new TCanvas("M1CanvasSig","M1CanvasSig",11,51,700,500);
M1CanvasSig->SetGrid();

M1CanvasSig->cd();
M1graph->Draw("ALP");
M1graph->SetMinimum(0);
M1graph->SetMaximum(4);
if(savefile1==1) outputfileM1Sig->cd();
if(savefile1==1) M1graph->Write();
}

if(exec2==1)
{
TGraph *M2graph= new TGraph(Nvalues,x2,y2);
M2graph->SetTitle("method2: Significance as a function of the Invariant Mass");
M2graph->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graph->GetYaxis()->SetTitle("Significance");
M2graph->SetLineColor(2);
M2graph->SetLineWidth(2);
M2graph->SetMarkerColor(4);
M2graph->SetMarkerStyle(21);
M2graph->SetMarkerSize(0.7);

TCanvas *M2CanvasSig = new TCanvas("M2CanvasSig","M2CanvasSig",11,51,700,500);
M2CanvasSig->SetGrid();

M2CanvasSig->cd();
M2graph->Draw("ALP");
M2graph->SetMinimum(0);
M2graph->SetMaximum(4);
if(savefile2==1) outputfileM2Sig->cd();
if(savefile2==1) M2graph->Write();
}


if(exec3==1)
{
TGraph *M3graph= new TGraph(Nvalues,x3,y3);
M3graph->SetTitle("method3: Significance as a function of the Invariant Mass");
M3graph->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M3graph->GetYaxis()->SetTitle("Significance");
M3graph->SetLineColor(2);
M3graph->SetLineWidth(2);
M3graph->SetMarkerColor(4);
M3graph->SetMarkerStyle(21);
M3graph->SetMarkerSize(0.7);

TCanvas *M3CanvasSig = new TCanvas("M3CanvasSig","M3CanvasSig",11,51,700,500);
M3CanvasSig->SetGrid();

M3CanvasSig->cd();
M3graph->Draw("ALP");
M3graph->SetMinimum(0);
M3graph->SetMaximum(4);
if(savefile3==1) outputfileM3Sig->cd();
if(savefile3==1) M3graph->Write();
}



}


void CalcSignificance(double xmin,double xmax,TH1D* pData, TF1* bkg, TF1* sigbkg, double&Significance, double &lnLB, double &lnLSpB)
{
int NBin=pData->GetNbinsX();
lnLB=0;
lnLSpB=0;
for (int i = 1; i < NBin+1; i++)
	{
 	if(pData->GetXaxis()->GetBinLowEdge(i)>=xmin && pData->GetXaxis()->GetBinUpEdge(i)<=xmax)
 		{
		double mass = pData->GetBinCenter(i);
		int N = abs(pData->GetBinContent(i));
		double muB = bkg->Eval(mass);
		double muSpB = sigbkg->Eval(mass);
		//cout <<"mass: "<<mass<<"  N = " << N << "  muB = " << muB << "  muSpB = " << muSpB <<endl;
		lnLB = lnLB-muB+N*log(muB);
		lnLSpB = lnLSpB-muSpB+N*log(muSpB);
 		 }
	}
//cout<<"lnLB: "<<lnLB<<endl;
//cout<<"lnLSpLB: "<<lnLSpB<<endl;

    if(lnLB<lnLSpB) Significance = TMath::Sqrt(-2*(lnLB - lnLSpB));
    else Significance=0;
}


void CalcPull(double xmin, double xmax,int NBin,int binwidth,TH1D* pData, TF1* bkg,TF1* sig, TH1D*&Mdiv_Hist, TH1D*&Mdiv_HistSignal)
{
Mdiv_Hist->Reset();
Mdiv_HistSignal->Reset();
sig->SetParameter(0,0);
for(int j=1;j<NBin+1;j++)
{

if(pData->GetXaxis()->GetBinLowEdge(j)>=xmin && pData->GetXaxis()->GetBinUpEdge(j)<=xmax)
{

 if(pData->GetBinError(j)!=0) 
	{
//cout<<"j: "<<j<<endl;
//cout<<"data: "<<pData->GetBinContent(j)<<endl;
//cout<<"function: "<<bkg->Eval(pData->GetBinCenter(j))<<endl;
//cout<<"j: "<<j<<endl;
//cout<<"data: "<<pData->GetBinContent(j)<<endl;
//cout<<"error: "<<pData->GetBinError(j)<<endl;
//cout<<"signal: "<<sig->Eval(pData->GetBinCenter(j))<<endl;
	Mdiv_Hist->SetBinContent(j,(pData->GetBinContent(j)-bkg->Eval(pData->GetBinCenter(j)))/pData->GetBinError(j));
	Mdiv_Hist->SetBinError(j,1);
	Mdiv_HistSignal->SetBinContent(j,sig->Eval(pData->GetBinCenter(j))/pData->GetBinError(j));
	Mdiv_HistSignal->SetBinError(j,1);
//cout<<"mdivhist signal content: "<<Mdiv_HistSignal->GetBinContent(j)<<"  mdivhist signal error: "<<Mdiv_HistSignal->GetBinError(j)<<endl;
	} 
}
}
//pData->Draw();
//Mdiv_Hist->Draw();
//bkg->Draw("same");



}


void CalcPullRebin(double xmin,double xmax,int NBin,int binwidth,TH1D* pData, TH1D* pDataRebin, TF1* bkg,TF1*sig,TH1D*&Mdiv_Hist,TH1D*&Mdiv_HistSignal)
{
Mdiv_Hist->Reset();
Mdiv_HistSignal->Reset();
sig->SetParameter(0,0);
double sumBKG=0;
double sumSIG=0;
double integral;
for(int j=1;j<NBin+1;j++)
{
if(pData->GetXaxis()->GetBinLowEdge(j)>=xmin && pData->GetXaxis()->GetBinUpEdge(j)<=xmax)
{
//integral=bkg->Integral(pData->GetXaxis()->GetBinLowEdge(j-9),pData->GetXaxis()->GetBinUpEdge(j))/10;
integral=bkg->Integral(pDataRebin->GetXaxis()->GetBinLowEdge(j/10),pDataRebin->GetXaxis()->GetBinUpEdge(j/10))/binwidth;
//if(j==90)integral=bkg->Integral(pData->GetXaxis()->GetBinLowEdge(89),pData->GetXaxis()->GetBinUpEdge(90))/10;
sumBKG=sumBKG+bkg->Eval(pData->GetBinCenter(j));
sumSIG=sumSIG+sig->Eval(pData->GetBinCenter(j));
//if (j%10==0) cout<<"j: "<<j<<"     :   "<<pDataRebin->GetXaxis()->GetBinLowEdge(j/10)<<" < "<< pData->GetBinCenter(j)<<" < "<<pDataRebin->GetXaxis()->GetBinUpEdge(j/10)<<endl;
//else cout<<"j: "<<j<<"     :   "<<pDataRebin->GetXaxis()->GetBinLowEdge(TMath::Floor(j/10)+1)<<" < "<< pData->GetBinCenter(j)<<" < "<<pDataRebin->GetXaxis()->GetBinUpEdge(TMath::Floor(j/10)+1)<<endl;
if(j%10==0) 
{
//cout<<"j: "<<j<<endl;
//cout<<"low edge: "<<pData->GetXaxis()->GetBinLowEdge(j-9)<<endl;
//cout<<"up edge: "<<pData->GetXaxis()->GetBinUpEdge(j)<<endl;
//cout<<"low edge: "<<pDataRebin->GetXaxis()->GetBinLowEdge(j/10)<<endl;
//cout<<"up edge: "<<pDataRebin->GetXaxis()->GetBinUpEdge(j/10)<<endl;
//cout<<"sum: "<<sumBKG<<endl;
//cout<<"integral: "<<integral<<endl;


 if(pDataRebin->GetBinError(j/10)!=0)
	{
//cout<<"j: "<<j<<" data: "<<pDataRebin->GetBinContent(j/10)<<" function: "<<sumBKG<<"  error: "<<pDataRebin->GetBinError(j/10)<<" div: "<<(pDataRebin->GetBinContent(j/10)-sumBKG)/pDataRebin->GetBinError(j/10)<<endl;
	Mdiv_Hist->SetBinContent(j/10,(pDataRebin->GetBinContent(j/10)-sumBKG)/pDataRebin->GetBinError(j/10));
	Mdiv_Hist->SetBinError(j/10,1);
	Mdiv_HistSignal->SetBinContent(j/10,sumSIG/pDataRebin->GetBinError(j/10));
	Mdiv_HistSignal->SetBinError(j/10,1);

	}
sumBKG=0;
sumSIG=0;
}
}
}


//pData->Draw();
//Mdiv_Hist->Draw();
//bkg->Draw("same");
}





void PseudoExp(double xmin, double xmax, int binwidth, double nevt, int iend, int savefile, TF1 * bkg, double mean, double sigma, double significance)
{
  TCanvas *CanvasNEW_massHist = new TCanvas("CanvasNEW_massHist","CanvasNEW_massHist",11,51,700,500);
  TCanvas *CanvasNEW_sigHist = new TCanvas("CanvasNEW_sigHist","CanvasNEW_sigHist",11,51,700,500);
double significanceOUT=0;
bkg->SetName("bkg");
double Nbins=500;
  TH1D* SigHist = new TH1D("SigHist","Significance histogram",Nbins,0,5);
  SigHist->Sumw2();
  TF1 * signal_background_fit = new TF1 ("signal_background_fit","[0]*pow((1-x/8000),[1])/pow(x/8000,[2])+[3]*exp(-0.5*pow((x-[4])/[5],2))", xmin, xmax);
  TF1 * background_fit = new TF1 ("background_fit","[0]*pow((1-x/8000.),[1])/pow(x/8000.,[2])",xmin, xmax);

for(int i=0;i<iend;i++)
{
cout<<i<<endl;
  TH1D* MassHist = new TH1D("MassHist","Mass histogram",TMath::Floor((xmax-xmin)/binwidth),xmin,xmax);
MassHist->Sumw2();
MassHist->FillRandom("bkg",nevt);
signal_background_fit->SetParameters(2.6,9.1,5.1,1,mean,sigma);
signal_background_fit->FixParameter(4,mean);
signal_background_fit->FixParameter(5,sigma);
CanvasNEW_massHist->cd();
MassHist->Draw("");
MassHist->Fit("signal_background_fit","L");
bkg->Draw("same");
background_fit->FixParameter(0,signal_background_fit->GetParameter(0));
background_fit->FixParameter(1,signal_background_fit->GetParameter(1));
background_fit->FixParameter(2,signal_background_fit->GetParameter(2));

double lnLB;
double lnLSpB;
if(signal_background_fit->GetParameter(3)<0) {
significanceOUT=0;
lnLB=0;
lnLSpB=0;
}
else
{
CalcSignificance(xmin,xmax,MassHist, background_fit,signal_background_fit, significanceOUT, lnLB,lnLSpB);
}
//cout<<"lnLBout: "<<lnLB<<endl;
//cout<<"lnLSpBout: "<<lnLSpB<<endl;
//cout<<"significanceOUT: "<<significanceOUT<<endl;
SigHist->Fill(significanceOUT);
if(i<iend-1) delete MassHist;
}

CanvasNEW_sigHist->cd();
SigHist->Draw();
  TLine *line=new TLine(significance,0,significance,200);
line->SetLineColor(kRed);
line->SetLineWidth(2);
line->Draw("same");
double Ntot=SigHist->GetEntries();
double Nint=0;
int testint=0;
double pvalue;
cout<<"significance: "<<significance<<endl;
for(int j=Nbins;j>0;j--)
{
cout<<"j: "<<j<<"value in sighist: "<<SigHist->GetBinLowEdge(j)<<endl;
if(SigHist->GetBinLowEdge(j)< significance && testint==0) 
{
cout<<" test: "<<j<<endl;
Nint=SigHist->Integral(j,Nbins);
testint=1;
}
}
cout<<"Nint: "<<Nint<<endl;
pvalue=Nint/Ntot;
cout<<"pvalue: "<<pvalue<<endl;

if(savefile==1)
{
  TFile *pvalueFile = new TFile("pvalueGauss.root","update");
pvalueFile->cd();
CanvasNEW_sigHist->Write(Form("PvalueHist_%d",nevt));
}

} 


