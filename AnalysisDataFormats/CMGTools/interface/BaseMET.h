#ifndef _AnalysisDataFormats_CMGTools_BaseMET_h_
#define _AnalysisDataFormats_CMGTools_BaseMET_h_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include <vector>

namespace cmg{
    
  class BaseMET : public AbstractPhysicsObject{
  public:
    typedef reco::Candidate::LorentzVector GenMET; 

    BaseMET(const reco::LeafCandidate& cand):
      AbstractPhysicsObject(cand),
      sumEt_(UnSet(double)){
      }

    BaseMET() : sumEt_(-1) {}

    double sumEt() const {return sumEt_;}
    const GenMET& genMET() const {return genMET_;}
    
  private:
    double sumEt_;
    
    GenMET genMET_;

    template <class T> friend class BaseMETFactory;
    friend class BaseMETModificationFactory;
	
  };

}

#endif 
