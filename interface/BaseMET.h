#ifndef _AnalysisDataFormats_CMGTools_BaseMET_h_
#define _AnalysisDataFormats_CMGTools_BaseMET_h_

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include <vector>

namespace cmg{

  class BaseMET : public PhysicsObject {
  public:
    BaseMET(const math::PtEtaPhiMLorentzVector& p4, double sumEt ):
      PhysicsObject::PhysicsObject( p4 ), sumEt_(sumEt) {}

    BaseMET() : sumEt_(-1) {}

    double sumEt() const {return sumEt_;}
		
  private:
    double sumEt_;
	
  };

}

#endif 
