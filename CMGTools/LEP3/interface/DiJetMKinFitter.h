#ifndef __CMGTools_LEP3_DiJetMKinFitter__
#define __CMGTools_LEP3_DiJetMKinFitter__

#include "PhysicsTools/KinFitter/interface/TKinFitter.h"
#include "DataFormats/Candidate/interface/Candidate.h"


class DiJetMKinFitter : public TKinFitter {

 public: 
  typedef reco::Candidate::LorentzVector LorentzVector; 
  DiJetMKinFitter() {}
  DiJetMKinFitter(const char* name, const char* title, double mass = 91.1876);
  
  std::pair<TLorentzVector,TLorentzVector> fit( LorentzVector jet1, LorentzVector jet2 );
  
  double getChi2();
  
  double ErrEt(double Et, double Eta); 
  double ErrEta(double Et, double Eta); 
  double ErrPhi(double Et, double Eta); 
  
  ClassDef(DiJetMKinFitter, 1);
  
  
private:
  
  double mass_;
  TString name_;
  TString title_;
  
  };

#endif 


