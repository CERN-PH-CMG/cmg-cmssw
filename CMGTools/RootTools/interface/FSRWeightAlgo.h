#ifndef __CMGTools_RootTools_FSRWeightAlgo__
#define __CMGTools_RootTools_FSRWeightAlgo__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class FSRWeightAlgo {

 public:
  FSRWeightAlgo() {}
  virtual ~FSRWeightAlgo() {}
  void addGenParticle(const reco::GenParticle& gen) {genParticles_.push_back(gen);}
  void clear() {genParticles_.clear();}
  double weight() const;
  
 private:
  double alphaRatio(double) const;
  
  std::vector< reco::GenParticle > genParticles_;
  };

#endif
