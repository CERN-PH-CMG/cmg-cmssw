/* 
 * Wrapper for common operations on a gamma event
 * Get weights/mass shapes from file
 * Analyze event and assign trigger categories, weights and massive candidates
 * $Date: 2012/04/05 00:26:45 $
 * $Revision: 1.6 $
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
  bool isGood() { return isGoodEvent_; }
  LorentzVector massiveGamma(TString channel) { return massiveGamma_[channel]; }
  std::map<TString,LorentzVector> getMassiveGamma() { return massiveGamma_; }
  float getWeight(TString channel) { return evWeights_[channel]; }
  std::map<TString,float> getWeights(PhysicsEvent_t &phys, TString evCategoryLabel="");

  ~GammaEventHandler();


  //these can be accessed after call isGood(physics)
  bool isGoodEvent_;
  std::map<TString,LorentzVector> massiveGamma_;
  std::map<TString,float> evWeights_;
  
 private:

  std::map<TString, std::map<TString,TH1 *> > wgtsH_;
  std::map<TString, TH1 *> zmassH_;  
};

#endif
