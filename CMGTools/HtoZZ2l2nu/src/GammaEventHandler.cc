#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"

using namespace std;

//
GammaEventHandler::GammaEventHandler(const edm::ParameterSet &runProcess)
  : weightMode_(NOWEIGHTS)
{
  //trigger thresholds to consider
  gammaCats_ = runProcess.getParameter<std::vector<int> >("gammaCategories"); 
  bool isMC = runProcess.getParameter<bool>("isMC");
  
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
bool GammaEventHandler::isGood(PhysicsEvent_t &phys)
{
  //reset
  isGoodEvent_=false;
  triggerThr_=0;
  photonCategory_="";
  massiveGamma_.clear();
  evWeights_.clear();
  
  //check if it is a gamma event and get the trigger threshold
  if( phys.cat<22) return isGoodEvent_;
  triggerThr_ = (phys.cat-22)/1000;
  if(triggerThr_==0) return isGoodEvent_;

  //require one gamma only in the event in the barrel with pT>20
  if( phys.gammas.size()==0 ) return isGoodEvent_;
  LorentzVector gamma=phys.gammas[0];
  if(gamma.pt()<20) return isGoodEvent_;
  //if(fabs(gamma.eta())>1.4442) return isGoodEvent_;

  //gamma can't be softer than the trigger 
  //fix-me use trigger matching next time
  for(size_t icat=0; icat<gammaCats_.size()-1; icat++)
    {
      if(gammaCats_[icat]<triggerThr_) continue;
      if(gamma.pt()<gammaCats_[icat] /*|| gamma.pt()>=gammaCats_[icat+1]*/) return isGoodEvent_;
      if(gamma.pt()>=gammaCats_[icat+1]) triggerThr_ = findTriggerCategoryFor(gamma.pt());
      else                               triggerThr_ = gammaCats_[icat];
      break;
    }
  
  photonCategory_="photon";  photonCategory_ += triggerThr_; 
  
  //generate a massive gamma and retrieve the weights for each dilepton channel
  TString dilCategories[]={"ee","mumu","ll"};
  int eventCategory       = eventClassifComp_.Get(phys);
  TString evCategoryLabel = eventClassifComp_.GetLabel(eventCategory);

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
      
      float weight(1.0);
      evWeights_[dilCategories[idilcat]]=weight;
      TString wgtKey=evCategoryLabel+"_"+dilCategories[idilcat];
      if( wgtsH_.find(wgtKey) == wgtsH_.end()) 
	{
	  //	  cout << "[Warning] can't find weight for " << wgtKey << endl;
	  continue;
	}
      
      //take the last bin weight if pT>max available
      TH1 *theH = wgtsH_[wgtKey];
      for(int ibin=1; ibin<=theH->GetXaxis()->GetNbins(); ibin++)
	{
	  if(gamma.pt()<theH->GetXaxis()->GetBinLowEdge(ibin) ) 
	    {
	      //cout << wgtKey << " " << ibin-1 << " " << gamma.pt() << " " << triggerThr_ << " " << weight << " "<< endl;
	      break;
	    }
	  if(weightMode_==PT)              weight = theH->GetBinContent(ibin);
	  else if(weightMode_==PTANDETA)   weight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(fabs(gamma.eta())));
	  else if(weightMode_==PTANDNVTX) weight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(phys.nvtx));
	}
      evWeights_[dilCategories[idilcat]]=weight;
    }
  
  //all done here
  isGoodEvent_=true;
  return isGoodEvent_;
}


//
int GammaEventHandler::findTriggerCategoryFor(float pt)
{
  int thr(0);
  //find the trigger - threshold category (assume 100% efficiency...) 
  if(pt>=gammaCats_[gammaCats_.size()-1]) thr=gammaCats_[gammaCats_.size()-1];
  else
    {
      size_t icat=0;
      for(; icat<gammaCats_.size()-1; icat++)	
	if(pt>=gammaCats_[icat] && pt<gammaCats_[icat+1]) break;
      thr=gammaCats_[icat];
    }
  return thr;
}

//
GammaEventHandler::~GammaEventHandler()
{
}
