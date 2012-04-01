#include <string>
#include <vector>
#include <iostream>
#include <iterator>

#include <stdlib.h>
#include "TFile.h"
#include "TObject.h"
#include "TTree.h"
#include <cstring>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "THStack.h"
#include "TAttLine.h"
#include "TCutG.h"
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TF1.h>
#include <TChain.h>
#include <TMath.h>
#include <TLegend.h>
#include <RooFit.h>
#include <RooNovosibirsk.h>
#include <RooRealVar.h>
#include <RooNovosibirsk.h>
#include <RooPlot.h>
#include <RooDataSet.h>
#include "../bin/UserCode/lpernie/src/CommonTools/DrawLPBase.h"
#include <TROOT.h>
#include <TStyle.h>


#include "../src/tdrstyle.C"
#include "../src/JSONWrapper.cc"

#define NdataSet 2
#define PointToPlot 20

using namespace std;

double iLumi = 5035;
string inDir   = "../test/results/";
string outDir   = "PlotMet/";
int ChooseMet = 1;
int OptMet = 1;
int FitDY = 1;
//string inPut   = "all_ZZ_RedMetInd_Opt";

double max_array(double *A, int n);
double max(double x, double y);
Double_t singleGaussian(Double_t *x, Double_t *par);
void singleGaus();
Double_t doubleGaussian(Double_t *x, Double_t *par);
void doubleGaus();
Double_t gaussianAndQuartic(Double_t *x, Double_t *par);
void gaussAndQuart();
Double_t gaussianExp(Double_t *x, Double_t *par);
void gausExp();
void novosibirskFunct();
Double_t novosibirsk(Double_t *x, Double_t *par);


int main(int argc, char* argv[]){

   TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
   myc1->SetFillColor(0);
   setTDRStyle();  
   gStyle->SetPadTopMargin   (0.06);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.16);
   gStyle->SetPadLeftMargin  (0.14);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);

   string output;

   for(int i=1;i<argc;i++){
     string arg(argv[i]);

     if(arg.find("--help")!=string::npos){
        printf("--help   --> print this helping text\n");

        printf("--iLumi   --> integrated luminosity to be used for the MC rescale\n");
        printf("--inDir   --> path to the directory containing the .root with Histo\n");
        printf("--outDir  --> path of the directory that will contains the output plots and tables\n");
        printf("--ChooseMet   --> 1 if you want to compare different Met\n");
        printf("--OptMet   --> 1 if you want to maximize S/sqrt(S+B)\n");
        printf("--FitDY   --> 1 if you want Fit MC_DY distribution\n");
        //printf("--json    --> containing list of process (and associated style) to process to process\n");
	return 0;
     }

     if(arg.find("--iLumi"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%lf",&iLumi); i++; printf("Lumi = %f\n", iLumi); }
     if(arg.find("--inDir"  )!=string::npos && i+1<argc){ inDir    = argv[i+1];  i++;  printf("inDir = %s\n", inDir.c_str());  }
     if(arg.find("--outDir" )!=string::npos && i+1<argc){ outDir   = argv[i+1];  i++;  printf("outDir = %s\n", outDir.c_str());  }
     if(arg.find("--ChooseMet"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&ChooseMet);  i++;  printf("ChooseMet = %s\n", (ChooseMet == 1)?"true":"false");  }
     if(arg.find("--OptMet"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&OptMet);  i++;  printf("OptMet = %s\n", (OptMet == 1)?"true":"false");  }   
     if(arg.find("--FitDY"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&FitDY);  i++;  printf("FitDY = %s\n", (FitDY == 1)?"true":"false");  }   
    //if(arg.find("--json"   )!=string::npos && i+1<argc){ jsonFile = argv[i+1];  i++;  }
   } 
     system( (string("mkdir -p ") + outDir).c_str());
     system("hadd -f ../test/results/MC_ZZ.root ../test/results/MC_ZZ_*.root");
     system("hadd -f ../test/results/MC_DYJetsToLL.root ../test/results/MC_DYJetsToLL_*.root");
     system("hadd -f ../test/results/MC_WZ.root ../test/results/MC_WZ_*.root");

   //JSONWrapper::Object Root(jsonFile, true);
   //GetInitialNumberOfEvents(Root,inDir,cutflowhisto);
     string DY_root = "MC_DYJetsToLL.root";
     string ZZ_root = "MC_ZZ.root";
     string WW_root = "MC_WW.root";
     string WZ_root = "MC_WZ.root";
     string WJ_root = "MC_WJetsToLNu.root";
     string TT_root = "MC_TTJets.root ";
     string Nevent = "all_cutflow";
     string RedMet_IND_Best = "all_ZZ_RedMetInd_Best";
     string RedMet_D0_Best = "all_ZZ_RedMetD0_Best";
     string PFMet_Best = "all_ZZ_PFMet_Best";
     string RedMet_IND_Hist = "all_ZZ_RedMetInd_Opt";
     string RedMet_D0_Hist = "all_ZZ_RedMetD0_Opt";
     string Mass_Hist = "all_ZZ_Mass_Opt";
     string Dphi_Hist = "all_ZZ_Dphi_Opt";

	//Open FilE
     string Path_DY = inDir + DY_root;     TFile* File_DY  =  TFile::Open( Path_DY.c_str() );
     string Path_ZZ = inDir + ZZ_root;     TFile* File_ZZ  =  TFile::Open( Path_ZZ.c_str() );
     string Path_WW = inDir + WW_root;     TFile* File_WW  =  TFile::Open( Path_WW.c_str() );
     string Path_WZ = inDir + WZ_root;     TFile* File_WZ  =  TFile::Open( Path_WZ.c_str() );
     string Path_WJ = inDir + WJ_root;     TFile* File_WJ  =  TFile::Open( Path_WJ.c_str() );
     string Path_TT = inDir + TT_root;     TFile* File_TT  =  TFile::Open( Path_TT.c_str() );
	//Get N_EV
     TH1F *h_nevDY =  ( (TH1F*)File_DY->Get( Nevent.c_str() ));
     double NevDY = h_nevDY->GetBinContent(1);
     TH1F *h_nevZZ =  ( (TH1F*)File_ZZ->Get( Nevent.c_str() ));
     double NevZZ = h_nevZZ->GetBinContent(1);
     TH1F *h_nevWW =  ( (TH1F*)File_WW->Get( Nevent.c_str() ));
     double NevWW = h_nevWW->GetBinContent(1);
     TH1F *h_nevWZ =  ( (TH1F*)File_WZ->Get( Nevent.c_str() ));
     double NevWZ = h_nevWZ->GetBinContent(1);
     TH1F *h_nevWJ =  ( (TH1F*)File_WJ->Get( Nevent.c_str() ));
     double NevWJ = h_nevWJ->GetBinContent(1);
     TH1F *h_nevTT =  ( (TH1F*)File_TT->Get( Nevent.c_str() ));
     double NevTT = h_nevTT->GetBinContent(1);
	//Get Histo
// Best Met
     TH1F *hBest_DY_IND = ( (TH1F*)File_DY->Get( RedMet_IND_Best.c_str() ));
     TH1F *hBest_DY_D0 = ( (TH1F*)File_DY->Get( RedMet_D0_Best.c_str() ));
     TH1F *hBest_DY_PF = ( (TH1F*)File_DY->Get( PFMet_Best.c_str() ));
     TH1F *hBest_ZZ_IND = ( (TH1F*)File_ZZ->Get( RedMet_IND_Best.c_str() ));
     TH1F *hBest_ZZ_D0 = ( (TH1F*)File_ZZ->Get( RedMet_D0_Best.c_str() ));
     TH1F *hBest_ZZ_PF = ( (TH1F*)File_ZZ->Get( PFMet_Best.c_str() ));
// Optim Cut
     TH1F *h_DY_IND = ( (TH1F*)File_DY->Get( RedMet_IND_Hist.c_str() ));
     TH1F *h_ZZ_IND = ( (TH1F*)File_ZZ->Get( RedMet_IND_Hist.c_str() ));
     TH1F *h_WW_IND = ( (TH1F*)File_WW->Get( RedMet_IND_Hist.c_str() ));
     TH1F *h_WZ_IND = ( (TH1F*)File_WZ->Get( RedMet_IND_Hist.c_str() ));
     TH1F *h_WJ_IND = ( (TH1F*)File_WJ->Get( RedMet_IND_Hist.c_str() ));
     TH1F *h_TT_IND = ( (TH1F*)File_TT->Get( RedMet_IND_Hist.c_str() ));

     TH1F *h_DY_D0 = ( (TH1F*)File_DY->Get( RedMet_D0_Hist.c_str() ));
     TH1F *h_ZZ_D0 = ( (TH1F*)File_ZZ->Get( RedMet_D0_Hist.c_str() ));
     TH1F *h_WW_D0 = ( (TH1F*)File_WW->Get( RedMet_D0_Hist.c_str() ));
     TH1F *h_WZ_D0 = ( (TH1F*)File_WZ->Get( RedMet_D0_Hist.c_str() ));
     TH1F *h_WJ_D0 = ( (TH1F*)File_WJ->Get( RedMet_D0_Hist.c_str() ));
     TH1F *h_TT_D0 = ( (TH1F*)File_TT->Get( RedMet_D0_Hist.c_str() ));

     TH1F *h_DY_Mass = ( (TH1F*)File_DY->Get( Mass_Hist.c_str() ));
     TH1F *h_ZZ_Mass = ( (TH1F*)File_ZZ->Get( Mass_Hist.c_str() ));
     TH1F *h_WW_Mass = ( (TH1F*)File_WW->Get( Mass_Hist.c_str() ));
     TH1F *h_WZ_Mass = ( (TH1F*)File_WZ->Get( Mass_Hist.c_str() ));
     TH1F *h_WJ_Mass = ( (TH1F*)File_WJ->Get( Mass_Hist.c_str() ));
     TH1F *h_TT_Mass = ( (TH1F*)File_TT->Get( Mass_Hist.c_str() ));

     TH1F *h_DY_Dphi = ( (TH1F*)File_DY->Get( Dphi_Hist.c_str() ));
     TH1F *h_ZZ_Dphi = ( (TH1F*)File_ZZ->Get( Dphi_Hist.c_str() ));
     TH1F *h_WW_Dphi = ( (TH1F*)File_WW->Get( Dphi_Hist.c_str() ));
     TH1F *h_WZ_Dphi = ( (TH1F*)File_WZ->Get( Dphi_Hist.c_str() ));
     TH1F *h_WJ_Dphi = ( (TH1F*)File_WJ->Get( Dphi_Hist.c_str() ));
     TH1F *h_TT_Dphi = ( (TH1F*)File_TT->Get( Dphi_Hist.c_str() ));

     double DYWeight = (iLumi*3048)/NevDY;
     double ZZWeight = (iLumi*5.9)/NevZZ;
     double WWWeight = (iLumi*43.0*0.10497)/NevWW;
     double WZWeight = (iLumi*18.2*0.0327)/NevWZ;
     double WJWeight = (iLumi*31314.0)/NevWJ;
     double TTWeight = (iLumi*165.0)/NevTT;

if( ChooseMet == 1 ){
     cout<<"Comparing different Met"<<endl;

     double Eff_DY_IND[30] = {0.}, Eff_ZZ_IND[30] = {0.};
     double Eff_DY_D0[30] = {0.}, Eff_ZZ_D0[30] = {0.};
     double Eff_DY_PF[30] = {0.}, Eff_ZZ_PF[30] = {0.};

     double NBefore_DY_IND = hBest_DY_IND->GetBinContent(1)*DYWeight;
     double NBefore_DY_D0 = hBest_DY_D0->GetBinContent(1)*DYWeight;
     double NBefore_DY_PF = hBest_DY_PF->GetBinContent(1)*DYWeight;
     double NBefore_ZZ_IND = hBest_ZZ_IND->GetBinContent(1)*ZZWeight;
     double NBefore_ZZ_D0 = hBest_ZZ_D0->GetBinContent(1)*ZZWeight;
     double NBefore_ZZ_PF = hBest_ZZ_PF->GetBinContent(1)*ZZWeight;
     for(int i = 1; i < hBest_ZZ_IND->GetNbinsX()-1 ; i++){
       Eff_DY_IND[i] = hBest_DY_IND->GetBinContent(i+1)/NBefore_DY_IND;
       Eff_ZZ_IND[i] = hBest_ZZ_IND->GetBinContent(i+1)/NBefore_ZZ_IND;
       Eff_DY_D0[i] = hBest_DY_D0->GetBinContent(i+1)/NBefore_DY_D0;
       Eff_ZZ_D0[i] = hBest_ZZ_D0->GetBinContent(i+1)/NBefore_ZZ_D0;
       Eff_DY_PF[i] = hBest_DY_PF->GetBinContent(i+1)/NBefore_DY_PF;
       Eff_ZZ_PF[i] = hBest_ZZ_PF->GetBinContent(i+1)/NBefore_ZZ_PF;
     }
        TGraph *PFMet = new TGraph(30, Eff_DY_PF, Eff_ZZ_PF);
        PFMet->SetLineColor(2);
        PFMet->SetLineWidth(1);
        PFMet->SetMarkerColor(2);
        PFMet->SetMarkerStyle(20);
        PFMet->SetMarkerSize(0.5);
        PFMet->SetTitle("MET Efficiency");
        PFMet->GetXaxis()->SetTitle("DY Efficiency");
        PFMet->GetYaxis()->SetTitle("ZZ Efficiency");

        TGraph *Met_IND = new TGraph(30, Eff_DY_IND, Eff_ZZ_IND);
        Met_IND->SetLineColor(kBlue);
        Met_IND->SetLineWidth(1);
        Met_IND->SetMarkerColor(kBlue);
        Met_IND->SetMarkerStyle(20);
        Met_IND->SetMarkerSize(0.5);

        TGraph *Met_D0 = new TGraph(30, Eff_DY_D0, Eff_ZZ_D0);
        Met_D0->SetLineColor(3);
        Met_D0->SetLineWidth(1);
        Met_D0->SetMarkerColor(3);
        Met_D0->SetMarkerStyle(20);
        Met_D0->SetMarkerSize(0.5);

     //Legend
     TLegend *leg = new TLegend(0.50, 0.57, 0.80, 0.87);
     leg->SetFillColor(kWhite);
     leg->AddEntry(PFMet,"PFMET","P");
     leg->AddEntry(Met_IND,"REDMet IND","P");
     leg->AddEntry(Met_D0,"REDMet D0","P");

     myc1->cd();
     myc1->SetLogx();
     PFMet->Draw("ACP");
     Met_IND->Draw("CPsame");
     Met_D0->Draw("CPsame");
     leg->Draw("same");
     output = outDir + "Met_Eff.png";
     myc1->SaveAs(output.c_str());
     
     cout<<"Done with Met comparison..."<<endl<<endl;
     }

if( OptMet == 1 ){
     cout<<"Optimizing IND Met Cut"<<endl;
     double Bkg = 0., DYnum = 0., ZZnum = 0., WWnum = 0., WZnum = 0., WJnum = 0., TTnum = 0.;
     double SB[30]={0.};

     for(int i = 0; i<h_ZZ_IND->GetNbinsX(); i++){
       DYnum = (double)(h_DY_IND->GetBinContent(i+1))*DYWeight;
       ZZnum = (double)(h_ZZ_IND->GetBinContent(i+1))*ZZWeight;
       WWnum = (double)(h_WW_IND->GetBinContent(i+1))*WWWeight;
       WZnum = (double)(h_WZ_IND->GetBinContent(i+1))*WZWeight;
       WJnum = (double)(h_WJ_IND->GetBinContent(i+1))*WJWeight;
       TTnum = (double)(h_TT_IND->GetBinContent(i+1))*TTWeight;
       Bkg = DYnum+WWnum+WZnum+WJnum+TTnum;
       if( (ZZnum+Bkg) != 0 ){
         cout<<"cut = "<<20+2*i<<"  Num of BKG: "<< Bkg<<"  Num of ZZ: "<< ZZnum <<"  S/sqrt(S+B): "<< ZZnum/(sqrt(ZZnum+Bkg)) <<endl;
         SB[i] = ZZnum/(sqrt(ZZnum+Bkg));
       }
     }
     cout<<"MET IND:----> S/sqrt(S+B) Max is: "<<max_array(SB,30)<<endl<<endl;

     cout<<"Optimizing D0 Met Cut"<<endl;
      Bkg = 0., DYnum = 0., ZZnum = 0., WWnum = 0., WZnum = 0., WJnum = 0., TTnum = 0.;
     for(int i=0; i<30; i++) SB[i]=0.;

     for(int i = 0; i<h_ZZ_D0->GetNbinsX(); i++){
       DYnum = (double)(h_DY_D0->GetBinContent(i+1))*DYWeight;
       ZZnum = (double)(h_ZZ_D0->GetBinContent(i+1))*ZZWeight;
       WWnum = (double)(h_WW_D0->GetBinContent(i+1))*WWWeight;
       WZnum = (double)(h_WZ_D0->GetBinContent(i+1))*WZWeight;
       WJnum = (double)(h_WJ_D0->GetBinContent(i+1))*WJWeight;
       TTnum = (double)(h_TT_D0->GetBinContent(i+1))*TTWeight;
       Bkg = DYnum+WWnum+WZnum+WJnum+TTnum;
       if( (ZZnum+Bkg) != 0 ){
         cout<<"cut = "<<20+2*i<<"  Num of BKG: "<< Bkg<<"  Num of ZZ: "<< ZZnum <<"  S/sqrt(S+B): "<< ZZnum/(sqrt(ZZnum+Bkg)) <<endl;
         SB[i] = ZZnum/(sqrt(ZZnum+Bkg));
       }
     }
     cout<<"MET D0:----> S/sqrt(S+B) Max is: "<<max_array(SB,30)<<endl<<endl;

     cout<<"Optimizing Mass Cut"<<endl;
     for(int i=0; i<30; i++) SB[i]=0.;
     for(int i = 0; i<h_ZZ_Mass->GetNbinsX(); i++){
       DYnum = (double)(h_DY_Mass->GetBinContent(i+1))*DYWeight;
       ZZnum = (double)(h_ZZ_Mass->GetBinContent(i+1))*ZZWeight;
       WWnum = (double)(h_WW_Mass->GetBinContent(i+1))*WWWeight;
       WZnum = (double)(h_WZ_Mass->GetBinContent(i+1))*WZWeight;
       WJnum = (double)(h_WJ_Mass->GetBinContent(i+1))*WJWeight;
       TTnum = (double)(h_TT_Mass->GetBinContent(i+1))*TTWeight;
       Bkg = DYnum+WWnum+WZnum+WJnum+TTnum;
       if( (ZZnum+Bkg) != 0 ){
         cout<<"cut = "<<5+0.5*i<<"  Num of BKG: "<< Bkg<<"  Num of ZZ: "<< ZZnum <<"  S/sqrt(S+B): "<< ZZnum/(sqrt(ZZnum+Bkg)) <<endl;
         SB[i] = ZZnum/(sqrt(ZZnum+Bkg));
       }
     }
     cout<<"Mass:----> S/sqrt(S+B) Max is: "<<max_array(SB,30)<<endl<<endl;

     cout<<"Optimizing Phi Cut"<<endl;
     for(int i=0; i<30; i++) SB[i]=0.;
     for(int i = 0; i<h_ZZ_Dphi->GetNbinsX(); i++){
       DYnum = (double)(h_DY_Dphi->GetBinContent(i+1))*DYWeight;
       ZZnum = (double)(h_ZZ_Dphi->GetBinContent(i+1))*ZZWeight;
       WWnum = (double)(h_WW_Dphi->GetBinContent(i+1))*WWWeight;
       WZnum = (double)(h_WZ_Dphi->GetBinContent(i+1))*WZWeight;
       WJnum = (double)(h_WJ_Dphi->GetBinContent(i+1))*WJWeight;
       TTnum = (double)(h_TT_Dphi->GetBinContent(i+1))*TTWeight;
       Bkg = DYnum+WWnum+WZnum+WJnum+TTnum;
       if( (ZZnum+Bkg) != 0 ){
         cout<<"cut = "<<1+i*0.071333 <<"  Num of DY: "<< DYnum<<"  Num of ZZ: "<< ZZnum <<"  S/sqrt(S+B): "<< ZZnum/(sqrt(ZZnum+Bkg)) <<endl;
         SB[i] = ZZnum/(sqrt(ZZnum+Bkg));
       }
     }
     cout<<"PHI:----> S/sqrt(S+B) Max is: "<<max_array(SB,30)<<endl<<endl;
}


    //DY FIT

if( FitDY == 1 ){
     
  string DY_RedMet = "all_Fit_RedMet_IND";
  TH1F *h_DY_RedMetIND = ( (TH1F*)File_DY->Get( DY_RedMet.c_str() ));
  float StartFit = 12., EndFit = 33.;

  //Change 50.01 if you have a different cut
  int firstBin = h_DY_RedMetIND->FindBin(50.01);
  double firstBinCenter = h_DY_RedMetIND->GetBinCenter(firstBin);
  double binWidth = h_DY_RedMetIND->GetBinWidth(firstBin);

  singleGaus();
  h_DY_RedMetIND->Fit("singleGaus", "", "", StartFit, EndFit);
  TF1 *fSingleGaus = (TF1*) gROOT->GetFunction("singleGaus");
  fSingleGaus->SetLineWidth(2);  fSingleGaus->SetLineColor(kBlue);
  double nSingleGaus = 0.;
  for(double cntrBin = firstBinCenter; cntrBin < 100.; cntrBin += binWidth) {
    nSingleGaus += fSingleGaus->Eval(cntrBin);
  }
  std::cout << std::endl;  std::cout << " Single Gaus: " << nSingleGaus << " +- " << sqrt(nSingleGaus);  std::cout << std::endl;

  gausExp();
  h_DY_RedMetIND->Fit("gausExp", "", "", StartFit, EndFit);
  TF1 *fGausExp = (TF1*)gROOT->GetFunction("gausExp");
  fGausExp->SetLineWidth(2);  fGausExp->SetLineColor(kRed);
  double nGausExp = 0.;
  for(double cntrBin=firstBinCenter; cntrBin<100.; cntrBin+=binWidth) {
    nGausExp += fGausExp->Eval(cntrBin);
  }
  std::cout << std::endl;  std::cout << " Gaus + Exp: " << nGausExp << " +- " << sqrt(nGausExp);  std::cout << std::endl;

  novosibirskFunct();
  h_DY_RedMetIND->Fit("novosibirskFunct", "", "", StartFit, EndFit);
  TF1 *fNovosibirsk = (TF1*)gROOT->GetFunction("novosibirskFunct");
  fNovosibirsk->SetLineWidth(2);  fNovosibirsk->SetLineColor(kGreen);
  double nNovosibirsk = 0.;
  for(double cntrBin=firstBinCenter; cntrBin<100.; cntrBin+=binWidth) {
    nNovosibirsk += fNovosibirsk->Eval(cntrBin);
  }
  std::cout << std::endl;  std::cout << " Novosibirsk: " << nNovosibirsk << " +- " << sqrt(nNovosibirsk);  std::cout << std::endl;

  gaussAndQuart();
  h_DY_RedMetIND->Fit("gaussAndQuart", "", "", StartFit, EndFit);
  TF1 *fGausAndQuart = (TF1*)gROOT->GetFunction("gaussAndQuart");
  fGausAndQuart->SetLineWidth(2);  fGausAndQuart->SetLineColor(kViolet);
  double nGausAndQuart = 0.;
  for(double cntrBin=firstBinCenter; cntrBin<100.; cntrBin+=binWidth) {
  }
  std::cout << std::endl;  std::cout << " Gaus*Quart: " << nGausAndQuart << " +- " << sqrt(nGausAndQuart);  std::cout << std::endl;

     float cut = 50.;
     std::cout << std::endl;
     std::cout << "---> Integral: Novosibirsk " << fNovosibirsk->Integral(cut,999.) <<endl;
     std::cout << "---> Integral: fGausExp " << fGausExp->Integral(cut,999.) <<endl;
     std::cout << "---> Integral: fSingleGaus " << fSingleGaus->Integral(cut,999.) <<endl;
     std::cout << std::endl;

     myc1->cd();
     myc1->SetLogx(0);
     gStyle->SetOptStat(0);
     h_DY_RedMetIND->Draw();
     fGausAndQuart->Draw("same");
     gStyle->SetOptStat(0);
     fNovosibirsk->Draw("same");
     gStyle->SetOptStat(0);
     fSingleGaus->Draw("same");
     fGausExp->Draw("same");
     gStyle->SetOptStat(0);
     myc1->SetLogy();
     output = outDir + "ProvaFit.png";
     myc1->SaveAs(output.c_str());

/*     RooRealVar x("x","x", 0, 60,"");
     RooRealVar peak("peak","peak of Novosibirsk",0,0,60);
     RooRealVar width("width","width of Novosibirsk",10);
     RooRealVar tail("tail","tail of Novosibirsk",20);
     RooNovosibirsk Novo("Novosibirsk","RooNovosibirsk PDF", x, peak, width, tail);
     RooDataSet* data = Novo.generate(x,10000);
     Novo.fitTo(*data) ;
myc1->cd();
Novo.Draw();
myc1->SaveAs("ProvaFit.png");*/
//RooPlot* xframe = x.frame();
//Novo.plotOn(xframe) ;
//xframe->Draw() ;

}

}//End Main

double max_array(double *A, int n){ 
 
if(n==1)  return A[0]; 
 
else      return max(A[0],max_array(A+1, n-1)); 
} 
 
double max (double x, double y) { 
if(x>=y)  return x; 
else      return y; 
}

// SINGLE GAUSS
Double_t singleGaussian(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t N1 = par[0];
  Double_t m1 = par[1];
  Double_t s1 = par[2];
  Double_t f = N1*TMath::Gaus(xx, m1, s1);
  return f;
}
void singleGaus() {
  TF1 *f1 = new TF1("singleGaus", singleGaussian, 0., 100., 3);
  f1->SetParameter(0, 100000);
  f1->SetParameter(1, 0.);
  f1->SetParameter(2, 10.);
  f1->SetParNames("N", "#mu", "#sigma");
  }
// DOUBLE GAUSS
Double_t doubleGaussian(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t N1 = par[0];
  Double_t m1 = par[1];
  Double_t s1 = par[2];
  Double_t N2 = par[3];
  Double_t m2 = par[4];
  Double_t s2 = par[5];
  Double_t f = N1*TMath::Gaus(xx, m1, s1) + N2*TMath::Gaus(xx, m2, s2);
  return f;
}

void doubleGaus() {
  TF1 *f1 = new TF1("doubleGaus", doubleGaussian, 0., 100., 6);
  f1->SetParameter(0, 100000);
  f1->SetParameter(1, 0.);
  f1->SetParameter(2, 10.);
  f1->SetParameter(3, 10.);
  f1->SetParLimits(3, 1., 1000000.);
  f1->SetParameter(4, -100.);
  f1->SetParLimits(4, -500., 0.);
  f1->SetParameter(5, 50.);
  f1->SetParLimits(5, 1., 500.);
  f1->SetParNames("N_{1}", "#mu_{1}", "#sigma_{1}", "N_{2}", "#mu_{2}", "#sigma_{2}");
  }

// GAUSS 4dratic
Double_t gaussianAndQuartic(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t N  = par[0];
  Double_t m = par[1];
  Double_t s = par[2];
  Double_t f = N*(xx-m)*(xx-m)*(xx-m)*TMath::Exp( ( - (xx - m)*(xx - m) )/(2.*s*s) );
  return f;
}
void gaussAndQuart() {
  TF1 *f1 = new TF1("gaussAndQuart", gaussianAndQuartic, 0., 100., 3);
  f1->SetParameter(0, 100000);
  f1->SetParameter(1, 0.);
  f1->SetParNames("N_{1}", "#mu_{1}", "#sigma_{1}");
}

// Gaussian + Exp
Double_t gaussianExp(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t N1 = par[0];
  Double_t m  = par[1];
  Double_t s  = par[2];
  Double_t N2 = par[3];
  Double_t alpha = par[4];
  Double_t f = N1*TMath::Gaus(xx, m, s) + N2*TMath::Exp(alpha*xx);
  return f;
}
void gausExp() {
  TF1 *f1 = new TF1("gausExp", gaussianExp, 0., 100., 5);
  f1->SetParameter(0, 100000);
  f1->SetParameter(1, -10.);
  f1->SetParLimits(1, -500., 0.);
  f1->SetParameter(2, 10.);
  f1->SetParLimits(2, 1., 20.);
  f1->SetParameter(3, 10000.);
  f1->SetParLimits(3, 10., 100000.);
  f1->SetParameter(4, -0.2);
  f1->SetParLimits(4, -1., 0.);
  f1->SetParNames("N_{1}", "#mu", "#sigma", "N_{2}", "#alpha");
  }

// Novosibirsk
Double_t novosibirsk(Double_t *x, Double_t *par) {
  Double_t xx    = x[0];
  Double_t N     = par[0];
  Double_t peak  = par[1];
  Double_t width = par[2];
  Double_t tail  = par[3];

  Double_t qc = 0.;
  if(TMath::Abs(tail) < 1.e-7) 
    qc = 0.5*TMath::Power(((xx-peak)/width),2);  
  else {
    Double_t qa = tail*sqrt(log(4.));
    Double_t qb = sinh(qa)/qa;
    Double_t qx = (xx-peak)/width*qb;
    Double_t qy = 1.+tail*qx;
    if(qy > 1.E-7)
      qc = 0.5*(TMath::Power((log(qy)/tail),2) + tail*tail);
    else
      qc = 15.0;
  }
  Double_t f = N*exp(-qc);
  return f;
}
void novosibirskFunct() {
  TF1 *f1 = new TF1("novosibirskFunct", novosibirsk, 1., 60., 4);
  f1->SetParameter(0, 10000.);
  f1->SetParLimits(0, 1., 1000000.);
  f1->SetParameter(1, -10.);
  f1->SetParLimits(1, -50., -1.);
  f1->SetParameter(2, 10.);
  f1->SetParameter(3, 1.);
  f1->SetParNames("N", "#mu", "#sigma", "#tau");
  }
