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
  TString cmd=TString(landsExe + " -d ") + INURL + TString(" -M Hybrid --freq --ExpectationHints Asymptotic --scanRs 1 --freq --nToysForCLsb 10000 --nToysForCLb 5000 --seed 1234 -rMax 50 -rMin 0.1 > ") + logUrl + TString(" "); \
  cout << "Launching : " << cmd << endl;				\
  gSystem->Exec(cmd);


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
};


void printHelp();
int findBinFor(Float_t minMet, Float_t minMt, Float_t maxMt, TString infURL);
Shape_t getShapeFromFile(TString ch, TString shapeName, int cutBin, TString infURL,JSONWrapper::Object &Root);
void showShape(const Shape_t &shape, TString SaveName);
void getCutFlowFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName);
void estimateNonResonantBackground(std::vector<TString> &selCh,TString ctrlCh,const map<TString, Shape_t> &allShapes, TString shape);


void convertHistosForLimits_core(DataCardInputs& dci, TString& proc, TString& ch, std::vector<TString>& systs, std::vector<TH1*>& hshapes,  bool runSystematics, bool shape, Int_t index);
DataCardInputs convertHistosForLimits(Int_t mass,TString histo="finalmt",TString url="plotter.root",TString Json="", TString outDir="./", bool runSystematics=true, bool shape=true, Int_t index=-1);
std::vector<TString> buildDataCard(Int_t mass, TString histo="finalmt", TString url="plotter.root",TString Json="", TString outDir="./", bool runSystematics=true, bool shape=true, Int_t index=-1);
void doBackgroundSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t> &allShapes, TString mainHisto, TString sideBandHisto);

bool subNRB2011 = false;
bool subNRB2012 = false;
bool MCclosureTest = false;

bool mergeWWandZZ = true;
bool skipWW = true;

void printHelp()
{
  printf("Options\n");
  printf("--in      --> input file with from plotter\n");
  printf("--json    --> json file with the sample descriptor\n");
  printf("--histo   --> name of histogram to be used\n");
  printf("--index   --> index of selection to be used (Xbin in histogram to be used)\n");
  printf("--m       --> higgs mass to be considered\n");
  printf("--syst    --> use this flag if you want to run systematics, default is no systematics\n");
  printf("--shape   --> use this flag if you want to run shapeBased analysis, default is cut&count\n");
  printf("--subNRB  --> use this flag if you want to subtract non-resonant-backgounds\n");
  printf("--closure --> use this flag if you want to perform a MC closure test (use only MC simulation)\n");
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
  TString inFileUrl(""),jsonFile(""), histo("");
  int mass=-1; int index = -1; bool runSystematics = false; bool shape = false;
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg.find("--help")  !=string::npos) { printHelp(); return -1;} 
    else if(arg.find("--syst")  !=string::npos) { runSystematics=true; printf("syst = True\n");}
    else if(arg.find("--shape") !=string::npos) { shape=true; printf("shapeBased = True\n");}
    else if(arg.find("--subNRB")!=string::npos) { subNRB2011=true; skipWW=true; printf("subNRB2011 = True\n");}
    else if(arg.find("--subNRB12")!=string::npos) { subNRB2012=true; skipWW=false; printf("subNRB2012 = True\n");}
    else if(arg.find("--closure")!=string::npos) { MCclosureTest=true; printf("MCclosureTest = True\n");}
    else if(arg.find("--index") !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&index); i++; printf("index = %i\n", index);}
    else if(arg.find("--in")    !=string::npos && i+1<argc)  { inFileUrl = argv[i+1];  i++;  printf("in = %s\n", inFileUrl.Data()); }
    else if(arg.find("--json")  !=string::npos && i+1<argc)  { jsonFile  = argv[i+1];  i++;  printf("json = %s\n", jsonFile.Data()); }
    else if(arg.find("--histo") !=string::npos && i+1<argc)  { histo     = argv[i+1];  i++;  printf("histo = %s\n", histo.Data()); }
    else if(arg.find("--m")     !=string::npos && i+1<argc)  { sscanf(argv[i+1],"%i",&mass ); i++; printf("mass = %i\n", mass);}
  }
  if(jsonFile.IsNull() || inFileUrl.IsNull() || histo.IsNull() || index == -1 || mass==-1) { printHelp(); return -1; }


  //prepare the output directory
  TString outDir("H"); outDir += mass;
  if(shape){outDir+=TString("_shape");}else{outDir+=TString("_count");}
  outDir+=TString("_");outDir+=index;
  TString mkdirCmd("mkdir -p "); mkdirCmd+=outDir;
  gSystem->Exec(mkdirCmd.Data());
  mkdirCmd += "/combined";
  gSystem->Exec(mkdirCmd);

  //build the datacard for this mass point
  std::vector<TString> dcUrls = buildDataCard(mass,histo,inFileUrl, jsonFile, outDir, runSystematics, shape, index);

  //run the combined limits 
  //need to create a new directory with the exclusive datacards and link the root file with the histos
  for(size_t i=0; i<dcUrls.size(); i++){
        TString cpCmd("cp ");
        cpCmd += dcUrls[i];
        TString newDCUrl(dcUrls[i]); newDCUrl.ReplaceAll(outDir,outDir+"/combined/");
        cpCmd += " " + newDCUrl;
        gSystem->Exec(cpCmd);
  }

  TString lnCmd("ln -sf ");
  lnCmd += outDir;  lnCmd += "/Shapes_"; lnCmd += mass; lnCmd += ".root ";
  lnCmd += outDir;  lnCmd += "/combined/Shapes_"; lnCmd += mass; lnCmd += ".root ";
  gSystem->Exec(lnCmd);

  TString logUrl(outDir+"/combined/Shapes_"); logUrl += mass; logUrl += ".log";
  RUNASYMPTOTIC(outDir+"/combined/*.dat",logUrl);

  if(runSystematics){
     //run limits in the exclusive channels
     for(size_t i=0; i<dcUrls.size(); i++){
        TString logUrl(dcUrls[i]); logUrl.ReplaceAll(".dat",".log");
//      RUNASYMPTOTIC(dcUrls[i],logUrl);
     }
  }

}





//
void getCutFlowFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName)
{
  fstream cutflow("CutFlow.tex",ios::out | ios::trunc);

  //table header
  const size_t nch=ch.size();
  cutflow <<"\\begin{table}[htp]" << endl
       << "\\begin{center}" << endl
       << "\\caption{Event yields expected for background and signal processes and observed in data.}" << endl
       <<"\\label{tab:table}" << endl;
  TString colfmt = "l";  for(size_t ich=0; ich<nch; ich++) colfmt += "c";
  TString colname("");
  for(size_t ich=0; ich<nch; ich++) {
    TString icol(ch[ich]); 
    icol.ReplaceAll("mu","\\mu"); icol.ReplaceAll("_"," ");
    colname = colname + "& " + icol;
  }
  cutflow << "\\begin{tabular}{" << colfmt << "} \\hline\\hline" << endl
       << "Process " << colname << " \\\\ \\hline" << flush;

  //backgrounds
  size_t nbckg=allShapes.find(ch[0]+shName)->second.bckg.size();
  for(size_t ibckg=0; ibckg<nbckg; ibckg++)
    {
      TH1 *h=allShapes.find(ch[0]+shName)->second.bckg[ibckg];
      TString procTitle(h->GetTitle()); procTitle.ReplaceAll("#","\\");
      cutflow << endl << procTitle << " ";
      for(size_t ich=0; ich<nch; ich++)
	{
	  h=allShapes.find(ch[ich]+shName)->second.bckg[ibckg];
	  Double_t valerr;
	  Double_t val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
	  cutflow << " & " << toLatexRounded(val,valerr);
	}
      cutflow << "\\\\";
    }
  cutflow << "\\hline" << flush;
  
  //total bckg
  cutflow << endl << "SM ";
  for(size_t ich=0; ich<nch; ich++)
    {
      TH1 *h=allShapes.find(ch[ich]+shName)->second.totalBckg;
      Double_t valerr;
      Double_t val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
      cutflow << " & " << toLatexRounded(val,valerr);
    }
  cutflow << "\\\\\\hline" << flush;

  //signal
  size_t nsig=allShapes.find(ch[0]+shName)->second.signal.size();
  for(size_t isig=0; isig<nsig; isig++)
    {
      TH1 *h=allShapes.find(ch[0]+shName)->second.signal[isig];
      TString procTitle(h->GetTitle()); procTitle.ReplaceAll("#","\\");
      cutflow << endl << procTitle << " ";
      for(size_t ich=0; ich<nch; ich++)
        {
          h=allShapes.find(ch[ich]+shName)->second.signal[isig];
          Double_t valerr;
          Double_t val = h->IntegralAndError(1,h->GetXaxis()->GetNbins(),valerr);
          cutflow << " & " << toLatexRounded(val,valerr);
        }
      cutflow << "\\\\";
    }
  cutflow << "\\hline" << flush;

  //data
  cutflow << endl << "data ";
  for(size_t ich=0; ich<nch; ich++)
    {
      TH1 *h=allShapes.find(ch[ich]+shName)->second.data;
      cutflow << " & " << int(h->Integral());
    }
  cutflow << "\\\\\\hline" << flush;

  // end table
  cutflow << "\\hline" << endl
       << "\\end{tabular}" << endl
       << "\\end{center}" << endl
       << "\\end{table}" << endl;

  cutflow.close();
}


//
void estimateNonResonantBackground(std::vector<TString> &selCh,TString ctrlCh,const map<TString, Shape_t> &allShapes, TString shape)
{

  fstream cutflow("CutFlow.tex",ios::out | ios::app);

  //table header
  const size_t nch=selCh.size();
  cutflow << endl << endl
	  <<"\\begin{table}[htp]" << endl
	  << "\\begin{center}" << endl
	  << "\\caption{Non resonant background estimation.}" << endl
	  <<"\\label{tab:table}" << endl;
  TString colfmt = "l";  for(size_t ich=0; ich<nch; ich++) colfmt += "c";
  TString colname("");
  for(size_t ich=0; ich<nch; ich++) {
    TString icol(selCh[ich]); 
    icol.ReplaceAll("mu","\\mu"); icol.ReplaceAll("_"," ");
    colname = colname + "& " + icol;
  }
  cutflow << "\\begin{tabular}{" << colfmt << "} \\hline\\hline" << endl
	  << "  " << colname << " \\\\ \\hline" << endl;

  //compute estimate
  string alphamcRow("\\alpha_{MC}");
  string nnrmcRow("N_{nr}^{MC}");
  string nnrexpRow("N_{nr}^{expected}");
  string alphaRow("\\alpha");
  string nnrRow("N_{nr}");
  for(size_t i=0; i<selCh.size(); i++)
    {
      TString key(selCh[i]+shape);

      //MC closure test
      TH1 *hch         = allShapes.find(key)->second.totalBckg;
      TH1 *hctrl       = allShapes.find(ctrlCh+shape)->second.totalBckg;
      Float_t ch       = hch->GetBinContent(5);     Float_t ch_err      = hch->GetBinError(5);
      Float_t ctrl     = hctrl->GetBinContent(5);   Float_t ctrl_err    = hctrl->GetBinError(5);
      Float_t ctrlSig  = hctrl->GetBinContent(1);   Float_t ctrlSig_err = hctrl->GetBinError(1);
      if(ctrl)
	{
	  Float_t alpha(ch/ctrl), alpha_err( sqrt(pow(ch*ctrl_err,2)+pow(ch_err*ctrl,2))/pow(ctrl,2) );
	  alphamcRow += " & " + toLatexRounded(alpha,alpha_err); 

	  Float_t nnr(alpha*ctrlSig) , nnr_err( sqrt(pow(ch*ctrlSig*ctrl_err,2)+pow(ch_err*ctrlSig*ctrl,2)+pow(ch*ctrlSig_err*ctrl,2) )/pow(ctrl,2) );
	  nnrmcRow += " & " + toLatexRounded(nnr,nnr_err);
	}
      else
	{
	  alphamcRow += " & - ";
	  nnrmcRow   += " & - ";
	}
      Float_t nnrexp(0),nnrexp_err(0);
      for(size_t ibckg=0; ibckg<allShapes.find(key)->second.bckg.size(); ibckg++)
	{
	  TH1 *hmc=allShapes.find(key)->second.bckg[ibckg];
	  TString procTit(hmc->GetTitle());
	  if(!procTit.Contains("WW") && !procTit.Contains("t#bar{t}") && !procTit.Contains("Single top") && !procTit.Contains("#tau#tau")) continue;
	  nnrexp += hmc->GetBinContent(1);  nnrexp_err += pow(hmc->GetBinError(1),2);
	}
      nnrexp_err = sqrt(nnrexp_err);
      nnrexpRow += " & " + toLatexRounded(nnrexp,nnrexp_err);

      //data estimate
      hch         = allShapes.find(key)->second.data;
      hctrl       = allShapes.find(ctrlCh+shape)->second.data;
      ch       = hch->GetBinContent(5);     ch_err      = hch->GetBinError(5);
      ctrl     = hctrl->GetBinContent(5);   ctrl_err    = hctrl->GetBinError(5);
      ctrlSig  = hctrl->GetBinContent(1);   ctrlSig_err = hctrl->GetBinError(1);
      if(ctrl)
	{
	  Float_t alpha(ch/ctrl), alpha_err( sqrt(pow(ch*ctrl_err,2)+pow(ch_err*ctrl,2))/pow(ctrl,2) );
	  alphaRow += " & " + toLatexRounded(alpha,alpha_err); 

	  Float_t nnr(alpha*ctrlSig) , nnr_err( sqrt(pow(ch*ctrlSig*ctrl_err,2)+pow(ch_err*ctrlSig*ctrl,2)+pow(ch*ctrlSig_err*ctrl,2) )/pow(ctrl,2) );
	  nnrRow += " & " + toLatexRounded(nnr,nnr_err);
	}
      else
	{
	  alphaRow += " & - ";
	  nnrRow   += " & - ";
	}
    }	  
  cutflow << alphamcRow << " \\\\" << endl
	  << nnrmcRow << " \\\\" << endl
	  << nnrexpRow << "\\\\\\hline" << endl
	  << alphaRow  << " \\\\" << endl
	  << nnrRow << "\\\\\\hline" << endl;

  // end table
  cutflow << "\\hline\\hline" << endl
       << "\\end{tabular}" << endl
       << "\\end{center}" << endl
       << "\\end{table}" << endl;

} 

//

//
int findBinFor(Float_t minMet, Float_t minMt, Float_t maxMt,TString infURL)
{ 
  int reqBin(0);

  //find the bin from the histos defining the cuts
  //take them from the first sub-directory in the plots file
  TFile *inF = TFile::Open(infURL);
  if( inF->IsZombie() ){ cout << "Invalid file name : " << infURL << endl; return reqBin; }
  TIter pnext(inF->GetListOfKeys());
  TDirectoryFile *pdir= (TDirectoryFile *)pnext();
  if(pdir==0) {inF->Close(); return reqBin; }
  TString dirName(pdir->GetName());
  TH1 *hcutsmet   = (TH1 *)inF->Get(dirName+"/optim_cut1_met");
  TH1 *hcutsmtmin = (TH1 *)inF->Get(dirName+"/optim_cut1_mtmin");
  TH1 *hcutsmtmax = (TH1 *)inF->Get(dirName+"/optim_cut1_mtmax");
  for(int ibin=1; ibin<=hcutsmet->GetXaxis()->GetNbins(); ibin++)
    {
      Float_t metcut   = hcutsmet->GetBinContent(ibin);
      Float_t mtmincut = hcutsmtmin->GetBinContent(ibin);
      Float_t mtmaxcut = hcutsmtmax->GetBinContent(ibin);
      if(metcut!= minMet || mtmincut!=minMt || mtmaxcut!=maxMt) continue;
      reqBin=ibin;
      break;
    }
  inF->Close();

  //done
  return reqBin;
}



//
Shape_t getShapeFromFile(TString ch, TString shapeName, int cutBin, TString infURL, JSONWrapper::Object &Root)
{
  Shape_t shape; shape.totalBckg=0;

  TFile *inF = TFile::Open(infURL);
  if( inF->IsZombie() ){ cout << "Invalid file name : " << infURL << endl; return shape; }

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
            if(hshape->Integral()<=0){hshape->Reset(); hshape->SetBinContent(1, 1E-10);}
	    hshape->SetDirectory(0);
	    hshape->SetTitle(proc);
	    fixExtremities(hshape,true,true);
	    hshape->SetFillColor(color); hshape->SetLineColor(lcolor); hshape->SetMarkerColor(mcolor);
	    hshape->SetFillStyle(fill);  hshape->SetLineWidth(lwidth); hshape->SetMarkerStyle(marker);
         }else{		
            printf("Histo does not exist: %s\n", histoName.Data());
            continue;
         }
	
	 //save in structure
	 if(isData){
	    if(varName=="")  shape.data=hshape;
	    else continue;
	 }else if(isSignal){

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

  inF->Close();

  //compute the total
  for(size_t i=0; i<shape.bckg.size(); i++)
    {
      if(i==0) { shape.totalBckg = (TH1 *)shape.bckg[i]->Clone(ch+"_"+shapeName+"_total"); shape.totalBckg->SetDirectory(0); }
      else     { shape.totalBckg->Add(shape.bckg[i]); }
    }

  if(MCclosureTest){
     shape.data->Reset();
     shape.data->Add(shape.totalBckg, 1);
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


//
std::vector<TString>  buildDataCard(Int_t mass, TString histo, TString url, TString Json, TString outDir, bool runSystematics, bool shape, Int_t index)
{
  std::vector<TString> dcUrls;
  

  //get the datacard inputs 
  DataCardInputs dci = convertHistosForLimits(mass,histo,url,Json, outDir, runSystematics, shape, index);


  //build the datacard separately for each channel
  for(size_t i=1; i<=dci.ch.size(); i++) 
    {
  
      ofstream dcF;
      TString dcName=outDir +"/" + dci.shapesFile;
      dcName.ReplaceAll(".root","_"+dci.ch[i-1]+".dat");
      dcF.open(dcName.Data(),ios::out);

      //header
      dcF << "imax 1" << endl
	  << "jmax *" << endl //<< dci.procs.size() << endl
	  << "kmax *" << endl
	  << "-------------------------------" << endl
	  << "shapes * * " << outDir << "/" << dci.shapesFile << " $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  
      //observations
      dcF << "-------------------------------" << endl
	  << "bin " << dci.ch[i-1] << endl
	  << "Observation " << dci.obs[RateKey_t("obs",dci.ch[i-1])] << endl;
      
      //yields per bin
      dcF << "-------------------------------" << endl
	  << "bin\t ";
      for(size_t j=1; j<=dci.procs.size(); j++) 
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1 << " ";
	}
      dcF << endl
	  << "process\t ";
      for(size_t j=1; j<=dci.procs.size(); j++) 
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << dci.procs[j-1] << " ";
	}
      dcF << endl
	  << "process\t ";
      int procCtr(0);
      for(size_t j=1; j<=dci.procs.size(); j++) 
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << procCtr << " ";
	  procCtr++;
	}
      dcF << endl
	  << "rate\t";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])] << " ";
	}
      dcF << endl;

if(runSystematics){
      
      //systematics
      dcF << "-------------------------------" << endl;
      dcF << "Lumi\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1.045 << " ";
	}
      dcF << endl;
  
      dcF << "trigEff\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1.01 << "  ";
	}
      dcF << endl;

      dcF << "idEff\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1.03 << "  ";
	}
      dcF << endl;
      
      //systematics with shape description
      for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator it=dci.systs.begin(); it!=dci.systs.end(); it++)
	{
	  dcF << it->first << "\t shapeN2\t";
	  for(size_t j=1; j<=dci.procs.size(); j++)
	    {
	      if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	      if(it->second.find(RateKey_t(dci.procs[j-1],dci.ch[i-1])) != it->second.end())
		dcF << it->second[RateKey_t(dci.procs[j-1],dci.ch[i-1])];
	      else
		dcF << "-";
	      dcF << " ";
	    }
	  dcF << endl;
	}
}
  
      dcF.close();
      cout << "Data card for " << dci.shapesFile << " and " << dci.ch[i-1] << " channel @ " << dcName << endl;
      dcUrls.push_back(dcName);
    }

  //all done
  return dcUrls;
}


//
DataCardInputs convertHistosForLimits(Int_t mass,TString histo,TString url,TString Json, TString outDir, bool runSystematics, bool shape, Int_t index)
{
  DataCardInputs dci;
 
  //init the json wrapper
  JSONWrapper::Object Root(Json.Data(), true);

  //init globalVariables
  TString massStr(""); massStr += mass;
  std::set<TString> allCh,allProcs;
  
  //get the shapes for each channel
  map<TString, Shape_t> allShapes;
  TString ch[]={"mumu","ee","emu"};
  const size_t nch=sizeof(ch)/sizeof(TString);
  TString sh[]={"nonresbckg_ctrl", histo};
  const size_t nsh=sizeof(sh)/sizeof(TString);
  for(size_t i=0; i<nch; i++){
      for(size_t j=0; j<nsh; j++){
	  allShapes[ch[i]+sh[j]]=getShapeFromFile(ch[i],sh[j],index,url,Root);
      }
  }

  //define vector for search
  std::vector<TString> selCh; selCh.push_back("ee"); selCh.push_back("mumu");

  //print event yields from the mt shapes
  //getCutFlowFromShape(selCh,allShapes,histo);

  //non-resonant background estimation
  //estimateNonResonantBackground(selCh,"emu",allShapes,"nonresbckg_ctrl");

  //remove the non-resonant background from data
  if(subNRB2011 || subNRB2012)doBackgroundSubtraction(selCh,"emu",allShapes,histo,"nonresbckg_ctrl");

  //prepare the output
  dci.shapesFile="Shapes_"+massStr+".root";
  TFile *fout=TFile::Open(outDir + "/" + dci.shapesFile,"recreate");

  //loop on channel/proc/systematics
  for(size_t ich=0; ich<selCh.size(); ich++){
     fout->mkdir(ch[ich]);
     fout->cd(ch[ich]);
     allCh.insert(ch[ich]);
     Shape_t shapeSt = allShapes.find(selCh[ich]+histo)->second;

     //backgrounds
     size_t nbckg=allShapes.find(ch[ich]+histo)->second.bckg.size();
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
        convertHistosForLimits_core(dci, proc, ch[ich], systs, hshapes, runSystematics, shape, index);
        allProcs.insert(proc);
     }


     //signals
     size_t nsignal=allShapes.find(ch[ich]+histo)->second.signal.size();
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
	proc = "asignal";
        convertHistosForLimits_core(dci, proc, ch[ich], systs, hshapes, runSystematics, shape, index);
        allProcs.insert(proc);
     }


     //data
     TH1* h=shapeSt.data;
//     TH1* h=shapeSt.totalBckg;
     std::vector<TString> systs;
     std::vector<TH1*>    hshapes;
     systs.push_back("");
     hshapes.push_back(h);
     TString proc(h->GetTitle());
     convertHistosForLimits_core(dci, proc, ch[ich], systs, hshapes, runSystematics, shape, index);

     //return to parent dir
     fout->cd("..");     
  }
  dci.ch.resize(allCh.size());        std::copy(allCh.begin(), allCh.end(),dci.ch.begin());
  dci.procs.resize(allProcs.size());  std::copy(allProcs.begin(), allProcs.end(),dci.procs.begin());

/*
// DEBUGGING
  printf("-----------------------\n");
  printf("shapesFile=%s\n",dci.shapesFile.Data());
  for(unsigned int i=0;i<dci.ch.size();i++){printf("%s - ",dci.ch[i].Data());}printf("\n");
  for(unsigned int i=0;i<dci.procs.size();i++){printf("%s - ",dci.procs[i].Data());}printf("\n");
  for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator iter = dci.systs.begin();   iter != dci.systs.end(); ++iter ){
       printf("%s : ", iter->first.Data());
       for(std::map<RateKey_t, Double_t>::iterator it = iter->second.begin();   it != iter->second.end(); ++it ){
                 printf("%s_%s (%f) ", it->first.first.Data(), it->first.second.Data(), it->second);
       }
       printf("\n");
  }
  printf("-----------------------\n");
*/

/*
  //################# START BACKGROUND SUBTRACTION CODE

    TH1* proj_em = ((TH2*)fin->Get(TString("data/emu_" ) + "nonresbckg_ctrl" ))->ProjectionY("_py", index,index);
    TH1* proj_mm = ((TH2*)fin->Get(TString("data/mumu_") + "nonresbckg_ctrl" ))->ProjectionY("_py", index,index);
    TH1* proj_ee = ((TH2*)fin->Get(TString("data/ee_"  ) + "nonresbckg_ctrl" ))->ProjectionY("_py", index,index);

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



void convertHistosForLimits_core(DataCardInputs& dci, TString& proc, TString& ch, std::vector<TString>& systs, std::vector<TH1*>& hshapes,  bool runSystematics, bool shape, Int_t index){
   proc.ReplaceAll("#bar{t}","tbar");
   proc.ReplaceAll("Z-#gamma^{*}+jets#rightarrow ll","dy");
   proc.ReplaceAll("(","");    proc.ReplaceAll(")","");    proc.ReplaceAll("+","");    proc.ReplaceAll(" ","");
   proc.ToLower();

   for(unsigned int i=0;i<systs.size();i++){
       TString syst   = systs[i];
       TH1*    hshape = hshapes[i];
       hshape->SetDirectory(0);

       //Do Renaming and cleaning
       syst.ReplaceAll("down","Down");
       syst.ReplaceAll("up","Up");
       if(syst.Contains("btag")) continue;

       //If cut&count keep only 1 bin in the histo
       if(!shape){
          hshape = hshape->Rebin(hshape->GetXaxis()->GetNbins(), TString(hshape->GetName())+"_Rebin"); 
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
         if(proc=="data")  hshape->SetName("data_obs");
         hshape->Write();
       }else if(runSystematics && proc!="data" && (syst.Contains("Up") || syst.Contains("Down"))){
         //write variation to file
         hshape->SetName(proc+syst);
         hshape->Write();
       }else if(runSystematics){
         //for one sided systematics the down variation mirrors the difference bin by bin
         hshape->SetName(proc+syst);
         hshape->Write(proc+syst+"Up");
         TH1 *hmirrorshape=(TH1 *)hshape->Clone(proc+syst+"Down");
         for(int ibin=1; ibin<=hmirrorshape->GetXaxis()->GetNbins(); ibin++){
//            double bin = hmirrorshape->GetBinContent(ibin);
            double bin = 2*hshapes[0]->GetBinContent(ibin)-hmirrorshape->GetBinContent(ibin);
            if(bin<0)bin=0;
            hmirrorshape->SetBinContent(ibin,bin);
         }
	 if(hmirrorshape->Integral()<=0)hmirrorshape->SetBinContent(1, 1E-10);
         hmirrorshape->Write(proc+syst+"Down");
       }

       if(runSystematics && syst!=""){
          TString systName(syst); 
          systName.ReplaceAll("Up",""); systName.ReplaceAll("Down","");  systName.ReplaceAll("_","");

          TH1 *temp=(TH1*) hshape->Clone();
          temp->Add(hshapes[0],-1);
        if(temp->Integral()!=0)dci.systs[systName][RateKey_t(proc,ch)]=1.0;
          delete temp;
        }else if(proc!="data" && syst==""){if(hshape->Integral()>1E-9)dci.rates[RateKey_t(proc,ch)]=hshape->Integral();
        }else if(proc=="data" && syst==""){dci.obs[RateKey_t("obs",ch)]=hshape->Integral();
        }
   }
}

void doBackgroundSubtraction(std::vector<TString>& selCh,TString ctrlCh,map<TString, Shape_t>& allShapes, TString mainHisto, TString sideBandHisto)
{
     for(size_t i=0;i<selCh.size();i++){
        Shape_t& shapeCtrl_SB = allShapes.find(ctrlCh+sideBandHisto)->second;
        TH1* hCtrl_SB=shapeCtrl_SB.data;
        Shape_t& shapeCtrl_SI = allShapes.find(ctrlCh+mainHisto)->second;
        TH1* hCtrl_SI=shapeCtrl_SI.data;
        Shape_t& shapeChan_SB = allShapes.find(selCh[i]+sideBandHisto)->second;
        TH1* hChan_SB=shapeChan_SB.data;
        Shape_t& shapeChan_SI = allShapes.find(selCh[i]+mainHisto)->second;
        TH1* hChan_SI=shapeChan_SI.data;

	printf("Channel = %s\n", selCh[i].Data());
        printf("Bin %f %f %f %f %f %f\n", hCtrl_SB->GetBinContent(1), hCtrl_SB->GetBinContent(2), hCtrl_SB->GetBinContent(3), hCtrl_SB->GetBinContent(4), hCtrl_SB->GetBinContent(5), hCtrl_SB->GetBinContent(6) );
        printf("Bin %f %f %f %f %f %f\n", hChan_SB->GetBinContent(1), hChan_SB->GetBinContent(2), hChan_SB->GetBinContent(3), hChan_SB->GetBinContent(4), hChan_SB->GetBinContent(5), hChan_SB->GetBinContent(6) );
        double alpha     = hChan_SB->GetBinContent(5) / hCtrl_SB->GetBinContent(5);
        double alpha_err = ( fabs( hChan_SB->GetBinContent(5) * hCtrl_SB->GetBinError(5) ) + fabs(hChan_SB->GetBinError(5) * hCtrl_SB->GetBinContent(5) )  ) / pow(hCtrl_SB->GetBinContent(5), 2);        
        printf("alpha %s=%f+-%f\n", selCh[i].Data(),alpha, alpha_err);

        //add 100% syst uncertainty on alpha
        alpha_err = sqrt(1+alpha_err*alpha_err);

        printf("Integral = %f --> ", hChan_SI->Integral());
        if(subNRB2011){
           for(int b=0;b<=hCtrl_SI->GetXaxis()->GetNbins()+1;b++){
  	      double val = hCtrl_SI->GetBinContent(b);
              double err = hCtrl_SI->GetBinError(b);
              val = val*alpha;
              err = err*alpha + val*alpha_err;
              hCtrl_SI->SetBinContent(b, val );
              hCtrl_SI->SetBinError  (b, err );
           }
           hChan_SI->Add(hCtrl_SI,-1);
           for(int b=0;b<=hChan_SI->GetXaxis()->GetNbins()+1;b++){
              if(hChan_SI->GetBinContent(b)<0)hChan_SI->SetBinContent(b,0.0);
           }
        }else if(subNRB2012){
           for(int b=0;b<=hCtrl_SB->GetXaxis()->GetNbins()+1;b++){
              double val = hCtrl_SB->GetBinContent(b);
              double err = hCtrl_SB->GetBinError(b);
              val = val*alpha;
              err = err*alpha + val*alpha_err;
              hCtrl_SB->SetBinContent(b, val );
              hCtrl_SB->SetBinError  (b, err );
           }
           hChan_SI->Add(hCtrl_SB,-1);
           for(int b=0;b<=hChan_SI->GetXaxis()->GetNbins()+1;b++){
              if(hChan_SI->GetBinContent(b)<0)hChan_SI->SetBinContent(b,0.0);
           }
        }
        printf("%f\n", hChan_SI->Integral());


        //Clean background collection
        size_t nbckg=shapeChan_SI.bckg.size();
        for(size_t ibckg=0; ibckg<nbckg; ibckg++){
           TString proc(shapeChan_SI.bckg[ibckg]->GetTitle());
	   if((subNRB2011 && (proc.Contains("t#bar{t}") || proc.Contains("Single top") || proc.Contains("WW") || proc.Contains("Z#rightarrow #tau#tau")) ) ||
              (subNRB2012 && (proc.Contains("t#bar{t}") || proc.Contains("Single top") ) ) ){
              shapeChan_SI.totalBckg->Add(shapeChan_SI.bckg[ibckg], -1);
	      shapeChan_SI.bckg.erase(shapeChan_SI.bckg.begin()+ibckg);  ibckg--;
           }
        }
     }

}




