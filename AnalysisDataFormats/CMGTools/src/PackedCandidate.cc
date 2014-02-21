#include "AnalysisDataFormats/CMGTools/interface/PackedCandidate.h"
#include "AnalysisDataFormats/CMGTools/interface/libminifloat.h"

void cmg::PackedCandidate::pack() {
    packedPt_  =  MiniFloatConverter::float32to16(p4_.Pt());
    packedEta_ =  int16_t(p4_.Eta()/6.0f*std::numeric_limits<int16_t>::max());
    packedPhi_ =  int16_t(p4_.Phi()/3.2f*std::numeric_limits<int16_t>::max());
    packedM_   =  MiniFloatConverter::float32to16(p4_.M());
    packedX_   =  int16_t(vertex_.X()/5.0f*std::numeric_limits<int16_t>::max());
    packedY_   =  int16_t(vertex_.Y()/5.0f*std::numeric_limits<int16_t>::max());
    packedZ_   =  int16_t(vertex_.Z()/40.f*std::numeric_limits<int16_t>::max());
    unpack(); // force the values to match with the packed ones
}

void cmg::PackedCandidate::unpack() const {
    p4_ = PolarLorentzVector(MiniFloatConverter::float16to32(packedPt_),
                             int16_t(packedEta_)*6.0f/std::numeric_limits<int16_t>::max(),
                             int16_t(packedPhi_)*3.2f/std::numeric_limits<int16_t>::max(),
                             MiniFloatConverter::float16to32(packedM_));
    p4c_ = p4_;
    vertex_ = Point(int16_t(packedX_)*5.0f/std::numeric_limits<int16_t>::max(),
                    int16_t(packedY_)*5.0f/std::numeric_limits<int16_t>::max(),
                    int16_t(packedZ_)*40.f/std::numeric_limits<int16_t>::max());
    unpacked_ = true;
}

cmg::PackedCandidate::~PackedCandidate() { }

cmg::PackedCandidate::const_iterator cmg::PackedCandidate::begin() const { 
  return const_iterator( new const_iterator_imp_specific ); 
}

cmg::PackedCandidate::const_iterator cmg::PackedCandidate::end() const { 
  return  const_iterator( new const_iterator_imp_specific ); 
}

cmg::PackedCandidate::iterator cmg::PackedCandidate::begin() { 
  return iterator( new iterator_imp_specific ); 
}

cmg::PackedCandidate::iterator cmg::PackedCandidate::end() { 
  return iterator( new iterator_imp_specific ); 
}

const reco::CandidateBaseRef & cmg::PackedCandidate::masterClone() const {
  throw cms::Exception("Invalid Reference")
    << "this Candidate has no master clone reference."
    << "Can't call masterClone() method.\n";
}

bool cmg::PackedCandidate::hasMasterClone() const {
  return false;
}

bool cmg::PackedCandidate::hasMasterClonePtr() const {
  return false;
}


const reco::CandidatePtr & cmg::PackedCandidate::masterClonePtr() const {
  throw cms::Exception("Invalid Reference")
    << "this Candidate has no master clone ptr."
    << "Can't call masterClonePtr() method.\n";
}

size_t cmg::PackedCandidate::numberOfDaughters() const { 
  return 0; 
}

size_t cmg::PackedCandidate::numberOfMothers() const { 
  return 0; 
}

bool cmg::PackedCandidate::overlap( const reco::Candidate & o ) const { 
  return  p4() == o.p4() && vertex() == o.vertex() && charge() == o.charge();
//  return  p4() == o.p4() && charge() == o.charge();
}

const reco::Candidate * cmg::PackedCandidate::daughter( size_type ) const {
  return 0;
}

const reco::Candidate * cmg::PackedCandidate::mother( size_type ) const {
  return 0;
}

const reco::Candidate * cmg::PackedCandidate::daughter(const std::string&) const {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "This Candidate type does not implement daughter(std::string). "
    << "Please use CompositeCandidate or NamedCompositeCandidate.\n";
}

reco::Candidate * cmg::PackedCandidate::daughter(const std::string&) {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "This Candidate type does not implement daughter(std::string). "
    << "Please use CompositeCandidate or NamedCompositeCandidate.\n";
}



reco::Candidate * cmg::PackedCandidate::daughter( size_type ) {
  return 0;
}

double cmg::PackedCandidate::vertexChi2() const {
  return 0;
}

double cmg::PackedCandidate::vertexNdof() const {
  return 0;
}

double cmg::PackedCandidate::vertexNormalizedChi2() const {
  return 0;
}

double cmg::PackedCandidate::vertexCovariance(int i, int j) const {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "reco::ConcreteCandidate does not implement vertex covariant matrix.\n";
}

void cmg::PackedCandidate::fillVertexCovariance(CovarianceMatrix & err) const {
  throw edm::Exception(edm::errors::UnimplementedFeature)
    << "reco::ConcreteCandidate does not implement vertex covariant matrix.\n";
}

bool cmg::PackedCandidate::isElectron() const { return false; }

bool cmg::PackedCandidate::isMuon() const { return false; }

bool cmg::PackedCandidate::isGlobalMuon() const { return false; }

bool cmg::PackedCandidate::isStandAloneMuon() const { return false; }

bool cmg::PackedCandidate::isTrackerMuon() const { return false; }

bool cmg::PackedCandidate::isCaloMuon() const { return false; }

bool cmg::PackedCandidate::isPhoton() const { return false; }

bool cmg::PackedCandidate::isConvertedPhoton() const { return false; }

bool cmg::PackedCandidate::isJet() const { return false; }

bool cmg::PackedCandidate::longLived() const {return false;}

bool cmg::PackedCandidate::massConstraint() const {return false;}




