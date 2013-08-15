#ifndef __CMGTools_LEP3_FourJetEpMKinFitter__
#define __CMGTools_LEP3_FourJetEpMKinFitter__

#include "PhysicsTools/KinFitter/interface/TKinFitter.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <vector>

class FourJetEpMKinFitter : public TKinFitter {

 public: 
  typedef reco::Candidate::LorentzVector LorentzVector; 
  FourJetEpMKinFitter() {}
  FourJetEpMKinFitter(const char*  name, const char* title, double energy = 240, double mass = 91.2);

  std::vector<TLorentzVector> fit(  LorentzVector jet1, 
				    LorentzVector jet2,
				    LorentzVector jet3,
				    LorentzVector jet4, int njet1 = 0, int njet2 = 1 );

  double getChi2();
  int getStatus();

  double ErrEt(double Et, double Eta); 
  double ErrEta(double Et, double Eta); 
  double ErrPhi(double Et, double Eta); 
  
  ClassDef(FourJetEpMKinFitter, 1);

 private:
  
  double energy_;
  double mass_;
  TString name_;
  TString title_;  


};

#endif 

