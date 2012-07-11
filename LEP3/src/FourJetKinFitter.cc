#include "CMGTools/LEP3/interface/FourJetKinFitter.h"

#include <TString.h>
#include <iostream>

using namespace std;
typedef reco::Candidate::LorentzVector LorentzVector;

FourJetKinFitter::FourJetKinFitter(const char* name, 
				   const char* title) : 
  TKinFitter( TString(name), TString(title) ) {
  cout<<"FourJetKinFitter: "<<name<<", "<<title<<endl;
}

std::vector<LorentzVector> FourJetKinFitter::fit( LorentzVector jet1, 
						  LorentzVector jet2,
						  LorentzVector jet3,
						  LorentzVector jet4 ) {
  std::vector<LorentzVector> output(4);
  output.push_back(jet1);
  output.push_back(jet2);
  output.push_back(jet3);
  output.push_back(jet4);
  
  // note that you will need to translate all LorentzVector (from Candidate.h)
  // to TLorentzVector to use the fitter

  return output;
}
