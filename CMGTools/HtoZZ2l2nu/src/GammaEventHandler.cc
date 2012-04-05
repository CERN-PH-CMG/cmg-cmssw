#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"

using namespace std;

//
GammaEventHandler::GammaEventHandler(const edm::ParameterSet &runProcess)
  : weightMode_(NOWEIGHTS),
    isGoodEvent_(false)
{
  //trigger thresholds to consider
  bool isMC = runProcess.getParameter<bool>("isMC");
  gammaCats_ = runProcess.getParameter<std::vector<int> >("gammaCategories"); 
  if(!isMC) gammaTriggerRenWeights_ = runProcess.getParameter<std::vector<double> >("gammaTriggerRenWeights");
  else      gammaTriggerRenWeights_.resize(gammaCats_.size(),1.0);
 
  //open file and retrieve weights + mass shapes
  TString gammaPtWeightsFile =  runProcess.getParameter<std::string>("weightsFile");   gSystem->ExpandPathName(gammaPtWeightsFile);
  fwgt_=TFile::Open(gammaPtWeightsFile);
  if(fwgt_)
    {
      TString wgtName("");
      if(gammaPtWeightsFile.Contains("eta"))        { wgtName="qtvseta"; weightMode_=PTANDETA; }
      else if(gammaPtWeightsFile.Contains("nvtx"))  { wgtName="qtvsnvtx"; weightMode_=PTANDNVTX; }
      else                                          { wgtName="qt"; weightMode_=PT; }
      
      TString wgtType( isMC ? "mcwgts" : "datawgts");

      if(wgtName!="")
	{
	  TString cats[]   =  {"eq0jets","eq1jets","eq2jets","geq3jets"};
	  TString dilCats[] = {"ee","mumu","ll"};
	  for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	    {
	      for(size_t id=0; id<sizeof(dilCats)/sizeof(TString); id++)
		{
		  //event weights
		  TString key = dilCats[id] + "_" + wgtName + "_" + cats[ic] + "_" + wgtType;
		  TH1 *h = (TH1 *) fwgt_->Get(key);
		  if(h!=0)
		    {
		      key = dilCats[id] + "_" + cats[ic];
		      wgtsH_[key] = h;
		      wgtsH_[key]->SetDirectory(0);
		    }

		  //mass shape
		  key = dilCats[id]; key += (isMC ? "_mczmass_" : "_datazmass_"); key += cats[ic];
		  h = (TH1 *) fwgt_->Get(key);
		  if(h!=0)
		    {
		      key = dilCats[id] + "_" + cats[ic];
		      zmassH_[key]= h;
		      zmassH_[key]->SetDirectory(0);
		    }
		}
	    }
	}
      fwgt_->Close();
    }
  
  if(wgtsH_.size()) 
    std::cout << "[GammaEventHandler] gamma spectrum will be reweighted using distributions found @ "  << gammaPtWeightsFile << " weight mode is: " << weightMode_ << std::endl;
}


//
bool GammaEventHandler::isGood(PhysicsEvent_t &phys)
{
  //reset
  isGoodEvent_=false;
  triggerThr_=0;
  photonCategory_="";
  massiveGamma_.clear();
  evWeights_.clear();
  triggerPrescaleWeight_=1;

  //check if it is a gamma event
  if( phys.cat<22) return isGoodEvent_;
  triggerThr_ = (phys.cat-22)/1000;
  if(triggerThr_==0) return isGoodEvent_;

  //check which category this event belongs to (use the trigger)
  int eventTriggerCat(-1);
  for(size_t icat=0; icat<gammaCats_.size()-1; icat++)
    {
      if(triggerThr_<gammaCats_[icat])    break;
      eventTriggerCat=icat;   
    }
  if(eventTriggerCat<0) return isGoodEvent_;
  triggerPrescaleWeight_ = gammaTriggerRenWeights_[eventTriggerCat];

  photonCategory_="photon";  photonCategory_ += triggerThr_; 
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
      TString key = dilCats[id] + "_" + evCategoryLabel;
      
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
      float weight(wgtsH_.size() ? 0.0 : 1.0);
      if(wgtsH_.find(key) != wgtsH_.end()) 
	{
	  TH1 *theH = wgtsH_[key];
	  for(int ibin=1; ibin<=theH->GetXaxis()->GetNbins(); ibin++)
	    {
	      if(gamma.pt()<theH->GetXaxis()->GetBinLowEdge(ibin) ) break; 
	      if(weightMode_==PT)              weight = theH->GetBinContent(ibin);
	      else if(weightMode_==PTANDETA)   weight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(fabs(gamma.eta())));
	      else if(weightMode_==PTANDNVTX)
		{
		  for(int jbin=1; jbin<=theH->GetYaxis()->GetNbins(); jbin++)
		    {
		      if(phys.nvtx<theH->GetYaxis()->GetBinLowEdge(jbin)) break;
		      weight = theH->GetBinContent(ibin,jbin);
		    }
		}
	    }
	}
      evWeights_[dilCats[id]]=weight;
    }

  return evWeights_;
}

//
GammaEventHandler::~GammaEventHandler()
{
}
