#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"

using namespace std;

//
GammaEventHandler::GammaEventHandler(const edm::ParameterSet &runProcess)
  : isGoodEvent_(false)
{
  //trigger thresholds to consider
  bool isMC = runProcess.getParameter<bool>("isMC");
 
  //open file and retrieve weights + mass shapes
  std::vector<std::string> gammaPtWeightsFiles =  runProcess.getParameter<std::vector<std::string> >("weightsFile");  
  TString cats[]   =  {"eq0jets","eq1jets","eq2jets","geq3jets","vbf"};
  TString dilCats[] = {"ee","mumu","ll"};
  TString wgtType( isMC ? "mcwgts" : "datawgts");
  for(size_t ifile=0; ifile<gammaPtWeightsFiles.size(); ifile++)
    {
      TString gammaPtWeightsFile(gammaPtWeightsFiles[ifile].c_str());
      gSystem->ExpandPathName(gammaPtWeightsFile);
      TFile *fwgt=TFile::Open(gammaPtWeightsFile);
      if(fwgt)
	{
	  TString wgtName("qt");
	  if (gammaPtWeightsFile.Contains("nvtx"))  wgtName="nvtx";
	  
	  //event weights
	  std::map<TString, TH1*> iWgtsH;
	  for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	    {
	      for(size_t id=0; id<sizeof(dilCats)/sizeof(TString); id++)
		{
		  TString key = dilCats[id] + cats[ic] + "_" + wgtName + "_" + wgtType;
		  TH1 *h = (TH1 *) fwgt->Get(key);
		  if(h!=0)
		    {
		      h->SetDirectory(0);
		      key = dilCats[id] + cats[ic];
		      iWgtsH[key] = h;
		    }
		  
		  //mass shape (take it from the first file with weights)
		  if(ic==0)
		    {
		      key = dilCats[id]; 
		      if(zmassH_.find(key)!=zmassH_.end()) continue;
		      
		      TString hname= key+ (isMC ? "_mczmass" : "_datazmass"); 
		      h = (TH1 *) fwgt->Get(hname);
		      if(h!=0)
			{
			  h->SetDirectory(0);
			  zmassH_[key]= h;
			}
		    }
		}
	    }
	  fwgt->Close();
	  wgtsH_[wgtName]=iWgtsH;
	}
    }
  
  if(wgtsH_.size()) std::cout << "[GammaEventHandler] gamma spectrum will be reweighted using distributions found in "  << gammaPtWeightsFiles.size() << " files" << std::endl;
}


//
bool GammaEventHandler::isGood(PhysicsEvent_t &phys)
{
  //reset
  isGoodEvent_=false;
  massiveGamma_.clear();
  evWeights_.clear();

  //check if it is a gamma event
  if( phys.cat<22) return isGoodEvent_;

  //all done here
  isGoodEvent_=true;
  return isGoodEvent_;
}

//
std::map<TString,float> GammaEventHandler::getWeights(PhysicsEvent_t &phys, TString evCategoryLabel)
{
  //loop over categories
  LorentzVector gamma=phys.gammas[0];
  TString dilCats[]={"ee","mumu","ll"};
  for(size_t id=0; id<sizeof(dilCats)/sizeof(TString); id++)
    {
      //the key to search for
      TString key = dilCats[id];
      
      //generate a massive gamma (0 if in non-weighting mode)
      float mass(0);
      if(zmassH_.find(key)!=zmassH_.end())
	{
	  if(zmassH_[key]->Integral())
	    while(fabs(mass-91)>15) 
	      mass = zmassH_[key]->GetRandom();
	}
      massiveGamma_[dilCats[id]]=LorentzVector(gamma.px(),gamma.py(),gamma.pz(),sqrt(pow(mass,2)+pow(gamma.energy(),2)));

      //get event weight (will be 0 by default if we're running in weighting mode)
      key += evCategoryLabel;
      float weight(1.0);//wgtsH_.size() ? 0.0 : 1.0);
      for(std::map<TString, std::map<TString,TH1 *> >::iterator wIt = wgtsH_.begin(); wIt != wgtsH_.end(); wIt++)
	{
	  if(wIt->second.find(key) == wIt->second.end()) continue; //{ cout << key; continue; }
	  TH1 *h = wIt->second[key];

	  float iweight(1.0);
	  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
	    {
	      if(wIt->first=="qt")
		{
		  if(gamma.pt()<h->GetXaxis()->GetBinLowEdge(ibin) ) break; 
		  iweight = h->GetBinContent(ibin);
		}
	      else if(wIt->first=="nvtx")
		{
		  if(phys.nvtx<h->GetXaxis()->GetBinLowEdge(ibin)) break;
		  iweight = h->GetBinContent(ibin); 
		}
	    }
	  //	  cout << wIt->first << " " << iweight << "\t";
	  weight *= iweight;
	}
      //      cout << endl;
      evWeights_[dilCats[id]]=weight;
    }
  
  return evWeights_;
}

//
GammaEventHandler::~GammaEventHandler()
{
}
