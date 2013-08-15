#include <iostream>
#include <vector>
#include <fstream>

#include "TString.h"
#include "TGraph.h"
#include "TFile.h"
#include "TSystem.h"

void getHiggsQtWeights(){

  Int_t masses[]={130,140,150,160,180,190,200,250,300,350,400,450,500,550,600};
  const size_t nMasses=sizeof(masses)/sizeof(Int_t);  
  std::vector<TGraph *> wgtGrVec;
  
  //loop over masses and variationsr
  for(size_t imass=0; imass<nMasses; imass++)
    {
      for(size_t ivar=0; ivar<5; ivar++)
	{
	  Int_t mass=masses[imass];
	  Int_t ren=masses[imass];
	  Int_t fac=masses[imass];
	  TString dir(ivar==0 ? "Std" : "ScaleVariations" );
	  if(ivar==1)  ren=mass*2;
	  if(ivar==2)  ren=mass/2;
	  if(ivar==3)  fac=mass*2;
	  if(ivar==4)  fac=mass/2;
	  char buf[100];
	  sprintf(buf,"kfactors_mh%d_ren%d_fac%d.dat",mass,ren,fac);
	  TString fileName(buf);

	  //fecth from CVS
	  gSystem->Exec("wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/HiggsToWW2Leptons/data/kfactors_"+dir+"/" + fileName + "?view=co -O " + fileName );

	  std::vector<Double_t> x, wgt;  
	  
	  //parse file
	  ifstream in(fileName.Data());
	  bool dataFound=false;
	  float xlast(0),wlast(0);
	  if(!in.bad())
	    {   
	      while (!in.eof()) {
		TString line;
		in >> line;
		if(line.Contains("-------")) { dataFound=true; continue; }
		if(!dataFound) continue;
		
		x.push_back(line.Atof());
		
		in >> line;
		xlast=line.Atof();
		
		in >> line;
		wlast=line.Atof();
		wgt.push_back(wlast);
		
	      }
	      x.push_back(xlast);
	      wgt.push_back(wlast);
	      in.close();
	    }
	  

	  //create weight histogram
	  TString hname(fileName); hname.ReplaceAll(".dat","");
	  TGraph *wgtGr = new TGraph; 
	  wgtGr->SetName(hname);
	  wgtGr->SetTitle(hname);
	  for(size_t i=0; i<x.size(); i++) wgtGr->SetPoint(i,x[i],wgt[i]);
	  
	  wgtGrVec.push_back(wgtGr);

	  gSystem->Exec("rm " + fileName); 
	}
    }


  //save all to file
  TFile *fout = TFile::Open("HiggsQtWeights.root","RECREATE");
  fout->cd();
  for(std::vector<TGraph *>::iterator it = wgtGrVec.begin(); it!=wgtGrVec.end(); it++)   (*it)->Write();
  fout->Close();
}
