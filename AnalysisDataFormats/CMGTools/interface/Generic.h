#ifndef ANALYSISDATAFORMATS_CMGTOOLS_GENERIC_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_GENERIC_H_

#include "DataFormats/Candidate/interface/Candidate.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include <vector>

namespace cmg
{

//forward def needed
class Generic;
class GenericFactory;

class Generic : public cmg::AbstractPhysicsObject{
public:
	
  Generic(){
  }
  Generic(const reco::LeafCandidate& cand ):
    AbstractPhysicsObject( cand ),
    charge_(UnSet(int)),
    pdgId_(UnSet(int)){
  }
  virtual ~Generic(){
  }
    
  int charge() const{
    return charge_;
  }
  
  int pdgId() const{
    return pdgId_;
  }
  
  friend class cmg::GenericFactory;
	
 private:
  
    int charge_;
    int pdgId_;
    
};

}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_GENERIC_H_*/
