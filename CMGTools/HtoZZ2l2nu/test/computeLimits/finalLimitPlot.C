
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
#include "TGraph2D.h"
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
#include "TMarker.h"
//#include "tdrstyle.C"


TGraph* getGraph(string name, int color, int width, int style, TLegend* LEG, TGraph* Ref, int type, string filePath){
//   filePath+="/LimitSummary";
   FILE* pFile = fopen(filePath.c_str(),"r");
   if(!pFile){printf("Can't open %s\n",filePath.c_str()); exit(0);}
   double mass, th, exp, obs, unused;// char buffer[1024];
   double explow2, explow1, expup1, expup2;

   TGraph* graph = new TGraph(100);
   int N=0;
   while(fscanf(pFile,"$%le$ & $%le$ & $[%le,%le]$ & $[%le,%le]$ & $%le$ & Th=$%le$\\\\\\hline\n",&mass, &exp, &explow1, &expup1, &explow2,&expup2,&obs, &th) != EOF){
//      printf("%i %f - %f - %f\n",N,mass,exp, th);

      double value = exp;
      if(abs(type)==0) value = th;
      else if(abs(type)==1) value = exp;
      else if(abs(type)==2) value = obs;
      else if(abs(type)==3) value = explow1;
      else if(abs(type)==4) value = expup1;
      else if(abs(type)==5) value = explow2;
      else if(abs(type)==6) value = expup2;
      else value = obs;

      if(type<0 && filePath.find("cp0.80")!=string::npos) value *= pow(0.8, 2);
      if(type<0 && filePath.find("cp0.60")!=string::npos) value *= pow(0.6, 2);
      if(type<0 && filePath.find("cp0.30")!=string::npos) value *= pow(0.3, 2);
      if(type<0 && filePath.find("cp0.10")!=string::npos) value *= pow(0.1, 2);

      if(Ref){value/=Ref->Eval(mass);}
      graph->SetPoint(N, mass, value);N++;
      if(N>25)break;
   }
   fclose(pFile);
   graph->Set(N);

   graph->SetName(name.c_str());
   graph->SetLineColor(color);
   graph->SetLineWidth(width);
   graph->SetLineStyle(style);
   if(LEG)LEG->AddEntry(graph, name.c_str()      ,"L");
   return graph;
}


TGraph** getGraphs(string name, int color, int width, TLegend* LEG, TGraph* Ref, string filePath){
   TGraph** graphs = new TGraph*[7];
   for(int i=0;i<7;i++){  
      char nameBuf[255];sprintf(nameBuf,"%s_%i",name.c_str(),i);  
      graphs[i] = getGraph(nameBuf, color, width, i<=1?2:1, NULL, Ref, i, filePath);
   }
   if(LEG)LEG->AddEntry(graphs[2], name.c_str()      ,"L");
   return graphs;
}


TCutG* GetErrorBand(string name, int N, double* Mass, double* Low, double* High, double ymin=0.0, double ymax=1.0)
{
   TCutG* cutg = new TCutG(name.c_str(),2*N+2);
   cutg->SetFillColor(kGreen-7);
   cutg->SetLineStyle(0);
   cutg->SetLineColor(0);
   int I = 0;
   for(int i=0;i<N;i++){cutg->SetPoint(I,Mass[i]    , std::max(ymin,std::min(ymax,Low[i]     )));I++; }
                        cutg->SetPoint(I,Mass[N-1]  , std::max(ymin,std::min(ymax,Low[N-1]   )));I++;
                        cutg->SetPoint(I,Mass[N-1]  , std::max(ymin,std::min(ymax,High[N-1]  )));I++;
   for(int i=0;i<N;i++){cutg->SetPoint(I,Mass[N-1-i], std::max(ymin,std::min(ymax,High[N-1-i])));I++;}
   return cutg;
}



void finalLimitPlot(){
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

   TCanvas* c1;
   TH1F* framework;
   TH2F* framework2d;
   TLegend* LEG, *LEGTH;
   TGraph* Ref;


   string Directories[]={"cards_CC8TeV_GG", "cards_SB8TeV_GG", "cards_CC8TeV_QQ", "cards_SB8TeV_QQ",  "cards_CC7TeV_GG", "cards_SB7TeV_GG", "cards_CC7TeV_QQ", "cards_SB7TeV_QQ"};
   for(unsigned int D=0;D<sizeof(Directories)/sizeof(string);D++){
      string Dir = Directories[D];
      for(int observed=0;observed<=1;observed++){


         c1 = new TCanvas("c", "c",600,600);
         c1->SetLogy(true);
         framework = new TH1F("Graph","Graph",1,150,1050);
         framework->SetStats(false);
         framework->SetTitle("");
         framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
         framework->GetYaxis()->SetTitle("#sigma_{95%} (fb)");
         framework->GetYaxis()->SetTitleOffset(1.40);
         framework->GetYaxis()->SetRangeUser(1.0,100);
         framework->Draw();

         LEG = new TLegend(0.70,0.70,0.95,0.94);
         LEG->SetFillStyle(0);
         LEG->SetBorderSize(0);
         LEG->SetHeader(observed==0?"Expected @95% CL":"Observed @95% CL");

         LEGTH = new TLegend(0.45,0.70,0.70,0.94);
         LEGTH->SetFillStyle(0);
         LEGTH->SetBorderSize(0);
         LEGTH->SetHeader("Theoretical");


         getGraph("SM"                         , 1, 1, 2, LEGTH, NULL, 0, Dir+               "/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=1.0"                     , 2, 1, 2, LEGTH, NULL, 0, Dir+"_cp1.00_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.8"                     , 3, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.80_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.6"                     , 4, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.60_brn0.00/XSec_LimitSummary")->Draw("C same");
//       getGraph("C'=0.5"                     , 6, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.50_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.4"                     , 6, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.40_brn0.00/XSec_LimitSummary")->Draw("C same");
//       getGraph("C'=0.3"                     , 6, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.30_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.2"                     , 8, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.20_brn0.00/XSec_LimitSummary")->Draw("C same");
   //      getGraph("C'=0.1"                     , 8, 1, 2, LEGTH, NULL, 0, Dir+"_cp0.10_brn0.00/XSec_LimitSummary")->Draw("C same");

         getGraph("SM"                         , 1, 2, 1, LEG  , NULL, 1+observed, Dir+               "/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=1.0"                     , 2, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp1.00_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.8"                     , 3, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.80_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.6"                     , 4, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.60_brn0.00/XSec_LimitSummary")->Draw("C same");
//         getGraph("C'=0.5"                     , 4, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.50_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.4"                     , 6, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.40_brn0.00/XSec_LimitSummary")->Draw("C same");
//         getGraph("C'=0.3"                     , 4, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.30_brn0.00/XSec_LimitSummary")->Draw("C same");
         getGraph("C'=0.2"                     , 8, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.20_brn0.00/XSec_LimitSummary")->Draw("C same");
   //      getGraph("C'=0.1"                     , 8, 2, 1, LEG  , NULL, 1+observed, Dir+"_cp0.10_brn0.00/XSec_LimitSummary")->Draw("C same");

         LEGTH->Draw("same");
         LEG  ->Draw("same");

         char LumiLabel[1024];
         if(Dir.find("7TeV")!=string::npos)sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=7 TeV, #int L=%6.1ffb^{-1}",5.035);
         if(Dir.find("8TeV")!=string::npos)sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=8 TeV, #int L=%6.1ffb^{-1}",19.6);
         if(Dir.find("Comb")!=string::npos)sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f TeV #scale[0.5]{#int} L=%6.1ffb^{-1}, #sqrt{s}=%.0f TeV #scale[0.5]{#int} L=%6.1ffb^{-1}",7.0,5.0,8.0,19.7);
         TPaveText *pave = new TPaveText(0.1,0.96,0.94,0.99,"NDC");
         pave->SetBorderSize(0);
         pave->SetFillStyle(0);
         pave->SetTextAlign(32);
         pave->SetTextFont(42);
         pave->AddText(LumiLabel);
         pave->Draw("same");

         //system("mkdir -p LimitPlots");
         if(observed==0){
            c1->SaveAs((Dir+"/XSec_FinalPlot.png").c_str());
            c1->SaveAs((Dir+"/XSec_FinalPlot.pdf").c_str());
            c1->SaveAs((Dir+"/XSec_FinalPlot.C"  ).c_str());
         }else{
            c1->SaveAs((Dir+"/XSec_FinalPlot_Obs.png").c_str());
            c1->SaveAs((Dir+"/XSec_FinalPlot_Obs.pdf").c_str());
            c1->SaveAs((Dir+"/XSec_FinalPlot_Obs.C"  ).c_str());
         }
      }
   }

//      string Directories2[]={"cards_CC7TeV", "cards_SB7TeV", "cards_CC7TeV_GG", "cards_SB7TeV_GG", "cards_CC7TeV_QQ", "cards_SB7TeV_QQ",    "cards_CC8TeV", "cards_SB8TeV", "cards_CC8TeV_GG", "cards_SB8TeV_GG", "cards_CC8TeV_QQ", "cards_SB8TeV_QQ",  "cards_CCComb", "cards_SBComb", "cards_CCComb_GG", "cards_SBComb_GG", "cards_CCComb_QQ", "cards_SBComb_QQ"};
   string Directories2[]={"cards_CC7TeV", "cards_SB7TeV", "cards_CC7TeV_GG", "cards_SB7TeV_GG", "cards_CC7TeV_QQ", "cards_SB7TeV_QQ",    "cards_CC8TeV", "cards_SB8TeV", "cards_CC8TeV_GG", "cards_SB8TeV_GG", "cards_CC8TeV_QQ", "cards_SB8TeV_QQ",  "cards_CCComb", "cards_SBComb"};
   for(unsigned int D=0;D<sizeof(Directories2)/sizeof(string);D++){
      string Dir = Directories2[D];
      TGraph* gMvsCp[7];

      LEG = new TLegend(0.70,0.70,0.95,0.94);
      LEG->SetFillStyle(0);
      LEG->SetBorderSize(0);
      TGraph** gSM=getGraphs("SM"                         , 1, 2, LEG  , NULL, Dir+               "/Stength_LimitSummary");
      TGraph** g10=getGraphs("C'=1.0"                     , 2, 2, LEG  , NULL, Dir+"_cp1.00_brn0.00/Stength_LimitSummary");
      TGraph** g08=getGraphs("C'=0.8"                     , 3, 2, LEG  , NULL, Dir+"_cp0.80_brn0.00/Stength_LimitSummary");
      TGraph** g06=getGraphs("C'=0.6"                     , 4, 2, LEG  , NULL, Dir+"_cp0.60_brn0.00/Stength_LimitSummary");
      TGraph** g05=getGraphs("C'=0.5"                     , 6, 2, NULL , NULL, Dir+"_cp0.50_brn0.00/Stength_LimitSummary");
      TGraph** g04=getGraphs("C'=0.4"                     , 6, 2, LEG  , NULL, Dir+"_cp0.40_brn0.00/Stength_LimitSummary");
      TGraph** g03=getGraphs("C'=0.3"                     , 6, 2, NULL , NULL, Dir+"_cp0.30_brn0.00/Stength_LimitSummary");
      TGraph** g02=getGraphs("C'=0.2"                     , 8, 2, LEG  , NULL, Dir+"_cp0.20_brn0.00/Stength_LimitSummary");
      TGraph** g01=getGraphs("C'=0.1"                     , 8, 2, NULL , NULL, Dir+"_cp0.10_brn0.00/Stength_LimitSummary");

      char LumiLabel[1024];
      if(Dir.find("7TeV")!=string::npos)sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=7 TeV, #int L=%6.1ffb^{-1}",5.035);
      if(Dir.find("8TeV")!=string::npos)sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=8 TeV, #int L=%6.1ffb^{-1}",19.6);
      if(Dir.find("Comb")!=string::npos)sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=%.0f TeV #scale[0.5]{#int} L=%6.1ffb^{-1}, #sqrt{s}=%.0f TeV #scale[0.5]{#int} L=%6.1ffb^{-1}",7.0,5.0,8.0,19.7);
      TPaveText *pave = new TPaveText(0.1,0.96,0.94,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText(LumiLabel);
      pave->Draw("same");


      for(int observed=0;observed<=1;observed++){
         c1 = new TCanvas("c", "c",600,600);
         c1->SetLogy(true);
         framework = new TH1F("Graph","Graph",1,150,1050);
         framework->SetStats(false);
         framework->SetTitle("");
         framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
         framework->GetYaxis()->SetTitle("#sigma_{95%} / #sigma_{TH}");
         framework->GetYaxis()->SetTitleOffset(1.40);
         framework->GetYaxis()->SetRangeUser(0.1,500);
   //      framework->GetYaxis()->SetRangeUser(0.1,10);
         framework->Draw();


         gSM[1+observed]->SetLineStyle(1);
         g10[1+observed]->SetLineStyle(1);
         g08[1+observed]->SetLineStyle(1);
         g06[1+observed]->SetLineStyle(1);
         g04[1+observed]->SetLineStyle(1);
         g02[1+observed]->SetLineStyle(1);

         gSM[1+observed]->Draw("C same");
         g10[1+observed]->Draw("C same");
         g08[1+observed]->Draw("C same");
         g06[1+observed]->Draw("C same");
         g04[1+observed]->Draw("C same");
         g02[1+observed]->Draw("C same");
         //g01[1+observed]->Draw("C same");

         LEG  ->Draw("same");
         LEG->SetHeader(observed==0?"Expected @95% CL":"Observed @95% CL");


         TLine* SMLine = new TLine(framework->GetXaxis()->GetXmin(),1.0,framework->GetXaxis()->GetXmax(),1.0);
         SMLine->SetLineWidth(2); SMLine->SetLineStyle(2); SMLine->SetLineColor(1);
         SMLine->Draw("same C");

         //system("mkdir -p LimitPlots");
         if(observed==0){
            c1->SaveAs((Dir+"/Stength_FinalPlot.png").c_str());
            c1->SaveAs((Dir+"/Stength_FinalPlot.pdf").c_str());
            c1->SaveAs((Dir+"/Stength_FinalPlot.C"  ).c_str());
         }else{
            c1->SaveAs((Dir+"/Stength_FinalPlot_Obs.png").c_str());
            c1->SaveAs((Dir+"/Stength_FinalPlot_Obs.pdf").c_str());
            c1->SaveAs((Dir+"/Stength_FinalPlot_Obs.C"  ).c_str());
         }
     }


          ///////////////////////////////////////////////
          //Mass Versus Cprime limits
          ///////////////////////////////////////////////

      for(int type=1;type<7;type++){
            if(type==0)continue;
            TGraph* g1dV[] = {g10[type], g08[type], g06[type], g05[type], g04[type], g03[type], g02[type], g01[type]};
            double* Masses = g1dV[0]->GetX();  
            int NMasses = g1dV[0]->GetN();
            double    cp[] = {1.0, 0.8, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
           
            gMvsCp[type] = (TGraph*)g1dV[0]->Clone();
 
            for(int Mi=0;Mi<NMasses;Mi++){
               TGraph* g1d = new TGraph( (sizeof(cp)/sizeof(double)) );
               for(unsigned int C=0;C<(sizeof(g1dV)/sizeof(TGraph*));C++){
                  g1d->SetPoint(C, cp[C], g1dV[C]->Eval(Masses[Mi]));
//                  if(Masses[Mi]==400.0)printf("xxx %f --> %f\n",cp[C], g1dV[C]->Eval(Masses[Mi]));
               }

               //find C' value that is excluded
               double cpExcluded = 2.0;  double cpPrev=-1;  double limitPrev=-1;
               for(double cp=1.0; cp>0.0;cp-=0.01){
//                  if(Masses[Mi]==400.0) printf("c=%f --> %f (compared to  %f)\n", cp, g1d->Eval(cp), limitPrev);

                  if(limitPrev!=-1 && limitPrev<=1.0 && g1d->Eval(cp)>=1.0){cpExcluded=cpPrev;break;}
                  limitPrev=g1d->Eval(cp);  cpPrev=cp;
               }
               gMvsCp[type]->SetPoint(Mi, Masses[Mi], pow(cpExcluded,2));
//               gMvsCp[type]->SetPoint(Mi, Masses[Mi], cpExcluded);
//               printf("%i Mass = %f -> cpexcluded = %f\n", type, Masses[Mi], cpExcluded);
            }
      }

         if(true){
            c1 = new TCanvas("c", "c",600,600);
            framework = new TH1F("Graph","Graph",1,150,1050);
            framework->SetStats(false);
            framework->SetTitle("");
            framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
            framework->GetYaxis()->SetTitle("C'^{2}_{95%}");
            framework->GetYaxis()->SetTitleOffset(1.40);
            framework->GetYaxis()->SetRangeUser(0.0,1.0);
            framework->Draw();

            pave->Draw("same");

            //expected bands
            TCutG* TGExpLimit1S  = GetErrorBand("1S", gMvsCp[3]->GetN(), gMvsCp[3]->GetX(), gMvsCp[3]->GetY(), gMvsCp[4]->GetY());  
            TCutG* TGExpLimit2S  = GetErrorBand("2S", gMvsCp[3]->GetN(), gMvsCp[3]->GetX(), gMvsCp[5]->GetY(), gMvsCp[6]->GetY());  TGExpLimit2S->SetFillColor(5);
            TGExpLimit2S->Draw("f same");
            TGExpLimit1S->Draw("f same");

            //expected
            gMvsCp[1]->SetLineColor(1);
            gMvsCp[1]->SetLineWidth(1);
            gMvsCp[1]->SetLineStyle(2);
            gMvsCp[1]->Draw("same");

            //observed
            gMvsCp[2]->SetLineColor(1);
            gMvsCp[2]->SetLineWidth(2);
            gMvsCp[2]->SetLineStyle(1);
            gMvsCp[2]->Draw("same");


//for(int i=3;i<7;i++){
//            gMvsCp[i]->SetLineColor(4);
//            gMvsCp[i]->SetLineWidth(1);
//            gMvsCp[i]->SetLineStyle(2);
//            gMvsCp[i]->Draw("C same");
//}

            LEG = new TLegend(0.50,0.20,0.95,0.44);
            LEG->SetFillStyle(0);
            LEG->SetBorderSize(0);
            LEG->SetHeader(NULL);
            LEG->AddEntry(gMvsCp[1],"Expected @95% CL", "L");
            LEG->AddEntry(gMvsCp[2],"Observed @95% CL", "L");
            LEG->Draw();

            c1->SaveAs((Dir+"/Stength_FinalPlot_Cprime.png").c_str());
            c1->SaveAs((Dir+"/Stength_FinalPlot_Cprime.pdf").c_str());
            c1->SaveAs((Dir+"/Stength_FinalPlot_Cprime.C"  ).c_str());
        }

          ///////////////////////////////////////////////
          //Mass Versus Cprime limits
          ///////////////////////////////////////////////

      for(int observed=0;observed<=1;observed++){

         for(int mode=0; mode<=1; mode++){
            //mode=0 --> CPrime versus BRNew
            //mode=1 --> width  versus BRNew
////
            double Masses[] = {200,400,600,800};
            for(int Mi=0;Mi<sizeof(Masses)/sizeof(double);Mi++){
               double Mass = Masses[Mi];
               TGraph* g1dV[] = {g10[1+observed], g08[1+observed], g06[1+observed], g05[1+observed], g04[1+observed], g03[1+observed], g02[1+observed], g01[1+observed]};
               double    cp[] = {1.0, 0.8, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
               TGraph*   g1d  = new TGraph  ( (sizeof(g1dV)/sizeof(TGraph*)) );
               for(unsigned int C=0;C<(sizeof(g1dV)/sizeof(TGraph*));C++){
                  g1d->SetPoint(C, cp[C], g1dV[C]->Eval(Mass));
               }

               int I=0;      
//               double   cp2[] = {1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.3, 0.2, 0.1, 0.0};
//               double  brn[] = {0.995, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0};
               double   cp2[] = {1.0, 0.975, 0.95, 0.925, 0.9, 0.875, 0.85, 0.825, 0.8, 0.775, 0.75, 0.725, 0.7, 0.675, 0.65, 0.625, 0.6, 0.575, 0.55, 0.525, 0.5, 0.475, 0.45, 0.425, 0.4, 0.375, 0.35, 0.325, 0.3, 0.275, 0.25, 0.225, 0.2, 0.175, 0.15, 0.125, 0.1, 0.075, 0.05, 0.025, 0.0};
               double  brn[] = {0.995, 0.975, 0.95, 0.925, 0.9, 0.875, 0.85, 0.825, 0.8, 0.775, 0.75, 0.725, 0.7, 0.675, 0.65, 0.625, 0.6, 0.575, 0.55, 0.525, 0.5, 0.475, 0.45, 0.425, 0.4, 0.375, 0.35, 0.325, 0.3, 0.275, 0.25, 0.225, 0.2, 0.175, 0.15, 0.125, 0.1, 0.075, 0.05, 0.025, 0.0};
               TGraph2D* g2d = new TGraph2D( (sizeof(cp2)/sizeof(double)) * (sizeof(brn)/sizeof(double))  );
               for(unsigned int C=0;C<(sizeof( cp2)/sizeof(double));C++){
               for(unsigned int B=0;B<(sizeof(brn)/sizeof(double));B++){
                  double BR = brn[B];
                  double sl = g1d->Eval(sqrt(cp2[C]));

                  //move from br=0 --> br=BR
                  double cp2_true = cp2[C]*(1-BR);
                  double sl_true = sl/(1-BR);
                  if(mode==0){        g2d->SetPoint(I, cp2_true, BR, sl_true);
                  }else if(mode==1){  g2d->SetPoint(I, sqrt(cp2[C]), BR, sl_true);
   //               }else if(mode==1){  g2d->SetPoint(I, cp2[C]*(1-BR), BR, sl_true);
                  }
                  I++;
               }}g2d->Set(I);

               c1 = new TCanvas("c", "c",600,600);
               c1->SetLogz(true);      
               c1->SetRightMargin(0.17);

               framework2d = new TH2F("Graph","Graph",1,mode==0?0.0:0.3,1, 1,0,1);
               framework2d->SetStats(false);
               framework2d->SetTitle("");
               framework2d->GetXaxis()->SetTitle(mode==0?"c'^{2}":"#Gamma/#Gamma_{SM}");
               framework2d->GetYaxis()->SetTitle("BR_{new}");
               framework2d->GetYaxis()->SetTitleOffset(1.40);
               framework2d->GetYaxis()->SetRangeUser(0, 1);
               framework2d->Draw("");
               pave->Draw("same");

               TH2D* h2d = g2d->GetHistogram();
               h2d->SetMaximum(10);
               h2d->SetMinimum(1E-1);
               h2d->GetZaxis()->SetTitle(observed==0?"Expected #sigma_{95%} / #sigma_{TH}":"Observed #sigma_{95%} / #sigma_{TH}");
               h2d->GetZaxis()->SetTitleOffset(1.33);
               h2d->Draw("COLZ same");

               TH1D* h2dLimit = new TH1D("ExcludedArea", "ExcludedArea", h2d->GetNbinsX(), 0.0, 1.0);  
               for(int x=0;x<=h2d->GetNbinsX();x++){
               double limit = -1;
               for(int y=0;y<=h2d->GetNbinsY()+1;y++){
                     double bin = h2d->GetBinContent(x,y);
                     if(x==15)printf("Dir=%s Mass=%f: Mode=%i cprime2 = %f  brnew=%f  limit=%f\n", Dir.c_str(), Mass, mode, h2d->GetXaxis()->GetBinCenter(x), h2d->GetYaxis()->GetBinCenter(y), bin );
                     if(mode==0 && (bin>=1 || h2d->GetYaxis()->GetBinCenter(y)>=1-h2d->GetXaxis()->GetBinCenter(x))){limit = std::max(0.0, h2d->GetYaxis()->GetBinLowEdge(y) ); break;}
                     if(mode==1 &&  bin<=1 && y<h2d->GetNbinsY()){limit = std::max(0.0, h2d->GetYaxis()->GetBinUpEdge(y) );}
                  }
                  h2dLimit->SetBinContent(x,limit);
               } 
               h2dLimit->SetLineColor(1);
               h2dLimit->SetLineWidth(1);
               h2dLimit->SetFillStyle(3654);
               h2dLimit->SetFillColor(1);
               h2dLimit->Draw("HIST same");

               
               if(mode==0){
                  TLine* W50 = new TLine(0.0, 1.0, 1.0, 0.8);
                  W50->SetLineWidth(1); W50->SetLineStyle(3); W50->SetLineColor(1);    W50->Draw("same C");

                  TLine* W10 = new TLine(0.0, 1.00, 1.0, 0.0);
                  W10->SetLineWidth(1); W10->SetLineStyle(3); W10->SetLineColor(1);    W10->Draw("same C");

                  TLine* W05 = new TLine(0.0, 1.0, 0.5, 0.0);
                  W05->SetLineWidth(1); W05->SetLineStyle(3); W05->SetLineColor(1);    W05->Draw("same C");

                  TLine* W01 = new TLine(0.0, 1.0, 0.1, 0.0);
                  W01->SetLineWidth(1); W01->SetLineStyle(3); W01->SetLineColor(1);    W01->Draw("same C");
               }

               //add marker on the plots
               for(unsigned int C=0;C<(sizeof( cp2)/sizeof(double));C++){
               for(unsigned int B=0;B<(sizeof(brn)/sizeof(double));B++){
                  double BR = brn[B];
                  double sl = g1d->Eval(sqrt(cp2[C]));

                  //move from br=0 --> br=BR
                  double cp2_true = cp2[C]*(1-BR);
                  double sl_true = sl/(1-BR);
                  //TMarker* m = new TMarker(mode==0?cp2_true:sqrt(cp2[C]), BR, 20); m->SetMarkerSize(1.0); m->SetMarkerColor(1);      m->Draw("same");
               }}



               char massStr[512]; sprintf(massStr, "%04.0f", Mass);
               if(mode>0)sprintf(massStr, "%s_Width", massStr);
               if(observed!=0)sprintf(massStr, "%s_Obs", massStr);
               c1->SaveAs((Dir+"/Stength_FinalPlot2D_"+massStr+".png").c_str());
               c1->SaveAs((Dir+"/Stength_FinalPlot2D_"+massStr+".pdf").c_str());
               c1->SaveAs((Dir+"/Stength_FinalPlot2D_"+massStr+".C").c_str());
            }
////
         }
      }
   }
}






