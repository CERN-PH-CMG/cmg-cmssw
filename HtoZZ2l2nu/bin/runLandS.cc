#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/src/tdrstyle.C"
#include "CMGTools/HtoZZ2l2nu/src/JSONWrapper.cc"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TString.h"
#include "TList.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "THStack.h"

#include<iostream>
#include<fstream>
#include<map>
#include<algorithm>
#include<vector>
#include<set>



using namespace std;
double NonResonnantSyst = 0.25;
double GammaJetSyst = 1.0;

//wrapper for a projected shape for a given set of cuts
struct Shape_t
{
  TH1* data, *totalBckg;
  std::vector<TH1 *> bckg, signal;
  //the key corresponds to the proc name
  //the key is the name of the variation: e.g. jesup, jesdown, etc.
  std::map<TString,std::vector<std::pair<TString, TH1*> > > bckgVars, signalVars;

  std::map<TString, double> xsections;

};

typedef std::pair<TString,TString> RateKey_t;
struct DataCardInputs
{
  TString shapesFile;
  std::vector<TString> ch;
  std::vector<TString> procs;
  std::map<RateKey_t, Double_t> obs;
  std::map<RateKey_t, Double_t> rates;
  std::map<TString, std::map<RateKey_t,Double_t> > systs;
  int nsignalproc;
};


void printHelp();
Shape_t getShapeFromFile(TFile* inF, TString ch, TString shapeName, int cutBin,JSONWrapper::Object &Root);
void showShape(std::vector<TString>& selCh ,map<TString, Shape_t>& allShapes, TString mainHisto, TString SaveName);
void getYieldsFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName);

void convertHistosForLimits_core(DataCardInputs& dci, TString& proc, TString& bin, TString& ch, std::vector<TString>& systs, std::vector<TH1*>& hshapes);
DataCardInputs convertHistosForLimits(Int_t mass,TString histo="finalmt",TString url="plotter.root",TString Json="");
std::vector<TString> buildDataCard(Int_t mass, TString histo="finalmt", TString url="plotter.root",TString Json="");
void doBackgroundSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t> &allShapes, TString mainHisto, TString sideBandHisto);
void doDYReplacement(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString metHistoForRescale);
void doWZSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString sideBandHisto);
void BlindData(std::vector<TString>& selCh, map<TString, Shape_t>& allShapes, TString mainHisto);

//void SignalInterpolation(std::vector<TString>& selCh, map<TString, Shape_t>& allShapes, TString mainHisto, JSONWrapper::Object &Root);
void SignalInterpolation(std::vector<TString>& selCh,map<TString, Shape_t>& allShapesL, map<TString, Shape_t>& allShapes, map<TString, Shape_t>& allShapesR, TString mainHisto);


void setTGraph(TString proc, TString suffix);
void initializeTGraph();

TGraph *ggH7TG_xsec=NULL, *ggH7TG_errp=NULL, *ggH7TG_errm=NULL, *ggH7TG_scap=NULL, *ggH7TG_scam=NULL, *ggH7TG_pdfp=NULL, *ggH7TG_pdfm=NULL;
TGraph *qqH7TG_xsec=NULL, *qqH7TG_errp=NULL, *qqH7TG_errm=NULL, *qqH7TG_scap=NULL, *qqH7TG_scam=NULL, *qqH7TG_pdfp=NULL, *qqH7TG_pdfm=NULL;
TGraph *ggH8TG_xsec=NULL, *ggH8TG_errp=NULL, *ggH8TG_errm=NULL, *ggH8TG_scap=NULL, *ggH8TG_scam=NULL, *ggH8TG_pdfp=NULL, *ggH8TG_pdfm=NULL;
TGraph *qqH8TG_xsec=NULL, *qqH8TG_errp=NULL, *qqH8TG_errm=NULL, *qqH8TG_scap=NULL, *qqH8TG_scam=NULL, *qqH8TG_pdfp=NULL, *qqH8TG_pdfm=NULL;
TGraph *    TG_xsec=NULL, *    TG_errp=NULL, *    TG_errm=NULL, *    TG_scap=NULL, *    TG_scam=NULL, *    TG_pdfp=NULL, *    TG_pdfm=NULL;

TGraph* TG_QCDScaleK0ggH0=NULL, *TG_QCDScaleK0ggH1=NULL, *TG_QCDScaleK1ggH1=NULL, *TG_QCDScaleK1ggH2=NULL, *TG_QCDScaleK2ggH2=NULL;
TGraph* TG_UEPSf0=NULL, *TG_UEPSf1=NULL, *TG_UEPSf2=NULL;

bool subNRB2011 = false;
bool subNRB2012 = false;
bool MCclosureTest = false;

bool mergeWWandZZ = true;
bool skipWW = true;
std::vector<TString> Channels;
std::vector<TString> AnalysisBins;
bool fast = false;
bool skipGGH = false;
bool skipQQH = false;
bool subDY = false;
bool subWZ = false;
double DDRescale = 1.0;
bool blindData = false;
TString DYFile ="";
TString inFileUrl(""),jsonFile(""), histo("");
TString postfix="";
TString systpostfix="";

int indexvbf = -1;
int indexcut   = -1, indexcutL=-1, indexcutR=-1;
int mass=-1, massL=-1, massR=-1;
bool runSystematics = false; bool shape = false;

void printHelp()
{
  printf("Options\n");
  printf("--in        --> input file with from plotter\n");
  printf("--json      --> json file with the sample descriptor\n");
  printf("--histo     --> name of histogram to be used\n");
  printf("--indexvbf  --> index of selection to be used for the vbf bin (if unspecified same as --index)\n");
  printf("--index     --> index of selection to be used (Xbin in histogram to be used)\n");
  printf("--indexL    --> index of selection to be used (Xbin in histogram to be used) used for interpolation\n");
  printf("--indexR    --> index of selection to be used (Xbin in histogram to be used) used for interpolation\n");
  printf("--m         --> higgs mass to be considered\n");
  printf("--mL        --> higgs mass on the left  of the mass to be considered (used for interpollation\n");
  printf("--mR        --> higgs mass on the right of the mass to be considered (used for interpollation\n");
  printf("--syst      --> use this flag if you want to run systematics, default is no systematics\n");
  printf("--shape     --> use this flag if you want to run shapeBased analysis, default is cut&count\n");
  printf("--subNRB    --> use this flag if you want to subtract non-resonant-backgounds similarly to what was done in 2011 (will also remove H->WW)\n");
  printf("--subNRB12  --> use this flag if you want to subtract non-resonant-backgounds using a new technique that keep H->WW\n");
  printf("--subDY     --> histogram that contains the Z+Jets background estimated from Gamma+Jets)\n");
  printf("--subWZ     --> use this flag if you want to subtract WZ background by the 3rd lepton SB)\n");
  printf("--DDRescale --> factor to be used in order to multiply/rescale datadriven estimations\n");
  printf("--closure   --> use this flag if you want to perform a MC closure test (use only MC simulation)\n");
  printf("--bins      --> list of bins to be used (they must be comma separated without space)\n");
  printf("--HWW       --> use this flag to consider HWW signal)\n");
  printf("--skipGGH   --> use this flag to skip GGH signal)\n");
  printf("--skipQQH   --> use this flag to skip GGH signal)\n");
  printf("--blind     --> use this flag to replace observed data by total predicted background)\n");
  printf("--fast      --> use this flag to only do assymptotic prediction (very fast but inaccurate))\n");
  printf("--postfix    --> use this to specify a postfix that will be added to the process names)\n");
  printf("--systpostfix    --> use this to specify a syst postfix that will be added to the process names)\n");
}

//
int main(int argc, char* argv[])
{
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
  gStyle->SetOptStat(0);  
  gStyle->SetOptFit(0);

  //init the TGraphs
  initializeTGraph();

  //get input arguments
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg.find("--help")          !=string::npos) { printHelp(); return -1;} 
    else if(arg.find("--shape")    !=string::npos) { shape=true; printf("shapeBased = True\n");}
    else if(arg.find("--subNRB12") !=string::npos) { subNRB2012=true; skipWW=false; printf("subNRB2012 = True\n");}
    else if(arg.find("--subNRB")   !=string::npos) { subNRB2011=true; skipWW=true; printf("subNRB2011 = True\n");}
    else if(arg.find("--subDY")    !=string::npos) { subDY=true; DYFile=argv[i+1];  i++; printf("Z+Jets will be replaced by %s\n",DYFile.Data());}
    else if(arg.find("--subWZ")    !=string::npos) { subWZ=true; printf("WZ will be estimated from 3rd lepton SB\n");}
    else if(arg.find("--DDRescale")!=string::npos && i+1<argc)  { sscanf(argv[i+1],"%lf",&DDRescale); i++;}
    else if(arg.find("--HWW")      !=string::npos) { skipWW=false; printf("HWW = True\n");}
    else if(arg.find("--skipGGH")  !=string::npos) { skipGGH=true; printf("skipGGH = True\n");}
    else if(arg.find("--skipQQH")  !=string::npos) { skipQQH=true; printf("skipQQH = True\n");}
    else if(arg.find("--blind")    !=string::npos) { blindData=true; printf("blindData = True\n");}
    else if(arg.find("--closure")  !=string::npos) { MCclosureTest=true; printf("MCclosureTest = True\n");}
    else if(arg.find("--indexvbf") !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&indexvbf); i++; printf("indexVBF = %i\n", indexvbf);}
    else if(arg.find("--indexL")    !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&indexcutL); i++; printf("indexL = %i\n", indexcutL);}
    else if(arg.find("--indexR")    !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&indexcutR); i++; printf("indexR = %i\n", indexcutR);}
    else if(arg.find("--index")    !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&indexcut); i++; printf("index = %i\n", indexcut);}
    else if(arg.find("--in")       !=string::npos && i+1<argc)  { inFileUrl = argv[i+1];  i++;  printf("in = %s\n", inFileUrl.Data());  }
    else if(arg.find("--json")     !=string::npos && i+1<argc)  { jsonFile  = argv[i+1];  i++;  printf("json = %s\n", jsonFile.Data()); }
    else if(arg.find("--histo")    !=string::npos && i+1<argc)  { histo     = argv[i+1];  i++;  printf("histo = %s\n", histo.Data()); }
    else if(arg.find("--mL")       !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&massL ); i++; printf("massL = %i\n", massL);}
    else if(arg.find("--mR")       !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&massR ); i++; printf("massR = %i\n", massR);}
    else if(arg.find("--m")        !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&mass ); i++; printf("mass = %i\n", mass);}
    else if(arg.find("--bins")     !=string::npos && i+1<argc)  { char* pch = strtok(argv[i+1],",");printf("bins are : ");while (pch!=NULL){printf(" %s ",pch); AnalysisBins.push_back(pch);  pch = strtok(NULL,",");}printf("\n"); i++; }
    else if(arg.find("--channels") !=string::npos && i+1<argc)  { char* pch = strtok(argv[i+1],",");printf("channels are : ");while (pch!=NULL){printf(" %s ",pch); Channels.push_back(pch);  pch = strtok(NULL,",");}printf("\n"); i++; }
    else if(arg.find("--fast")     !=string::npos) { fast=true; printf("fast = True\n");}
    else if(arg.find("--postfix")   !=string::npos && i+1<argc)  { postfix = argv[i+1]; systpostfix = argv[i+1]; i++;  printf("postfix '%s' will be used\n", postfix.Data());  }
    else if(arg.find("--systpostfix")   !=string::npos && i+1<argc)  { systpostfix = argv[i+1];  i++;  printf("systpostfix '%s' will be used\n", systpostfix.Data());  }
    else if(arg.find("--syst")     !=string::npos) { runSystematics=true; printf("syst = True\n");}
  }
  if(jsonFile.IsNull() || inFileUrl.IsNull() || histo.IsNull() || indexcut == -1 || mass==-1) { printHelp(); return -1; }
  if(AnalysisBins.size()==0)AnalysisBins.push_back("");
  if(Channels.size()==0){Channels.push_back("ee");Channels.push_back("mumu");}

  //build the datacard for this mass point
  std::vector<TString> dcUrls = buildDataCard(mass,histo,inFileUrl, jsonFile);
}




//
Shape_t getShapeFromFile(TFile* inF, TString ch, TString shapeName, int cutBin, JSONWrapper::Object &Root)
{
  Shape_t shape; shape.totalBckg=NULL;shape.data=NULL;

  //iterate over the processes required
  std::vector<JSONWrapper::Object> Process = Root["proc"].daughters();
  for(unsigned int i=0;i<Process.size();i++)
    {
      TString procCtr(""); procCtr+=i;
      TString proc=(Process[i])["tag"].toString();
      TDirectory *pdir = (TDirectory *)inF->Get(proc);         
      if(pdir==0){ printf("Skip Proc=%s because its directory is missing in root file\n", proc.Data()); continue;}

      bool isData(Process[i]["isdata"].toBool());
      bool isSignal(Process[i]["spimpose"].toBool());
      int color(1);       if(Process[i].isTag("color" ) ) color  = (int)Process[i]["color" ].toInt();
      int lcolor(color);  if(Process[i].isTag("lcolor") ) lcolor = (int)Process[i]["lcolor"].toInt();
      int mcolor(color);  if(Process[i].isTag("mcolor") ) mcolor = (int)Process[i]["mcolor"].toInt();
      int fcolor(color);  if(Process[i].isTag("fcolor") ) fcolor = (int)Process[i]["fcolor"].toInt();
      int lwidth(1);      if(Process[i].isTag("lwidth") ) lwidth = (int)Process[i]["lwidth"].toInt();
      int lstyle(1);      if(Process[i].isTag("lstyle") ) lstyle = (int)Process[i]["lstyle"].toInt();
      int fill(1001);     if(Process[i].isTag("fill"  ) ) fill   = (int)Process[i]["fill"  ].toInt();
      int marker(20);     if(Process[i].isTag("marker") ) marker = (int)Process[i]["marker"].toInt();
  
      TH1* syst = (TH1*)pdir->Get("optim_systs");
      if(syst==NULL){syst=new TH1F("optim_systs","optim_systs",1,0,1);syst->GetXaxis()->SetBinLabel(1,"");}
      for(int ivar = 1; ivar<=syst->GetNbinsX();ivar++){
	 TH1D* hshape   = NULL;

	 TString varName = syst->GetXaxis()->GetBinLabel(ivar);
         TString histoName = ch+"_"+shapeName+varName ;
         TH2* hshape2D = (TH2*)pdir->Get(histoName );
         if(!hshape2D){
            if(varName==""){
               //replace by empty histogram
               hshape2D = (TH2*)pdir->Get(shapeName+varName);
               if(hshape2D)hshape2D->Reset();
            }else{
               continue;
            }
         }

         if(hshape2D){
            histoName.ReplaceAll(ch,ch+"_proj"+procCtr);
   	    hshape   = hshape2D->ProjectionY(histoName,cutBin,cutBin);
            if(hshape->Integral()<=0 && varName=="" && !isData){hshape->Reset(); hshape->SetBinContent(1, 1E-10);}
	    hshape->SetDirectory(0);
	    hshape->SetTitle(proc);
	    fixExtremities(hshape,true,true);
	    hshape->SetFillColor(color); hshape->SetLineColor(lcolor); hshape->SetMarkerColor(mcolor);
	    hshape->SetFillStyle(fill);  hshape->SetLineWidth(lwidth); hshape->SetMarkerStyle(marker); hshape->SetLineStyle(lstyle);
            hshape->GetYaxis()->SetTitle("Events (/25GeV)");
         }else{		
            printf("Histo %s does not exist for syst:%s\n", histoName.Data(), varName.Data());
            continue;
         }
	
	 //save in structure
	 if(isData){
	    if(varName=="")  shape.data=hshape;
	    else continue;
	 }else if(isSignal){
	    if(skipGGH && proc.Contains("ggH"))continue;
            if(skipQQH && proc.Contains("qqH"))continue;

            if(skipWW && string(proc.Data()).find("WW")!=string::npos )continue;
            if(!skipWW && mergeWWandZZ){proc.ReplaceAll("WW","VV"); proc.ReplaceAll("ZZ","VV");}

            if(varName==""){
               if(Process[i]["data"].daughters()[0].isTag("xsec"))shape.xsections[proc] = Process[i]["data"].daughters()[0]["xsec"].toDouble();

               int procIndex = -1;
               for(unsigned int i=0;i<shape.signal.size();i++){ if(string(proc.Data())==shape.signal[i]->GetTitle() ){procIndex=i;break;}  }
               if(procIndex>=0) shape.signal[procIndex]->Add(hshape);
               else             {hshape->SetTitle(proc);shape.signal.push_back(hshape);}
            }else{
               std::map<TString,std::vector<std::pair<TString, TH1*> > >::iterator it = shape.signalVars.find(proc);
                
               bool newVar = true;
               if(it!=shape.signalVars.end()){
                  for(unsigned int i=0;i<it->second.size();i++){ if( string(it->second[i].first.Data()) == varName ){it->second[i].second->Add(hshape); newVar=false;break;}  }
               }

               if(newVar){
                  shape.signalVars[proc].push_back( std::pair<TString,TH1*>(varName,hshape) );
               }
            }
	 }else{
	    if(varName=="")  shape.bckg.push_back(hshape);
	    else             shape.bckgVars[proc].push_back( std::pair<TString,TH1*>(varName,hshape) );
	    //printf("histoName = B %i -- %i  -- %s - %s --> %s\n", i, int(varName==""), proc.Data(), histoName.Data(), hshape->GetTitle());
	 }
      }
    }

  //compute the total
  for(size_t i=0; i<shape.bckg.size(); i++)
    {
      if(i==0) { shape.totalBckg = (TH1 *)shape.bckg[i]->Clone(ch+"_"+shapeName+"_total"); shape.totalBckg->SetDirectory(0); }
      else     { shape.totalBckg->Add(shape.bckg[i]); }
    }

  if(MCclosureTest){
     if(shape.totalBckg){
        if(!shape.data){
           shape.data=(TH1F*)shape.totalBckg->Clone("data"); shape.data->SetDirectory(0); shape.data->SetTitle("data");
        }else{
           shape.data->Reset();
           shape.data->Add(shape.totalBckg, 1);
        }
     }
  }

  //all done
  return shape;
}


//
void showShape(std::vector<TString>& selCh ,map<TString, Shape_t>& allShapes, TString mainHisto, TString SaveName)
{

  TH1* allbkg=NULL;
  std::map<TString, TH1*> mapbkg;
  std::map<TString, TH1*> mapsig;
  TH1* alldata=NULL;


  for(size_t i=0;i<selCh.size();i++){
  for(size_t b=0; b<AnalysisBins.size(); b++){
     Shape_t& shape = allShapes.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;

     if(!allbkg){allbkg=(TH1*)shape.totalBckg->Clone("mc");}else{allbkg->Add(shape.totalBckg);}
     if(!alldata){alldata=(TH1*)shape.data->Clone("data");}else{alldata->Add(shape.data);}

     for(size_t i=0; i<shape.bckg.size(); i++){
        if(shape.bckg[i]->Integral()<=1E-6) continue;
        if(mapbkg.find(shape.bckg[i]->GetTitle())!=mapbkg.end()){mapbkg[shape.bckg[i]->GetTitle()]->Add(shape.bckg[i]);}else{ mapbkg[shape.bckg[i]->GetTitle()]=(TH1*)shape.bckg[i]->Clone(shape.bckg[i]->GetTitle()); }
     }

     TString massStr("");if(mass>0) massStr += mass;
     for(size_t ip=0; ip<shape.signal.size(); ip++){
         TString proc(shape.signal[ip]->GetTitle());
         if(mass>0 && !proc.Contains(massStr))continue;
              if(mass>0 && proc.Contains("ggH") && proc.Contains("ZZ"))proc = "ggHZZ2l2v";
         else if(mass>0 && proc.Contains("qqH") && proc.Contains("ZZ"))proc = "qqHZZ2l2v";
         else if(mass>0 && proc.Contains("ggH") && proc.Contains("WW"))proc = "ggHWW2l2v";
         else if(mass>0 && proc.Contains("qqH") && proc.Contains("WW"))proc = "qqHWW2l2v";

        if(proc=="qqHZZ"){shape.signal[ip]->SetLineStyle(2);}

        if(mapsig.find(shape.signal[ip]->GetTitle())!=mapsig.end()){mapsig[shape.signal[ip]->GetTitle()]->Add(shape.signal[ip]);}else{mapsig[shape.signal[ip]->GetTitle()]=(TH1*)shape.signal[ip]->Clone(shape.signal[ip]->GetTitle());}
     }
  }}


  TCanvas* c1 = new TCanvas("c1","c1",800,800);

  TPad* t1 = new TPad("t1","t1", 0.0, 0.20, 1.0, 1.0);  t1->Draw();  t1->cd();
  TLegend* legA  = new TLegend(0.845,0.5,0.99,0.99, "NDC");

  bool canvasIsFilled(false);
  THStack *stack=0;
  TH1 *mc=allbkg;
  if(allbkg){

      TH1* axis = (TH1*)allbkg->Clone("axis");
      axis->Reset();
      axis->GetXaxis()->SetTitle(mc->GetXaxis()->GetTitle());
      axis->GetYaxis()->SetTitle(mc->GetYaxis()->GetTitle());
      axis->SetMinimum(mc->GetMinimum());
      axis->SetMaximum(1.1*mc->GetMaximum());
      axis->GetXaxis()->SetRangeUser(175,450);
      axis->Draw();

      stack = new THStack("stack","stack"); 
      for(std::map<TString, TH1*>::iterator it=mapbkg.begin(); it!=mapbkg.end(); it++){
          it->second->GetXaxis()->SetRangeUser(175,450);
          it->second->SetLineColor( it->second->GetFillColor());
          stack->Add(it->second,"HIST");
          legA->AddEntry(it->second,it->second->GetTitle(),"F");
      }
      stack->Draw("same");
      canvasIsFilled=true;
  }
  if(alldata){
//      alldata->Draw(canvasIsFilled ? "E1same" : "E1");
//      legA->AddEntry(alldata,alldata->GetTitle(),"P");
      legA->AddEntry(alldata,"data (blinded)","P");
//      canvasIsFilled=true;
  }

  for(std::map<TString, TH1*>::iterator it=mapsig.begin(); it!=mapsig.end(); it++){
      it->second->Draw(canvasIsFilled ? "histsame" : "hist");
      legA->AddEntry(it->second,it->second->GetTitle(),"L");
      canvasIsFilled=true;
  }
  TPaveText* T = new TPaveText(0.1,0.995,0.84,0.95, "NDC");
  T->SetFillColor(0);  T->SetFillStyle(0);  T->SetLineColor(0); T->SetBorderSize(0);  T->SetTextAlign(22);
  T->AddText("CMS preliminary, #sqrt{s}=7.0 TeV, #scale[0.5]{#int} L=5.0  fb^{-1}");  T->Draw();
 
  legA->SetFillColor(0); legA->SetFillStyle(0); legA->SetLineColor(0);  legA->SetBorderSize(); legA->SetHeader("");
  legA->Draw("same");    legA->SetTextFont(42);


  TH1 *ratio=0; 
  if(allbkg && alldata){
      c1->cd();
      TPad* t2 = new TPad("t2","t2", 0.0, 0.0, 1.0, 0.2);     t2->Draw();
      t2->cd();
      t2->SetGridy(true);
      t2->SetTopMargin(0);   t2->SetBottomMargin(0.5);
      float yscale = (1.0-0.2)/(0.18-0);
      TH1 *ratio = (TH1*)alldata->Clone("RatioHistogram");
      ratio->SetDirectory(0);
      ratio->Divide(allbkg);
      ratio->GetYaxis()->SetTitle("Obs/Ref");
      ratio->GetXaxis()->SetTitle("");
      ratio->SetMinimum(0);
      ratio->SetMaximum(2.2);
      ratio->GetXaxis()->SetTitleOffset(1.3);
      ratio->GetXaxis()->SetLabelSize(0.033*yscale);
      ratio->GetXaxis()->SetTitleSize(0.036*yscale);
      ratio->GetXaxis()->SetTickLength(0.03*yscale);
      ratio->GetYaxis()->SetTitleOffset(0.3);
      ratio->GetYaxis()->SetNdivisions(5);
      ratio->GetYaxis()->SetLabelSize(0.033*yscale);
      ratio->GetYaxis()->SetTitleSize(0.036*yscale);
      ratio->GetXaxis()->SetRangeUser(175,450);
      ratio->Draw("E1");
  }

  c1->cd();
  c1->Update();
  c1->SaveAs(SaveName+".png");
  c1->SaveAs(SaveName+".pdf");
  c1->SaveAs(SaveName+".C");
  delete c1;
  if(alldata)delete alldata;
  if(allbkg)delete allbkg;
  if(stack) delete stack;
  if(ratio) delete ratio;
}

//
void getYieldsFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName)
{
  FILE* pFile = fopen("Yields.tex","w");
  fprintf(pFile,"\\begin{sidewaystable}[htp]\n\\begin{center}\n\\caption{Event yields expected for background and signal processes and observed in data.}\n\\label{tab:table}\n");

  string Ccol   = "\\begin{tabular}{|c|";
  string Cname  = "channel";
  string Cval   = "";

  TString massStr(""); if(mass>0)massStr += mass;


  TH1* h;
  Double_t valerr, val, syst;
  for(size_t b=0; b<AnalysisBins.size(); b++){
  for(size_t ich=0; ich<ch.size(); ich++) {
    TString icol(ch[ich]+"-"+AnalysisBins[b]);
    icol.ReplaceAll("mu","\\mu"); icol.ReplaceAll("_"," ");
    Cval = "$ "+icol+" $";

    //bckg
    size_t nbckg=allShapes.find(ch[ich]+AnalysisBins[b]+shName)->second.bckg.size();
    for(size_t ibckg=0; ibckg<nbckg; ibckg++){
       TH1* h=allShapes.find(ch[ich]+AnalysisBins[b]+shName)->second.bckg[ibckg];       
       TString procTitle(h->GetTitle()); 
       if(procTitle.Contains("QCD"))continue;
       if(procTitle.Contains("W#rightarrow l#nu"))continue;
       if(procTitle.Contains("Z#rightarrow #tau#tau"))continue;
       procTitle.ReplaceAll("#","\\");
       if(b==0&&ich==0)Ccol  += "c|";
       if(b==0&&ich==0)Cname += "&$" + procTitle + "$";

       val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
       syst = h->GetBinError(0)<=0 ? -1 : h->GetBinError(0); 
       if(val<1E-6){val=0.0; valerr=0.0; syst=-1;}
       Cval += "&" + toLatexRounded(val,valerr, syst);
    }

    //total bckg
    if(b==0&&ich==0)Ccol  += "c|";
    if(b==0&&ich==0)Cname += "&$Total$";
    h=allShapes.find(ch[ich]+AnalysisBins[b]+shName)->second.totalBckg;
    val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
    syst = h->GetBinError(0)<=0 ? -1 : h->GetBinError(0);
    if(val<1E-6){val=0.0; valerr=0.0; syst=-1;}
    Cval += "&\\boldmath " + toLatexRounded(val,valerr,syst);

    //signal
    size_t nsig=allShapes.find(ch[ich]+AnalysisBins[b]+shName)->second.signal.size();
    for(size_t isig=0; isig<nsig; isig++){
       h=allShapes.find(ch[ich]+AnalysisBins[b]+shName)->second.signal[isig];
       TString procTitle(h->GetTitle()); procTitle.ReplaceAll("#","\\");

       if(mass>0 && !procTitle.Contains(massStr))continue;
            if(mass>0 && procTitle.Contains("ggH") && procTitle.Contains("ZZ"))procTitle = "ggH("+massStr+")";
       else if(mass>0 && procTitle.Contains("qqH") && procTitle.Contains("ZZ"))procTitle = "qqH("+massStr+")";
       else if(mass>0 && procTitle.Contains("ggH") && procTitle.Contains("WW"))procTitle = "ggH("+massStr+")WW";
       else if(mass>0 && procTitle.Contains("qqH") && procTitle.Contains("WW"))procTitle = "qqH("+massStr+")WW";

       if(b==0&&ich==0)Ccol  += "c|";
       if(b==0&&ich==0)Cname += "&$" + procTitle+"$";

       val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
       if(val<1E-6){val=0.0; valerr=0.0;}
       Cval += "&" + toLatexRounded(val,valerr);
    }

    //data
    if(b==0&&ich==0)Ccol  += "c|";
    if(b==0&&ich==0)Cname += "&$Data$";
    h=allShapes.find(ch[ich]+AnalysisBins[b]+shName)->second.data;
    val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
    if(val<1E-6){val=0.0; valerr=0.0;}
    Cval += "&\\boldmath " + toLatexRounded(val,0.0);

    //endline
    if(b==0&&ich==0)fprintf(pFile,"%s}\\hline\n", Ccol.c_str());
    if(b==0&&ich==0)fprintf(pFile,"%s\\\\\\hline\n", Cname.c_str());
    fprintf(pFile,"%s\\\\\n", Cval.c_str());
  }}
  fprintf(pFile,"\\hline\n");
  fprintf(pFile,"\\end{tabular}\n\\end{center}\n\\end{sidewaystable}\n");
  fprintf(pFile,"\n\n\n\n");
  fclose(pFile);
}



std::vector<TString>  buildDataCard(Int_t mass, TString histo, TString url, TString Json)
{
  std::vector<TString> dcUrls;

  //get the datacard inputs 
  DataCardInputs dci = convertHistosForLimits(mass,histo,url,Json);

  TString eecard = "";
  TString mumucard = "";
  TString combinedcard = "";

  //build the datacard separately for each channel
  for(size_t i=1; i<=dci.ch.size(); i++){  
      TString dcName=dci.shapesFile;
      dcName.ReplaceAll(".root","_"+dci.ch[i-1]+".dat");
      FILE* pFile = fopen(dcName.Data(),"w");

      combinedcard += dci.ch[i-1]+"="+dcName+" ";
      if(dci.ch[i-1].Contains("ee"))eecard += dci.ch[i-1]+"="+dcName+" ";
      if(dci.ch[i-1].Contains("mumu"))mumucard += dci.ch[i-1]+"="+dcName+" ";

      //header
      fprintf(pFile, "imax 1\n");
      fprintf(pFile, "jmax *\n");
      fprintf(pFile, "kmax *\n");
      fprintf(pFile, "-------------------------------\n");

      fprintf(pFile, "shapes * * %s %s/$PROCESS %s/$PROCESS_$SYSTEMATIC\n",dci.shapesFile.Data(), dci.ch[i-1].Data(), dci.ch[i-1].Data());
      fprintf(pFile, "-------------------------------\n");

      //observations
      fprintf(pFile, "bin 1\n");
      fprintf(pFile, "Observation %f\n",dci.obs[RateKey_t("obs",dci.ch[i-1])]);
      fprintf(pFile, "-------------------------------\n");

      fprintf(pFile,"%45s ", "bin");
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6i ", 1);}  fprintf(pFile,"\n");

      fprintf(pFile,"%45s ", "process");
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6s ", (postfix+dci.procs[j-1]).Data());}  fprintf(pFile,"\n");

      fprintf(pFile,"%45s ", "process"); int procCtr(1-dci.nsignalproc);
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6i ", procCtr );  procCtr++;}  fprintf(pFile,"\n");

      fprintf(pFile,"%45s ", "rate");
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6f ", dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])] );}  fprintf(pFile,"\n");
      fprintf(pFile, "-------------------------------\n");


      //systematics
      char sFile[2048];
      bool isSyst;
      if(runSystematics){
         fprintf(pFile,"%35s %10s ", "lumi", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; 
            if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.022);}else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         if(mass>0){
         fprintf(pFile,"%35s %10s ", "theoryUncXS_HighMH", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
            if((int)j<=dci.nsignalproc){fprintf(pFile,"%6f ",1.0+1.5*pow((mass/1000.0),3));}else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");
         }

         //Id+Trigger efficiencies combined
         if(dci.ch[i-1].Contains("ee")){
            fprintf(pFile,"%35s %10s ", "CMS_eff_e", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.03);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }else{
            fprintf(pFile,"%35s %10s ", "CMS_eff_m", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.03);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }

         if(dci.ch[i-1].Contains("ee")){
            fprintf(pFile,"%35s %10s ", "CMS_scale_e", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.01);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }else{
            fprintf(pFile,"%35s %10s ", "CMS_scale_m", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.01);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }

         if(mass>0){
         fprintf(pFile,"%35s %10s ", "Signal_rescaling_8TeV", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
            if(systpostfix.Contains('8') && (dci.procs[j-1].Contains("ggh") || dci.procs[j-1].Contains("qqh"))){fprintf(pFile,"%6f ",1.25);
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "pdf_gg", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
            if(dci.procs[j-1].Contains("ggh")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",1+0.01*sqrt(pow(TG_pdfp->Eval(mass,NULL,"S"),2) + pow(TG_pdfm->Eval(mass,NULL,"S"),2)));
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "pdf_qqbar", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
            if(dci.procs[j-1].Contains("qqh")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",1+0.01*sqrt(pow(TG_pdfp->Eval(mass,NULL,"S"),2) + pow(TG_pdfm->Eval(mass,NULL,"S"),2)));
            }else if(dci.procs[j-1].BeginsWith("zz")){if(systpostfix.Contains('8')){fprintf(pFile,"%6f ",1.0384);}else{fprintf(pFile,"%6f ",1.0477);}
            }else if(dci.procs[j-1].BeginsWith("wz")){if(systpostfix.Contains('8')){fprintf(pFile,"%6f ",1.1360);}else{fprintf(pFile,"%6f ",1.0552);}
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "UEPS", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                  if(dci.procs[j-1].Contains("ggh") && dci.ch[i-1].Contains("eq0jet")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_UEPSf0->Eval(mass,NULL,"S"));
            }else if(dci.procs[j-1].Contains("ggh") && dci.ch[i-1].Contains("eq1jet")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_UEPSf1->Eval(mass,NULL,"S"));
            }else if(dci.procs[j-1].Contains("ggh") && (dci.ch[i-1].Contains("eq2jet") || dci.ch[i-1].Contains("vbf"))){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_UEPSf2->Eval(mass,NULL,"S"));
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "QCDscale_ggH", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                  if(dci.procs[j-1].Contains("ggh") && dci.ch[i-1].Contains("eq0jet")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_QCDScaleK0ggH0->Eval(mass,NULL,"S"));
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "QCDscale_ggH1in", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                  if(dci.procs[j-1].Contains("ggh") && dci.ch[i-1].Contains("eq0jet")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_QCDScaleK0ggH1->Eval(mass,NULL,"S"));
            }else if(dci.procs[j-1].Contains("ggh") && dci.ch[i-1].Contains("eq1jet")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_QCDScaleK1ggH1->Eval(mass,NULL,"S"));
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "QCDscale_ggH2in", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                  if(dci.procs[j-1].Contains("ggh") && dci.ch[i-1].Contains("eq1jet")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_QCDScaleK1ggH2->Eval(mass,NULL,"S"));
            }else if(dci.procs[j-1].Contains("ggh") && (dci.ch[i-1].Contains("eq2jet") || dci.ch[i-1].Contains("vbf"))){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",TG_QCDScaleK2ggH2->Eval(mass,NULL,"S"));
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "QCDscale_qqH", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
            if(dci.procs[j-1].Contains("qqh")){setTGraph(dci.procs[j-1], systpostfix ); fprintf(pFile,"%6f ",1+0.01*sqrt(pow(TG_scap->Eval(mass,NULL,"S"),2) + pow(TG_scam->Eval(mass,NULL,"S"),2)));
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         }

         fprintf(pFile,"%35s %10s ", "QCDscale_ggVV", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
            fprintf(pFile,"%6s ","-");
         }fprintf(pFile,"\n");

         fprintf(pFile,"%35s %10s ", "QCDscale_VV", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                  if(dci.procs[j-1].BeginsWith("zz")){if(systpostfix.Contains('8')){fprintf(pFile,"%6f ",1.0716);}else{fprintf(pFile,"%6f ",1.0617);}
            }else if(dci.procs[j-1].BeginsWith("wz")){if(systpostfix.Contains('8')){fprintf(pFile,"%6f ",1.1440);}else{fprintf(pFile,"%6f ",1.0847);}
            }else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");


         ///////////////////////////////////////////////

            
         for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator it=dci.systs.begin(); it!=dci.systs.end(); it++){
             if(!runSystematics && string(it->first.Data()).find("stat")>0 )continue;

             isSyst=false;
             if(it->first.Contains("_sys_") || it->first.Contains("_interpol_")){
                sprintf(sFile,"%35s %10s ", it->first.Data(), "lnN");
                for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                   if(it->second.find(RateKey_t(dci.procs[j-1],dci.ch[i-1])) != it->second.end()){
                      Double_t systUnc = it->second[RateKey_t(dci.procs[j-1],dci.ch[i-1])];
                      if(systUnc<=0){ sprintf(sFile,"%s%6s ",sFile,"-");
                      }else{          sprintf(sFile,"%s%6f ",sFile,(1.0+ (systUnc / dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])]) )); isSyst=true;
                      }
                   }else{
                      sprintf(sFile,"%s%6s ",sFile,"-");
                   }
                }if(isSyst)fprintf(pFile,"%s\n",sFile);

             }else{
                sprintf(sFile,"%35s %10s ", it->first.Data(), "shapeN2");
                for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                   if(it->second.find(RateKey_t(dci.procs[j-1],dci.ch[i-1])) != it->second.end()){
                      sprintf(sFile,"%s%6f ",sFile,it->second[RateKey_t(dci.procs[j-1],dci.ch[i-1])]); isSyst=true;
                   }else{
                      sprintf(sFile,"%s%6s ",sFile,"-");
                   }
                }if(isSyst)fprintf(pFile,"%s\n",sFile);
            }
         }
      }

  
      fclose(pFile);
      cout << "Data card for " << dci.shapesFile << " and " << dci.ch[i-1] << " channel @ " << dcName << endl;
      dcUrls.push_back(dcName);
  }

   FILE* pFile = fopen("combineCards.sh","w");
   fprintf(pFile,"%s;\n",(TString("combineCards.py ") + combinedcard + " > " + "card_combined.dat").Data());
   fprintf(pFile,"%s;\n",(TString("combineCards.py ") + eecard       + " > " + "card_ee.dat").Data());
   fprintf(pFile,"%s;\n",(TString("combineCards.py ") + mumucard     + " > " + "card_mumu.dat").Data());
   fclose(pFile);

//  gSystem->Exec(TString("combineCards.py ") + combinedcard + " > " + outDir+"/card_combined.dat");
//  gSystem->Exec(TString("combineCards.py ") + eecard       + " > " + outDir+"/card_ee.dat");
//  gSystem->Exec(TString("combineCards.py ") + mumucard     + " > " + outDir+"/card_mumu.dat");

  //all done
  return dcUrls;
}

//
DataCardInputs convertHistosForLimits(Int_t mass,TString histo,TString url,TString Json)
{
  DataCardInputs dci;
 
  //init the json wrapper
  JSONWrapper::Object Root(Json.Data(), true);

  //init globalVariables
  TString massStr(""); if(mass>0)massStr += mass;
//  std::set<TString> allCh,allProcs;
  std::vector<TString> allCh,allProcs;

  //open input file
  TFile* inF = TFile::Open(url);
  if( !inF || inF->IsZombie() ){ cout << "Invalid file name : " << url << endl; return dci; }

  
  //get the shapes for each channel
  map<TString, Shape_t> allShapes;
  map<TString, Shape_t> allShapesL;
  map<TString, Shape_t> allShapesR;
  TString ch[]={"mumu","ee","emu"};
  const size_t nch=sizeof(ch)/sizeof(TString);
  std::vector<TString> sh;
  sh.push_back(histo);
  if(subNRB2011 || subNRB2012)sh.push_back("nonresbckg_ctrl");
  if(subNRB2012)sh.push_back(histo+"BTagSB");
  if(subWZ)sh.push_back(histo+"_3rdLepton");
  const size_t nsh=sh.size();
  for(size_t i=0; i<nch; i++){
     for(size_t b=0; b<AnalysisBins.size(); b++){
       int indexcut_ = indexcut;
       if(indexvbf>=0 && AnalysisBins[b] =="vbf"){printf("use vbf index(%i) for bin %s\n", indexvbf, AnalysisBins[b].Data()); indexcut_ = indexvbf;}
        for(size_t j=0; j<nsh; j++){
	     allShapes[ch[i]+AnalysisBins[b]+sh[j]]=getShapeFromFile(inF, ch[i]+AnalysisBins[b],sh[j],indexcut_,Root);
             if(indexcutL>=0 && indexcutR>=0){
                if(indexvbf>=0 && AnalysisBins[b] =="vbf"){
                   allShapesL[ch[i]+AnalysisBins[b]+sh[j]]=getShapeFromFile(inF, ch[i]+AnalysisBins[b],sh[j],indexvbf,Root);
                   allShapesR[ch[i]+AnalysisBins[b]+sh[j]]=getShapeFromFile(inF, ch[i]+AnalysisBins[b],sh[j],indexvbf,Root);
                }else{
                   allShapesL[ch[i]+AnalysisBins[b]+sh[j]]=getShapeFromFile(inF, ch[i]+AnalysisBins[b],sh[j],indexcutL,Root);
                   allShapesR[ch[i]+AnalysisBins[b]+sh[j]]=getShapeFromFile(inF, ch[i]+AnalysisBins[b],sh[j],indexcutR,Root);
                }
             }
        }
     }
  }

  //all done with input file
  inF->Close();


  //define vector for search
  std::vector<TString>& selCh = Channels;
//  selCh.push_back("ee"); selCh.push_back("mumu");

  //non-resonant background estimation
  //estimateNonResonantBackground(selCh,"emu",allShapes,"nonresbckg_ctrl");

  //remove the non-resonant background from data
  if(subNRB2011 || subNRB2012)doBackgroundSubtraction(selCh,"emu",allShapes,histo,"nonresbckg_ctrl");

  //replace WZ by its estimate from 3rd Lepton SB
  if(subWZ)doWZSubtraction(selCh,"emu",allShapes,histo,histo+"_3rdLepton");

  //replace Z+Jet background by Gamma+Jet estimates
  if(subDY)doDYReplacement(selCh,"gamma",allShapes,histo,"met_met");

  //replace data by total MC background
  if(blindData)BlindData(selCh,allShapes,histo);

  //interpollate signal sample if desired mass point is not available
  SignalInterpolation(selCh,allShapesL, allShapes, allShapesR, histo);

  //print event yields from the mt shapes
  if(!fast)getYieldsFromShape(selCh,allShapes,histo);

  if(!fast)showShape(selCh,allShapes,histo,"plot");


  //prepare the output
  dci.shapesFile="hzz2l2v_"+massStr+systpostfix+".root";
  TFile *fout=TFile::Open(dci.shapesFile,"recreate");

  //loop on channel/proc/systematics
  for(size_t ich=0; ich<selCh.size(); ich++){
  for(size_t b=0; b<AnalysisBins.size(); b++){
     TString chbin = selCh[ich]+AnalysisBins[b];
     fout->mkdir(chbin);
     fout->cd(chbin);
     allCh.push_back(chbin);
     Shape_t shapeSt = allShapes.find(chbin+histo)->second;

     //signals
     dci.nsignalproc = 0;
     size_t nsignal=allShapes.find(chbin+histo)->second.signal.size();
     for(size_t isignal=0; isignal<nsignal; isignal++){
	TH1* h=shapeSt.signal[isignal];
	std::vector<std::pair<TString, TH1*> > vars = shapeSt.signalVars[h->GetTitle()];

        std::vector<TString> systs;        
	std::vector<TH1*>    hshapes;
	systs.push_back("");
        hshapes.push_back(shapeSt.signal[isignal]);
	for(size_t v=0;v<vars.size();v++){
           systs.push_back(vars[v].first);
           hshapes.push_back(vars[v].second);
        }

        TString proc(h->GetTitle());
	if(mass>0 && !proc.Contains(massStr))continue;
	     if(mass>0 && proc.Contains("ggH") && proc.Contains("ZZ"))proc = "ggHZZ2l2v";
        else if(mass>0 && proc.Contains("qqH") && proc.Contains("ZZ"))proc = "qqHZZ2l2v";
        else if(mass>0 && proc.Contains("ggH") && proc.Contains("WW"))proc = "ggHWW2l2v";
        else if(mass>0 && proc.Contains("qqH") && proc.Contains("WW"))proc = "qqHWW2l2v";

        convertHistosForLimits_core(dci, proc, AnalysisBins[b], chbin, systs, hshapes);
        if(ich==0 && b==0)allProcs.push_back(proc);
        dci.nsignalproc++;
     }

     //backgrounds
     size_t nbckg=allShapes.find(chbin+histo)->second.bckg.size();
     size_t nNonNullBckg=0; 
     for(size_t ibckg=0; ibckg<nbckg; ibckg++){
	TH1* h=shapeSt.bckg[ibckg];
	std::vector<std::pair<TString, TH1*> > vars = shapeSt.bckgVars[h->GetTitle()];

        std::vector<TString> systs;        
	std::vector<TH1*>    hshapes;
	systs.push_back("");
        hshapes.push_back(shapeSt.bckg[ibckg]);
	for(size_t v=0;v<vars.size();v++){
           systs.push_back(vars[v].first);
           hshapes.push_back(vars[v].second);
        }

        TString proc(h->GetTitle());
        convertHistosForLimits_core(dci, proc, AnalysisBins[b], chbin, systs, hshapes);
        if(ich==0 && b==0)allProcs.push_back(proc);

        //remove backgrounds with rate=0 (but keep at least one background)
        std::map<RateKey_t, Double_t>::iterator it = dci.rates.find(RateKey_t(proc,chbin));
        if(it==dci.rates.end()){
           printf("proc=%s not found --> THIS SHOULD NEVER HAPPENS.  PLEASE CHECK THE CODE\n",proc.Data());
        }else{
           if(it->second>0){  nNonNullBckg++;
           }else if(ibckg<nbckg-1 || nNonNullBckg>0){dci.rates.erase(dci.rates.find(RateKey_t(proc,chbin)));
           }
        }

     }
/*
     //remove all backgrounds with rate=0
     size_t nNonNullBckg=0; 
     for(size_t ibckg=0; ibckg<nbckg; ibckg++){        
        TString proc(shapeSt.bckg[ibckg]->GetTitle());
   proc.ReplaceAll("#bar{t}","tbar");
   proc.ReplaceAll("Z-#gamma^{*}+jets#rightarrow ll","dy");
   proc.ReplaceAll("(","");    proc.ReplaceAll(")","");    proc.ReplaceAll("+","");    proc.ReplaceAll(" ","");
   proc.ToLower();

        std::map<RateKey_t, Double_t>::iterator it = dci.rates.find(RateKey_t(proc,chbin));
        if(it==dci.rates.end()){printf("proc=%s not found\n",proc.Data());continue;}
        printf("Rates for %s-%s is %6.2E  - NonNull=%i\n",chbin.Data(), proc.Data(), it->second,(int) nNonNullBckg);
        if(it->second>0){
           nNonNullBckg++;
	   printf("nNonNullBckg++\n");
        }else{
           printf("ibckg<nbckg-1=%i || nNonNullBckg=%i --> %i\n",(int)(ibckg<nbckg-1), (int)(nNonNullBckg>0), (int)(ibckg<nbckg-1 || nNonNullBckg>0));
//           if(ibckg<nbckg-1 || nNonNullBckg>0){printf("Erase Rate %i --> ", (int)dci.rates.size());  dci.rates.erase(dci.rates.find(RateKey_t(proc,chbin))); printf("%i\n", (int)dci.rates.size());}
           printf("Erase Rate %i --> ", (int)dci.rates.size());  dci.rates.erase(it); printf("%i\n", (int)dci.rates.size());
        }
     }
*/

     //data
     TH1* h=shapeSt.data;
     std::vector<TString> systs;
     std::vector<TH1*>    hshapes;
     systs.push_back("");
     hshapes.push_back(h);
     TString proc(h->GetTitle());
     convertHistosForLimits_core(dci, proc, AnalysisBins[b], chbin, systs, hshapes);

     //return to parent dir
     fout->cd("..");     
  }}
  dci.ch.resize(allCh.size());        std::copy(allCh.begin(), allCh.end(),dci.ch.begin());
  dci.procs.resize(allProcs.size());  std::copy(allProcs.begin(), allProcs.end(),dci.procs.begin());


// DEBUGGING
//  printf("-----------------------\n");
//  printf("shapesFile=%s\n",dci.shapesFile.Data());
//  for(unsigned int i=0;i<dci.ch.size();i++){printf("%s - ",dci.ch[i].Data());}printf("\n");
//  for(unsigned int i=0;i<dci.procs.size();i++){printf("%s - ",dci.procs[i].Data());}printf("\n");
//  for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator iter = dci.systs.begin();   iter != dci.systs.end(); ++iter ){
//       printf("%s : ", iter->first.Data());
//       for(std::map<RateKey_t, Double_t>::iterator it = iter->second.begin();   it != iter->second.end(); ++it ){
//                 printf("%s_%s (%f) ", it->first.first.Data(), it->first.second.Data(), it->second);
//       }
//       printf("\n");
//  }
//  printf("-----------------------\n");


/*
  //################# START BACKGROUND SUBTRACTION CODE

    TH1* proj_em = ((TH2*)fin->Get(TString("data/emu_" ) + "nonresbckg_ctrl" ))->ProjectionY("_py", indexcut,indexcut);
    TH1* proj_mm = ((TH2*)fin->Get(TString("data/mumu_") + "nonresbckg_ctrl" ))->ProjectionY("_py", indexcut,indexcut);
    TH1* proj_ee = ((TH2*)fin->Get(TString("data/ee_"  ) + "nonresbckg_ctrl" ))->ProjectionY("_py", indexcut,indexcut);

    printf("Bin %f %f %f %f %f %f\n", proj_em->GetBinContent(1), proj_em->GetBinContent(2), proj_em->GetBinContent(3), proj_em->GetBinContent(4), proj_em->GetBinContent(5), proj_em->GetBinContent(6) );
    printf("Bin %f %f %f %f %f %f\n", proj_ee->GetBinContent(1), proj_ee->GetBinContent(2), proj_ee->GetBinContent(3), proj_ee->GetBinContent(4), proj_ee->GetBinContent(5), proj_ee->GetBinContent(6) );
    printf("Bin %f %f %f %f %f %f\n", proj_mm->GetBinContent(1), proj_mm->GetBinContent(2), proj_mm->GetBinContent(3), proj_mm->GetBinContent(4), proj_mm->GetBinContent(5), proj_mm->GetBinContent(6) );
    double alpha_e     = proj_ee->GetBinContent(5) / proj_em->GetBinContent(5);
    double alpha_e_err = ( fabs( proj_ee->GetBinContent(5) * proj_em->GetBinError(5) ) + fabs(proj_ee->GetBinError(5) * proj_em->GetBinContent(5) )  ) / pow(proj_em->GetBinContent(5), 2);

    double alpha_m     = proj_mm->GetBinContent(5) / proj_em->GetBinContent(5);
    double alpha_m_err = ( fabs( proj_mm->GetBinContent(5) * proj_em->GetBinError(5) ) + fabs(proj_mm->GetBinError(5) * proj_em->GetBinContent(5) )  ) / pow(proj_em->GetBinContent(5), 2);

    printf("alpha e=%f+-%f mu=%f+-%f\n", alpha_e, alpha_e_err, alpha_m, alpha_m_err);





  //################# END   BACKGROUND SUBTRACTION CODE

*/


  //all done
  fout->Close();

  return dci;
}



void convertHistosForLimits_core(DataCardInputs& dci, TString& proc, TString& bin, TString& ch, std::vector<TString>& systs, std::vector<TH1*>& hshapes){
   proc.ReplaceAll("#bar{t}","tbar");
   proc.ReplaceAll("Z-#gamma^{*}+jets#rightarrow ll","dy");
   proc.ReplaceAll("#rightarrow","");
   proc.ReplaceAll("(","");    proc.ReplaceAll(")","");    proc.ReplaceAll("+","");    proc.ReplaceAll(" ","");   proc.ReplaceAll("/","");  proc.ReplaceAll("#","");
   proc.ToLower();

   for(unsigned int i=0;i<systs.size();i++){
       TString syst   = systs[i];
       TH1*    hshape = hshapes[i];
       hshape->SetDirectory(0);

       //Do Renaming and cleaning
       syst.ReplaceAll("down","Down");
       syst.ReplaceAll("up","Up");

       if(syst==""){syst="";
       }else if(syst.BeginsWith("_jes")){syst.ReplaceAll("_jes","_CMS_scale_j");
       }else if(syst.BeginsWith("_jer")){syst.ReplaceAll("_jer","_CMS_res_j"); // continue;//skip res for now
       }else if(syst.BeginsWith("_btag")){syst.ReplaceAll("_btag","_CMS_eff_b"); 
       }else if(syst.BeginsWith("_pu" )){syst.ReplaceAll("_pu", "_CMS_hzz2l2v_pu");
       }else if(syst.BeginsWith("_ren" )){continue;   //already accounted for in QCD scales
       }else if(syst.BeginsWith("_fact" )){continue; //skip this one
       }else{   syst="_CMS_hzz2l2v"+syst;
       }

       double systUncertainty = hshape->GetBinError(0);  hshape->SetBinError(0,0);

       //If cut&count keep only 1 bin in the histo
       if(!shape){
//          hshape = hshape->Rebin(hshape->GetXaxis()->GetNbins(), TString(hshape->GetName())+"_Rebin"); 
          hshape = hshape->Rebin(hshape->GetXaxis()->GetNbins()); 
          //make sure to also count the underflow and overflow
          double bin  = hshape->GetBinContent(0) + hshape->GetBinContent(1) + hshape->GetBinContent(2);
          double bine = sqrt(hshape->GetBinError(0)*hshape->GetBinError(0) + hshape->GetBinError(1)*hshape->GetBinError(1) + hshape->GetBinError(2)*hshape->GetBinError(2));
          hshape->SetBinContent(0,0);              hshape->SetBinError  (0,0);
          hshape->SetBinContent(1,bin);            hshape->SetBinError  (1,bine);
          hshape->SetBinContent(2,0);              hshape->SetBinError  (2,0);
       }

       if(syst==""){
         //central shape (for data call it data_obs)
         hshape->SetName(proc); 
         if(proc=="data"){
            hshape->Write("data_obs");
         }else{
            hshape->Write(proc+postfix);

            if(hshape->Integral()>0){
               hshape->SetName(proc+syst);
               TH1* statup=(TH1 *)hshape->Clone(proc+"_stat"+ch+proc+"Up");
               TH1* statdown=(TH1 *)hshape->Clone(proc+"_stat"+ch+proc+"Down");
               for(int ibin=1; ibin<=statup->GetXaxis()->GetNbins(); ibin++){
                  statup  ->SetBinContent(ibin,std::min(2*hshape->GetBinContent(ibin), std::max(0.01*hshape->GetBinContent(ibin), statup  ->GetBinContent(ibin) + statup  ->GetBinError(ibin))));
                  statdown->SetBinContent(ibin,std::min(2*hshape->GetBinContent(ibin), std::max(0.01*hshape->GetBinContent(ibin), statdown->GetBinContent(ibin) - statdown->GetBinError(ibin))));
               }               
               statup  ->Write(proc+postfix+"_CMS_hzz2l2v_stat_"+ch+"_"+proc+systpostfix+"Up");
               statdown->Write(proc+postfix+"_CMS_hzz2l2v_stat_"+ch+"_"+proc+systpostfix+"Down");
               dci.systs["CMS_hzz2l2v_stat_"+ch+"_"+proc+systpostfix][RateKey_t(proc,ch)]=1.0;              
               if(systUncertainty>0){
                  if(proc.Contains("ggh") || proc.Contains("qqh")){
                     dci.systs["CMS_hzz2l2v_interpol_"+bin+"_"+proc+systpostfix][RateKey_t(proc,ch)]=systUncertainty;
                  }else{
                     //makesure that syst+stat error is never bigger than 100%
                     //double valerr, val  = hshape->IntegralAndError(1,hshape->GetXaxis()->GetNbins(),valerr);
                     //if(sqrt(pow(valerr,2)+pow(systUncertainty,2))>val){systUncertainty = sqrt(std::max(0.0, pow(val,2) - pow(valerr,2)));}
                     dci.systs["CMS_hzz2l2v_sys_"+bin+"_"+proc+systpostfix][RateKey_t(proc,ch)]=systUncertainty;
                  }
               }
            }
         }
       }else if(runSystematics && proc!="data" && (syst.Contains("Up") || syst.Contains("Down"))){
         //if empty histogram --> no variation is applied
         if(hshape->Integral()<hshapes[0]->Integral()*0.01){hshape->Reset(); hshape->Add(hshapes[0],1); }

         //write variation to file
         hshape->SetName(proc+syst);
         hshape->Write(proc+postfix+syst);
       }else if(runSystematics){
         //for one sided systematics the down variation mirrors the difference bin by bin
         hshape->SetName(proc+syst);
         hshape->Write(proc+postfix+syst+"Up");
         TH1 *hmirrorshape=(TH1 *)hshape->Clone(proc+syst+"Down");
         for(int ibin=1; ibin<=hmirrorshape->GetXaxis()->GetNbins(); ibin++){
//            double bin = hmirrorshape->GetBinContent(ibin);
            double bin = 2*hshapes[0]->GetBinContent(ibin)-hmirrorshape->GetBinContent(ibin);
            if(bin<0)bin=0;
            hmirrorshape->SetBinContent(ibin,bin);
         }
	 if(hmirrorshape->Integral()<=0)hmirrorshape->SetBinContent(1, 1E-10);
         hmirrorshape->Write(proc+postfix+syst+"Down");
       }

       if(runSystematics && syst!=""){
          TString systName(syst); 
          systName.ReplaceAll("Up",""); systName.ReplaceAll("Down","");//  systName.ReplaceAll("_","");
          if(systName.First("_")==0)systName.Remove(0,1);

          TH1 *temp=(TH1*) hshape->Clone();
          temp->Add(hshapes[0],-1);
          if(temp->Integral()!=0)dci.systs[systName][RateKey_t(proc,ch)]=1.0;
          delete temp;
//        }else if(proc=="asignal" && syst==""){dci.rates[RateKey_t(proc,ch)]=hshape->Integral();
//        }else if(proc!="data" && syst==""){if(hshape->Integral()>1E-6)dci.rates[RateKey_t(proc,ch)]=hshape->Integral();
        }else if(proc!="data" && syst==""){dci.rates[RateKey_t(proc,ch)]= hshape->Integral()>1E-6 ? hshape->Integral() : 0.0;
        }else if(proc=="data" && syst==""){dci.obs[RateKey_t("obs",ch)]=hshape->Integral();
        }
   }
}

void doBackgroundSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString sideBandHisto)
{
     string Lcol   = "\\begin{tabular}{|l";
     string Lchan  = "channel";
     string Lalph1 = "$\\alpha$ measured";
     string Lalph2 = "$\\alpha$ used";
     string Lyield   = "yield data";
     string LyieldMC = "yield mc";

     string Ccol   = "\\begin{tabular}{|l|c|c|c|c|";
     string Cname  = "channel & $\\alpha$ measured & $\\alpha$ used & yield data & yield mc";
     string Cval   = "";
     FILE* pFile = NULL;
     if(!fast){
        pFile = fopen("NonResonnant.tex","w");
        fprintf(pFile,"\\begin{table}[htp]\n\\begin{center}\n\\caption{Non resonant background estimation.}\n\\label{tab:table}\n");
        fprintf(pFile,"%s}\\hline\n", Ccol.c_str());
        fprintf(pFile,"%s\\\\\\hline\n", Cname.c_str());
     }



    for(size_t i=0;i<selCh.size();i++){
    for(size_t b=0; b<AnalysisBins.size(); b++){     
        Lcol += " |c";
        Lchan += string(" &")+selCh[i]+string(" - ")+AnalysisBins[b];
        Cval   = selCh[i]+string(" - ")+AnalysisBins[b];


        Shape_t& shapeCtrl_SB = allShapes.find(ctrlCh+AnalysisBins[b]+sideBandHisto)->second;
        TH1* hCtrl_SB=shapeCtrl_SB.data;
        Shape_t& shapeCtrl_SI = allShapes.find(ctrlCh+AnalysisBins[b]+mainHisto)->second;
        TH1* hCtrl_SI=shapeCtrl_SI.data;
        Shape_t& shapeChan_SB = allShapes.find(selCh[i]+AnalysisBins[b]+sideBandHisto)->second;
        TH1* hChan_SB=shapeChan_SB.data;
        Shape_t& shapeChan_SI = allShapes.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;
//        TH1* hChan_SI=shapeChan_SI.data;

	//printf("Channel = %s\n", selCh[i].Data());
        //printf("Bin %f %f %f %f %f %f\n", hCtrl_SB->GetBinContent(1), hCtrl_SB->GetBinContent(2), hCtrl_SB->GetBinContent(3), hCtrl_SB->GetBinContent(4), hCtrl_SB->GetBinContent(5), hCtrl_SB->GetBinContent(6) );
        //printf("Bin %f %f %f %f %f %f\n", hChan_SB->GetBinContent(1), hChan_SB->GetBinContent(2), hChan_SB->GetBinContent(3), hChan_SB->GetBinContent(4), hChan_SB->GetBinContent(5), hChan_SB->GetBinContent(6) );
        double alpha=0 ,alpha_err=0;
        if(hCtrl_SB->GetBinContent(5)>0){
           alpha     = hChan_SB->GetBinContent(5) / hCtrl_SB->GetBinContent(5);
           alpha_err = ( fabs( hChan_SB->GetBinContent(5) * hCtrl_SB->GetBinError(5) ) + fabs(hChan_SB->GetBinError(5) * hCtrl_SB->GetBinContent(5) )  ) / pow(hCtrl_SB->GetBinContent(5), 2);        
        }

        Lalph1 += string(" &") + toLatexRounded(alpha,alpha_err);
        Cval   += string(" &") + toLatexRounded(alpha,alpha_err);

//        if(selCh[i].First("ee"  )!=kNPOS){alpha = 0.339286; alpha_err=0.043549;}
//        if(selCh[i].First("mumu")!=kNPOS){alpha = 0.529018; alpha_err=0.059357;}

        if(selCh[i].First("ee"  )!=kNPOS){alpha = 0.34; alpha_err=0.03;}
        if(selCh[i].First("mumu")!=kNPOS){alpha = 0.61; alpha_err=0.04;}


        //add 100% syst uncertainty on alpha
        //alpha_err = sqrt(pow(alpha*1.0,2)+pow(alpha_err,2));

        Lalph2 += string(" &") + toLatexRounded(alpha,alpha_err);
        Cval   += string(" &") + toLatexRounded(alpha,alpha_err);


        TH1* NonResonant = NULL;
        if(subNRB2011){     
                                NonResonant = (TH1*)hCtrl_SI->Clone("Top/WW/W+Jets (data)");
                                NonResonant->SetTitle("Top/WW/W+Jets (data)");
        }else if(subNRB2012){  
                                Shape_t& shapeChan_BTag = allShapes.find(selCh[i]+mainHisto+"BTagSB")->second;                                
                                NonResonant = (TH1*)shapeChan_BTag.data->Clone("Top (data)");
                                NonResonant->SetTitle("Top (data)");
        }else{                  return;
        }

        double valvalerr, valval;
        valval = NonResonant->IntegralAndError(1,NonResonant->GetXaxis()->GetNbins(),valvalerr);
        for(int b=1;b<=NonResonant->GetXaxis()->GetNbins()+1;b++){
           double val = NonResonant->GetBinContent(b);
           double err = NonResonant->GetBinError(b);
           double newval = val*alpha;
           double newerr = sqrt(pow(err*alpha,2) + pow(val*alpha_err,2));
           NonResonant->SetBinContent(b, newval );
           NonResonant->SetBinError  (b, newerr );
        }
        NonResonant->Scale(DDRescale);

        Double_t valerr;
        Double_t val = NonResonant->IntegralAndError(1,NonResonant->GetXaxis()->GetNbins(),valerr);
        Double_t systError = val*NonResonnantSyst;
        NonResonant->SetBinError(0,systError);//save syst error in underflow bin that is always empty
        if(val<1E-6){val=0.0; valerr=0.0; systError=-1;}
        Lyield += string(" &") + toLatexRounded(val,valerr,systError);
        Cval   += string(" &") + toLatexRounded(val,valerr,systError);

        //Clean background collection
        TH1* MCNRB = (TH1*)shapeChan_SI.totalBckg->Clone("MCNRB"); MCNRB->Reset();
        for(size_t ibckg=0; ibckg<shapeChan_SI.bckg.size(); ibckg++){           
           TString proc(shapeChan_SI.bckg[ibckg]->GetTitle());
	   if(( subNRB2011 && (proc.Contains("t#bar{t}") || proc.Contains("Single top") || proc.Contains("WW") || proc.Contains("Z#rightarrow #tau#tau")) ) ||
              ( subNRB2012 && (proc.Contains("t#bar{t}") || proc.Contains("Single top") ) ) ){
              MCNRB->Add(shapeChan_SI.bckg[ibckg], 1);
              NonResonant->SetFillColor(shapeChan_SI.bckg[ibckg]->GetFillColor());
              NonResonant->SetLineColor(shapeChan_SI.bckg[ibckg]->GetLineColor());
	      shapeChan_SI.bckg.erase(shapeChan_SI.bckg.begin()+ibckg);  ibckg--;
           }
        }

        NonResonant->SetFillStyle(1001);
        NonResonant->SetFillColor(592);
//        NonResonant->SetLineColor(592);


        //add the background estimate
        shapeChan_SI.bckg.push_back(NonResonant);


        //recompute total background
        shapeChan_SI.totalBckg->Reset();
        for(size_t i=0; i<shapeChan_SI.bckg.size(); i++){shapeChan_SI.totalBckg->Add(shapeChan_SI.bckg[i]);}

        val = MCNRB->IntegralAndError(1,MCNRB->GetXaxis()->GetNbins(),valerr);
          if(val<1E-6){val=0.0; valerr=0.0;}
        LyieldMC += string(" &") + toLatexRounded(val,valerr);
        Cval     += string(" &") + toLatexRounded(val,valerr);

        if(pFile){
           fprintf(pFile,"%s\\\\\n", Cval.c_str());
        }
     }}

     if(pFile){
        fprintf(pFile,"\\hline\n");
        fprintf(pFile,"\\end{tabular}\n\\end{center}\n\\end{table}\n");
        fprintf(pFile,"\n\n\n\n");

        fprintf(pFile,"\\begin{table}[htp]\n\\begin{center}\n\\caption{Non resonant background estimation.}\n\\label{tab:table}\n");
        fprintf(pFile,"%s|}\\hline\n", Lcol.c_str());
        fprintf(pFile,"%s\\\\\n", Lchan.c_str());
        fprintf(pFile,"%s\\\\\n", Lalph1.c_str());
        fprintf(pFile,"%s\\\\\n", Lalph2.c_str());
        fprintf(pFile,"%s\\\\\n", Lyield.c_str());
        fprintf(pFile,"%s\\\\\n", LyieldMC.c_str());
        fprintf(pFile,"\\hline\n");
        fprintf(pFile,"\\end{tabular}\n\\end{center}\n\\end{table}\n");
        fclose(pFile);
     }
}



void doDYReplacement(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString metHistoForRescale){
  TString DYProcName = "Z#rightarrow ll";
  TString GammaJetProcName = "Instr. background (data)";
  std::map<TString, double> LowMetIntegral;


  string Ccol   = "\\begin{tabular}{|l|c|c|c|";
  string Cname  = "channel & rescale & yield data & yield mc";
  string Cval   = "";
  FILE* pFile = NULL;
  if(!fast){
     pFile = fopen("GammaJets.tex","w");
     fprintf(pFile,"\\begin{table}[htp]\n\\begin{center}\n\\caption{Instrumental background estimation.}\n\\label{tab:table}\n");
     fprintf(pFile,"%s}\\hline\n", Ccol.c_str());
     fprintf(pFile,"%s\\\\\\hline\n", Cname.c_str());
  }

  //open input file
  TFile* inF = TFile::Open(inFileUrl);
  if( !inF || inF->IsZombie() ){ cout << "Invalid file name : " << inFileUrl << endl; return; }

  TDirectory *pdir = (TDirectory *)inF->Get(DYProcName);        
  if(!pdir){ printf("Skip Z+Jet estimation because %s directory is missing in root file\n", DYProcName.Data()); return;}

  for(size_t i=0;i<selCh.size();i++){
  for(size_t b=0; b<AnalysisBins.size(); b++){
     TH1* met = (TH1*)pdir->Get(selCh[i]+AnalysisBins[b]+"_"+metHistoForRescale);
     LowMetIntegral[selCh[i]+AnalysisBins[b]] = met->Integral(0,met->GetXaxis()->FindBin(50));
  }}

  //all done with input file
  inF->Close();


  //open gamma+jet file
  inF = TFile::Open(DYFile);
  if( !inF || inF->IsZombie() ){ cout << "Invalid file name : " << DYFile << endl; return; }
  
  pdir = (TDirectory *)inF->Get(GammaJetProcName);
  if(!pdir){ printf("Skip Z+Jet estimation because %s directory is missing in Gamma+Jet file\n", GammaJetProcName.Data()); return;}

  for(size_t i=0;i<selCh.size();i++){
  for(size_t b=0; b<AnalysisBins.size(); b++){ 
     Cval   = selCh[i]+string(" - ")+AnalysisBins[b];
     Shape_t& shapeChan_SI = allShapes.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;

     //find DY background
     for(size_t ibckg=0; ibckg<shapeChan_SI.bckg.size(); ibckg++){           
        TString proc(shapeChan_SI.bckg[ibckg]->GetTitle());
        if( proc.Contains(DYProcName) ){

           //compute rescale factor using low MET events
           TH1* met = (TH1*)pdir->Get(selCh[i]+AnalysisBins[b]+"_"+metHistoForRescale);           
           double integral = met->Integral(0,met->GetXaxis()->FindBin(50));
           double rescaleFactor = LowMetIntegral[selCh[i]+AnalysisBins[b]] / integral;
           printf("Rescale in %s = %f/%f = %f\n",  (selCh[i]+AnalysisBins[b]).Data(), LowMetIntegral[selCh[i]+AnalysisBins[b]], integral, rescaleFactor);         
           char buffer[255]; sprintf(buffer,"%6.3f",rescaleFactor);
           Cval   += string(" &") + buffer;

           Double_t valerr, val = shapeChan_SI.bckg[ibckg]->IntegralAndError(1,shapeChan_SI.bckg[ibckg]->GetXaxis()->GetNbins(),valerr);
           if(val<1E-6){val=0.0; valerr=0.0;}
           string MCTruth = toLatexRounded(val,valerr);
  
           //replace DY histogram by G+Jets data
           int indexcut_ = indexcut;
           if(indexvbf>=0 && AnalysisBins[b] =="vbf"){printf("use vbf index(%i) for bin %s\n", indexvbf, AnalysisBins[b].Data()); indexcut_ = indexvbf;}
           TH2* gjets2Dshape  = (TH2*)pdir->Get(selCh[i]+AnalysisBins[b]+"_"+mainHisto);
           TH1* gjets1Dshape  = gjets2Dshape->ProjectionY("tmpName",indexcut_,indexcut_);
           shapeChan_SI.bckg[ibckg]->SetTitle(DYProcName + " (data)");    
           for(int i=0;i<shapeChan_SI.bckg[ibckg]->GetNbinsX();i++){             
              double val = gjets1Dshape->GetBinContent(i);
              double err = gjets1Dshape->GetBinError(i);
              if(val<0)val=0;
              double newval = rescaleFactor*val;
              double newerr = rescaleFactor*err;
              shapeChan_SI.bckg[ibckg]->SetBinContent(i, newval);  
              shapeChan_SI.bckg[ibckg]->SetBinError  (i, newerr);
           } 
           delete gjets1Dshape;

           shapeChan_SI.bckg[ibckg]->Scale(DDRescale);



           val  = shapeChan_SI.bckg[ibckg]->IntegralAndError(1,shapeChan_SI.bckg[ibckg]->GetXaxis()->GetNbins(),valerr);
           double systError = GammaJetSyst * val;
           shapeChan_SI.bckg[ibckg]->SetBinError(0,systError);//save syst error in underflow bin that is always empty
           if(val<1E-6){val=0.0; valerr=0.0; systError=-1;}
           Cval+= string(" &") + toLatexRounded(val,valerr,systError) +" & " + MCTruth;

           //erase Systematic relatated to DY background
           std::map<TString,std::vector<std::pair<TString, TH1*> > >::iterator dyvars = shapeChan_SI.bckgVars.find(DYProcName);
           if(dyvars!=shapeChan_SI.bckgVars.end()){shapeChan_SI.bckgVars.erase(dyvars);}

           //recompute total background
           shapeChan_SI.totalBckg->Reset();
           for(size_t i=0; i<shapeChan_SI.bckg.size(); i++){shapeChan_SI.totalBckg->Add(shapeChan_SI.bckg[i]);}

           if(pFile){
              fprintf(pFile,"%s\\\\\n", Cval.c_str());
           }
        }
     }

   
  }}

  if(pFile){
     fprintf(pFile,"\\hline\n");
     fprintf(pFile,"\\end{tabular}\n\\end{center}\n\\end{table}\n");
     fprintf(pFile,"\n\n\n\n");
     fclose(pFile);
  }

  //all done with gamma+jet file
  inF->Close();
}



void doWZSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString sideBandHisto)
{
     string Ccol   = "\\begin{tabular}{|l|c|c|c|c|";
     string Cname  = "channel & $\\alpha$ measured & $\\alpha$ used & yield data & yield mc";
     string Cval   = "";
     FILE* pFile = NULL;
     if(!fast){
        pFile = fopen("WZ.tex","w");
        fprintf(pFile,"\\begin{table}[htp]\n\\begin{center}\n\\caption{Non resonant background estimation.}\n\\label{tab:table}\n");
        fprintf(pFile,"%s}\\hline\n", Ccol.c_str());
        fprintf(pFile,"%s\\\\\\hline\n", Cname.c_str());
     }

    for(size_t i=0;i<selCh.size();i++){
    for(size_t b=0; b<AnalysisBins.size(); b++){     
        Cval   = selCh[i]+string(" - ")+AnalysisBins[b];

        Shape_t& shapeCtrl_SB = allShapes.find(ctrlCh+AnalysisBins[b]+sideBandHisto)->second;
        Shape_t& shapeCtrl_SI = allShapes.find(ctrlCh+AnalysisBins[b]+mainHisto)->second;
        Shape_t& shapeChan_SB = allShapes.find(selCh[i]+AnalysisBins[b]+sideBandHisto)->second;
        Shape_t& shapeChan_SI = allShapes.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;

        fprintf(pFile,"#############%s:\n",(string(" &")+selCh[i]+string(" - ")+AnalysisBins[b]).Data());
        fprintf(pFile,"MC: em 3leptons=%6.2E  em 2leptons=%6.2E  ll 3leptons=%6.2E  ll 2leptons=%6.2E\n",shapeCtrl_SB.totalBckg->Integral(), shapeCtrl_SI.totalBckg->Integral(), shapeChan_SB.totalBckg->Integral(), shapeChan_SI.totalBckg->Integral());

        TH1* histo1=NULL, *histo2=NULL, *histo3=NULL, *histo4=NULL;
        for(size_t ibckg=0; ibckg<shapeCtrl_SB.bckg.size(); ibckg++){if(TString(shapeCtrl_SB.bckg[ibckg]->GetTitle()).Contains("WZ"))histo1=shapeCtrl_SB.bckg[ibckg]; }
        for(size_t ibckg=0; ibckg<shapeCtrl_SI.bckg.size(); ibckg++){if(TString(shapeCtrl_SI.bckg[ibckg]->GetTitle()).Contains("WZ"))histo2=shapeCtrl_SI.bckg[ibckg]; }
        for(size_t ibckg=0; ibckg<shapeChan_SB.bckg.size(); ibckg++){if(TString(shapeChan_SB.bckg[ibckg]->GetTitle()).Contains("WZ"))histo3=shapeChan_SB.bckg[ibckg]; }
        for(size_t ibckg=0; ibckg<shapeChan_SI.bckg.size(); ibckg++){if(TString(shapeChan_SI.bckg[ibckg]->GetTitle()).Contains("WZ"))histo4=shapeChan_SI.bckg[ibckg]; }
        fprintf(pFile,"WZ: em 3leptons=%6.2E  em 2leptons=%6.2E  ll 3leptons=%6.2E  ll 2leptons=%6.2E\n",histo1->Integral(), histo2->Integral(), histo3->Integral(), histo4->Integral());

        double Num, Denom, NumError, DenomError;
        Num = histo4->IntegralAndError(1,histo4->GetXaxis()->GetNbins(),NumError);
        Denom = histo3->IntegralAndError(1,histo3->GetXaxis()->GetNbins(),DenomError);
        double ratio = Num/Denom; 
        double ratio_err  = sqrt(pow(Num*DenomError,2) + pow(Denom*NumError,2))/ pow(Denom,2);
        double ratio_syst = fabs(histo3->Integral() - shapeChan_SB.totalBckg->Integral())/shapeChan_SB.totalBckg->Integral();
        fprintf(pFile,"Ratio = %s\n",toLatexRounded(ratio,ratio_err,ratio_syst).c_str());
        
        Double_t valerr;
        Double_t val = histo4->IntegralAndError(1,histo4->GetXaxis()->GetNbins(),valerr);
        
        Double_t valerr2, valsyst2;
        Double_t val2 = shapeChan_SB.totalBckg->IntegralAndError(1,shapeChan_SB.data->GetXaxis()->GetNbins(),valerr2);
        valerr2= sqrt(pow(valerr2*ratio,2) + pow(val2*ratio_err,2) );
        valsyst2 = val2*ratio_syst;
        val2=val2*ratio;
        fprintf(pFile,"WZ (MC closure test): %s --> %s\n",toLatexRounded(val,valerr).c_str(), toLatexRounded(val2,valerr2,valsyst2).c_str());

        bool noDataObserved=false;
        Double_t valerr3, valsyst3;
        Double_t val3 = shapeChan_SB.data->IntegralAndError(1,shapeChan_SB.data->GetXaxis()->GetNbins(),valerr3);
        if(val3<=0){noDataObserved=true; val3=1.0; valerr3=1.0;}
        valerr3= sqrt(pow(valerr3*ratio,2) + pow(val3*ratio_err,2) );
        valsyst3 = val3*ratio_syst;
        val3=val3*ratio;
        if(!noDataObserved){
        fprintf(pFile,"WZ (from data)      : %s --> %s\n",toLatexRounded(val,valerr).c_str(), toLatexRounded(val3,valerr3,valsyst3).c_str());
        }else{
        fprintf(pFile,"WZ (from data)      : %s --> smaller than %s (because no data was observed in 3dlepton SideBand--> assume 1+-1 observed data for rescale)\n",toLatexRounded(val,valerr).c_str(), toLatexRounded(val3,valerr3,valsyst3).c_str());
        }


     }}

     if(pFile){
        fclose(pFile);
     }
}



void BlindData(std::vector<TString>& selCh, map<TString, Shape_t>& allShapes, TString mainHisto){
    for(size_t i=0;i<selCh.size();i++){
    for(size_t b=0; b<AnalysisBins.size(); b++){
        Shape_t& shapeChan_SI = allShapes.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;
        shapeChan_SI.data->Reset(); 
        shapeChan_SI.data->Add(shapeChan_SI.totalBckg,1);
    }}
}

/*
void SignalInterpolation(std::vector<TString>& selCh, map<TString, Shape_t>& allShapes, TString mainHisto, JSONWrapper::Object &Root){    
   std::vector<TString> signalTypes;
   signalTypes.push_back("ggH");
   signalTypes.push_back("qqH");

   for(unsigned int t=0;t<signalTypes.size();t++){

      int  leftMass = 0;
      int rightMass =9999;
      Shape_t& shapeRef = allShapes.find(selCh[0]+AnalysisBins[0]+mainHisto)->second;
      for(unsigned int i=0;i<shapeRef.signal.size();i++){
         string proctmp = shapeRef.signal[i]->GetTitle();
         if(proctmp.find(signalTypes[t].Data())!=0)continue;
         proctmp = proctmp.substr(proctmp.find('(')+1,4);
         int currentMass;  sscanf(proctmp.c_str(),"%d",&currentMass);
         printf("%s --> currentMass Point=%i\n",proctmp.c_str(),currentMass);
         if(currentMass<=0 || currentMass>2000){printf("skip\n");continue;}
         if(currentMass<=mass && currentMass>= leftMass){ leftMass=currentMass;}
         if(currentMass>=mass && currentMass<=rightMass){rightMass=currentMass;}
      }   
      if(leftMass==mass || rightMass==mass)continue;
      if(leftMass==0    || rightMass==9999)continue;
      printf("MASS: %i will be interpolated from %i and %i for %s\n",mass, leftMass,rightMass,signalTypes[t].Data());

      TString  LeftName = signalTypes[t]+"("; LeftName+= leftMass;
      TString RightName = signalTypes[t]+"(";RightName+=rightMass;

      for(size_t i=0;i<selCh.size();i++){
      for(size_t b=0; b<AnalysisBins.size(); b++){
           Shape_t& shapeChan_SI = allShapes.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;

           int leftIndex=0, rightIndex=0;
           for(unsigned int i=0;i<shapeChan_SI.signal.size();i++){if(TString(shapeChan_SI.signal[i]->GetTitle()).BeginsWith( LeftName)) leftIndex=i;}
           for(unsigned int i=0;i<shapeChan_SI.signal.size();i++){if(TString(shapeChan_SI.signal[i]->GetTitle()).BeginsWith(RightName))rightIndex=i;}

           double Ratio = (mass - leftMass); Ratio/=(rightMass - leftMass);

           //centralValue
           TH1* histoLeft  = (TH1*) shapeChan_SI.signal[ leftIndex]->Clone("tmpLeft");    histoLeft->Scale(1.0/shapeChan_SI.xsections[ histoLeft->GetTitle()]);
           TH1* histoRight = (TH1*) shapeChan_SI.signal[rightIndex]->Clone("tmpRight");  histoRight->Scale(1.0/shapeChan_SI.xsections[histoRight->GetTitle()]);           
           TString newName = signalTypes[t]+"("; newName+= mass;
           TH1* histoNew = (TH1*)histoLeft->Clone(TString(histoLeft->GetTitle()).ReplaceAll(LeftName,newName));         
           histoNew->SetTitle(histoNew->GetName());
           histoNew->Scale(1-Ratio);
           histoNew->Add(shapeChan_SI.signal[rightIndex],Ratio);

           setTGraph(histoNew->GetTitle(), systpostfix );
           double XSection = TG_xsec->Eval(mass,NULL,"S");
           histoNew->Scale(XSection);
           histoNew->SetBinError(0,0.15*histoNew->Integral());
           shapeChan_SI.signal.insert(shapeChan_SI.signal.begin()+leftIndex,histoNew);

           

           //systematics
           std::vector<std::pair<TString, TH1*> > varsLeft  = shapeChan_SI.signalVars[histoLeft->GetTitle()];
           std::vector<std::pair<TString, TH1*> > varsRight = shapeChan_SI.signalVars[histoRight->GetTitle()];
           std::vector<std::pair<TString, TH1*> > varsNew;
           for(size_t v=0;v<varsLeft.size();v++){
              TH1* histoSystLeft  = (TH1*) varsLeft [v].second->Clone("tmpSystLeft");    histoSystLeft ->Scale(1.0/shapeChan_SI.xsections[histoLeft ->GetTitle()]);
              TH1* histoSystRight = (TH1*) varsRight[v].second->Clone("tmpSystRight");   histoSystRight->Scale(1.0/shapeChan_SI.xsections[histoRight->GetTitle()]);
              TH1* histoSystNew   = (TH1*)histoSystLeft->Clone(TString(histoSystLeft->GetTitle()).ReplaceAll(LeftName,newName));
              histoSystNew->SetTitle(histoSystNew->GetName());
              histoSystNew->Scale(1-Ratio);
              histoSystNew->Add(histoSystRight,Ratio);
              histoSystNew->Scale(XSection);
              varsNew.push_back(std::make_pair<TString, TH1*>(varsLeft[v].first,histoSystNew));
              delete histoSystLeft;
              delete histoSystRight;
           }
           shapeChan_SI.signalVars[histoNew->GetTitle()] = varsNew;

           delete histoLeft;
           delete histoRight;
      }}

   }
}
*/

void SignalInterpolation(std::vector<TString>& selCh,map<TString, Shape_t>& allShapesL, map<TString, Shape_t>& allShapes, map<TString, Shape_t>& allShapesR, TString mainHisto){
   if(massL<0 || massR<0 || massL==massR)return;

   std::vector<TString> signalTypes;
   signalTypes.push_back("ggH");
   signalTypes.push_back("qqH");
   for(unsigned int t=0;t<signalTypes.size();t++){
      printf("MASS: %i will be interpolated from %i and %i for %s\n",mass, massL,massR,signalTypes[t].Data());

      TString nameL = signalTypes[t]+"(";nameL+=massL;
      TString nameR = signalTypes[t]+"(";nameR+=massR;

      for(size_t i=0;i<selCh.size();i++){
      for(size_t b=0; b<AnalysisBins.size(); b++){
           Shape_t& shapeL = allShapesL.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;
           Shape_t& shape  = allShapes .find(selCh[i]+AnalysisBins[b]+mainHisto)->second;
           Shape_t& shapeR = allShapesR.find(selCh[i]+AnalysisBins[b]+mainHisto)->second;


           int indexL=0, indexR=0;
           for(unsigned int i=0;i<shapeL.signal.size();i++){if(TString(shapeL.signal[i]->GetTitle()).BeginsWith(nameL))indexL=i;}
           for(unsigned int i=0;i<shapeR.signal.size();i++){if(TString(shapeR.signal[i]->GetTitle()).BeginsWith(nameR))indexR=i;}

           double Ratio = (mass - massL); Ratio/=(massR - massL);

           //centralValue
           TH1* histoL = (TH1*) shapeL.signal[indexL]->Clone("tmpLeft");   histoL->Scale(1.0/shapeL.xsections[histoL->GetTitle()]);
           TH1* histoR = (TH1*) shapeR.signal[indexR]->Clone("tmpRight");  histoR->Scale(1.0/shapeR.xsections[histoR->GetTitle()]);           
           TString newName = signalTypes[t]+"("; newName+= mass;
           TH1* histoNew = (TH1*)histoL->Clone(TString(histoL->GetTitle()).ReplaceAll(nameL,newName));         
           histoNew->SetTitle(histoNew->GetName());
           histoNew->Scale(1-Ratio);
           histoNew->Add(histoR,Ratio);

           setTGraph(histoNew->GetTitle(), systpostfix );
           double XSection = TG_xsec->Eval(mass,NULL,"S");
           histoNew->Scale(XSection);
           histoNew->SetBinError(0,0.15*histoNew->Integral());
           shape.signal.insert(shape.signal.begin()+indexL,histoNew);

           
           //systematics
           std::vector<std::pair<TString, TH1*> > varsLeft  = shapeL.signalVars[histoL->GetTitle()];
           std::vector<std::pair<TString, TH1*> > varsRight = shapeR.signalVars[histoR->GetTitle()];
           std::vector<std::pair<TString, TH1*> > varsNew;
           for(size_t v=0;v<varsLeft.size();v++){
              TH1* histoSystL = (TH1*) varsLeft [v].second->Clone("tmpSystLeft");    histoSystL->Scale(1.0/shapeL.xsections[histoL->GetTitle()]);
              TH1* histoSystR = (TH1*) varsRight[v].second->Clone("tmpSystRight");   histoSystR->Scale(1.0/shapeR.xsections[histoR->GetTitle()]);
              TH1* histoSystNew   = (TH1*)histoSystL->Clone(TString(histoSystL->GetTitle()).ReplaceAll(nameL,newName));
              histoSystNew->SetTitle(histoSystNew->GetName());
              histoSystNew->Scale(1-Ratio);
              histoSystNew->Add(histoSystR,Ratio);
              histoSystNew->Scale(XSection);
              //varsNew.push_back(std::make_pair<TString, TH1*>(varsLeft[v].first,histoSystNew));
	      varsNew.push_back(std::pair<TString, TH1*>(varsLeft[v].first,histoSystNew));
              delete histoSystL;
              delete histoSystR;
           }
           shape.signalVars[histoNew->GetTitle()] = varsNew;

           delete histoL;
           delete histoR;
      }}

   }
}





//TGraph *ggH7TG_xsec=NULL, *ggH7TG_errp=NULL, *ggH7TG_errm=NULL, *ggH7TG_scap=NULL, *ggH7TG_scam=NULL, *ggH7TG_pdfp=NULL, *ggH7TG_pdfm=NULL;
//TGraph *qqH7TG_xsec=NULL, *qqH7TG_errp=NULL, *qqH7TG_errm=NULL, *qqH7TG_scap=NULL, *qqH7TG_scam=NULL, *qqH7TG_pdfp=NULL, *qqH7TG_pdfm=NULL;
//TGraph *ggH8TG_xsec=NULL, *ggH8TG_errp=NULL, *ggH8TG_errm=NULL, *ggH8TG_scap=NULL, *ggH8TG_scam=NULL, *ggH8TG_pdfp=NULL, *ggH8TG_pdfm=NULL;
//TGraph *qqH8TG_xsec=NULL, *qqH8TG_errp=NULL, *qqH8TG_errm=NULL, *qqH8TG_scap=NULL, *qqH8TG_scam=NULL, *qqH8TG_pdfp=NULL, *qqH8TG_pdfm=NULL;
//TGraph *    TG_xsec=NULL, *    TG_errp=NULL, *    TG_errm=NULL, *    TG_scap=NULL, *    TG_scam=NULL, *    TG_pdfp=NULL, *    TG_pdfm=NULL;
void setTGraph(TString proc, TString suffix){
         if( suffix.Contains('8') &&  proc.Contains("qqH")){ TG_xsec=qqH8TG_xsec;    TG_errp=qqH8TG_errp;    TG_errm=qqH8TG_errm;    TG_scap=qqH8TG_scap;    TG_scam=qqH8TG_scam;    TG_pdfp=qqH8TG_pdfp;    TG_pdfm=qqH8TG_pdfm;
   }else if( suffix.Contains('8') && !proc.Contains("qqH")){ TG_xsec=ggH8TG_xsec;    TG_errp=ggH8TG_errp;    TG_errm=ggH8TG_errm;    TG_scap=ggH8TG_scap;    TG_scam=ggH8TG_scam;    TG_pdfp=ggH8TG_pdfp;    TG_pdfm=ggH8TG_pdfm;
   }else if(!suffix.Contains('8') &&  proc.Contains("qqH")){ TG_xsec=qqH7TG_xsec;    TG_errp=qqH7TG_errp;    TG_errm=qqH7TG_errm;    TG_scap=qqH7TG_scap;    TG_scam=qqH7TG_scam;    TG_pdfp=qqH7TG_pdfp;    TG_pdfm=qqH7TG_pdfm;
   }else if(!suffix.Contains('8') && !proc.Contains("qqH")){ TG_xsec=ggH7TG_xsec;    TG_errp=ggH7TG_errp;    TG_errm=ggH7TG_errm;    TG_scap=ggH7TG_scap;    TG_scam=ggH7TG_scam;    TG_pdfp=ggH7TG_pdfp;    TG_pdfm=ggH7TG_pdfm;
   }
}
void initializeTGraph(){
   double ggH7_mass [] = {90.0, 95.0, 100.0, 105.0, 110.0, 110.5, 111.0, 111.5, 112.0, 112.5, 113.0, 113.5, 114.0, 114.5, 115.0, 115.5, 116.0, 116.5, 117.0, 117.5, 118.0, 118.5, 119.0, 119.5, 120.0, 120.5, 121.0, 121.5, 122.0, 122.5, 123.0, 123.5, 124.0, 124.5, 125.0, 125.5, 126.0, 126.5, 127.0, 127.5, 128.0, 128.5, 129.0, 129.5, 130.0, 130.5, 131.0, 131.5, 132.0, 132.5, 133.0, 133.5, 134.0, 134.5, 135.0, 135.5, 136.0, 136.5, 137.0, 137.5, 138.0, 138.5, 139.0, 139.5, 140.0, 141.0, 142.0, 143.0, 144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0, 152.0, 153.0, 154.0, 155.0, 156.0, 157.0, 158.0, 159.0, 160.0, 162.0, 164.0, 166.0, 168.0, 170.0, 172.0, 174.0, 176.0, 178.0, 180.0, 182.0, 184.0, 186.0, 188.0, 190.0, 192.0, 194.0, 196.0, 198.0, 200.0, 202.0, 204.0, 206.0, 208.0, 210.0, 212.0, 214.0, 216.0, 218.0, 220.0, 222.0, 224.0, 226.0, 228.0, 230.0, 232.0, 234.0, 236.0, 238.0, 240.0, 242.0, 244.0, 246.0, 248.0, 250.0, 252.0, 254.0, 256.0, 258.0, 260.0, 262.0, 264.0, 266.0, 268.0, 270.0, 272.0, 274.0, 276.0, 278.0, 280.0, 282.0, 284.0, 286.0, 288.0, 290.0, 295.0, 300.0, 305.0, 310.0, 315.0, 320.0, 325.0, 330.0, 335.0, 340.0, 345.0, 350.0, 360.0, 370.0, 380.0, 390.0, 400.0, 420.0, 440.0, 460.0, 480.0, 500.0, 520.0, 540.0, 560.0, 580.0, 600.0, 620.0, 640.0, 660.0, 680.0, 700.0, 720.0, 740.0, 760.0, 780.0, 800.0, 820.0, 840.0, 860.0, 880.0, 900.0, 920.0, 940.0, 960.0, 980.0, 1000.0};
   double ggH7_xsec [] = {29.47, 26.58, 24.02, 21.78, 19.84, 19.65, 19.48, 19.30, 19.13, 18.95, 18.79, 18.62, 18.45, 18.29, 18.13, 17.97, 17.82, 17.66, 17.51, 17.36, 17.21, 17.06, 16.92, 16.78, 16.63, 16.49, 16.36, 16.22, 16.08, 15.94, 15.82, 15.69, 15.56, 15.43, 15.31, 15.18, 15.06, 14.94, 14.82, 14.70, 14.58, 14.46, 14.35, 14.23, 14.12, 14.01, 13.90, 13.80, 13.69, 13.58, 13.48, 13.37, 13.28, 13.18, 13.08, 12.98, 12.88, 12.78, 12.68, 12.60, 12.50, 12.40, 12.31, 12.22, 12.13, 11.95, 11.78, 11.60, 11.44, 11.27, 11.12, 10.96, 10.80, 10.65, 10.50, 10.36, 10.21, 10.07, 9.934, 9.795, 9.655, 9.514, 9.367, 9.225, 9.080, 8.770, 8.465, 8.190, 7.952, 7.729, 7.515, 7.310, 7.112, 6.923, 6.739, 6.553, 6.379, 6.212, 6.050, 5.896, 5.751, 5.616, 5.487, 5.364, 5.249, 5.136, 5.027, 4.924, 4.822, 4.723, 4.630, 4.539, 4.454, 4.369, 4.288, 4.207, 4.128, 4.053, 3.980, 3.908, 3.839, 3.771, 3.707, 3.643, 3.581, 3.523, 3.468, 3.414, 3.362, 3.312, 3.261, 3.212, 3.164, 3.118, 3.072, 3.028, 2.984, 2.944, 2.903, 2.864, 2.828, 2.793, 2.760, 2.728, 2.696, 2.664, 2.633, 2.603, 2.574, 2.546, 2.480, 2.422, 2.369, 2.322, 2.281, 2.247, 2.221, 2.204, 2.195, 2.198, 2.225, 2.306, 2.361, 2.341, 2.266, 2.158, 2.032, 1.756, 1.482, 1.237, 1.026, 0.8491, 0.7006, 0.5782, 0.4771, 0.3944, 0.3267, 0.2713, 0.2257, 0.1883, 0.1574, 0.1320, 0.1109, 0.09335, 0.07883, 0.06668, 0.05655, 0.04806, 0.04089, 0.03490, 0.02982, 0.02555, 0.02193, 0.01885, 0.01624, 0.01400, 0.01210};
   double ggH7_errp [] = {22.9, 21.9, 21.2, 20.8, 20.4, 20.4, 20.3, 20.3, 20.3, 20.3, 20.2, 20.2, 20.2, 20.1, 20.0, 20.0, 20.0, 19.9, 19.9, 19.9, 19.8, 19.8, 19.7, 19.7, 19.7, 19.7, 19.7, 19.7, 19.7, 19.6, 19.5, 19.5, 19.5, 19.5, 19.5, 19.5, 19.4, 19.4, 19.4, 19.3, 19.3, 19.3, 19.3, 19.2, 19.2, 19.2, 19.2, 19.1, 19.1, 19.0, 19.0, 19.0, 18.9, 18.9, 18.9, 18.9, 18.9, 18.9, 18.9, 18.7, 18.8, 18.8, 18.8, 18.8, 18.8, 18.8, 18.8, 18.7, 18.7, 18.7, 18.7, 18.7, 18.7, 18.7, 18.7, 18.7, 18.6, 18.6, 18.5, 18.5, 18.6, 18.6, 18.6, 18.6, 18.6, 18.5, 18.2, 18.0, 17.9, 17.9, 17.9, 17.9, 18.0, 18.0, 18.1, 17.9, 17.5, 17.3, 17.3, 17.3, 17.3, 17.2, 17.2, 17.2, 17.2, 17.1, 17.1, 16.9, 16.9, 16.9, 16.9, 16.9, 16.9, 16.9, 16.8, 16.8, 16.7, 16.7, 16.6, 16.6, 16.6, 16.6, 16.7, 16.7, 16.7, 16.7, 16.7, 16.6, 16.6, 16.5, 16.5, 16.4, 16.3, 16.3, 16.2, 16.2, 16.3, 16.2, 16.2, 16.2, 16.2, 16.1, 16.1, 16.1, 16.0, 16.0, 16.0, 16.0, 16.1, 16.1, 16.6, 16.8, 16.8, 16.8, 16.8, 16.9, 17.1, 17.2, 17.4, 18.1, 19.5, 19.7, 19.4, 18.2, 17.1, 16.4, 15.7, 14.8, 15.6, 16.4, 17.1, 17.7, 18.1, 18.4, 18.7, 19.1, 19.4, 19.6, 20.0, 20.3, 20.7, 20.9, 21.1, 21.5, 21.7, 22.0, 22.2, 22.5, 23.1, 23.4, 24.1, 24.6, 25.1, 25.6, 26.0, 26.6, 27.1};
   double ggH7_errm [] = {-15.6, -15.9, -15.6, -15.5, -15.3, -15.3, -15.3, -15.3, -15.3, -15.2, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.0, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.2, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -15.1, -14.9, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -14.9, -15.0, -15.0, -15.0, -15.0, -15.0, -15.0, -14.9, -14.8, -14.7, -14.9, -14.9, -14.9, -14.8, -14.7, -14.7, -14.7, -14.6, -14.9, -15.0, -15.1, -15.0, -15.1, -15.1, -15.2, -15.1, -15.2, -15.2, -15.2, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.3, -15.4, -15.4, -15.5, -15.5, -15.5, -15.4, -15.5, -15.5, -15.4, -15.4, -15.5, -15.6, -15.6, -15.6, -15.7, -15.7, -15.8, -15.8, -15.9, -15.8, -15.8, -15.9, -15.8, -15.8, -15.9, -15.9, -16.1, -16.1, -16.2, -16.2, -16.2, -16.2, -16.1, -16.1, -15.8, -15.5, -15.6, -15.6, -15.8, -15.8, -15.8, -15.8, -15.7, -15.3, -14.8, -14.8, -14.6, -15.1, -15.6, -16.0, -16.3, -16.9, -17.2, -17.2, -17.5, -17.6, -17.6, -17.7, -17.8, -17.8, -17.9, -18.0, -18.0, -18.3, -18.3, -18.4, -18.6, -18.6, -18.8, -18.9, -19.1, -19.4, -19.6, -19.9, -20.2, -20.6, -21.1, -21.4, -21.9, -22.2, -22.6};
   double ggH7_scap [] = {14.8, 13.9, 13.3, 12.8, 12.5, 12.5, 12.4, 12.4, 12.4, 12.3, 12.3, 12.3, 12.2, 12.1, 12.1, 12.1, 12.1, 12.0, 12.0, 12.0, 12.0, 12.0, 11.9, 11.9, 11.9, 11.9, 11.9, 11.8, 11.8, 11.8, 11.7, 11.7, 11.7, 11.7, 11.7, 11.7, 11.6, 11.6, 11.6, 11.5, 11.5, 11.5, 11.5, 11.3, 11.3, 11.3, 11.3, 11.3, 11.3, 11.2, 11.2, 11.2, 11.1, 11.1, 11.1, 11.1, 11.1, 11.1, 11.1, 10.9, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 11.0, 10.9, 10.9, 10.9, 10.9, 10.9, 10.9, 10.9, 10.9, 10.9, 10.8, 10.9, 10.8, 10.8, 10.9, 10.9, 10.9, 10.9, 10.9, 10.8, 10.5, 10.3, 10.2, 10.2, 10.2, 10.2, 10.3, 10.3, 10.3, 10.2, 9.8, 9.6, 9.6, 9.6, 9.6, 9.5, 9.5, 9.4, 9.4, 9.3, 9.3, 9.2, 9.2, 9.2, 9.2, 9.2, 9.1, 9.1, 9.0, 9.0, 8.9, 8.9, 8.8, 8.8, 8.8, 8.8, 8.8, 8.8, 8.9, 8.8, 8.8, 8.7, 8.6, 8.6, 8.5, 8.5, 8.3, 8.3, 8.3, 8.3, 8.3, 8.2, 8.2, 8.2, 8.2, 8.1, 8.1, 8.0, 7.9, 7.9, 7.9, 7.9, 8.0, 8.0, 8.4, 8.6, 8.6, 8.5, 8.5, 8.6, 8.7, 8.8, 9.0, 9.6, 11.0, 11.1, 10.8, 9.5, 8.5, 7.6, 6.8, 5.8, 6.4, 7.0, 7.6, 8.0, 8.3, 8.5, 8.7, 8.9, 9.0, 9.1, 9.4, 9.5, 9.8, 9.9, 10.0, 10.3, 10.4, 10.6, 10.7, 10.8, 10.9, 11.0, 11.3, 11.5, 11.6, 11.9, 11.9, 12.1, 12.3};
   double ggH7_scam [] = {-8.7, -9.0, -8.6, -8.5, -8.2, -8.2, -8.2, -8.2, -8.2, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.0, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.7, -7.7, -7.7, -7.7, -7.7, -7.6, -7.6, -7.7, -7.7, -7.5, -7.5, -7.5, -7.6, -7.6, -7.6, -7.6, -7.6, -7.5, -7.5, -7.6, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.2, -7.2, -7.2, -7.2, -7.3, -7.2, -7.2, -7.3, -7.3, -7.3, -7.2, -7.2, -7.2, -7.1, -6.9, -6.8, -6.9, -7.0, -6.9, -6.9, -6.7, -6.7, -6.7, -6.6, -6.9, -7.0, -7.1, -7.0, -7.1, -7.1, -7.2, -7.1, -7.2, -7.2, -7.2, -7.3, -7.3, -7.2, -7.3, -7.3, -7.2, -7.2, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.2, -7.3, -7.3, -7.3, -7.4, -7.4, -7.4, -7.5, -7.5, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.5, -7.6, -7.6, -7.7, -7.8, -7.9, -7.9, -7.9, -7.8, -7.7, -7.7, -7.3, -7.1, -7.1, -7.2, -7.3, -7.2, -7.3, -7.3, -7.1, -6.7, -6.1, -6.2, -5.9, -6.3, -6.9, -7.3, -7.7, -8.2, -8.4, -8.4, -8.5, -8.6, -8.6, -8.5, -8.5, -8.4, -8.3, -8.3, -8.2, -8.3, -8.2, -8.3, -8.3, -8.2, -8.3, -8.3, -8.4, -8.4, -8.3, -8.4, -8.4, -8.4, -8.5, -8.5, -8.6, -8.6, -8.6};
   double ggH7_pdfp [] = {8.1, 8.0, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.7, 7.8, 7.8, 7.8, 7.8, 7.7, 7.7, 7.7, 7.7, 7.7, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.8, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 7.9, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.1, 8.1, 8.1, 8.1, 8.1, 8.1, 8.1, 8.1, 8.2, 8.2, 8.2, 8.2, 8.3, 8.3, 8.3, 8.4, 8.4, 8.5, 8.5, 8.6, 8.6, 8.6, 8.6, 8.8, 8.9, 9.1, 9.3, 9.4, 9.5, 9.7, 9.8, 9.9, 10.0, 10.2, 10.4, 10.5, 10.6, 10.8, 10.9, 11.0, 11.1, 11.2, 11.3, 11.4, 11.5, 11.8, 12.1, 12.4, 12.7, 13.0, 13.4, 13.8, 14.1, 14.4, 14.8};
   double ggH7_pdfm [] = {-6.9, -6.9, -7.0, -7.1, -7.1, -7.1, -7.1, -7.1, -7.1, -7.1, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.2, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.4, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.6, -7.6, -7.6, -7.6, -7.6, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.8, -7.8, -7.8, -7.8, -7.9, -7.9, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.3, -8.4, -8.4, -8.4, -8.4, -8.4, -8.4, -8.5, -8.5, -8.5, -8.5, -8.6, -8.6, -8.6, -8.7, -8.7, -8.7, -8.8, -8.7, -8.7, -8.7, -8.7, -8.8, -8.8, -8.9, -9.0, -9.1, -9.2, -9.3, -9.4, -9.6, -9.7, -9.8, -10.0, -10.1, -10.2, -10.3, -10.4, -10.5, -10.6, -10.7, -10.9, -11.3, -11.6, -11.9, -12.2, -12.6, -12.9, -13.3, -13.6, -14.0};


   double qqH7_mass [] = {90.0, 95.0, 100.0, 105.0, 110.0, 110.5, 111.0, 111.5, 112.0, 112.5, 113.0, 113.5, 114.0, 114.5, 115.0, 115.5, 116.0, 116.5, 117.0, 117.5, 118.0, 118.5, 119.0, 119.5, 120.0, 120.5, 121.0, 121.5, 122.0, 122.5, 123.0, 123.5, 124.0, 124.5, 125.0, 125.5, 126.0, 126.5, 127.0, 127.5, 128.0, 128.5, 129.0, 129.5, 130.0, 130.5, 131.0, 131.5, 132.0, 132.5, 133.0, 133.5, 134.0, 134.5, 135.0, 135.5, 136.0, 136.5, 137.0, 137.5, 138.0, 138.5, 139.0, 139.5, 140.0, 141.0, 142.0, 143.0, 144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0, 152.0, 153.0, 154.0, 155.0, 156.0, 157.0, 158.0, 159.0, 160.0, 162.0, 164.0, 166.0, 168.0, 170.0, 172.0, 174.0, 176.0, 178.0, 180.0, 182.0, 184.0, 186.0, 188.0, 190.0, 192.0, 194.0, 196.0, 198.0, 200.0, 202.0, 204.0, 206.0, 208.0, 210.0, 212.0, 214.0, 216.0, 218.0, 220.0, 222.0, 224.0, 226.0, 228.0, 230.0, 232.0, 234.0, 236.0, 238.0, 240.0, 242.0, 244.0, 246.0, 248.0, 250.0, 252.0, 254.0, 256.0, 258.0, 260.0, 262.0, 264.0, 266.0, 268.0, 270.0, 272.0, 274.0, 276.0, 278.0, 280.0, 282.0, 284.0, 286.0, 288.0, 290.0, 295.0, 300.0, 305.0, 310.0, 315.0, 320.0, 325.0, 330.0, 335.0, 340.0, 345.0, 350.0, 360.0, 370.0, 380.0, 390.0, 400.0, 420.0, 440.0, 460.0, 480.0, 500.0, 520.0, 540.0, 560.0, 580.0, 600.0, 620.0, 640.0, 660.0, 680.0, 700.0, 720.0, 740.0, 760.0, 780.0, 800.0, 820.0, 840.0, 860.0, 880.0, 900.0, 920.0, 940.0, 960.0, 980.0, 1000.0};
   double qqH7_xsec [] = {1.710, 1.628, 1.546, 1.472, 1.398, 1.391, 1.384, 1.378, 1.371, 1.364, 1.358, 1.351, 1.345, 1.339, 1.332, 1.326, 1.319, 1.313, 1.307, 1.300, 1.294, 1.288, 1.282, 1.276, 1.269, 1.263, 1.257, 1.251, 1.246, 1.240, 1.234, 1.228, 1.222, 1.216, 1.211, 1.205, 1.199, 1.193, 1.188, 1.182, 1.176, 1.171, 1.165, 1.159, 1.154, 1.148, 1.143, 1.137, 1.132, 1.126, 1.121, 1.115, 1.110, 1.105, 1.100, 1.095, 1.090, 1.085, 1.080, 1.076, 1.071, 1.066, 1.062, 1.057, 1.052, 1.043, 1.033, 1.023, 1.013, 1.004, 0.9951, 0.9866, 0.9782, 0.9699, 0.9617, 0.9529, 0.9441, 0.9353, 0.9266, 0.9180, 0.9095, 0.9013, 0.8934, 0.8859, 0.8787, 0.8676, 0.8571, 0.8453, 0.8316, 0.8173, 0.8029, 0.7885, 0.7744, 0.7609, 0.7480, 0.7361, 0.7248, 0.7139, 0.7032, 0.6925, 0.6812, 0.6699, 0.6587, 0.6478, 0.6371, 0.6267, 0.6164, 0.6064, 0.5965, 0.5869, 0.5775, 0.5684, 0.5594, 0.5506, 0.5420, 0.5335, 0.5252, 0.5170, 0.5089, 0.5011, 0.4934, 0.4859, 0.4785, 0.4712, 0.4641, 0.4572, 0.4503, 0.4436, 0.4369, 0.4304, 0.4239, 0.4174, 0.4111, 0.4049, 0.3988, 0.3931, 0.3875, 0.3821, 0.3767, 0.3715, 0.3663, 0.3611, 0.3560, 0.3510, 0.3461, 0.3413, 0.3365, 0.3318, 0.3271, 0.3226, 0.3116, 0.3011, 0.2908, 0.2809, 0.2716, 0.2627, 0.2539, 0.2453, 0.2368, 0.2286, 0.2206, 0.2132, 0.2018, 0.1910, 0.1808, 0.1712, 0.1620, 0.1451, 0.1304, 0.1171, 0.1054, 0.09497, 0.08568, 0.07746, 0.07010, 0.06353, 0.05771, 0.05246, 0.04776, 0.04356, 0.03977, 0.03637, 0.03330, 0.03052, 0.02805, 0.02580, 0.02373, 0.02188, 0.02018, 0.01864, 0.01724, 0.01597, 0.01479, 0.01375, 0.01275, 0.01186, 0.01104};
   double qqH7_errp [] = {2.7, 2.5, 2.6, 2.5, 2.8, 2.8, 2.8, 2.7, 2.7, 2.7, 2.6, 2.6, 2.6, 2.6, 2.5, 2.6, 2.6, 2.6, 2.6, 2.7, 2.7, 2.7, 2.7, 2.7, 2.8, 2.7, 2.8, 2.8, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.9, 2.9, 2.9, 2.9, 2.9, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.9, 2.9, 2.9, 2.9, 2.9, 2.8, 2.8, 2.9, 2.9, 3.0, 3.1, 3.1, 3.1, 3.0, 3.0, 3.0, 2.9, 3.0, 3.0, 3.1, 3.1, 3.1, 3.1, 3.0, 3.0, 2.9, 2.9, 3.0, 3.1, 3.1, 3.1, 3.1, 3.2, 3.2, 3.2, 3.1, 3.1, 3.2, 3.4, 3.4, 3.3, 3.3, 3.3, 3.4, 3.4, 3.4, 3.4, 3.4, 3.4, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.6, 3.6, 3.7, 3.7, 3.8, 3.8, 3.8, 3.8, 3.8, 3.8, 3.8, 3.8, 3.9, 3.9, 4.0, 4.0, 4.1, 4.2, 4.2, 4.3, 4.2, 4.2, 4.2, 4.2, 4.2, 4.2, 4.2, 4.3, 4.3, 4.3, 4.4, 4.4, 4.4, 4.5, 4.5, 4.5, 4.6, 4.7, 4.8, 4.8, 4.9, 5.0, 5.0, 5.0, 5.1, 5.2, 5.2, 5.3, 5.5, 5.7, 5.8, 5.9, 6.2, 6.5, 6.7, 7.0, 7.2, 7.5, 7.8, 8.0, 8.3, 8.6, 8.9, 9.2, 9.4, 9.7, 9.9, 10.2, 10.5, 10.8, 11.0, 11.3, 11.5, 11.8, 12.1, 12.3, 12.6, 12.9, 13.3, 13.6, 13.9, 14.2};
   double qqH7_errm [] = {-2.3, -2.5, -2.4, -2.4, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.5, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.3, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.1, -2.1, -2.1, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.1, -2.1, -2.1, -2.1, -2.2, -2.2, -2.2, -2.3, -2.2, -2.2, -2.2, -2.2, -2.2, -2.2, -2.1, -2.2, -2.3, -2.4, -2.3, -2.2, -2.2, -2.2, -2.2, -2.3, -2.4, -2.4, -2.4, -2.3, -2.3, -2.3, -2.3, -2.4, -2.4, -2.4, -2.4, -2.4, -2.4, -2.5, -2.5, -2.5, -2.5, -2.4, -2.4, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.6, -2.6, -2.6, -2.6, -2.6, -2.5, -2.5, -2.4, -2.4, -2.4, -2.5, -2.5, -2.5, -2.6, -2.6, -2.6, -2.6, -2.7, -2.7, -2.7, -2.7, -2.7, -2.6, -2.6, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.8, -2.8, -2.9, -2.9, -2.9, -3.0, -3.0, -3.0, -3.0, -3.0, -3.1, -3.1, -3.2, -3.3, -3.4, -3.4, -3.5, -3.5, -3.7, -3.8, -3.8, -3.8, -3.9, -4.0, -4.0, -4.1, -4.2, -4.2, -4.3, -4.3, -4.4, -4.5, -4.5, -4.6, -4.6, -4.7, -4.7, -4.8, -4.8, -4.9};
   double qqH7_scap [] = {0.6, 0.4, 0.4, 0.3, 0.5, 0.5, 0.4, 0.4, 0.4, 0.3, 0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.5, 0.4, 0.4, 0.4, 0.4, 0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.3, 0.3, 0.4, 0.4, 0.4, 0.3, 0.3, 0.3, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.3, 0.2, 0.2, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.1, 0.0, 0.1, 0.3, 0.3, 0.2, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 0.4, 0.5, 0.6, 0.6, 0.6, 0.7, 0.7, 0.8, 0.8, 0.9, 1.0, 1.0, 1.1, 1.1, 1.2, 1.2, 1.3, 1.4, 1.4, 1.4, 1.5, 1.5, 1.5, 1.6, 1.7, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2};
   double qqH7_scam [] = {-0.2, -0.4, -0.3, -0.3, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.4, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2, -0.2, -0.1, -0.1, -0.1, -0.1, -0.0, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.0, -0.1, -0.1, -0.1, -0.2, -0.2, -0.2, -0.1, -0.1, -0.1, -0.2, -0.1, -0.1, -0.1, -0.2, -0.3, -0.2, -0.2, -0.1, -0.2, -0.2, -0.3, -0.4, -0.4, -0.3, -0.2, -0.3, -0.3, -0.3, -0.3, -0.3, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.4, -0.5, -0.5, -0.5, -0.5, -0.5, -0.6, -0.6, -0.6, -0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.4, -0.5, -0.5, -0.5, -0.6, -0.6, -0.6, -0.6, -0.7, -0.7, -0.7, -0.7, -0.7, -0.7, -0.7, -0.7, -0.8, -0.8, -0.8, -0.8, -0.7, -0.8, -0.9, -0.9, -0.9, -1.0, -1.0, -1.1, -1.1, -1.1, -1.1, -1.2, -1.2, -1.3, -1.4, -1.5, -1.6, -1.7, -1.7, -1.8, -1.9, -2.0, -2.1, -2.1, -2.2, -2.3, -2.4, -2.5, -2.6, -2.7, -2.7, -2.8, -2.8, -2.9, -3.0, -3.1, -3.2, -3.2, -3.3, -3.3, -3.4, -3.5};
   double qqH7_pdfp [] = {2.1, 2.1, 2.2, 2.2, 2.3, 2.3, 2.3, 2.3, 2.3, 2.3, 2.3, 2.3, 2.3, 2.3, 2.3, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.4, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.6, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.7, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 2.9, 2.9, 2.9, 2.9, 3.0, 3.0, 3.0, 3.0, 3.0, 3.1, 3.1, 3.1, 3.1, 3.2, 3.2, 3.2, 3.2, 3.2, 3.3, 3.3, 3.3, 3.3, 3.3, 3.4, 3.4, 3.4, 3.4, 3.5, 3.5, 3.5, 3.5, 3.5, 3.6, 3.6, 3.6, 3.6, 3.7, 3.7, 3.7, 3.7, 3.7, 3.8, 3.8, 3.8, 3.8, 3.8, 3.9, 3.9, 3.9, 3.9, 4.0, 4.0, 4.0, 4.0, 4.0, 4.1, 4.1, 4.1, 4.1, 4.2, 4.2, 4.2, 4.2, 4.2, 4.3, 4.3, 4.4, 4.4, 4.5, 4.5, 4.6, 4.6, 4.7, 4.8, 4.8, 4.9, 4.9, 5.0, 5.1, 5.2, 5.3, 5.5, 5.7, 5.9, 6.1, 6.3, 6.6, 6.8, 7.0, 7.2, 7.4, 7.6, 7.9, 8.1, 8.3, 8.5, 8.7, 8.9, 9.2, 9.4, 9.6, 9.8, 10.0, 10.3, 10.5, 10.7, 10.9, 11.1, 11.4, 11.6, 11.8, 12.0};
   double qqH7_pdfm [] = {-2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.1, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.9, -1.8, -1.8, -1.8, -1.8, -1.8, -1.8, -1.7, -1.7, -1.7, -1.7, -1.7, -1.7, -1.6, -1.6, -1.6, -1.6, -1.6, -1.6, -1.5, -1.5, -1.5, -1.5, -1.5, -1.5, -1.5, -1.4, -1.4, -1.4};


   double ggH8_mass [] = {80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0, 89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 110.5, 111.0, 111.5, 112.0, 112.5, 113.0, 113.5, 114.0, 114.5, 115.0, 115.5, 116.0, 116.5, 117.0, 117.5, 118.0, 118.5, 119.0, 119.5, 120.0, 120.5, 121.0, 121.5, 122.0, 122.5, 123.0, 123.5, 124.0, 124.5, 125.0, 125.5, 126.0, 126.5, 127.0, 127.5, 128.0, 128.5, 129.0, 129.5, 130.0, 130.5, 131.0, 131.5, 132.0, 132.5, 133.0, 133.5, 134.0, 134.5, 135.0, 135.5, 136.0, 136.5, 137.0, 137.5, 138.0, 138.5, 139.0, 139.5, 140.0, 141.0, 142.0, 143.0, 144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0, 152.0, 153.0, 154.0, 155.0, 156.0, 157.0, 158.0, 159.0, 160.0, 162.0, 164.0, 165.0, 166.0, 168.0, 170.0, 172.0, 174.0, 175.0, 176.0, 178.0, 180.0, 182.0, 184.0, 185.0, 186.0, 188.0, 190.0, 192.0, 194.0, 195.0, 196.0, 198.0, 200.0, 202.0, 204.0, 206.0, 208.0, 210.0, 212.0, 214.0, 216.0, 218.0, 220.0, 222.0, 224.0, 226.0, 228.0, 230.0, 232.0, 234.0, 236.0, 238.0, 240.0, 242.0, 244.0, 246.0, 248.0, 250.0, 252.0, 254.0, 256.0, 258.0, 260.0, 262.0, 264.0, 266.0, 268.0, 270.0, 272.0, 274.0, 276.0, 278.0, 280.0, 282.0, 284.0, 286.0, 288.0, 290.0, 295.0, 300.0, 305.0, 310.0, 315.0, 320.0, 325.0, 330.0, 335.0, 340.0, 345.0, 350.0, 360.0, 370.0, 380.0, 390.0, 400.0, 420.0, 440.0, 450.0, 460.0, 480.0, 500.0, 520.0, 540.0, 550.0, 560.0, 580.0, 600.0, 620.0, 640.0, 650.0, 660.0, 680.0, 700.0, 720.0, 740.0, 750.0, 760.0, 780.0, 800.0, 820.0, 840.0, 850.0, 860.0, 880.0, 900.0, 920.0, 940.0, 950.0, 960.0, 980.0, 1000.0};
   double ggH8_xsec [] = {46.12, 45.04, 43.99, 42.99, 42.01, 41.07, 40.17, 39.29, 38.44, 37.62, 36.80, 36.05, 35.30, 34.58, 33.87, 33.19, 32.53, 31.89, 31.27, 30.66, 30.12, 29.55, 28.99, 28.44, 27.92, 27.39, 26.89, 26.42, 25.95, 25.49, 25.04, 24.82, 24.60, 24.39, 24.18, 24.05, 23.76, 23.56, 23.36, 23.16, 22.96, 22.84, 22.58, 22.39, 22.20, 22.09, 21.90, 21.72, 21.55, 21.37, 21.20, 20.96, 20.86, 20.69, 20.53, 20.37, 20.21, 20.04, 19.89, 19.73, 19.57, 19.42, 19.27, 19.07, 18.97, 18.78, 18.67, 18.53, 18.39, 18.21, 18.11, 17.98, 17.84, 17.68, 17.55, 17.42, 17.29, 17.19, 17.07, 16.92, 16.82, 16.70, 16.60, 16.46, 16.36, 16.24, 16.13, 16.01, 15.89, 15.78, 15.67, 15.43, 15.20, 14.98, 14.79, 14.59, 14.40, 14.31, 14.11, 13.92, 13.65, 13.33, 13.12, 12.97, 12.86, 12.84, 12.92, 12.84, 12.52, 12.17, 11.98, 11.96, 11.27, 10.96, 10.69, 10.40, 10.15, 9.895, 9.637, 9.531, 9.429, 9.203, 8.923, 8.634, 8.410, 8.315, 8.219, 7.994, 7.984, 7.724, 7.443, 7.431, 7.430, 7.237, 7.127, 7.002, 6.885, 6.767, 6.644, 6.534, 6.421, 6.327, 6.249, 6.138, 6.038, 5.938, 5.851, 5.776, 5.680, 5.593, 5.513, 5.438, 5.352, 5.272, 5.183, 5.101, 5.020, 4.945, 4.874, 4.802, 4.734, 4.667, 4.602, 4.539, 4.479, 4.421, 4.366, 4.308, 4.253, 4.198, 4.149, 4.100, 4.053, 4.008, 3.964, 3.921, 3.880, 3.841, 3.803, 3.767, 3.683, 3.606, 3.539, 3.482, 3.434, 3.392, 3.364, 3.349, 3.349, 3.367, 3.405, 3.406, 3.390, 3.336, 3.235, 3.093, 2.924, 2.552, 2.180, 2.003, 1.837, 1.538, 1.283, 1.069, 0.8912, 0.8141, 0.7442, 0.6229, 0.5230, 0.4403, 0.3718, 0.3423, 0.3152, 0.2680, 0.2288, 0.1962, 0.1687, 0.1566, 0.1455, 0.1260, 0.1095, 0.09547, 0.08346, 0.07811, 0.07321, 0.06443, 0.05684, 0.05030, 0.04463, 0.04206, 0.03969, 0.03539, 0.03163};
   double ggH8_errp [] = {+16.7, +16.7, +16.6, +16.6, +16.5, +16.5, +16.4, +16.4, +16.3, +16.2, +16.1, +16.1, +16.0, +16.0, +15.9, +15.9, +15.9, +15.8, +15.8, +15.8, +15.7, +15.7, +15.7, +15.6, +15.6, +15.5, +15.4, +15.4, +15.4, +15.3, +15.3, +15.3, +15.2, +15.1, +15.1, +15.1, +15.1, +15.1, +15.1, +15.1, +15.0, +15.0, +14.9, +14.9, +14.9, +14.9, +14.9, +14.8, +14.8, +14.8, +14.8, +14.8, +14.8, +14.8, +14.8, +14.7, +14.7, +14.7, +14.7, +14.7, +14.7, +14.7, +14.7, +14.7, +14.6, +14.6, +14.6, +14.6, +14.6, +14.6, +14.6, +14.6, +14.6, +14.5, +14.5, +14.5, +14.4, +14.4, +14.4, +14.4, +14.4, +14.4, +14.3, +14.3, +14.3, +14.3, +14.3, +14.3, +14.3, +14.3, +14.3, +14.1, +14.1, +14.1, +14.1, +14.1, +14.0, +14.0, +14.0, +14.1, +14.1, +14.1, +14.0, +14.1, +14.1, +14.1, +14.1, +14.1, +14.0, +14.0, +14.0, +14.0, +13.9, +13.9, +13.9, +13.9, +13.9, +13.8, +13.7, +13.7, +13.7, +13.7, +13.6, +13.6, +13.6, +13.6, +13.5, +13.5, +13.5, +13.5, +13.5, +13.5, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.3, +13.3, +13.2, +13.2, +13.3, +13.3, +13.3, +13.3, +13.3, +13.2, +13.2, +13.2, +13.2, +13.2, +13.2, +13.2, +13.2, +13.2, +13.2, +13.3, +13.3, +13.3, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.3, +13.3, +13.3, +13.3, +13.3, +13.3, +13.3, +13.3, +13.4, +13.4, +13.4, +13.4, +13.4, +13.4, +13.5, +13.6, +13.6, +13.6, +13.7, +13.8, +13.9, +13.9, +14.0, +14.0, +14.1, +14.3, +14.4, +14.5, +14.7, +14.9, +15.0, +15.2, +15.2, +15.3, +15.4, +15.4, +15.5, +15.6, +15.6, +15.7, +15.9, +16.1, +16.2, +16.5, +16.5, +16.6, +16.6, +16.7, +16.9, +17.0, +17.1, +17.1, +17.3, +17.4, +17.5, +17.8, +17.9, +18.0, +18.3, +18.6};
   double ggH8_errm [] = {-15.9, -15.8, -15.7, -15.6, -15.6, -15.6, -15.5, -15.5, -15.4, -15.4, -15.4, -15.3, -15.2, -15.2, -15.1, -15.1, -15.1, -15.0, -15.0, -15.0, -14.9, -14.9, -14.9, -14.9, -14.8, -14.8, -14.9, -14.9, -14.8, -14.9, -14.9, -14.9, -14.9, -14.8, -14.8, -14.8, -14.8, -14.9, -14.9, -14.9, -14.9, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.8, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.6, -14.6, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.7, -14.6, -14.6, -14.6, -14.6, -14.6, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.5, -14.5, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.4, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.5, -14.4, -14.4, -14.4, -14.5, -14.5, -14.5, -14.4, -14.5, -14.5, -14.5, -14.5, -14.4, -14.5, -14.5, -14.4, -14.4, -14.3, -14.2, -14.2, -14.2, -14.2, -14.2, -14.2, -14.2, -14.2, -14.2, -14.2, -14.2, -14.1, -14.1, -14.1, -14.1, -14.1, -14.1, -14.1, -13.9, -13.8, -13.7, -13.7, -13.8, -13.9, -14.0, -14.0, -14.1, -14.1, -14.1, -14.2, -14.2, -14.2, -14.2, -14.1, -14.1, -14.1, -14.1, -14.1, -14.0, -13.9, -14.0, -14.0, -14.0, -14.1, -14.1, -14.0, -14.0, -14.1, -14.1, -14.1, -14.0, -13.8, -13.7, -13.6, -13.6, -13.7, -13.6, -13.6, -13.7, -13.6, -13.7, -13.7, -13.8, -13.8, -13.8, -13.8, -13.9, -14.0, -14.0, -14.2, -14.3, -14.4, -14.6, -14.7, -14.8, -14.9, -15.0, -15.0, -15.0, -15.1, -15.2, -15.3, -15.4, -15.5, -15.7, -16.0, -16.1, -16.2, -16.4, -16.7};
   double ggH8_scap [] = {+8.8, +8.8, +8.7, +8.7, +8.6, +8.6, +8.5, +8.5, +8.4, +8.4, +8.3, +8.3, +8.2, +8.2, +8.1, +8.1, +8.1, +8.0, +8.0, +8.0, +7.9, +7.9, +7.9, +7.8, +7.8, +7.8, +7.7, +7.7, +7.7, +7.6, +7.6, +7.6, +7.6, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.2, +7.2, +7.2, +7.2, +7.2, +7.2, +7.2, +7.2, +7.2, +7.1, +7.1, +7.1, +7.1, +7.1, +7.1, +7.1, +7.1, +7.1, +7.0, +7.0, +7.0, +7.0, +7.0, +7.0, +7.0, +7.0, +7.0, +6.9, +6.9, +6.9, +6.9, +6.9, +6.9, +6.9, +6.9, +6.9, +6.8, +6.8, +6.8, +6.8, +6.8, +6.7, +6.7, +6.7, +6.7, +6.7, +6.7, +6.6, +6.6, +6.6, +6.6, +6.6, +6.6, +6.5, +6.5, +6.5, +6.5, +6.4, +6.4, +6.4, +6.4, +6.4, +6.3, +6.3, +6.3, +6.3, +6.3, +6.2, +6.2, +6.2, +6.2, +6.1, +6.1, +6.1, +6.1, +6.1, +6.1, +6.0, +6.0, +6.0, +6.0, +6.0, +6.0, +6.0, +6.0, +6.0, +6.0, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.7, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.8, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +5.9, +6.0, +6.0, +6.0, +6.1, +6.1, +6.1, +6.1, +6.1, +6.2, +6.2, +6.2, +6.2, +6.3, +6.3, +6.3, +6.4, +6.4, +6.4, +6.5, +6.5};
   double ggH8_scam [] = {-9.2, -9.1, -9.1, -9.0, -9.0, -9.0, -8.9, -8.9, -8.8, -8.8, -8.8, -8.7, -8.7, -8.7, -8.6, -8.6, -8.6, -8.5, -8.5, -8.5, -8.4, -8.4, -8.4, -8.4, -8.3, -8.3, -8.3, -8.3, -8.2, -8.2, -8.2, -8.2, -8.2, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.1, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.9, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.8, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.6, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.4, -7.4, -7.4, -7.4, -7.4, -7.4, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.3, -7.2, -7.2, -7.2, -7.2, -7.2, -7.1, -7.1, -7.1, -7.1, -7.1, -7.0, -7.0, -7.0, -7.0, -7.0, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.8, -6.8, -6.8, -6.8, -6.8, -6.7, -6.7, -6.7, -6.7, -6.7, -6.6, -6.6, -6.6, -6.6, -6.6, -6.6, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.4, -6.4, -6.4, -6.4, -6.4, -6.4, -6.4, -6.3, -6.3, -6.3, -6.3, -6.3, -6.3, -6.3, -6.2, -6.2, -6.2, -6.2, -6.2, -6.2, -6.2, -6.2, -6.1, -6.1, -6.1, -6.1, -6.1, -6.1, -6.0, -6.0, -6.0, -6.0, -6.0, -6.0, -5.9, -5.9, -5.9, -5.9, -5.9, -5.8, -5.6, -5.5, -5.4, -5.3, -5.3, -5.2, -5.2, -5.2, -5.1, -5.1, -5.1, -5.1, -5.1, -5.1, -5.0, -5.0, -5.0, -5.0, -5.1, -5.1, -5.1, -5.1, -5.1, -5.1, -5.2, -5.2, -5.2, -5.2, -5.2, -5.3, -5.3, -5.3, -5.3, -5.3, -5.4, -5.4, -5.4, -5.4, -5.4};
   double ggH8_pdfp [] = {+7.9, +7.9, +7.9, +7.9, +7.9, +7.9, +7.9, +7.9, +7.9, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.8, +7.7, +7.7, +7.7, +7.7, +7.7, +7.7, +7.7, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.4, +7.4, +7.4, +7.4, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.5, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.4, +7.3, +7.3, +7.4, +7.4, +7.4, +7.4, +7.4, +7.3, +7.3, +7.3, +7.3, +7.3, +7.3, +7.4, +7.4, +7.4, +7.4, +7.5, +7.5, +7.5, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.6, +7.7, +7.7, +7.7, +7.7, +7.7, +7.7, +7.8, +7.9, +7.9, +7.9, +8.0, +8.0, +8.1, +8.1, +8.2, +8.2, +8.3, +8.5, +8.6, +8.7, +8.9, +9.1, +9.2, +9.4, +9.4, +9.4, +9.5, +9.5, +9.6, +9.7, +9.7, +9.8, +9.9, +10.1, +10.2, +10.4, +10.4, +10.5, +10.5, +10.6, +10.7, +10.8, +10.9, +10.9, +11.0, +11.1, +11.2, +11.4, +11.5, +11.6, +11.8, +12.1};
   double ggH8_pdfm [] = {-6.7, -6.7, -6.6, -6.6, -6.6, -6.6, -6.6, -6.6, -6.6, -6.6, -6.6, -6.6, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.5, -6.6, -6.6, -6.6, -6.7, -6.7, -6.7, -6.7, -6.7, -6.7, -6.7, -6.7, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.8, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -7.0, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -6.9, -7.0, -7.0, -7.0, -7.0, -7.1, -7.1, -7.1, -7.1, -7.1, -7.1, -7.1, -7.1, -7.1, -7.2, -7.2, -7.3, -7.3, -7.3, -7.4, -7.4, -7.4, -7.4, -7.4, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.5, -7.6, -7.6, -7.6, -7.6, -7.7, -7.7, -7.7, -7.7, -7.7, -7.8, -7.8, -7.7, -7.7, -7.7, -7.6, -7.6, -7.6, -7.6, -7.6, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.7, -7.6, -7.5, -7.4, -7.4, -7.5, -7.6, -7.7, -7.8, -7.9, -7.9, -7.9, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -8.0, -7.9, -7.9, -8.0, -8.0, -8.0, -8.1, -8.1, -8.1, -8.1, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.2, -8.3, -8.4, -8.4, -8.4, -8.5, -8.5, -8.6, -8.6, -8.7, -8.7, -8.7, -8.8, -8.9, -9.0, -9.0, -9.1, -9.2, -9.3, -9.5, -9.6, -9.7, -9.7, -9.8, -9.8, -9.8, -9.9, -9.9, -10.0, -10.1, -10.2, -10.4, -10.6, -10.7, -10.8, -11.0, -11.3};

   double qqH8_mass [] = {80.0, 81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0, 89.0, 90.0, 91.0, 92.0, 93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0, 104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 110.5, 111.0, 111.5, 112.0, 112.5, 113.0, 113.5, 114.0, 114.5, 115.0, 115.5, 116.0, 116.5, 117.0, 117.5, 118.0, 118.5, 119.0, 119.5, 120.0, 120.5, 121.0, 121.5, 122.0, 122.5, 123.0, 123.5, 124.0, 124.5, 125.0, 125.5, 126.0, 126.5, 127.0, 127.5, 128.0, 128.5, 129.0, 129.5, 130.0, 130.5, 131.0, 131.5, 132.0, 132.5, 133.0, 133.5, 134.0, 134.5, 135.0, 135.5, 136.0, 136.5, 137.0, 137.5, 138.0, 138.5, 139.0, 139.5, 140.0, 141.0, 142.0, 143.0, 144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0, 152.0, 153.0, 154.0, 155.0, 156.0, 157.0, 158.0, 159.0, 160.0, 162.0, 164.0, 165.0, 166.0, 168.0, 170.0, 172.0, 174.0, 175.0, 176.0, 178.0, 180.0, 182.0, 184.0, 185.0, 186.0, 188.0, 190.0, 192.0, 194.0, 195.0, 196.0, 198.0, 200.0, 202.0, 204.0, 206.0, 208.0, 210.0, 212.0, 214.0, 216.0, 218.0, 220.0, 222.0, 224.0, 226.0, 228.0, 230.0, 232.0, 234.0, 236.0, 238.0, 240.0, 242.0, 244.0, 246.0, 248.0, 250.0, 252.0, 254.0, 256.0, 258.0, 260.0, 262.0, 264.0, 266.0, 268.0, 270.0, 272.0, 274.0, 276.0, 278.0, 280.0, 282.0, 284.0, 286.0, 288.0, 290.0, 295.0, 300.0, 305.0, 310.0, 315.0, 320.0, 325.0, 330.0, 335.0, 340.0, 345.0, 350.0, 360.0, 370.0, 380.0, 390.0, 400.0, 420.0, 440.0, 450.0, 460.0, 480.0, 500.0, 520.0, 540.0, 550.0, 560.0, 580.0, 600.0, 620.0, 640.0, 650.0, 660.0, 680.0, 700.0, 720.0, 740.0, 750.0, 760.0, 780.0, 800.0, 820.0, 840.0, 850.0, 860.0, 880.0, 900.0, 920.0, 940.0, 950.0, 960.0, 980.0, 1000.0};
   double qqH8_xsec [] = {2.410, 2.384, 2.360, 2.336, 2.311, 2.289, 2.265, 2.243, 2.221, 2.199, 2.176, 2.154, 2.133, 2.112, 2.090, 2.071, 2.050, 2.030, 2.010, 1.991, 1.971, 1.952, 1.934, 1.915, 1.897, 1.878, 1.860, 1.843, 1.826, 1.808, 1.791, 1.783, 1.775, 1.766, 1.758, 1.750, 1.742, 1.733, 1.725, 1.717, 1.709, 1.701, 1.693, 1.686, 1.678, 1.670, 1.661, 1.654, 1.647, 1.639, 1.632, 1.624, 1.617, 1.609, 1.602, 1.595, 1.588, 1.580, 1.573, 1.566, 1.559, 1.552, 1.544, 1.539, 1.531, 1.524, 1.517, 1.511, 1.504, 1.497, 1.490, 1.483, 1.477, 1.470, 1.463, 1.458, 1.451, 1.444, 1.439, 1.432, 1.425, 1.419, 1.413, 1.407, 1.401, 1.395, 1.388, 1.382, 1.376, 1.370, 1.365, 1.352, 1.341, 1.329, 1.317, 1.306, 1.295, 1.284, 1.272, 1.261, 1.251, 1.240, 1.229, 1.218, 1.208, 1.197, 1.187, 1.176, 1.166, 1.155, 1.146, 1.136, 1.123, 1.115, 1.106, 1.088, 1.070, 1.052, 1.035, 1.026, 1.017, 1.000, 0.9820, 0.9670, 0.9558, 0.9496, 0.9429, 0.9286, 0.9139, 0.8998, 0.8854, 0.8783, 0.8714, 0.8574, 0.8441, 0.8309, 0.8178, 0.8051, 0.7927, 0.7805, 0.7687, 0.7568, 0.7452, 0.7340, 0.7229, 0.7120, 0.7016, 0.6913, 0.6808, 0.6707, 0.6610, 0.6513, 0.6418, 0.6326, 0.6234, 0.6144, 0.6056, 0.5969, 0.5885, 0.5802, 0.5720, 0.5640, 0.5562, 0.5484, 0.5408, 0.5333, 0.5259, 0.5187, 0.5116, 0.5047, 0.4978, 0.4910, 0.4845, 0.4780, 0.4715, 0.4652, 0.4590, 0.4530, 0.4470, 0.4716, 0.4562, 0.4408, 0.4266, 0.4131, 0.3999, 0.3875, 0.3753, 0.3637, 0.3526, 0.3422, 0.3303, 0.3200, 0.3028, 0.2896, 0.2776, 0.2660, 0.2543, 0.2317, 0.2103, 0.2002, 0.1905, 0.1724, 0.1561, 0.1414, 0.1283, 0.1223, 0.1166, 0.1062, 0.09688, 0.08861, 0.08121, 0.07784, 0.07459, 0.06865, 0.06330, 0.05853, 0.05420, 0.05235, 0.05032, 0.04682, 0.04365, 0.04078, 0.03815, 0.03706, 0.03579, 0.03363, 0.03164, 0.02986, 0.02820, 0.02745, 0.02669, 0.02524, 0.02399};
   double qqH8_errp [] = {+2.9, +3.0, +2.9, +3.0, +2.9, +2.9, +2.9, +2.9, +2.8, +2.9, +2.9, +2.9, +2.8, +2.9, +2.9, +2.8, +2.9, +2.9, +2.9, +2.8, +2.8, +2.9, +2.7, +2.8, +2.8, +2.8, +2.8, +2.7, +2.7, +2.8, +2.7, +2.7, +2.7, +2.7, +2.7, +2.8, +2.8, +2.8, +2.7, +2.7, +2.7, +2.8, +2.8, +2.8, +2.7, +2.8, +2.9, +2.8, +2.9, +2.8, +2.8, +2.8, +2.8, +2.8, +2.9, +2.8, +2.8, +2.8, +2.9, +2.8, +2.8, +2.8, +2.9, +2.8, +2.9, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.7, +2.7, +2.8, +2.8, +2.7, +2.7, +2.7, +2.7, +2.7, +2.7, +2.7, +2.8, +2.7, +2.7, +2.8, +2.7, +2.8, +2.7, +2.7, +2.8, +2.7, +2.7, +2.7, +2.7, +2.7, +2.8, +2.7, +2.7, +2.7, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.9, +2.8, +2.7, +2.7, +2.7, +2.8, +2.8, +2.7, +2.8, +2.7, +2.8, +2.8, +2.8, +2.7, +2.8, +2.8, +2.8, +2.7, +2.7, +2.7, +2.7, +2.6, +2.7, +2.9, +2.8, +2.8, +2.8, +2.8, +2.8, +2.7, +2.8, +2.7, +2.8, +2.8, +2.7, +2.8, +2.7, +2.8, +2.8, +2.8, +2.7, +2.8, +2.8, +2.8, +2.8, +2.8, +2.7, +2.8, +2.8, +2.8, +2.7, +2.6, +2.7, +2.7, +2.8, +2.7, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.7, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.8, +2.9, +2.9, +2.9, +3.1, +3.1, +3.1, +3.2, +3.2, +3.4, +3.4, +3.5, +3.5, +3.5, +3.7, +3.9, +3.9, +4.0, +4.0, +4.2, +4.3, +4.4, +4.4, +4.5, +4.7, +4.8, +4.9, +5.0, +5.0, +5.2, +5.4, +5.5, +5.5, +5.6, +5.7, +5.8, +6.0, +6.1, +6.2, +6.3};
   double qqH8_errm [] = {-3.3, -3.3, -3.1, -3.0, -3.0, -3.0, -3.0, -3.2, -3.2, -3.2, -2.9, -2.9, -3.2, -2.9, -2.9, -3.1, -3.1, -3.0, -3.1, -3.1, -3.1, -3.1, -3.0, -3.0, -3.1, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.8, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.9, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.8, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.9, -2.8, -2.8, -2.8, -2.8, -2.8, -2.9, -2.9, -2.8, -2.8, -2.8, -2.9, -2.7, -2.8, -2.8, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.6, -2.6, -2.6, -2.6, -2.8, -2.6, -2.8, -2.8, -2.8, -2.8, -2.8, -2.7, -2.8, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.7, -2.7, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.9, -2.8, -2.8, -2.8, -2.9, -3.0, -3.0, -3.1, -3.2, -3.2, -3.2, -3.3, -3.6, -3.6, -3.6, -3.7, -3.9, -4.1, -4.1, -4.1, -4.3, -4.3, -4.4, -4.6, -4.8, -4.8, -4.9, -5.0, -5.3, -5.2, -5.2, -5.4, -5.6, -5.7, -6.0, -6.1, -6.0, -6.2, -6.3, -6.5, -6.8, -6.9, -6.8, -7.0, -7.1, -7.2};
   double qqH8_scap [] = {+0.2, +0.4, +0.3, +0.4, +0.3, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.3, +0.3, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.3, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.3, +0.2, +0.3, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.3, +0.2, +0.2, +0.2, +0.3, +0.3, +0.2, +0.3, +0.2, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.2, +0.2, +0.2, +0.2, +0.2, +0.2, +0.3, +0.2, +0.2, +0.3, +0.3, +0.3, +0.2, +0.3, +0.2, +0.3, +0.3, +0.2, +0.3, +0.2, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.2, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.3, +0.4, +0.4, +0.4, +0.4, +0.4, +0.4, +0.4, +0.4, +0.4, +0.4, +0.5, +0.5, +0.5, +0.5, +0.6, +0.6, +0.6, +0.6, +0.6, +0.6, +0.6, +0.6, +0.7, +0.7, +0.7};
   double qqH8_scam [] = {-0.3, -0.3, -0.3, -0.2, -0.2, -0.2, -0.2, -0.3, -0.3, -0.3, -0.2, -0.2, -0.3, -0.2, -0.2, -0.2, -0.2, -0.3, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2, -0.2, -0.1, -0.1, -0.1, -0.2, -0.1, -0.2, -0.2, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.1, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.2, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.3, -0.4, -0.4, -0.4, -0.4, -0.5, -0.5, -0.5, -0.5, -0.6, -0.6, -0.6, -0.6, -0.7, -0.7, -0.7, -0.8, -0.8, -0.8, -0.8, -0.8, -0.9, -0.9, -0.9, -0.9, -1.0, -1.0, -1.1, -1.0, -1.1, -1.1, -1.1, -1.2, -1.2, -1.2, -1.3, -1.2, -1.3, -1.3};
   double qqH8_pdfp [] = {+2.7, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.5, +2.6, +2.6, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.6, +2.5, +2.6, +2.5, +2.5, +2.5, +2.6, +2.6, +2.6, +2.5, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.5, +2.6, +2.5, +2.5, +2.6, +2.6, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.6, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.4, +2.5, +2.6, +2.6, +2.6, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.4, +2.4, +2.4, +2.4, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.4, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.5, +2.6, +2.6, +2.6, +2.8, +2.8, +2.8, +2.9, +2.9, +3.1, +3.1, +3.2, +3.2, +3.2, +3.4, +3.5, +3.5, +3.6, +3.6, +3.8, +3.9, +4.0, +4.0, +4.1, +4.3, +4.3, +4.4, +4.5, +4.5, +4.6, +4.8, +4.9, +4.9, +5.0, +5.1, +5.2, +5.4, +5.4, +5.5, +5.6};
   double qqH8_pdfm [] = {-3.0, -3.0, -2.8, -2.8, -2.8, -2.8, -2.8, -2.9, -2.9, -2.9, -2.7, -2.7, -2.9, -2.7, -2.7, -2.9, -2.9, -2.7, -2.9, -2.9, -2.9, -2.9, -2.8, -2.8, -2.9, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.6, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.8, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.7, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.5, -2.5, -2.5, -2.5, -2.7, -2.5, -2.7, -2.7, -2.7, -2.7, -2.7, -2.6, -2.7, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.5, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.6, -2.5, -2.5, -2.5, -2.6, -2.7, -2.7, -2.8, -2.8, -2.8, -2.8, -2.9, -3.1, -3.1, -3.1, -3.2, -3.3, -3.5, -3.5, -3.5, -3.6, -3.6, -3.7, -3.8, -4.0, -4.0, -4.1, -4.2, -4.4, -4.3, -4.3, -4.5, -4.6, -4.7, -4.9, -5.1, -4.9, -5.1, -5.2, -5.3, -5.6, -5.7, -5.5, -5.8, -5.8, -5.9};


   ggH7TG_xsec = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_xsec);
   ggH7TG_errp = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_errp);
   ggH7TG_errm = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_errm);
   ggH7TG_scap = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_scap);
   ggH7TG_scam = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_scam);
   ggH7TG_pdfp = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_pdfp);
   ggH7TG_pdfm = new TGraph(sizeof(ggH7_mass)/sizeof(double), ggH7_mass, ggH7_pdfm);

   qqH7TG_xsec = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_xsec);
   qqH7TG_errp = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_errp);
   qqH7TG_errm = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_errm);
   qqH7TG_scap = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_scap);
   qqH7TG_scam = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_scam);
   qqH7TG_pdfp = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_pdfp);
   qqH7TG_pdfm = new TGraph(sizeof(qqH7_mass)/sizeof(double), qqH7_mass, qqH7_pdfm);

   ggH8TG_xsec = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_xsec);
   ggH8TG_errp = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_errp);
   ggH8TG_errm = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_errm);
   ggH8TG_scap = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_scap);
   ggH8TG_scam = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_scam);
   ggH8TG_pdfp = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_pdfp);
   ggH8TG_pdfm = new TGraph(sizeof(ggH8_mass)/sizeof(double), ggH8_mass, ggH8_pdfm);

   qqH8TG_xsec = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_xsec);
   qqH8TG_errp = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_errp);
   qqH8TG_errm = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_errm);
   qqH8TG_scap = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_scap);
   qqH8TG_scam = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_scam);
   qqH8TG_pdfp = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_pdfp);
   qqH8TG_pdfm = new TGraph(sizeof(qqH8_mass)/sizeof(double), qqH8_mass, qqH8_pdfm);

   double QCDScaleMass   [] = {200, 250, 300, 350, 400, 450, 500, 550, 600};
   double QCDScaleK0ggH0 [] = {1.15 , 1.16, 1.17, 1.20, 1.17, 1.19, 1.22, 1.24, 1.25};
   double QCDScaleK0ggH1 [] = {0.88, 0.86, 0.84, 0.83, 0.82, 0.81, 0.80, 0.78, 0.78};
   double QCDScaleK1ggH1 [] = {1.27, 1.27, 1.27, 1.27, 1.26, 1.26, 1.25, 1.26, 1.26};
   double QCDScaleK1ggH2 [] = {0.96, 0.96, 0.95, 0.95, 0.95, 0.95, 0.95,  0.95, 0.94};
   double QCDScaleK2ggH2 [] = { 1.20, 1.17, 1.20, 1.21, 1.20, 1.20, 1.17, 1.19, 1.19};

   double UEPSf0 []         = {0.952, 0.955, 0.958, 0.964, 0.966, 0.954, 0.946, 0.931, 0.920};
   double UEPSf1 []         = {1.055, 1.058, 1.061, 1.068, 1.078, 1.092, 1.102, 1.117, 1.121};
   double UEPSf2 []         = {0.059, 0.990, 0.942, 0.889, 0.856, 0.864, 0.868, 0.861, 0.872}; 

  TG_QCDScaleK0ggH0 = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, QCDScaleK0ggH0);
  TG_QCDScaleK0ggH1 = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, QCDScaleK0ggH1);
  TG_QCDScaleK1ggH1 = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, QCDScaleK1ggH1);
  TG_QCDScaleK1ggH2 = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, QCDScaleK1ggH2);
  TG_QCDScaleK2ggH2 = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, QCDScaleK2ggH2);

  TG_UEPSf0         = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, UEPSf0);
  TG_UEPSf1         = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, UEPSf1);
  TG_UEPSf2         = new TGraph(sizeof(QCDScaleMass)/sizeof(double), QCDScaleMass, UEPSf2);



}



