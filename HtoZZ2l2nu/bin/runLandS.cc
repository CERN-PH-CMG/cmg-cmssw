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

TString landsExe("$CMSSW_BASE/src/UserCode/mschen/LandS/test/lands.exe");
#define RUNBAYESIAN(INURL,OUTURL)\
  gSystem->ExpandPathName(landsExe); \
  TString cmd=TString(landsExe + " -M Bayesian -d ") + INURL + TString( " --doExpectation 1 -t 10000 > ") + OUTURL + TString(" "); \
  cout << "Launching : " << cmd << endl;\
  gSystem->Exec(cmd);

#define RUNASYMPTOTIC(INURL,OUTURL)\
  gSystem->ExpandPathName(landsExe); \
  TString cmd=TString(landsExe + " -d ") + INURL + TString(" -M Hybrid --freq --ExpectationHints Asymptotic --scanRs 1 --freq --nToysForCLsb 4000 --nToysForCLb 2000 --seed 1234 -rMax 50 -rMin 0.1 > ") + logUrl + TString(" "); \
  cout << "Launching : " << cmd << endl;				\
  gSystem->Exec(cmd);
//  TString cmd=TString(landsExe + " -d ") + INURL + TString(" -M Hybrid --freq --ExpectationHints Asymptotic --scanRs 1 --freq --nToysForCLsb 10000 --nToysForCLb 5000 --seed 1234 -rMax 50 -rMin 0.1 > ") + logUrl + TString(" "); 

#define RUNASYMPTOTIC_FAST(INURL,OUTURL)\
  gSystem->ExpandPathName(landsExe); \
  TString cmd=TString(landsExe + " -d ") + INURL + TString(" -M Hybrid --ExpectationHints Asymptotic > ") + logUrl + TString(" "); \
  cout << "Launching : " << cmd << endl;                                \
  gSystem->Exec(cmd);


double NonResonnantSyst = 0.25;
double GammaJetSyst = 0.5;

//wrapper for a projected shape for a given set of cuts
struct Shape_t
{
  TH1* data, *totalBckg;
  std::vector<TH1 *> bckg, signal;
  //the key corresponds to the proc name
  //the key is the name of the variation: e.g. jesup, jesdown, etc.
  std::map<TString,std::vector<std::pair<TString, TH1*> > > bckgVars, signalVars;
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
void showShape(const Shape_t &shape, TString SaveName);
void getYieldsFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName);

void convertHistosForLimits_core(DataCardInputs& dci, TString& proc, TString& bin, TString& ch, std::vector<TString>& systs, std::vector<TH1*>& hshapes,  bool runSystematics, bool shape);
DataCardInputs convertHistosForLimits(Int_t mass,TString histo="finalmt",TString url="plotter.root",TString Json="", bool runSystematics=true, bool shape=true);
std::vector<TString> buildDataCard(Int_t mass, TString histo="finalmt", TString url="plotter.root",TString Json="", bool runSystematics=true, bool shape=true);
void doBackgroundSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t> &allShapes, TString mainHisto, TString sideBandHisto);
void doDYReplacement(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString metHistoForRescale);
void doWZSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString sideBandHisto);

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
TString DYFile ="";
TString inFileUrl(""),jsonFile(""), histo("");
TString prefix="";

int indexvbf = -1;
int indexcut   = -1;
int mass=-1; 

void printHelp()
{
  printf("Options\n");
  printf("--in        --> input file with from plotter\n");
  printf("--json      --> json file with the sample descriptor\n");
  printf("--histo     --> name of histogram to be used\n");
  printf("--index     --> index of selection to be used (Xbin in histogram to be used)\n");
  printf("--indexvbf  --> index of selection to be used for the vbf bin (if unspecified same as --index)\n");
  printf("--m         --> higgs mass to be considered\n");
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
  printf("--fast      --> use this flag to only do assymptotic prediction (very fast but inaccurate))\n");
  printf("--prefix    --> use this to specify a prefix that will be added to the process names)\n");
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

  //get input arguments
  bool runSystematics = false; bool shape = false;
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg.find("--help")          !=string::npos) { printHelp(); return -1;} 
    else if(arg.find("--syst")     !=string::npos) { runSystematics=true; printf("syst = True\n");}
    else if(arg.find("--shape")    !=string::npos) { shape=true; printf("shapeBased = True\n");}
    else if(arg.find("--subNRB12") !=string::npos) { subNRB2012=true; skipWW=false; printf("subNRB2012 = True\n");}
    else if(arg.find("--subNRB")   !=string::npos) { subNRB2011=true; skipWW=true; printf("subNRB2011 = True\n");}
    else if(arg.find("--subDY")    !=string::npos) { subDY=true; DYFile=argv[i+1];  i++; printf("Z+Jets will be replaced by %s\n",DYFile.Data());}
    else if(arg.find("--subWZ")    !=string::npos) { subWZ=true; printf("WZ will be estimated from 3rd lepton SB\n");}
    else if(arg.find("--DDRescale")!=string::npos && i+1<argc)  { sscanf(argv[i+1],"%lf",&DDRescale); i++;}
    else if(arg.find("--HWW")      !=string::npos) { skipWW=false; printf("HWW = True\n");}
    else if(arg.find("--skipGGH")  !=string::npos) { skipGGH=true; printf("skipGGH = True\n");}
    else if(arg.find("--skipQQH")  !=string::npos) { skipQQH=true; printf("skipQQH = True\n");}
    else if(arg.find("--closure")  !=string::npos) { MCclosureTest=true; printf("MCclosureTest = True\n");}
    else if(arg.find("--indexvbf") !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&indexvbf); i++; printf("indexVBF = %i\n", indexvbf);}
    else if(arg.find("--index")    !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&indexcut); i++; printf("index = %i\n", indexcut);}
    else if(arg.find("--in")       !=string::npos && i+1<argc)  { inFileUrl = argv[i+1];  i++;  printf("in = %s\n", inFileUrl.Data());  }
    else if(arg.find("--json")     !=string::npos && i+1<argc)  { jsonFile  = argv[i+1];  i++;  printf("json = %s\n", jsonFile.Data()); }
    else if(arg.find("--histo")    !=string::npos && i+1<argc)  { histo     = argv[i+1];  i++;  printf("histo = %s\n", histo.Data()); }
    else if(arg.find("--m")        !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&mass ); i++; printf("mass = %i\n", mass);}
    else if(arg.find("--bins")     !=string::npos && i+1<argc)  { char* pch = strtok(argv[i+1],",");printf("bins are : ");while (pch!=NULL){printf(" %s ",pch); AnalysisBins.push_back(pch);  pch = strtok(NULL,",");}printf("\n"); i++; }
    else if(arg.find("--channels") !=string::npos && i+1<argc)  { char* pch = strtok(argv[i+1],",");printf("channels are : ");while (pch!=NULL){printf(" %s ",pch); Channels.push_back(pch);  pch = strtok(NULL,",");}printf("\n"); i++; }
    else if(arg.find("--fast")     !=string::npos) { fast=true; printf("fast = True\n");}
    else if(arg.find("--prefix")   !=string::npos && i+1<argc)  { prefix = argv[i+1];  i++;  printf("prefix '%s' will be used\n", prefix.Data());  }
  }
  if(jsonFile.IsNull() || inFileUrl.IsNull() || histo.IsNull() || indexcut == -1 || mass==-1) { printHelp(); return -1; }
  if(AnalysisBins.size()==0)AnalysisBins.push_back("");
  if(Channels.size()==0){Channels.push_back("ee");Channels.push_back("mumu");}

  //build the datacard for this mass point
  std::vector<TString> dcUrls = buildDataCard(mass,histo,inFileUrl, jsonFile, runSystematics, shape);
}





//
void getYieldsFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName)
{
  FILE* pFile = fopen("Yields.tex","w");
  fprintf(pFile,"\\begin{sidewaystable}[htp]\n\\begin{center}\n\\caption{Event yields expected for background and signal processes and observed in data.}\n\\label{tab:table}\n");

  string Ccol   = "\\begin{tabular}{|c|";
  string Cname  = "channel";
  string Cval   = "";

  TString massStr(""); massStr += mass;


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

       if(!procTitle.Contains(massStr))continue;
            if(procTitle.Contains("ggH") && procTitle.Contains("ZZ"))procTitle = "ggH("+massStr+")";
       else if(procTitle.Contains("qqH") && procTitle.Contains("ZZ"))procTitle = "qqH("+massStr+")";
       else if(procTitle.Contains("ggH") && procTitle.Contains("WW"))procTitle = "ggH("+massStr+")WW";
       else if(procTitle.Contains("qqH") && procTitle.Contains("WW"))procTitle = "qqH("+massStr+")WW";

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
      int color(1);       if(Process[i].isTag("color" ) ) color  = (int)Process[i]["color" ].toDouble();
      int lcolor(color);  if(Process[i].isTag("lcolor") ) lcolor = (int)Process[i]["lcolor"].toDouble();
      int mcolor(color);  if(Process[i].isTag("mcolor") ) mcolor = (int)Process[i]["mcolor"].toDouble();
      int fcolor(color);  if(Process[i].isTag("fcolor") ) fcolor = (int)Process[i]["fcolor"].toDouble();
      int lwidth(1);      if(Process[i].isTag("lwidth") ) lwidth = (int)Process[i]["lwidth"].toDouble();
      int fill(1001);     if(Process[i].isTag("fill"  ) ) fill   = (int)Process[i]["fill"  ].toDouble();
      int marker(20);     if(Process[i].isTag("marker") ) marker = (int)Process[i]["marker"].toDouble();
  
      TH1* syst = (TH1*)pdir->Get("optim_systs");
      if(syst==NULL){syst=new TH1F("optim_systs","optim_systs",1,0,1);syst->GetXaxis()->SetBinLabel(1,"");}
      for(int ivar = 1; ivar<=syst->GetNbinsX();ivar++){
	 TH1D* hshape   = NULL;

	 TString varName = syst->GetXaxis()->GetBinLabel(ivar);
         TString histoName = ch+"_"+shapeName+varName ;
         TH2* hshape2D = (TH2*)pdir->Get(histoName );
         if(!hshape2D){
            //replace by empty histogram
            hshape2D = (TH2*)pdir->Get(shapeName+varName);
            if(hshape2D)hshape2D->Reset();
         }

         if(hshape2D){
            histoName.ReplaceAll(ch,ch+"_proj"+procCtr);
   	    hshape   = hshape2D->ProjectionY(histoName,cutBin,cutBin);
            if(hshape->Integral()<=0 && !isData){hshape->Reset(); hshape->SetBinContent(1, 1E-10);}
	    hshape->SetDirectory(0);
	    hshape->SetTitle(proc);
	    fixExtremities(hshape,true,true);
	    hshape->SetFillColor(color); hshape->SetLineColor(lcolor); hshape->SetMarkerColor(mcolor);
	    hshape->SetFillStyle(fill);  hshape->SetLineWidth(lwidth); hshape->SetMarkerStyle(marker);
         }else{		
            if(ivar==1)printf("Histo does not exist: %s\n", histoName.Data());
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
               int procIndex = -1;
               for(unsigned int i=0;i<shape.signal.size();i++){ if(string(proc.Data())==shape.signal[i]->GetTitle() ){procIndex=i;break;}  }
               if(procIndex>=0) shape.signal[procIndex]->Add(hshape);
               else             {hshape->SetTitle(proc);shape.signal.push_back(hshape);}
            }else{
               std::map<TString,std::vector<std::pair<TString, TH1*> > >::iterator it = shape.signalVars.find(proc);
                
               bool newVar = false;
               if(it!=shape.signalVars.end()){
                  for(unsigned int i=0;i<it->second.size();i++){ if( string(it->second[i].first.Data()) == varName ){it->second[i].second->Add(hshape); newVar=true;break;}  }
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
void showShape(const Shape_t &shape,TString SaveName)
{
  TCanvas* c1 = new TCanvas("c1","c1",800,800);
  c1->SetWindowSize(800,800);
  c1->cd();

  TPad* t1 = new TPad("t1","t1", 0.0, 0.20, 1.0, 1.0);  t1->Draw();  t1->cd();
  TLegend* legA  = new TLegend(0.845,0.2,0.99,0.99, "NDC");

  bool canvasIsFilled(false);
  THStack *stack=0;
  TH1 *mc=0;
  if(shape.bckg.size())
    {
      mc=(TH1 *)shape.totalBckg->Clone("mc");
      stack = new THStack("stack","stack"); 
      for(size_t i=0; i<shape.bckg.size(); i++) 
	{
	  if(shape.bckg[i]->Integral()<=0) continue;
	  stack->Add(shape.bckg[i],"HIST");
	  legA->AddEntry(shape.bckg[i],shape.bckg[i]->GetTitle(),"F");
	}
      stack->Draw();
      stack->GetXaxis()->SetTitle(mc->GetXaxis()->GetTitle());
      stack->GetYaxis()->SetTitle(mc->GetYaxis()->GetTitle());
      stack->SetMinimum(mc->GetMinimum());
      stack->SetMaximum(1.1*mc->GetMaximum());
      canvasIsFilled=true;
    }
  if(shape.data)
    {
      shape.data->Draw(canvasIsFilled ? "E1same" : "E1");
      legA->AddEntry(shape.data,shape.data->GetTitle(),"P");
      canvasIsFilled=true;
    }
  for(size_t ip=0; ip<shape.signal.size(); ip++)
    {
      shape.signal[ip]->Draw(canvasIsFilled ? "histsame" : "hist");
      legA->AddEntry(shape.signal[ip],shape.signal[ip]->GetTitle(),"L");
      canvasIsFilled=true;
    }
  TPaveText* T = new TPaveText(0.1,0.995,0.84,0.95, "NDC");
  T->SetFillColor(0);  T->SetFillStyle(0);  T->SetLineColor(0); T->SetBorderSize(0);  T->SetTextAlign(22);
  T->AddText("CMS preliminary");  T->Draw();
  
  legA->SetFillColor(0); legA->SetFillStyle(0); legA->SetLineColor(0);  legA->SetBorderSize(); legA->SetHeader("");
  legA->Draw("same");    legA->SetTextFont(42);


  TH1 *ratio=0; 
  if(shape.data && mc)
    {
      c1->cd();
      TPad* t2 = new TPad("t2","t2", 0.0, 0.0, 1.0, 0.2);     t2->Draw();
      t2->cd();
      t2->SetGridy(true);
      t2->SetTopMargin(0);   t2->SetBottomMargin(0.5);
      float yscale = (1.0-0.2)/(0.18-0);
      TH1 *ratio = (TH1*)shape.data->Clone("RatioHistogram");
      ratio->SetDirectory(0);
      ratio->Divide(mc);
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
      ratio->Draw("E1");
    }

  c1->cd();
  c1->Modified();
  c1->Update();
  c1->SaveAs(SaveName+".png");
  c1->SaveAs(SaveName+".C");
  delete c1;
  if(mc)    delete mc;
  if(stack) delete stack;
  if(ratio) delete ratio;
}

/*
//
std::vector<TString>  buildDataCard(Int_t mass, TString histo, TString url, TString Json, bool runSystematics, bool shape)
{
  std::vector<TString> dcUrls;

  //get the datacard inputs 
  DataCardInputs dci = convertHistosForLimits(mass,histo,url,Json, runSystematics, shape);

  TString eecard = "";
  TString mumucard = "";
  TString combinedcard = "";

  //build the datacard separately for each channel
  for(size_t i=1; i<=dci.ch.size(); i++){  
      ofstream dcF;
      TString dcName=dci.shapesFile;
      dcName.ReplaceAll(".root","_"+dci.ch[i-1]+".dat");
      dcF.open(dcName.Data(),ios::out);

      combinedcard += dci.ch[i-1]+"="+dcName+" ";
      if(dci.ch[i-1].Contains("ee"))eecard += dci.ch[i-1]+"="+dcName+" ";
      if(dci.ch[i-1].Contains("mumu"))mumucard += dci.ch[i-1]+"="+dcName+" ";

      //header
      dcF << "imax 1" << endl
	  << "jmax *" << endl //<< dci.procs.size() << endl
	  << "kmax *" << endl
	  << "-------------------------------" << endl
//	  << "shapes * * " << outDir << "/" << dci.shapesFile << " $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
//        << "shapes * * " << outDir << "/" << dci.shapesFile << " " << dci.ch[i-1] << "/$PROCESS " << dci.ch[i-1] << "/$PROCESS_$SYSTEMATIC" << endl;
        << "shapes * * " << dci.shapesFile << " " << dci.ch[i-1] << "/$PROCESS " << dci.ch[i-1] << "/$PROCESS_$SYSTEMATIC" << endl;
  
      //observations
      dcF << "-------------------------------" << endl
	  << "bin " << 1 << endl
	  << "Observation " << dci.obs[RateKey_t("obs",dci.ch[i-1])] << endl;
      
      //yields per bin
      dcF << "-------------------------------" << endl
	  << "bin\t ";
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1 << " ";
      }dcF << endl;
 
      dcF << "process\t ";
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << prefix+dci.procs[j-1] << " ";
      }dcF << endl;

      dcF << "process\t ";
      int procCtr(1-dci.nsignalproc);
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << procCtr << " ";
	  procCtr++;
      }dcF << endl;

      dcF << "rate\t";
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])] << " ";
      }
      dcF << endl;
if(runSystematics){
      //systematics
      dcF << "-------------------------------" << endl;
      dcF << "Lumi\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
          if(!dci.procs[j-1].Contains("data")){    dcF << 1.022 << " ";
          }else{                                   dcF << "-"   << " ";  }
      }dcF << endl;
  
      dcF << "trigEff\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
          if(!dci.procs[j-1].Contains("data")){
             if(dci.ch[i-1].Contains("ee")){ dcF << 1.01 << "  ";
             }else                         { dcF << 1.02 << "  "; }
          }else{                             dcF << "-"   << " "; }
      }dcF << endl;

      dcF << "idEff\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++){
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
          if(!dci.procs[j-1].Contains("data")){    dcF << 1.02 << " ";
          }else{                                   dcF << "-"  << " ";  }
      }dcF << endl;
}
      
      //systematics with shape description
      for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator it=dci.systs.begin(); it!=dci.systs.end(); it++){
          if(!runSystematics && string(it->first.Data()).find("stat")>0 )continue;

          //temporary placed there to speed up computation
          //if(shape && string(it->first.Data())!="stat" )continue;
          if(string(it->first.Data()).find("syst")==0 ){
             dcF << it->first << "\t lnN\t";
             for(size_t j=1; j<=dci.procs.size(); j++){
                if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                if(it->second.find(RateKey_t(dci.procs[j-1],dci.ch[i-1])) != it->second.end()){
                   Double_t systUnc = it->second[RateKey_t(dci.procs[j-1],dci.ch[i-1])];
                   if(systUnc<=0){ dcF << "- ";
                   }else{          dcF << (1.0+ (systUnc / dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])]) ) << " ";
                   }
                }else{
                   dcF << "- ";
                }
             }dcF << endl;

          }else{

             dcF << it->first << "\t shapeN2\t";
             for(size_t j=1; j<=dci.procs.size(); j++){
                if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
                if(it->second.find(RateKey_t(dci.procs[j-1],dci.ch[i-1])) != it->second.end()){
                   dcF << it->second[RateKey_t(dci.procs[j-1],dci.ch[i-1])] << " ";
                }else{
                   dcF << "- ";
                }
	     }dcF << endl;
         }
      }

  

      dcF.close();
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
*/


std::vector<TString>  buildDataCard(Int_t mass, TString histo, TString url, TString Json, bool runSystematics, bool shape)
{
  std::vector<TString> dcUrls;

  //get the datacard inputs 
  DataCardInputs dci = convertHistosForLimits(mass,histo,url,Json, runSystematics, shape);

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
      fprintf(pFile, "Observation %.0f\n",dci.obs[RateKey_t("obs",dci.ch[i-1])]);
      fprintf(pFile, "-------------------------------\n");

      fprintf(pFile,"%45s ", "bin");
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6i ", 1);}  fprintf(pFile,"\n");

      fprintf(pFile,"%45s ", "process");
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6s ", (prefix+dci.procs[j-1]).Data());}  fprintf(pFile,"\n");

      fprintf(pFile,"%45s ", "process"); int procCtr(1-dci.nsignalproc);
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6i ", procCtr );  procCtr++;}  fprintf(pFile,"\n");

      fprintf(pFile,"%45s ", "rate");
      for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; fprintf(pFile,"%6f ", dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])] );}  fprintf(pFile,"\n");
      fprintf(pFile, "-------------------------------\n");


      //systematics
      char sFile[2048];
      bool isSyst;
      if(runSystematics){
         fprintf(pFile,"%35s %10s ", "Lumi", "lnN");
         for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; 
            if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.022);}else{fprintf(pFile,"%6s ","-");}
         }fprintf(pFile,"\n");

         if(dci.ch[i-1].Contains("ee")){
            fprintf(pFile,"%35s %10s ", "eeTrigEff", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue; 
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.01);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }else{
            fprintf(pFile,"%35s %10s ", "mmTrigEff", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.02);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }

         if(dci.ch[i-1].Contains("ee")){
            fprintf(pFile,"%35s %10s ", "eeIDEff", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.02);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }else{
            fprintf(pFile,"%35s %10s ", "mmIDEff", "lnN");
            for(size_t j=1; j<=dci.procs.size(); j++){ if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
               if(!dci.procs[j-1].Contains("data")){fprintf(pFile,"%6f ",1.02);}else{fprintf(pFile,"%6s ","-");}
            }fprintf(pFile,"\n");
         }
            
         for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator it=dci.systs.begin(); it!=dci.systs.end(); it++){
             if(!runSystematics && string(it->first.Data()).find("stat")>0 )continue;

             isSyst=false;
             if(string(it->first.Data()).find("syst")==0 ){
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
DataCardInputs convertHistosForLimits(Int_t mass,TString histo,TString url,TString Json, bool runSystematics, bool shape)
{
  DataCardInputs dci;
 
  //init the json wrapper
  JSONWrapper::Object Root(Json.Data(), true);

  //init globalVariables
  TString massStr(""); massStr += mass;
//  std::set<TString> allCh,allProcs;
  std::vector<TString> allCh,allProcs;

  //open input file
  TFile* inF = TFile::Open(url);
  if( !inF || inF->IsZombie() ){ cout << "Invalid file name : " << url << endl; return dci; }

  
  //get the shapes for each channel
  map<TString, Shape_t> allShapes;
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

  //replace Z+Jet background by Gamma+Jet estimates
  if(subDY)doDYReplacement(selCh,"gamma",allShapes,histo,"met_met");

  //replace WZ by its estimate from 3rd Lepton SB
  if(subWZ)doWZSubtraction(selCh,"emu",allShapes,histo,histo+"_3rdLepton");


  //print event yields from the mt shapes
  if(runSystematics)getYieldsFromShape(selCh,allShapes,histo);

  //prepare the output
  dci.shapesFile="Shapes_"+massStr+".root";
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
	if(!proc.Contains(massStr))continue;
	     if(proc.Contains("ggH") && proc.Contains("ZZ"))proc = "ggHZZ";
        else if(proc.Contains("qqH") && proc.Contains("ZZ"))proc = "qqHZZ";
        else if(proc.Contains("ggH") && proc.Contains("WW"))proc = "ggHWW";
        else if(proc.Contains("qqH") && proc.Contains("WW"))proc = "qqHWW";

        convertHistosForLimits_core(dci, proc, AnalysisBins[b], chbin, systs, hshapes, runSystematics, shape);
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
        convertHistosForLimits_core(dci, proc, AnalysisBins[b], chbin, systs, hshapes, runSystematics, shape);
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
     convertHistosForLimits_core(dci, proc, AnalysisBins[b], chbin, systs, hshapes, runSystematics, shape);

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



void convertHistosForLimits_core(DataCardInputs& dci, TString& proc, TString& bin, TString& ch, std::vector<TString>& systs, std::vector<TH1*>& hshapes,  bool runSystematics, bool shape){
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

       double systUncertainty = hshape->GetBinError(0);

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
            hshape->Write(prefix+proc);

            if(hshape->Integral()>0){
               hshape->SetName(proc+syst);
               TH1* statup=(TH1 *)hshape->Clone(proc+"_stat"+ch+proc+"Up");
               TH1* statdown=(TH1 *)hshape->Clone(proc+"_stat"+ch+proc+"Down");
               for(int ibin=1; ibin<=statup->GetXaxis()->GetNbins(); ibin++){
                  statup  ->SetBinContent(ibin,statup  ->GetBinContent(ibin) + statup  ->GetBinError(ibin));
                  statdown->SetBinContent(ibin,statdown->GetBinContent(ibin) - statdown->GetBinError(ibin));
               }
               statup  ->Write(prefix+proc+"_stat"+ch+proc+"Up");
               statdown->Write(prefix+proc+"_stat"+ch+proc+"Down");
               dci.systs["stat"+ch+proc][RateKey_t(proc,ch)]=1.0;

               if(systUncertainty>0)dci.systs["syst"+bin+proc][RateKey_t(proc,ch)]=systUncertainty;
            }
         }
       }else if(runSystematics && proc!="data" && (syst.Contains("Up") || syst.Contains("Down"))){
         //write variation to file
         hshape->SetName(proc+syst);
         hshape->Write(prefix+proc+syst);
       }else if(runSystematics){
         //for one sided systematics the down variation mirrors the difference bin by bin
         hshape->SetName(proc+syst);
         hshape->Write(prefix+proc+syst+"Up");
         TH1 *hmirrorshape=(TH1 *)hshape->Clone(proc+syst+"Down");
         for(int ibin=1; ibin<=hmirrorshape->GetXaxis()->GetNbins(); ibin++){
//            double bin = hmirrorshape->GetBinContent(ibin);
            double bin = 2*hshapes[0]->GetBinContent(ibin)-hmirrorshape->GetBinContent(ibin);
            if(bin<0)bin=0;
            hmirrorshape->SetBinContent(ibin,bin);
         }
	 if(hmirrorshape->Integral()<=0)hmirrorshape->SetBinContent(1, 1E-10);
         hmirrorshape->Write(prefix+proc+syst+"Down");
       }

       if(runSystematics && syst!=""){
          TString systName(syst); 
          systName.ReplaceAll("Up",""); systName.ReplaceAll("Down","");  systName.ReplaceAll("_","");

          TH1 *temp=(TH1*) hshape->Clone();
          temp->Add(hshapes[0],-1);
        if(temp->Integral()!=0)dci.systs[systName][RateKey_t(proc,ch)]=1.0;
          delete temp;
        }else if(proc=="asignal" && syst==""){dci.rates[RateKey_t(proc,ch)]=hshape->Integral();
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
	      shapeChan_SI.bckg.erase(shapeChan_SI.bckg.begin()+ibckg);  ibckg--;
           }
        }

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


