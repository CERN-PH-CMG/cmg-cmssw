#ifndef _AnalysisDataFormats_CMGTools_BaseMET_h_
#define _AnalysisDataFormats_CMGTools_BaseMET_h_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include <vector>

namespace cmg{

  class BaseMET : public AbstractPhysicsObject{
  public:
    BaseMET(const reco::LeafCandidate& cand, double sumEt ):
      AbstractPhysicsObject(cand),
      sumEt_(sumEt) {}

    BaseMET() : sumEt_(-1) {}

    double sumEt() const {return sumEt_;}
		
  private:
    double sumEt_;
	
  };

}

#endif 
