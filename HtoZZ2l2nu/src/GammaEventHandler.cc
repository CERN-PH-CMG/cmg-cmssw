#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"

using namespace std;

//
GammaEventHandler::GammaEventHandler(const edm::ParameterSet &runProcess)
  : isGoodEvent_(false)
{
  //trigger thresholds to consider
  isMC_ = runProcess.getParameter<bool>("isMC");
 
  //open file and retrieve weights + mass shapes
  std::vector<std::string> gammaPtWeightsFiles =  runProcess.getParameter<std::vector<std::string> >("weightsFile");  
  TString cats[]   =  {"eq0jets","eq1jets","eq2jets","geq3jets","vbf","geq1jets"};
  TString dilCats[] = {"ee","mumu","ll"};
  TString wgtType( isMC_ ? "mcwgts" : "datawgts");
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
		  key = dilCats[id] + (isMC_ ? "" : cats[ic]);
		  if(zmassH_.find(key)!=zmassH_.end()) continue;
		      
		  TString hname= key+ (isMC_ ? "_mczmass" : "_datazmass"); 
		  h = (TH1 *) fwgt->Get(hname);
		  if(h!=0)
		    {
		      h->SetDirectory(0);
		      zmassH_[key]= h;
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
bool GammaEventHandler::isGood(PhysicsEvent_t &phys, bool is2011)
{
  //reset
  isGoodEvent_=false;
  massiveGamma_.clear();
  evWeights_.clear();
  triggerWgt_=0;

  //check if it is a gamma event
  if( phys.cat<22) return isGoodEvent_;
  triggerThr_ =( phys.cat-22)/1000;

  if(is2011) triggerWgt_=1;
  else
    { 
      if(triggerThr_>90) return isGoodEvent_;
      if(triggerThr_==22)
	{
	  if(phys.run<195398) triggerWgt_=50.28;
	  if(phys.run<197774) triggerWgt_=194.74;
	  if(phys.run<198913) triggerWgt_=207.38;
	  if(phys.run<200519) triggerWgt_=209.99;
	  else                triggerWgt_=211.76;
	}
      else if(triggerThr_==36)
	{
	  if(phys.run<195398) triggerWgt_=7.97;
	  if(phys.run<197774) triggerWgt_=64.91;
	  if(phys.run<198913) triggerWgt_=69.13;
	  if(phys.run<200519) triggerWgt_=70.00;
	  else                triggerWgt_=70.59;
	}
      else if(triggerThr_==50)
	{
	  if(phys.run<195398) triggerWgt_=2.74;
	  if(phys.run<197774) triggerWgt_=30.00;
	  if(phys.run<198913) triggerWgt_=30.00;
	  if(phys.run<200519) triggerWgt_=30.00;
	  else                triggerWgt_=30.00;
	}
      else if(triggerThr_==75)
	{
	  if(phys.run<195398) triggerWgt_=2.47;
	  if(phys.run<197774) triggerWgt_=10.00;
	  if(phys.run<198913) triggerWgt_=10.00;
	  if(phys.run<200519) triggerWgt_=10.00;
	  else                triggerWgt_=10.00;
	}
      else if(triggerThr_==90)
       	{
       	  if(phys.run<195398) triggerWgt_=2.14;
       	  if(phys.run<197774) triggerWgt_=5.00;
       	  if(phys.run<198913) triggerWgt_=5.00;
       	  if(phys.run<200519) triggerWgt_=5.00;
       	  else                triggerWgt_=5.00;
       	}
      //fixme once new ntuples with correct triggers are built
      else if(triggerThr_>=250) triggerWgt_=1.0;
    }

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
      TString key = dilCats[id]+(isMC_?"":evCategoryLabel);
      
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
      key = dilCats[id]+evCategoryLabel;
      float weight(1.0);//wgtsH_.size() ? 0.0 : 1.0);
      for(std::map<TString, std::map<TString,TH1 *> >::iterator wIt = wgtsH_.begin(); wIt != wgtsH_.end(); wIt++)
	{
	  if(wIt->second.find(key) == wIt->second.end()) continue; // { cout << key; continue; }
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
