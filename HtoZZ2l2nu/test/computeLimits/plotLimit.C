
#include <string>
#include <vector>

#include "TROOT.h"
#include "TStyle.h"
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
#include "../../../../UserCode/mschen/LandS/test/fitRvsCLs.C"

struct Limit {double Obs; double Exp_minus2s; double Exp_minus1s; double Exp_Mean; double Exp_plus1s; double Exp_plus2s; double Exp_Median;};
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
	double med_err = limitErr;
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

printf("XXX ERRRO = %f\n",med_err);


        if(p1s<med)p1s=med;
        if(p2s<p1s)p2s=p1s;



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
   TCutG* cutg = new TCutG(name.c_str(),2*N+2);
   cutg->SetFillColor(kGreen-7);
   int I = 0;
   for(int i=0;i<N;i++){
      cutg->SetPoint(I,Mass[i], Low[i]);
      I++;
   }

   cutg->SetPoint(I,Mass[N-1], Low[N-1]);
   I++;

   cutg->SetPoint(I,Mass[N-1], High[N-1]);
   I++;

   for(int i=0;i<N;i++){
        cutg->SetPoint(I,Mass[N-1-i], High[N-1-i]);
        I++;
   }
   return cutg;
}



void plotLimit(TString outputName="Limit", TString inputs="", double energy=7, double luminosity=5.035, TString legendName="ee and #mu#mu channels"){
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetOptFile(0);
  //gStyle->SetOptStat(0);
  gStyle->SetOptStat("mr");
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.04);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.2);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.02);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);
  gStyle->SetEndErrorSize(2);
  gStyle->SetErrorX(0.);
  gStyle->SetMarkerStyle(20); 
  gROOT->ForceStyle();
//   setTDRStyle();
   gStyle->SetPadTopMargin   (0.06);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.06);
   gStyle->SetPadLeftMargin  (0.14);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);


   std::vector<string> files;
   FILE* pFile = fopen(inputs.Data(),"r");
   if(!pFile){printf("######  FILE %s DOES NOT EXIST\n",inputs.Data());exit(0);}
   while(1){
      char buff[2048];
      if(fscanf(pFile,"%s",buff)==EOF)break;
      files.push_back(buff);
      printf("%s\n", buff);
   }
   fclose(pFile);

   int N = files.size();

   double* ObsLimit   = new double[N];
   double* ExpLimitm2 = new double[N];
   double* ExpLimitm1 = new double[N];
   double* ExpLimit   = new double[N];
   double* ExpLimitp1 = new double[N];
   double* ExpLimitp2 = new double[N];
   double* MassAxis   = new double[N];
   double* MassAxisExp= new double[N];

   FILE* pFileSum = fopen((outputName+"_LimitSummary").Data(),"w");

   int NExp = 0;
   for(int i=0;i<N;i++){
      string massStr = files[i].substr(files[i].rfind('/')-3,3);
      int mass; sscanf(massStr.c_str(),"%d",&mass);
      Limit limits = GetLimit(files[i],string("Higgs")+massStr.c_str());

         //fprintf(pFileSum, "Mass=%s --> Limits are %lf %lf %lf %lf %lf %lf %lf\n",massStr.c_str(), limits.Obs,limits.Exp_minus2s, limits.Exp_minus1s, limits.Exp_Mean, limits.Exp_plus1s, limits.Exp_plus2s, limits.Exp_Median);
         fprintf(pFileSum, "$%s$ & $%6.2f$ & $[%6.2f,%6.2f]$ & $[%6.2f,%6.2f]$ & $%6.2f$ \\\\\\hline\n",massStr.c_str(), limits.Exp_Median, limits.Exp_minus1s, limits.Exp_plus1s, limits.Exp_minus2s,  limits.Exp_plus2s, limits.Obs );
         MassAxis[i]   = mass;
         ObsLimit[i]   = limits.Obs;

//         if(int(MassAxis[i])%10==0){
            MassAxisExp[NExp]= mass;
            ExpLimitm2[NExp] = limits.Exp_minus2s;
            ExpLimitm1[NExp] = limits.Exp_minus1s;
            ExpLimit[NExp]   = limits.Exp_Mean;
            ExpLimitp1[NExp] = limits.Exp_plus1s;
            ExpLimitp2[NExp] = limits.Exp_plus2s;
           // fprintf(pFileSum,"%f %f %f+-%f\n",MassAxis[i],ObsLimit[i],ExpLimit[NExp],(ExpLimitp1[NExp]-ExpLimitm1[NExp])/2.0);
            NExp++;
//         }
   }
   fclose(pFileSum);


   TGraph* TGObsLimit   = new TGraph(N,MassAxis,ObsLimit);  TGObsLimit->SetLineWidth(2);
   TGraph* TGExpLimit   = new TGraph(NExp,MassAxisExp,ExpLimit);  TGExpLimit->SetLineWidth(2); TGExpLimit->SetLineStyle(2);
   TCutG* TGExpLimit1S = GetErrorBand("1S", NExp, MassAxisExp, ExpLimitm1, ExpLimitp1);  
   TCutG* TGExpLimit2S = GetErrorBand("2S", NExp, MassAxisExp, ExpLimitm2, ExpLimitp2);  TGExpLimit2S->SetFillColor(5);


   printf("Obs Limits for Model are: ");for(int i=0;i<N;i++){if(int(MassAxis[i])%50!=0)continue; printf("%f ",ObsLimit[i]);}printf("\n");
   printf("Exp Limits for Model are: ");for(int i=0;i<NExp;i++){printf("%f+-%f ",ExpLimit[i], (ExpLimitp1[i]-ExpLimitm1[i]))/2.0;}printf("\n");

   TGExpLimit->SetLineColor(2);  TGExpLimit->SetLineStyle(1);
   TGObsLimit->SetLineWidth(2);  TGObsLimit->SetMarkerStyle(20);

   TCanvas* c1 = new TCanvas("c1", "c1",600,600);
   TH1F* framework = new TH1F("Graph","Graph",1,190,610);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
   framework->GetYaxis()->SetTitleOffset(1.70);
   framework->GetYaxis()->SetRangeUser(0.1,40);
   framework->Draw();

   TGExpLimit2S->Draw("fc same");
   TGExpLimit1S->Draw("fc same");

//   TGObsLimit->Draw("same CP");
   TGExpLimit->Draw("same C");

//   TMultiGraph* MG = new TMultiGraph();
////   MG->Add(TGObsLimit       ,"CP");
//   MG->Add(TGExpLimit       ,"C");
//   MG->SetTitle("");
//   MG->Draw("same");

   char LumiLabel[1024];
   sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f TeV, #int L=%6.1ffb^{-1}   -   %20s",energy, luminosity,legendName.Data());
   TPaveText *pave = new TPaveText(0.1,0.96,0.94,0.99,"NDC");
   pave->SetBorderSize(0);
   pave->SetFillStyle(0);
   pave->SetTextAlign(12);
   pave->SetTextFont(42);
   TObjArray* tokens = (TString(LumiLabel)).Tokenize("\\\\");
   int nt = tokens->GetEntries();
   for(int it=0; it<nt; ++it){
      TObjString * t = (TObjString *)tokens->At(it);
      pave->AddText(t->GetString());
   }
   pave->Draw("same");

   TLine* SMLine = new TLine(framework->GetXaxis()->GetXmin(),1.0,framework->GetXaxis()->GetXmax(),1.0);
   SMLine->Draw("same");

   TLegend* LEG = new TLegend(0.15,0.80,0.40,0.93);
   LEG->SetHeader("");
   LEG->SetFillColor(0);
   LEG->SetBorderSize(0);
   LEG->AddEntry(TGObsLimit  , "Observed"  ,"P");
   LEG->AddEntry(TGExpLimit  , "Expected"  ,"L");
//   LEG->Draw();
   //c1->SetGridx(true);
//   c1->SetGridy(true);
   c1->SetLogy(true);
   c1->SaveAs(outputName+"_Limit.png");
   c1->SaveAs(outputName+"_Limit.C");
   c1->SaveAs(outputName+"_Limit.pdf");
   delete c1;



}









