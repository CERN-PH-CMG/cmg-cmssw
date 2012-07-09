
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

//tree variables
double Tmh, Tlimit, TlimitErr; float TquantExp;

void fillLimitArray(TTree* tree, float Quantil, double* limitArray, double* massArray=NULL){
  int i=0;
  for(int ientry=0;ientry<tree->GetEntriesFast();ientry++){
     tree->GetEntry(ientry);
     if(TquantExp==Quantil){
        //printf("Quantil = %f - mH=%f --> %f\n",TquantExp,Tmh,Tlimit);
        limitArray[i]=Tlimit;
        if(massArray){massArray[i]=Tmh;} 
        i++;
     }
  }
}


TCutG* GetErrorBand(string name, int N, double* Mass, double* Low, double* High)
{
   TCutG* cutg = new TCutG(name.c_str(),2*N+2);
   cutg->SetFillColor(kGreen-7);
   cutg->SetLineStyle(0);
   cutg->SetLineColor(0);
   int I = 0;
   for(int i=0;i<N;i++){cutg->SetPoint(I,Mass[i]    , Low[i]     );I++; }
                        cutg->SetPoint(I,Mass[N-1]  , Low[N-1]   );I++;
                        cutg->SetPoint(I,Mass[N-1]  , High[N-1]  );I++;
   for(int i=0;i<N;i++){cutg->SetPoint(I,Mass[N-1-i], High[N-1-i]);I++;}
   return cutg;
}


void printLimits(TGraph* graph, double Mmin=200, double Mmax=600){
   double previous = graph->Eval(Mmin);
   printf("Exclude ");
   for(double M=Mmin;M<=Mmax;M+=1.0){
      double NEW = graph->Eval(M);
      if(previous>1 && NEW<1){printf("[%f,",M);}
      if(previous<1 && NEW>1){printf("%f]",M-1);}
      previous = NEW;
   }printf("\n");
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


   TFile* file = TFile::Open(inputs);
   printf("Looping on %s\n",inputs.Data());
   if(!file) return;
   if(file->IsZombie()) return;
   TTree* tree = (TTree*)file->Get("limit");

   tree->GetBranch("mh"              )->SetAddress(&Tmh      );
   tree->GetBranch("limit"           )->SetAddress(&Tlimit   );
   tree->GetBranch("limitErr"        )->SetAddress(&TlimitErr);
   tree->GetBranch("quantileExpected")->SetAddress(&TquantExp);

   int N = tree->GetEntriesFast() / 6 ;// 6Limits per mass point (observed, meand , +-1sigma, +-2sigma)
   double* MassAxis   = new double[N];
   double* ObsLimit   = new double[N];  fillLimitArray(tree,-1   ,ObsLimit,MassAxis);
   double* ExpLimitm2 = new double[N];  fillLimitArray(tree,0.025,ExpLimitm2);
   double* ExpLimitm1 = new double[N];  fillLimitArray(tree,0.160,ExpLimitm1);
   double* ExpLimit   = new double[N];  fillLimitArray(tree,0.500,ExpLimit  );
   double* ExpLimitp1 = new double[N];  fillLimitArray(tree,0.840,ExpLimitp1);
   double* ExpLimitp2 = new double[N];  fillLimitArray(tree,0.975,ExpLimitp2);
   file->Close();



   FILE* pFileSum = fopen((outputName+"_LimitSummary").Data(),"w");
   for(int i=0;i<N;i++){
      if(((int)MassAxis[i])%50==0)
      fprintf(pFileSum, "$%7.3f$ & $%6.2f$ & $[%6.2f,%6.2f]$ & $[%6.2f,%6.2f]$ & $%6.2f$ \\\\\\hline\n",MassAxis[i], ExpLimit[i], ExpLimitm1[i], ExpLimitp1[i], ExpLimitm2[i],  ExpLimitp2[i], ObsLimit[i]);
      if(int(MassAxis[i])%50!=0)continue; printf("%f ",ObsLimit[i]);
   }printf("\n");
   fclose(pFileSum);


   TGraph* TGObsLimit   = new TGraph(N,MassAxis,ObsLimit);  TGObsLimit->SetLineWidth(2);
   TGraph* TGExpLimit   = new TGraph(N,MassAxis,ExpLimit);  TGExpLimit->SetLineWidth(2); TGExpLimit->SetLineStyle(2);
   TCutG* TGExpLimit1S = GetErrorBand("1S", N, MassAxis, ExpLimitm1, ExpLimitp1);  
   TCutG* TGExpLimit2S = GetErrorBand("2S", N, MassAxis, ExpLimitm2, ExpLimitp2);  TGExpLimit2S->SetFillColor(5);


   printf("EXPECTED LIMIT --> ");   printLimits(TGExpLimit, MassAxis[0], MassAxis[N-1]);
   printf("OBSERVED LIMIT --> ");   printLimits(TGObsLimit, MassAxis[0], MassAxis[N-1]);



   printf("Obs Limits for Model are: ");for(int i=0;i<N;i++){if(int(MassAxis[i])%50!=0)continue; printf("%f ",ObsLimit[i]);}printf("\n");
   printf("Exp Limits for Model are: ");for(int i=0;i<N;i++){if(int(MassAxis[i])%50!=0)continue; printf("%f+-%f ",ExpLimit[i], (ExpLimitp1[i]-ExpLimitm1[i]))/2.0;}printf("\n");

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

   TGObsLimit->Draw("same CP");
   TGExpLimit->Draw("same C");

//   TMultiGraph* MG = new TMultiGraph();
////   MG->Add(TGObsLimit       ,"CP");
//   MG->Add(TGExpLimit       ,"C");
//   MG->SetTitle("");
//   MG->Draw("same");

   char LumiLabel[1024];
   if(energy<9){
//      sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f TeV, #int L=%6.1ffb^{-1}   -   %20s",energy, luminosity,legendName.Data());
      sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f TeV, #int L=%6.1ffb^{-1}",energy, luminosity);
   }else{
//      sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f/%.0f TeV, #int L=%6.1ffb^{-1}   -   %20s",7.0,8.0, luminosity,legendName.Data());
      sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f/%.0f TeV, #int L=%6.1ffb^{-1}",7.0,8.0, luminosity);
   }
   TPaveText *pave = new TPaveText(0.1,0.96,0.94,0.99,"NDC");
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

   TLine* SMLine = new TLine(framework->GetXaxis()->GetXmin(),1.0,framework->GetXaxis()->GetXmax(),1.0);
   SMLine->Draw("same");

   TLegend* LEG = new TLegend(0.35,0.70,0.65,0.93);
   LEG->SetHeader(legendName.Data());
   LEG->SetFillColor(0);
   LEG->SetBorderSize(0);
   LEG->AddEntry(TGExpLimit  , "median expected"  ,"L");
   LEG->AddEntry(TGExpLimit1S  , "expected #pm 1#sigma"  ,"F");
   LEG->AddEntry(TGExpLimit2S  , "expected #pm 2#sigma"  ,"F");
   LEG->AddEntry(TGObsLimit  , "observed"  ,"LP");
   LEG->Draw();
   //c1->SetGridx(true);
//   c1->SetGridy(true);
   c1->SetLogy(true);
   c1->SaveAs(outputName+"_Limit.png");
   c1->SaveAs(outputName+"_Limit.C");
   c1->SaveAs(outputName+"_Limit.pdf");
   delete c1;



}









