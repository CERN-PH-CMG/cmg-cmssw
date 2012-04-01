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
 
 //open file and retrieve weights
  TString gammaPtWeightsFile =  runProcess.getParameter<std::string>("weightsFile"); 
  gSystem->ExpandPathName(gammaPtWeightsFile);
  if(!isMC) gammaPtWeightsFile=gammaPtWeightsFile.ReplaceAll("mc_","data_");

  fwgt_=TFile::Open(gammaPtWeightsFile);
  if(fwgt_)
    {

      weightMode_=PT;
      if(gammaPtWeightsFile.Contains("eta"))        weightMode_=PTANDETA;
      else if(gammaPtWeightsFile.Contains("nvtx"))  weightMode_=PTANDNVTX;
      
      TString wgtName = gammaPtWeightsFile;
      wgtName=gSystem->BaseName(wgtName.ReplaceAll(".root",""));
      wgtName=((TObjString *)(wgtName.Tokenize("_")->At(1)))->GetString();
      wgtName=wgtName.ReplaceAll("gamma",""); 
      TString categories[]={"eq0jets","eq1jets","geq2jets","vbf"};
      TString dilCategories[]={"ee","mumu","ll"};
    
      //event weights
      for(size_t icat=0; icat<sizeof(categories)/sizeof(TString); icat++)
	{
	  for(size_t idilcat=0; idilcat<sizeof(dilCategories)/sizeof(TString); idilcat++)
	    {
	      TString key=categories[icat]+"_"+dilCategories[idilcat];
	      TH1 *h=(TH1 *)fwgt_->Get(key+wgtName);
	      if(h==0) continue;
	      wgtsH_[key] = h;
	      wgtsH_[key]->SetDirectory(0);
	    }
	}

      //mass shapes
      for(size_t idilcat=0; idilcat<sizeof(dilCategories)/sizeof(TString); idilcat++)
	{
	  zmassH_[dilCategories[idilcat]]= (TH1 *) fwgt_->Get(dilCategories[idilcat]+"zmass");
	  zmassH_[dilCategories[idilcat]]->SetDirectory(0);
	}

      fwgt_->Close();
    }
  
  if(wgtsH_.size()) 
    std::cout << "[GammaEventHandler] gamma spectrum will be reweighted using distributions found @ "  << gammaPtWeightsFile 
	      << " weight mode is: " << weightMode_ << std::endl;
}


//
bool GammaEventHandler::isGood(PhysicsEvent_t &phys, TString evCategoryLabel)
{
  //reset
  isGoodEvent_=false;
  triggerThr_=0;
  photonCategory_="";
  massiveGamma_.clear();
  evWeights_.clear();
  
  //check if it is a gamma event
  if( phys.cat<22) return isGoodEvent_;
  triggerThr_ = (phys.cat-22)/1000;
  if(triggerThr_==0) return isGoodEvent_;

  //check which category this event belongs to (use the trigger)
  size_t eventTriggerCat(0);
  for(size_t icat=0; icat<gammaCats_.size()-1; icat++)
    {
      if(triggerThr_<gammaCats_[icat])    return isGoodEvent_;
      if(triggerThr_>=gammaCats_[icat+1]) eventTriggerCat=icat+1; 
      else                                eventTriggerCat=icat;   
      break;
    }

  //require one gamma only in the event within the trigger which has fired
  if( phys.gammas.size()==0 ) return isGoodEvent_;
  LorentzVector gamma=phys.gammas[0];
  if(gamma.pt()<triggerThr_) return isGoodEvent_;

  
  //the photon category
  photonCategory_="photon";  photonCategory_ += triggerThr_; 
  
  //generate a massive gamma and retrieve the weights for each dilepton channel
  TString dilCategories[]={"ee","mumu","ll"};
  for(size_t idilcat=0; idilcat<sizeof(dilCategories)/sizeof(TString); idilcat++)
    {
      float mass(0);
      if(zmassH_.find(dilCategories[idilcat])!=zmassH_.end())
	{
	  if(zmassH_[dilCategories[idilcat]]->Integral())
	    while(fabs(mass-91)>15) 
	      mass = zmassH_[dilCategories[idilcat]]->GetRandom();
	}
      massiveGamma_[dilCategories[idilcat]]=LorentzVector(gamma.px(),gamma.py(),gamma.pz(),sqrt(pow(mass,2)+pow(gamma.energy(),2)));
      
      float weight(gammaTriggerRenWeights_[eventTriggerCat]);
      evWeights_[dilCategories[idilcat]]=weight;
      TString wgtKey=dilCategories[idilcat]; if(evCategoryLabel!="") wgtKey=evCategoryLabel+"_"+wgtKey;
      if( wgtsH_.find(wgtKey) == wgtsH_.end())  continue;
      
      //take the last bin weight if pT>max available
      TH1 *theH = wgtsH_[wgtKey];
      for(int ibin=1; ibin<=theH->GetXaxis()->GetNbins(); ibin++)
	{
	  if(gamma.pt()<theH->GetXaxis()->GetBinLowEdge(ibin) ) break; 
	  if(weightMode_==PT)              weight = theH->GetBinContent(ibin);
	  else if(weightMode_==PTANDETA)   weight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(fabs(gamma.eta())));
	  else if(weightMode_==PTANDNVTX)  weight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(phys.nvtx));
	}
      evWeights_[dilCategories[idilcat]]=weight;
    }
  
  //all done here
  isGoodEvent_=true;
  return isGoodEvent_;
}

//
GammaEventHandler::~GammaEventHandler()
{
}
