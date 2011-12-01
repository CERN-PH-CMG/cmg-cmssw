
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
      if(cutIndex>=0){
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
        toReturn.Obs = 1000.0;
        toReturn.Exp_minus2s = 1000.0;
        toReturn.Exp_minus1s = 1000.0;
        toReturn.Exp_Mean    = 1000.0;
        toReturn.Exp_Median  = 1000.0;
        toReturn.Exp_plus1s = 1000.0;
        toReturn.Exp_plus2s = 1000.0;

        FILE* pFile = fopen(inFileName.Data(),"r");
        if(!pFile){printf("######  FILE %s DOES NOT EXIST\n",inFileName.Data());return toReturn;}
        fclose(pFile);

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


double getUnc(double centralYield, double Syst1, double Syst2=-1){
   double Unc = 1;
   if(centralYield<=0)return Unc;

   if(Syst2!=-1){
      Unc += 0.5 * (fabs(Syst1 - centralYield)/centralYield  + fabs(Syst2 - centralYield)/centralYield);
   }else{
      Unc += fabs(Syst1 - centralYield)/centralYield;
   }
   if(isnan(Unc))Unc=1.0;
   if(Unc>2)Unc=2.0;
   return Unc;
}


double getUnc(TFile* File, string subChannelsDir, int BinA, int BinB, string centralYield, string Syst1="",  string Syst2=""){
   stYields central = Efficiency(File,subChannelsDir, centralYield, BinA);

   
   double StatErr = getUnc(central.Rate, central.Rate+central.Err);
   if(Syst1==""){
      return StatErr;
   }

   int Bin = BinA;
//   if(StatErr>1.05){
//      Bin = BinB;
//      central = Efficiency(File,subChannelsDir, centralYield, Bin);
//   }

   stYields systA   = Efficiency(File,subChannelsDir, Syst1, Bin);

   if(Syst2==""){
      return getUnc(central.Rate, systA.Rate);
   }


   stYields systB   = Efficiency(File,subChannelsDir, Syst2, Bin);
   return getUnc(central.Rate, systA.Rate, systB.Rate);
}




void computeLimit(){
   setTDRStyle();
   gStyle->SetPadTopMargin   (0.06);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.06);
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

   TFile* File = new TFile("../plotter.root");
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
/*
   printf("Samples & mumu (0Jets) & mumu (1Jets) & mumu (2Jets) & mumu (vbf) & ee (0Jets) & ee (1Jets) & ee (2Jets) & ee (vbf) \\\\\n");
   for(unsigned int s=0;s<Sample.size();s++){
      stYields tmp;
      printf("%-10s &"    ,Legend[s].c_str());

//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eeeq0jets_finaleventflow"   ,s+1),EfficiencyErr(File,Sample[s],"/eeeq0jets_finaleventflow"   ,s+1));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eeeq1jets_finaleventflow"   ,s+1),EfficiencyErr(File,Sample[s],"/eeeq1jets_finaleventflow"   ,s+1));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/eegeq2jets_finaleventflow"  ,s+1),EfficiencyErr(File,Sample[s],"/eegeq2jets_finaleventflow"  ,s+1));
      tmp = Efficiency(File,Sample[s],"/eevbf_finaleventflow"       ,s+1); printf(" %6.2E+-%6.2E &",tmp.Rate,tmp.Err);
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumueq0jets_finaleventflow" ,s+1),EfficiencyErr(File,Sample[s],"/mumueq0jets_finaleventflow" ,s+1));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumueq1jets_finaleventflow" ,s+1),EfficiencyErr(File,Sample[s],"/mumueq1jets_finaleventflow" ,s+1));
//      printf(" %6.2E+-%6.2E &",Efficiency(File,Sample[s],"/mumugeq2jets_finaleventflow",s+1),EfficiencyErr(File,Sample[s],"/mumugeq2jets_finaleventflow",s+1));
      tmp = Efficiency(File,Sample[s],"/mumuvbf_finaleventflow"     ,s+1); printf(" %6.2E+-%6.2E &",tmp.Rate,tmp.Err);
      printf("\\\\\n");
   }


   printf("Samples        : ");for(unsigned int s=0;s<Sample.size();s++){printf(" & %-18s"    ,Legend[s].c_str());}printf("\\\n");
   printf("mumu 0Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumueq0jets_finaleventflow" ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 1Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumueq1jets_finaleventflow" ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 2Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumugeq2jets_finaleventflow",1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu VBF   bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/mumuvbf_finaleventflow"     ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   0Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eeeq0jets_finaleventflow"   ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   1Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eeeq1jets_finaleventflow"   ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   2Jets bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eegeq2jets_finaleventflow"  ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   VBF   bin : ");for(unsigned int s=0;s<Sample.size();s++){stYields tmp = Efficiency(File,Sample[s],"/eevbf_finaleventflow"       ,1); printf(" & %6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");

   printf("Samples        : ");for(unsigned int s=0;s<5;s++){printf(" & %-18s"        ,Legend[s].c_str());}printf("\\\n");
   printf("mumu 0Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumueq0jets_finaleventflow" ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 1Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumueq1jets_finaleventflow" ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu 2Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumugeq2jets_finaleventflow",1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("mumu VBF   bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/mumuvbf_finaleventflow"     ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   0Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eeeq0jets_finaleventflow"   ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   1Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eeeq1jets_finaleventflow"   ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   2Jets bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eegeq2jets_finaleventflow"  ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
   printf("ee   VBF   bin : ");for(unsigned int s=0;s<5;s++){stYields tmp = SoverN(File,Sample[s], Sample[6] ,"/eevbf_finaleventflow"       ,1); printf(" & S/B=%6.2E+-%6.2E",tmp.Rate, tmp.Err);}printf("\\\n");
*/

   std::string data = "data";
   std::string VV  = "ZZ;WW;WZ";
   std::string ZZ  = "ZZ";
   std::string WW  = "WW";
   std::string WZ  = "WZ";
   std::string TT = "t#bar{t}";
   std::string ST = "Single top";
   std::string Top = "t#bar{t};Single top";
   std::string DY  = "Z-#gamma^{*}+jets#rightarrow ll";
   std::string WJ  = "W+jets";
   std::string H[8];  std::string mass_H[8];
   double HErr[8];  double HMass[8];
   H[0] = "H(130)#rightarrow VV";   mass_H[0] = "130";  HErr[0] = 1.077;  HMass[0] = 130;
   H[1] = "H(150)#rightarrow VV";   mass_H[1] = "150";  HErr[1] = 1.077;  HMass[1] = 150;
   H[2] = "H(170)#rightarrow VV";   mass_H[2] = "170";  HErr[2] = 1.077;  HMass[2] = 170;
   H[3] = "H(200)#rightarrow VV";   mass_H[3] = "200";  HErr[3] = 1.077;  HMass[3] = 200;
   H[4] = "H(300)#rightarrow VV";   mass_H[4] = "300";  HErr[4] = 1.077;  HMass[4] = 300;
   H[5] = "H(400)#rightarrow VV";   mass_H[5] = "400";  HErr[5] = 1.080;  HMass[5] = 400;
   H[6] = "H(500)#rightarrow VV";   mass_H[6] = "500";  HErr[6] = 1.086;  HMass[6] = 500;
   H[7] = "H(600)#rightarrow VV";   mass_H[7] = "600";  HErr[7] = 1.093;  HMass[7] = 600;
   double ObsLimit[8];  
   double ExpLimitm2[8];
   double ExpLimitm1[8];
   double ExpLimit[8];
   double ExpLimitp1[8];
   double ExpLimitp2[8];
   double MassAxis[8];

   string Model[]={"metBin", "metvbf", "metInc", "rmetBin", "rmetvbf", "rmetInc"};
//   string Model[]={"metBin", "metvbf", "metInc"};//, "rmetBin", "rmetvbf", "rmetInc"};
//   string Model[]={"rmetBin", "rmetvbf", "rmetInc"};
   int NModel    = sizeof(Model)/sizeof(string);
/*
   for(int M=0;M<NModel;M++){
   int NMass = 8; int Offset=0;
   for(int i=0;i<NMass;i++){
      if(i<3)continue;
      string* Channels; string* HistoName; int* BinForYields;   int* BinForSyst;
      int NChannels; 
      if(Model[M]=="metBin"){
         NChannels= 4;
         Channels = new string[NChannels];   	HistoName=new string[NChannels];       BinForYields = new int[NChannels];	BinForSyst = new int[NChannels];
         Channels[0]="mumunovbf";    		HistoName[0]="finaleventflowmet";      BinForYields[0] = i+1;			BinForSyst[0] = 9;
         Channels[1]="mumuvbf";        		HistoName[1]="finaleventflowvbf";      BinForYields[1] = 4;			BinForSyst[1] = 9;
         Channels[2]="eenovbf";    		HistoName[2]="finaleventflowmet";      BinForYields[2] = i+1;			BinForSyst[2] = 9;
         Channels[3]="eevbf";    		HistoName[3]="finaleventflowvbf";      BinForYields[3] = 4;			BinForSyst[3] = 9;
      }else if(Model[M]=="metvbf"){
         NChannels= 2;
         Channels = new string[NChannels];      HistoName=new string[NChannels];       BinForYields = new int[NChannels];	BinForSyst = new int[NChannels];
         Channels[0]="mumuvbf";                 HistoName[0]="finaleventflowvbf";      BinForYields[0] = 4;			BinForSyst[0] = 9;
         Channels[1]="eevbf";                   HistoName[1]="finaleventflowvbf";      BinForYields[1] = 4;			BinForSyst[1] = 9;
      }else if(Model[M]=="metInc"){
         NChannels= 2;
         Channels = new string[NChannels];      HistoName=new string[NChannels];       BinForYields = new int[NChannels];	BinForSyst = new int[NChannels];
         Channels[0]="mumu";                    HistoName[0]="finaleventflowmet";      BinForYields[0] = i+1;			BinForSyst[0] = 9;
         Channels[1]="ee";                      HistoName[1]="finaleventflowmet";      BinForYields[1] = i+1;			BinForSyst[1] = 9;
      }else if(Model[M]=="rmetBin"){
         NChannels= 4;
         Channels = new string[NChannels];      HistoName=new string[NChannels];       BinForYields = new int[NChannels];	BinForSyst = new int[NChannels];
         Channels[0]="mumunovbf";               HistoName[0]="finaleventflowrmet";     BinForYields[0] = i+1;			BinForSyst[0] = 9;
         Channels[1]="mumuvbf";                 HistoName[1]="finaleventflowvbf";      BinForYields[1] = 8;			BinForSyst[1] = 9;
         Channels[2]="eenovbf";                 HistoName[2]="finaleventflowrmet";     BinForYields[2] = i+1;			BinForSyst[2] = 9;
         Channels[3]="eevbf";                   HistoName[3]="finaleventflowvbf";      BinForYields[3] = 8;			BinForSyst[3] = 9;
      }else if(Model[M]=="rmetvbf"){
         NChannels= 2;
         Channels = new string[NChannels];      HistoName=new string[NChannels];       BinForYields = new int[NChannels];	BinForSyst = new int[NChannels];
         Channels[0]="mumuvbf";                 HistoName[0]="finaleventflowvbf";      BinForYields[0] = 8;			BinForSyst[0] = 9;
         Channels[1]="eevbf";                   HistoName[1]="finaleventflowvbf";      BinForYields[1] = 8;			BinForSyst[1] = 9;
      }else if(Model[M]=="rmetInc"){
         NChannels= 2;
         Channels = new string[NChannels];      HistoName=new string[NChannels];       BinForYields = new int[NChannels];	BinForSyst = new int[NChannels];
         Channels[0]="mumu";                    HistoName[0]="finaleventflowrmet";     BinForYields[0] = i+1;			BinForSyst[0] = 9;
         Channels[1]="ee";                      HistoName[1]="finaleventflowrmet";     BinForYields[1] = i+1;			BinForSyst[1] = 9;
      }else{
         printf("Model %s is unknown\n",Model[M].c_str());exit(0);
      }
      string subChannelsDir[] = {H[Offset+i],DY,WJ, VV, Top};
      double subChannelsThErr[] = {HErr[Offset+i],1.04345, 1.04976, 1.0+sqrt(pow(0.0254,2)+ pow(0.0349,2) + pow(0.0385,2)), 1.0+sqrt(pow(0.0606,2) + pow(0.0334,2))};

      string subChannels[] = {"higgs","DY", "WJ", "VV", "Top"};
      int NsubChannels = sizeof(subChannels)/sizeof(string);
   
      FILE* pFile = fopen((string("datacard_") + Model[M] + mass_H[Offset+i] + ".txt").c_str(),"w");
      fprintf(pFile,"****datacard ******\n");
      fprintf(pFile,"\n");
      fprintf(pFile,"# Counting experiment with multiple channels for testing\n");
      fprintf(pFile,"imax %i  number of channels\n",NChannels);
      fprintf(pFile,"jmax *  number of backgrounds ('*' = automatic)\n");
      fprintf(pFile,"kmax *  number of nuisance parameters (sources of systematical uncertainties)\n");
      fprintf(pFile,"------------\n");
      fprintf(pFile,"# two channels, each with it's number of observed events\n");
      fprintf(pFile,"bin         ");for(int c=0;c<NChannels;c++){fprintf(pFile,"%7s   ",Channels[c].c_str());}fprintf(pFile,"\n");
      fprintf(pFile,"observation ");for(int c=0;c<NChannels;c++){fprintf(pFile,"%7f   ",Efficiency(File,data   ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]).Rate);}fprintf(pFile,"\n");
      fprintf(pFile,"------------\n");
      fprintf(pFile,"# now we list the expected events for signal and all backgrounds in\n");
      fprintf(pFile,"# those three bins\n");
      fprintf(pFile,"# the second 'process' line must have a positive number for\n");
      fprintf(pFile,"# backgrounds, and 0 for signal\n");
      fprintf(pFile,"# for the signal, we normalize the yields to an hypothetical cross\n");
      fprintf(pFile,"# section of 1/pb\n");
      fprintf(pFile,"# so that we get an absolute limit in cross section in units of pb.\n");
      fprintf(pFile,"# then we list the independent sources of uncertainties, and give\n");
      fprintf(pFile,"# their effect (syst. error)\n");
      fprintf(pFile,"# on each process and bin\n");
      fprintf(pFile,"bin          ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7s   ",Channels[c].c_str())    ;}fprintf(pFile,"\n");
      fprintf(pFile,"process      ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7s   ",subChannels[sc].c_str());}fprintf(pFile,"\n");
      fprintf(pFile,"process      ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7i   ",sc)                     ;}fprintf(pFile,"\n");
      fprintf(pFile,"rate         ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++)fprintf(pFile,"%7f   ",Efficiency(File,subChannelsDir[sc] ,string("/") + Channels[c] + "_" + HistoName[c] ,BinForYields[c]).Rate);}fprintf(pFile,"\n");

      fprintf(pFile,"------------\n");
      char pFileB[4096];  double Unc; bool isUncert=false;
      isUncert=false; sprintf(pFileB,"lumi     lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){Unc=1.045; isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);

      isUncert=false; sprintf(pFileB,"thxsec   lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){Unc=subChannelsThErr[sc]; isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);

      isUncert=false; sprintf(pFileB,"jes      lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
         Unc=getUnc(File, subChannelsDir[sc], BinForYields[c], BinForSyst[c], (string("/")+Channels[c]+"_"+HistoName[c]).c_str(), (string("/")+Channels[c]+"_"+"jesup"+HistoName[c]).c_str() , (string("/")+Channels[c]+"_"+HistoName[c], string("/")+Channels[c]+"_"+"jesdown"+HistoName[c]).c_str() );
         isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);

      isUncert=false; sprintf(pFileB,"jer      lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
         Unc=getUnc(File, subChannelsDir[sc], BinForYields[c], BinForSyst[c], (string("/")+Channels[c]+"_"+HistoName[c]).c_str(), (string("/")+Channels[c]+"_"+"jer"+HistoName[c]).c_str() , "" ); 
         Unc = 1.0+((Unc-1.0)*0.5);
         isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);

      isUncert=false; sprintf(pFileB,"factscal lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
         Unc=getUnc(File, subChannelsDir[sc], BinForYields[c], BinForSyst[c], (string("/")+Channels[c]+"_"+HistoName[c]).c_str(), (string("/")+Channels[c]+"_"+"hfactup"+HistoName[c]).c_str() , (string("/")+Channels[c]+"_"+HistoName[c], string("/")+Channels[c]+"_"+"hfactdown"+HistoName[c]).c_str() ); 
         isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);

      isUncert=false; sprintf(pFileB,"renscale lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
         Unc=getUnc(File, subChannelsDir[sc], BinForYields[c], BinForSyst[c], (string("/")+Channels[c]+"_"+HistoName[c]).c_str(), (string("/")+Channels[c]+"_"+"hrenup"+HistoName[c]).c_str() , (string("/")+Channels[c]+"_"+HistoName[c], string("/")+Channels[c]+"_"+"hrendown"+HistoName[c]).c_str() );
         isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);

      isUncert=false; sprintf(pFileB,"pileup   lnN ");for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
         Unc=getUnc(File, subChannelsDir[sc], BinForYields[c], BinForSyst[c], (string("/")+Channels[c]+"_"+HistoName[c]).c_str(), (string("/")+Channels[c]+"_"+"puup"+HistoName[c]).c_str() , (string("/")+Channels[c]+"_"+HistoName[c], string("/")+Channels[c]+"_"+"pudown"+HistoName[c]).c_str() );
         if(Unc>1.10)Unc=1.10;
         isUncert|=(Unc!=1.0); sprintf(pFileB,"%s%7f   ",pFileB,Unc);}}if(isUncert)fprintf(pFile,"%s\n",pFileB);


      for(int u=0;u<NsubChannels;u++){
         isUncert=false; sprintf(pFileB,"MCStat%02i lnN ",u);
         for(int c=0;c<NChannels;c++){for(int sc=0;sc<NsubChannels;sc++){
            if(sc==u){Unc=getUnc(File, subChannelsDir[sc], BinForYields[c], -1, (string("/")+Channels[c]+"_"+HistoName[c]).c_str());isUncert|=(Unc!=1.0);  sprintf(pFileB,"%s%7f   ",pFileB,Unc);
            }else{    Unc=1.0; isUncert|=(Unc!=1.0);  sprintf(pFileB,"%s%7f   ",pFileB,Unc);
            }      
         }}if(isUncert)fprintf(pFile,"%s\n",pFileB);
      }

      fclose(pFile);


      char Buffer[1024];
      printf("Running LandS on Higgs %s:",mass_H[Offset+i].c_str());
//     sprintf(Buffer,"../../../../UserCode/mschen/LandS/test/lands.exe -M Bayesian -d %s --doExpectation 1 -t 100000 > %s &",(string("datacard_") + Model[M] + mass_H[Offset+i] + ".txt").c_str(),(string("datacard_") + Model[M] + mass_H[Offset+i] + ".out").c_str() );
      sprintf(Buffer,"../../../../UserCode/mschen/LandS/test/lands.exe -d %s -M Hybrid --freq --ExpectationHints Asymptotic --scanRs 1 --freq --nToysForCLsb 20000 --nToysForCLb 10000 --seed 1234 -rMax 15 -rMin 0.1 -n %s > /tmp/%s &",(string("datacard_") + Model[M] + mass_H[Offset+i] + ".txt").c_str(),(string("Higgs") +  Model[M] + mass_H[Offset+i]).c_str(),(string("datacard_") + Model[M] + mass_H[Offset+i] + ".out").c_str());
   printf("%s\n",Buffer);
   system(Buffer);
   }}
   return;
*/


   TGraph** TGObsLimit   = new TGraph*[NModel];
   TGraph** TGExpLimit   = new TGraph*[NModel];
   TCutG**  TGExpLimit1S = new TCutG* [NModel];
   TCutG**  TGExpLimit2S = new TCutG* [NModel];

   FILE* pFileSum = fopen("LimitSummary","w");
   for(int M=0;M<NModel;M++){
      int NMass = 5; int Offset=3;

      for(int i=0;i<NMass;i++){
//       Limit toReturn = GetLimitFromFile((string("datacard_") + Model[M] + mass_H[Offset+i] + ".out"));
         Limit toReturn = GetLimit(string("Higgs") + Model[M] + mass_H[Offset+i] + "_m2lnQ.root",string("plot_Higgs")+mass_H[Offset+i]);
         fprintf(pFileSum, "Model=%s Mass=%s --> Limits are %lf %lf %lf %lf %lf %lf %lf\n", Model[M].c_str(),mass_H[Offset+i].c_str(), toReturn.Obs,toReturn.Exp_minus2s, toReturn.Exp_minus1s, toReturn.Exp_Mean, toReturn.Exp_plus1s, toReturn.Exp_plus2s, toReturn.Exp_Median);
         ObsLimit[i]   = toReturn.Obs;
         ExpLimitm2[i] = toReturn.Exp_minus2s;
         ExpLimitm1[i] = toReturn.Exp_minus1s;
         ExpLimit[i]   = toReturn.Exp_Mean;
         ExpLimitp1[i] = toReturn.Exp_plus1s;
         ExpLimitp2[i] = toReturn.Exp_plus2s;
         MassAxis[i]   = HMass[Offset+i];
         fprintf(pFileSum,"%f %f %f+-%f\n",MassAxis[i],ObsLimit[i],ExpLimit[i],(ExpLimitp1[i]-ExpLimitm1[i])/2.0);
      }

      TGObsLimit  [M] = new TGraph(NMass,MassAxis,ObsLimit);  TGObsLimit[M]->SetLineWidth(2);
      TGExpLimit  [M] = new TGraph(NMass,MassAxis,ExpLimit);  TGExpLimit[M]->SetLineWidth(2); TGExpLimit[M]->SetLineStyle(2);
      TGExpLimit1S[M] = GetErrorBand((Model[M] + "1S").c_str(), NMass, MassAxis, ExpLimitm1, ExpLimitp1);  
      TGExpLimit2S[M] = GetErrorBand((Model[M] + "2S").c_str(), NMass, MassAxis, ExpLimitm2, ExpLimitp2);  TGExpLimit2S[M]->SetFillColor(5);

      printf("Obs Limits for Model are: ");for(int i=0;i<NMass;i++){printf("%f ",ObsLimit[i]);}printf("\n");
      printf("Exp Limits for Model are: ");for(int i=0;i<NMass;i++){printf("%f+-%f ",ExpLimit[i], (ExpLimitp1[i]-ExpLimitm1[i]))/2.0;}printf("\n");
   }
   fclose(pFileSum);

   TGExpLimit[0]->SetLineColor(13); TGExpLimit[0]->SetLineStyle(1);
   TGExpLimit[1]->SetLineColor(2);  TGExpLimit[1]->SetLineStyle(1);
   TGExpLimit[2]->SetLineColor(4);  TGExpLimit[2]->SetLineStyle(1);
   TGObsLimit[0]->SetLineWidth(2);  TGObsLimit[0]->SetMarkerStyle(20);
   TGObsLimit[2]->SetLineWidth(2);  TGObsLimit[2]->SetMarkerStyle(22);

   TCanvas* c1 = new TCanvas("c1", "c1",600,600);
   TMultiGraph* MG = new TMultiGraph();
   MG->Add(TGObsLimit[0]       ,"CP");
   MG->Add(TGObsLimit[2]       ,"CP");
   MG->Add(TGExpLimit[0]       ,"C");
   MG->Add(TGExpLimit[1]       ,"C");
   MG->Add(TGExpLimit[2]       ,"C");
   MG->Draw("AXIS");
   MG->SetTitle("");
   MG->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   MG->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
   MG->GetYaxis()->SetTitleOffset(1.70);
//   MG->GetYaxis()->SetRangeUser(0.1,30.0);
   MG->GetYaxis()->SetRangeUser(0.1,40.0);
   TGExpLimit2S[0]->Draw("f");
   TGExpLimit1S[0]->Draw("f");
   MG->Draw("same");

   char LumiLabel[1024];
   sprintf(LumiLabel,"CMS preliminary,#sqrt{s}=7 TeV, #int L=%6.1ffb^{-1}",4.6);
   TPaveText *pave = new TPaveText(0.5,0.96,0.94,0.99,"NDC");
   pave->SetBorderSize(0);
   pave->SetFillStyle(0);
   pave->SetTextAlign(32);
   pave->SetTextFont(42);
   TObjArray* tokens = (TString(LumiLabel)).Tokenize("\\\\");
   int nt = tokens->GetEntries();
   for(int it=0; it<nt; ++it){
      TObjString * t = (TObjString *)tokens->At(it);
      pave->AddText(t->GetString());
   }
   pave->Draw("same");

   TLegend* LEG = new TLegend(0.25,0.70,0.60,0.90);
   LEG->SetHeader("");
   LEG->SetFillColor(0);
   LEG->SetBorderSize(0);
   LEG->AddEntry(TGObsLimit[0]  , "Observed (Binned)"  ,"LP");
   LEG->AddEntry(TGObsLimit[2]  , "Observed (Incl)"    ,"LP");
   LEG->AddEntry(TGExpLimit[0]  , "Expected (Binned)"  ,"L");
   LEG->AddEntry(TGExpLimit[1]  , "Expected (VBF)"         ,"L");
   LEG->AddEntry(TGExpLimit[2]  , "Expected (Incl)"        ,"L");
   LEG->Draw();
   c1->SetGridx(true);
   c1->SetGridy(true);
   c1->SetLogy(true);
   c1->SaveAs("Limit.png");
   c1->SaveAs("Limit.C");
   delete c1;




   TGExpLimit[3]->SetLineColor(13); TGExpLimit[3]->SetLineStyle(1);
   TGExpLimit[4]->SetLineColor(2);  TGExpLimit[4]->SetLineStyle(1);
   TGExpLimit[5]->SetLineColor(4);  TGExpLimit[5]->SetLineStyle(1);
   TGObsLimit[3]->SetLineWidth(2);  TGObsLimit[3]->SetMarkerStyle(20);
   TGObsLimit[5]->SetLineWidth(2);  TGObsLimit[5]->SetMarkerStyle(22);

   c1 = new TCanvas("c1", "c1",600,600);
   MG = new TMultiGraph();
   MG->Add(TGObsLimit[3]       ,"CP");
   MG->Add(TGObsLimit[5]       ,"CP");
   MG->Add(TGExpLimit[3]       ,"C");
   MG->Add(TGExpLimit[4]       ,"C");
   MG->Add(TGExpLimit[5]       ,"C");
   MG->Draw("AXIS");
   MG->SetTitle("");
   MG->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   MG->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
   MG->GetYaxis()->SetTitleOffset(1.70);
//   MG->GetYaxis()->SetRangeUser(0.1,30.0);
   MG->GetYaxis()->SetRangeUser(0.1,40.0);
   TGExpLimit2S[3]->Draw("f");
   TGExpLimit1S[3]->Draw("f");
   MG->Draw("same");

   pave = new TPaveText(0.5,0.96,0.94,0.99,"NDC");
   pave->SetBorderSize(0);
   pave->SetFillStyle(0);
   pave->SetTextAlign(32);
   pave->SetTextFont(42);
   tokens = (TString(LumiLabel)).Tokenize("\\\\");
   nt = tokens->GetEntries();
   for(int it=0; it<nt; ++it){
      TObjString * t = (TObjString *)tokens->At(it);
      pave->AddText(t->GetString());
   }
   pave->Draw("same");

   LEG = new TLegend(0.25,0.70,0.60,0.90);
   LEG->SetHeader("");
   LEG->SetFillColor(0);
   LEG->SetBorderSize(0);
   LEG->AddEntry(TGObsLimit[3]  , "Observed (Binned)"  ,"LP");
   LEG->AddEntry(TGObsLimit[5]  , "Observed (Incl)"    ,"LP");
   LEG->AddEntry(TGExpLimit[3]  , "Expected (Binned)"  ,"L");
   LEG->AddEntry(TGExpLimit[4]  , "Expected (VBF)"     ,"L");
   LEG->AddEntry(TGExpLimit[5]  , "Expected (Incl)"    ,"L");
   LEG->Draw();
   c1->SetGridx(true);
   c1->SetGridy(true);
   c1->SetLogy(true);
   c1->SaveAs("LimitRMet.png");
   c1->SaveAs("LimitRMet.C");
   delete c1;



}









