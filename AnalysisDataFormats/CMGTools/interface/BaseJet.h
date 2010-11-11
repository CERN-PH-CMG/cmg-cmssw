#ifndef _AnalysisDataFormats_CMGTools_BaseJet_H_
#define _AnalysisDataFormats_CMGTools_BaseJet_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

#include <vector>

namespace cmg {

  //forward def needed
  class BaseJet;

  class BaseJet : public PhysicsObjectWithPtr< pat::JetPtr > {
  public:
    
    BaseJet(){}
    BaseJet(const value& m):
        PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(m),
        btag_(UnSet(double)){
    }
    virtual ~BaseJet(){}
    
    cmg::TriBool jetId() const{ return jetId_; }
    
    double btag() const{ return btag_; }
    
    friend class BaseJetFactory;
    
  private:
 
    cmg::TriBool jetId_;
    double btag_;
    
  };
}

#endif /*JET_H_*/
