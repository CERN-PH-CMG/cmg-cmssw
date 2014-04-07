#ifndef __AnalysisDataFormats_CMGTools_SimpleParticle_h__
#define __AnalysisDataFormats_CMGTools_SimpleParticle_h__

#include "DataFormats/Math/interface/LorentzVector.h"

namespace cmg {

  /**
     A simple and light particle class. 
     Can be used to store constituents in cmg physics objects, 
     as is done in cmg::Tau.
     Can also be used to store all PFCandidates in a light format.
  */
  class SimpleParticle : public math::PtEtaPhiMLorentzVector {
  public:
    SimpleParticle() : type_(0) {} 

    SimpleParticle( int type, 
		    float pt, float eta, float phi, float M) 
      : math::PtEtaPhiMLorentzVector(pt, eta, phi, M), type_( type ) {}

    int type() const {return type_;}

    float charge() const;

  private:
    int type_;
  };
}

#endif 
