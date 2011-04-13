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
      btag_(UnSet(double)), 
      rawFactor_(1){}

    virtual ~BaseJet(){}
    
    cmg::TriBool jetId() const{ return jetId_; }
    
    /// \return btag discriminator
    double btag() const{ return btag_; }
    
    /// \return a correction factor that can be applied to the jet energy or pT to bring
    /// it back to the uncorrected value
    double rawFactor() const {return rawFactor_;}

    friend class BaseJetFactory;
    
  private:
 
    /// is the jet ID'ed?
    cmg::TriBool jetId_;

    /// b tagging discriminator
    double btag_;

    /// correction factor that can be applied to the jet energy or pT to bring
    /// it back to the uncorrected value
    float rawFactor_;
  };
}

#endif /*JET_H_*/
