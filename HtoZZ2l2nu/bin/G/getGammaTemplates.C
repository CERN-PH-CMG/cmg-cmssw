/*
  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  .L getGammaTemplates.C+
 */
#include <iostream>


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"

string cats[]      ={"eq0jets", "eq1jets", "eq2jets",  "geq3jets"};
TString catLabels[]={"=0 jets", "=1 jets", "=2 jets", "#geq 3 jets"};
const size_t nCats=sizeof(cats)/sizeof(string);

string dilCats[]       = {"ee","mumu"};
TString dilCatLabels[] = {"ee","#mu#mu"};
const size_t nDilCats=sizeof(dilCats)/sizeof(string);

string dymcDir="Z#rightarrow ll";
string dyDir="data (ll)";
string gmcDirs[] = { "Z+#gamma#rightarrow#nu#nu#gamma",
		     "W#rightarrow l#nu",
		     "QCD",
		     "#gamma+jets" };
const size_t nGmcDirs=sizeof(gmcDirs)/sizeof(string);
string gDir = "data (#gamma)";


TObject* getObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false);
void show(std::vector<TH1F *> &plots,TString outName);

//
TObject* getObjectFromPath(TDirectory* File, std::string Path, bool GetACopy)
{
   TObject* toReturn = NULL;
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
   else if(GetACopy){ toReturn = toReturn->Clone();  }
   return toReturn;
}

//
void getGammaWeights(TString inputFile="plotter.root",TString varname="qt")
{

  //get plots from file for each category
  TFile *fin = TFile::Open(inputFile);
  std::vector<TH1F *> dataWgts,     mcWgts;
  std::vector<TH1F *> mcMassShapes, dataMassShapes;
  for(size_t id=0; id<nDilCats; id++)
    {
      for(size_t ic=0; ic<nCats; ic++)
	{
	  //mass shape
	  string pName=dilCats[id]+"_zmass_"+cats[ic];
	  string newPname=dilCats[id]+"_datazmass_"+cats[ic];
	  TH1F *hdymass=(TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);      hdymass->SetName(newPname.c_str());   hdymass->SetDirectory(0);   dataMassShapes.push_back( hdymass );
	  newPname=dilCats[id]+"_mczmass_"+cats[ic];
	  TH1F * hmcdymass=(TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true); hmcdymass->SetName(newPname.c_str()); hmcdymass->SetDirectory(0); mcMassShapes.push_back( hmcdymass );
	  
	  //variable
	  pName=dilCats[id]+"_"; pName+=varname.Data(); pName+="_"+cats[ic];
	  TH1F *hdy   = (TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);
	  TH1F *hmcdy = (TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true);
	  TH1F *hg    = (TH1F *) getObjectFromPath(fin,gDir+"/"+pName,true);
	  TH1F *hmcg  = 0;
	  for(size_t imc=0; imc<nGmcDirs; imc++)
	    {
	      if(hmcg==0) hmcg =     (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,true);
	      else        hmcg->Add( (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+pName,false) ); 
	    }
	  TH1F *hMCwgts   = (TH1F *) hmcdy->Clone((pName+"_mcwgts").c_str()); hMCwgts->SetDirectory(0);   hMCwgts->Divide(hmcg);    mcWgts.push_back(hMCwgts);
	  TH1F *hDataWgts = (TH1F *) hdy->Clone((pName+"_datawgts").c_str()); hDataWgts->SetDirectory(0); hDataWgts->Divide(hg);    dataWgts.push_back(hDataWgts);
	}
    }
  fin->Close();
  
  //show weights
  setStyle();
  show(dataWgts,"DataGammaWeights");
  show(mcWgts,"MCGammaWeights");
  show(dataMassShapes,"DataMassShapes");
  show(mcMassShapes,"MCMassShapes");

  //save histograms to file
  TString foutName="gamma"+varname+"weights.root";
  TFile *fout = TFile::Open(foutName,"RECREATE");   fout->cd();
  for(size_t ip=0; ip<dataWgts.size(); ip++)        dataWgts[ip]->Write();
  for(size_t ip=0; ip<mcWgts.size(); ip++)          mcWgts[ip]->Write();
  for(size_t ip=0; ip<dataMassShapes.size(); ip++)  dataMassShapes[ip]->Write();
  for(size_t ip=0; ip<mcMassShapes.size(); ip++)    mcMassShapes[ip]->Write();
  fout->Close();
  
  cout << "*** Gamma pT weights (inclusive/differential) available @ " << foutName << endl;
}

//
void show(std::vector<TH1F *> &plots,TString outName)
{
  TCanvas *cnv = getNewCanvas("c","c",false);
  cnv->Clear();
  cnv->SetWindowSize(nDilCats*400,400);
  cnv->Divide(nDilCats,1);
  Int_t colors[]={kBlue,1,kRed,kGreen,kOrange};
  for(size_t ip=0; ip<plots.size(); ip++)
    {
      cnv->cd(ip/nCats+1);
      plots[ip]->SetLineColor(colors[ip%nCats]);
      plots[ip]->Draw(ip%nCats==0 ? "hist" : "histsame");
      plots[ip]->SetTitle(catLabels[ip%nCats]);
    }
  for(size_t id=0; id<nDilCats; id++)
    {
      cnv->cd(id+1);
      TPaveText *pave = new TPaveText(0.5,0.75,0.85,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText(dilCatLabels[id])->SetTextFont(42);
      pave->Draw();
    }
  cnv->SaveAs(outName+".png");
}



