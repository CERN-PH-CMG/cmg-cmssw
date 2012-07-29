#ifndef __CMGTools_LEP3_FourJetEpKinFitter__
#define __CMGTools_LEP3_FourJetEpKinFitter__

#include "PhysicsTools/KinFitter/interface/TKinFitter.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <vector>

class FourJetEpKinFitter : public TKinFitter {

 public: 
  typedef reco::Candidate::LorentzVector LorentzVector; 
  FourJetEpKinFitter() {}
  FourJetEpKinFitter(const char*  name, const char* title, double energy = 240);

  std::vector<TLorentzVector> fit(  LorentzVector jet1, 
				    LorentzVector jet2,
				    LorentzVector jet3,
				    LorentzVector jet4 );

  double getChi2();

  double ErrEt(double Et, double Eta); 
  double ErrEta(double Et, double Eta); 
  double ErrPhi(double Et, double Eta); 
  
  ClassDef(FourJetEpKinFitter, 1);

 private:
  
  double energy_;
  TString name_;
  TString title_;  


};

#endif 

