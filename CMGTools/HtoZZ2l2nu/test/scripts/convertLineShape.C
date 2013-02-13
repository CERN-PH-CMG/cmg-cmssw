#include <map>
#include <iostream>
#include <fstream>
#include "TString.h"
#include "TGraph.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TSystem.h"

void convertLineShape()
{
  TFile *outF=TFile::Open("LineShapeWeights.root","RECREATE");

  TString baseUrl="http://tier2.ihepa.ufl.edu/~tcheng/LineShape_Lineshape_runningWidth/";
  TString files[]={
    "mZZ_Higgs400_7TeV_Lineshape+Interference.txt",
    "mZZ_Higgs400_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs425_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs450_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs450_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs475_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs500_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs500_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs550_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs550_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs575_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs600_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs600_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs650_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs650_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs700_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs700_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs750_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs750_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs800_7TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs800_8TeV_Lineshape+Interference.txt", 
    "mZZ_Higgs850_7TeV_Lineshape+Interference.txt",
    "mZZ_Higgs850_8TeV_Lineshape+Interference.txt",
    "mZZ_Higgs900_7TeV_Lineshape+Interference.txt",
    "mZZ_Higgs900_8TeV_Lineshape+Interference.txt",
    "mZZ_Higgs950_7TeV_Lineshape+Interference.txt",
    "mZZ_Higgs950_8TeV_Lineshape+Interference.txt",
    "mZZ_Higgs1000_8TeV_Lineshape+Interference.txt"
  };
  
  
  for(size_t i=0; i<sizeof(files)/sizeof(TString); i++)
    {
      TObjArray *tkns=files[i].Tokenize("_");
      TString key(tkns->At(1)->GetName());
      key += "_";
      key += tkns->At(2)->GetName();
      
      gSystem->Exec("curl -O " + baseUrl + files[i]);

      //parse file
      ifstream in(files[i].Data());
      std::map<float, std::vector<float> > dists;
      if(!in.bad())
	{   
	  while (!in.eof()) {
	    float mass, origShape, rwgtShape, rwgtShapePlus, rwgtShapeMinus, 
	      rwgtPlusInterf, rwgtPlusInterfPlus, rwgtPlusInterfMinus;
	    in >> mass;
	    in >> origShape;
	    in >> rwgtShape;
	    in >> rwgtShapePlus;
	    in >> rwgtShapeMinus;
	    in >> rwgtPlusInterf;
	    in >> rwgtPlusInterfPlus;
	    in >> rwgtPlusInterfMinus;
	    if(origShape==0) continue;
	    std::vector<float> ipt;
	    ipt.push_back(origShape);
	    ipt.push_back(rwgtShape);
	    ipt.push_back(rwgtShapePlus);
	    ipt.push_back(rwgtShapeMinus);
	    ipt.push_back(rwgtPlusInterf);
	    ipt.push_back(rwgtPlusInterfPlus);
	    ipt.push_back(rwgtPlusInterfMinus);
	    dists[mass]=ipt;
	  }
	  in.close();
	}
      if(dists.size()==0) continue;
      std::vector<TGraph *> graphs;
      std::vector<TGraph *> shapeGraphs;
      for(std::map<float, std::vector<float> >::iterator it =dists.begin();
	  it!= dists.end(); 
	  it++)
	{
	  for(size_t p=0; p<it->second.size(); p++)
	    {
	      if(graphs.size()<=p) 
		{
		  TGraph *gr=new TGraph;
		  TString grname="nominal";
		  if(p==1) grname="cps";
		  if(p==2) grname="cps_up";
		  if(p==3) grname="cps_down";
		  if(p==4) grname="int";
		  if(p==5) grname="int_up";
		  if(p==6) grname="int_down";
		  gr->SetName(grname);
		  graphs.push_back(gr);

		  gr=new TGraph;
		  gr->SetName(grname+"_shape");
		  shapeGraphs.push_back(gr);
		}

	      int refGraph=0;
	      if(p>=4) refGraph==1;

	      if( (it->second)[0]==0)
		{
		  graphs[p]->SetPoint(graphs[p]->GetN(),it->first,0);
		  shapeGraphs[p]->SetPoint(shapeGraphs[p]->GetN(),it->first,0);
		}
	      else
		{
		  graphs[p]->SetPoint(graphs[p]->GetN(),it->first,(it->second)[p]/(it->second)[refGraph]);
		  shapeGraphs[p]->SetPoint(shapeGraphs[p]->GetN(),it->first,(it->second)[p]);
		}
	    }
	}
      
      TDirectory*dir=outF->mkdir(key);
      dir->cd();
      for(size_t p=0; p<graphs.size(); p++)
	{
	  graphs[p]->SetMarkerStyle(20+p);
	  graphs[p]->SetFillStyle(0);
	  graphs[p]->Write();

	  shapeGraphs[p]->SetMarkerStyle(20+p);
	  shapeGraphs[p]->SetFillStyle(0);
	  shapeGraphs[p]->Write();
	}

      gSystem->Exec("rm " + files[i]);
    }

  outF->Close();
}
