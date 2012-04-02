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
//#include "../bin/UserCode/lpernie/src/CommonTools/DrawLPBase.h"
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
int WZ_Estim = 1;
int CrossSec = 1;
int SistDy = 1;
int hadd = 1;

Double_t singleGaussian(Double_t *x, Double_t *par);
void singleGaus();
Double_t doubleGaussian(Double_t *x, Double_t *par);
void doubleGaus();
Double_t novosibirsk(Double_t *x, Double_t *par);
void novosibirskFunct();

double min_array(double *A, int n);
double min(double x, double y);

void FindMinAndVar(double *Chi2, double *sigma1 );

string jsonFile = "../data/samples.json";
string cutflowhisto = "all_cutflow";
std::map<string, double> PURescale_up;
std::map<string, double> PURescale_down;
std::map<string, double> initialNumberOfEvents;
//std::map<string, double> FinalWeight;
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
   gStyle->SetPadLeftMargin  (0.14);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);

   string output = "";

   for(int i=1;i<argc;i++){
     string arg(argv[i]);

     if(arg.find("--help")!=string::npos){
        printf("--help   --> print this helping text\n");
        printf("--iLumi     --> integrated luminosity to be used for the MC rescale\n");
        printf("--inDir     --> path to the directory containing the .root with Histo\n");
        printf("--outDir    --> path of the directory that will contains the output plots and tables\n");
        printf("--WZ_Estim  --> 1 if you want to compare different Met\n");
        printf("--hadd      --> 1 if you want to merge the dataset\n");
        printf("--CrossSec  --> 1 if you want to maximize S/sqrt(S+B)\n");
        printf("--SistDy    --> containing list of process (and associated style) to process to process\n");
        printf("--json      --> json file with all proces and info\n");
	return 0;
     }

     if(arg.find("--iLumi"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%lf",&iLumi); i++; printf("Lumi = %f\n", iLumi); }
     if(arg.find("--inDir"  )!=string::npos && i+1<argc){ inDir    = argv[i+1];  i++;  printf("inDir = %s\n", inDir.c_str());  }
     if(arg.find("--outDir" )!=string::npos && i+1<argc){ outDir   = argv[i+1];  i++;  printf("outDir = %s\n", outDir.c_str());  }
     if(arg.find("--WZ_Estim"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&WZ_Estim);  i++;  printf("WZ_Estim = %s\n", (WZ_Estim == 1)?"true":"false");  }
     if(arg.find("--hadd"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&hadd);  i++;  printf("hadd = %s\n", (hadd == 1)?"true":"false");  }
     if(arg.find("--CrossSec"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&CrossSec);  i++;  printf("CrossSec = %s\n", (CrossSec == 1)?"true":"false");  }   
     if(arg.find("--SistDy"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&SistDy);  i++;  printf("SistDy = %s\n", (SistDy == 1)?"true":"false");  }   
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
/*
     string DataEE_root = "Data_DoubleElectron.root";
     string DataMuMu_root = "Data_DoubleMu.root";
     string DataEMu_root = "Data_MuEG.root";
     string WZ_root = "MC_WZ.root";
     string WW_root = "MC_WW.root";
     string ZZ_root = "MC_ZZ.root";
     string DY_root = "MC_DYJetsToLL.root";
     string WJ_root = "MC_WJetsToLNu.root";
     string TT_root = "MC_TTJets.root";

	//Open Files
     string Path_DataEE = inDir + DataEE_root;         TFile* File_DataEE    =  TFile::Open( Path_DataEE.c_str() );
     string Path_DataMuMu = inDir + DataMuMu_root;     TFile* File_DataMuMu  =  TFile::Open( Path_DataMuMu.c_str() );
     string Path_DataEMu = inDir + DataEMu_root;       TFile* File_DataEMu   =  TFile::Open( Path_DataEMu.c_str() );
     string Path_WZ = inDir + WZ_root;                 TFile* File_WZ        =  TFile::Open( Path_WZ.c_str() );
     string Path_WW = inDir + WW_root;                 TFile* File_WW        =  TFile::Open( Path_WW.c_str() );
     string Path_ZZ = inDir + ZZ_root;                 TFile* File_ZZ        =  TFile::Open( Path_ZZ.c_str() );
     string Path_DY = inDir + DY_root;                 TFile* File_DY        =  TFile::Open( Path_DY.c_str() );
     string Path_TT = inDir + TT_root;                 TFile* File_TT        =  TFile::Open( Path_TT.c_str() );
     string Path_WJ = inDir + WJ_root;                 TFile* File_WJ        =  TFile::Open( Path_WJ.c_str() );
*/
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
/*
     double DYWeight = FinalWeight["MC_DYJetsToLL"];
     double ZZWeight = FinalWeight["MC_ZZ"];
     double WWWeight = FinalWeight["MC_WW"];
     double WZWeight = FinalWeight["MC_WZ"];
     double WJWeight = FinalWeight["MC_WJetsToLNu"];
     double TTWeight = FinalWeight["MC_TTJets"];*/

     std::vector<NameAndType> histlist;
     GetListOfObject(Root,inDir,histlist);

if( SistDy == 1 ){
  string emu_1nvtx = "emu_Sys_MetEff_RedMet_1nvtx";
  string emu_2nvtx = "emu_Sys_MetEff_RedMet_2nvtx";
  string emu_3nvtx = "emu_Sys_MetEff_RedMet_3nvtx";
  string emu_4nvtx = "emu_Sys_MetEff_RedMet_Maxnvtx";
  TH1F *h_TT_1nvtx = ( (TH1F*)File[TT]->Get( emu_1nvtx.c_str() )); //h_TT_1nvtx->Sumw2();
  TH1F *h_DataEmu_1nvtx = ( (TH1F*)File[EMU]->Get( emu_1nvtx.c_str() ));
  TH1F *h_TT_2nvtx = ( (TH1F*)File[TT]->Get( emu_2nvtx.c_str() ));
  TH1F *h_DataEmu_2nvtx = ( (TH1F*)File[EMU]->Get( emu_2nvtx.c_str() ));
  TH1F *h_TT_3nvtx = ( (TH1F*)File[TT]->Get( emu_3nvtx.c_str() ));
  TH1F *h_DataEmu_3nvtx = ( (TH1F*)File[EMU]->Get( emu_3nvtx.c_str() ));
  TH1F *h_TT_4nvtx = ( (TH1F*)File[TT]->Get( emu_4nvtx.c_str() ));
  TH1F *h_DataEmu_4nvtx = ( (TH1F*)File[EMU]->Get( emu_4nvtx.c_str() ));

  //singleGaus();
  h_TT_1nvtx->Scale(h_DataEmu_1nvtx->Integral()/h_TT_1nvtx->Integral());
  myc1->cd();
  h_DataEmu_1nvtx->Draw();
  h_TT_1nvtx->Draw("HISTOsame");
  output = outDir + emu_1nvtx + ".png";
  myc1->SaveAs(output.c_str());

  const int NBin = 30;
  double FakData[NBin]; for(int i=0 ; i<NBin; i++) FakData[i]=0;
  double sigma1[100]; for(int i=0 ; i<100; i++) sigma1[i]=0;
  double FunSig = 0.;

  double Chi2[100]={0.};
  double chi =  h_DataEmu_1nvtx->Chi2Test(h_TT_1nvtx,"UW CHI2 P");
  cout<<"Initial: "<<chi<<endl;
  cout<<"Initial Integral: "<<h_DataEmu_1nvtx->Integral()<<endl;
  
  double SUMprob=0.;
  
  TH1F *h1vtx = new TH1F("h1vtx", "", NBin, 0., 220.);
   for(int isigma = 1 ; isigma < 100; isigma++ ){
   double sigma = double(isigma)/2.;
   FunSig = 0.;
   for(int i=0 ; i<NBin; i++) FakData[i]=0;
   for(int i=0 ; i<NBin; i++) h1vtx->SetBinContent(i,0.);
  
   for( int i = 1; i < h_TT_1nvtx->GetNbinsX(); i++  ){
        SUMprob =0.;
        for(int j = 1; j < h_TT_1nvtx->GetNbinsX(); j++  ){
            
  
             double Prob = 0.5*fabs((TMath::Erf( (h_TT_1nvtx->GetBinCenter(h_TT_1nvtx->GetBin(j)) - h_TT_1nvtx->GetBinLowEdge(h_TT_1nvtx->GetBin(i)) )/sigma)-
                      TMath::Erf( (h_TT_1nvtx->GetBinCenter(h_TT_1nvtx->GetBin(j)) - (h_TT_1nvtx->GetBinLowEdge(h_TT_1nvtx->GetBin(i))+h_TT_1nvtx->GetBinWidth(i) )) /sigma)));
  
             FakData[i] += h_TT_1nvtx->GetBinContent(j)*Prob;
             SUMprob += Prob;
       }
    h1vtx->SetBinContent(i,FakData[i]);
    }
  chi = h_DataEmu_1nvtx->Chi2Test(h1vtx,"UW CHI2 P");
  cout<<"Sigma  "<<sigma<<"Prob: "<<chi<<endl;
  myc1->cd();
  h1vtx->Draw();
  stringstream ss;
  ss << isigma;
  output = outDir+ "Sigma_"+ss.str()+"_Smear_1vtx.png";
  myc1->SaveAs(output.c_str());
  Chi2[isigma] = chi;
  sigma1[isigma] = sigma;
   }
    TGraph *h_Fsigma_1nvtx = new TGraph(99, sigma1, Chi2);
    myc1->cd();
    h_Fsigma_1nvtx->Draw("ACP");
    output = outDir+ "Sigma_1vtx.png";
    myc1->SaveAs(output.c_str());

    FindMinAndVar(Chi2, sigma1 );
  //2vtx
  h_TT_2nvtx->Scale(h_DataEmu_2nvtx->Integral()/h_TT_2nvtx->Integral());
  myc1->cd();
  h_DataEmu_2nvtx->Draw();
  h_TT_2nvtx->Draw("HISTOsame");
  output = outDir + emu_2nvtx + ".png";
  myc1->SaveAs(output.c_str());
   for(int i=0 ; i<NBin; i++) Chi2[i] = 0.;
  TH1F *h2vtx = new TH1F("h2vtx", "", NBin, 0., 220.);
   for(int isigma = 1 ; isigma < 100; isigma++ ){
   double sigma = double(isigma)/2.;
   FunSig = 0.;
   for(int i=0 ; i<NBin; i++) FakData[i]=0;
   for(int i=0 ; i<NBin; i++) h2vtx->SetBinContent(i,0.);
  
   for( int i = 1; i < h_TT_2nvtx->GetNbinsX(); i++  ){
        for(int j = 1; j < h_TT_2nvtx->GetNbinsX(); j++  ){
            
             double Prob = 0.5*fabs((TMath::Erf( (h_TT_2nvtx->GetBinCenter(h_TT_2nvtx->GetBin(j)) - h_TT_2nvtx->GetBinLowEdge(h_TT_2nvtx->GetBin(i)) )/sigma)-
                      TMath::Erf( (h_TT_2nvtx->GetBinCenter(h_TT_2nvtx->GetBin(j)) - (h_TT_2nvtx->GetBinLowEdge(h_TT_2nvtx->GetBin(i))+h_TT_2nvtx->GetBinWidth(i) )) /sigma)));
  
             FakData[i] += h_TT_2nvtx->GetBinContent(j)*Prob;
       }
    h2vtx->SetBinContent(i,FakData[i]);
    }
  Chi2[isigma] = h_DataEmu_2nvtx->Chi2Test(h2vtx,"UW CHI2 P");
  cout<<"Sigma  "<<sigma<<"Prob: "<<chi<<endl;
  sigma1[isigma] = sigma;
   }
    TGraph *h_Fsigma_2nvtx = new TGraph(99, sigma1, Chi2);
    myc1->cd();
    h_Fsigma_2nvtx->Draw("ACP");
    output = outDir+ "Sigma_2vtx.png";
    myc1->SaveAs(output.c_str());
    FindMinAndVar(Chi2, sigma1 );

  //3vtx
  h_TT_3nvtx->Scale(h_DataEmu_3nvtx->Integral()/h_TT_3nvtx->Integral());
  myc1->cd();
  h_DataEmu_3nvtx->Draw();
  h_TT_3nvtx->Draw("HISTOsame");
  output = outDir + emu_3nvtx + ".png";
  myc1->SaveAs(output.c_str());
   for(int i=0 ; i<NBin; i++) Chi2[i] = 0.;
  TH1F *h3vtx = new TH1F("h3vtx", "", NBin, 0., 220.);
   for(int isigma = 1 ; isigma < 100; isigma++ ){
   double sigma = double(isigma)/2.;
   FunSig = 0.;
   for(int i=0 ; i<NBin; i++) FakData[i]=0;
   for(int i=0 ; i<NBin; i++) h3vtx->SetBinContent(i,0.);
  
   for( int i = 1; i < h_TT_3nvtx->GetNbinsX(); i++  ){
        for(int j = 1; j < h_TT_3nvtx->GetNbinsX(); j++  ){
            
             double Prob = 0.5*fabs((TMath::Erf( (h_TT_3nvtx->GetBinCenter(h_TT_3nvtx->GetBin(j)) - h_TT_3nvtx->GetBinLowEdge(h_TT_3nvtx->GetBin(i)) )/sigma)-
                      TMath::Erf( (h_TT_3nvtx->GetBinCenter(h_TT_3nvtx->GetBin(j)) - (h_TT_3nvtx->GetBinLowEdge(h_TT_3nvtx->GetBin(i))+h_TT_3nvtx->GetBinWidth(i) )) /sigma)));
  
             FakData[i] += h_TT_3nvtx->GetBinContent(j)*Prob;
       }
    h3vtx->SetBinContent(i,FakData[i]);
    }
  Chi2[isigma] = h_DataEmu_3nvtx->Chi2Test(h3vtx,"UW CHI2 P");
  cout<<"Sigma  "<<sigma<<"Prob: "<<chi<<endl;
  sigma1[isigma] = sigma;
   }
    TGraph *h_Fsigma_3nvtx = new TGraph(99, sigma1, Chi2);
    myc1->cd();
    h_Fsigma_3nvtx->Draw("ACP");
    output = outDir+ "Sigma_3vtx.png";
    myc1->SaveAs(output.c_str());
    FindMinAndVar(Chi2, sigma1 );

  //4vtx
  h_TT_4nvtx->Scale(h_DataEmu_4nvtx->Integral()/h_TT_4nvtx->Integral());
  myc1->cd();
  h_DataEmu_4nvtx->Draw();
  h_TT_4nvtx->Draw("HISTOsame");
  output = outDir + emu_4nvtx + ".png";
  myc1->SaveAs(output.c_str());
   for(int i=0 ; i<NBin; i++) Chi2[i] = 0.;
  TH1F *h4vtx = new TH1F("h4vtx", "", NBin, 0., 220.);
   for(int isigma = 1 ; isigma < 100; isigma++ ){
   double sigma = double(isigma)/2.;
   FunSig = 0.;
   for(int i=0 ; i<NBin; i++) FakData[i]=0;
   for(int i=0 ; i<NBin; i++) h4vtx->SetBinContent(i,0.);
  
   for( int i = 1; i < h_TT_4nvtx->GetNbinsX(); i++  ){
        for(int j = 1; j < h_TT_4nvtx->GetNbinsX(); j++  ){
            
             double Prob = 0.5*fabs((TMath::Erf( (h_TT_4nvtx->GetBinCenter(h_TT_4nvtx->GetBin(j)) - h_TT_4nvtx->GetBinLowEdge(h_TT_4nvtx->GetBin(i)) )/sigma)-
                      TMath::Erf( (h_TT_4nvtx->GetBinCenter(h_TT_4nvtx->GetBin(j)) - (h_TT_4nvtx->GetBinLowEdge(h_TT_4nvtx->GetBin(i))+h_TT_4nvtx->GetBinWidth(i) )) /sigma)));
  
             FakData[i] += h_TT_4nvtx->GetBinContent(j)*Prob;
       }
    h4vtx->SetBinContent(i,FakData[i]);
    }
  Chi2[isigma] = h_DataEmu_4nvtx->Chi2Test(h4vtx,"UW CHI2 P");
  cout<<"Sigma  "<<sigma<<"Prob: "<<chi<<endl;
  sigma1[isigma] = sigma;
   }
    TGraph *h_Fsigma_4nvtx = new TGraph(99, sigma1, Chi2);
    myc1->cd();
    h_Fsigma_4nvtx->Draw("ACP");
    output = outDir+ "Sigma_4vtx.png";
    myc1->SaveAs(output.c_str());
    FindMinAndVar(Chi2, sigma1 );
  
  cout<<endl;
  cout<<"Efficiency (ee): "<<((TH1F*)File[ZZ]->Get( "ee_Effic" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1)<<endl;
  cout<<"Efficiency (mumu): "<<((TH1F*)File[ZZ]->Get( "mumu_Effic" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1)<<endl;
  cout<<"New Efficiency (ee): "<<( (TH1F*)File[ZZ]->Get("ee_Effic_MetSmear"))->GetBinContent(1)/( (TH1F*)File[ZZ]->Get("ee_Effic_tot"))->GetBinContent(1)<<endl;
  cout<<"New Efficiency (mumu): "<<( (TH1F*)File[ZZ]->Get("mumu_Effic_MetSmear"))->GetBinContent(1)/( (TH1F*)File[ZZ]->Get("mumu_Effic_tot"))->GetBinContent(1)<<endl;
  cout<<"New Efficiency minus (ee): "<<( (TH1F*)File[ZZ]->Get("ee_Effic_MetSmearm"))->GetBinContent(1)/( (TH1F*)File[ZZ]->Get("ee_Effic_tot"))->GetBinContent(1)<<endl;
  cout<<"New Efficiency minus (mumu): "<<( (TH1F*)File[ZZ]->Get("mumu_Effic_MetSmearm"))->GetBinContent(1)/( (TH1F*)File[ZZ]->Get("mumu_Effic_tot"))->GetBinContent(1)<<endl;
  cout<<"New Efficiency plus (ee): "<<( (TH1F*)File[ZZ]->Get("ee_Effic_MetSmearp"))->GetBinContent(1)/( (TH1F*)File[ZZ]->Get("ee_Effic_tot"))->GetBinContent(1)<<endl;
  cout<<"New Efficiency plus (mumu): "<<( (TH1F*)File[ZZ]->Get("mumu_Effic_MetSmearp"))->GetBinContent(1)/( (TH1F*)File[ZZ]->Get("mumu_Effic_tot"))->GetBinContent(1)<<endl;

}

if( CrossSec == 1 ){
     cout<<endl;
     cout<<"Cross Section Measurement... Get Ready!!"<<endl;

     string NEvFin_ee   = "ee_ZZ_eventflow";
     string NEvFin_mumu = "mumu_ZZ_eventflow";
     double Nevent_ee[NInput];
     for(int i=0; i<NInput; i++){  Nevent_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(9)*iWeight[i]; }
     double Nevent_mumu[NInput];
     for(int i=0; i<NInput; i++){  Nevent_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(9)*iWeight[i]; }

     double Nbkg_ee = Nevent_ee[WW] + Nevent_ee[WZ] + Nevent_ee[DY] + Nevent_ee[TT] + Nevent_ee[WJ];
     double EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     double CrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     double Error_ee = ( sqrt(Nevent_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section: Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     double Nbkg_mumu = Nevent_mumu[WW]+Nevent_mumu[WZ]+Nevent_mumu[DY]+Nevent_mumu[TT]+Nevent_mumu[WJ];
     double EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
     double CrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     double Error_mumu = ( sqrt(Nevent_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section: Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;

     //Syst on Met (Jer)
     double EffZZJer_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficJer" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     double JerCrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZJer_ee);
     cout<< "Syst on Met ee (Eff:"<<EffZZJer_ee<<") (Jer): "<< (JerCrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;
     double EffZZJer_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficJer" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
     double JerCrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZJer_mumu);
     cout<< "Syst on Met mumu  (Eff:"<<EffZZJer_mumu<<")  (Jer): "<< (JerCrossSection_mumu-CrossSection_mumu)*100./CrossSection_mumu<<"%"<<endl;

     //Syst on Met (Jesm)
     double EffZZJes_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficJesm" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     double JesCrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZJes_ee);
     cout<< "Syst on Met (ee) (Eff:"<<EffZZJes_ee<<") (Jes m): "<< (JesCrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;
     double EffZZJes_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficJesm" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
     double JesCrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZJes_mumu);
     cout<< "Syst on Met (mumu)  (Eff:"<<EffZZJes_mumu<<")  (Jes m): "<< (JesCrossSection_mumu-CrossSection_mumu)*100./CrossSection_mumu<<"%"<<endl;
     //Syst on Met (Jesp)
     EffZZJes_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficJesp" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     JesCrossSection_ee = (Nevent_ee[EE] - Nbkg_ee)/(iLumi*EffZZJes_ee);
     cout<< "Syst on Met (ee) (Eff:"<<EffZZJes_ee<<") (Jer p): "<< (JesCrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;
     EffZZJes_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficJesp" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
     JesCrossSection_mumu = (Nevent_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZJes_mumu);
     cout<< "Syst on Met (mumu)  (Eff:"<<EffZZJes_mumu<<")  (Jer p): "<< (JesCrossSection_mumu-CrossSection_mumu)*100./CrossSection_mumu<<"%"<<endl;

     //Syst on PU
     //PUm
     string pu_NEvFin_ee   = "ee_eventflow_minus";
     string pu_NEvFin_mumu = "mumu_eventflow_minus";
     double NeventPUm_ee[NInput];
     for(int i=0; i<NInput; i++){  NeventPUm_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(9)*iWeight[i]; }
     double NeventPUm_mumu[NInput];
     for(int i=0; i<NInput; i++){  NeventPUm_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(9)*iWeight[i]; }
 
     double pu_CrossSection_ee = 0.;
     double pu_Nbkg_ee = NeventPUm_ee[WW] + NeventPUm_ee[WZ] + NeventPUm_ee[DY] + NeventPUm_ee[TT] + NeventPUm_ee[WJ];
     double pu_EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficPUm" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_EfficPUm_tot" ))->GetBinContent(1);
     pu_CrossSection_ee = (NeventPUm_ee[EE] - Nbkg_ee)/(iLumi*pu_EffZZ_ee);
     cout<< "Syst on PUm ee (Cross:"<<pu_CrossSection_ee<<"): "<< (pu_CrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;

     double pu_CrossSection_mumu = 0.;
     double pu_Nbkg_mumu = NeventPUm_mumu[WW] + NeventPUm_mumu[WZ] + NeventPUm_mumu[DY] + NeventPUm_mumu[TT] + NeventPUm_mumu[WJ];
     double pu_EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficPUm" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_EfficPUm_tot" ))->GetBinContent(1);
     pu_CrossSection_mumu = (NeventPUm_mumu[MuMu] - Nbkg_mumu)/(iLumi*pu_EffZZ_mumu);
     cout<< "Syst on PUm mumu ("<<pu_CrossSection_mumu<<"): "<< (pu_CrossSection_mumu-CrossSection_mumu)*100/CrossSection_mumu<<"%"<<endl;

     //PUp
     pu_NEvFin_ee   = "ee_eventflow_minus";
     pu_NEvFin_mumu = "mumu_eventflow_minus";
     double NeventPUp_ee[NInput];
     for(int i=0; i<NInput; i++){  NeventPUp_ee[i] = ((TH1F*)File[i]->Get( NEvFin_ee.c_str() ))->GetBinContent(9)*iWeight[i]; }
     double NeventPUp_mumu[NInput];
     for(int i=0; i<NInput; i++){  NeventPUp_mumu[i] = ((TH1F*)File[i]->Get( NEvFin_mumu.c_str() ))->GetBinContent(9)*iWeight[i]; }

     pu_CrossSection_ee = 0., pu_EffZZ_ee =0.;
     pu_Nbkg_ee = NeventPUp_ee[WW] + NeventPUp_ee[WZ] + NeventPUp_ee[DY] + NeventPUp_ee[TT] + NeventPUp_ee[WJ];
     pu_EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_EfficPUp" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_EfficPUp_tot" ))->GetBinContent(1);
     pu_CrossSection_ee = (NeventPUp_ee[EE] - Nbkg_ee)/(iLumi*pu_EffZZ_ee);
     cout<< "Syst on PUp ee ("<<pu_CrossSection_ee<<"): "<< (pu_CrossSection_ee-CrossSection_ee)*100/CrossSection_ee<<"%"<<endl;

     pu_CrossSection_mumu = 0.; pu_EffZZ_mumu =0.;
     pu_Nbkg_mumu = NeventPUp_mumu[WW] + NeventPUp_mumu[WZ] + NeventPUp_mumu[DY] + NeventPUp_mumu[TT] + NeventPUp_mumu[WJ];
     pu_EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_EfficPUp" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_EfficPUp_tot" ))->GetBinContent(1);
     pu_CrossSection_mumu = (NeventPUp_mumu[MuMu] - Nbkg_mumu)/(iLumi*pu_EffZZ_mumu);
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
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin1" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     CrossSection_ee = (NeventPt1_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt1_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (70>Pt>90): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt1_mumu[WW] + NeventPt1_mumu[WZ] + NeventPt1_mumu[DY] + NeventPt1_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin1" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
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
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin2" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     CrossSection_ee = (NeventPt2_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt2_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (90>Pt>110): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt2_mumu[WW] + NeventPt2_mumu[WZ] + NeventPt2_mumu[DY] + NeventPt2_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin2" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
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
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin3" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     CrossSection_ee = (NeventPt3_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt3_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (110>Pt>140): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt3_mumu[WW] + NeventPt3_mumu[WZ] + NeventPt3_mumu[DY] + NeventPt3_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin3" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
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
     EffZZ_ee = ((TH1F*)File[ZZ]->Get( "ee_Effic_ptBin4" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "ee_Effic_tot" ))->GetBinContent(1);
     CrossSection_ee = (NeventPt4_ee[EE] - Nbkg_ee)/(iLumi*EffZZ_ee);
     Error_ee = ( sqrt(NeventPt4_ee[EE] + Nbkg_ee))/(iLumi*EffZZ_ee);
     cout<<endl; cout<<"Cross Section (Pt>140): Sigma x B.r.(pp-ZZ-eevv):  "<<CrossSection_ee<< " /pm " << Error_ee <<endl;

     CrossSection_mumu = 0. ;
     Nbkg_mumu = NeventPt4_mumu[WW] + NeventPt4_mumu[WZ] + NeventPt4_mumu[DY] + NeventPt4_mumu[TT]; // +WJ_Nev_mumu;
     EffZZ_mumu = ((TH1F*)File[ZZ]->Get( "mumu_Effic_ptBin4" ))->GetBinContent(1)/((TH1F*)File[ZZ]->Get( "mumu_Effic_tot" ))->GetBinContent(1);
     CrossSection_mumu = (NeventPt4_mumu[MuMu] - Nbkg_mumu)/(iLumi*EffZZ_mumu);
     Error_mumu = ( sqrt(NeventPt4_mumu[MuMu] + Nbkg_mumu))/(iLumi*EffZZ_mumu);
     cout<<endl; cout<<"Cross Section (Pt>140): Sigma x B.r.(pp-ZZ-mumuvv):  "<<CrossSection_mumu<< " /pm " << Error_mumu <<endl;
}

if( WZ_Estim == 1 ){
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
