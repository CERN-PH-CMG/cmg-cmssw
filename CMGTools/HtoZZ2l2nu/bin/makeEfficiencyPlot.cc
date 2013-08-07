#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#include "TGraph.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TGraphErrors.h"

#include "../src/tdrstyle.C"

using namespace std;


struct stData{
   double mass; 
   double xsec;
   double br;
   double eff;
   double err;
};

bool dataOrdering (stData i,stData j){ return (i.mass<j.mass);}



//
int main(int argc, char *argv[])
{
   setTDRStyle();
   gStyle->SetPadTopMargin   (0.04);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.05);
   gStyle->SetPadLeftMargin  (0.12);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);

   std::map<string, std::map<string, std::vector<stData> >   > map;
   std::map<string,  TGraph* > graphs;


   for(int i=1;i<argc;i++){
     string arg(argv[i]);

     FILE* pFile = fopen(arg.c_str(),"r");
     if(!pFile){printf("Can't open %s\n",arg.c_str()); return 0;}
     while(true){
        char bin[1024]; char proc[1024]; stData data;
        if(fscanf(pFile,"%s %s %lf %lf %lf %lf %lf\n",bin, proc, &data.mass, &data.xsec, &data.br, &data.eff, &data.err)==EOF)break;
        string procS = proc;
        procS = procS.substr(0,procS.find('('));

        std::map<string, std::vector<stData> >& tmp = map[procS];
        std::vector<stData>& tmp2 = tmp[bin];
        tmp2.push_back(data);
     }
     fclose(pFile);     
   }


   for(std::map<string, std::map<string, std::vector<stData> > >::iterator it=map.begin(); it!=map.end(); it++){
      printf("%s\n",it->first.c_str());
      for(std::map<string, std::vector<stData> >::iterator itt=it->second.begin(); itt!=it->second.end(); itt++){
         printf("   %s\n",itt->first.c_str());
         std::vector<stData>& dataVec = itt->second;
         std::sort(dataVec.begin(), dataVec.end(), dataOrdering);
         for(unsigned int m=0;m<dataVec.size();m++){ printf("      %f --> %f x %f\n",dataVec[m].mass, dataVec[m].eff / 10198.0, dataVec[m].br);  }


         string name = it->first + "-" + itt->first;
         TGraphErrors* graph = new TGraphErrors(dataVec.size());
         int N=0;         
//         for(unsigned int m=0;m<dataVec.size();m++){ if(dataVec[m].xsec<=0 || isnan((float)(dataVec[m].eff) || isinf((float)dataVec[m].eff)))continue;  double scaleFactor = 10198 / (dataVec[m].xsec * dataVec[m].br);  graph->SetPoint(N, dataVec[m].mass, dataVec[m].eff / scaleFactor ); graph->SetPointError(N,0,dataVec[m].err / scaleFactor);  N++; }
         for(unsigned int m=0;m<dataVec.size();m++){ if(dataVec[m].xsec<=0 || isnan((float)(dataVec[m].eff) || isinf((float)dataVec[m].eff)))continue;  double scaleFactor = 1.0 / (dataVec[m].xsec * dataVec[m].br);  graph->SetPoint(N, dataVec[m].mass, dataVec[m].eff / scaleFactor ); graph->SetPointError(N,0,dataVec[m].err / scaleFactor);  N++; }
         graph->Set(N);
         graph->SetName(name.c_str());
         graph->SetLineColor(4);
         graph->SetLineWidth(3);
         graph->SetLineStyle(1);
         graphs[name] = graph;
      }
   }


   for(std::map<string,  TGraph* >::iterator it=graphs.begin();it!=graphs.end();it++){
      TCanvas* c1 = new TCanvas("c1", "c1",600,600);
      c1->SetLogy(true);
      c1->SetGridx(true);
      c1->SetGridy(true);

      TH1F* framework = new TH1F("Graph","Graph",1,150,1050);
      framework->SetStats(false);
      framework->SetTitle("");
      framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
      framework->GetYaxis()->SetTitle("final selection efficiency (#epsilon)");
      framework->GetYaxis()->SetTitleOffset(1.40);
      framework->GetYaxis()->SetRangeUser(1E-3, 5E-1);
      framework->Draw();

      it->second->Draw("AC*");


      TPaveText *pave = new TPaveText(0.1,0.96,0.94,0.99,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(12);
      pave->SetTextFont(42);
      pave->AddText(it->first.c_str());
      pave->Draw("same");


      c1->SaveAs((string("Eff_")+it->first+".png").c_str());
      delete c1; delete framework;
   }




/*


   char LumiLabel[1024];
   sprintf(LumiLabel,"CMS preliminary,  #sqrt{s}=7 TeV, #int L=%6.1ffb^{-1}",5.035);
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

   c1 = new TCanvas("c1", "c1",600,600);
   c1->SetLogy(true);
   framework = new TH1F("Graph","Graph",1,150,650);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("#sigma_{95%}/#sigma_{SM}");
   framework->GetYaxis()->SetTitleOffset(1.40);
   framework->GetYaxis()->SetRangeUser(0.4,25);
   framework->Draw();
   pave->Draw("same");

   LEG = new TLegend(0.30,0.75,0.60,0.94);
   LEG->SetFillStyle(0);
   LEG->SetBorderSize(0);
   LEG->SetHeader("Expected @95% C.L.");
   LEG2 = new TLegend(0.50,0.75,0.80,0.94);
   LEG2->SetFillStyle(0);
   LEG2->SetBorderSize(0);
   LEG2->SetHeader(" ");


   getGraph("0jet"                  ,42, 2, 2, LEG , NULL, "../computeLimits_0/COUNT_LimitSummary")->Draw("C same");
   getGraph("1jet"                  ,44, 2, 2, LEG , NULL, "../computeLimits_1/COUNT_LimitSummary")->Draw("C same");
   getGraph("2jets"                 ,46, 2, 2, LEG , NULL, "../computeLimits_2/COUNT_LimitSummary")->Draw("C same");
   getGraph("VBF"                   ,40, 2, 2, LEG , NULL, "../computeLimits_VonlyFixed/COUNT_LimitSummary")->Draw("C same");
//   getGraph("VonlyFree"           , 5, 2, 1, LEG , NULL, "../computeLimits_VonlyFree/COUNT_LimitSummary")->Draw("C same");
   getGraph("Inclusive"             , 1, 2, 1, LEG , NULL, "../computeLimits_Inc/COUNT_LimitSummary")->Draw("C same");

   getGraph("VBF/noVBF"             , 6, 2, 1, LEG2, NULL, "../computeLimits_VnoV/COUNT_LimitSummary")->Draw("C same");
   getGraph("0,#geq1jet,VBF"            , 8, 2, 1, LEG2, NULL, "../computeLimits_01V/COUNT_LimitSummary")->Draw("C same");
   getGraph("0,1,#geq2jets"             ,16, 2, 1, LEG2, NULL, "../computeLimits_012/COUNT_LimitSummary")->Draw("C same");
   getGraph("0,1,#geq2jets,VBF"         , 2, 2, 1, LEG2, NULL, "../computeLimits_012V/COUNT_LimitSummary")->Draw("C same");
//   getGraph("012V_NRBDY"          , 1, 2, 1, LEG2, NULL, "../computeLimits_012V_NRBDY/COUNT_LimitSummary")->Draw("C same");
   getGraph("0,1,#geq2jets,VBF (redMET)", 4, 2, 1, LEG2, NULL, "../computeLimits_012V_redMET/COUNT_LimitSummary")->Draw("C same");
//   getGraph("012V_Z10"            , 1, 2, 1, LEG2, NULL, "../computeLimits_012V_Z10/COUNT_LimitSummary")->Draw("C same");
//   getGraph("012V_Z5"             , 1, 2, 1, LEG2, NULL, "../computeLimits_012V_Z5/COUNT_LimitSummary")->Draw("C same");
   
   LEG ->Draw("same");
   LEG2->Draw("same");

   system("mkdir -p LimitPlots");
   c1->SaveAs("LimitPlots/LimitSummary.png");
   c1->SaveAs("LimitPlots/LimitSummary.pdf");
   c1->SaveAs("LimitPlots/LimitSummary.C");
   delete c1;


   c1 = new TCanvas("c1", "c1",600,600);
   framework = new TH1F("Graph","Graph",1,150,650);
   framework->SetStats(false);
   framework->SetTitle("");
   framework->GetXaxis()->SetTitle("M_{H} [GeV/c^{2}]");
   framework->GetYaxis()->SetTitle("R / R_{inclusive}");
   framework->GetYaxis()->SetTitleOffset(1.40);
   framework->GetYaxis()->SetRangeUser(0.5,1.2);
   framework->Draw();
   pave->Draw("same");

   LEG = new TLegend(0.40,0.20,0.70,0.54);
   LEG->SetFillStyle(0);
   LEG->SetBorderSize(0);
   LEG->SetHeader(NULL);

   Ref = getGraph("Inclusive", 1, 2, 1, NULL, NULL, "../computeLimits_Inc/COUNT_LimitSummary");
   getGraph("Inclusive"             , 1, 2, 1, LEG, Ref, "../computeLimits_Inc/COUNT_LimitSummary")->Draw("L same");
   getGraph("VBF/noVBF"             , 6, 2, 1, LEG, Ref, "../computeLimits_VnoV/COUNT_LimitSummary")->Draw("L same");
   getGraph("0,#geq1jet,VBF"            , 8, 2, 1, LEG, Ref, "../computeLimits_01V/COUNT_LimitSummary")->Draw("L same");
   getGraph("0,1,#geq2jets"             ,16, 2, 1, LEG, Ref, "../computeLimits_012/COUNT_LimitSummary")->Draw("L same");
   getGraph("0,1,#geq2jets,VBF"         , 2, 2, 1, LEG, Ref, "../computeLimits_012V/COUNT_LimitSummary")->Draw("L same");
   getGraph("0,1,#geq2jets,VBF (redMET)", 4, 2, 1, LEG, Ref, "../computeLimits_012V_redMET/COUNT_LimitSummary")->Draw("L same");

   
   LEG ->Draw("same");

   c1->SaveAs("LimitPlots/LimitRatio.png");
   c1->SaveAs("LimitPlots/LimitRatio.pdf");
   c1->SaveAs("LimitPlots/LimitRatio.C");
   delete c1;


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
*/
}
