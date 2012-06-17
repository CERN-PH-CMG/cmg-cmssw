/* 
 * Wrapper for common operations on a gamma event
 * Get weights/mass shapes from file
 * Analyze event and assign trigger categories, weights and massive candidates
 * $Date: 2012/06/12 15:12:37 $
 * $Revision: 1.7 $
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

  GammaEventHandler(const edm::ParameterSet &runProcess);

  //to be called event by event
  bool isGood(PhysicsEvent_t &phys);

  //getters
  float triggerThr() { return triggerThr_; }
  bool isGood() { return isGoodEvent_; }
  LorentzVector massiveGamma(TString channel) { return massiveGamma_[channel]; }
  std::map<TString,LorentzVector> getMassiveGamma() { return massiveGamma_; }
  float getWeight(TString channel) { return evWeights_[channel]; }
  std::map<TString,float> getWeights(PhysicsEvent_t &phys, TString evCategoryLabel="");

  ~GammaEventHandler();


  //these can be accessed after call isGood(physics)
  bool isGoodEvent_;
  float triggerThr_;
  std::map<TString,LorentzVector> massiveGamma_;
  std::map<TString,float> evWeights_;
  
 private:

  bool isMC_;
  std::map<TString, std::map<TString,TH1 *> > wgtsH_;
  std::map<TString, TH1 *> zmassH_;  
};

#endif
