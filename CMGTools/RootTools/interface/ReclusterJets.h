#ifndef ReclusterJets_h
#define ReclusterJets_h

#include <vector>
#include <iostream>
#include <cmath>
#include <TLorentzVector.h>
#include <TMath.h>
#include "DataFormats/Math/interface/LorentzVector.h"

#include <boost/shared_ptr.hpp>
#include <fastjet/internal/base.hh>
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "fastjet/PseudoJet.hh"


class ReclusterJets {
    
 public:
  typedef math::XYZTLorentzVector LorentzVector;

  ReclusterJets(const std::vector<LorentzVector> & objects, double ktpower, double rparam);

  /// get grouping (inclusive jets)
  std::vector<LorentzVector> getGrouping();

 private:

  int Reconstruct();

  // used to handle the inputs
  std::vector<fastjet::PseudoJet> fjInputs_;        // fastjet inputs

  double ktpower_;
  double rparam_;

  /// fastjet outputs
  std::vector<fastjet::PseudoJet> inclusiveJets_;          // fastjet jets
  typedef boost::shared_ptr<fastjet::ClusterSequence>  ClusterSequencePtr;
  ClusterSequencePtr fjClusterSeq_;    

  /// friendly outputs
  std::vector<LorentzVector> JetObjectsAll_;
};

//FASTJET_END_NAMESPACE
 
#endif    
