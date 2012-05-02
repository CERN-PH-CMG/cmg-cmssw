#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include "TMath.h"
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
#include <TGraphBentErrors.h>
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
int OptCut = 1;
int CrossSec = 1;
int SystGlob = 1;
int SistDy = 1;
int ComputeDy = 1;
int hadd = 1;

void Smear(TFile **File, TCanvas* myc1, string nvtx);
Double_t singleGaussian(Double_t *x, Double_t *par);
void singleGaus();
Double_t doubleGaussian(Double_t *x, Double_t *par);
void doubleGaus();
Double_t novosibirsk(Double_t *x, Double_t *par);
void novosibirskFunct();

double min_array(double *A, int n);
double min(double x, double y);
double max_array(double *A, int n);
double max(double x, double y);

void FindMinAndVar(double *Chi2, double *sigma1 );

string jsonFile = "../data/samples_ZZ.json";
string cutflowhisto = "all_cutflow";
std::map<string, double> PURescale_up;
std::map<string, double> PURescale_down;
std::map<string, double> initialNumberOfEvents;
std::map<string, bool>   FileExist;

TObject* GetObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false)
{
   size_t pos = Path.find("/");
   if(pos < 256){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());

      if(TMP!=NULL){
         TObject* TMP2 =  GetObjectFromPath(TMP,endPart,GetACopy);

         return TMP2;
      }
      return NULL;
   }else{
      TObject* TMP = File->Get(Path.c_str());
      if(GetACopy){
         return TMP->Clone();
      }else{
         return TMP;
      }
   }
}

struct NameAndType{
   std::string Name;
   bool isTH1;
   NameAndType(std::string Name_,  bool isTH1_){Name = Name_; isTH1 = isTH1_;}
};

void GetListOfObject(JSONWrapper::Object& Root, std::string RootDir, std::vector<NameAndType>& histlist, TDirectory* dir=NULL, std::string parentPath=""){
   TFile* file = NULL;
   if(parentPath=="" && !dir){
      std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
      std::vector<JSONWrapper::Object> Samples = (Process[0])["data"].daughters();
      int split = 1;
      if(Samples[0].isTag("split"))split = Samples[0]["split"].toInt();
      char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",0);}else{sprintf(segmentExt,".root");}
      TFile* file = new TFile((RootDir + (Samples[0])["dtag"].toString() + segmentExt).c_str());
      dir = file;
   }


   if(dir==NULL)return;
   TList* list = dir->GetListOfKeys();
   for(int i=0;i<list->GetSize();i++){
      TObject* tmp = GetObjectFromPath(dir,list->At(i)->GetName(),false);
      if(tmp->InheritsFrom("TTree")) continue;

      if(tmp->InheritsFrom("TDirectory")){
         GetListOfObject(Root,RootDir,histlist,(TDirectory*)tmp,parentPath+ list->At(i)->GetName()+"/" );
      }else if(tmp->InheritsFrom("TH1")){
         histlist.push_back(NameAndType(parentPath+list->At(i)->GetName(), !(tmp->InheritsFrom("TH2") || tmp->InheritsFrom("TH3")) ) );
      }

   }
   if(file)delete dir;
}

void GetInitialNumberOfEvents(JSONWrapper::Object& Root, std::string RootDir, std::string HistoName){
   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
       int split = 1;
       if(Samples[j].isTag("split"))split = Samples[j]["split"].toInt();

         TH1* tmphist = NULL;
         for(int s=0;s<split;s++){
           char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",s);}else{sprintf(segmentExt,".root");}

            string FileName = RootDir + (Samples[j])["dtag"].toString() + segmentExt;
            TFile* File = new TFile(FileName.c_str());
            if(!File || File->IsZombie() || !File->IsOpen() || File->TestBit(TFile::kRecovered) ){FileExist[FileName]=false; continue; }else{FileExist[FileName]=true;}
            TH1* tmptmphist = (TH1*) GetObjectFromPath(File,HistoName);
          if(tmptmphist)
           {
          if(!tmphist){gROOT->cd(); tmphist = (TH1*)tmptmphist->Clone(tmptmphist->GetName());}else{tmphist->Add(tmptmphist);}
           delete tmptmphist;  
           }
            delete File;
         }
         if(!tmphist)continue;

         bool isMC( !Process[i]["isdata"].toBool() && !Process[i]["isdatadriven"].toBool() );

         double PUCentralnnorm =  1; if(tmphist->GetBinContent(3)>0)PUCentralnnorm = tmphist->GetBinContent(2) / tmphist->GetBinContent(3);
         double PUDownnorm     =  1; if(tmphist->GetBinContent(4)>0)PUDownnorm     = tmphist->GetBinContent(3) / tmphist->GetBinContent(4);
         double PUUpnorm       =  1; if(tmphist->GetBinContent(5)>0)PUUpnorm       = tmphist->GetBinContent(3) / tmphist->GetBinContent(5);
         PURescale_down[(Samples[j])["dtag"].toString()] = PUDownnorm;
         PURescale_up  [(Samples[j])["dtag"].toString()] = PUUpnorm;
         if(isMC)printf("PU Renormalization %25s Shift Down --> %6.2f  Central = %6.2f  Up Down --> %6.2f\n",(Samples[j])["dtag"].toString().c_str(),PUDownnorm, PUCentralnnorm, PUUpnorm);

         double cnorm = 1.0;
         if(tmphist)cnorm = tmphist->GetBinContent(1);
         if(cnorm<=0 || !isMC)cnorm = 1.0;
         if(cnorm==1 && isMC)printf("is there a problem with %s ? cnorm = %f\n",(Samples[j])["dtag"].toString().c_str(), cnorm);
         if(!isMC)PUCentralnnorm = 1;
         initialNumberOfEvents[(Samples[j])["dtag"].toString()] = cnorm / PUCentralnnorm;
         if(!isMC)printf("is there a problem with %s ? cnorm = %f - PUC=%f\n",(Samples[j])["dtag"].toString().c_str(), cnorm, PUCentralnnorm );

         delete tmphist;
      }
   }
}

enum Dataset { WW=0, ZZ, WZ, TT, WJ, DY, tw, tbw, ts, tbs, tt, tbt, EE, MuMu, EMU };
int main(int argc, char* argv[]){

   TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
   myc1->SetFillColor(0);
   setTDRStyle();  
   gStyle->SetPadTopMargin   (0.06);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.16);
   gStyle->SetPadLeftMargin  (0.18);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);

   string output = "";

   for(int i=1;i<argc;i++){
     string arg(argv[i]);

     if(arg.find("--help")!=string::npos){
        printf("--help      --> print this helping text\n");
        printf("--iLumi     --> integrated luminosity to be used for the MC rescale\n");
        printf("--inDir     --> path to the directory containing the .root with Histo\n");
        printf("--outDir    --> path of the directory that will contains the output plots and tables\n");
        printf("--hadd      --> 1 if you want to merge the dataset\n");
        printf("--CrossSec  --> 1 if you want to maximize S/sqrt(S+B)\n");
        printf("--SystGlob  --> Cross sect with all uncertainties\n");
        printf("--ChooseMet --> 1 if you want to choose the Best Met\n");
        printf("--OptCut    --> 1 if you want to optimize variables\n");
        printf("--SistDy    --> containing list of process (and associated style) to process to process\n");
        printf("--ComputeDy --> Compute the Number of DY evets\n");
        printf("--json      --> json file with all proces and info\n");
	return 0;
     }

     if(arg.find("--iLumi"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%lf",&iLumi); i++; printf("Lumi = %f\n", iLumi); }
     if(arg.find("--inDir"  )!=string::npos && i+1<argc){ inDir    = argv[i+1];  i++;  printf("inDir = %s\n", inDir.c_str());  }
     if(arg.find("--outDir" )!=string::npos && i+1<argc){ outDir   = argv[i+1];  i++;  printf("outDir = %s\n", outDir.c_str());  }
     if(arg.find("--hadd"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&hadd);  i++;  printf("hadd = %s\n", (hadd == 1)?"true":"false");  }
     if(arg.find("--CrossSec"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&CrossSec);  i++;  printf("CrossSec = %s\n", (CrossSec == 1)?"true":"false");  }   
     if(arg.find("--SystGlob"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&SystGlob);  i++;  printf("SystGlob = %s\n", (SystGlob == 1)?"true":"false");  }   
     if(arg.find("--SistDy"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&SistDy);  i++;  printf("SistDy = %s\n", (SistDy == 1)?"true":"false");  }   
     if(arg.find("--ComputeDy"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&ComputeDy);  i++;  printf("ComputeDy = %s\n", (ComputeDy == 1)?"true":"false");  }   
     if(arg.find("--ChooseMet"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&ChooseMet);  i++;  printf("ChooseMet = %s\n", (ChooseMet == 1)?"true":"false");  }
     if(arg.find("--OptCut"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&OptCut);  i++;  printf("OptCut = %s\n", (OptCut == 1)?"true":"false");  }
     if(arg.find("--json"   )!=string::npos && i+1<argc){ jsonFile = argv[i+1];  i++;  }
   } 
     system( (string("mkdir -p ") + outDir).c_str());
     JSONWrapper::Object Root(jsonFile, true);
     GetInitialNumberOfEvents(Root,inDir,cutflowhisto);  //Used to get the rescale factor based on the total number of events geenrated

     if( hadd ){
     system("hadd -f ../test/results/MC_WZ.root ../test/results/MC_WZ_*.root");
     system("hadd -f ../test/results/MC_ZZ.root ../test/results/MC_ZZ_*.root");
     system("hadd -f ../test/results/MC_DYJetsToLL.root ../test/results/MC_DYJetsToLL_*.root");
     system("hadd -f ../test/results/Data_DoubleElectron.root ../test/results/Data_DoubleElectron*_*.root");
     system("hadd -f ../test/results/Data_DoubleMu.root ../test/results/Data_DoubleMu*_*.root");
     system("hadd -f ../test/results/Data_MuEG.root ../test/results/Data_MuEG2011*_*.root");
     }

	//Get Weight
   std::vector<TString> pathFile;
   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   const int NInput = 15;
   TFile **File = new TFile*[NInput];
   double iWeight[NInput]={0.};
   string pathtmp;
   int TfileCount=0,TfileCount2=0;
   for(unsigned int i=0;i<Process.size();i++){
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool() && !Process[i]["isdatadriven"].toBool() )Weight*= iLumi;
         if( Samples[j].isTag("xsec")  )  Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
         Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()];
         //if((Samples[j])["dtag"].toString()=="MC_ZZ" ) cout<<"N tot ev. "<< initialNumberOfEvents[(Samples[j])["dtag"].toString()]<<endl;
         //FinalWeight[(Samples[j])["dtag"].toString()] = Weight;
         if( !Process[i]["isdata"].toBool() && Samples.size()==1  ){
              pathtmp = inDir + (Samples[j])["dtag"].toString() + ".root";
              File[TfileCount] = TFile::Open( pathtmp.c_str() );
              iWeight[TfileCount] = Weight;
              cout<<TfileCount<<":  "<< pathtmp.c_str()<<endl;
              TfileCount++;
         }
      }
   }
   for(unsigned int i=0;i<Process.size();i++){
    std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
    for(unsigned int j=0;j<Samples.size();j++){
      if( !Process[i]["isdata"].toBool() && Samples.size() > 1){
      double Weight = 1.0;
      if(!Process[i]["isdata"].toBool() && !Process[i]["isdatadriven"].toBool() )Weight*= iLumi;
      if( Samples[j].isTag("xsec")  )  Weight*= Samples[j]["xsec"].toDouble();
      std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
      Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()]; 
         pathtmp = inDir + (Samples[j])["dtag"].toString() + ".root";
         File[TfileCount+TfileCount2] = TFile::Open( pathtmp.c_str()  );
         iWeight[TfileCount+TfileCount2] = Weight;
         cout<<TfileCount+TfileCount2<<":  "<< pathtmp.c_str()<<endl;
         TfileCount2++;
      }
    }
   }
   File[TfileCount+TfileCount2] = TFile::Open( "../test/results/Data_DoubleElectron.root" );
   iWeight[TfileCount+TfileCount2] = 1.;
   File[TfileCount+TfileCount2+1] = TFile::Open( "../test/results/Data_DoubleMu.root" );
   iWeight[TfileCount+TfileCount2+1] = 1.;
   File[TfileCount+TfileCount2+2] = TFile::Open( "../test/results/Data_MuEG.root" );
   iWeight[TfileCount+TfileCount2+2] = 1.;

   std::vector<NameAndType> histlist;
   GetListOfObject(Root,inDir,histlist);

//Num Ev Gen: ZZ->llvv
double Br=0., NevTot=0.;
Br = 20./100. * 3.36/100.; // valid for ee, mumu
NevTot = 4191045*Br;  // valid for ee, mumu

if( ChooseMet == 1 ){

  cout<<"Comparing different Met"<<endl;
  int Niter=5; // Num of variation
  const int NEff = 30;
  for(int it = 0; it<Niter; it++){
  //Get Histo
  string RedMet_IND_Best, RedMet_D0_Best, PFMet_Best;
  if( it == 0 ){
     RedMet_IND_Best = "all_ZZ_RedMetInd_Best";
     RedMet_D0_Best = "all_ZZ_RedMetD0_Best";
     PFMet_Best = "all_ZZ_PFMet_Best";
     output = outDir + "BestMet.png";
  }
  if( it == 1 ){
     RedMet_IND_Best = "all_ZZ_RedMetInd_Best_Jesp";
     RedMet_D0_Best = "all_ZZ_RedMetD0_Best_Jesp";
     PFMet_Best = "all_ZZ_PFMet_Best_Jesp";
     output = outDir + "BestMet_Jesp.png";
  }
  if( it == 2 ){
     RedMet_IND_Best = "all_ZZ_RedMetInd_Best_Jesm";
     RedMet_D0_Best = "all_ZZ_RedMetD0_Best_Jesm";
     PFMet_Best = "all_ZZ_PFMet_Best_Jesm";
     output = outDir + "BestMet_Jesm.png";
  }
  if( it == 3 ){
     RedMet_IND_Best = "all_ZZ_RedMetInd_BestPUp";
     RedMet_D0_Best = "all_ZZ_RedMetD0_BestPUp";
     PFMet_Best = "all_ZZ_PFMet_BestPUp";
     output = outDir + "BestMet_PUp.png";
  }
  if( it == 4 ){
     RedMet_IND_Best = "all_ZZ_RedMetInd_BestPUm";
     RedMet_D0_Best = "all_ZZ_RedMetD0_BestPUm";
     PFMet_Best = "all_ZZ_PFMet_BestPUm";
     output = outDir + "BestMet_PUm.png";
  }
     TH1F *hBest_DY_IND = ( (TH1F*)File[DY]->Get( RedMet_IND_Best.c_str() ));
     TH1F *hBest_DY_D0 = ( (TH1F*)File[DY]->Get( RedMet_D0_Best.c_str() ));
     TH1F *hBest_DY_PF = ( (TH1F*)File[DY]->Get( PFMet_Best.c_str() ));
     TH1F *hBest_ZZ_IND = ( (TH1F*)File[ZZ]->Get( RedMet_IND_Best.c_str() ));
     TH1F *hBest_ZZ_D0 = ( (TH1F*)File[ZZ]->Get( RedMet_D0_Best.c_str() ));
     TH1F *hBest_ZZ_PF = ( (TH1F*)File[ZZ]->Get( PFMet_Best.c_str() ));

     double Eff_DY_IND[NEff] = {0.}, Eff_ZZ_IND[NEff] = {0.};
     double Eff_DY_D0[NEff] = {0.}, Eff_ZZ_D0[NEff] = {0.};
     double Eff_DY_PF[NEff] = {0.}, Eff_ZZ_PF[NEff] = {0.};
     double NBefore_DY = hBest_DY_IND->GetBinContent(1);
     double NBefore_ZZ = hBest_ZZ_D0->GetBinContent(1);

     for(int i = 0; i < hBest_ZZ_IND->GetNbinsX()-1 ; i++){
       Eff_DY_IND[i] = hBest_DY_IND->GetBinContent(i+2)/NBefore_DY;
       Eff_ZZ_IND[i] = hBest_ZZ_IND->GetBinContent(i+2)/NBefore_ZZ;
       Eff_DY_D0[i] = hBest_DY_D0->GetBinContent(i+2)/NBefore_DY;
       Eff_ZZ_D0[i] = hBest_ZZ_D0->GetBinContent(i+2)/NBefore_ZZ;
       Eff_DY_PF[i] = hBest_DY_PF->GetBinContent(i+2)/NBefore_DY;
       Eff_ZZ_PF[i] = hBest_ZZ_PF->GetBinContent(i+2)/NBefore_ZZ;
     }

        TGraph *PFMet = new TGraph(NEff, Eff_DY_PF, Eff_ZZ_PF);
        PFMet->SetLineColor(2);
        PFMet->SetLineWidth(1);
        PFMet->SetMarkerColor(2);
        PFMet->SetMarkerStyle(20);
        PFMet->SetMarkerSize(0.5);
        PFMet->SetTitle("MET Efficiency");
        PFMet->GetXaxis()->SetTitle("DY Efficiency");
        PFMet->GetYaxis()->SetTitle("ZZ Efficiency");

        TGraph *Met_IND = new TGraph(NEff, Eff_DY_IND, Eff_ZZ_IND);
        Met_IND->SetLineColor(kBlue);
        Met_IND->SetLineWidth(1);
        Met_IND->SetMarkerColor(kBlue);
        Met_IND->SetMarkerStyle(20);
        Met_IND->SetMarkerSize(0.5);

        TGraph *Met_D0 = new TGraph(NEff, Eff_DY_D0, Eff_ZZ_D0);
        Met_D0->SetLineColor(3);
        Met_D0->SetLineWidth(1);
        Met_D0->SetMarkerColor(3);
        Met_D0->SetMarkerStyle(20);
        Met_D0->SetMarkerSize(0.5);

     //Legend
     TLegend *leg = new TLegend(0.15,0.7,0.48,0.87);
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
     myc1->SaveAs(output.c_str());
    
 }//N iter

cout<<"Done with Met comparison..."<<endl<<endl;
}

if( OptCut ){

string MetOpt = "all_optim_eventflow1";
TH1F *h_ZZ = ( (TH1F*)File[ZZ]->Get( MetOpt.c_str() ));
TH1F *h_DY = ( (TH1F*)File[DY]->Get( MetOpt.c_str() ));
TH1F *h_WW = ( (TH1F*)File[WW]->Get( MetOpt.c_str() ));
TH1F *h_WZ = ( (TH1F*)File[WZ]->Get( MetOpt.c_str() ));
TH1F *h_TT = ( (TH1F*)File[TT]->Get( MetOpt.c_str() ));
TH1F *h_WJ = ( (TH1F*)File[WJ]->Get( MetOpt.c_str() ));

     cout<<"Optimizing Cut... "<<endl;
     const int Nbin = 475;//bin

     double Bkg = 0., DYnum = 0., ZZnum = 0., WWnum = 0., WZnum = 0., WJnum = 0., TTnum = 0.;
     double SB[Nbin]={0.}; //30.
     double Val[Nbin]={0.};

     for(int i = 0; i<h_ZZ->GetNbinsX(); i++){
       DYnum = (double)(h_DY->GetBinContent(i))*iWeight[DY];
       ZZnum = (double)(h_ZZ->GetBinContent(i))*iWeight[ZZ];
       WWnum = (double)(h_WW->GetBinContent(i))*iWeight[WW];
       WZnum = (double)(h_WZ->GetBinContent(i))*iWeight[WZ];
       WJnum = (double)(h_WJ->GetBinContent(i))*iWeight[WJ];
       TTnum = (double)(h_TT->GetBinContent(i))*iWeight[TT];
       Bkg = DYnum+WWnum+WZnum+WJnum+TTnum;
       if( (ZZnum+Bkg) != 0 ){
         //cout<<"bin = "<<i<<"  Num of BKG: "<< Bkg<<"  Num of ZZ: "<< ZZnum <<"  S/sqrt(S+B): "<< ZZnum/(sqrt(ZZnum+Bkg)) <<endl;
         SB[i] = ZZnum/(sqrt(ZZnum+Bkg));
         Val[i] = i;
       }
       else{ SB[i] = 0.; Val[i] = i;}
     }
     TGraph *h_BestMet = new TGraph(Nbin, Val, SB);
     myc1->cd();
     myc1->SetLogx(0);
     h_BestMet->GetXaxis()->SetTitle("Bin corresponding a set of cuts");
     h_BestMet->GetYaxis()->SetTitle("S/sqrtr(B)");
     h_BestMet->Draw("ACP");
     output = outDir+ "BestMetValue.png";
     myc1->SaveAs(output.c_str());
     //cout<<"MET IND:----> S/sqrt(S+B) Max is: "<<max_array(SB,Nbin)<<endl<<endl;

     int bestbin=-1;
     for(int i=0; i<Nbin;i++){ if( SB[i]==max_array(SB,Nbin) ) bestbin=i;   }
     TH1F *dmz = ( (TH1F*)File[ZZ]->Get( "all_optim_cut1_dmz" ));
     TH1F *met = ( (TH1F*)File[ZZ]->Get( "all_optim_cut1_met" ));
     cout<<"Bin: "<<bestbin<<endl;
     cout<<"Taglio in MET: "<<met->GetBinContent(bestbin)/2.<<endl;
     cout<<"Taglio in Dmz: "<<dmz->GetBinContent(bestbin)/2.<<endl;
}

if( SistDy == 1 ){

Smear(File, myc1, "1");
Smear(File, myc1, "2");
Smear(File, myc1, "3");
Smear(File, myc1, "4");

  cout<<endl;
  float Eff_in_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic" ))->GetBinContent(1)/NevTot;
  float Eff_in_mu = ((TH1F*)File[ZZ]->Get( "mumu_Effic" ))->GetBinContent(1)/NevTot;
  float Eff_fin_ee = ((TH1F*)File[ZZ]->Get("ee_Effic_MetSmear"))->GetBinContent(1)/NevTot;
  float Eff_fin_mu = ((TH1F*)File[ZZ]->Get("mumu_Effic_MetSmear"))->GetBinContent(1)/NevTot;
  float Eff_fin_ee_m = ( (TH1F*)File[ZZ]->Get("ee_Effic_MetSmearm"))->GetBinContent(1)/NevTot;
  float Eff_fin_mu_m = ( (TH1F*)File[ZZ]->Get("mumu_Effic_MetSmearm"))->GetBinContent(1)/NevTot;
  float Eff_fin_ee_p = ( (TH1F*)File[ZZ]->Get("ee_Effic_MetSmearp"))->GetBinContent(1)/NevTot;
  float Eff_fin_mu_p = ( (TH1F*)File[ZZ]->Get("mumu_Effic_MetSmearp"))->GetBinContent(1)/NevTot;
  cout<<"Efficiency (ee): "<<Eff_in_ee<<endl;
  cout<<"Efficiency (mumu): "<<Eff_in_ee<<endl;
  cout<<"New Efficiency (ee): "<<Eff_fin_ee<<endl;
  cout<<"    Corr of: "<< (Eff_fin_ee-Eff_in_ee)*100./Eff_in_ee <<"%"<<endl;
  cout<<"New Efficiency (mumu): "<< Eff_fin_mu <<endl;
  cout<<"    Corr of: "<< (Eff_fin_mu-Eff_in_mu)*100./Eff_in_mu <<"%"<<endl;
  cout<<"New Efficiency minus (ee): "<<Eff_fin_ee_m<<endl;
  cout<<"    Syst: "<< (Eff_fin_ee_m-Eff_fin_ee)*100./Eff_fin_ee <<"%"<<endl;
  cout<<"New Efficiency minus (mumu): "<<Eff_fin_mu_m<<endl;
  cout<<"    Syst: "<< (Eff_fin_mu_m-Eff_fin_mu)*100./Eff_fin_mu <<"%"<<endl;
  cout<<"New Efficiency plus (ee): "<<Eff_fin_ee_p<<endl;
  cout<<"    Syst: "<< (Eff_fin_ee_p-Eff_fin_ee)*100./Eff_fin_ee <<"%"<<endl;
  cout<<"New Efficiency plus (mumu): "<<Eff_fin_mu_p<<endl;
  cout<<"    Syst: "<< (Eff_fin_mu_p-Eff_fin_mu)*100./Eff_fin_mu <<"%"<<endl;
}

if( ComputeDy == 1 ){
//taking histos
//TString filepath = "/afs/cern.ch/user/l/lpernie/work/gamma/plotter_wgt.root";
TString filepath = "/afs/cern.ch/user/l/lpernie/scratch0/plotter.root";
TString Histo_ee = "ZZee_met_redMet_eq0jets";
TString Histo_mu = "ZZmumu_met_redMet_eq0jets";
TString Dir_data = "data (#gamma)";
TString Dir_Wga = "W#rightarrow l#nu";
TString Dir_Wnu = "W#gamma#rightarrowl#nu#gamma";
TString Dir_Zga = "Z#gamma#rightarrow#nu#nu#gamma";
TFile *file = new TFile( filepath.Data() );

TH1F *hTOT_ee = (TH1F*) file->Get( Dir_data + "/" + Histo_ee );
TH1F *hTOT_mu = (TH1F*) file->Get( Dir_data + "/" +Histo_mu );
TH1F *hWga_ee = (TH1F*) file->Get( Dir_Wga + "/" + Histo_ee );
TH1F *hWga_mu = (TH1F*) file->Get( Dir_Wga + "/" + Histo_mu );
TH1F *hWnu_ee = (TH1F*) file->Get( Dir_Wnu + "/" + Histo_ee );
TH1F *hWnu_mu = (TH1F*) file->Get( Dir_Wnu + "/" + Histo_mu );
TH1F *hZga_ee = (TH1F*) file->Get( Dir_Zga + "/" + Histo_ee );
TH1F *hZga_mu = (TH1F*) file->Get( Dir_Zga + "/" + Histo_mu );

TH1F *FinalHistoDY_ee = new TH1F("FinalHistoDY_ee", "ee FinalHistoDY", 50, 0., 500. );
TH1F *FinalHistoDY_mu = new TH1F("FinalHistoDY_mu", "mu FinalHistoDY", 50, 0., 500. );
FinalHistoDY_ee->Add( hTOT_ee );
FinalHistoDY_ee->Add( hWga_ee, -1. );
FinalHistoDY_ee->Add( hWnu_ee, -1. );
FinalHistoDY_ee->Add( hZga_ee, -1. );
FinalHistoDY_mu->Add( hTOT_mu );
FinalHistoDY_mu->Add( hWga_mu, -1. );
FinalHistoDY_mu->Add( hWnu_mu, -1. );
FinalHistoDY_mu->Add( hZga_mu, -1. );

//subtracting
for(int i=1; i<FinalHistoDY_ee->GetNbinsX(); i++){
   double BinVa_ee = FinalHistoDY_ee->GetBinContent(i);
   if(BinVa_ee<0.) BinVa_ee=0.;
   FinalHistoDY_ee->SetBinContent(i, BinVa_ee);

   double BinVa_mu = FinalHistoDY_mu->GetBinContent(i);
   if(BinVa_mu<0.) BinVa_mu=0.;
   FinalHistoDY_mu->SetBinContent(i, BinVa_mu);
}

double cut = 50.;
double maxcut = 500.;
double err_ee=0., err_mu=0. ;
double NbkgDY_ee = FinalHistoDY_ee->IntegralAndError( FinalHistoDY_ee->FindBin(cut)-1,FinalHistoDY_ee->FindBin(maxcut)-1, err_ee );
double NbkgDY_mu = FinalHistoDY_mu->IntegralAndError( FinalHistoDY_mu->FindBin(cut)-1,FinalHistoDY_mu->FindBin(maxcut)-1, err_mu );

cout<<"The Number of DY event in ee channel is: "<<NbkgDY_ee<<" +-  "<<err_ee<<endl;
cout<<"The Number of DY event in mumu channel is: "<<NbkgDY_mu<<" +_ "<<err_mu<<endl;

//Systematic *2
TH1F *mFinalHistoDY_ee = new TH1F("mFinalHistoDY_ee", "ee FinalHistoDY min", 50, 0., 500. );
TH1F *mFinalHistoDY_mu = new TH1F("mFinalHistoDY_mu", "mu FinalHistoDY min", 50, 0., 500. );
mFinalHistoDY_ee->Add( hTOT_ee );
mFinalHistoDY_ee->Add( hWga_ee, -2. );
mFinalHistoDY_ee->Add( hWnu_ee, -2. );
mFinalHistoDY_ee->Add( hZga_ee, -2. );
mFinalHistoDY_mu->Add( hTOT_mu );
mFinalHistoDY_mu->Add( hWga_mu, -2. );
mFinalHistoDY_mu->Add( hWnu_mu, -2. );
mFinalHistoDY_mu->Add( hZga_mu, -2. );
//subtracting
for(int i=1; i<mFinalHistoDY_ee->GetNbinsX(); i++){
   double BinVa_ee = mFinalHistoDY_ee->GetBinContent(i);
   if(BinVa_ee<0.) BinVa_ee=0.;
   mFinalHistoDY_ee->SetBinContent(i, BinVa_ee);

   double BinVa_mu = mFinalHistoDY_mu->GetBinContent(i);
   if(BinVa_mu<0.) BinVa_mu=0.;
   mFinalHistoDY_mu->SetBinContent(i, BinVa_mu);
}
err_ee=0., err_mu=0.;
double mNbkgDY_ee = mFinalHistoDY_ee->IntegralAndError( mFinalHistoDY_ee->FindBin(cut)-1,mFinalHistoDY_ee->FindBin(maxcut)-1, err_ee );
double mNbkgDY_mu = mFinalHistoDY_mu->IntegralAndError( mFinalHistoDY_mu->FindBin(cut)-1,mFinalHistoDY_mu->FindBin(maxcut)-1, err_mu );
cout<<"The Systematic (bkg *2) for nDY in ee is: "<< (NbkgDY_ee-mNbkgDY_ee)*100/NbkgDY_ee <<"%  you have now:"<<mNbkgDY_ee<<" events"<<endl;
cout<<"The Systematic (bkg *2) for nDY in mumu is: "<< (NbkgDY_mu-mNbkgDY_mu)*100/NbkgDY_mu <<"%  you have now"<<mNbkgDY_mu<<" events."<<endl;

//Systematic /2
TH1F *pFinalHistoDY_ee = new TH1F("pFinalHistoDY_ee", "ee FinalHistoDY plus", 50, 0., 500. );
TH1F *pFinalHistoDY_mu = new TH1F("pFinalHistoDY_mu", "mu FinalHistoDY plus", 50, 0., 500. );
pFinalHistoDY_ee->Add( hTOT_ee );
pFinalHistoDY_ee->Add( hWga_ee, -0.5 );
pFinalHistoDY_ee->Add( hWnu_ee, -0.5 );
pFinalHistoDY_ee->Add( hZga_ee, -0.5 );
pFinalHistoDY_mu->Add( hTOT_mu );
pFinalHistoDY_mu->Add( hWga_mu, -0.5 );
pFinalHistoDY_mu->Add( hWnu_mu, -0.5 );
pFinalHistoDY_mu->Add( hZga_mu, -0.5 );
//subtracting
for(int i=1; i<pFinalHistoDY_ee->GetNbinsX(); i++){
   double BinVa_ee = pFinalHistoDY_ee->GetBinContent(i);
   if(BinVa_ee<0.) BinVa_ee=0.;
   pFinalHistoDY_ee->SetBinContent(i, BinVa_ee);

   double BinVa_mu = pFinalHistoDY_mu->GetBinContent(i);
   if(BinVa_mu<0.) BinVa_mu=0.;
   pFinalHistoDY_mu->SetBinContent(i, BinVa_mu);
}
err_ee=0., err_mu=0.;
double pNbkgDY_ee = pFinalHistoDY_ee->IntegralAndError( pFinalHistoDY_ee->FindBin(cut)-1,pFinalHistoDY_ee->FindBin(maxcut)-1, err_ee );
double pNbkgDY_mu = pFinalHistoDY_mu->IntegralAndError( pFinalHistoDY_mu->FindBin(cut)-1,pFinalHistoDY_mu->FindBin(maxcut)-1, err_mu );
cout<<"The Systematic (bkg /2) for nDY in ee is: "<< (NbkgDY_ee-pNbkgDY_ee)*100/NbkgDY_ee <<"%  you have now:"<<pNbkgDY_ee<<" events"<<endl;
cout<<"The Systematic (bkg /2) for nDY in mumu is: "<< (NbkgDY_mu-pNbkgDY_mu)*100/NbkgDY_mu <<"%  you have now"<<pNbkgDY_mu<<" events."<<endl;
}

if( CrossSec == 1 ){
     double NevTot_ee = NevTot;
     double NevTot_mumu = NevTot;
     //double NevTot_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     //double NevTot_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
     cout<<endl;
     cout<<"Cross Section Measurement... Get Ready!!"<<endl;

     string NEvFin_ee   = "ee_ZZ_eventflow";
     string NEvFin_mumu = "mumu_ZZ_eventflow";
     double Nevent_ee[NInput];
     for(int i=0; i<NInput; i++){  Nevent_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(9)*iWeight[i]; }
     double Nevent_mumu[NInput];
     for(int i=0; i<NInput; i++){  Nevent_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(9)*iWeight[i]; }

     double Nbkg_ee = Nevent_ee[WW] + Nevent_ee[WZ] + Nevent_ee[DY] + Nevent_ee[TT] + Nevent_ee[WJ];
     double EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic" ))->GetBinContent(1)/NevTot_ee;
     double CrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     double Error_ee = ( sqrt(Nevent_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section: Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     double Nbkg_mumu = Nevent_mumu[WW]+Nevent_mumu[WZ]+Nevent_mumu[DY]+Nevent_mumu[TT]+Nevent_mumu[WJ];
     double EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic" ))->GetBinContent(1)/NevTot_mumu;
     double CrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     double Error_mumu = ( sqrt(Nevent_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section: Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;

     //Syst on Met (Jer)
     double EffZZJer_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficJer" ))->GetBinContent(1)/NevTot_ee;
     double JerCrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZJer_ee);
     cout<< "Syst on Met ee (Eff:"<<EffZZJer_ee<<") (Jer): "<< (JerCrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;
     double EffZZJer_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficJer" ))->GetBinContent(1)/NevTot_mumu;
     double JerCrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZJer_mumu);
     cout<< "Syst on Met mumu  (Eff:"<<EffZZJer_mumu<<")  (Jer): "<< (JerCrossSection_mumu-CrossSection_mumu)*100./CrossSection_mumu<<"%"<<endl;

     //Syst on Met (Jesm)
     double EffZZJes_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficJesm" ))->GetBinContent(1)/NevTot_ee;
     double JesCrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZJes_ee);
     cout<< "Syst on Met (ee) (Eff:"<<EffZZJes_ee<<") (Jes m): "<< (JesCrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;
     double EffZZJes_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficJesm" ))->GetBinContent(1)/NevTot_mumu;
     double JesCrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZJes_mumu);
     cout<< "Syst on Met (mumu)  (Eff:"<<EffZZJes_mumu<<")  (Jes m): "<< (JesCrossSection_mumu-CrossSection_mumu)*100./CrossSection_mumu<<"%"<<endl;
     //Syst on Met (Jesp)
     EffZZJes_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficJesp" ))->GetBinContent(1)/NevTot_ee;
     JesCrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZJes_ee);
     cout<< "Syst on Met (ee) (Eff:"<<EffZZJes_ee<<") (Jer p): "<< (JesCrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;
     EffZZJes_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficJesp" ))->GetBinContent(1)/NevTot_mumu;
     JesCrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZJes_mumu);
     cout<< "Syst on Met (mumu)  (Eff:"<<EffZZJes_mumu<<")  (Jer p): "<< (JesCrossSection_mumu-CrossSection_mumu)*100./CrossSection_mumu<<"%"<<endl;

     //Syst on PU
     //PUm
     double NormFactor = ((TH1F*)File[ZZ]->Get("Norm_pum"))->GetBinContent(1)/((TH1F*)File[ZZ]->Get("Norm_pu"))->GetBinContent(1);
     //string pu_NEvFin_ee   = "ee_eventflow_minus";
     //string pu_NEvFin_mumu = "mumu_eventflow_minus";
     //double NeventPUm_ee[NInput];
     //for(int i=0; i<NInput; i++){  NeventPUm_ee[i] = ((TH1F*)File[i]->Get( pu_NEvFin_ee.c_str() ))->GetBinContent(9)*iWeight[i]; }
     //double NeventPUm_mumu[NInput];
     //for(int i=0; i<NInput; i++){  NeventPUm_mumu[i] = ((TH1F*)File[i]->Get( pu_NEvFin_mumu.c_str() ))->GetBinContent(9)*iWeight[i]; }
 
     double pu_CrossSection_ee = 0.;
     //double pu_Nbkg_ee = NeventPUm_ee[WW] + NeventPUm_ee[WZ] + NeventPUm_ee[DY] + NeventPUm_ee[TT] + NeventPUm_ee[WJ];
     //double pu_EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficPUm" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_EfficPUm_tot" ))->GetBinContent(1);
     //double NormFactor = pu_Nbkg_ee/Nbkg_ee;
     double pu_EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficPUm" ))->GetBinContent(1)/(NormFactor*NevTot_ee);
     pu_CrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*pu_EffZZ_ee);
     cout<< "Syst on PUm ee (Cross:"<<pu_CrossSection_ee<<"): "<< (pu_CrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;

     double pu_CrossSection_mumu = 0.;
     //double pu_Nbkg_mumu = NeventPUm_mumu[WW] + NeventPUm_mumu[WZ] + NeventPUm_mumu[DY] + NeventPUm_mumu[TT] + NeventPUm_mumu[WJ];
     //double pu_EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficPUm" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_EfficPUm_tot" ))->GetBinContent(1);
     //NormFactor = pu_Nbkg_mumu/Nbkg_mumu;
     double pu_EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficPUm" ))->GetBinContent(1)/(NormFactor*NevTot_mumu);
     pu_CrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*pu_EffZZ_mumu);
     cout<< "Syst on PUm mumu ("<<pu_CrossSection_mumu<<"): "<< (pu_CrossSection_mumu-CrossSection_mumu)*100/CrossSection_mumu<<"%"<<endl;

     //PUp
     NormFactor = ((TH1F*)File[ZZ]->Get("Norm_pup"))->GetBinContent(1)/((TH1F*)File[ZZ]->Get("Norm_pu"))->GetBinContent(1);
     //pu_NEvFin_ee   = "ee_eventflow_minus";
     //pu_NEvFin_mumu = "mumu_eventflow_minus";
     //double NeventPUp_ee[NInput];
     //for(int i=0; i<NInput; i++){  NeventPUp_ee[i] = ((TH1F*)File[i]->Get( pu_NEvFin_ee.c_str() ))->GetBinContent(9)*iWeight[i]; }
     //double NeventPUp_mumu[NInput];
     //for(int i=0; i<NInput; i++){  NeventPUp_mumu[i] = ((TH1F*)File[i]->Get( pu_NEvFin_mumu.c_str() ))->GetBinContent(9)*iWeight[i]; }
     pu_CrossSection_ee = 0., pu_EffZZ_ee =0.;
     //pu_Nbkg_ee = NeventPUp_ee[WW] + NeventPUp_ee[WZ] + NeventPUp_ee[DY] + NeventPUp_ee[TT] + NeventPUp_ee[WJ];
     //pu_EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficPUp" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_EfficPUp_tot" ))->GetBinContent(1);
     //NormFactor = pu_Nbkg_ee/Nbkg_ee;
     pu_EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficPUp" ))->GetBinContent(1)/(NormFactor*NevTot_ee);
     pu_CrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*pu_EffZZ_ee);
     cout<< "Syst on PUp ee ("<<pu_CrossSection_ee<<"): "<< (pu_CrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;

     pu_CrossSection_mumu = 0.; pu_EffZZ_mumu =0.;
     //pu_Nbkg_mumu = NeventPUp_mumu[WW] + NeventPUp_mumu[WZ] + NeventPUp_mumu[DY] + NeventPUp_mumu[TT] + NeventPUp_mumu[WJ];
     //pu_EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficPUp" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_EfficPUp_tot" ))->GetBinContent(1);
     //NormFactor = pu_Nbkg_mumu/Nbkg_mumu;
     pu_EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficPUp" ))->GetBinContent(1)/(NormFactor*NevTot_mumu);
     pu_CrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*pu_EffZZ_mumu);
     cout<< "Syst on PUp mumu ("<<pu_CrossSection_mumu<<"): "<< (pu_CrossSection_mumu-CrossSection_mumu)*100/CrossSection_mumu<<"%"<<endl;

     //Pt Bin 1
     NEvFin_ee   = "ee_ZZ_eventflow_ptBin";
     NEvFin_mumu = "mumu_ZZ_eventflow_ptBin";
     double NeventPt1_ee[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=MuMu && !(i>=6 && i<=11) ) NeventPt1_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(1)*iWeight[i]; }
     double NeventPt1_mumu[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=EE && !(i>=6 && i<=11) ) NeventPt1_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(1)*iWeight[i]; }

     CrossSection_ee = 0. ;
     Nbkg_ee = NeventPt1_ee[WW] + NeventPt1_ee[WZ] + NeventPt1_ee[DY] + NeventPt1_ee[TT]; //+ WJ_Nev_ee;
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin1" ))->GetBinContent(1)/NevTot_ee;
     CrossSection_ee = (NeventPt1_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt1_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (70>Pt>90): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt1_mumu[WW] + NeventPt1_mumu[WZ] + NeventPt1_mumu[DY] + NeventPt1_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin1" ))->GetBinContent(1)/NevTot_mumu;
     CrossSection_mumu = (NeventPt1_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     Error_mumu = ( sqrt(NeventPt1_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section (70>Pt>90): Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;
     //Pt Bin2
     double NeventPt2_ee[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=MuMu && !(i>=6 && i<=11) ) NeventPt2_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(2)*iWeight[i]; }
     double NeventPt2_mumu[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=EE && !(i>=6 && i<=11) ) NeventPt2_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(2)*iWeight[i]; }

     CrossSection_ee = 0. ;
     Nbkg_ee = NeventPt2_ee[WW] + NeventPt2_ee[WZ] + NeventPt2_ee[DY] + NeventPt2_ee[TT]; //+ WJ_Nev_ee;
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin2" ))->GetBinContent(1)/NevTot_ee;
     CrossSection_ee = (NeventPt2_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt2_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (90>Pt>110): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt2_mumu[WW] + NeventPt2_mumu[WZ] + NeventPt2_mumu[DY] + NeventPt2_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin2" ))->GetBinContent(1)/NevTot_mumu;
     CrossSection_mumu = (NeventPt2_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     Error_mumu = ( sqrt(NeventPt2_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section (90>Pt>110): Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;
     //Pt Bin 3
     double NeventPt3_ee[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=MuMu && !(i>=6 && i<=11) ) NeventPt3_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(3)*iWeight[i]; }
     double NeventPt3_mumu[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=EE && !(i>=6 && i<=11) ) NeventPt3_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(3)*iWeight[i]; }

     CrossSection_ee = 0. ;
     Nbkg_ee = NeventPt3_ee[WW] + NeventPt3_ee[WZ] + NeventPt3_ee[DY] + NeventPt3_ee[TT]; //+ WJ_Nev_ee;
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin3" ))->GetBinContent(1)/NevTot_ee;
     CrossSection_ee = (NeventPt3_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt3_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (110>Pt>140): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt3_mumu[WW] + NeventPt3_mumu[WZ] + NeventPt3_mumu[DY] + NeventPt3_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin3" ))->GetBinContent(1)/NevTot_mumu;
     CrossSection_mumu = (NeventPt3_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     Error_mumu = ( sqrt(NeventPt3_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section (110>Pt>140): Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;

     //Pt Bin 4
     double NeventPt4_ee[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=MuMu && !(i>=6 && i<=11) ) NeventPt4_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(4)*iWeight[i]; }
     double NeventPt4_mumu[NInput];
     for(int i=0; i<NInput; i++){ if(i!=WJ && i!=EMU && i!=EE && !(i>=6 && i<=11) ) NeventPt4_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(4)*iWeight[i]; }

     CrossSection_ee = 0. ;
     Nbkg_ee = NeventPt4_ee[WW] + NeventPt4_ee[WZ] + NeventPt4_ee[DY] + NeventPt4_ee[TT]; //+ WJ_Nev_ee;
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin4" ))->GetBinContent(1)/NevTot_ee;
     CrossSection_ee = (NeventPt4_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt4_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (Pt>140): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt4_mumu[WW] + NeventPt4_mumu[WZ] + NeventPt4_mumu[DY] + NeventPt4_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin4" ))->GetBinContent(1)/NevTot_mumu;
     CrossSection_mumu = (NeventPt4_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     Error_mumu = ( sqrt(NeventPt4_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section (Pt>140): Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;
}

if( SystGlob==1 ){
double ERR_stat=0.,ERR_syst=0.;
//Stat
double Ndy = 0., Nnres = 0., Nwz = 0., Nww = 0., Ndata = 0.;
//Syst
double NSdy = 0., NSnres = 0., NSwz = 0., NSww = 0.;

ERR_stat = sqrt( pow(Ndy,2) );
ERR_syst = sqrt( pow(NSdy,2) );

}

}//End Main

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

double min_array(double *A, int n){

if(n==1)  return A[0];

else      return min(A[0],min_array(A+1, n-1));
}

double min (double x, double y) {
if(x<=y)  return x;
else      return y;
}


double max_array(double *A, int n){ 
 
if(n==1)  return A[0]; 
 
else      return max(A[0],max_array(A+1, n-1)); 
} 
 
double max (double x, double y) { 
if(x>=y)  return x; 
else      return y; 
}

void FindMinAndVar(double *Chi2, double *sigma1 ){

int split=0;
double ChiCentral[99] = {100.}; for(int i=0;i<99;i++) ChiCentral[i] = Chi2[i+1];
for(int i=0;i<100;i++){
 if (Chi2[i] == min_array(ChiCentral,99)) {split = i;}
}
cout<<"MIN Sig: "<<sigma1[split]<<endl;
cout<<"MIN Chi: "<<Chi2[split]<<endl;

double FirstPart[100];  for(int i=0;i<100;i++) FirstPart[i]=100.;
double SecondPart[100]; for(int i=0;i<100;i++) SecondPart[i]=100.;
double FindOne[100];    for(int i=0;i<100;i++) FindOne[i]=100.;
double FindTwo[100];    for(int i=0;i<100;i++) FindTwo[i]=100.;
double MySigOnChi = Chi2[split]+1.;

for(int i=0;i<split;i++){ if(i==0){ FirstPart[i]=100.;} else{FirstPart[i]= fabs(Chi2[i]-MySigOnChi);} }
for(int i=0;i<99-split;i++){ SecondPart[i]= fabs(Chi2[i+1+split]-MySigOnChi); }
double MinA = min_array(FirstPart,100), MinB = min_array(SecondPart,100);
double sigma1a=-1., sigma1b=-1.;
for(int i=0;i<100;i++){    if (FirstPart[i] == MinA) sigma1a= sigma1[i];    }    cout<<" RealMin A "<<sigma1a<<endl;
for(int i=0;i<100;i++){    if (SecondPart[i] == MinB) sigma1b= sigma1[i+1+split];    }    cout<<" RealMin B "<<sigma1b<<endl;

}

void Smear(TFile **File, TCanvas* myc1, string nvtx){
  string output = "";
  string emu_1nvtx = "Sys_MetSmearedOnLine_" + nvtx + "vtx_";
  string emu_px1nvtx = "Sys_MetSmearedOnLine_px" + nvtx + "vtx_";
  string emu_py1nvtx = "Sys_MetSmearedOnLine_py" + nvtx + "vtx_";

  TString emu_Sys_MetEff_RedMet_px = "emu_Sys_MetEff_RedMet_px" + nvtx + "nvtx";
  TString emu_Sys_MetEff_RedMet_py = "emu_Sys_MetEff_RedMet_py" + nvtx + "nvtx";
  TString emu_Sys_MetEff_RedMet = "emu_Sys_MetEff_RedMet_" + nvtx + "nvtx";

  TH1F *h_DataEmu_px1nvtx = ( (TH1F*)File[EMU]->Get( emu_Sys_MetEff_RedMet_px.Data() ));
  TH1F *h_DataEmu_py1nvtx = ( (TH1F*)File[EMU]->Get( emu_Sys_MetEff_RedMet_py.Data() ));
  TH1F *h_DataEmu_1nvtx = ( (TH1F*)File[EMU]->Get( emu_Sys_MetEff_RedMet.Data() ));
  TH1F **Sys_MetSmearedOnLine_px1vtx = new TH1F*[100];
  TH1F **Sys_MetSmearedOnLine_py1vtx = new TH1F*[100];
  TH1F **Sys_MetSmearedOnLine_1vtx = new TH1F*[100];
  double Chi2px[100] = {0.}, Chi2py[100] = {0.}, Chi2[100] = {0.};
  double sigma1px[100] = {0.}, sigma1py[100] = {0.}, sigma1[100] = {0.};
  for(int isigma=0; isigma<100; isigma++){
       //Px
       char numHistopx[5];
       sprintf(numHistopx,"%d",isigma);
       TString nameHistopx = "Sys_MetSmearedOnLine_px" + nvtx + "vtx_" + string(numHistopx);
       Sys_MetSmearedOnLine_px1vtx[isigma] = ( (TH1F*)File[TT]->Get( nameHistopx.Data() ));
       Sys_MetSmearedOnLine_px1vtx[isigma]->Scale(h_DataEmu_px1nvtx->Integral()/Sys_MetSmearedOnLine_px1vtx[isigma]->Integral());
       Chi2px[isigma] =  h_DataEmu_px1nvtx->Chi2Test(Sys_MetSmearedOnLine_px1vtx[isigma],"UW CHI2 P");
       sigma1px[isigma] = double(isigma)/2.;
       //Py
       char numHistopy[5];
       sprintf(numHistopy,"%d",isigma);
       TString nameHistopy = "Sys_MetSmearedOnLine_py" + nvtx + "vtx_" + string(numHistopy);
       Sys_MetSmearedOnLine_py1vtx[isigma] = ( (TH1F*)File[TT]->Get( nameHistopy.Data() ));
       Sys_MetSmearedOnLine_py1vtx[isigma]->Scale(h_DataEmu_py1nvtx->Integral()/Sys_MetSmearedOnLine_py1vtx[isigma]->Integral());
       Chi2py[isigma] =  h_DataEmu_py1nvtx->Chi2Test(Sys_MetSmearedOnLine_py1vtx[isigma],"UW CHI2 P");
       sigma1py[isigma] = double(isigma)/2.;  
       //Pt
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_" + nvtx + "vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_1vtx[isigma] = ( (TH1F*)File[TT]->Get( nameHisto.Data() ));
       Sys_MetSmearedOnLine_1vtx[isigma]->Scale(h_DataEmu_1nvtx->Integral()/Sys_MetSmearedOnLine_1vtx[isigma]->Integral());
       Chi2[isigma] =  h_DataEmu_1nvtx->Chi2Test(Sys_MetSmearedOnLine_1vtx[isigma],"UW CHI2 P");
       sigma1[isigma] = double(isigma)/2.;

       //myc1->cd();
       //h_DataEmu_py1nvtx->Draw();
       //Sys_MetSmearedOnLine_py1vtx[isigma]->Draw("HISTOsame");
       //output = outDir + nameHisto + ".png";
       //myc1->SaveAs(output.c_str());
  }
    //Px
    TGraph *h_Fsigma_px1nvtx = new TGraph(99, sigma1px, Chi2px);
    TF1 *ffit = new TF1("pol2","pol2",0.,50.);
    h_Fsigma_px1nvtx->Fit(ffit,"qB","", 0.1,45.);
    TF1 *ffit_min = new TF1("pol2","pol2",0.,50.);
    ffit_min->SetParameters(ffit->GetParameters()[0], ffit->GetParameters()[1], ffit->GetParameters()[2]);
    cout<<"Il minimo Per Px:"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.) )<<endl;
    cout<<"Il minimo Per Px Piu' uno e':"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)+1.,0.,ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)) )<<endl;
    cout<<"Il minimo Per Px Meno uno e':"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)+1.,ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)),50. )<<endl;  
    myc1->cd();
    h_Fsigma_px1nvtx->Draw("ACP");
    output = outDir+ "Sigma_px" + nvtx + "vtx.png";
    myc1->SaveAs(output.c_str());
    //Py
    TGraph *h_Fsigma_py1nvtx = new TGraph(99, sigma1py, Chi2py);
    h_Fsigma_py1nvtx->Fit(ffit,"qB","", 0.1,45.);
    ffit_min->SetParameters(ffit->GetParameters()[0], ffit->GetParameters()[1], ffit->GetParameters()[2]);
    cout<<"Il minimo Per Py:"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.) )<<endl;
    cout<<"Il minimo Per Py Piu' uno e':"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)+1.,0.,ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)) )<<endl;
    cout<<"Il minimo Per Py Meno uno e':"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)+1.,ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)),50. )<<endl;  
    myc1->cd();
    h_Fsigma_py1nvtx->Draw("ACP");
    output = outDir+ "Sigma_py" + nvtx + "vtx.png";
    myc1->SaveAs(output.c_str());
    //Pt
    TGraph *h_Fsigma_1nvtx = new TGraph(99, sigma1, Chi2);
    h_Fsigma_1nvtx->Fit(ffit,"qB","", 0.1,45.);
    ffit_min->SetParameters(ffit->GetParameters()[0], ffit->GetParameters()[1], ffit->GetParameters()[2]);
    cout<<"Il minimo Per Pt:"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.) )<<endl;
    cout<<"Il minimo Per Pt Piu' uno e':"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)+1.,0.,ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)) )<<endl;
    cout<<"Il minimo Per Pt Meno uno e':"<<ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)+1.,ffit_min->GetX( ffit_min->GetMinimum(0.1,45.)),50. )<<endl;  
    myc1->cd();
    h_Fsigma_1nvtx->Draw("ACP");
    output = outDir+ "Sigma_" + nvtx + "vtx.png";
    myc1->SaveAs(output.c_str());

}

