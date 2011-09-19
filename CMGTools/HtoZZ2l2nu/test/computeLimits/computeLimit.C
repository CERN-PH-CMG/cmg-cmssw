
#include <string>
#include <vector>

#include "TROOT.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TChain.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
#include "TF1.h"
#include "TCutG.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"
#include "tdrstyle.C"
#include "fitRvsCLs.C"

TObject* GetObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false)
{
   size_t pos = Path.find("/");
   if(pos < 256){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());
//      if(TMP!=NULL)return GetObjectFromPath(TMP,endPart,GetACopy);
      if(TMP!=NULL){
         TObject* TMP2 =  GetObjectFromPath(TMP,endPart,GetACopy);
         if(!TMP2)printf("BUG: %s\n",Path.c_str());
         return TMP2;
      }

      printf("BUG: %s\n",Path.c_str());
      return NULL;
   }else{
      TObject* TMP = File->Get(Path.c_str());
      if(!TMP)printf("BUG: %s\n",Path.c_str());
       
      if(GetACopy){
         return TMP->Clone();
      }else{
         return TMP;
      }
   }

}

struct stYields{double Rate; double Err;};

stYields Efficiency(TFile* File, std::string Sample, std::string HistoName, int cutIndex){
   stYields toReturn;

   size_t fpos = Sample.find(";");
   if(fpos==std::string::npos){
      if(cutIndex<8){
         TH1D* hist = (TH1D*) GetObjectFromPath(File,Sample+HistoName);
         toReturn.Rate = hist->GetBinContent(cutIndex);
         toReturn.Err  = hist->GetBinError(cutIndex);
      }else{
         TH1D* hist = (TH1D*) GetObjectFromPath(File,Sample+HistoName);
         toReturn.Rate=0;   toReturn.Err=0;
         for(int i=hist->GetXaxis()->FindBin(0.5);i<=hist->GetNbinsX();i++){toReturn.Rate+=hist->GetBinContent(i);  toReturn.Err+=pow(hist->GetBinError(i),2);}
         toReturn.Err = sqrt(toReturn.Err);
      }
   }else{
      std::string subStringA = Sample.substr(0,fpos);
      std::string subStringB = Sample.substr(fpos+1);
      stYields yieldsA = Efficiency(File,subStringA,HistoName,cutIndex);
      stYields yieldsB = Efficiency(File,subStringB,HistoName,cutIndex);
      toReturn.Rate= yieldsA.Rate + yieldsB.Rate;
      toReturn.Err = sqrt(pow(yieldsA.Err,2) + pow(yieldsB.Err,2));
   }
   return toReturn;
}


stYields SoverN(TFile* File, std::string Sample1, std::string Sample2, std::string HistoName, int cutIndex){
   stYields S = Efficiency   (File,Sample1,HistoName, cutIndex);
   stYields B = Efficiency   (File,Sample2,HistoName, cutIndex);
   stYields toReturn;
   toReturn.Rate = S.Rate/B.Rate;
   toReturn.Err  = sqrt( pow(S.Err/B.Rate,2) + pow(B.Err*S.Rate/(B.Rate*B.Rate),2) );   
   return toReturn;
}

struct Limit {double Obs; double Exp_minus2s; double Exp_minus1s; double Exp_Mean; double Exp_plus1s; double Exp_plus2s; double Exp_Median;};


Limit GetLimitFromFile(string File){
  FILE* pFile = fopen(File.c_str(),"r");

  Limit toReturn;

  char Buffer[2048];
  while(!feof(pFile)){
     fgets(Buffer,2048,pFile);
     string Line = Buffer;
     if(strstr(Buffer,"Observed Upper Limit on the ratio R at 95% CL = ")>0){
        sscanf(Buffer,"Observed Upper Limit on the ratio R at 95%% CL = %lf\n",&toReturn.Obs);        
     }else if(strstr(Buffer,"BANDS")>0){
        sscanf(Buffer, "BANDS   %lf %lf %lf %lf %lf %lf\n",&toReturn.Exp_minus2s, &toReturn.Exp_minus1s, &toReturn.Exp_Mean, &toReturn.Exp_plus1s, &toReturn.Exp_plus2s, &toReturn.Exp_Median);
     }
  }
  fclose(pFile);
  return toReturn;
}


Limit GetLimit(TString inFileName, TString plotName, TString sfile="bands", double mH = -1){
        Limit toReturn;
	double m2s =	extractLimitAtQuantile(inFileName, plotName+"_-2sigma", 0.0275 );
//	double m2s_err = limitErr;
	double m1s =	extractLimitAtQuantile(inFileName, plotName+"_-1sigma", 0.16 );
//	double m1s_err = limitErr;
	double med =	extractLimitAtQuantile(inFileName, plotName+"_median", 0.5 );
//	double med_err = limitErr;
	double p1s =	extractLimitAtQuantile(inFileName, plotName+"_1sigma", 0.84 );
//	double p1s_err = limitErr;
	double p2s =	extractLimitAtQuantile(inFileName, plotName+"_2sigma", 0.975 );
//	double p2s_err = limitErr;
	double dat =	extractLimitAtQuantile(inFileName, plotName+"_observed", -1 );
//	double dat_err = limitErr;
/*
	cout<<"EXPECTED LIMIT BANDS from(obs, -2s,-1s,median,1s,2s) mass= "<<mH<<": ";
	cout<<dat<<"+/-"<<dat_err<<", ";
	cout<<m2s<<"+/-"<<m2s_err<<", "<<m1s<<"+/-"<<m1s_err<<", ";
	cout<<med<<"+/-"<<med_err<<", ";
	cout<<p1s<<"+/-"<<p1s_err<<", "<<p2s<<"+/-"<<p2s_err<<endl;

	cout<<"Observed data limit: "<<dat<<endl;
	cout<<"Observed data limit: "<<dat<<" +/- "<<dat_err<<endl;
	cout<<"expected median limit: "<<med<<" +/- "<<med_err<<endl;
	SaveResults(sfile, mH, dat, dat_err, 0, 0, m2s, m1s, med, 0, p1s, p2s);
*/
        toReturn.Obs = dat;
        toReturn.Exp_minus2s = m2s;
        toReturn.Exp_minus1s = m1s;
        toReturn.Exp_Mean    = med;
        toReturn.Exp_Median  = med;
        toReturn.Exp_plus1s = p1s;
        toReturn.Exp_plus2s = p2s;
        return toReturn;
}



TCutG* GetErrorBand(string name, int N, double* Mass, double* Low, double* High)
{
   TCutG* cutg = new TCutG(name.c_str(),2*N);
   cutg->SetFillColor(kGreen-7);
   for(int i=0;i<N;i++){
//      double Min = std::max(Low[i],PlotMinScale);
//      cutg->SetPoint( i,Mass[i], Min);
      cutg->SetPoint( i,Mass[i], Low[i]);
   }
   for(int i=0;i<N;i++){
//      double Max = std::min(High[N-1-i],PlotMaxScale);
//      cutg->SetPoint(N+i,Mass[N-1-i], Max);
        cutg->SetPoint(N+i,Mass[N-1-i], High[N-1-i]);
   }
   return cutg;
}



void computeLimit(){
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

   std::vector<std::string> Backgrounds;			std::vector<std::string> BackgroundsLeg;
   Backgrounds.push_back("ZZ");					BackgroundsLeg.push_back("ZZ");
   Backgrounds.push_back("WW");					BackgroundsLeg.push_back("WW");
   Backgrounds.push_back("WZ");					BackgroundsLeg.push_back("WZ");
   Backgrounds.push_back("Single top");				BackgroundsLeg.push_back("Single top");
   Backgrounds.push_back("t#bar{t}");				BackgroundsLeg.push_back("tT");
   Backgrounds.push_back("W+jets");				BackgroundsLeg.push_back("W+jets");
   Backgrounds.push_back("Z-#gamma^{*}+jets#rightarrow ll");	BackgroundsLeg.push_back("DY");

   //TFile* File = new TFile("../plotter.root");
   TFile* File = new TFile("/afs/cern.ch/user/p/psilva/public/plotter.root");
   std::vector<std::string> Sample;               std::vector<std::string> Legend;
   Sample.push_back("H(200)#rightarrow VV");      Legend.push_back("H200");
   Sample.push_back("H(300)#rightarrow VV");      Legend.push_back("H300");
   Sample.push_back("H(400)#rightarrow VV");      Legend.push_back("H400");
   Sample.push_back("H(500)#rightarrow VV");      Legend.push_back("H500");
   Sample.push_back("H(600)#rightarrow VV");      Legend.push_back("H600");
   Sample.push_back("data");                      Legend.push_back("Data");
   std::string tmpS = Backgrounds[0];
   for(unsigned int i=1;i<Backgrounds.size();i++){tmpS += ";" + Backgrounds[i];}
   Sample.push_back(tmpS);                         Legend.push_back("Backgrounds");
   for(unsigned int i=0;i<Backgrounds.size();i++){Sample.push_back(Backgrounds[i]); Legend.push_back(BackgroundsLeg[i]);}

   printf("Samples & mumu (0Jets) & mumu (1Jets) & mumu (2Jets) & mumu (vbf) & ee (0Jets) & ee (1Jets) & ee (2Jets) & ee (vbf) \\\\\n");
   for(unsigned int s=0;s<Sample.size();s++){
      stYields tmp;
      printf("%-10s &"    ,Legend[s].c_str());
/*      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumueq0jets_eventflow" ,6),EfficiencyErr(File,Sample[s],"/mumueq0jets_eventflow" ,6));
      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumueq1jets_eventflow" ,6),EfficiencyErr(File,Sample[s],"/mumueq1jets_eventflow" ,6));
      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumugeq2jets_eventflow",6),EfficiencyErr(File,Sample[s],"/mumugeq2jets_eventflow",6));
      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumuvbf_eventflow"     ,6),EfficiencyErr(File,Sample[s],"/mumuvbf_eventflow"     ,6));
      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eeeq0jets_eventflow"   ,6),EfficiencyErr(File,Sample[s],"/eeeq0jets_eventflow"   ,6));
      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eeeq1jets_eventflow"   ,6),EfficiencyErr(File,Sample[s],"/eeeq1jets_eventflow"   ,6));
      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eegeq2jets_eventflow"  ,6),EfficiencyErr(File,Sample[s],"/eegeq2jets_eventflow"  ,6));
      printf(" %6.2E+-%6.2E  ",Efficiency(File,Sample[s],"/eevbf_eventflow"       ,6),EfficiencyErr(File,Sample[s],"/eevbf_eventflow"       ,6));
*/

//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eeeq0jets_eventflow"   ,6),EfficiencyErr(File,Sample[s],"/eeeq0jets_eventflow"   ,6));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eeeq1jets_eventflow"   ,6),EfficiencyErr(File,Sample[s],"/eeeq1jets_eventflow"   ,6));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eegeq2jets_eventflow"  ,6),EfficiencyErr(File,Sample[s],"/eegeq2jets_eventflow"  ,6));
      tmp = Efficiency(File,Sample[s],"/eevbf_eventflow"       ,6); printf(" %6.2E+-%6.2E &",tmp.Rate,tmp.Err);
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumueq0jets_eventflow" ,6),EfficiencyErr(File,Sample[s],"/mumueq0jets_eventflow" ,6));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumueq1jets_eventflow" ,6),EfficiencyErr(File,Sample[s],"/mumueq1jets_eventflow" ,6));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumugeq2jets_eventflow",6),EfficiencyErr(File,Sample[s],"/mumugeq2jets_eventflow",6));
      tmp = Efficiency(File,Sample[s],"/mumuvbf_eventflow"     ,6); printf(" %6.2E+-%6.2E &",tmp.Rate,tmp.Err);
      printf("\\\\\n");
   }


   printf("Samples        : ");for(unsigned int s=0;s<Sample.size();s++){printf(" & %-18s"    ,Legend[s].c_str());}printf("\\\n");
   printf("mumu 0Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumueq0jets_eventflow" ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 1Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumueq1jets_eventflow" ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 2Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumugeq2jets_eventflow",6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu VBF   bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumuvbf_eventflow"     ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   0Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eeeq0jets_eventflow"   ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   1Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eeeq1jets_eventflow"   ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   2Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eegeq2jets_eventflow"  ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   VBF   bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eevbf_eventflow"       ,6); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");

   printf("Samples        : ");for(unsigned int s=0;s<5;s++){printf(" & %-18s"        ,Legend[s].c_str());}printf("\\\n");
   printf("mumu 0Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumueq0jets_eventflow" ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 1Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumueq1jets_eventflow" ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 2Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumugeq2jets_eventflow",6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu VBF   bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumuvbf_eventflow"     ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   0Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eeeq0jets_eventflow"   ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   1Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eeeq1jets_eventflow"   ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   2Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eegeq2jets_eventflow"  ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   VBF   bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eevbf_eventflow"       ,6); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");

   std::string data = "data";
   std::string VV  = "ZZ;WW;WZ";
   std::string Top = "t#bar{t};Single top";
   std::string DY  = "Z-#gamma^{*}+jets#rightarrow ll";
   std::string WJ  = "W+jets";
   std::string H[6];  std::string mass_H[6];
   double HErr[6];
   H[0] = "H(200)#rightarrow VV";   mass_H[0] = "200";  HErr[0] = 1.108958801;
   H[1] = "H(300)#rightarrow VV";   mass_H[1] = "300";  HErr[1] = 1.111920863;
   H[2] = "H(400)#rightarrow VV";   mass_H[2] = "400";  HErr[2] = 1.118228673;
   H[3] = "H(500)#rightarrow VV";   mass_H[3] = "500";  HErr[3] = 1.126111468;
   H[4] = "H(600)#rightarrow VV";   mass_H[4] = "600";  HErr[4] = 1.135045566;
   double ObsLimit[5];  
   double ExpLimitm2[5];
   double ExpLimitm1[5];
   double ExpLimit[5];
   double ExpLimitp1[5];
   double ExpLimitp2[5];
   double MassAxis[5];

   string Model[]={"ggvbf", "vbf","gg"};
   int NModel    = sizeof(Model)/sizeof(string);

   for(unsigned int M=0;M<sizeof(Model)/sizeof(string);M++){
   for(int i=0;i<5;i++){
      string* Channels; string* HistoName; int* BinForYields;
      int NChannels; 
      if(Model[M]=="" || Model[M]=="ggvbf"){
         NChannels= 8;
         Channels = new string[NChannels];   	HistoName=new string[NChannels];    BinForYields = new int[NChannels];
         Channels[0]="mumueq0jets";    		HistoName[0]="finaleventflow";      BinForYields[0] = i+1;
         Channels[1]="mumueq1jets";    		HistoName[1]="finaleventflow";      BinForYields[1] = i+1;
         Channels[2]="mumugeq2jets";   		HistoName[2]="finaleventflow";      BinForYields[2] = i+1;
         Channels[3]="mumuvbf";        		HistoName[3]="finaleventflow";      BinForYields[3] = i+1;
         Channels[4]="eeeq0jets";    		HistoName[4]="finaleventflow";      BinForYields[4] = i+1;
         Channels[5]="eeeq1jets";    		HistoName[5]="finaleventflow";      BinForYields[5] = i+1;
         Channels[6]="eegeq2jets";   		HistoName[6]="finaleventflow";      BinForYields[6] = i+1;
         Channels[7]="eevbf";        		HistoName[7]="finaleventflow";      BinForYields[7] = i+1;
      }else if(Model[M]=="gg"){
         NChannels= 6;
         Channels = new string[NChannels];      HistoName=new string[NChannels];    BinForYields = new int[NChannels];
         Channels[0]="mumueq0jets";             HistoName[0]="finaleventflow";      BinForYields[0] = i+1;
         Channels[1]="mumueq1jets";             HistoName[1]="finaleventflow";      BinForYields[1] = i+1; 
         Channels[2]="mumugeq2jets";            HistoName[2]="finaleventflow";      BinForYields[2] = i+1;
         Channels[3]="eeeq0jets";               HistoName[3]="finaleventflow";      BinForYields[3] = i+1;
         Channels[4]="eeeq1jets";               HistoName[4]="finaleventflow";      BinForYields[4] = i+1;
         Channels[5]="eegeq2jets";              HistoName[5]="finaleventflow";      BinForYields[5] = i+1;
      }else if(Model[M]=="vbf"){
         NChannels= 2;
         Channels = new string[NChannels];      HistoName=new string[NChannels];    BinForYields = new int[NChannels];
         Channels[0]="mumuvbf";                 HistoName[0]="finaleventflow";      BinForYields[0] = i+1;
         Channels[1]="eevbf";                   HistoName[1]="finaleventflow";      BinForYields[1] = i+1;
      }else{
         printf("Model %s is unknown\n",Model[M].c_str());exit(0);
      }
//    string Channels[] = {"mumueq0jets", "mumueq1jets", "mumugeq2jets", "eeeq0jets", "eeeq1jets", "eegeq2jets"};
//    string HistoName[] = {"_finaleventflow","_finaleventflow", "_finaleventflow", "_finaleventflow","_finaleventflow", "_finaleventflow"};
//    int    BinForYields[] = {i+1, i+1, i+1, i+1, i+1, i+1};
      string subChannelsDir[] = {H[i],DY,WJ, VV,Top};
      double subChannelsThErr[] = {HErr[i],1.04345, 1.04976, sqrt(pow(1.0254,2) + pow(1.0349,2) + pow(1.0385,2)),sqrt(pow(1.0606,2) + pow(1.0334,2))};

      string subChannels[] = {"higgs","DY", "W+J", "VV", "Top"};
      int NsubChannels = sizeof(subChannels)/sizeof(string);
   
      FILE* pFile = fopen((string("datacard_") + Model[M] + mass_H[i] + ".txt").c_str(),"w");
      fprintf(pFile,"****datacard ******\n");
      fprintf(pFile,"\n");
      fprintf(pFile,"# Counting experiment with multiple channels for testing\n");
      fprintf(pFile,"imax %i  number of channels\n",NChannels);
      fprintf(pFile,"jmax *  number of backgrounds ('*' = automatic)\n");
      fprintf(pFile,"kmax %i  number of nuisance parameters (sources of systematical uncertainties)\n",7+NChannels*NsubChannels);
      fprintf(pFile,"------------\n");
      fprintf(pFile,"# two channels, each with it's number of observed events\n");
      fprintf(pFile,"bin         ");for(int c=0;c<NChannels;c++){fprintf(pFile,"%7s   ",Channels[c].c_str());}fprintf(pFile,"\n");
      fprintf(pFile,"observation ");for(int c=0;c<NChannels;c++){fprintf(pFile,"%7f   ",Efficiency(File,data   ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]).Rate);}fprintf(pFile,"\n");
      fprintf(pFile,"------------\n");
      fprintf(pFile,"# now we list the expected events for signal and all backgrounds in\n");
      fprintf(pFile,"those three bins\n");
      fprintf(pFile,"# the second 'process' line must have a positive number for\n");
      fprintf(pFile,"backgrounds, and 0 for signal\n");
      fprintf(pFile,"# for the signal, we normalize the yields to an hypothetical cross\n");
      fprintf(pFile,"section of 1/pb\n");
      fprintf(pFile,"# so that we get an absolute limit in cross section in units of pb.\n");
      fprintf(pFile,"# then we list the independent sources of uncertainties, and give\n");
      fprintf(pFile,"their effect (syst. error)\n");
      fprintf(pFile,"# on each process and bin\n");
      fprintf(pFile,"bin          ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7s   ",Channels[c].c_str())    ;}fprintf(pFile,"\n");
      fprintf(pFile,"process      ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7s   ",subChannels[sc].c_str());}fprintf(pFile,"\n");
      fprintf(pFile,"process      ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7i   ",sc)                     ;}fprintf(pFile,"\n");
      fprintf(pFile,"rate         ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7f   ",Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]).Rate);}fprintf(pFile,"\n");

      fprintf(pFile,"------------\n");
      fprintf(pFile,"lumi     lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7f   ",1.045);}fprintf(pFile,"\n");
      fprintf(pFile,"thxsec   lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7f   ",subChannelsThErr[sc]);}fprintf(pFile,"\n");
      fprintf(pFile,"jesup    lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){stYields tmpSyst = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "jesup" + HistoName[c] ,BinForYields[c]); stYields tmp = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]); fprintf(pFile,"%7f   ",tmp.Rate==0.0 ? 1.0 : tmpSyst.Rate/tmp.Rate);}}fprintf(pFile,"\n");
      fprintf(pFile,"jesdown  lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){stYields tmpSyst = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "jesdown" + HistoName[c] ,BinForYields[c]); stYields tmp = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]); fprintf(pFile,"%7f   ",tmp.Rate==0.0 ? 1.0 : tmpSyst.Rate/tmp.Rate);}}fprintf(pFile,"\n");
      fprintf(pFile,"factscal lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){stYields tmpSystA = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "hfactup" + HistoName[c] ,BinForYields[c]); stYields tmpSystB = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "hfactdown" + HistoName[c] ,BinForYields[c]); stYields tmp = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]); fprintf(pFile,"%7f   ",tmp.Rate==0.0 ? 1.0 : std::max(tmpSystA.Rate,tmpSystB.Rate)/tmp.Rate);}}fprintf(pFile,"\n");
      fprintf(pFile,"renscale lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){stYields tmpSystA = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "hrenup" + HistoName[c] ,BinForYields[c]); stYields tmpSystB = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "hrendown" + HistoName[c] ,BinForYields[c]); stYields tmp = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]); fprintf(pFile,"%7f   ",tmp.Rate==0.0 ? 1.0 : std::max(tmpSystA.Rate,tmpSystB.Rate)/tmp.Rate);}}fprintf(pFile,"\n");
      fprintf(pFile,"pileup   lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){stYields tmpSyst = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + "flatpu" + HistoName[c] ,BinForYields[c]); stYields tmp = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]); fprintf(pFile,"%7f   ",tmp.Rate==0.0 ? 1.0 : tmpSyst.Rate/tmp.Rate);}}fprintf(pFile,"\n");

      for(int u=0;u<NChannels*NsubChannels;u++){
         fprintf(pFile,"MCStat%02i lnN ",u);
         for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
            if((c*NsubChannels+sc)==u){stYields tmp = Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]); fprintf(pFile,"%f   ",1.0+( tmp.Rate==0.0 ? 0 : (tmp.Err/tmp.Rate) ));
            }else{                fprintf(pFile,"%f   ",1.0);
            }      
         }}fprintf(pFile,"\n");
      }

      fclose(pFile);


      char Buffer[1024];
      printf("Running LandS on Higgs %s:",mass_H[i].c_str());
 //   sprintf(Buffer,"../../../../UserCode/mschen/LandS/test/lands.exe -M Bayesian -d %s --doExpectation 1 -t 1000000 > %s",(string("datacard_") + Model[M] + mass_H[i] + ".txt").c_str(),(string("datacard_") + Model[M] + mass_H[i] + ".out").c_str());
      sprintf(Buffer,"../../../../UserCode/mschen/LandS/test/lands.exe -d %s -M Hybrid --freq --ExpectationHints Asymptotic --scanRs 1 --freq --nToysForCLsb 5000 --nToysForCLb 2500 --seed 1234 -n %s > %s &",(string("datacard_") + Model[M] + mass_H[i] + ".txt").c_str(),(string("Higgs") +  Model[M] + mass_H[i]).c_str(),(string("datacard_") + Model[M] + mass_H[i] + ".out").c_str());
   printf("%s\n",Buffer);
   system(Buffer);
   }}
   return;



   TGraph** TGObsLimit   = new TGraph*[NModel];
   TGraph** TGExpLimit   = new TGraph*[NModel];
   TCutG**  TGExpLimit1S = new TCutG* [NModel];
   TCutG**  TGExpLimit2S = new TCutG* [NModel];

   FILE* pFileSum = fopen("LimitSummary","w");
   for(unsigned int M=0;M<sizeof(Model)/sizeof(string);M++){
      for(int i=0;i<5;i++){
//       Limit toReturn = GetLimitFromFile((string("datacard_") + Model[M] + mass_H[i] + ".out"));
         Limit toReturn = GetLimit(string("Higgs") + Model[M] + mass_H[i] + "_m2lnQ.root",string("plot_Higgs")+mass_H[i]);
         fprintf(pFileSum, "Model=%s Mass=%s --> Limits are %lf %lf %lf %lf %lf %lf %lf\n", Model[M].c_str(),mass_H[i].c_str(), toReturn.Obs,toReturn.Exp_minus2s, toReturn.Exp_minus1s, toReturn.Exp_Mean, toReturn.Exp_plus1s, toReturn.Exp_plus2s, toReturn.Exp_Median);
         ObsLimit[i]   = toReturn.Obs;
         ExpLimitm2[i] = toReturn.Exp_minus2s;
         ExpLimitm1[i] = toReturn.Exp_minus1s;
         ExpLimit[i]   = toReturn.Exp_Mean;
         ExpLimitp1[i] = toReturn.Exp_plus1s;
         ExpLimitp2[i] = toReturn.Exp_plus2s;
         MassAxis[i]   = 200+i*100;
         printf("%f %f\n",MassAxis[i],ObsLimit[i]);
      }

      TGObsLimit  [M] = new TGraph(5,MassAxis,ObsLimit);  TGObsLimit[M]->SetLineWidth(2);
      TGExpLimit  [M] = new TGraph(5,MassAxis,ExpLimit);  TGExpLimit[M]->SetLineWidth(2); TGExpLimit[M]->SetLineStyle(2);
      TGExpLimit1S[M] = GetErrorBand((Model[M] + "1S").c_str(), 5, MassAxis, ExpLimitm1, ExpLimitp1);  
      TGExpLimit2S[M] = GetErrorBand((Model[M] + "2S").c_str(), 5, MassAxis, ExpLimitm2, ExpLimitp2);  TGExpLimit2S[M]->SetFillColor(5);
   }
   fclose(pFileSum);


   TGExpLimit[1]->SetLineColor(2);  TGExpLimit[1]->SetLineStyle(2);
   TGExpLimit[2]->SetLineColor(4);  TGExpLimit[2]->SetLineStyle(2);

   TCanvas* c1 = new TCanvas("c1", "c1",600,600);
   TMultiGraph* MG = new TMultiGraph();
   MG->Add(TGObsLimit[0]    ,"C");
   MG->Add(TGExpLimit[0]    ,"C");
   MG->Add(TGExpLimit[2]    ,"C");
   MG->Add(TGExpLimit[1]    ,"C");
   MG->Draw("AXIS");
   MG->SetTitle("");
   MG->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   MG->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
   MG->GetYaxis()->SetTitleOffset(1.70);
//   MG->GetYaxis()->SetRangeUser(0.1,30.0);
   MG->GetYaxis()->SetRangeUser(0.5,50.0);
   TGExpLimit2S[0]->Draw("f");
   TGExpLimit1S[0]->Draw("f");
   MG->Draw("same");

   TLegend* LEG = new TLegend(0.15,0.70,0.50,0.90);
   LEG->SetHeader("");
   LEG->SetFillColor(0);
   LEG->SetBorderSize(0);
   LEG->AddEntry(TGObsLimit[0]  , "Observed"     ,"L");
   LEG->AddEntry(TGExpLimit[0]  , "Expected"     ,"L");
   LEG->AddEntry(TGExpLimit1S[0], "Expected #pm 1#sigma"     ,"F");
   LEG->AddEntry(TGExpLimit2S[0], "Expected #pm 2#sigma"     ,"F");
   LEG->AddEntry(TGExpLimit[2]  , "Expected (Jet bins)"      ,"L");
   LEG->AddEntry(TGExpLimit[1]  , "Expected (VBF bin)"       ,"L");
   LEG->Draw();
   c1->SetGridx(true);
   c1->SetGridy(true);
   c1->SetLogy(true);
   c1->SaveAs("Limit.png");
   c1->SaveAs("Limit.C");
   delete c1;
}









