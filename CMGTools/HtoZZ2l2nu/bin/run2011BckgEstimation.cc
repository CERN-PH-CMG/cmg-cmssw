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

//wrapper for a projected shape for a given set of cuts
struct Shape_t
{
  TH1* data, *totalBckg;
  std::vector<TH1 *> bckg, signal;
  //the index corresponds to the entry in the bckg/signal vectors
  //the key is the name of the variation: e.g. jesup, jesdown, etc.
  std::map<int,std::vector<std::pair<TString, TH1*> > > bckgVars, signalVars;
};

void printHelp();
int findBinFor(Float_t minMet, Float_t minMt, Float_t maxMt, TString infURL);
Shape_t getShapeFromFile(TString ch, TString shapeName, int cutBin, TString infURL,JSONWrapper::Object &Root);
void showShape(const Shape_t &shape, TString SaveName);
void getCutFlowFromShape(std::vector<TString> ch, const map<TString, Shape_t> &allShapes, TString shName);
void estimateNonResonantBackground(std::vector<TString> &selCh,TString ctrlCh,const map<TString, Shape_t> &allShapes, TString shape);


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


  TString inFileUrl(""),jsonFile("");
  Float_t reqMetCut(50), reqMtMinCut(200), reqMtMaxCut(350);
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg.find("--help")!=string::npos) { printHelp(); return -1;} 
    if(arg.find("--in")!=string::npos && i+1<argc)   { inFileUrl  = argv[i+1];  i++;  printf("in = %s\n", inFileUrl.Data()); }
    if(arg.find("--json")!=string::npos && i+1<argc) { jsonFile = argv[i+1];    i++;  printf("json = %s\n", jsonFile.Data()); }
    if(arg.find("--met") != string::npos && i+1<argc)  { sscanf(argv[i+1],"%f",&reqMetCut); i++; }
    if(arg.find("--mtmin") != string::npos && i+1<argc)  { sscanf(argv[i+1],"%f",&reqMtMinCut); i++; }
    if(arg.find("--mtmax") != string::npos && i+1<argc)  { sscanf(argv[i+1],"%f",&reqMtMaxCut); i++; }
  }
  if(jsonFile.IsNull() || inFileUrl.IsNull()) { printHelp(); return -1; }
 
  //init the json wrapper
  JSONWrapper::Object Root(jsonFile.Data(), true);

  //find the bin to project
  cout << "Projecting for selection: MET>" << reqMetCut << " " << reqMtMinCut << "<MT<" << reqMtMaxCut << endl; 
  int reqBin = findBinFor(reqMetCut, reqMtMinCut, reqMtMaxCut, inFileUrl);
  if(reqBin==0) { cout << "Could not find bin corresponding to the bins required" << endl; }
  else          { cout << "Projecting shapes from bin: " << reqBin << endl;                }
  
  //get the shapes for each channel
  map<TString, Shape_t> allShapes;
  TString ch[]={"mumu","ee","emu"};
  const size_t nch=sizeof(ch)/sizeof(TString);
  TString sh[]={"mt3","nonresbckg_ctrl","mt_shapes"};
  const size_t nsh=sizeof(sh)/sizeof(TString);
  for(size_t i=0; i<nch; i++)
    {
      for(size_t j=0; j<nsh; j++)
	{
	  allShapes[ch[i]+sh[j]]=getShapeFromFile(ch[i],sh[j],reqBin,inFileUrl,Root);
	  //showShape(allShapes[ch[i]+sh[j]],ch[i]+"_"+sh[j]);
	}
    }

  //print event yields from the mt shapes
  std::vector<TString> selCh; selCh.push_back("ee"); selCh.push_back("mumu");
  getCutFlowFromShape(selCh,allShapes,"mt_shapes");

  // non-resonant background estimation
  estimateNonResonantBackground(selCh,"emu",allShapes,"nonresbckg_ctrl");
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
void printHelp()
{
  printf("Options\n");
  printf("--in   --> input file with from plotter\n");
  printf("--json --> json file with the sample descriptor\n");
  printf("--met  --> met cut to apply\n");
  printf("--mtmin --> min MT to require\n");
  printf("--mtmax --> max MT to require\n");

}

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
      if(pdir==0) continue;

      bool isData(Process[i]["isdata"].toBool());
      bool isSignal(Process[i]["spimpose"].toBool());
      int color(1);       if(Process[i].isTag("color" ) ) color  = (int)Process[i]["color" ].toDouble();
      int lcolor(color);  if(Process[i].isTag("lcolor") ) lcolor = (int)Process[i]["lcolor"].toDouble();
      int mcolor(color);  if(Process[i].isTag("mcolor") ) mcolor = (int)Process[i]["mcolor"].toDouble();
      int fcolor(color);  if(Process[i].isTag("fcolor") ) fcolor = (int)Process[i]["fcolor"].toDouble();
      int lwidth(1);      if(Process[i].isTag("lwidth") ) lwidth = (int)Process[i]["lwidth"].toDouble();
      int fill(1001);     if(Process[i].isTag("fill"  ) ) fill   = (int)Process[i]["fill"  ].toDouble();
      int marker(20);     if(Process[i].isTag("marker") ) marker = (int)Process[i]["marker"].toDouble();
      
      //iterate over all the histos produced for this process
      TIter pnext(pdir->GetListOfKeys());   pdir->cd();
      TObject *p=0;
      while((p=pnext())){
	      
	TString objName=p->GetName();
	if(!objName.BeginsWith(ch+"_"+shapeName) ) continue;

	//check if this corresponds to a systematic variation
	TString varName("");
	if(!objName.EndsWith(shapeName))
	  {
	    TObjArray *tkns=objName.Tokenize("_");
	    TObjString *lastTkn=(TObjString *)tkns->At(tkns->GetEntriesFast()-1);
	    varName=lastTkn->GetString();
	  }

	//project the histogram for the required cut
	TH2F *hshape2D = (TH2F *)pdir->Get(objName);
	objName.ReplaceAll(ch,ch+"_proj"+procCtr);
	TH1D *hshape   = hshape2D->ProjectionY(objName,cutBin,cutBin);
	hshape->SetDirectory(0);
	hshape->SetTitle(proc);
	fixExtremities(hshape,true,true);
	hshape->SetFillColor(color); hshape->SetLineColor(lcolor); hshape->SetMarkerColor(mcolor);
	hshape->SetFillStyle(fill);  hshape->SetLineWidth(lwidth); hshape->SetMarkerStyle(marker);
	
	//save in structure
	if(isData)
	  {
	    if(varName=="")  shape.data=hshape;
	    else continue;
	  }
	else if(isSignal)
	  {
	    if(varName=="")  shape.signal.push_back(hshape);
	    else             shape.signalVars[i].push_back( std::pair<TString,TH1*>(varName,hshape) );
	  }
	else
	  {
	    if(varName=="")  shape.bckg.push_back(hshape);
	    else             shape.bckgVars[i].push_back( std::pair<TString,TH1*>(varName,hshape) );
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


