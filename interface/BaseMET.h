#ifndef _AnalysisDataFormats_CMGTools_BaseMET_h_
#define _AnalysisDataFormats_CMGTools_BaseMET_h_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include <vector>

namespace cmg{
    
  class BaseMET : public AbstractPhysicsObject{
  public:
    BaseMET(const reco::LeafCandidate& cand):
      AbstractPhysicsObject(cand),
      sumEt_(UnSet(double)),
      significance_(UnSet(double)){
      }

    BaseMET() : sumEt_(-1) {}

    double sumEt() const {return sumEt_;}
    double significance() const{return significance_;}
		
  private:
    double sumEt_;
    double significance_;
    
    template <class T> friend class BaseMETFactory;
	
  };

}

#endif 
