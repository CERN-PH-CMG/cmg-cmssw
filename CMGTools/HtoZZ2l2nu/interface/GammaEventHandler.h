/* 
 * Wrapper for common operations on a gamma event
 * Get weights/mass shapes from file
 * Analyze event and assign trigger categories, weights and massive candidates
 * $Date: 2012/04/03 11:41:18 $
 * $Revision: 1.5 $
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
  bool isGood(PhysicsEvent_t &phys);

  //getters
  bool isGood() { return isGoodEvent_; }
  int triggerThr() { return triggerThr_; }
  TString eventCategory() { return photonCategory_; }
  LorentzVector massiveGamma(TString channel) { return massiveGamma_[channel]; }
  std::map<TString,LorentzVector> getMassiveGamma() { return massiveGamma_; }
  float getWeight(TString channel) { return evWeights_[channel]; }
  float getTriggerPrescaleWeight() { return triggerPrescaleWeight_;}
  std::map<TString,float> getWeights(PhysicsEvent_t &phys, TString evCategoryLabel="");
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
      if(pt>category(itrigCat) && pt<category(itrigCat+1)) 
	return itrigCat;
    
    if(pt>category(nCategories()-1)) return nCategories()-1;
    return -1;
  }
  
  ~GammaEventHandler();


  //these can be accessed after call isGood(physics)
  bool isGoodEvent_;
  int triggerThr_;
  TString photonCategory_;
  std::map<TString,LorentzVector> massiveGamma_;
  std::map<TString,float> evWeights_;
  float triggerPrescaleWeight_;
  
 private:
  
  int weightMode_;
  std::vector<int> gammaCats_;
  std::vector<double> gammaTriggerRenWeights_;

  TFile *fwgt_;
  std::map<TString,TH1 *> wgtsH_;
  std::map<TString, TH1 *> zmassH_;  
};

#endif
