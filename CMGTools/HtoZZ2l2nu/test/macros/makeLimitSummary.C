
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
#include "TStyle.h"
//#include "tdrstyle.C"


TGraph* getGraph(string name, int color, int width, int style, TLegend* LEG, TGraph* Ref, string filePath){
//   filePath+="/LimitSummary";
   FILE* pFile = fopen(filePath.c_str(),"r");
   if(!pFile){printf("Can't open %s\n",filePath.c_str()); exit(0);}
   double mass, exp, unused; char buffer[1024];

   TGraph* graph = new TGraph(100);
   int N=0;
   while(fscanf(pFile,"$%lf$ & $%lf$ & $[%lf,%lf]$ & $[%lf,%lf]$ & $%lf$%s\n",&mass, &exp, &unused, &unused, &unused,&unused,&unused,buffer) != EOF){
//      printf("%i %f - %f\n",N,mass,exp);
      if(Ref){exp/=Ref->Eval(mass);}
      graph->SetPoint(N, mass, exp);N++;
      if(N>25)break;
   }
   graph->Set(N);

   graph->SetName(name.c_str());
   graph->SetLineColor(color);
   graph->SetLineWidth(width);
   graph->SetLineStyle(style);
   if(LEG)LEG->AddEntry(graph, name.c_str()      ,"L");
   return graph;
}


void makeLimitSummary(){
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  //   setTDRStyle();
   gStyle->SetPadTopMargin   (0.04);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.05);
   gStyle->SetPadLeftMargin  (0.12);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);

   char LumiLabel[1024];
   //   sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=7 TeV, #int L=%6.1ffb^{-1}",5.035);
   sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=8 TeV, #int L=%6.1ffb^{-1}",12.2);
   TPaveText *pave = new TPaveText(0.5,0.96,0.94,0.99,"NDC");
   pave->SetBorderSize(0);
   pave->SetFillStyle(0);
   pave->SetTextAlign(32);
   pave->SetTextFont(42);
   pave->AddText(LumiLabel);

   TCanvas* c1;
   TH1F* framework;
   TLegend* LEG, *LEG2;
   TGraph* Ref;

   c1 = new TCanvas("c", "c",600,600);
   //   c1->SetLogy(true);
   framework = new TH1F("Graph","Graph",1,150,1000);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
   framework->GetYaxis()->SetTitleOffset(1.40);
   framework->GetYaxis()->SetRangeUser(0,8);
   framework->Draw();
   pave->Draw("same");

   LEG = new TLegend(0.20,0.75,0.90,0.94);
   LEG->SetFillStyle(0);
   LEG->SetBorderSize(0);
   LEG->SetHeader("Expected @95% C.L.");

   // getGraph("Baseline=AK5PF+E_{T}^{miss}"                      , 1, 1, 1, LEG , NULL, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/Council/COUNT8_LimitSummary")->Draw("L same");
   //   getGraph("AK5PF+red-E_{T}^{miss}"                           , 2, 1, 1, LEG , NULL, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/Council/COUNT8RMET_LimitSummary")->Draw("L same");
   ///getGraph("AK5PF+E_{T}^{miss}+min #Delta#phi_{jet>30}"       , 4, 1, 1, LEG , NULL, "../computeLimits/JOBS_nodphisoftjmet/_LimitSummary")->Draw("L same");
   //getGraph("AK5PFchs+E_{T}^{miss}"                            , 1, 2, 1, LEG , NULL, "../computeLimits/JOBS_chs/_LimitSummary")->Draw("L same");
   // getGraph("AK5PFchs+red-E_{T}^{miss}+min #Delta#phi_{jet>30}", 2, 2, 1, LEG , NULL, "../computeLimits/JOBS_chsnodphisoftjmet_rmet/_LimitSummary")->Draw("L same");
//    getGraph("Cut&Count",1, 1, 1, LEG , NULL, "../computeLimits/JOBS_chsnodphisoftjmet/_LimitSummary")->Draw("L same");
//    getGraph("Shape(inc)",2, 2, 1, LEG , NULL, "../computeLimits/JOBS_chsnodphisoftjmet_incs/_LimitSummary")->Draw("L same");
//    getGraph("Shape(binned)"    ,4, 2, 1, LEG , NULL, "../computeLimits/JOBS_chsnodphisoftjmet/_LimitSummary_shape")->Draw("L same");

   getGraph("Cut&Count",1, 1, 1, LEG , NULL, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/computeLimits_Morriond_13_01_16/cards_CC8TeV/CC8TeV_LimitSummary")->Draw("L same");
   getGraph("Shape"    ,4, 2, 1, LEG , NULL, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/computeLimits_Morriond_13_01_16/cards_SB8TeV/SB8TeV_LimitSummary")->Draw("L same");

   
   LEG ->Draw("same");
   LEG->SetNColumns(2);

   system("mkdir -p LimitPlots");
   c1->SaveAs("LimitPlots/LimitSummary.png");
   c1->SaveAs("LimitPlots/LimitSummary.pdf");
   c1->SaveAs("LimitPlots/LimitSummary.C");
   //   delete c1;


   c1 = new TCanvas("c1", "c1",600,600);
   framework = new TH1F("Graph","Graph",1,150,1000);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("R / R_{Baseline}");
   framework->GetYaxis()->SetTitleOffset(1.40);
   framework->GetYaxis()->SetRangeUser(0.5,1.1);
   framework->Draw();
   pave->Draw("same");

   LEG = new TLegend(0.20,0.20,0.90,0.54);
   LEG->SetFillStyle(0);
   LEG->SetBorderSize(0);
   LEG->SetHeader(NULL);
   
   TLine *l=new TLine(150,1,1000,1); l->Draw("same");

   //   Ref=getGraph("Baseline=AK5PF+E_{T}^{miss}"                      , 1, 1, 1, LEG , NULL, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/Council/COUNT8_LimitSummary");
   //   getGraph("AK5PF+red-E_{T}^{miss}"                           , 2, 1, 1, LEG , Ref, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/Council/COUNT8RMET_LimitSummary")->Draw("L same");
   // getGraph("AK5PF+E_{T}^{miss}+min #Delta#phi_{jet>30}"       , 4, 1, 1, LEG , Ref, "../computeLimits/JOBS_nodphisoftjmet/_LimitSummary")->Draw("L same");
   // getGraph("AK5PFchs+E_{T}^{miss}"                            , 1, 2, 1, LEG , Ref, "../computeLimits/JOBS_chs/_LimitSummary")->Draw("L same");
   // getGraph("AK5PFchs+red-E_{T}^{miss}+min #Delta#phi_{jet>30}", 2, 2, 1, LEG , Ref, "../computeLimits/JOBS_chsnodphisoftjmet_rmet/_LimitSummary")->Draw("L same");
//    Ref=getGraph("Cut&Count", 1, 1, 1, LEG , NULL, "../computeLimits/JOBS_chsnodphisoftjmet/_LimitSummary");
//    getGraph("Shape(inc)",         2, 2, 1, LEG , Ref, "../computeLimits/JOBS_chsnodphisoftjmet_incs/_LimitSummary")->Draw("L same");
//    getGraph("Shape(binned)",         4, 2, 1, LEG , Ref, "../computeLimits/JOBS_chsnodphisoftjmet/_LimitSummary_shape")->Draw("L same");

   Ref=getGraph("Cut&Count",1, 1, 1, LEG , NULL, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/computeLimits_Morriond_13_01_16/cards_CC8TeV/CC8TeV_LimitSummary");
   getGraph("Shape"    ,4, 2, 1, LEG , Ref, "/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_5_3_3_patch3/src/CMGTools/HtoZZ2l2nu/test/computeLimits_Morriond_13_01_16/cards_SB8TeV/SB8TeV_LimitSummary")->Draw("L same");

   LEG ->Draw("same");
   LEG->SetNColumns(2);

   c1->SaveAs("LimitPlots/LimitRatio.png");
   c1->SaveAs("LimitPlots/LimitRatio.pdf");
   c1->SaveAs("LimitPlots/LimitRatio.C");
   //delete c1;


   /*
   c1 = new TCanvas("c1", "c1",600,600);
   framework = new TH1F("Graph","Graph",1,150,650);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("R / R_{|Zll-Zmass|<15GeV}");
   framework->GetYaxis()->SetTitleOffset(1.40);
   framework->GetYaxis()->SetRangeUser(0.7,1.3);
   framework->Draw();
   pave->Draw("same");

   LEG = new TLegend(0.30,0.75,0.60,0.94);
   LEG->SetFillStyle(0);
   LEG->SetBorderSize(0);
   LEG->SetHeader("Bins: 0,1,2jets,VBF");

   Ref = getGraph("ref" , 1, 2, 1, NULL, NULL, "../computeLimits_012V/COUNT_LimitSummary");
   getGraph("|Zll-Zmass|<15GeV"         , 1, 2, 1, LEG, Ref, "../computeLimits_012V/COUNT_LimitSummary")->Draw("C same");
   getGraph("|Zll-Zmass|<10GeV"         , 4, 2, 1, LEG, Ref, "../computeLimits_012V_Z10/COUNT_LimitSummary")->Draw("C same");
   getGraph("|Zll-Zmass|< 5GeV"         , 2, 2, 1, LEG, Ref, "../computeLimits_012V_Z5/COUNT_LimitSummary")->Draw("C same");
   LEG ->Draw("same");

   c1->SaveAs("LimitPlots/LimitZMassWindow.png");
   c1->SaveAs("LimitPlots/LimitZMassWindow.pdf");
   c1->SaveAs("LimitPlots/LimitZMassWindow.C");
   delete c1;


   c1 = new TCanvas("c1", "c1",600,600);
   framework = new TH1F("Graph","Graph",1,150,650);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("R / R_{cut&count inclusive}");
   framework->GetYaxis()->SetTitleOffset(1.40);
   framework->GetYaxis()->SetRangeUser(0.6,1.2);
   framework->Draw();
   pave->Draw("same");

   LEG = new TLegend(0.30,0.75,0.60,0.94);
   LEG->SetFillStyle(0);
   LEG->SetBorderSize(0);
   LEG->SetHeader(NULL);

   Ref = getGraph("ref" , 1, 2, 1, NULL, NULL, "../computeLimits_Inc/COUNT_LimitSummary");
   getGraph("Cut&Count (inclusive)"    , 1, 2, 1, LEG, Ref, "../computeLimits_Inc/COUNT_LimitSummary")->Draw("C same");
   getGraph("Shape (inclusive)"        , 2, 2, 1, LEG, Ref, "../computeLimits_Inc/SHAPE_LimitSummary")->Draw("C same");
   getGraph("Cut&Count (binned)" , 1, 2, 2, LEG, Ref, "../computeLimits_012V/COUNT_LimitSummary")->Draw("C same");
   getGraph("Shape (binned)"     , 2, 2, 2, LEG, Ref, "../computeLimits_012V/SHAPE_LimitSummary")->Draw("C same");
   LEG ->Draw("same");

   c1->SaveAs("LimitPlots/LimitShape.png");
   c1->SaveAs("LimitPlots/LimitShape.pdf");
   c1->SaveAs("LimitPlots/LimitShape.C");
   delete c1;
   */

}






