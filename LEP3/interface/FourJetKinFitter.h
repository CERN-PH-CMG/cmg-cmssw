#ifndef __CMGTools_LEP3_FourJetKinFitter__
#define __CMGTools_LEP3_FourJetKinFitter__

#include "PhysicsTools/KinFitter/interface/TKinFitter.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <TLorentzVector.h>
#include <vector>

class FourJetKinFitter : public TKinFitter {

 public: 
  typedef reco::Candidate::LorentzVector LorentzVector;

  FourJetKinFitter() {}
  FourJetKinFitter(const char* name, const char* title);

  // here, add your interface as in the DiJetKinFitter
  std::vector<LorentzVector> fit( LorentzVector jet1, 
				  LorentzVector jet2,
				  LorentzVector jet3,
				  LorentzVector jet4 );

  ClassDef(FourJetKinFitter, 1);
};

#endif 
