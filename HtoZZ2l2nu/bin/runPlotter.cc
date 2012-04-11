#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

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

int cutIndex=-1;
string cutIndexStr="";
double iLumi = 2007;
bool do2D  = true;
bool do1D  = true;
bool doTex = true;
bool StoreInFile = true;
bool doPlot = true;
bool splitCanvas = false;
bool onlyCutIndex = false;
string objectSearchKey = "";
string inDir   = "OUTNew/";
string jsonFile = "../../data/beauty-samples.json";
string outDir  = "Img/";
string plotExt = ".png";
string outFile = "plotter.root";
string cutflowhisto = "all_cutflow";

std::map<string, double> PURescale_up;
std::map<string, double> PURescale_down;
std::map<string, double> initialNumberOfEvents;
std::map<string, bool>   FileExist;

//typedef std::pair<std::string,bool> NameAndType;
struct NameAndType{
   std::string name;
   bool type; 
   bool cutIndex;
   NameAndType(std::string name_,  bool type_, bool cutIndex_){name = name_; type = type_; cutIndex = cutIndex_;}

   bool operator==(const NameAndType& a){ return a.name == name;}
   bool operator< (const NameAndType& a){ return a.name < name;}
 };



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
//         if(!TMP2)printf("BUG: %s\n",Path.c_str());
         return TMP2;
      }

//      printf("BUG: %s\n",Path.c_str());
      return NULL;
   }else{
      TObject* TMP = File->Get(Path.c_str());
//      if(!TMP)printf("BUG: %s\n",Path.c_str());

      if(GetACopy){
         return TMP->Clone();
      }else{
         return TMP;
      }
   }

}

void GetListOfObject(JSONWrapper::Object& Root, std::string RootDir, std::list<NameAndType>& histlist, TDirectory* dir=NULL, std::string parentPath=""){

  if(parentPath=="" && !dir)
    {
      std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
      for(size_t ip=0; ip<Process.size(); ip++)
	{
	  std::vector<JSONWrapper::Object> Samples = (Process[ip])["data"].daughters();
	  for(size_t id=0; id<Samples.size(); id++)
	    {
	      int split = 1;
	      if(Samples[id].isTag("split"))split = Samples[id]["split"].toInt();
	      char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",0);}else{sprintf(segmentExt,".root");}
	      TFile* file = new TFile((RootDir + (Samples[id])["dtag"].toString() + segmentExt).c_str());
	      GetListOfObject(Root,RootDir,histlist,(TDirectory*)file,"" );
	      file->Close();
	    }
	}
      return;
   }

   if(dir==NULL)return;

   TList* list = dir->GetListOfKeys();
   for(int i=0;i<list->GetSize();i++){
      TObject* tmp = GetObjectFromPath(dir,list->At(i)->GetName(),false);
      if(tmp->InheritsFrom("TTree")) continue;
//      if(tmp->InheritsFrom("TH1")){
//         histlist.push_back(NameAndType(parentPath+list->At(i)->GetName(), !(tmp->InheritsFrom("TH2") || tmp->InheritsFrom("TH3")) ) );
//      }else if(tmp->InheritsFrom("TDirectory")){
//         GetListOfObject(Root,RootDir,histlist,(TDirectory*)tmp,parentPath+ list->At(i)->GetName()+"/" );
//      }


      if(tmp->InheritsFrom("TDirectory")){
         GetListOfObject(Root,RootDir,histlist,(TDirectory*)tmp,parentPath+ list->At(i)->GetName()+"/" );
      }else if(tmp->InheritsFrom("TH1")){
        bool isTH1 = !(tmp->InheritsFrom("TH2") || tmp->InheritsFrom("TH3"));
        bool hasIndex = string(((TH1*)tmp)->GetXaxis()->GetTitle()) =="cut index";
        if(hasIndex && !isTH1){isTH1=true;}
	histlist.push_back(NameAndType(parentPath+list->At(i)->GetName(), isTH1, hasIndex ) );
      }

      delete tmp;
   }
   

   

}

void GetInitialNumberOfEvents(JSONWrapper::Object& Root, std::string RootDir, NameAndType HistoProperties){
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
            TH1* tmptmphist = (TH1*) GetObjectFromPath(File,HistoProperties.name); 
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
         //if(isMC)printf("PU Renormalization %25s Shift Down --> %6.2f  Central = %6.2f  Up Down --> %6.2f\n",(Samples[j])["dtag"].toString().c_str(),PUDownnorm, PUCentralnnorm, PUUpnorm);	

         double cnorm = 1.0;
         if(tmphist)cnorm = tmphist->GetBinContent(1);
         if(cnorm<=0 || !isMC)cnorm = 1.0;         
         if(cnorm==1 && isMC)printf("is there a problem with %s ? cnorm = %f\n",(Samples[j])["dtag"].toString().c_str(), cnorm);          
         if(!isMC)PUCentralnnorm = 1;
         initialNumberOfEvents[(Samples[j])["dtag"].toString()] = cnorm / PUCentralnnorm;

         delete tmphist;
      }   
   }
}



void SavingToFile(JSONWrapper::Object& Root, std::string RootDir, NameAndType HistoProperties, TFile* OutputFile){
   std::vector<TObject*> ObjectToDelete;

   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool() && !Process[i]["isdatadriven"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
         Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()];

         if(HistoProperties.name.find("puup"  )!=string::npos){Weight *= PURescale_up  [(Samples[j])["dtag"].toString()];}
         if(HistoProperties.name.find("pudown")!=string::npos){Weight *= PURescale_down[(Samples[j])["dtag"].toString()];}



         if(HistoProperties.name.find("optim_cut")!=string::npos){Weight=1.0;}

         int split = 1;
         if(Samples[j].isTag("split"))split = Samples[j]["split"].toInt();
         TH1* tmphist = NULL;
         for(int s=0;s<split;s++){
            char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",s);}else{sprintf(segmentExt,".root");}

            string FileName = RootDir + (Samples[j])["dtag"].toString() + segmentExt;
            if(!FileExist[FileName])continue;
            TFile* File = new TFile(FileName.c_str());
            if(!File || File->IsZombie() || !File->IsOpen() || File->TestBit(TFile::kRecovered) )continue;
            TH1* tmptmphist = (TH1*) GetObjectFromPath(File,HistoProperties.name); 
            if(!tmptmphist){delete File;continue;}
            if(!tmphist){gROOT->cd(); tmphist = (TH1*)tmptmphist->Clone(tmptmphist->GetName());}else{tmphist->Add(tmptmphist);}
            delete tmptmphist;
            delete File;
         }
         if(!tmphist)continue;
         if(!hist){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
      }   
      if(!hist)continue;

      string dirName = Process[i]["tag"].c_str();while(dirName.find("/")!=std::string::npos)dirName.replace(dirName.find("/"),1,"-");
      OutputFile->cd();
      TDirectory* subdir = OutputFile->GetDirectory(dirName.c_str());
      if(!subdir || subdir==OutputFile) subdir = OutputFile->mkdir(dirName.c_str());
      subdir->cd();
      hist->Write();
      delete hist;
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


void Draw2DHistogramSplitCanvas(JSONWrapper::Object& Root, std::string RootDir, NameAndType HistoProperties){
   if(HistoProperties.cutIndex && cutIndex<0)return;

   std::string SaveName = "";

   TPaveText* T = new TPaveText(0.40,0.995,0.85,0.945, "NDC");
   T->SetFillColor(0);
   T->SetFillStyle(0);  T->SetLineColor(0);
   T->SetTextAlign(32);
  char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #int L=%.1f fb^{-1}", iLumi/1000);
   T->AddText(Buffer);

   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   std::vector<TObject*> ObjectToDelete;
   for(unsigned int i=0;i<Process.size();i++){
      if(Process[i]["isinvisible"].toBool())continue;

      TCanvas* c1 = new TCanvas("c1","c1",500,500);
      c1->SetLogz(true);

      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool()  && !Process[i]["isdatadriven"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
         Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()];

         if(HistoProperties.name.find("puup"  )!=string::npos){Weight *= PURescale_up  [(Samples[j])["dtag"].toString()];}
         if(HistoProperties.name.find("pudown")!=string::npos){Weight *= PURescale_down[(Samples[j])["dtag"].toString()];}

         int split = 1;
         if(Samples[j].isTag("split"))split = Samples[j]["split"].toInt();
         TH1* tmphist = NULL;
         for(int s=0;s<split;s++){
            char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",s);}else{sprintf(segmentExt,".root");}

            string FileName = RootDir + (Samples[j])["dtag"].toString() + segmentExt;
            if(!FileExist[FileName])continue;
            TFile* File = new TFile(FileName.c_str());
            if(!File || File->IsZombie() || !File->IsOpen() || File->TestBit(TFile::kRecovered) )continue;
            TH1* tmptmphist = (TH1*) GetObjectFromPath(File,HistoProperties.name); 
            if(!tmptmphist){delete File;continue;}
            if(!tmphist){gROOT->cd(); tmphist = (TH1*)tmptmphist->Clone(tmptmphist->GetName());}else{tmphist->Add(tmptmphist);}
            //if(Process[i]["isdata"].toBool())printf("%s --> %f*%f(%f)\n",(Samples[j])["dtag"].toString().c_str(), tmptmphist->Integral(),Weight, initialNumberOfEvents[(Samples[j])["dtag"].toString()]);
            delete tmptmphist;
            delete File;
         }
         if(!tmphist)continue;
         if(!hist){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
      }   
      if(!hist)continue;

      SaveName = hist->GetName();
      ObjectToDelete.push_back(hist);
      hist->SetTitle("");
      hist->SetStats(kFALSE);

      hist->Draw("COLZ");
  
      TPaveText* leg = new TPaveText(0.20,0.95,0.40,0.80, "NDC");
      leg->SetFillColor(0);
      leg->SetFillStyle(0);  leg->SetLineColor(0);
      leg->SetTextAlign(12);
      leg->AddText(Process[i]["tag"].c_str());
      leg->Draw("same");
      ObjectToDelete.push_back(leg);
//      delete leg;
//      delete hist;

      T->Draw("same");

      string SavePath = SaveName + "_" + (Process[i])["tag"].toString() + plotExt;
      while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
      while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
      while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
      while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
      while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
      while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
      while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
      while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"-");
      if(outDir.size()) SavePath = outDir +"/"+ SavePath;
      system(string(("rm -f ") + SavePath).c_str());
      c1->SaveAs(SavePath.c_str());
      delete c1;
   }

   for(unsigned int d=0;d<ObjectToDelete.size();d++){delete ObjectToDelete[d];}ObjectToDelete.clear();
   delete T;
}


void Draw2DHistogram(JSONWrapper::Object& Root, std::string RootDir, NameAndType HistoProperties){
   if(HistoProperties.cutIndex && cutIndex<0)return;

   std::string SaveName = "";

   TPaveText* T = new TPaveText(0.40,0.995,0.85,0.945, "NDC");
   T->SetFillColor(0);
   T->SetFillStyle(0);  T->SetLineColor(0);
   T->SetTextAlign(32);
  char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #int L=%.1f fb^{-1}", iLumi/1000);
   T->AddText(Buffer);

   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   int NSampleToDraw = 0;
   for(unsigned int i=0;i<Process.size();i++){
      if(Process[i]["isinvisible"].toBool())continue;
      NSampleToDraw++;
   }
   int CanvasX = 4;
   int CanvasY = ceil(NSampleToDraw/CanvasX);
   TCanvas* c1 = new TCanvas("c1","c1",1000,1000);
   c1->Divide(CanvasX,CanvasY,0,0);


   std::vector<TObject*> ObjectToDelete;
   for(unsigned int i=0;i<Process.size();i++){
      if(Process[i]["isinvisible"].toBool())continue;

      TVirtualPad* pad = c1->cd(i+1);
      pad->SetLogz(true);
      pad->SetTopMargin(0.0); pad->SetBottomMargin(0.10);  pad->SetRightMargin(0.20);

      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool()  && !Process[i]["isdatadriven"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
         Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()];

         if(HistoProperties.name.find("puup"  )!=string::npos){Weight *= PURescale_up  [(Samples[j])["dtag"].toString()];}
         if(HistoProperties.name.find("pudown")!=string::npos){Weight *= PURescale_down[(Samples[j])["dtag"].toString()];}

         int split = 1;
         if(Samples[j].isTag("split"))split = Samples[j]["split"].toInt();
         TH1* tmphist = NULL;
         for(int s=0;s<split;s++){
            char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",s);}else{sprintf(segmentExt,".root");}

            string FileName = RootDir + (Samples[j])["dtag"].toString() + segmentExt;
            if(!FileExist[FileName])continue;
            TFile* File = new TFile(FileName.c_str());
            if(!File || File->IsZombie() || !File->IsOpen() || File->TestBit(TFile::kRecovered) )continue;
            TH1* tmptmphist = (TH1*) GetObjectFromPath(File,HistoProperties.name); 
            if(!tmptmphist){delete File;continue;}
            if(!tmphist){gROOT->cd(); tmphist = (TH1*)tmptmphist->Clone(tmptmphist->GetName());}else{tmphist->Add(tmptmphist);}
            //if(Process[i]["isdata"].toBool())printf("%s --> %f\n",(Samples[j])["dtag"].toString().c_str(), tmptmphist->Integral());
            delete tmptmphist;
            delete File;
         }
         if(!tmphist)continue;
         if(!hist){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
      }   
      if(!hist)continue;

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
   string SavePath = SaveName + plotExt;
   while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
   while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
   while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
   while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
   while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
   while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
   while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
   while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"-");
   if(outDir.size()) SavePath = outDir +"/"+ SavePath; 
   system(string(("rm -f ") + SavePath).c_str());
   c1->SaveAs(SavePath.c_str());
   for(unsigned int d=0;d<ObjectToDelete.size();d++){delete ObjectToDelete[d];}ObjectToDelete.clear();
   delete c1;
   delete T;
}


void Draw1DHistogram(JSONWrapper::Object& Root, std::string RootDir, NameAndType HistoProperties){
   if(HistoProperties.cutIndex && cutIndex<0)return;

   TCanvas* c1 = new TCanvas("c1","c1",800,800);
   TPad* t1 = new TPad("t1","t1", 0.0, 0.20, 1.0, 1.0);
   t1->Draw();
   t1->cd();
   t1->SetLogy(true);

   TLegend* legA  = new TLegend(0.845,0.2,0.99,0.99, "NDC"); 
   //   TLegend* legA  = new TLegend(0.51,0.93,0.67,0.75, "NDC"); 
   // TLegend* legB  = new TLegend(0.67,0.93,0.83,0.75, "NDC");
   THStack* stack = new THStack("MC","MC");
   TH1*     mc   = NULL;
   std::vector<TH1 *> spimpose;
   std::vector<TString> spimposeOpts;
   TH1*     data = NULL;
   std::vector<TObject*> ObjectToDelete;


   std::string SaveName = "";
   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      if(Process[i]["isinvisible"].toBool())continue;
      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();
      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool() && !Process[i]["isdatadriven"].toBool() )Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
         Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()];

         if(HistoProperties.name.find("puup"  )!=string::npos){Weight *= PURescale_up  [(Samples[j])["dtag"].toString()];}
         if(HistoProperties.name.find("pudown")!=string::npos){Weight *= PURescale_down[(Samples[j])["dtag"].toString()];}

         if(HistoProperties.name.find("optim_cut")!=string::npos){Weight=1.0;}

         int split = 1; 
         if(Samples[j].isTag("split"))split = Samples[j]["split"].toInt();
         TH1* tmphist = NULL;
         for(int s=0;s<split;s++){
            char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",s);}else{sprintf(segmentExt,".root");}

	    string FileName = RootDir + (Samples[j])["dtag"].toString() + segmentExt;
            if(!FileExist[FileName])continue;
            TFile* File = new TFile(FileName.c_str());
            if(!File || File->IsZombie() || !File->IsOpen() || File->TestBit(TFile::kRecovered) )continue;
            TH1* tmptmphist = NULL; 
            if(HistoProperties.cutIndex && cutIndex>=0){
               TH2* tmp2D = (TH2*) GetObjectFromPath(File,HistoProperties.name);
               if(tmp2D){tmptmphist = tmp2D->ProjectionY((string(tmp2D->GetName())+cutIndexStr).c_str(),cutIndex,cutIndex); delete tmp2D;}
            }else if(!HistoProperties.cutIndex){
               tmptmphist = (TH1*) GetObjectFromPath(File,HistoProperties.name);
            }
	    if(!tmptmphist){delete File;continue;}
            if(!tmphist){gROOT->cd(); tmphist = (TH1*)tmptmphist->Clone(tmptmphist->GetName());}else{tmphist->Add(tmptmphist);}
            //if(Process[i]["isdata"].toBool())printf("%s --> %f\n",(Samples[j])["dtag"].toString().c_str(), tmptmphist->Integral());
            delete tmptmphist;
            delete File;
         }
         if(!tmphist)continue;
         if(!hist){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
      }   
      if(!hist)continue;

      SaveName = hist->GetName();
      if(Process[i].isTag("color" ) )hist->SetLineColor  ((int)Process[i][ "color"].toDouble()); else hist->SetLineColor  (1);
      if(Process[i].isTag("color" ) )hist->SetMarkerColor((int)Process[i][ "color"].toDouble()); else hist->SetMarkerColor(1);
      if(Process[i].isTag("color" ) )hist->SetFillColor  ((int)Process[i][ "color"].toDouble()); else hist->SetFillColor  (0);
      if(Process[i].isTag("lcolor") )hist->SetLineColor  ((int)Process[i]["lcolor"].toDouble());
      if(Process[i].isTag("mcolor") )hist->SetMarkerColor((int)Process[i]["mcolor"].toDouble()); 
      if(Process[i].isTag("fcolor") )hist->SetFillColor  ((int)Process[i]["fcolor"].toDouble()); 
      if(Process[i].isTag("lwidth") )hist->SetLineWidth  ((int)Process[i]["lwidth"].toDouble());// else hist->SetLineWidth  (1);
      if(Process[i].isTag("lstyle") )hist->SetLineStyle  ((int)Process[i]["lstyle"].toDouble());// else hist->SetLinStyle  (1);
      if(Process[i].isTag("fill"  ) )hist->SetFillColor  ((int)Process[i]["fill"  ].toDouble());
      if(Process[i].isTag("marker") )hist->SetMarkerStyle((int)Process[i]["marker"].toDouble());// else hist->SetMarkerStyle(1);

      fixExtremities(hist,true,true);
      hist->SetTitle("");
      hist->SetStats(kFALSE);
      hist->SetMinimum(1E-3);
      //hist->SetMaximum(1E6);
      hist->SetMaximum(hist->GetBinContent(hist->GetMaximumBin())*1.10);
      ObjectToDelete.push_back(hist);

      if((!Process[i].isTag("spimpose") || !Process[i]["spimpose"].toBool()) && !Process[i]["isdata"].toBool()){
         //Add to Stack
	stack->Add(hist, "HIST");               
         legA->AddEntry(hist, Process[i]["tag"].c_str(), "F");	 
         if(!mc){mc = (TH1D*)hist->Clone("mc");}else{mc->Add(hist);}
      }
      else if(Process[i].isTag("spimpose") && Process[i]["spimpose"].toBool())
	{
	  //legB->AddEntry(hist, Process[i]["tag"].c_str(), "L");
	  legA->AddEntry(hist, Process[i]["tag"].c_str(), Process[i]["isdata"].toBool() ? "L" : "P" );
	  spimposeOpts.push_back( Process[i]["isdata"].toBool() ? "e1" : "hist" );
	  spimpose.push_back(hist);
	}
      else{
	if(Process[i]["isdata"].toBool()){
	  if(!data){
	    data = hist; 
	    legA->AddEntry(hist, Process[i]["tag"].c_str(), "P"); 
	  }
	  else data->Add(hist);
	}
      }
   }

   bool canvasIsFilled(false);
   if(stack && stack->GetStack() && stack->GetStack()->GetEntriesFast()>0){
     stack->Draw("");
     TH1 *hist=(TH1*)stack->GetStack()->At(0);
     stack->GetXaxis()->SetTitle(hist->GetXaxis()->GetTitle());
     stack->GetYaxis()->SetTitle(hist->GetYaxis()->GetTitle());
     stack->SetMinimum(hist->GetMinimum());
     //stack->SetMaximum(hist->GetMaximum());
     ObjectToDelete.push_back(stack);
     canvasIsFilled=true;
   }
   if(data){
       data->Draw(canvasIsFilled ? "E1 same" : "E1");
       canvasIsFilled=true;
   }
   for(size_t ip=0; ip<spimpose.size(); ip++){
     TString opt=spimposeOpts[ip];
     spimpose[ip]->Draw(opt + (canvasIsFilled ? "same": "") );
     canvasIsFilled=true;
   }

   TPaveText* T = new TPaveText(0.1,0.995,0.84,0.95, "NDC");
   T->SetFillColor(0);
   T->SetFillStyle(0);  T->SetLineColor(0);
   T->SetTextAlign(22);
   char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", iLumi/1000);
   T->AddText(Buffer);
   T->Draw("same");

   
   legA->SetFillColor(0); legA->SetFillStyle(0); legA->SetLineColor(0);
   legA->SetHeader("");
   legA->Draw("same");
   legA->SetTextFont(42);
//    legB->SetFillColor(0); legB->SetFillStyle(0); legB->SetLineColor(0);
//    legB->SetHeader("");
//    legB->Draw("same");
//   legB->SetTextFont(42);


   if(data && mc){
   c1->cd();
   TPad* t2 = new TPad("t2","t2", 0.0, 0.0, 1.0, 0.2);
   t2->Draw();
   t2->cd();
   t2->SetGridy(true);
   t2->SetPad(0,0.0,1.0,0.2);
   t2->SetTopMargin(0);
   t2->SetBottomMargin(0.5);
   float yscale = (1.0-0.2)/(0.18-0);
   data = (TH1D*)data->Clone("RatioHistogram");
   data->Divide(mc);
   data->GetYaxis()->SetTitle("Obs/Ref");
   data->GetXaxis()->SetTitle("");
   //data->SetMinimum(0);
   //data->SetMaximum(data->GetBinContent(data->GetMaximumBin())*1.10);
   data->SetMinimum(0);
   data->SetMaximum(2.2);
   data->GetXaxis()->SetTitleOffset(1.3);
   data->GetXaxis()->SetLabelSize(0.033*yscale);
   data->GetXaxis()->SetTitleSize(0.036*yscale);
   data->GetXaxis()->SetTickLength(0.03*yscale);
   data->GetYaxis()->SetTitleOffset(0.3);
   data->GetYaxis()->SetNdivisions(5);
   data->GetYaxis()->SetLabelSize(0.033*yscale);
   data->GetYaxis()->SetTitleSize(0.036*yscale);
   
   data->Draw("E1");
   }


   c1->cd();
   string SavePath = SaveName + plotExt;
   while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
   while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
   while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
   while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
   while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
   while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
   while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
   while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"-");
   if(outDir.size()) SavePath = outDir +"/"+ SavePath;
   system(string(("rm -f ") + SavePath).c_str());
   c1->SaveAs(SavePath.c_str());
   delete c1;
   for(unsigned int d=0;d<ObjectToDelete.size();d++){delete ObjectToDelete[d];}ObjectToDelete.clear();
   delete legA;
   //   delete legB;
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



void ConvertToTex(JSONWrapper::Object& Root, std::string RootDir, NameAndType HistoProperties){
   if(HistoProperties.cutIndex && cutIndex<0)return;

   FILE* pFile = NULL;

   std::vector<TObject*> ObjectToDelete;
   TH1* stack = NULL; 
   std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
   for(unsigned int i=0;i<Process.size();i++){
      TH1* hist = NULL;
      std::vector<JSONWrapper::Object> Samples = (Process[i])["data"].daughters();




      for(unsigned int j=0;j<Samples.size();j++){
         double Weight = 1.0;
         if(!Process[i]["isdata"].toBool()  && !Process[i]["isdatadriven"].toBool())Weight*= iLumi;
         if(Samples[j].isTag("xsec")     )Weight*= Samples[j]["xsec"].toDouble();
         std::vector<JSONWrapper::Object> BR = Samples[j]["br"].daughters();for(unsigned int b=0;b<BR.size();b++){Weight*=BR[b].toDouble();}
         Weight /= initialNumberOfEvents[(Samples[j])["dtag"].toString()];

         if(HistoProperties.name.find("puup"  )!=string::npos){Weight *= PURescale_up  [(Samples[j])["dtag"].toString()];}
         if(HistoProperties.name.find("pudown")!=string::npos){Weight *= PURescale_down[(Samples[j])["dtag"].toString()];}


         int split = 1;
         if(Samples[j].isTag("split"))split = Samples[j]["split"].toInt();
         TH1* tmphist = NULL;
         for(int s=0;s<split;s++){
            char segmentExt[255];if(split>1){sprintf(segmentExt,"_%i.root",s);}else{sprintf(segmentExt,".root");}

            string FileName = RootDir + (Samples[j])["dtag"].toString() + segmentExt;
            if(!FileExist[FileName])continue;
            TFile* File = new TFile(FileName.c_str());
            if(!File || File->IsZombie() || !File->IsOpen() || File->TestBit(TFile::kRecovered) )continue;
            TH1* tmptmphist = NULL;
            if(HistoProperties.cutIndex && cutIndex>=0){
               TH2* tmp2D = (TH2*) GetObjectFromPath(File,HistoProperties.name);
               if(tmp2D){tmptmphist = tmp2D->ProjectionY("_py",cutIndex,cutIndex); delete tmp2D;}
            }else if(!HistoProperties.cutIndex){
               tmptmphist = (TH1*) GetObjectFromPath(File,HistoProperties.name);
            }
            if(!tmptmphist){delete File;continue;}
            if(!tmphist){gROOT->cd(); tmphist = (TH1*)tmptmphist->Clone(tmptmphist->GetName());}else{tmphist->Add(tmptmphist);}
            delete tmptmphist;
            delete File;
         }
         if(!tmphist)continue;
         if(!hist){gROOT->cd(); hist = (TH1*)tmphist->Clone(tmphist->GetName());hist->Scale(Weight);}else{hist->Add(tmphist,Weight);}
         delete tmphist;
      }
      if(!hist)continue;

      if(!pFile){
         string SavePath = string(hist->GetName()) + ".tex";
         while(SavePath.find("*")!=std::string::npos)SavePath.replace(SavePath.find("*"),1,"");
         while(SavePath.find("#")!=std::string::npos)SavePath.replace(SavePath.find("#"),1,"");
         while(SavePath.find("{")!=std::string::npos)SavePath.replace(SavePath.find("{"),1,"");
         while(SavePath.find("}")!=std::string::npos)SavePath.replace(SavePath.find("}"),1,"");
         while(SavePath.find("(")!=std::string::npos)SavePath.replace(SavePath.find("("),1,"");
         while(SavePath.find(")")!=std::string::npos)SavePath.replace(SavePath.find(")"),1,"");
         while(SavePath.find("^")!=std::string::npos)SavePath.replace(SavePath.find("^"),1,"");
         while(SavePath.find("/")!=std::string::npos)SavePath.replace(SavePath.find("/"),1,"-");
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

   if(pFile){
      fprintf(pFile,"\\hline\n");
      fprintf(pFile,"\\end{tabular}\n");
      fprintf(pFile,"\\end{center}\n");
      fprintf(pFile,"\\end{table}\n");
      fclose(pFile);
   }
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

   for(int i=1;i<argc;i++){
     string arg(argv[i]);
     //printf("--- %i - %s\n",i,argv[i]);

     if(arg.find("--help")!=string::npos){
        printf("--help   --> print this helping text\n");

        printf("--iLumi   --> integrated luminosity to be used for the MC rescale\n");
        printf("--inDir   --> path to the directory containing the .root files to process\n");
        printf("--outDir  --> path of the directory that will contains the output plots and tables\n");
        printf("--outFile --> path of the output summary .root file\n");
        printf("--json    --> containing list of process (and associated style) to process to process\n");
        printf("--only    --> processing only the objects containing the following argument in their name\n");
        printf("--index   --> will do the projection on that index for histos of type cutIndex\n");


        printf("--no1D   --> Skip processing of 1D objects\n");
        printf("--no2D   --> Skip processing of 2D objects\n");
        printf("--noTex  --> Do not create latex table (when possible)\n");
        printf("--noRoot --> Do not make a summary .root file\n");
        printf("--noPlot --> Do not creates plot files (useful to speedup processing)\n");
	printf("--plotExt --> extension to save\n");
	printf("--cutflow --> name of the histogram with the original number of events (cutflow by default)\n");
        printf("--splitCanvas --> (only for 2D plots) save all the samples in separated pltos\n");

        printf("command line example: runPlotter --json ../data/beauty-samples.json --iLumi 2007 --inDir OUT/ --outDir OUT/plots/ --outFile plotter.root --noRoot --noPlot\n");
	return 0;
     }

     if(arg.find("--iLumi"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%lf",&iLumi); i++; printf("Lumi = %f\n", iLumi); }

     if(arg.find("--inDir"  )!=string::npos && i+1<argc){ inDir    = argv[i+1];  i++;  printf("inDir = %s\n", inDir.c_str());  }
     if(arg.find("--outDir" )!=string::npos && i+1<argc){ outDir   = argv[i+1];  i++;  printf("outDir = %s\n", outDir.c_str());  }
     if(arg.find("--outFile")!=string::npos && i+1<argc){ outFile  = argv[i+1];  i++; printf("output file = %s\n", outFile.c_str()); }
     if(arg.find("--json"   )!=string::npos && i+1<argc){ jsonFile = argv[i+1];  i++;  }
     if(arg.find("--only"   )!=string::npos && i+1<argc){ objectSearchKey = argv[i+1]; i++;    }
     if(arg.find("--index"  )!=string::npos && i+1<argc){ sscanf(argv[i+1],"%i",&cutIndex); i++; onlyCutIndex=(cutIndex>=0); printf("index = %i\n", cutIndex);  }

     if(arg.find("--no2D"  )!=string::npos){ do2D = false;    }
     if(arg.find("--no1D"  )!=string::npos){ do1D = false;    }
     if(arg.find("--noTex" )!=string::npos){ doTex= false;    }
     if(arg.find("--noRoot")!=string::npos){ StoreInFile = false;    }
     if(arg.find("--noPlot")!=string::npos){ doPlot = false;    }
     if(arg.find("--plotExt" )!=string::npos && i+1<argc){ plotExt   = argv[i+1];  i++;  printf("saving plots as = %s\n", plotExt.c_str());  }
     if(arg.find("--cutflow" )!=string::npos && i+1<argc){ cutflowhisto   = argv[i+1];  i++;  printf("Normalizing from 1st bin in = %s\n", cutflowhisto.c_str());  }
     if(arg.find("--splitCanvas")!=string::npos){ splitCanvas = true;    }
   } 
   system( (string("mkdir -p ") + outDir).c_str());

   char buf[255];
   sprintf(buf, "_Index%i", cutIndex);
   cutIndexStr = buf;


   JSONWrapper::Object Root(jsonFile, true);
   GetInitialNumberOfEvents(Root,inDir,NameAndType(cutflowhisto,true, false));  //Used to get the rescale factor based on the total number of events geenrated

   std::list<NameAndType> histlist;
   GetListOfObject(Root,inDir,histlist);
   histlist.sort();
   histlist.unique();


   TFile* OutputFile = NULL;
   if(StoreInFile) OutputFile = new TFile(outFile.c_str(),"RECREATE");
   printf("Progressing Bar              :0%%       20%%       40%%       60%%       80%%       100%%\n");
   printf("                             :");
   int TreeStep = histlist.size()/50;if(TreeStep==0)TreeStep=1;
   system("echo \"\" > /tmp/histlist.csv");
   int ictr(0);
   for(std::list<NameAndType>::iterator it= histlist.begin(); it!= histlist.end(); it++,ictr++)
     {
       if(ictr%TreeStep==0){printf(".");fflush(stdout);}
       if(objectSearchKey != "" && it->name.find(objectSearchKey)==std::string::npos)continue;
       system(("echo \"" + it->name + "\" >> /tmp/histlist.csv").c_str());
       if(doTex && it->name.find("eventflow")!=std::string::npos && it->name.find("optim_eventflow")==std::string::npos){    ConvertToTex(Root,inDir,*it); }
       if(doPlot && do2D  && !it->type){                      if(!splitCanvas){Draw2DHistogram(Root,inDir,*it); }else{Draw2DHistogramSplitCanvas(Root,inDir,*it);}}
       if(doPlot && do1D  &&  it->type){                                       Draw1DHistogram(Root,inDir,*it); }
      
       if(StoreInFile && do2D  && !it->type){                                  SavingToFile(Root,inDir,*it, OutputFile); }
       if(StoreInFile && do1D  &&  it->type){                                  SavingToFile(Root,inDir,*it, OutputFile); }
     }printf("\n");
   if(StoreInFile) OutputFile->Close();
   
   system(("python ${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/data/html/generateJSONplotterFromList.py -i /tmp/histlist.csv -o "+outDir+"/plotter.json").c_str());
   system("rm /tmp/histlist.csv");
   system(("cp ${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/data/html/index.html " + outDir).c_str());
   printf("You can browse the results using %s/index.html\n",outDir.c_str());
}

