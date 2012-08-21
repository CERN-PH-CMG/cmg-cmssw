

// code used to  scan the significance over the masses using a crystal ball function description for signal
// these function is described by 8 parameters which are implemented with a text file (CristalBall.txt)
// for sqrt(s)= 8 TeV only, else need to change 8000--> CollisionMass in all background functions and also in BKGpCBall


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
// 6) some additional plots or displays on screen: igraph=1 or iprint=1
// 7) define the signal ratio r1 , r2, r3  (here: isotropic decay description) for each DeltaEta bin
// 8) define the range for fit and function descritpion (xmin, xmax)
//    if the pvalue is calculated, xmin and xmax must be divisible by the binwidth, else it can be any real values
// 9) define the range for significance scan (minmass, maxmass) and the number of values (NValues and NValuesLoop for the loop only)
//    These values must be consistent with the Crystal Ball description in the text file CristalBall.txt (from 1000 to 4700 and divisible by 100 here)
//10) Calculation of significance for a given mass only --> meantest= given mass , else put 0. to scan over all masses defined within minmass and maxmass

double r1=0.436786; //ratio for signal
double r2=0.375497;
double r3=0.187717;

double r1_gg = 0.35;
double r2_gg = 0.40;
double r3_gg = 0.25;

double r1_qq = 0.52;
double r2_qq = 0.35; 
double r3_qq = 0.13;


Double_t Ntot=1;
Double_t N1=0;
Double_t N2=0;
Double_t N3=0;

void SigDataCB()
{

// variables definition and initialization


  int filenumber=5; // check that you have created the proper Plot repertory. See below where filenumber is used
  int Detastart=1; // this is the bin number
  int Detacut1=5; // last bin for the 1st region
  int Detacut2=10; // last bin for the 2nd region
  int Detaend=13;

int exec1=1; //method1
int exec2=1; //method2
int exec3=1; //method3
int savefile1=1; //method1
int savefile2=1; //method2
int savefile3=1; //method3
int iPValue=0; //generate pseudo experiment and calculate pvalue --> iPValue=1
int iendPValue=10000; //number of pseudo experiments
int savefilePValue=0; // to save the PValue plot -->savefilePValue=1
int igraph=1;  // plot signal ratio for method 2 and 3 -->igraph=1
int iprint=0;


double xmin=900; // range for the input histograms and fits
double xmax=4700.;
double minmass=1000.; // range for the significance scan only
double maxmass=4700.;
int Nvalues=(maxmass-minmass)/100+1; // number of values in the text file for Crystal Ball shapes, size of arrays and pointers
int NvaluesLoop=(maxmass-minmass)/100+1; // number of values in the loop

 double meantest=0.; // scan over all masses --> meantest=0. else choose the mass 

 double chisquare = 0; // chisquare to control the fits convergence

 string SignalShape("Qstar_8TeV");
 //string SignalShape("RSG_gg_8TeV");
 //string SignalShape("RSG_qq_8TeV");

//*******************************************************************************

// first part: display of data histograms


if(filenumber==0)
{
  TFile *file0=TFile::Open("data/DataScouting_V00-01-03_Run2012B_runrange_193752-197044_dijet_alfaT_razor.root");
  TDirectoryFile* DQMData_Merged_Runs_DataScouting_Run_summary_DiJet = (TDirectoryFile*) file0->Get("DQMData_Merged Runs_DataScouting_Run summary_DiJet;1");
  //  DQMData_Merged_Runs_DataScouting_Run_summary_DiJet->cd();
  TH2D* h_DEta_Mass = (TH2D*) DQMData_Merged_Runs_DataScouting_Run_summary_DiJet->Get("h2_DetajjVsMjjWide;1");
  string outputPlots("Plots_DataScouting_5TeV/");
  int Detastart=1; // this is the bin number
  int Detacut1=10; // last bin for the 1st region
  int Detacut2=20; // last bin for the 2nd region
  int Detaend=26;
  xmin = 500.;
  minmass = 1000.;
  h_DEta_Mass->Draw("COLZ");
}

if(filenumber==1)
{
  TFile *file0=TFile::Open("data/histograms_delta_Mass_ak5_3fbm1_190456_195947_8TeV.root");
  TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin10GeV;1");
  string outputPlots("Plots_3fbm1_");
  outputPlots = outputPlots + ""  + SignalShape + "/";
}

if(filenumber==2)
{
  TFile *file0=TFile::Open("data/histograms_delta_Mass_ak5_5fbm1_8TeV.root");
  TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat;1");
  string outputPlots("Plots_5fbm1_");
  outputPlots = outputPlots + ""  + SignalShape + "/";
}

if(filenumber==3)
{
  TFile *file0=TFile::Open("data/histograms_delta_Mass_ak5_7fbm1_8TeV.root");
  TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin1GeV;1");
  string outputPlots("Plots_7fbm1_");
  outputPlots = outputPlots + ""  + SignalShape + "/";
}

if(filenumber==4)
{
  TFile *file0=TFile::Open("data/histograms_delta_Mass_ak5_2fbm1_POST_ICHEP_8TeV.root");
  TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin1GeV;1");
  string outputPlots("Plots_2fbm1_POST_ICHEP_");
  outputPlots = outputPlots + ""  + SignalShape + "/";
}
if(filenumber==5)
{
TFile *file0=TFile::Open("data/histograms_delta_Mass_ak5_8p48fbm1_8TeV.root");
 TH2D* h_DEta_Mass = (TH2D*) file0->Get("h_DEta_Mass_data_fat_bin1GeV;1");
  string outputPlots("Plots_8p5fbm1_");
  outputPlots = outputPlots + ""  + SignalShape + "/";

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
if(filenumber>1 || filenumber == 0 )
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



if(filenumber==1 || filenumber == 0 )
{
hist_mass=h_DEta_Mass->ProjectionX("hist_mass",Detastart,Detaend,"e");
hist_mass1=h_DEta_Mass->ProjectionX("hist_mass1",Detastart,Detacut1,"e");
hist_mass2=h_DEta_Mass->ProjectionX("hist_mass2",Detacut1+1,Detacut2,"e");
hist_mass3=h_DEta_Mass->ProjectionX("hist_mass3",Detacut2+1,Detaend,"e");
}

if(filenumber>1)
{

hist_mass=h_DEta_Mass->ProjectionY("hist_mass",Detastart,Detaend,"e");
hist_mass1=h_DEta_Mass->ProjectionY("hist_mass1",Detastart,Detacut1,"e");
hist_mass2=h_DEta_Mass->ProjectionY("hist_mass2",Detacut1+1,Detacut2,"e");
hist_mass3=h_DEta_Mass->ProjectionY("hist_mass3",Detacut2+1,Detaend,"e");
}


if(filenumber>1 || filenumber == 0 )
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

Ntot=hist_mass->GetEntries();
N1=hist_mass1->GetEntries();
N2=hist_mass2->GetEntries();
N3=hist_mass3->GetEntries();

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

 if(savefile1==1) {
   string output = outputPlots + "CBM1MassSig.root";
   TFile *outputfileM1Mass = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM1PullSig.root";
   TFile *outputfileM1Pull = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM1PullRebinSig.root";
   TFile *outputfileM1PullRebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM1SigSig.root";
   TFile *outputfileM1Sig = new TFile(output.c_str(),"recreate");

 }

 if(savefile2==1) {

   string output = outputPlots + "CBM2MassSig.root";
   TFile *outputfileM2Mass = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullSig.root";
   TFile *outputfileM2Pull = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullRebinSig.root";
   TFile *outputfileM2PullRebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2SigSig.root";
   TFile *outputfileM2Sig = new TFile(output.c_str(),"recreate");


   string output = outputPlots + "CBM2MassSig1.root";
   TFile *outputfileM2Mass1 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullSig1.root";
   TFile *outputfileM2Pull1 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullRebinSig1.root";
   TFile *outputfileM2Pull1Rebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2SigSig1.root";
   TFile *outputfileM2Sig1 = new TFile(output.c_str(),"recreate");

   string output = outputPlots + "CBM2MassSig2.root";
   TFile *outputfileM2Mass2 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullSig2.root";
   TFile *outputfileM2Pull2 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullRebinSig2.root";
   TFile *outputfileM2Pull2Rebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2SigSig2.root";
   TFile *outputfileM2Sig2 = new TFile(output.c_str(),"recreate");


   string output = outputPlots + "CBM2MassSig3.root";
   TFile *outputfileM2Mass3 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullSig3.root";
   TFile *outputfileM2Pull3 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2PullRebinSig3.root";
   TFile *outputfileM2Pull3Rebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM2SigSig3.root";
   TFile *outputfileM2Sig3 = new TFile(output.c_str(),"recreate");
 }


 if(savefile3==1) {

   string output = outputPlots + "CBM3MassSig.root";
   TFile *outputfileM3Mass = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullSig.root";
   TFile *outputfileM3Pull = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullRebinSig.root";
   TFile *outputfileM3PullRebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3SigSig.root";
   TFile *outputfileM3Sig = new TFile(output.c_str(),"recreate");

   string output = outputPlots + "CBM3MassSig1.root";
   TFile *outputfileM3Mass1 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullSig1.root";
   TFile *outputfileM3Pull1 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullRebinSig1.root";
   TFile *outputfileM3Pull1Rebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3SigSig1.root";
   TFile *outputfileM3Sig1 = new TFile(output.c_str(),"recreate");

   string output = outputPlots + "CBM3MassSig2.root";
   TFile *outputfileM3Mass2 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullSig2.root";
   TFile *outputfileM3Pull2 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullRebinSig2.root";
   TFile *outputfileM3Pull2Rebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3SigSig2.root";
   TFile *outputfileM3Sig2 = new TFile(output.c_str(),"recreate");


   string output = outputPlots + "CBM3MassSig3.root";
   TFile *outputfileM3Mass3 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullSig3.root";
   TFile *outputfileM3Pull3 = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3PullRebinSig3.root";
   TFile *outputfileM3Pull3Rebin = new TFile(output.c_str(),"recreate");
   output = outputPlots + "CBM3SigSig3.root";
   TFile *outputfileM3Sig3 = new TFile(output.c_str(),"recreate");
 }

int NBin=hist_mass->GetNbinsX();
double minHist=hist_mass->GetXaxis()->GetBinLowEdge(1);
double maxHist=hist_mass->GetXaxis()->GetBinUpEdge(NBin);
int BinWidth=(maxHist-minHist)/NBin;
if(meantest!=0) NvaluesLoop=1;
if(meantest!=0) iprint=1;

//method1
double M1Significance;
double M2Significance1;
double M2Significance2;
double M2Significance3;
double M3Significance1;
double M3Significance2;
double M3Significance3;
double *M1significance=new double[Nvalues];
double M1lnLB;
double M1lnLSpB;
double *x1=new double[Nvalues];
double *y1=new double[Nvalues];
//method2
double *M2significance=new double[Nvalues];
double *M2significance1=new double[Nvalues];
double *M2significance2=new double[Nvalues];
double *M2significance3=new double[Nvalues];

double *M1nevents=new double[Nvalues]; 
double *M2nevents1=new double[Nvalues]; 
double *M2nevents2=new double[Nvalues]; 
double *M2nevents3=new double[Nvalues]; 

double *M1chisq=new double[Nvalues]; 
double *M2chisq1=new double[Nvalues]; 
double *M2chisq2=new double[Nvalues]; 
double *M2chisq3=new double[Nvalues]; 
double *M3chisq1=new double[Nvalues]; 
double *M3chisq2=new double[Nvalues]; 
double *M3chisq3=new double[Nvalues]; 

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
//method3
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

double *mass = new double[Nvalues];
double *sMean = new double[Nvalues];
double *sSigma = new double[Nvalues];
double *sAlphaHigh = new double[Nvalues];
double *sAlphaLow = new double[Nvalues];
double *sNHigh = new double[Nvalues];
double *sNLow = new double[Nvalues];
double *sFrac = new double[Nvalues];

ifstream flux;
string shapes("data/CristalBall_");
shapes = shapes + "" + SignalShape + ".txt";
flux.open(shapes.c_str());
for(int i=0;i<Nvalues;i++)
{
flux>>mass[i]>>sMean[i]>>sSigma[i]>>sAlphaHigh[i]>>sAlphaLow[i]>>sNHigh[i]>>sNLow[i]>>sFrac[i];
}
flux.close();

for(int i=0;i<Nvalues;i++)
{
M1significance[i]=0;
M2significance[i]=0;
M2significance1[i]=0;
M2significance2[i]=0;
M2significance3[i]=0;
M3significance[i]=0;
M3significance1[i]=0;
M3significance2[i]=0;
M3significance3[i]=0;
}


// *************************************************************************

if(exec1==1)
{
  TH1D*  M1M_Hist = (TH1D*) hist_mass->Clone("M1M_Hist");
  M1M_Hist->SetTitle("Method 1: Invariant Mass (GeV) for 0<#Delta#eta<1.3");
  M1M_Hist->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M1M_Hist->GetXaxis()->SetTitleOffset(1.1);
  M1M_Hist->GetYaxis()->SetTitle("number of events");
  TCanvas *M1Canvas = new TCanvas("M1Canvas","M1Canvas",11,51,700,500);
  TF1 *M1Bkg= new TF1("M1Bkg","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M1Sig = new TF1("M1Sig", CBall, xmin, xmax, 8);
  TF1* M1SigBkg = new TF1("M1SigBkg", BKGpCBall, xmin, xmax, 11);

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
  TF1 *M2Bkg1= new TF1("M2Bkg1","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M2Sig1 = new TF1("M2Sig1", CBall, xmin, xmax, 8);
  TF1* M2SigBkg1 = new TF1("M2SigBkg1", BKGpCBall, xmin, xmax, 11);

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
  TF1 *M2Bkg2= new TF1("M2Bkg2","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M2Sig2 = new TF1("M2Sig2", CBall, xmin, xmax, 8);
  TF1* M2SigBkg2 = new TF1("M2SigBkg2", BKGpCBall, xmin, xmax, 11);

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
  TF1 *M2Bkg3= new TF1("M2Bkg3","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M2Sig3 = new TF1("M2Sig3", CBall, xmin, xmax, 8);
  TF1* M2SigBkg3 = new TF1("M2SigBkg3", BKGpCBall, xmin, xmax, 11);

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
  TF1* M3SigBkg0 = new TF1("M3SigBkg0", BKGpCBall, xmin, xmax, 11);

  TH1D*  M3M_Hist1 = (TH1D*) hist_mass1->Clone("M3M_Hist1");
  M3M_Hist1->SetTitle("Method3 : Invariant Mass (GeV) for 0<#Delta#eta<0.5");
  M3M_Hist1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3M_Hist1->GetXaxis()->SetTitleOffset(1.1);
  M3M_Hist1->GetYaxis()->SetTitle("number of events");
  TCanvas *M3Canvas1 = new TCanvas("M3Canvas1","M3Canvas1",11,51,700,500);
  TF1 *M3Bkg1= new TF1("M3Bkg1","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M3Sig1 = new TF1("M3Sig1", CBall, xmin, xmax, 8);
  TF1* M3SigBkg1 = new TF1("M3SigBkg1", BKGpCBall, xmin, xmax, 11);

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
  TF1 *M3Bkg2= new TF1("M3Bkg2","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M3Sig2 = new TF1("M3Sig2", CBall, xmin, xmax, 8);
  TF1* M3SigBkg2 = new TF1("M3SigBkg2", BKGpCBall, xmin, xmax, 11);

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
  TF1 *M3Bkg3= new TF1("M3Bkg3","([0]*TMath::Power(1-x/8000,[1]))/(TMath::Power(x/8000,[2]))",xmin,xmax);
  TF1* M3Sig3 = new TF1("M3Sig3", CBall, xmin, xmax, 8);
  TF1* M3SigBkg3 = new TF1("M3SigBkg3", BKGpCBall, xmin, xmax, 11);

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
}
  //TCanvas *canvastest = new TCanvas("canvastest","canvastest",11,51,700,500);
// **************************************************************************

for (int j=0;j<NvaluesLoop;j++)
{

if(meantest!=0) j=(meantest-minmass)/100;
if(j<3 && meantest!=1000 && filenumber!=0) j=3; // fit doesn't converge at 1000 GeV
cout<<"value nb "<<j+1<<endl;
cout<<"mass: "<<mass[j]<<endl;
int imass=mass[j];


// ******************************************* method1

if(exec1==1)
{
  TLegend* M1LegMass =  new TLegend(0.6, 0.2, 0.9, .4);
  TLegend* M1LegRebin = new TLegend(0.3, 0.2, .5, .35);


M1SigBkg->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],50.);
M1SigBkg->FixParameter(3,sMean[j]);
M1SigBkg->FixParameter(4,sSigma[j]);
M1SigBkg->FixParameter(5,sAlphaHigh[j]);
M1SigBkg->FixParameter(6,sAlphaLow[j]);
M1SigBkg->FixParameter(7,sNHigh[j]);
M1SigBkg->FixParameter(8,sNLow[j]);
M1SigBkg->FixParameter(9,sFrac[j]);

//cout<<M1SigBkg->GetParameter(0)<<endl<<M1SigBkg->GetParameter(3)<<endl;
//canvastest->cd();
//M1Sig->Draw();

M1M_Hist->SetName(Form("M1M_Hist_%d",imass));
M1Canvas->cd();
gPad->SetLogy();
M1SigBkg->SetLineColor(kRed);
M1M_Hist->Fit("M1SigBkg","L","",xmin,xmax);
M1M_Hist->Fit("M1SigBkg","L","",xmin,xmax);
M1M_Hist->Fit("M1SigBkg","L","",xmin,xmax);

M1Bkg->FixParameter(0,M1SigBkg->GetParameter(0));
M1Bkg->FixParameter(1,M1SigBkg->GetParameter(1));
M1Bkg->FixParameter(2,M1SigBkg->GetParameter(2));
M1Bkg->SetLineColor(kGreen);
M1Bkg->Draw("same");




/*
 
 for (int i = 0; i < 8; i++){
   double rnd = M1Bkg->GetRandom(3700., 3900.);
   M1M_Hist->Fill(rnd);
   hist_mass->Fill(rnd);
   hist_massRebin->Fill(rnd);
  }


M1M_Hist->Fit("M1SigBkg","L","",xmin,xmax);

M1Bkg->FixParameter(0,M1SigBkg->GetParameter(0));
M1Bkg->FixParameter(1,M1SigBkg->GetParameter(1));
M1Bkg->FixParameter(2,M1SigBkg->GetParameter(2));
M1Bkg->SetLineColor(kGreen);
M1Bkg->Draw("same");
*/









M1Sig->FixParameter(0,M1SigBkg->GetParameter(3));
M1Sig->FixParameter(1,M1SigBkg->GetParameter(4));
M1Sig->FixParameter(2,M1SigBkg->GetParameter(5));
M1Sig->FixParameter(3,M1SigBkg->GetParameter(6));
M1Sig->FixParameter(4,M1SigBkg->GetParameter(7));
M1Sig->FixParameter(5,M1SigBkg->GetParameter(8));
M1Sig->FixParameter(6,M1SigBkg->GetParameter(9));
M1Sig->FixParameter(7,M1SigBkg->GetParameter(10));
M1Sig->SetLineColor(kViolet);
M1Sig->Draw("same");

 M1LegMass->AddEntry(M1M_Hist,"invariant mass","l");
 M1LegMass->AddEntry((TObject*)0,"","");
 M1LegMass->AddEntry(M1SigBkg,"fit from background and signal","l");
 M1LegMass->AddEntry((TObject*)0,"","");
 M1LegMass->AddEntry(M1Sig,"signal fit","l");
 M1LegMass->AddEntry((TObject*)0,"","");
 M1LegMass->AddEntry(M1Bkg,"background fit","l");
 M1LegMass->AddEntry((TObject*)0,"","");
 M1LegMass->Draw();


cout<<"method1: number of signal events: "<<M1Sig->Integral(xmin,xmax)/BinWidth<<endl;

if(savefile1==1) outputfileM1Mass->cd();
//if(savefile1==1) M1M_Hist->Write();
if(savefile1==1) M1Canvas->Write(Form("M1M_Hist_%d",imass));

if(iprint==1)
{
cout<<"method1 SigBkg par0: "<<M1SigBkg->GetParameter(0)<<endl;
cout<<"method1 SigBkg par1: "<<M1SigBkg->GetParameter(1)<<endl;
cout<<"method1 SigBkg par2: "<<M1SigBkg->GetParameter(2)<<endl;
cout<<"method1 SigBkg par3: "<<M1SigBkg->GetParameter(3)<<endl;
cout<<"method1 SigBkg par4: "<<M1SigBkg->GetParameter(4)<<endl;
cout<<"method1 SigBkg par5: "<<M1SigBkg->GetParameter(5)<<endl;
cout<<"method1 SigBkg par6: "<<M1SigBkg->GetParameter(6)<<endl;
cout<<"method1 SigBkg par7: "<<M1SigBkg->GetParameter(7)<<endl;
cout<<"method1 SigBkg par8: "<<M1SigBkg->GetParameter(8)<<endl;
cout<<"method1 SigBkg par9: "<<M1SigBkg->GetParameter(9)<<endl;
cout<<"method1 SigBkg par10: "<<M1SigBkg->GetParameter(10)<<endl;
cout<<"method1 Bkg par0: "<<M1Bkg->GetParameter(0)<<endl;
cout<<"method1 Bkg par1: "<<M1Bkg->GetParameter(1)<<endl;
cout<<"method1 Bkg par2: "<<M1Bkg->GetParameter(2)<<endl;
cout<<"method1 Sig par0: "<<M1Sig->GetParameter(0)<<endl;
cout<<"method1 Sig par1: "<<M1Sig->GetParameter(1)<<endl;
cout<<"method1 Sig par2: "<<M1Sig->GetParameter(2)<<endl;
cout<<"method1 Sig par3: "<<M1Sig->GetParameter(3)<<endl;
cout<<"method1 Sig par4: "<<M1Sig->GetParameter(4)<<endl;
cout<<"method1 Sig par5: "<<M1Sig->GetParameter(5)<<endl;
cout<<"method1 Sig par6: "<<M1Sig->GetParameter(6)<<endl;
cout<<"method1 Sig par7: "<<M1Sig->GetParameter(7)<<endl;
}

if(M1SigBkg->GetParameter(10)<0) {
M1significance[j]=0;
M1lnLB=0;
M1lnLSpB=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass, M1Bkg, M1SigBkg, M1Significance, M1lnLB,M1lnLSpB);
 M1significance[j]=M1Significance;
 M1nevents[j] = M1Sig->Integral(xmin,xmax)/BinWidth;

}
cout<<"M1significance nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M1significance[j]<<endl;


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
M1M_divHist->SetName(Form("M1M_divHist_%d",imass));
M1M_divHist->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M1M_divHist->Draw();
M1M_divHistSignal->Draw("samehist");



if(savefile1==1) outputfileM1Pull->cd();
//if(savefile1==1) M1M_divHist->Write();
if(savefile1==1) M1CanvasDiv->Write(Form("M1M_divHist_%d",imass));

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
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass, hist_massRebin, M1Bkg,M1SigBkg,M1M_divHistRebin,M1M_divHistSignalRebin, chisquare);

 cout << "M1 rebinned chisq = " << chisquare << endl;

 M1chisq[j] = chisquare;

M1M_divHistRebin->SetName(Form("M1M_divHistRebin_%d",imass));
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
if(savefile1==1) M1CanvasDivRebin->Write(Form("M1M_divHistRebin_%d",imass));

if(iPValue==1) PseudoExp(xmin,xmax,BinWidth,Ntot, iendPValue,savefilePValue, M1Bkg,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],M1significance[j]);

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

M2SigBkg1->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],50.);
M2SigBkg1->FixParameter(3,sMean[j]);
M2SigBkg1->FixParameter(4,sSigma[j]);
M2SigBkg1->FixParameter(5,sAlphaHigh[j]);
M2SigBkg1->FixParameter(6,sAlphaLow[j]);
M2SigBkg1->FixParameter(7,sNHigh[j]);
M2SigBkg1->FixParameter(8,sNLow[j]);
M2SigBkg1->FixParameter(9,sFrac[j]);

M2M_Hist1->SetName(Form("M2M_Hist1_%d",imass));
M2Canvas1->cd();
gPad->SetLogy();
M2SigBkg1->SetLineColor(kRed);
M2M_Hist1->Fit("M2SigBkg1","L","",xmin,xmax);
M2M_Hist1->Fit("M2SigBkg1","L","",xmin,xmax);
M2M_Hist1->Fit("M2SigBkg1","L","",xmin,xmax);

M2Bkg1->FixParameter(0,M2SigBkg1->GetParameter(0));
M2Bkg1->FixParameter(1,M2SigBkg1->GetParameter(1));
M2Bkg1->FixParameter(2,M2SigBkg1->GetParameter(2));
M2Bkg1->SetLineColor(kGreen);
M2Bkg1->Draw("same");

M2Sig1->FixParameter(0,M2SigBkg1->GetParameter(3));
M2Sig1->FixParameter(1,M2SigBkg1->GetParameter(4));
M2Sig1->FixParameter(2,M2SigBkg1->GetParameter(5));
M2Sig1->FixParameter(3,M2SigBkg1->GetParameter(6));
M2Sig1->FixParameter(4,M2SigBkg1->GetParameter(7));
M2Sig1->FixParameter(5,M2SigBkg1->GetParameter(8));
M2Sig1->FixParameter(6,M2SigBkg1->GetParameter(9));
M2Sig1->FixParameter(7,M2SigBkg1->GetParameter(10));
M2Sig1->SetLineColor(kViolet);
M2Sig1->Draw("same");


 M2LegMass1->AddEntry(M2M_Hist1,"invariant mass","l");
 M2LegMass1->AddEntry((TObject*)0,"","");
 M2LegMass1->AddEntry(M2SigBkg1,"fit from background and signal","l");
 M2LegMass1->AddEntry((TObject*)0,"","");
 M2LegMass1->AddEntry(M2Sig1,"signal fit","l");
 M2LegMass1->AddEntry((TObject*)0,"","");
 M2LegMass1->AddEntry(M2Bkg1,"background fit","l");
 M2LegMass1->AddEntry((TObject*)0,"","");
 M2LegMass1->Draw();


cout<<"method2 hist1: number of signal events: "<<M2Sig1->Integral(xmin,xmax)/BinWidth<<endl;


if(savefile2==1) outputfileM2Mass1->cd();
//if(savefile2==1) M2M_Hist1->Write();
if(savefile2==1) M2Canvas1->Write(Form("M2M_Hist1_%d",imass));

if(iprint==1)
{
cout<<"method2 hist1 SigBkg par0: "<<M2SigBkg1->GetParameter(0)<<endl;
cout<<"method2 hist1 SigBkg par1: "<<M2SigBkg1->GetParameter(1)<<endl;
cout<<"method2 hist1 SigBkg par2: "<<M2SigBkg1->GetParameter(2)<<endl;
cout<<"method2 hist1 SigBkg par3: "<<M2SigBkg1->GetParameter(3)<<endl;
cout<<"method2 hist1 SigBkg par4: "<<M2SigBkg1->GetParameter(4)<<endl;
cout<<"method2 hist1 SigBkg par5: "<<M2SigBkg1->GetParameter(5)<<endl;
cout<<"method2 hist1 SigBkg par6: "<<M2SigBkg1->GetParameter(6)<<endl;
cout<<"method2 hist1 SigBkg par7: "<<M2SigBkg1->GetParameter(7)<<endl;
cout<<"method2 hist1 SigBkg par8: "<<M2SigBkg1->GetParameter(8)<<endl;
cout<<"method2 hist1 SigBkg par9: "<<M2SigBkg1->GetParameter(9)<<endl;
cout<<"method2 hist1 SigBkg par10: "<<M2SigBkg1->GetParameter(10)<<endl;
cout<<"method2 hist1 Bkg par0: "<<M2Bkg1->GetParameter(0)<<endl;
cout<<"method2 hist1 Bkg par1: "<<M2Bkg1->GetParameter(1)<<endl;
cout<<"method2 hist1 Bkg par2: "<<M2Bkg1->GetParameter(2)<<endl;
cout<<"method2 hist1 Sig par0: "<<M2Sig1->GetParameter(0)<<endl;
cout<<"method2 hist1 Sig par1: "<<M2Sig1->GetParameter(1)<<endl;
cout<<"method2 hist1 Sig par2: "<<M2Sig1->GetParameter(2)<<endl;
cout<<"method2 hist1 Sig par3: "<<M2Sig1->GetParameter(3)<<endl;
cout<<"method2 hist1 Sig par4: "<<M2Sig1->GetParameter(4)<<endl;
cout<<"method2 hist1 Sig par5: "<<M2Sig1->GetParameter(5)<<endl;
cout<<"method2 hist1 Sig par6: "<<M2Sig1->GetParameter(6)<<endl;
cout<<"method2 hist1 Sig par7: "<<M2Sig1->GetParameter(7)<<endl;
}

if(M2SigBkg1->GetParameter(10)<0) {
M2significance1[j]=0;
M2lnLB1=0;
M2lnLSpB1=0;
}
else
{
  CalcSignificance(xmin,xmax,hist_mass1, M2Bkg1, M2SigBkg1, M2Significance1, M2lnLB1,M2lnLSpB1);
  M2significance1[j]=M2Significance1;
  M2nevents1[j] = M2Sig1->Integral(xmin,xmax)/BinWidth;
}
cout<<"M2significance1 nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M2significance1[j]<<endl;


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
M2M_divHist1->SetName(Form("M2M_divHist1_%d",imass));
M2M_divHist1->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M2M_divHist1->Draw();
M2M_divHist1Signal->Draw("samehist");
if(savefile2==1) outputfileM2Pull1->cd();
if(savefile2==1) M2CanvasDiv1->Write(Form("M2M_divHist1_%d",imass));


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
 CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass1, hist_mass1Rebin, M2Bkg1,M2SigBkg1,M2M_divHist1Rebin,M2M_divHist1SignalRebin, chisquare);

 M2chisq1[j] = chisquare;
 cout << "M2 rebinned chisq bin 1 = " << chisquare << endl;


M2M_divHist1Rebin->SetName(Form("M2M_divHist1Rebin_%d",imass));
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
if(savefile2==1) M2CanvasDiv1Rebin->Write(Form("M2M_divHist1Rebin_%d",imass));



//  ***********                   **************


M2SigBkg2->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],50.);
M2SigBkg2->FixParameter(3,sMean[j]);
M2SigBkg2->FixParameter(4,sSigma[j]);
M2SigBkg2->FixParameter(5,sAlphaHigh[j]);
M2SigBkg2->FixParameter(6,sAlphaLow[j]);
M2SigBkg2->FixParameter(7,sNHigh[j]);
M2SigBkg2->FixParameter(8,sNLow[j]);
M2SigBkg2->FixParameter(9,sFrac[j]);


M2M_Hist2->SetName(Form("M2M_Hist2_%d",imass));
M2Canvas2->cd();
gPad->SetLogy();
M2SigBkg2->SetLineColor(kRed);
M2M_Hist2->Fit("M2SigBkg2","L","",xmin,xmax);
M2M_Hist2->Fit("M2SigBkg2","L","",xmin,xmax);
M2M_Hist2->Fit("M2SigBkg2","L","",xmin,xmax);

M2Bkg2->FixParameter(0,M2SigBkg2->GetParameter(0));
M2Bkg2->FixParameter(1,M2SigBkg2->GetParameter(1));
M2Bkg2->FixParameter(2,M2SigBkg2->GetParameter(2));
M2Bkg2->SetLineColor(kGreen);
M2Bkg2->Draw("same");

M2Sig2->FixParameter(0,M2SigBkg2->GetParameter(3));
M2Sig2->FixParameter(1,M2SigBkg2->GetParameter(4));
M2Sig2->FixParameter(2,M2SigBkg2->GetParameter(5));
M2Sig2->FixParameter(3,M2SigBkg2->GetParameter(6));
M2Sig2->FixParameter(4,M2SigBkg2->GetParameter(7));
M2Sig2->FixParameter(5,M2SigBkg2->GetParameter(8));
M2Sig2->FixParameter(6,M2SigBkg2->GetParameter(9));
M2Sig2->FixParameter(7,M2SigBkg2->GetParameter(10));
M2Sig2->SetLineColor(kViolet);
M2Sig2->Draw("same");


 M2LegMass2->AddEntry(M2M_Hist2,"invariant mass","l");
 M2LegMass2->AddEntry((TObject*)0,"","");
 M2LegMass2->AddEntry(M2SigBkg2,"fit from background and signal","l");
 M2LegMass2->AddEntry((TObject*)0,"","");
 M2LegMass2->AddEntry(M2Sig2,"signal fit","l");
 M2LegMass2->AddEntry((TObject*)0,"","");
 M2LegMass2->AddEntry(M2Bkg2,"background fit","l");
 M2LegMass2->AddEntry((TObject*)0,"","");
 M2LegMass2->Draw();



cout<<"method2 hist2: number of signal events: "<<M2Sig2->Integral(xmin,xmax)/BinWidth<<endl;

if(savefile2==1) outputfileM2Mass2->cd();
//if(savefile2==1) M2M_Hist2->Write();
if(savefile2==1) M2Canvas2->Write(Form("M2M_Hist2_%d",imass));

if(iprint==1)
{
cout<<"method2 hist2 SigBkg par0: "<<M2SigBkg2->GetParameter(0)<<endl;
cout<<"method2 hist2 SigBkg par1: "<<M2SigBkg2->GetParameter(1)<<endl;
cout<<"method2 hist2 SigBkg par2: "<<M2SigBkg2->GetParameter(2)<<endl;
cout<<"method2 hist2 SigBkg par3: "<<M2SigBkg2->GetParameter(3)<<endl;
cout<<"method2 hist2 SigBkg par4: "<<M2SigBkg2->GetParameter(4)<<endl;
cout<<"method2 hist2 SigBkg par5: "<<M2SigBkg2->GetParameter(5)<<endl;
cout<<"method2 hist2 SigBkg par6: "<<M2SigBkg2->GetParameter(6)<<endl;
cout<<"method2 hist2 SigBkg par7: "<<M2SigBkg2->GetParameter(7)<<endl;
cout<<"method2 hist2 SigBkg par8: "<<M2SigBkg2->GetParameter(8)<<endl;
cout<<"method2 hist2 SigBkg par9: "<<M2SigBkg2->GetParameter(9)<<endl;
cout<<"method2 hist2 SigBkg par10: "<<M2SigBkg2->GetParameter(10)<<endl;
cout<<"method2 hist2 Bkg par0: "<<M2Bkg2->GetParameter(0)<<endl;
cout<<"method2 hist2 Bkg par1: "<<M2Bkg2->GetParameter(1)<<endl;
cout<<"method2 hist2 Bkg par2: "<<M2Bkg2->GetParameter(2)<<endl;
cout<<"method2 hist2 Sig par0: "<<M2Sig2->GetParameter(0)<<endl;
cout<<"method2 hist2 Sig par1: "<<M2Sig2->GetParameter(1)<<endl;
cout<<"method2 hist2 Sig par2: "<<M2Sig2->GetParameter(2)<<endl;
cout<<"method2 hist2 Sig par3: "<<M2Sig2->GetParameter(3)<<endl;
cout<<"method2 hist2 Sig par4: "<<M2Sig2->GetParameter(4)<<endl;
cout<<"method2 hist2 Sig par5: "<<M2Sig2->GetParameter(5)<<endl;
cout<<"method2 hist2 Sig par6: "<<M2Sig2->GetParameter(6)<<endl;
cout<<"method2 hist2 Sig par7: "<<M2Sig2->GetParameter(7)<<endl;
}

if(M2SigBkg2->GetParameter(10)<0) {
M2significance2[j]=0;
M2lnLB2=0;
M2lnLSpB2=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass2, M2Bkg2, M2SigBkg2, M2Significance2, M2lnLB2,M2lnLSpB2);
M2significance2[j]=M2Significance2;
  M2nevents2[j] = M2Sig2->Integral(xmin,xmax)/BinWidth;
}
cout<<"M2significance2 nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M2significance2[j]<<endl;


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
M2M_divHist2->SetName(Form("M2M_divHist2_%d",imass));
M2M_divHist2->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M2M_divHist2->Draw();
M2M_divHist2Signal->Draw("samehist");
if(savefile2==1) outputfileM2Pull2->cd();
if(savefile2==1) M2CanvasDiv2->Write(Form("M2M_divHist2_%d",imass));


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
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass2, hist_mass2Rebin, M2Bkg2,M2SigBkg2,M2M_divHist2Rebin,M2M_divHist2SignalRebin, chisquare);

M2chisq2[j] = chisquare;
 cout << "M2 rebinned chisq bin 2 = " << chisquare << endl;

M2M_divHist2Rebin->SetName(Form("M2M_divHist2Rebin_%d",imass));
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
if(savefile2==1) M2CanvasDiv2Rebin->Write(Form("M2M_divHist2Rebin_%d",imass));


//  **************                      ***************


M2SigBkg3->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],50.);
M2SigBkg3->FixParameter(3,sMean[j]);
M2SigBkg3->FixParameter(4,sSigma[j]);
M2SigBkg3->FixParameter(5,sAlphaHigh[j]);
M2SigBkg3->FixParameter(6,sAlphaLow[j]);
M2SigBkg3->FixParameter(7,sNHigh[j]);
M2SigBkg3->FixParameter(8,sNLow[j]);
M2SigBkg3->FixParameter(9,sFrac[j]);


M2M_Hist3->SetName(Form("M2M_Hist3_%d",imass));
M2Canvas3->cd();
gPad->SetLogy();
M2SigBkg3->SetLineColor(kRed);
M2M_Hist3->Fit("M2SigBkg3","L","",xmin,xmax);
M2M_Hist3->Fit("M2SigBkg3","L","",xmin,xmax);
M2M_Hist3->Fit("M2SigBkg3","L","",xmin,xmax);

M2Bkg3->FixParameter(0,M2SigBkg3->GetParameter(0));
M2Bkg3->FixParameter(1,M2SigBkg3->GetParameter(1));
M2Bkg3->FixParameter(2,M2SigBkg3->GetParameter(2));
M2Bkg3->SetLineColor(kGreen);
M2Bkg3->Draw("same");

M2Sig3->FixParameter(0,M2SigBkg3->GetParameter(3));
M2Sig3->FixParameter(1,M2SigBkg3->GetParameter(4));
M2Sig3->FixParameter(2,M2SigBkg3->GetParameter(5));
M2Sig3->FixParameter(3,M2SigBkg3->GetParameter(6));
M2Sig3->FixParameter(4,M2SigBkg3->GetParameter(7));
M2Sig3->FixParameter(5,M2SigBkg3->GetParameter(8));
M2Sig3->FixParameter(6,M2SigBkg3->GetParameter(9));
M2Sig3->FixParameter(7,M2SigBkg3->GetParameter(10));
M2Sig3->SetLineColor(kViolet);
M2Sig3->Draw("same");


 M2LegMass3->AddEntry(M2M_Hist3,"invariant mass","l");
 M2LegMass3->AddEntry((TObject*)0,"","");
 M2LegMass3->AddEntry(M2SigBkg3,"fit from background and signal","l");
 M2LegMass3->AddEntry((TObject*)0,"","");
 M2LegMass3->AddEntry(M2Sig3,"signal fit","l");
 M2LegMass3->AddEntry((TObject*)0,"","");
 M2LegMass3->AddEntry(M2Bkg3,"background fit","l");
 M2LegMass3->AddEntry((TObject*)0,"","");
 M2LegMass3->Draw();


cout<<"method2 hist3: number of signal events: "<<M2Sig3->Integral(xmin,xmax)/BinWidth<<endl;


if(savefile2==1) outputfileM2Mass3->cd();
//if(savefile2==1) M2M_Hist3->Write();
if(savefile2==1) M2Canvas3->Write(Form("M2M_Hist3_%d",imass));

if(iprint==1)
{
cout<<"method2 hist3 SigBkg par0: "<<M2SigBkg3->GetParameter(0)<<endl;
cout<<"method2 hist3 SigBkg par1: "<<M2SigBkg3->GetParameter(1)<<endl;
cout<<"method2 hist3 SigBkg par2: "<<M2SigBkg3->GetParameter(2)<<endl;
cout<<"method2 hist3 SigBkg par3: "<<M2SigBkg3->GetParameter(3)<<endl;
cout<<"method2 hist3 SigBkg par4: "<<M2SigBkg3->GetParameter(4)<<endl;
cout<<"method2 hist3 SigBkg par5: "<<M2SigBkg3->GetParameter(5)<<endl;
cout<<"method2 hist3 SigBkg par6: "<<M2SigBkg3->GetParameter(6)<<endl;
cout<<"method2 hist3 SigBkg par7: "<<M2SigBkg3->GetParameter(7)<<endl;
cout<<"method2 hist3 SigBkg par8: "<<M2SigBkg3->GetParameter(8)<<endl;
cout<<"method2 hist3 SigBkg par9: "<<M2SigBkg3->GetParameter(9)<<endl;
cout<<"method2 hist3 SigBkg par10: "<<M2SigBkg3->GetParameter(10)<<endl;
cout<<"method2 hist3 Bkg par0: "<<M2Bkg3->GetParameter(0)<<endl;
cout<<"method2 hist3 Bkg par1: "<<M2Bkg3->GetParameter(1)<<endl;
cout<<"method2 hist3 Bkg par2: "<<M2Bkg3->GetParameter(2)<<endl;
cout<<"method2 hist3 Sig par0: "<<M2Sig3->GetParameter(0)<<endl;
cout<<"method2 hist3 Sig par1: "<<M2Sig3->GetParameter(1)<<endl;
cout<<"method2 hist3 Sig par2: "<<M2Sig3->GetParameter(2)<<endl;
cout<<"method2 hist3 Sig par3: "<<M2Sig3->GetParameter(3)<<endl;
cout<<"method2 hist3 Sig par4: "<<M2Sig3->GetParameter(4)<<endl;
cout<<"method2 hist3 Sig par5: "<<M2Sig3->GetParameter(5)<<endl;
cout<<"method2 hist3 Sig par6: "<<M2Sig3->GetParameter(6)<<endl;
cout<<"method2 hist3 Sig par7: "<<M2Sig3->GetParameter(7)<<endl;
}


if(M2SigBkg3->GetParameter(10)<0) {
M2significance3[j]=0;
M2lnLB3=0;
M2lnLSpB3=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass3, M2Bkg3, M2SigBkg3, M2Significance3, M2lnLB3,M2lnLSpB3);
M2significance3[j]=M2Significance3;
  M2nevents3[j] = M2Sig3->Integral(xmin,xmax)/BinWidth;
}
cout<<"M2significance3 nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M2significance3[j]<<endl;

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
M2M_divHist3->SetName(Form("M2M_divHist3_%d",imass));
M2M_divHist3->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M2M_divHist3->Draw();
M2M_divHist3Signal->Draw("samehist");
if(savefile2==1) outputfileM2Pull3->cd();
if(savefile2==1) M2CanvasDiv3->Write(Form("M2M_divHist3_%d",imass));


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
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass3, hist_mass3Rebin, M2Bkg3,M2SigBkg3,M2M_divHist3Rebin,M2M_divHist3SignalRebin, chisquare);

M2chisq3[j] = chisquare;
 
cout << "M2 rebinned chisq = " << chisquare << endl;


M2M_divHist3Rebin->SetName(Form("M2M_divHist3Rebin_%d",imass));
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
if(savefile2==1) M2CanvasDiv3Rebin->Write(Form("M2M_divHist3Rebin_%d",imass));


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


M3SigBkg0->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],50.);
M3SigBkg0->FixParameter(3,sMean[j]);
M3SigBkg0->FixParameter(4,sSigma[j]);
M3SigBkg0->FixParameter(5,sAlphaHigh[j]);
M3SigBkg0->FixParameter(6,sAlphaLow[j]);
M3SigBkg0->FixParameter(7,sNHigh[j]);
M3SigBkg0->FixParameter(8,sNLow[j]);
M3SigBkg0->FixParameter(9,sFrac[j]);


M3M_Hist->SetName(Form("M3M_Hist_%d",imass));
M3Canvas0->cd();
gPad->SetLogy();
M3SigBkg0->SetLineColor(kRed);
M3M_Hist->Fit("M3SigBkg0","L","",xmin,xmax);
norm0=M3SigBkg0->GetParameter(10);
norm1=norm0*r1;
norm2=norm0*r2;
norm3=norm0*r3;


 M3LegMass0->AddEntry(M3M_Hist,"invariant mass","l");
 M3LegMass0->AddEntry((TObject*)0,"","");
 M3LegMass0->AddEntry(M3SigBkg0,"fit from background and signal","l");
 M3LegMass0->AddEntry((TObject*)0,"","");
 M3LegMass0->Draw();


M3SigBkg1->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],norm1);
M3SigBkg1->FixParameter(3,sMean[j]);
M3SigBkg1->FixParameter(4,sSigma[j]);
M3SigBkg1->FixParameter(5,sAlphaHigh[j]);
M3SigBkg1->FixParameter(6,sAlphaLow[j]);
M3SigBkg1->FixParameter(7,sNHigh[j]);
M3SigBkg1->FixParameter(8,sNLow[j]);
M3SigBkg1->FixParameter(9,sFrac[j]);
M3SigBkg1->FixParameter(10,norm1);

M3Canvas1->cd();
gPad->SetLogy();
M3SigBkg1->SetLineColor(kRed);
M3M_Hist1->Fit("M3SigBkg1","L","",xmin,xmax);

M3Bkg1->FixParameter(0,M3SigBkg1->GetParameter(0));
M3Bkg1->FixParameter(1,M3SigBkg1->GetParameter(1));
M3Bkg1->FixParameter(2,M3SigBkg1->GetParameter(2));
M3Bkg1->SetLineColor(kGreen);
M3Bkg1->Draw("same");

M3Sig1->FixParameter(0,M3SigBkg1->GetParameter(3));
M3Sig1->FixParameter(1,M3SigBkg1->GetParameter(4));
M3Sig1->FixParameter(2,M3SigBkg1->GetParameter(5));
M3Sig1->FixParameter(3,M3SigBkg1->GetParameter(6));
M3Sig1->FixParameter(4,M3SigBkg1->GetParameter(7));
M3Sig1->FixParameter(5,M3SigBkg1->GetParameter(8));
M3Sig1->FixParameter(6,M3SigBkg1->GetParameter(9));
M3Sig1->FixParameter(7,M3SigBkg1->GetParameter(10));
M3Sig1->SetLineColor(kViolet);
M3Sig1->Draw("same");


 M3LegMass1->AddEntry(M3M_Hist1,"invariant mass","l");
 M3LegMass1->AddEntry((TObject*)0,"","");
 M3LegMass1->AddEntry(M3SigBkg1,"fit from background and signal","l");
 M3LegMass1->AddEntry((TObject*)0,"","");
 M3LegMass1->AddEntry(M3Sig1,"signal fit","l");
 M3LegMass1->AddEntry((TObject*)0,"","");
 M3LegMass1->AddEntry(M3Bkg1,"background fit","l");
 M3LegMass1->AddEntry((TObject*)0,"","");
 M3LegMass1->Draw();


if(savefile3==1) outputfileM3Mass1->cd();
//if(savefile3==1) M3M_Hist1->Write();
if(savefile3==1) M3Canvas1->Write(Form("M3M_Hist1_%d",imass));



cout<<"method3 hist1: number of signal events: "<<M3Sig1->Integral(xmin,xmax)/BinWidth<<endl;

if(iprint==1)
{
cout<<"method3 hist1 SigBkg par0: "<<M3SigBkg1->GetParameter(0)<<endl;
cout<<"method3 hist1 SigBkg par1: "<<M3SigBkg1->GetParameter(1)<<endl;
cout<<"method3 hist1 SigBkg par2: "<<M3SigBkg1->GetParameter(2)<<endl;
cout<<"method3 hist1 SigBkg par3: "<<M3SigBkg1->GetParameter(3)<<endl;
cout<<"method3 hist1 SigBkg par4: "<<M3SigBkg1->GetParameter(4)<<endl;
cout<<"method3 hist1 SigBkg par5: "<<M3SigBkg1->GetParameter(5)<<endl;
cout<<"method3 hist1 SigBkg par6: "<<M3SigBkg1->GetParameter(6)<<endl;
cout<<"method3 hist1 SigBkg par7: "<<M3SigBkg1->GetParameter(7)<<endl;
cout<<"method3 hist1 SigBkg par8: "<<M3SigBkg1->GetParameter(8)<<endl;
cout<<"method3 hist1 SigBkg par9: "<<M3SigBkg1->GetParameter(9)<<endl;
cout<<"method3 hist1 SigBkg par10: "<<M3SigBkg1->GetParameter(10)<<endl;
cout<<"method3 hist1 Bkg par0: "<<M3Bkg1->GetParameter(0)<<endl;
cout<<"method3 hist1 Bkg par1: "<<M3Bkg1->GetParameter(1)<<endl;
cout<<"method3 hist1 Bkg par2: "<<M3Bkg1->GetParameter(2)<<endl;
cout<<"method3 hist1 Sig par0: "<<M3Sig1->GetParameter(0)<<endl;
cout<<"method3 hist1 Sig par1: "<<M3Sig1->GetParameter(1)<<endl;
cout<<"method3 hist1 Sig par2: "<<M3Sig1->GetParameter(2)<<endl;
cout<<"method3 hist1 Sig par3: "<<M3Sig1->GetParameter(3)<<endl;
cout<<"method3 hist1 Sig par4: "<<M3Sig1->GetParameter(4)<<endl;
cout<<"method3 hist1 Sig par5: "<<M3Sig1->GetParameter(5)<<endl;
cout<<"method3 hist1 Sig par6: "<<M3Sig1->GetParameter(6)<<endl;
cout<<"method3 hist1 Sig par7: "<<M3Sig1->GetParameter(7)<<endl;
}

if(M3SigBkg1->GetParameter(10)<0) {
M3significance1[j]=0;
M3lnLB1=0;
M3lnLSpB1=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass1, M3Bkg1, M3SigBkg1, M3Significance1, M3lnLB1,M3lnLSpB1);
M3significance1[j]=M3Significance1;
}
cout<<"M3significance1 nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M3significance1[j]<<endl;

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
M3M_divHist1->SetName(Form("M3M_divHist1_%d",imass));
M3M_divHist1->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M3M_divHist1->Draw();
M3M_divHist1Signal->Draw("samehist");
if(savefile3==1) outputfileM3Pull1->cd();
if(savefile3==1) M3CanvasDiv1->Write(Form("M3M_divHist1_%d",imass));


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
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass1, hist_mass1Rebin, M3Bkg1,M3SigBkg1,M3M_divHist1Rebin,M3M_divHist1SignalRebin, chisquare);

M3chisq1[j] = chisquare;

 cout << "M3 rebinned chisq bin 1 = " << chisquare << endl;

M3M_divHist1Rebin->SetName(Form("M3M_divHist1Rebin_%d",imass));
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
if(savefile3==1) M3CanvasDiv1Rebin->Write(Form("M3M_divHist1Rebin_%d",imass));


//  ***********                   **************


M3SigBkg2->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],norm2);
M3SigBkg2->FixParameter(3,sMean[j]);
M3SigBkg2->FixParameter(4,sSigma[j]);
M3SigBkg2->FixParameter(5,sAlphaHigh[j]);
M3SigBkg2->FixParameter(6,sAlphaLow[j]);
M3SigBkg2->FixParameter(7,sNHigh[j]);
M3SigBkg2->FixParameter(8,sNLow[j]);
M3SigBkg2->FixParameter(9,sFrac[j]);
M3SigBkg2->FixParameter(10,norm2);

M3M_Hist2->SetName(Form("M3M_Hist2_%d",imass));
M3Canvas2->cd();
gPad->SetLogy();
M3SigBkg2->SetLineColor(kRed);
M3M_Hist2->Fit("M3SigBkg2","L","",xmin,xmax);

M3Bkg2->FixParameter(0,M3SigBkg2->GetParameter(0));
M3Bkg2->FixParameter(1,M3SigBkg2->GetParameter(1));
M3Bkg2->FixParameter(2,M3SigBkg2->GetParameter(2));
M3Bkg2->SetLineColor(kGreen);
M3Bkg2->Draw("same");

M3Sig2->FixParameter(0,M3SigBkg2->GetParameter(3));
M3Sig2->FixParameter(1,M3SigBkg2->GetParameter(4));
M3Sig2->FixParameter(2,M3SigBkg2->GetParameter(5));
M3Sig2->FixParameter(3,M3SigBkg2->GetParameter(6));
M3Sig2->FixParameter(4,M3SigBkg2->GetParameter(7));
M3Sig2->FixParameter(5,M3SigBkg2->GetParameter(8));
M3Sig2->FixParameter(6,M3SigBkg2->GetParameter(9));
M3Sig2->FixParameter(7,M3SigBkg2->GetParameter(10));
M3Sig2->SetLineColor(kViolet);
M3Sig2->Draw("same");

 M3LegMass2->AddEntry(M3M_Hist2,"invariant mass","l");
 M3LegMass2->AddEntry((TObject*)0,"","");
 M3LegMass2->AddEntry(M3SigBkg2,"fit from background and signal","l");
 M3LegMass2->AddEntry((TObject*)0,"","");
 M3LegMass2->AddEntry(M3Sig2,"signal fit","l");
 M3LegMass2->AddEntry((TObject*)0,"","");
 M3LegMass2->AddEntry(M3Bkg2,"background fit","l");
 M3LegMass2->AddEntry((TObject*)0,"","");
 M3LegMass2->Draw();

cout<<"method3 hist2: number of signal events: "<<M3Sig2->Integral(xmin,xmax)/BinWidth<<endl;


if(savefile3==1) outputfileM3Mass2->cd();
if(savefile3==1) M3M_Hist2->Write();
if(savefile3==1) M3Canvas2->Write(Form("M3M_Hist2_%d",imass));


if(iprint==1)
{
cout<<"method3 hist2 SigBkg par0: "<<M3SigBkg2->GetParameter(0)<<endl;
cout<<"method3 hist2 SigBkg par1: "<<M3SigBkg2->GetParameter(1)<<endl;
cout<<"method3 hist2 SigBkg par2: "<<M3SigBkg2->GetParameter(2)<<endl;
cout<<"method3 hist2 SigBkg par3: "<<M3SigBkg2->GetParameter(3)<<endl;
cout<<"method3 hist2 SigBkg par4: "<<M3SigBkg2->GetParameter(4)<<endl;
cout<<"method3 hist2 SigBkg par5: "<<M3SigBkg2->GetParameter(5)<<endl;
cout<<"method3 hist2 SigBkg par6: "<<M3SigBkg2->GetParameter(6)<<endl;
cout<<"method3 hist2 SigBkg par7: "<<M3SigBkg2->GetParameter(7)<<endl;
cout<<"method3 hist2 SigBkg par8: "<<M3SigBkg2->GetParameter(8)<<endl;
cout<<"method3 hist2 SigBkg par9: "<<M3SigBkg2->GetParameter(9)<<endl;
cout<<"method3 hist2 SigBkg par10: "<<M3SigBkg2->GetParameter(10)<<endl;
cout<<"method3 hist2 Bkg par0: "<<M3Bkg2->GetParameter(0)<<endl;
cout<<"method3 hist2 Bkg par1: "<<M3Bkg2->GetParameter(1)<<endl;
cout<<"method3 hist2 Bkg par2: "<<M3Bkg2->GetParameter(2)<<endl;
cout<<"method3 hist2 Sig par0: "<<M3Sig2->GetParameter(0)<<endl;
cout<<"method3 hist2 Sig par1: "<<M3Sig2->GetParameter(1)<<endl;
cout<<"method3 hist2 Sig par2: "<<M3Sig2->GetParameter(2)<<endl;
cout<<"method3 hist2 Sig par3: "<<M3Sig2->GetParameter(3)<<endl;
cout<<"method3 hist2 Sig par4: "<<M3Sig2->GetParameter(4)<<endl;
cout<<"method3 hist2 Sig par5: "<<M3Sig2->GetParameter(5)<<endl;
cout<<"method3 hist2 Sig par6: "<<M3Sig2->GetParameter(6)<<endl;
cout<<"method3 hist2 Sig par7: "<<M3Sig2->GetParameter(7)<<endl;
}

if(M3SigBkg2->GetParameter(10)<0) {
M3significance2[j]=0;
M3lnLB2=0;
M3lnLSpB2=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass2, M3Bkg2, M3SigBkg2, M3Significance2, M3lnLB2,M3lnLSpB2);
M3significance2[j]=M3Significance2;
}
cout<<"M3significance2 nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M3significance2[j]<<endl;


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
M3M_divHist2->SetName(Form("M3M_divHist2_%d",imass));
M3M_divHist2->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M3M_divHist2->Draw();
M3M_divHist2Signal->Draw("samehist");
if(savefile3==1) outputfileM3Pull2->cd();
if(savefile3==1) M3CanvasDiv2->Write(Form("M3M_divHist2_%d",imass));


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
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass2, hist_mass2Rebin, M3Bkg2,M3SigBkg2,M3M_divHist2Rebin,M3M_divHist2SignalRebin, chisquare);

M3chisq2[j] = chisquare;

 cout << "M3 rebinned chisq bin 2 = " << chisquare << endl;

M3M_divHist2Rebin->SetName(Form("M3M_divHist2Rebin_%d",imass));
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
if(savefile3==1) M3CanvasDiv2Rebin->Write(Form("M3M_divHist2Rebin_%d",imass));

//  **************                      ***************


M3SigBkg3->SetParameters(2,7,5,sMean[j],sSigma[j],sAlphaHigh[j],sAlphaLow[j],sNHigh[j],sNLow[j],sFrac[j],norm3);
M3SigBkg3->FixParameter(3,sMean[j]);
M3SigBkg3->FixParameter(4,sSigma[j]);
M3SigBkg3->FixParameter(5,sAlphaHigh[j]);
M3SigBkg3->FixParameter(6,sAlphaLow[j]);
M3SigBkg3->FixParameter(7,sNHigh[j]);
M3SigBkg3->FixParameter(8,sNLow[j]);
M3SigBkg3->FixParameter(9,sFrac[j]);
M3SigBkg3->FixParameter(10,norm3);

M3M_Hist3->SetName(Form("M3M_Hist3_%d",imass));
M3Canvas3->cd();
gPad->SetLogy();
M3SigBkg3->SetLineColor(kRed);
M3M_Hist3->Fit("M3SigBkg3","L","",xmin,xmax);

M3Bkg3->FixParameter(0,M3SigBkg3->GetParameter(0));
M3Bkg3->FixParameter(1,M3SigBkg3->GetParameter(1));
M3Bkg3->FixParameter(2,M3SigBkg3->GetParameter(2));
M3Bkg3->SetLineColor(kGreen);
M3Bkg3->Draw("same");

M3Sig3->FixParameter(0,M3SigBkg3->GetParameter(3));
M3Sig3->FixParameter(1,M3SigBkg3->GetParameter(4));
M3Sig3->FixParameter(2,M3SigBkg3->GetParameter(5));
M3Sig3->FixParameter(3,M3SigBkg3->GetParameter(6));
M3Sig3->FixParameter(4,M3SigBkg3->GetParameter(7));
M3Sig3->FixParameter(5,M3SigBkg3->GetParameter(8));
M3Sig3->FixParameter(6,M3SigBkg3->GetParameter(9));
M3Sig3->FixParameter(7,M3SigBkg3->GetParameter(10));
M3Sig3->SetLineColor(kViolet);
M3Sig3->Draw("same");

 M3LegMass3->AddEntry(M3M_Hist3,"invariant mass","l");
 M3LegMass3->AddEntry((TObject*)0,"","");
 M3LegMass3->AddEntry(M3SigBkg3,"fit from background and signal","l");
 M3LegMass3->AddEntry((TObject*)0,"","");
 M3LegMass3->AddEntry(M3Sig3,"signal fit","l");
 M3LegMass3->AddEntry((TObject*)0,"","");
 M3LegMass3->AddEntry(M3Bkg3,"background fit","l");
 M3LegMass3->AddEntry((TObject*)0,"","");
 M3LegMass3->Draw();


cout<<"method3 hist3: number of signal events: "<<M3Sig3->Integral(xmin,xmax)/BinWidth<<endl;


if(savefile3==1) outputfileM3Mass3->cd();
//if(savefile3==1) M3M_Hist3->Write();
if(savefile3==1) M3Canvas3->Write(Form("M3M_Hist3_%d",imass));


if(iprint==1)
{
cout<<"method3 hist3 SigBkg par0: "<<M3SigBkg3->GetParameter(0)<<endl;
cout<<"method3 hist3 SigBkg par1: "<<M3SigBkg3->GetParameter(1)<<endl;
cout<<"method3 hist3 SigBkg par2: "<<M3SigBkg3->GetParameter(2)<<endl;
cout<<"method3 hist3 SigBkg par3: "<<M3SigBkg3->GetParameter(3)<<endl;
cout<<"method3 hist3 SigBkg par4: "<<M3SigBkg3->GetParameter(4)<<endl;
cout<<"method3 hist3 SigBkg par5: "<<M3SigBkg3->GetParameter(5)<<endl;
cout<<"method3 hist3 SigBkg par6: "<<M3SigBkg3->GetParameter(6)<<endl;
cout<<"method3 hist3 SigBkg par7: "<<M3SigBkg3->GetParameter(7)<<endl;
cout<<"method3 hist3 SigBkg par8: "<<M3SigBkg3->GetParameter(8)<<endl;
cout<<"method3 hist3 SigBkg par9: "<<M3SigBkg3->GetParameter(9)<<endl;
cout<<"method3 hist3 SigBkg par10: "<<M3SigBkg3->GetParameter(10)<<endl;
cout<<"method3 hist3 Bkg par0: "<<M3Bkg3->GetParameter(0)<<endl;
cout<<"method3 hist3 Bkg par1: "<<M3Bkg3->GetParameter(1)<<endl;
cout<<"method3 hist3 Bkg par2: "<<M3Bkg3->GetParameter(2)<<endl;
cout<<"method3 hist3 Sig par0: "<<M3Sig3->GetParameter(0)<<endl;
cout<<"method3 hist3 Sig par1: "<<M3Sig3->GetParameter(1)<<endl;
cout<<"method3 hist3 Sig par2: "<<M3Sig3->GetParameter(2)<<endl;
cout<<"method3 hist3 Sig par3: "<<M3Sig3->GetParameter(3)<<endl;
cout<<"method3 hist3 Sig par4: "<<M3Sig3->GetParameter(4)<<endl;
cout<<"method3 hist3 Sig par5: "<<M3Sig3->GetParameter(5)<<endl;
cout<<"method3 hist3 Sig par6: "<<M3Sig3->GetParameter(6)<<endl;
cout<<"method3 hist3 Sig par7: "<<M3Sig3->GetParameter(7)<<endl;
}


if(M3SigBkg3->GetParameter(10)<0) {
M3significance3[j]=0;
M3lnLB3=0;
M3lnLSpB3=0;
}
else
{
CalcSignificance(xmin,xmax,hist_mass3, M3Bkg3, M3SigBkg3, M3Significance3, M3lnLB3,M3lnLSpB3);
M3significance3[j]=M3Significance3;
}
cout<<"M3significance3 nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M3significance3[j]<<endl;

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
M3M_divHist3->SetName(Form("M3M_divHist3_%d",imass));
M3M_divHist3->GetXaxis()->SetRange(TMath::Floor(xmin/BinWidth),TMath::Floor(xmax/BinWidth));
M3M_divHist3->Draw();
M3M_divHist3Signal->Draw("samehist");
if(savefile3==1) outputfileM3Pull3->cd();
if(savefile3==1) M3CanvasDiv3->Write(Form("M3M_divHist3_%d",imass));


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
CalcPullRebin(xmin,xmax,NBin,BinWidth,hist_mass3, hist_mass3Rebin, M3Bkg3,M3SigBkg3,M3M_divHist3Rebin,M3M_divHist3SignalRebin, chisquare);

M3chisq3[j] = chisquare;

 cout << "M3 rebinned chisq bin 3 = " << chisquare << endl;

M3M_divHist3Rebin->SetName(Form("M3M_divHist3Rebin_%d",imass));
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
if(savefile3==1) M3CanvasDiv3Rebin->Write(Form("M3M_divHist3Rebin_%d",imass));


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

if(igraph==1 && (fabs(mass[j]-3500) < 1e-10 || meantest != 0))
{
  if(exec2==1) graphratio(M2SigBkg1->GetParameter(10),M2SigBkg1->GetParError(10),M2SigBkg2->GetParameter(10), M2SigBkg2->GetParError(10),M2SigBkg3->GetParameter(10),M2SigBkg3->GetParError(10), outputPlots, mass[j]);
}

}// loop j

// ******************************************************************

for (int j=0;j<Nvalues;j++)
{
//if(meantest!=0) j=(meantest-minmass)/100;


if(exec1==1)
{
cout<<"M1significance nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M1significance[j]<<endl;
x1[j]=mass[j];
y1[j]=M1significance[j];
//cout<<"j: "<<j<<"  x1[j]: "<<x1[j]<<"  y1[j]: "<<y1[j]<<endl;
}
if(exec2==1)
{
cout<<"M2significance nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M2significance[j]<<endl;
x2[j]=mass[j];
y2[j]=M2significance[j];
//cout<<"j: "<<j<<"  x2[j]: "<<x2[j]<<"  y2[j]: "<<y2[j]<<endl;
}
if(exec3==1)
{
cout<<"M3significance nb "<<j+1<<" (mass = "<<mass[j]<<") : "<<M3significance[j]<<endl;
x3[j]=mass[j];
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


TGraph *M1graphEvents= new TGraph(Nvalues,x1,M1nevents);
M1graphEvents->SetTitle("method1: N events as a function of the Invariant Mass");
M1graphEvents->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M1graphEvents->GetYaxis()->SetTitle("N events");
M1graphEvents->SetLineColor(3);
M1graphEvents->SetLineWidth(2);
M1graphEvents->SetMarkerColor(kMagenta);
M1graphEvents->SetMarkerStyle(23);
M1graphEvents->SetMarkerSize(0.7);


TGraph *M1graphChisq= new TGraph(Nvalues,x1,M1chisq);
M1graphChisq->SetTitle("method1: #chi2 as a function of the Invariant Mass");
M1graphChisq->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M1graphChisq->GetYaxis()->SetTitle("#chi2");
M1graphChisq->SetLineColor(4);
M1graphChisq->SetLineWidth(2);
M1graphChisq->SetMarkerColor(kBlue);
M1graphChisq->SetMarkerStyle(25);
M1graphChisq->SetMarkerSize(0.7);


TCanvas *M1CanvasSig = new TCanvas("M1CanvasSig","M1CanvasSig",11,51,700,500);
M1CanvasSig->SetGrid();

TCanvas *M1CanvasSupport = new TCanvas("M1CanvasSupport","M1CanvasSupport",11,51,700,500);
 M1CanvasSupport->Divide(2,1);

 M1CanvasSupport->cd(1);
 M1graphEvents->Draw("ALP");
 gPad->SetLogy(1);
 M1graphEvents->SetMinimum(0.5);
 M1graphEvents->SetMaximum(10000.);

 M1CanvasSupport->cd(2);
 M1graphChisq->Draw("AL");
 M1graphChisq->SetMinimum(0);
 M1graphChisq->SetMaximum(80.);


M1CanvasSig->cd();
M1graph->Draw("ALP");
M1graph->SetMinimum(0);
M1graph->SetMaximum(4);
if(savefile1==1) outputfileM1Sig->cd();
if(savefile1==1) M1graph->Write();
 if(savefile1==1) {
   string output = outputPlots + "SigMethod1.png";
   M1CanvasSig->SaveAs(output.c_str());

   string output = outputPlots + "SupportMethod1.png";
   M1CanvasSupport->SaveAs(output.c_str());

 }
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




  //----------------------------------------------------------


  TGraph *M2graph1= new TGraph(Nvalues,x3,M2significance1);
  M2graph1->SetTitle("method3: Significance as a function of the Invariant Mass");
  M2graph1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2graph1->GetYaxis()->SetTitle("Significance");
  M2graph1->SetLineColor(2);
  M2graph1->SetLineWidth(2);
  M2graph1->SetMarkerColor(4);
  M2graph1->SetMarkerStyle(21);
  M2graph1->SetMarkerSize(0.7);
  
  TCanvas *M2CanvasSig1 = new TCanvas("M2CanvasSig1","M2CanvasSig1",11,51,700,500);
  M2CanvasSig1->SetGrid();
  
  M2CanvasSig1->cd();
  M2graph1->Draw("ALP");
  M2graph1->SetMinimum(0);
  M2graph1->SetMaximum(4);
  

  TGraph *M2graph2= new TGraph(Nvalues,x3,M2significance2);
  M2graph2->SetTitle("method3: Significance as a function of the Invariant Mass");
  M2graph2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2graph2->GetYaxis()->SetTitle("Significance");
  M2graph2->SetLineColor(2);
  M2graph2->SetLineWidth(2);
  M2graph2->SetMarkerColor(4);
  M2graph2->SetMarkerStyle(21);
  M2graph2->SetMarkerSize(0.7);
  
  TCanvas *M2CanvasSig2 = new TCanvas("M2CanvasSig2","M2CanvasSig2",11,51,700,500);
  M2CanvasSig2->SetGrid();
  
  M2CanvasSig2->cd();
  M2graph2->Draw("ALP");
  M2graph2->SetMinimum(0);
  M2graph2->SetMaximum(4);

  TGraph *M2graph3= new TGraph(Nvalues,x3,M2significance3);
  M2graph3->SetTitle("method3: Significance as a function of the Invariant Mass");
  M2graph3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M2graph3->GetYaxis()->SetTitle("Significance");
  M2graph3->SetLineColor(2);
  M2graph3->SetLineWidth(2);
  M2graph3->SetMarkerColor(4);
  M2graph3->SetMarkerStyle(21);
  M2graph3->SetMarkerSize(0.7);
  
  TCanvas *M2CanvasSig3 = new TCanvas("M2CanvasSig3","M2CanvasSig3",11,51,700,500);
  M2CanvasSig3->SetGrid();
  
  M2CanvasSig3->cd();
  M2graph3->Draw("ALP");
  M2graph3->SetMinimum(0);
  M2graph3->SetMaximum(4);




TGraph *M2graphEvents1= new TGraph(Nvalues,x1,M2nevents1);
M2graphEvents1->SetTitle("method1: N events as a function of the Invariant Mass bin 1");
M2graphEvents1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graphEvents1->GetYaxis()->SetTitle("N events");
M2graphEvents1->SetLineColor(kGreen);
M2graphEvents1->SetLineWidth(2);
M2graphEvents1->SetMarkerColor(kGreen);
M2graphEvents1->SetMarkerStyle(21);
M2graphEvents1->SetMarkerSize(1.5);


TGraph *M2graphChisq1= new TGraph(Nvalues,x1,M2chisq1);
M2graphChisq1->SetTitle("method1: #chi2 as a function of the Invariant Mass bin 1");
M2graphChisq1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graphChisq1->GetYaxis()->SetTitle("#chi2");
M2graphChisq1->SetLineColor(kGreen);
M2graphChisq1->SetLineWidth(2);
M2graphChisq1->SetMarkerColor(kGreen);
M2graphChisq1->SetMarkerStyle(21);
M2graphChisq1->SetMarkerSize(1.5);



TGraph *M2graphEvents2= new TGraph(Nvalues,x1,M2nevents2);
M2graphEvents2->SetTitle("method2: N events as a function of the Invariant Mass bin 2");
M2graphEvents2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graphEvents2->GetYaxis()->SetTitle("N events");
M2graphEvents2->SetLineColor(kBlue);
M2graphEvents2->SetLineWidth(2);
M2graphEvents2->SetMarkerColor(kBlue);
M2graphEvents2->SetMarkerStyle(25);
M2graphEvents2->SetMarkerSize(1.5);


TGraph *M2graphChisq2= new TGraph(Nvalues,x1,M2chisq2);
M2graphChisq2->SetTitle("method2: #chi2 as a function of the Invariant Mass bin 2");
M2graphChisq2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graphChisq2->GetYaxis()->SetTitle("#chi2");
M2graphChisq2->SetLineColor(kBlue);
M2graphChisq2->SetLineWidth(2);
M2graphChisq2->SetMarkerColor(kBlue);
M2graphChisq2->SetMarkerStyle(25);
M2graphChisq2->SetMarkerSize(1.5);

TGraph *M2graphEvents3= new TGraph(Nvalues,x1,M2nevents3);
M2graphEvents3->SetTitle("method2: N events as a function of the Invariant Mass bin 3");
M2graphEvents3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graphEvents3->GetYaxis()->SetTitle("N events");
M2graphEvents3->SetLineColor(kRed);
M2graphEvents3->SetLineWidth(2);
M2graphEvents3->SetMarkerColor(kRed);
M2graphEvents3->SetMarkerStyle(23);
M2graphEvents3->SetMarkerSize(1.5);


TGraph *M2graphChisq3= new TGraph(Nvalues,x1,M2chisq3);
M2graphChisq3->SetTitle("method2: #chi2 as a function of the Invariant Mass bin 3");
M2graphChisq3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
M2graphChisq3->GetYaxis()->SetTitle("#chi2");
M2graphChisq3->SetLineColor(kRed);
M2graphChisq3->SetLineWidth(2);
M2graphChisq3->SetMarkerColor(kRed);
M2graphChisq3->SetMarkerStyle(23);
M2graphChisq3->SetMarkerSize(1.5);



 
 TCanvas *M2CanvasSupport = new TCanvas("M2CanvasSupport","M2CanvasSupport",11,51,700,500);
 M2CanvasSupport->Divide(2,1);

 M2CanvasSupport->cd(1);
 M2graphEvents1->Draw("AL");
 gPad->SetLogy(1);
 M2graphEvents1->SetMinimum(0.5);
 M2graphEvents1->SetMaximum(10000.);

 M2graphEvents2->Draw("SAME");
 M2graphEvents3->Draw("SAME");



 M2CanvasSupport->cd(2);
 M2graphChisq1->Draw("AL");
 M2graphChisq1->SetMinimum(0);
 M2graphChisq1->SetMaximum(80.);
  
 M2graphChisq2->Draw("SAME");
 M2graphChisq3->Draw("SAME");

 TLegend* legend= new TLegend(0.20,0.70,0.55,0.89,"");
 legend->SetHeader("Method 2");
 legend->AddEntry(M2graphEvents1,"#eta bin 1","l");
 legend->AddEntry(M2graphEvents2,"#eta bin 2","l");
 legend->AddEntry(M2graphEvents3,"#eta bin 3","l");

 legend->Draw();

if(savefile2==1) outputfileM2Sig->cd();
if(savefile2==1) M2graph->Write();
 if(savefile2==1) {
   string output = outputPlots + "SigMethod2.png";
   M2CanvasSig->SaveAs(output.c_str());

   string output = outputPlots + "SupportMethod2.png";
   M2CanvasSupport->SaveAs(output.c_str());

   string output1 = outputPlots + "SigMethod2_Bin1.png";
   M2CanvasSig1->SaveAs(output1.c_str());
   string output2 = outputPlots + "SigMethod2_Bin2.png";
   M2CanvasSig2->SaveAs(output2.c_str());
   string output3 = outputPlots + "SigMethod2_Bin3.png";
   M2CanvasSig3->SaveAs(output3.c_str());
 }

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


  //----------------------------------------------------------


  TGraph *M3graph1= new TGraph(Nvalues,x3,M3significance1);
  M3graph1->SetTitle("method3: Significance as a function of the Invariant Mass");
  M3graph1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3graph1->GetYaxis()->SetTitle("Significance");
  M3graph1->SetLineColor(2);
  M3graph1->SetLineWidth(2);
  M3graph1->SetMarkerColor(4);
  M3graph1->SetMarkerStyle(21);
  M3graph1->SetMarkerSize(0.7);
  
  TCanvas *M3CanvasSig1 = new TCanvas("M3CanvasSig1","M3CanvasSig1",11,51,700,500);
  M3CanvasSig1->SetGrid();
  
  M3CanvasSig1->cd();
  M3graph1->Draw("ALP");
  M3graph1->SetMinimum(0);
  M3graph1->SetMaximum(4);
  

  TGraph *M3graph2= new TGraph(Nvalues,x3,M3significance2);
  M3graph2->SetTitle("method3: Significance as a function of the Invariant Mass");
  M3graph2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3graph2->GetYaxis()->SetTitle("Significance");
  M3graph2->SetLineColor(2);
  M3graph2->SetLineWidth(2);
  M3graph2->SetMarkerColor(4);
  M3graph2->SetMarkerStyle(21);
  M3graph2->SetMarkerSize(0.7);
  
  TCanvas *M3CanvasSig2 = new TCanvas("M3CanvasSig2","M3CanvasSig2",11,51,700,500);
  M3CanvasSig2->SetGrid();
  
  M3CanvasSig2->cd();
  M3graph2->Draw("ALP");
  M3graph2->SetMinimum(0);
  M3graph2->SetMaximum(4);

  TGraph *M3graph3= new TGraph(Nvalues,x3,M3significance3);
  M3graph3->SetTitle("method3: Significance as a function of the Invariant Mass");
  M3graph3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  M3graph3->GetYaxis()->SetTitle("Significance");
  M3graph3->SetLineColor(2);
  M3graph3->SetLineWidth(2);
  M3graph3->SetMarkerColor(4);
  M3graph3->SetMarkerStyle(21);
  M3graph3->SetMarkerSize(0.7);
  
  TCanvas *M3CanvasSig3 = new TCanvas("M3CanvasSig3","M3CanvasSig3",11,51,700,500);
  M3CanvasSig3->SetGrid();
  
  M3CanvasSig3->cd();
  M3graph3->Draw("ALP");
  M3graph3->SetMinimum(0);
  M3graph3->SetMaximum(4);


  if(savefile3==1) outputfileM3Sig->cd();
  if(savefile3==1) M3graph->Write();
  if(savefile3==1) {
    string output = outputPlots + "SigMethod3.png";
    M3CanvasSig->SaveAs(output.c_str());
    string output1 = outputPlots + "SigMethod3_Bin1.png";
    M3CanvasSig1->SaveAs(output1.c_str());
    string output2 = outputPlots + "SigMethod3_Bin2.png";
    M3CanvasSig2->SaveAs(output2.c_str());
    string output3 = outputPlots + "SigMethod3_Bin3.png";
    M3CanvasSig3->SaveAs(output3.c_str());
  }
  
 }

}


void CalcSignificance(double xmin, double xmax, TH1D* pData, TF1* bkg, TF1* sigbkg, double&Significance, double &lnLB, double &lnLSpB)
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


void CalcPull(double xmin, double xmax, int NBin,int binwidth,TH1D* pData, TF1* bkg,TF1* sig, TH1D*&Mdiv_Hist, TH1D*&Mdiv_HistSignal)
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


void CalcPullRebin(double xmin, double xmax, int NBin,int binwidth,TH1D* pData, TH1D* pDataRebin, TF1* bkg,TF1*sig,TH1D*&Mdiv_Hist,TH1D*&Mdiv_HistSignal, double& chisquare)
{

  chisquare = 0;

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
	  if(j%10==0) {
	    //cout<<"j: "<<j<<endl;
	    //cout<<"low edge: "<<pData->GetXaxis()->GetBinLowEdge(j-9)<<endl;
	    //cout<<"up edge: "<<pData->GetXaxis()->GetBinUpEdge(j)<<endl;
	    //cout<<"low edge: "<<pDataRebin->GetXaxis()->GetBinLowEdge(j/10)<<endl;
	    //cout<<"up edge: "<<pDataRebin->GetXaxis()->GetBinUpEdge(j/10)<<endl;
	    //cout<<"sum: "<<sumBKG<<endl;
	    //cout<<"integral: "<<integral<<endl;


	    if(pDataRebin->GetBinError(j/10)!=0){
	      //cout<<"j: "<<j<<" data: "<<pDataRebin->GetBinContent(j/10)<<" function: "<<sumBKG<<"  error: "<<pDataRebin->GetBinError(j/10)<<" div: "<<(pDataRebin->GetBinContent(j/10)-sumBKG)/pDataRebin->GetBinError(j/10)<<endl;

	      double n = pDataRebin->GetBinContent(j/10);
	      double error = 1e10;
	      float a = 0.3173/2;
    
	      if (n<25){
		double nl = n-0.5*TMath::ChisquareQuantile(a,2*n);
		double nh = 0.5*TMath::ChisquareQuantile(1-a,2*(n+1))-n;
		error = (nl + nh)/2.;
	      }
	      else{
		error = sqrt(n);
	      } 
    
    
	      double res = (n-sumBKG)/error;
    
	      Mdiv_Hist->SetBinContent(j/10, res);
	      Mdiv_Hist->SetBinError(j/10,1);
	      Mdiv_HistSignal->SetBinContent(j/10,sumSIG/error);
	      Mdiv_HistSignal->SetBinError(j/10,1);
    
	      chisquare += res*res;
    
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




Double_t CBall(Double_t *x, Double_t *p)
{
  double mean = p[0], sigma = p[1], alphaHigh = p[2], alphaLow = p[3], nHigh = p[4], nLow = p[5], fact = p[6], norm = p[7];
  
  double pull = (x[0]-mean)/sigma;

//  if (pull > -alphaLow && pull < alphaHigh) {
  if (pull > -alphaLow) {
    return norm*TMath::Exp(-pull*pull/2);
  } else if ((x[0]-mean)/sigma <= -alphaLow) {

    double A = pow(nLow/fabs(alphaLow),nLow)*TMath::Exp(-alphaLow*alphaLow/2);
    double B = nLow/fabs(alphaLow)-fabs(alphaLow);

   // return fact*norm*A*TMath::Power((B-pull),-nLow);
 return norm*A*TMath::Power((B-pull),-nLow);
 
 } /*else if(pull >=  alphaHigh) {

    double A = pow(nHigh/fabs(alphaHigh),nHigh)*TMath::Exp(-alphaHigh*alphaHigh/2);
    double B = nHigh/fabs(alphaHigh)-fabs(alphaHigh);

    return (1-fact)*norm*A*TMath::Power((B+pull),-nHigh);

  }*/
  return 0;

}


Double_t BKGpCBall(Double_t *x, Double_t *p)
{

  double p0=p[0], p1=p[1], p2=p[2],  mean = p[3], sigma = p[4], alphaHigh = p[5], alphaLow = p[6], nHigh = p[7], nLow = p[8], fact = p[9], norm = p[10];
  
  double pull = (x[0]-mean)/sigma;

 // if (pull > -alphaLow && pull < alphaHigh) {
  if (pull > -alphaLow) {
    return (norm*TMath::Exp(-pull*pull/2)+(p0*pow(1-x[0]/8000,p1))/(pow(x[0]/8000,p2)));
  } else if ((x[0]-mean)/sigma <= -alphaLow) {

    double A = pow(nLow/fabs(alphaLow),nLow)*TMath::Exp(-alphaLow*alphaLow/2);
    double B = nLow/fabs(alphaLow)-fabs(alphaLow);

  //  return (fact*norm*A*TMath::Power((B-pull),-nLow)+(p0*pow(1-x[0]/8000,p1))/(pow(x[0]/8000,p2)));
    return (norm*A*TMath::Power((B-pull),-nLow)+(p0*pow(1-x[0]/8000,p1))/(pow(x[0]/8000,p2)));
  } /*else if(pull >=  alphaHigh) {

    double A = pow(nHigh/fabs(alphaHigh),nHigh)*TMath::Exp(-alphaHigh*alphaHigh/2);
    double B = nHigh/fabs(alphaHigh)-fabs(alphaHigh);

    return ((1-fact)*norm*A*TMath::Power((B+pull),-nHigh)+(p0*pow(1-x[0]/8000,p1))/(pow(x[0]/8000,p2)));

  }*/
  return (0+(p0*pow(1-x[0]/8000,p1))/(pow(x[0]/8000,p2)));

}


void PseudoExp(double xmin, double xmax,int binwidth, double nevt, int iend,int savefile, TF1 * bkg, double sMean,double sSigma,double sAlphaHigh,double sAlphaLow,double sNHigh,double sNLow,double sFrac, double significance)
{
  TCanvas *CanvasNEW_massHist = new TCanvas("CanvasNEW_massHist","CanvasNEW_massHist",11,51,700,500);
  TCanvas *CanvasNEW_sigHist = new TCanvas("CanvasNEW_sigHist","CanvasNEW_sigHist",11,51,700,500);
double significanceOUT=0;
bkg->SetName("bkg");
double Nbins=500;
  TH1D* SigHist = new TH1D("SigHist","Significance histogram",Nbins,0,5);
  SigHist->Sumw2();
  TF1 * signal_background_fit = new TF1 ("signal_background_fit",BKGpCBall, xmin, xmax,11);
  TF1 * background_fit = new TF1 ("background_fit","[0]*pow((1-x/8000.),[1])/pow(x/8000.,[2])", xmin, xmax);

for(int i=0;i<iend;i++)
{
cout<<i<<endl;
  TH1D* MassHist = new TH1D("MassHist","Mass histogram",TMath::Floor((xmax-xmin)/binwidth),xmin,xmax);
MassHist->Sumw2();
MassHist->FillRandom("bkg",nevt);
signal_background_fit->SetParameters(2,7,5,sMean,sSigma,sAlphaHigh,sAlphaLow,sNHigh,sNLow,sFrac,50.);
signal_background_fit->FixParameter(3,sMean);
signal_background_fit->FixParameter(4,sSigma);
signal_background_fit->FixParameter(5,sAlphaHigh);
signal_background_fit->FixParameter(6,sAlphaLow);
signal_background_fit->FixParameter(7,sNHigh);
signal_background_fit->FixParameter(8,sNLow);
signal_background_fit->FixParameter(9,sFrac);
CanvasNEW_massHist->cd();
MassHist->Draw();
MassHist->Fit("signal_background_fit","L","",xmin,xmax);
bkg->Draw("same");
background_fit->FixParameter(0,signal_background_fit->GetParameter(0));
background_fit->FixParameter(1,signal_background_fit->GetParameter(1));
background_fit->FixParameter(2,signal_background_fit->GetParameter(2));

double lnLB;
double lnLSpB;
if(signal_background_fit->GetParameter(10)<0) {
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
  TFile *pvalueFile = new TFile("pvalueCB.root","recreate");
pvalueFile->cd();
CanvasNEW_sigHist->Write(Form("PvalueHist_%d",nevt));
}

}




void graphratio(double meth2p1,double meth2errorp1,double meth2p2, double meth2errorp2, double meth2p3, double meth2errorp3, string Rep, double mass)
{

double *eta2=new double[3];
double *eta3=new double[3];
double *ar2=new double[3];
double *ar3=new double[3];
double *ar3_gg=new double[3];
double *ad=new double[3];
double *eta2errorlow=new double[3];
double *eta3errorlow=new double[3];
double *eta2errorhigh=new double[3];
double *eta3errorhigh=new double[3];
double *r2errorlow=new double[3];
double *r3errorlow=new double[3];
double *r2errorhigh=new double[3];
double *r3errorhigh=new double[3];


 if (meth2p1 < 0) meth2p1 = 0;
 if (meth2p2 < 0) meth2p2 = 0;
 if (meth2p3 < 0) meth2p3 = 0;


eta2[0]=0.25;
eta2[1]=0.75;
eta2[2]=1.15;
eta3[0]=0.25;
eta3[1]=0.75;
eta3[2]=1.15;

ar2[0]=meth2p1/(meth2p1+meth2p2+meth2p3);
ar2[1]=meth2p2/(meth2p1+meth2p2+meth2p3);
ar2[2]=meth2p3/(meth2p1+meth2p2+meth2p3);

ar3[0]=r1; //ratio for signal
ar3[1]=r2;
ar3[2]=r3;

ar3_gg[0]=r1_gg; //ratio for signal
ar3_gg[1]=r2_gg;
ar3_gg[2]=r3_gg;


 ad[0]=N1/Ntot;
 ad[1]=N2/Ntot;
 ad[2]=N3/Ntot;

eta2errorlow[0]=0.25;
eta2errorlow[1]=0.25;
eta2errorlow[2]=0.15;
eta3errorlow[0]=0.25;
eta3errorlow[1]=0.25;
eta3errorlow[2]=0.15;
eta2errorhigh[0]=0.25;
eta2errorhigh[1]=0.25;
eta2errorhigh[2]=0.15;
eta3errorhigh[0]=0.25;
eta3errorhigh[1]=0.25;
eta3errorhigh[2]=0.15;

r2errorlow[0]=ar2[0]-(meth2p1-meth2errorp1)/(meth2p1-meth2errorp1+meth2p2+meth2p3);
r2errorlow[1]=ar2[1]-(meth2p2-meth2errorp2)/(meth2p1+meth2p2-meth2errorp2+meth2p3);
r2errorlow[2]=ar2[2]-(meth2p3-meth2errorp3)/(meth2p1+meth2p2+meth2p3-meth2errorp3);
r2errorhigh[0]=(meth2p1+meth2errorp1)/(meth2p1+meth2errorp1+meth2p2+meth2p3)-ar2[0];
r2errorhigh[1]=(meth2p2+meth2errorp2)/(meth2p1+meth2p2+meth2errorp2+meth2p3)-ar2[1];
r2errorhigh[2]=(meth2p3+meth2errorp3)/(meth2p1+meth2p2+meth2p3+meth2errorp3)-ar2[2];

r3errorlow[0]=0.;
r3errorlow[1]=0.;
r3errorlow[2]=0.;
r3errorhigh[0]=0.;
r3errorhigh[1]=0.;
r3errorhigh[2]=0.;



TGraphAsymmErrors *graph2= new TGraphAsymmErrors(3,eta2,ar2,eta2errorlow,eta2errorhigh,r2errorlow,r2errorhigh);
TGraphAsymmErrors *qstar= new TGraphAsymmErrors(3,eta2,ar3,eta3errorlow,eta3errorhigh,r3errorlow,r3errorhigh);
TGraphAsymmErrors *rsg= new TGraphAsymmErrors(3,eta2,ar3_gg,eta3errorlow,eta3errorhigh,r3errorlow,r3errorhigh);
TGraphAsymmErrors *data= new TGraphAsymmErrors(3,eta2,ad,eta3errorlow,eta3errorhigh,r3errorlow,r3errorhigh);


TCanvas *CanvasRatio = new TCanvas("CanvasRatio","CanvasRatio",11,51,700,500);
TLegend *LegRatio = new TLegend(0.4,0.6,0.6,0.8);
graph2->SetTitle("Signal ratio");
graph2->GetXaxis()->SetTitle("#Delta#eta");
graph2->GetYaxis()->SetTitle("");
graph2->SetLineColor(2);
graph2->SetLineWidth(2);
graph2->SetMarkerColor(2);
graph2->SetMarkerStyle(21);
graph2->SetMarkerSize(1.5);

qstar->SetLineColor(4);
qstar->SetLineWidth(2);
qstar->SetMarkerColor(4);
qstar->SetMarkerStyle(23);
 qstar->SetMarkerSize(1.5);

rsg->SetLineColor(kGreen);
rsg->SetLineWidth(2);
rsg->SetMarkerColor(kGreen);
rsg->SetMarkerStyle(25);
 rsg->SetMarkerSize(1.5);

data->SetLineColor(kBlack);
data->SetLineWidth(2);
data->SetMarkerColor(kBlack);
data->SetMarkerStyle(27);
 data->SetMarkerSize(1.5);

CanvasRatio->cd();
LegRatio->AddEntry(graph2,"Best Signal Fit","l");
LegRatio->AddEntry(qstar,"Q*","l");
LegRatio->AddEntry(rsg,"gg->RSG->gg","l");
LegRatio->AddEntry(data,"Data all mass cut","l");

 TH1D* PLOTTER = new TH1D("PLOTTER", "#Delta #eta distribution", 1, 0, 1.3);

 PLOTTER->SetStats(0);
 PLOTTER->SetMinimum(-0.02);
 PLOTTER->SetMaximum(1.0);

 PLOTTER->Draw();


graph2->Draw("SAMEP");
qstar->Draw("SAMEP");
rsg->Draw("SAMEP");
data->Draw("SAMEP");
LegRatio->Draw();

 string out = Rep + "RatioAt" + Form("%.0f", mass) + ".png";

  CanvasRatio->SaveAs(out.c_str());
//cout<<"error p1+: "<<r2errorhigh[0]<<endl;
//cout<<"error p1-: "<<r2errorlow[0]<<endl;
//cout<<"method2 hist1 par: "<<meth2p1<<endl;
//cout<<"method2 hist1 par: "<<meth2p2<<endl;
//cout<<"method2 hist3 par: "<<meth2p3<<endl;
//cout<<"method2 hist1 error: "<<meth2errorp1<<endl;
//cout<<"method2 hist1 error: "<<meth2errorp2<<endl;
//cout<<"method2 hist1 error: "<<meth2errorp3<<endl;
}
