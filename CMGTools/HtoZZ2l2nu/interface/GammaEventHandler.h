/* 
 * Wrapper for common operations on a gamma event
 * Get weights/mass shapes from file
 * Analyze event and assign trigger categories, weights and massive candidates
 * $Date: 2011/11/24 11:22:57 $
 * $Revision: 1.3 $
 * \author Pedro Silva
 */

#ifndef GammaEventHandler_H
#define GammaEventHandler_H

#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

class GammaEventHandler
{
 public: 

  enum WeightMode {NOWEIGHTS, PT, PTANDETA, PTANDNVTX};
  
  GammaEventHandler(const edm::ParameterSet &runProcess);

  //to be called event by event
  bool isGood(PhysicsEvent_t &phys, TString evCategoryLabel="");

  //getters
  bool isGood() { return isGoodEvent_; }
  int triggerThr() { return triggerThr_; }
  TString eventCategory() { return photonCategory_; }
  LorentzVector massiveGamma(TString channel) { return massiveGamma_[channel]; }
  std::map<TString,LorentzVector> getMassiveGamma() { return massiveGamma_; }
  float getWeight(TString channel) { return evWeights_[channel]; }
  std::map<TString,float> getWeights() { return evWeights_; }
  size_t nCategories() { return gammaCats_.size(); }
  int category(size_t icat) {  return (gammaCats_.size() > icat ? gammaCats_[icat] : 0 ); }
  std::vector<int> categories() { return gammaCats_; }
  int weightMode() { return weightMode_; }

  void printSummary()
  {
    //std::cout << "Event is " << (isGoodEvent_ ? "ok" : "not ok") << std::endl;
    if(!isGoodEvent_) return;
    std::cout << "Trigger thr: " << triggerThr_ << " pT(gamma)=" << massiveGamma_.begin()->second.pt() << "(" << photonCategory_ << ")" << std::endl;
  }

  inline int findTriggerCategoryFor(float pt)
  {
    for(size_t itrigCat=0; itrigCat<nCategories()-1; itrigCat++)
      {
	if(pt<category(itrigCat)) return -1;
	if(pt>category(itrigCat+1)) return category(itrigCat+1);
	else return category(itrigCat);
      }
    return -1;
  }

  ~GammaEventHandler();
  
 private:

  int weightMode_;

  std::vector<int> gammaCats_;
  std::vector<double> gammaTriggerRenWeights_;

  TFile *fwgt_;
  std::map<TString,TH1 *> wgtsH_;
  std::map<TString, TH1 *> zmassH_;  

  bool isGoodEvent_;
  int triggerThr_;
  TString photonCategory_;
  std::map<TString,LorentzVector> massiveGamma_;
  std::map<TString,float> evWeights_;
};

#endif
