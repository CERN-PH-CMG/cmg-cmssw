#include "AnalysisDataFormats/CMGTools/interface/Candidate.h"
#include "FWCore/Utilities/interface/EDMException.h"
using namespace cmg;

Candidate::~Candidate() { }

Candidate::const_iterator Candidate::begin() const { 
  return const_iterator( new const_iterator_imp_specific ); 
}

Candidate::const_iterator Candidate::end() const { 
  return  const_iterator( new const_iterator_imp_specific ); 
}

Candidate::iterator Candidate::begin() { 
  return iterator( new iterator_imp_specific ); 
}

Candidate::iterator Candidate::end() { 
  return iterator( new iterator_imp_specific ); 
}

const reco::CandidateBaseRef & Candidate::masterClone() const {
  throw cms::Exception("Invalid Reference")
    << "this Candidate has no master clone reference."
    << "Can't call masterClone() method.\n";
}

bool Candidate::hasMasterClone() const {
  return false;
}

bool Candidate::hasMasterClonePtr() const {
  return false;
}


const reco::CandidatePtr & Candidate::masterClonePtr() const {
  throw cms::Exception("Invalid Reference")
    << "this Candidate has no master clone ptr."
    << "Can't call masterClonePtr() method.\n";
}

size_t Candidate::numberOfDaughters() const { 
  return 0; 
}

size_t Candidate::numberOfMothers() const { 
  return 0; 
}

bool Candidate::overlap( const reco::Candidate & o ) const { 
  return  p4() == o.p4() && vertex() == o.vertex() && charge() == o.charge();
//  return  p4() == o.p4() && charge() == o.charge();
}

const Candidate * Candidate::daughter( size_type ) const {
  return 0;
}

const Candidate * Candidate::mother( size_type ) const {
  return 0;
}

const Candidate * Candidate::daughter(const std::string&) const {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "This Candidate type does not implement daughter(std::string). "
    << "Please use CompositeCandidate or NamedCompositeCandidate.\n";
}

Candidate * Candidate::daughter(const std::string&) {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "This Candidate type does not implement daughter(std::string). "
    << "Please use CompositeCandidate or NamedCompositeCandidate.\n";
}



Candidate * Candidate::daughter( size_type ) {
  return 0;
}

double Candidate::vertexChi2() const {
  return 0;
}

double Candidate::vertexNdof() const {
  return 0;
}

double Candidate::vertexNormalizedChi2() const {
  return 0;
}

double Candidate::vertexCovariance(int i, int j) const {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "reco::ConcreteCandidate does not implement vertex covariant matrix.\n";
}

void Candidate::fillVertexCovariance(CovarianceMatrix & err) const {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "reco::ConcreteCandidate does not implement vertex covariant matrix.\n";
}

bool Candidate::isElectron() const { return false; }

bool Candidate::isMuon() const { return false; }

bool Candidate::isGlobalMuon() const { return false; }

bool Candidate::isStandAloneMuon() const { return false; }

bool Candidate::isTrackerMuon() const { return false; }

bool Candidate::isCaloMuon() const { return false; }

bool Candidate::isPhoton() const { return false; }

bool Candidate::isConvertedPhoton() const { return false; }

bool Candidate::isJet() const { return false; }

bool Candidate::longLived() const {return false;}

bool Candidate::massConstraint() const {return false;}




