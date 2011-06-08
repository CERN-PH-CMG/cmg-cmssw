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
      rawFactor_(1.),
      uncOnFourVectorScale_(0.)
        {}

    virtual ~BaseJet(){}

    cmg::TriBool jetId() const{return jetId_;}

    /// \return btag discriminator
    Float_t btag() const{ return btag_; }

    /// \return a correction factor that can be applied to the jet energy or pT to bring
    /// it back to the uncorrected value
    Float_t rawFactor() const {return rawFactor_;}

    Float_t uncOnFourVectorScale() const {return uncOnFourVectorScale_;}

    friend class BaseJetFactory;

  private:

    /// Is the jet ID'ed?
    cmg::TriBool jetId_;

    /// b-Tagging discriminator.
    Float_t btag_;

    /// Correction factor that can be applied to the jet energy or pT
    /// to bring it back to the uncorrected value.
    Float_t rawFactor_;

    /// The uncertainty on the four-vector scale. This can be used to
    /// scale up/down the four-vector according to the scale
    /// uncertainty by multiplying by (1. +/- uncOnFourVectorScale_).
    Float_t uncOnFourVectorScale_;

  };
}

#endif /*JET_H_*/
