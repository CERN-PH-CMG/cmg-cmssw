//
// .L getGammaWeights.C+
// getGammaWeights("plotter.root");
//

#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TGraphErrors.h"


string cats[]      ={"eq0jets", "eq1jets", "eq2jets",  "geq3jets","vbf"};
TString catLabels[]={"=0 jets", "=1 jets", "=2 jets", "#geq 3 jets","VBF"};
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

//init 
void resetQtFitFunc()
{
  qtFitFunc->SetParameter(0,1e-5); 
  qtFitFunc->SetParameter(1,-3.07791e+03);
  qtFitFunc->SetParameter(2,1.01404e+03);
  qtFitFunc->SetParameter(3,-3.69992e-01);
}

//
void getGammaWeights(string inputFile="plotter.root",string varName="qt")
{
  //get plots from file for each category
  TFile *fin = TFile::Open(inputFile.c_str());
  cout << fin << endl;
  std::vector<TH1F *> dataWgts,     mcWgts;
  std::vector<TH1F *> dataVtxWgts,     mcVtxWgts;
  std::vector<TH1F *> mcMassShapes, dataMassShapes;
  for(size_t id=0; id<nDilCats; id++)
    {
      for(size_t ic=0; ic<nCats; ic++)
	{

	  if(ic==0)
	    {
	      //mass shape
	      string pName=dilCats[id]+"_zmass_"+cats[ic];
	      string newPname=dilCats[id]+"_datazmass";
	      cout << pName << endl;
	      TH1F *hdymass=(TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);      hdymass->SetName(newPname.c_str());   hdymass->SetDirectory(0);   dataMassShapes.push_back( hdymass );
	      newPname=dilCats[id]+"_mczmass";
	      TH1F * hmcdymass=(TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true); hmcdymass->SetName(newPname.c_str()); hmcdymass->SetDirectory(0); mcMassShapes.push_back( hmcdymass );
	    }
	  
	  //reset parameterization for boson pt
	  resetQtFitFunc();
	  
	  //get the histos (veto 0 soft jets for g+jets)
	  string pName=dilCats[id]+"_"+varName+"_"+cats[ic];
	  cout << pName << endl;
	  TH1F *hdy   = (TH1F *) getObjectFromPath(fin,dyDir+"/"+pName,true);
	  TH1F *hmcdy = (TH1F *) getObjectFromPath(fin,dymcDir+"/"+pName,true); 
	  
	  string gPName(pName);
	  //	  if(cats[ic]=="eq0jets") { gPName=dilCats[id]+"_qt_"+"eq0hardjets"; }
	  TH1F *hg    = (TH1F *) getObjectFromPath(fin,gDir+"/"+gPName,true);
	  TH1F *hmcg  = 0;
	  for(size_t imc=0; imc<nGmcDirs; imc++)
	    {
	      if(hmcg==0) hmcg =     (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+gPName,true);
	      else        hmcg->Add( (TH1F *) getObjectFromPath(fin, gmcDirs[imc]+"/"+gPName,false) ); 
	    }

	  //reweight to ZpT observed *in data*
	  TH1F *hMCwgts   = (TH1F *) hdy->Clone((pName+"_mcwgts").c_str()); hMCwgts->SetDirectory(0);   hMCwgts->Divide(hmcg);    mcWgts.push_back(hMCwgts);
	  //	  TH1F *hMCwgts   = (TH1F *) hmcdy->Clone((pName+"_mcwgts").c_str()); hMCwgts->SetDirectory(0);   hMCwgts->Divide(hmcg);    mcWgts.push_back(hMCwgts);
	  TH1F *hDataWgts = (TH1F *) hdy->Clone((pName+"_datawgts").c_str()); hDataWgts->SetDirectory(0); hDataWgts->Divide(hg);    dataWgts.push_back(hDataWgts);
	      
	  /*
	  hdy->Fit(qtFitFunc,"MER+");
	  TH1F *hMCwgts   = (TH1F *) hmcdy->Clone((pName+"_mcwgts").c_str()); 
	  hMCwgts->SetDirectory(0);  
	  hMCwgts->Reset("ICE");
	  for(int ibin=1; ibin<=hMCwgts->GetXaxis()->GetNbins(); ibin++) hMCwgts->SetBinContent(ibin,qtFitFunc->Eval(hMCwgts->GetBinLowEdge(ibin)));
	  hMCwgts->Divide(hmcg);
  
	  hmcdy->Fit(qtFitFunc,"MER+");
	  TH1F *hDataWgts = (TH1F *) hdy->Clone((pName+"_datawgts").c_str()); 
	  hDataWgts->SetDirectory(0); 
	  hDataWgts->Reset("ICE");
	  for(int ibin=1; ibin<=hDataWgts->GetXaxis()->GetNbins(); ibin++) hDataWgts->SetBinContent(ibin,qtFitFunc->Eval(hDataWgts->GetBinLowEdge(ibin)));

	  mcWgts.push_back(hMCwgts);
	  dataWgts.push_back(hDataWgts);
  */
	}
    }
  fin->Close();
  
  //save histograms to file
  TString foutName="gamma"+varName+"weights.root";
  TFile *fout = TFile::Open(foutName,"RECREATE");   fout->cd();
  for(size_t ip=0; ip<dataWgts.size(); ip++)        dataWgts[ip]->Write();
  for(size_t ip=0; ip<mcWgts.size(); ip++)          mcWgts[ip]->Write();
  for(size_t ip=0; ip<dataMassShapes.size(); ip++)  dataMassShapes[ip]->Write();
  for(size_t ip=0; ip<mcMassShapes.size(); ip++)    mcMassShapes[ip]->Write();
  fout->Close();
  
  cout << "*** Gamma pT weights (inclusive/differential) available @ " << foutName << endl;
}
