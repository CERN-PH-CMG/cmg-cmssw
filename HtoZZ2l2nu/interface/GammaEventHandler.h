/* 
 * Wrapper for common operations on a gamma event
 * Get weights/mass shapes from file
 * Analyze event and assign trigger categories, weights and massive candidates
 * $Date: 2011/11/02 15:32:02 $
 * $Revision: 1.1 $
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
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

class GammaEventHandler
{
 public: 

  enum WeightMode {PT,PTANDETA,PTANDNVTX};
  
  GammaEventHandler(const edm::ParameterSet &runProcess);

  bool isGood(PhysicsEvent_t &phys);
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
  
  int findTriggerCategoryFor(float pt);

  ~GammaEventHandler();
  
 private:

  int weightMode_;

  std::vector<int> gammaCats_;

  TFile *fwgt_;
  std::map<TString,TH1 *> wgtsH_;
  std::map<TString, TH1 *> zmassH_;  

  bool isGoodEvent_;
  int triggerThr_;
  TString photonCategory_;
  std::map<TString,LorentzVector> massiveGamma_;
  std::map<TString,float> evWeights_;
  
  EventCategory eventClassifComp_;
};

#endif
