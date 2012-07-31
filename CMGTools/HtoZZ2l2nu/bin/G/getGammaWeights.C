//
// .L getGammaWeights.C+
// getGammaWeights("plotter.root");
//

#include <iostream>
#include <strstream>

#include "TF1.h"
#include "TH1F.h"
#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TGraphErrors.h"

string cats[]      ={"eq0jets", "eq1jets", "eq2jets",  "geq3jets", "vbf", "geq1jets"};
TString catLabels[]={"=0 jets", "=1 jets", "=2 jets", "#geq 3 jets","VBF", "geq1jets"};
const size_t nCats=sizeof(cats)/sizeof(string);

string dilCats[]       = {"ee","mumu"};
TString dilCatLabels[] = {"ee","#mu#mu"};
const size_t nDilCats=sizeof(dilCats)/sizeof(string);

string dymcDir="Z#rightarrow ll";
string dyDir="data (ll)";
string gmcDirs[] = { "Z#gamma#rightarrow#nu#nu#gamma",
		     "W#gamma#rightarrowl#nu#gamma",
		     "W#rightarrow l#nu",
		     "QCD",
		     "#gamma+jets" };
bool gmcPure[] = { false,
		   false,
		   false,
		   true,
		   true };
const size_t nGmcDirs=sizeof(gmcDirs)/sizeof(string);
string gDir = "data (#gamma)";

TF1 *qtFitFunc = new TF1("qtff","[0]*exp( - TMath::Abs(x-[1])/([2]+[3]*TMath::Abs(x-[1])) )",50,1000);
			  
//
TObject* getObjectFromPath(TDirectory* File, std::string Path, bool GetACopy)
{
   TObject* toReturn = NULL;
   if(File==0) return toReturn;
   
   size_t pos = Path.find("/");
   if(pos < std::string::npos){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());
      toReturn =  getObjectFromPath(TMP,endPart,GetACopy);
   }else{
     toReturn = File->Get(Path.c_str());
   }
   
   if(!toReturn)       printf("BUG: %s\n",Path.c_str());
   else if(GetACopy && toReturn){ toReturn = toReturn->Clone();  }
   return toReturn;
}

//init 
void resetQtFitFunc(TH1 *h)
{
  if(h==0) return;
  if(h->Integral()==0) return;
  Double_t xmin(h->GetXaxis()->GetBinLowEdge(1)), xmax(h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetNbins()));
  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      Double_t val=h->GetBinContent(ibin);
      if(val==0) continue;
      qtFitFunc->SetParameter(0,h->Integral()); //qtFitFunc->SetParLimits(0,h->Integral()*0.5,h->Integral()*1.5);
      xmin=h->GetBinLowEdge(ibin);
      qtFitFunc->SetParameter(1,xmin-h->GetBinWidth(ibin)); // qtFitFunc->SetParLimits(1,xmin*0.9,xmin*1.1);
      break;
    }
  qtFitFunc->SetParameter(2,10.);
  qtFitFunc->SetParameter(3,0.02); qtFitFunc->SetParLimits(3,0.0,0.1);
  qtFitFunc->SetRange(xmin,xmax);
}

//
enum WeightsMode { ALL, ONLYDATA, ONLYMC };
void getGammaWeights(string inputFile="plotter.root",string varName="qt",int mode=ALL)
{
  strstream report;

  //output
  TString foutName="gamma"+varName+"weights.root";
  TFile *fout = TFile::Open(foutName,"RECREATE");   fout->cd();
  
  //get plots from file for each category
  TFile *fin = TFile::Open(inputFile.c_str());
  std::vector<TH1F *> dataWgts,     mcWgts;
  std::vector<TH1F *> dataVtxWgts,     mcVtxWgts;
  std::vector<TH1F *> mcMassShapes, dataMassShapes;
  for(size_t id=0; id<nDilCats; id++)
    {
      for(size_t ic=0; ic<nCats; ic++)
	{
	  //mass shape
	  if(mode != ONLYMC)
	    { 
	      //string pName=dilCats[id]+cats[ic]+"_zmass";
	      string pName=dilCats[id]+"_zmass";
	      string newPname=dilCats[id]+cats[ic]+"_datazmass";
	      TH1F *hdymass=(TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);      
	      if(hdymass)
		{
		  hdymass->SetName(newPname.c_str());  
		  hdymass->SetDirectory(0); 
		  dataMassShapes.push_back( hdymass );
		}
	    }

	  if(ic==0)
	    {	      
	      if( mode != ONLYDATA )
		{
		  string pName=dilCats[id]+"_zmass";
		  string newPname=dilCats[id]+"_mczmass";
		  TH1F * hmcdymass=(TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true); 
		  if(hmcdymass)
		    {
		      hmcdymass->SetName(newPname.c_str()); 
		      hmcdymass->SetDirectory(0);
		      mcMassShapes.push_back( hmcdymass );
		    }
		}
	    }

	  //get the histos 
	  string pName=dilCats[id]+cats[ic]+"_"+varName;
	  TH1F *hdy   = 0;
	  if(mode != ONLYMC )  { hdy   = (TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);   if(hdy==0) continue;  hdy->SetDirectory(0);   }
	  TH1F *hmcdy = 0;
	  if(mode != ONLYDATA) { hmcdy = (TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true); if(hmcdy==0) { if(mode==ONLYMC) continue; } else hmcdy->SetDirectory(0); }

	  //DERIVE WEIGHTS
	  string gPName(pName);
	  TH1F *hg    = 0;
	  TH1F *hDataWgts = 0;
	  
	  if(mode!=ONLYMC)
	    {
	      cout << " ----> " <<  gDir << endl;
	      hg = (TH1F *) getObjectFromPath(fin,gDir+"/"+gPName,true);
	      if(hg)
		{
		  //SIMPLE RATIO
		  hDataWgts = (TH1F *) hdy->Clone((pName+"_datawgts").c_str()); 
		  hDataWgts->SetDirectory(0); 
		  hDataWgts->Divide(hg);    
		  dataWgts.push_back(hDataWgts);
		  
		  //PARAMETRIZED FUNCTION
		  if(varName=="qt")
		    {
		      //resetQtFitFunc(hdy);
		      //hdy->Fit(qtFitFunc,"WLMER+");
		      TGraph *splHdy = new TGraph(hdy);
		      hDataWgts   = (TH1F *) hdy->Clone((pName+"_datafitwgts").c_str()); 
		      hDataWgts->SetDirectory(0);  
		      hDataWgts->Reset("ICE");
		      //		      for(int ibin=1; ibin<=hDataWgts->GetXaxis()->GetNbins(); ibin++) hDataWgts->SetBinContent(ibin,qtFitFunc->Eval(hDataWgts->GetBinCenter(ibin)));
		      for(int ibin=1; ibin<=hDataWgts->GetXaxis()->GetNbins(); ibin++) hDataWgts->SetBinContent(ibin,splHdy->Eval(hDataWgts->GetBinCenter(ibin),0,"S"));
		      hDataWgts->Divide(hg);
		      dataWgts.push_back(hDataWgts);
		      delete splHdy;
		    }
		}
	    }
	  
	  //MC
	  if(mode != ONLYDATA)
	    {
	      TH1F *hmcg=0;
	      TH1F *hMCwgts=0;
	      TH1F *hMCfitWgts=0; 

	      //build the total MC spectra
	      for(size_t imc=0; imc<nGmcDirs; imc++)
		{
		  if(hmcg==0) hmcg =     (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+gPName,true);
		  else        hmcg->Add( (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+gPName,false) ); 
		}
	      
	      //SIMPLE RATIO
	      //reweight to ZpT observed
	      if(mode==ONLYMC && hmcdy) hMCwgts   = (TH1F *) hmcdy->Clone((pName+"_mcwgts").c_str()); 
	      else if (hdy)             hMCwgts   = (TH1F *) hdy->Clone((pName+"_mcwgts").c_str()); 
	      if(hMCwgts)
		{
		  hMCwgts->SetDirectory(0);   
		  hMCwgts->Divide(hmcg);    
		  mcWgts.push_back(hMCwgts);
		}
		
	      if(varName=="qt")
		{
		  
		  //PARAMETRIZED ZPT
		  //reset parameterization for boson pt
		  if(hmcdy)
		    {
		      hMCfitWgts   = (TH1F *) hmcdy->Clone((pName+"_mcfitwgts").c_str()); 
		      hMCfitWgts->SetDirectory(0);  
		      hMCfitWgts->Reset("ICE");
		      TGraph *splHdy=0;
		      if(mode==ONLYMC && hmcdy)  { splHdy=new TGraph(hmcdy); }
		      else if (hdy)              { splHdy=new TGraph(hdy); }
		      //if(mode==ONLYMC && hmcdy)  { resetQtFitFunc(hmcdy); hmcdy->Fit(qtFitFunc,"MER+"); }
		      //else if (hdy)              { resetQtFitFunc(hdy);   hdy->Fit(qtFitFunc,"MER+");   }
		      //for(int ibin=1; ibin<=hMCfitWgts->GetXaxis()->GetNbins(); ibin++) hMCfitWgts->SetBinContent(ibin,qtFitFunc->Eval(hMCfitWgts->GetBinCenter(ibin)));
		      for(int ibin=1; ibin<=hMCfitWgts->GetXaxis()->GetNbins(); ibin++) hMCfitWgts->SetBinContent(ibin,splHdy->Eval(hMCfitWgts->GetBinCenter(ibin),0,"S"));
		      hMCfitWgts->Divide(hmcg);
		      if(splHdy) delete splHdy;
		      mcWgts.push_back(hMCfitWgts);
		    }
		}
	    }
	}
    }
  fin->Close();
 
  //save histograms to file

  cout << "*** Gamma pT weights (inclusive/differential) available @ " << foutName << endl;
  cout << report.str() << endl;

  fout->cd();
  if(mode != ONLYMC) 
    {
      for(size_t ip=0; ip<dataWgts.size(); ip++)        { dataWgts[ip]->Write(); cout << dataWgts[ip]->GetName() << endl; }
      for(size_t ip=0; ip<dataMassShapes.size(); ip++)  dataMassShapes[ip]->Write();
    }
  if(mode != ONLYDATA) 
    {
      for(size_t ip=0; ip<mcWgts.size(); ip++)          mcWgts[ip]->Write();
      for(size_t ip=0; ip<mcMassShapes.size(); ip++)    mcMassShapes[ip]->Write();
    }
  fout->Close();
}
