
#include <string>
#include <vector>
#include <iostream>
#include <iterator>

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
#include "THStack.h"

#include "../src/tdrstyle.C"
#include "../src/JSONWrapper.cc"

double iLumi = 2007;
bool do2D  = true;
bool do1D  = true;
bool doTex = true;
bool StoreInFile = true;
bool doPlot = true;
string objectSearchKey = "";
string inDir   = "OUTNew/";
string jsonFile = "../../data/beauty-samples.json";
string outDir  = "Img/";
string outFile = "plotter.root";

 

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

struct NameAndType{
   std::string Name;
   bool isTH1; 
   NameAndType(std::string Name_,  bool isTH1_){Name = Name_; isTH1 = isTH1_;}
};

void GetListOfObject(TDirectory* dir, std::vector<NameAndType>& histlist, std::string parentPath=""){
   if(dir==NULL)return;
   TList* list = dir->GetListOfKeys();
   for(int i=0;i<list->GetSize();i++){
      TObject* tmp = GetObjectFromPath(dir,list->At(i)->GetName(),false);
      if(tmp->InheritsFrom("TH1")){
         histlist.push_back(NameAndType(parentPath+list->At(i)->GetName(), !(tmp->InheritsFrom("TH2") || tmp->InheritsFrom("TH3")) ) );
      }else{
         GetListOfObject((TDirectory*)tmp,histlist, parentPath+ list->At(i)->GetName()+"/" );
      }
   }
}



void SavingToFile(JSONWrapper::Object& Root, std::string RootDir, std::string HistoName, TFile* OutputFile){
   std::vector<TObject*> ObjectToDelete;

   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}

         TFile* File = new TFile((RootDir + (Samples[j])["dtag"].toString() + ".root").c_str());
         TH1* tmphist = (TH1*) GetObjectFromPath(File,HistoName); 
         if(j==0){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}

         delete tmphist;
         delete File;
      }   

      string dirName = Process[i]["tag"].c_str();while(dirName.find("/")!=std::string::npos)dirName.replace(dirName.find("/"),1,"_");
      OutputFile->cd();
      TDirectory* subdir = OutputFile->GetDirectory(dirName.c_str());
      if(!subdir || subdir==OutputFile) subdir = OutputFile->mkdir(dirName.c_str());
      subdir->cd();
      hist->Write();
   }
}





void fixExtremities(TH1* h,bool addOverflow, bool addUnderflow)
{
  if(h==0) return;

  if(addUnderflow){
      double fbin  = h->GetBinContent(0) + h->GetBinContent(1);
      double fbine = sqrt(h->GetBinError(0)*h->GetBinError(0)
                          + h->GetBinError(1)*h->GetBinError(1));
      h->SetBinContent(1,fbin);
      h->SetBinError(1,fbine);
      h->SetBinContent(0,0);
      h->SetBinError(0,0);
    }
  
  if(addOverflow){  
      int nbins = h->GetNbinsX();
      double fbin  = h->GetBinContent(nbins) + h->GetBinContent(nbins+1);
      double fbine = sqrt(h->GetBinError(nbins)*h->GetBinError(nbins) 
                          + h->GetBinError(nbins+1)*h->GetBinError(nbins+1));
      h->SetBinContent(nbins,fbin);
      h->SetBinError(nbins,fbine);
      h->SetBinContent(nbins+1,0);
      h->SetBinError(nbins+1,0);
    }
}

/*
void Draw2DHistogram(Object& Root, std::string RootDir, std::string HistoName){

   TPaveText* T = new TPaveText(0.40,0.995,0.85,0.945, "NDC");
   T->SetFillColor(0);
   T->SetFillStyle(0);  T->SetLineColor(0);
   T->SetTextAlign(32);
   T->AddText("CMS preliminary, #sqrt{s}=7 TeV, #int L=2007 pb^{-1}");

   std::vector<Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      TCanvas* c1 = new TCanvas("c1","c1",800,800);
      c1->SetLogz(true);

      TH1* hist = NULL;
      std::vector<Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool())Weight*= 2007.0;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}

         TFile* File = new TFile((RootDir + (Samples[j])["dtag"].toString() + ".root").c_str());
         TH1* tmphist = (TH1*) GetObjectFromPath(File,HistoName); 
         if(j==0){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
         delete File;
      }   

      hist->SetTitle("");
      hist->SetStats(kFALSE);

      hist->Draw("COLZ");
      T->Draw("same");

      TPaveText* leg = new TPaveText(0.10,0.995,0.35,0.945, "NDC");
      leg->SetFillColor(0);
      leg->SetFillStyle(0);  leg->SetLineColor(0);
      leg->SetTextAlign(12);
      leg->AddText(Process[i]["tag"].c_str());
      leg->Draw("same");

      string SavePath = HistoName + "_" + Process[i]["tag"].c_str() + ".png";
      while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
      while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
      while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
      while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
      while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
      while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
      while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
      while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"_");
      SavePath = outDir + SavePath;
      system(string(("rm -f ") + SavePath).c_str());
      c1->SaveAs(SavePath.c_str());
      delete c1;
      delete leg;
      delete hist;
   }
   delete T;
}
*/

void Draw2DHistogram(JSONWrapper::Object& Root, std::string RootDir, std::string HistoName){
   std::string SaveName = "";

   TPaveText* T = new TPaveText(0.40,0.995,0.85,0.945, "NDC");
   T->SetFillColor(0);
   T->SetFillStyle(0);  T->SetLineColor(0);
   T->SetTextAlign(32);
  char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #int L=%.0f pb^{-1}", iLumi);
   T->AddText(Buffer);

   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   int CanvasX = 4;
   int CanvasY = ceil(Process.size()/CanvasX);
   TCanvas* c1 = new TCanvas("c1","c1",1000,1000);
   c1->Divide(CanvasX,CanvasY,0,0);

   std::vector<TObject*> ObjectToDelete;
   for(unsigned int i=0;i<Process.size();i++){
      TVirtualPad* pad = c1->cd(i+1);
      pad->SetLogz(true);
      pad->SetTopMargin(0.0); pad->SetBottomMargin(0.10);  pad->SetRightMargin(0.20);

      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}

         TFile* File = new TFile((RootDir + (Samples[j])["dtag"].toString() + ".root").c_str());
         TH1* tmphist = (TH1*) GetObjectFromPath(File,HistoName); 
         if(j==0){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
         delete File;
      }   
      SaveName = hist->GetName();
      ObjectToDelete.push_back(hist);
      hist->SetTitle("");
      hist->SetStats(kFALSE);

      hist->Draw("COLZ");
  
      TPaveText* leg = new TPaveText(0.10,0.995,0.30,0.90, "NDC");
      leg->SetFillColor(0);
      leg->SetFillStyle(0);  leg->SetLineColor(0);
      leg->SetTextAlign(12);
      leg->AddText(Process[i]["tag"].c_str());
      leg->Draw("same");
      ObjectToDelete.push_back(leg);
//      delete leg;
//      delete hist;
   }
   c1->cd(0);
   //T->Draw("same");
   string SavePath = SaveName + ".png";
   while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
   while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
   while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
   while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
   while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
   while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
   while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
   while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"_");
   SavePath = outDir + SavePath;
   system(string(("rm -f ") + SavePath).c_str());
   c1->SaveAs(SavePath.c_str());

   for(unsigned int d=0;d<ObjectToDelete.size();d++){delete ObjectToDelete[d];}ObjectToDelete.clear();
   delete c1;
   delete T;
}


void Draw1DHistogram(JSONWrapper::Object& Root, std::string RootDir, std::string HistoName){
   TCanvas* c1 = new TCanvas("c1","c1",800,800);
   c1->SetLogy(true);

   TLegend* legA  = new TLegend(0.51,0.93,0.67,0.75, "NDC");
   TLegend* legB  = new TLegend(0.67,0.93,0.83,0.75, "NDC");
   THStack* stack = new THStack("MC","MC");
   std::vector<TObject*> ObjectToDelete;

   std::string SaveName = "";
   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
//      printf("Process=%s\n", (Process[i])["tag"].toString().c_str());

      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
//         printf("   SubSample=%s\n",(Samples[j])["dtag"].toString().c_str());
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}

         TFile* File = new TFile((RootDir + (Samples[j])["dtag"].toString() + ".root").c_str());
         TH1* tmphist = (TH1*) GetObjectFromPath(File,HistoName); 
         if(j==0){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
//         if(j==0){hist = (TH1*)tmphist->Clone("tmp");hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}

         delete tmphist;
         delete File;
      }   

      SaveName = hist->GetName();
      if(Process[i].isTag("color" ) )hist->SetLineColor  ((int)Process[i][ "color"].toDouble()); else hist->SetLineColor  (1);
      if(Process[i].isTag("color" ) )hist->SetMarkerColor((int)Process[i][ "color"].toDouble()); else hist->SetMarkerColor(1);
      if(Process[i].isTag("color" ) )hist->SetFillColor  ((int)Process[i][ "color"].toDouble()); else hist->SetFillColor  (0);
      if(Process[i].isTag("lcolor") )hist->SetLineColor  ((int)Process[i]["lcolor"].toDouble());
      if(Process[i].isTag("mcolor") )hist->SetMarkerColor((int)Process[i]["mcolor"].toDouble()); 
      if(Process[i].isTag("fcolor") )hist->SetFillColor  ((int)Process[i]["fcolor"].toDouble()); 
      if(Process[i].isTag("lwidth") )hist->SetLineWidth  ((int)Process[i]["lwidth"].toDouble());// else hist->SetLineWidth  (1);
      if(Process[i].isTag("fill"  ) )hist->SetFillColor  ((int)Process[i]["fill"  ].toDouble());
      if(Process[i].isTag("marker") )hist->SetMarkerStyle((int)Process[i]["marker"].toDouble());// else hist->SetMarkerStyle(1);

      fixExtremities(hist,true,true);
      hist->SetTitle("");
      hist->SetStats(kFALSE);
      hist->SetMinimum(1E-3);
      hist->SetMaximum(1E6);
      ObjectToDelete.push_back(hist);

      if((!Process[i].isTag("spimpose") || !Process[i]["spimpose"].toBool()) && !Process[i]["isdata"].toBool()){
         //Add to Stack
         stack->Add(hist, "HIST");               
         legA->AddEntry(hist, Process[i]["tag"].c_str(), "F");
       }else{
          //Add to Canvas   
          if(stack){
             stack->Draw("");
             stack->GetXaxis()->SetTitle(hist->GetXaxis()->GetTitle());
             stack->GetYaxis()->SetTitle(hist->GetYaxis()->GetTitle());
             stack->SetMinimum(hist->GetMinimum());
             stack->SetMaximum(hist->GetMaximum());
             ObjectToDelete.push_back(stack);
             stack=NULL;
          }
          if(Process[i]["isdata"].toBool()){
             legA->AddEntry(hist, Process[i]["tag"].c_str(), "P");
             hist->Draw("E1 same");
          }else{
             legB->AddEntry(hist, Process[i]["tag"].c_str(), "L");
             hist->Draw("hist same");
          }
       }
   }

   TPaveText* T = new TPaveText(0.40,0.995,0.85,0.945, "NDC");
   T->SetFillColor(0);
   T->SetFillStyle(0);  T->SetLineColor(0);
   T->SetTextAlign(32);
  char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #int L=%.0f pb^{-1}", iLumi);
   T->AddText(Buffer);
   T->Draw("same");

   legA->SetFillColor(0); legA->SetFillStyle(0); legA->SetLineColor(0);
   legA->SetHeader("");
   legA->Draw("same");
   legB->SetFillColor(0); legB->SetFillStyle(0); legB->SetLineColor(0);
   legB->SetHeader("");
   legB->Draw("same");

   string SavePath = SaveName + ".png";
   while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
   while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
   while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
   while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
   while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
   while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
   while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
   while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"_");
   SavePath = outDir + SavePath;
   system(string(("rm -f ") + SavePath).c_str());
   c1->SaveAs(SavePath.c_str());
   delete c1;
   for(unsigned int d=0;d<ObjectToDelete.size();d++){delete ObjectToDelete[d];}ObjectToDelete.clear();
   delete legA;
   delete legB;
   delete T;
}

std::string toLatexRounded(double value, double error){
   if(value==0.0 && error==0.0)return string("");

   double power = floor(log10(value));
   if(power<=-3)     {power=power+3;
   }else if(power>=2){power=power-2;
   }else             {power=0;}       

   value = value / pow(10,power);
   error = error / pow(10,power);
   int ValueFloating = 1+std::max(-1*log10(error),0.0);
   int ErrorFloating = ValueFloating;

   char tmpchar[255];
   if(power!=0){
      sprintf(tmpchar,"$(%.*f\\pm%.*f)\\times 10^{%g}$",ValueFloating,value,ErrorFloating,error,power);
   }else{
      sprintf(tmpchar,"$%.*f\\pm%.*f$",ValueFloating,value,ErrorFloating,error);
   }
   return string(tmpchar);
}



void ConvertToTex(JSONWrapper::Object& Root, std::string RootDir, std::string HistoName){
   FILE* pFile = NULL;

   std::vector<TObject*> ObjectToDelete;
   TH1* stack = NULL; 
   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}

         TFile* File = new TFile((RootDir + (Samples[j])["dtag"].toString() + ".root").c_str());
         TH1* tmphist = (TH1*) GetObjectFromPath(File,HistoName); 
         if(j==0){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
         delete File;
      }  
 
      if(!pFile){
          string SavePath = string(hist->GetName()) + ".tex";
         while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
         while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
         while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
         while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
         while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
         while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
         while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
         while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"_");
         SavePath = outDir + SavePath;
         system(string(("rm -f ") + SavePath).c_str());
         pFile = fopen(SavePath.c_str(), "w");

         fprintf(pFile, "\\begin{table}[htp]\n");
         fprintf(pFile, "\\begin{center}\n");
         fprintf(pFile, "\\caption{}\n");
         fprintf(pFile, "\\label{tab:table}\n");

         string colfmt = "|l";  for(int b=1;b<=hist->GetXaxis()->GetNbins();b++){colfmt = colfmt + "c";} colfmt+="|";
         string colname = "";   for(int b=1;b<=hist->GetXaxis()->GetNbins();b++){
            std::string tempcolname =  hist->GetXaxis()->GetBinLabel(b);
            if(tempcolname.find("_")!=std::string::npos || tempcolname.find("^")!=std::string::npos)tempcolname = string("$") + tempcolname + "$";
            colname = colname + "& " + tempcolname;
         }
         fprintf(pFile, "\\begin{tabular}{ %s } \\hline\n", colfmt.c_str());
         fprintf(pFile, "Process %s \\\\ \\hline\\hline\n", colname.c_str());
      }
      ObjectToDelete.push_back(hist);

      std::string CleanTag = Process[i]["tag"].c_str();
      if(CleanTag.find("#")!=std::string::npos)CleanTag = string("$") + CleanTag + "$";
      while(CleanTag.find("#")!=std::string::npos)CleanTag.replace(CleanTag.find("#"),1,"\\");

      if((!Process[i].isTag("spimpose") || !Process[i]["spimpose"].toBool()) && !Process[i]["isdata"].toBool()){
         //Add to Stack
         if(!stack){stack = (TH1*)hist->Clone("Stack");}else{stack->Add(hist,1.0);}

         char numberastext[2048]; numberastext[0] = '\0';
         for(int b=1;b<=hist->GetXaxis()->GetNbins();b++){sprintf(numberastext,"%s & %s",numberastext, toLatexRounded(hist->GetBinContent(b), hist->GetBinError(b)).c_str());}
         fprintf(pFile, "%s %s \\\\\n",CleanTag.c_str(), numberastext);         
       }else{
          //Add to Canvas   
          if(stack){
            char numberastext[2048]; numberastext[0] = '\0';
            for(int b=1;b<=hist->GetXaxis()->GetNbins();b++){sprintf(numberastext,"%s & %s",numberastext, toLatexRounded(stack->GetBinContent(b), stack->GetBinError(b)).c_str());}
            fprintf(pFile, "%s %s \\\\\n\\hline\n","Total expected", numberastext);
             ObjectToDelete.push_back(stack);
             stack=NULL;
          }

          if(Process[i]["isdata"].toBool()){fprintf(pFile,"\\hline\n");}

          char numberastext[2048]; numberastext[0] = '\0';
          for(int b=1;b<=hist->GetXaxis()->GetNbins();b++){sprintf(numberastext,"%s & %s",numberastext, toLatexRounded(hist->GetBinContent(b), hist->GetBinError(b)).c_str());}
          fprintf(pFile, "%s %s \\\\\n",CleanTag.c_str(), numberastext);
       }
   }
   fprintf(pFile,"\\hline\n");
   fprintf(pFile,"\\end{tabular}\n");
   fprintf(pFile,"\\end{center}\n");
   fprintf(pFile,"\\end{table}\n");

   fclose(pFile);
   for(unsigned int d=0;d<ObjectToDelete.size();d++){delete ObjectToDelete[d];}ObjectToDelete.clear();
}


int main(int argc, char* argv[]){
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


   for(int i=0;i<argc;i++){
     string arg(argv[i]);
     if(arg.find("--help")!=string::npos){
        printf("--help   --> print this helping text\n");

        printf("--iLumi   --> integrated luminosity to be used for the MC rescale\n");
        printf("--inDir   --> path to the directory containing the .root files to process\n");
        printf("--outDir  --> path of the directory that will contains the output plots and tables\n");
        printf("--outFile --> path of the output summary .root file\n");
        printf("--json    --> containing list of process (and associated style) to process to process\n");
        printf("--only    --> processing only the objects containing the following argument in their name\n");

        printf("--no1D   --> Skip processing of 1D objects\n");
        printf("--no2D   --> Skip processing of 2D objects\n");
        printf("--noTex  --> Do not create latex table (when possible)\n");
        printf("--noRoot --> Do not make a summary .root file\n");
        printf("--noPlot --> Do not creates plot files (useful to speedup processing)\n");
     }

     if(arg.find("--iLumi"  )!=string::npos && i+1<argc){ sprintf(argv[i+1],"%f",iLumi); }

     if(arg.find("--inDir"  )!=string::npos && i+1<argc){ inDir    = argv[i+1];    }
     if(arg.find("--outDir" )!=string::npos && i+1<argc){ outDir   = argv[i+1];    }
     if(arg.find("--outFile")!=string::npos && i+1<argc){ outFile  = argv[i+1];    }
     if(arg.find("--json"   )!=string::npos && i+1<argc){ jsonFile = argv[i+1];    }
     if(arg.find("--only"   )!=string::npos && i+1<argc){ objectSearchKey = argv[i+1];    }

     if(arg.find("--no2D"  )!=string::npos){ do2D = false;    }
     if(arg.find("--no1D"  )!=string::npos){ do1D = false;    }
     if(arg.find("--noTex" )!=string::npos){ doTex= false;    }
     if(arg.find("--noRoot")!=string::npos){ StoreInFile = false;    }
     if(arg.find("--noPlot")!=string::npos){ doPlot = false;    }
   } 


   JSONWrapper::Object Root(jsonFile, true);
   TFile* File = new TFile((inDir + Root["proc"][0]["tag"].toString() + ".root").c_str());
   std::vector<NameAndType> histlist;
   GetListOfObject(File,histlist);
   delete File;

   TFile* OutputFile = NULL;
   if(StoreInFile) OutputFile = new TFile(outFile.c_str(),"RECREATE");
   printf("Progressing Bar              :0%%       20%%       40%%       60%%       80%%       100%%\n");
   printf("                             :");
   int TreeStep = histlist.size()/50;if(TreeStep==0)TreeStep=1;
   for(unsigned int i=0;i<histlist.size();i++){
      if(i%TreeStep==0){printf(".");fflush(stdout);}
      if(objectSearchKey != "" && histlist[i].Name.find(objectSearchKey)==std::string::npos)continue;

        if(doTex && histlist[i].Name.find("eventflow")!=std::string::npos){    ConvertToTex(Root,inDir,histlist[i].Name);     }
        if(doPlot && do2D  && !histlist[i].isTH1){                                       Draw2DHistogram(Root,inDir,histlist[i].Name);  }
        if(doPlot && do1D  &   histlist[i].isTH1){                                       Draw1DHistogram(Root,inDir,histlist[i].Name);  }

        if(StoreInFile && do2D  && !histlist[i].isTH1){                                  SavingToFile(Root,inDir,histlist[i].Name, OutputFile); }
        if(StoreInFile && do1D  &&  histlist[i].isTH1){                                  SavingToFile(Root,inDir,histlist[i].Name, OutputFile); }
   }printf("\n");
   if(StoreInFile) OutputFile->Close();
}

