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
  TString cats[]   =  {"eq0jets","eq1jets","eq2jets","geq3jets","vbf","geq1jets","mjjq100","mjjq092","mjjq083","mjjq066","mjjq049","mjjq033","mjjq016"};
  TString dilCats[] = {"ee","mumu","ll"};
  for(size_t ifile=0; ifile<gammaPtWeightsFiles.size(); ifile++)
    {
      TString gammaPtWeightsFile(gammaPtWeightsFiles[ifile].c_str());
      gSystem->ExpandPathName(gammaPtWeightsFile);
      TFile *fwgt=TFile::Open(gammaPtWeightsFile);
      if(fwgt)
	{
	  cout << "[GammaEventHandler] retrieving weights from: " << gammaPtWeightsFile << endl;
	  TString wgtName("qt");
	  TString wgtType( isMC_ ? "mcfitwgts" : "datafitwgts");
	  if (gammaPtWeightsFile.Contains("nvtx")) 
	    {
	      wgtName="nvtx";
	      wgtType=( isMC_ ? "mcwgts" : "datawgts");
	    }
	    
	  //event weights
	  std::map<TString, TGraph*> iWgtsH;
	  //std::map<TString, TH1*> iWgtsH;
	  for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	    {
	      for(size_t id=0; id<sizeof(dilCats)/sizeof(TString); id++)
		{
		  TString key = dilCats[id] + cats[ic] + "_" + wgtName + "_" + wgtType;
		  //TH1 *h = (TH1 *) fwgt->Get(key);
		  TGraph *h = (TGraph *) fwgt->Get(key);
		  if(h!=0)
		    {
		      //h->SetDirectory(0);
		      key = dilCats[id] + cats[ic];
		      iWgtsH[key] = h;
		    }
		  
		  //mass shape (take it from the first file with weights)
		  key = dilCats[id] + (isMC_ ? "" : cats[ic]);
		  if(zmassH_.find(key)!=zmassH_.end()) continue;
		      
		  TString hname= key+"_zmass"; 
		  //TString hname= key+ (isMC_ ? "_mczmass" : "_zmass"); 
		  //		  TString hname= key+ (isMC_ ? "_mczmass" : "_datazmass"); 
		  TH1 *massh = (TH1 *) fwgt->Get(hname);
		  if(massh!=0)
		    {
		      massh->SetDirectory(0);
		      zmassH_[key]= massh;
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
  if(phys.gammas.size()==0) return isGoodEvent_;
  if( phys.cat<22) return isGoodEvent_;

  float pt = phys.gammas[0].pt();
  if(!is2011)
    {
      if(pt<39)
	{
	  if( !(phys.gammaTriggerWord & 0x1) ) return isGoodEvent_;
	  triggerThr_=22; 
	  triggerWgt_=phys.gammaPrescale[0];
	}
      else if(pt>=39  && pt<55)
	{
	  if( !( (phys.gammaTriggerWord>>1) & 0x1) ) return isGoodEvent_;
	  triggerThr_=36; 
	  triggerWgt_=phys.gammaPrescale[1];
	}
      else if(pt>=55  && pt<82)
	{
	  if( !( (phys.gammaTriggerWord>>2) & 0x1) ) return isGoodEvent_;
	  triggerThr_=50; 
	  triggerWgt_=phys.gammaPrescale[2];
	}
      else if(pt>=82  && pt<100)
	{
	  if( !( (phys.gammaTriggerWord>>3) & 0x1) ) return isGoodEvent_;
	  triggerThr_=75; 
	  triggerWgt_=phys.gammaPrescale[3];
	}
      else if(pt>=100)
	{
	  if( !( (phys.gammaTriggerWord>>4) & 0x1) ) return isGoodEvent_;
	  triggerThr_=90; 
	  triggerWgt_=phys.gammaPrescale[4];
	}
    }
  else {   triggerThr_ =( phys.cat-22)/1000; triggerWgt_=1;}
  
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
      for(std::map<TString, std::map<TString,TGraph *> >::iterator wIt = wgtsH_.begin(); wIt != wgtsH_.end(); wIt++)
	{
	  if(wIt->second.find(key) == wIt->second.end()) { /*cout << key << " not found" << endl;*/ continue; }
	  TGraph *h = wIt->second[key];
	  weight=h->Eval(gamma.pt());
	  //if(gamma.pt()>900)                        weight=h->Eval(900);
	  if(weight<0) weight=0;
	}
      evWeights_[dilCats[id]]=weight;
    }
  
  return evWeights_;
}

//
GammaEventHandler::~GammaEventHandler()
{
}
