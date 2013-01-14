/* 
 * Wrapper for common operations on a gamma event
 * Get weights/mass shapes from file
 * Analyze event and assign trigger categories, weights and massive candidates
 * $Date: 2012/10/23 19:58:59 $
 * $Revision: 1.10 $
 * \author Pedro Silva
 */

#ifndef GammaEventHandler_H
#define GammaEventHandler_H

#include "TString.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

struct BosonPlusJets_t
{
  Float_t en,qt,eta, weight, mindrbj, minmbj,njets,nvtx,cat;
};


class GammaEventHandler
{
 public: 

  GammaEventHandler(const edm::ParameterSet &runProcess);

  TTree *initSummary()
    {
      t_=new TTree("data","boson+jets summary");
      t_->Branch("cat",     &evSummary_.cat,      "cat/F");
      t_->Branch("qt",      &evSummary_.qt,       "qt/F");
      t_->Branch("eta",      &evSummary_.eta,       "eta/F");
      t_->Branch("en",      &evSummary_.en,       "en/F");
      t_->Branch("weight",  &evSummary_.weight,   "weight/F");
      t_->Branch("mindrbj", &evSummary_.mindrbj,  "mindrbj/F");
      t_->Branch("minmbj",  &evSummary_.minmbj,   "minmbj/F");
      t_->Branch("njets",   &evSummary_.njets,    "njets/F");
      t_->Branch("nvtx",    &evSummary_.nvtx,     "nvtx/F");
      return t_;
    }

  //to be called event by event
  bool isGood(PhysicsEvent_t &phys,bool is2011);

  //getters
  float triggerThr() { return triggerThr_; }
  float triggerWeight() { return triggerWgt_; }
  bool isGood() { return isGoodEvent_; }
  LorentzVector massiveGamma(TString channel) { return massiveGamma_[channel]; }
  std::map<TString,LorentzVector> getMassiveGamma() { return massiveGamma_; }
  float getWeight(TString channel) { return evWeights_[channel]; }
  std::map<TString,float> getWeights(PhysicsEvent_t &phys, TString evCategoryLabel="");

  ~GammaEventHandler();


  //these can be accessed after call isGood(physics)
  bool isGoodEvent_;
  float triggerThr_,triggerWgt_;
  std::map<TString,LorentzVector> massiveGamma_;
  std::map<TString,float> evWeights_;
  
  BosonPlusJets_t evSummary_;

 private:

  TTree *t_;

  bool isMC_;
  std::map<TString, std::map<TString,TGraph *> > wgtsH_;
  //std::map<TString, std::map<TString,TH1 *> > wgtsH_;
  std::map<TString, TH1 *> zmassH_;  
};

#endif
